/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "glx_surface.h"

#include <cppual/gfx/gl/gldef.h>

#include <cppual/ui/platforms/xcb/xcb_def.h>

#include <cstring>
#include <iostream>
#include <algorithm>

#include <X11/Xlib.h>
#include <xcb/xcb.h>
#include <GL/glx.h>

namespace cppual { namespace gfx { namespace gl {

namespace { namespace internal { // optimize for internal unit usage

typedef u32                             value_type      ;
typedef Bool                            bool_type       ;
typedef ::xcb_connection_t*             conn_pointer    ;
typedef ::Display*                      display_pointer ;
typedef ::GLXFBConfig                   fbconfig_pointer;
typedef ::GLXFBConfigID                 fbconfig_id     ;
typedef ::XID                           surface_id      ;
typedef ::GLXContext                    context_pointer ;
typedef ::GLXPbuffer                    pbuffer_id      ;
typedef ::GLXPixmap                     pixmap_id       ;
typedef context_interface::version_type version_type    ;

enum
{
    Width               = GLX_WIDTH,
    Height              = GLX_HEIGHT,
    Extensions          = GLX_EXTENSIONS,
    ContextVersion      = GLX_VERSION,
    ContextProfile      = 0,
    RgbaType            = GLX_RGBA_TYPE,
    BadContext          = GLX_BAD_CONTEXT,
    BadVALUE            = GLX_BAD_VALUE,
    BadEnum             = GLX_BAD_ENUM,
    BadScreen           = GLX_BAD_SCREEN,
    BadVisual           = GLX_BAD_VISUAL,
    BadAttribute        = GLX_BAD_ATTRIBUTE,
    none                = GLX_NONE,
    TRUE                = 1,
    FALSE               = 0
};

enum class error_type : u8
{
    initialize,
    terminate,
    make_current,
    create,
    destroy
};

inline version_type& version () noexcept
{
    static version_type ver { 0, 0 };
    return ver;
}

// ====================================================

inline bool_type initialize (display_pointer dsp)
{
    return ::glXQueryVersion (dsp, &internal::version ().major, &internal::version ().minor);
}

// ====================================================

inline surface_id create_drawable (config const&                   gConf,
                                   point2u                         /*gSize*/,
                                   resource_interface::handle_type uWndHandle,
                                   bool                            /*bDouble*/)
{
    //value_type nSurfaceAttribs[9];

    //nSurfaceAttribs[0] = GLX_X_RENDERABLE;
    //nSurfaceAttribs[1] = bDouble ? GLX_BACK_BUFFER : GLX_SINGLE_BUFFER;
    //nSurfaceAttribs[2] = internal::none;

    auto const id = ::glXCreateWindow (gConf.legacy ().get<internal::display_pointer> (),
                                       gConf.handle ().get<internal::fbconfig_pointer> (),
                                       uWndHandle,
                                       nullptr); // attribs array pointer

    return id;
}

inline pbuffer_id create_pbuffer (config const& gConf, point2u /*gSize*/)
{
    auto const id = ::glXCreatePbuffer (gConf.legacy ().get<internal::display_pointer> (),
                                        gConf.handle ().get<internal::fbconfig_pointer> (),
                                        nullptr);

    return id;
}

inline pixmap_id create_pixmap (config const& gConf)
{
    auto const id = ::glXCreatePixmap (gConf.legacy ().get<internal::display_pointer> (),
                                       gConf.handle ().get<internal::fbconfig_pointer> (),
                                       0U, // pixmap
                                       nullptr);

    return id;
}

inline surface_id create_surface (config const&        gConf,
                                  point2u              size,
                                  surface_type         type,
                                  surface::handle_type owner)
{
    switch (type)
    {
    case surface_type::back_buffer:
        return internal::create_pbuffer (gConf, size);
    case surface_type::pixmap:
        return internal::create_pixmap (gConf);
    default:
        return internal::create_drawable (gConf, size, owner, type == surface_type::double_buffer);
    }
}

inline context_pointer create_gc (config const& gConf, version_type const& /*version*/)
{
    return ::glXCreateNewContext (gConf.legacy ().get<display_pointer> (),
                                  gConf.handle ().get<fbconfig_pointer> (),
                                  RgbaType,
                                  context_pointer (),
                                  TRUE);
}

inline point2u get_size (config const& config, surface::handle_type surface) noexcept
{
    value_type size[2];

    ::glXQueryDrawable (config.legacy ().get<display_pointer> (), surface, Width, &size[0]);
    ::glXQueryDrawable (config.legacy ().get<display_pointer> (), surface, Height, &size[1]);

    return { static_cast<u16> (size[0]), static_cast<u16> (size[1]) };
}

// ====================================================

inline config::feature_types convert_extensions (display_pointer dsp)
{
    config::feature_types eFeatures;

    auto extensions = ::glXQueryExtensionsString (dsp, 0);

    std::cout << extensions << std::endl;

    for (auto extension = extensions; extension++; extension = std::strchr (extension , ' '))
    {
        switch (constexpr_char_hash (extension))
        {
        case constexpr_char_hash ("GLX_EXT_swap_control"):
            eFeatures += config::feature::sync_control;
            break;
        case constexpr_char_hash ("GLX_MESA_swap_control"):
            eFeatures += config::feature::sync_control;
            eFeatures += config::feature::mesa_sync_control;
            break;
        }
    }

    return eFeatures;
}

} } // anonymous namespace internal

// ====================================================

config::config (connection_type native, connection_type legacy, format_type gFormat)
: _M_pNative   (native),
  _M_pLegacy   (legacy),
  _M_pCfg      (),
  _M_gFormat   (gFormat),
  _M_eFeatures ()
{
    if (!_M_pLegacy) throw std::logic_error ("invalid display");

    static auto init = internal::initialize (_M_pLegacy.get<internal::display_pointer> ());

    auto const screens = x::screen_setup (native.get<internal::conn_pointer> ());

    UNUSED (init);

    if (!screens.count ()) throw std::logic_error ("NO screens available!");

    int num_fb_configs = 0;
    internal::fbconfig_pointer* const fb_configs = ::glXGetFBConfigs (legacy.get<internal::display_pointer> (),
                                                                      0,
                                                                      &num_fb_configs);

    if (!fb_configs || !num_fb_configs)
    {
        throw std::logic_error ("NO fb configs available!");
    }

    internal::fbconfig_pointer fb_config;
    auto prevVisualID = value_type (), visualID = prevVisualID;

    for (int i = 0; !visualID && i < num_fb_configs; ++i)
    {
        /* Select first framebuffer config with a valid visual and query visualID */
        fb_config = fb_configs[i];

        ::XVisualInfo* const visual = ::glXGetVisualFromFBConfig (legacy.get<internal::display_pointer> (),
                                                                  fb_config);

        if (!visual) continue;

        if (visual->visualid)
        {
            auto const val_format = to_format (fb_config);

            if (_M_gFormat == val_format)
            {
                visualID = visual->visualid;
                break;
            }

            prevVisualID = visual->visualid;
        }
    }

    if (!visualID && !prevVisualID)
    {
        throw std::logic_error ("NO visualid available!");
    }

    _M_gFormat = to_format (fb_config);
    _M_pCfg    = fb_config;
}

config::value_type config::id () const
{
    int cfg_id = 0;

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_FBCONFIG_ID,
                            &cfg_id);

    return static_cast<value_type> (cfg_id);
}

pixel_format config::to_format (handle_type fb_cfg) const
{
    if (!_M_pCfg) return pixel_format ();

    int nAttribs[attrib_count];

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_RED_SIZE,
                            &nAttribs[red]);

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_GREEN_SIZE,
                            &nAttribs[green]);

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_BLUE_SIZE,
                            &nAttribs[blue]);

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_ALPHA_SIZE,
                            &nAttribs[alpha]);

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_DEPTH_SIZE,
                            &nAttribs[depth]);

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_STENCIL_SIZE,
                            &nAttribs[stencil]);

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_DRAWABLE_TYPE,
                            &nAttribs[surface_type]);

    ::glXGetFBConfigAttrib (legacy ().get<internal::display_pointer> (),
                            fb_cfg.get<internal::fbconfig_pointer> (),
                            GLX_X_VISUAL_TYPE,
                            &nAttribs[color_type]);

    return
    {
        (nAttribs[surface_type] & GLX_WINDOW_BIT) ? pixel_flag::drawable : 0 |
                (nAttribs[surface_type] & GLX_PBUFFER_BIT) ? pixel_flag::bitmap : 0,
        static_cast<u8> (nAttribs[red]),
        static_cast<u8> (nAttribs[green]),
        static_cast<u8> (nAttribs[blue]),
        static_cast<u8> (nAttribs[alpha]),
        static_cast<u8> (nAttribs[depth]),
        static_cast<u8> (nAttribs[stencil]),
        static_cast<gfx::color_type> (nAttribs[color_type] & GLX_TRUE_COLOR ?
                                          gfx::color_type::true_type :
                                          nAttribs[color_type] & GLX_DIRECT_COLOR ?
                                              gfx::color_type::direct :
                                              nAttribs[color_type] & GLX_PSEUDO_COLOR ?
                                                  gfx::color_type::pseudo :
                                                  nAttribs[color_type] & GLX_STATIC_COLOR ?
                                                      gfx::color_type::static_type :
                                                      nAttribs[color_type] & GLX_GRAY_SCALE ?
                                                          gfx::color_type::grey :
                                                          nAttribs[color_type] & GLX_STATIC_GRAY ?
                                                              gfx::color_type::static_grey :
                                                              gfx::color_type::none)
    };
}

void config::print () const
{
    static int value;

    std::cout << "Buffer Size: "  << static_cast<u16> (_M_gFormat.depth + _M_gFormat.alpha)
              << "\nRed Size: "   << static_cast<u16> (_M_gFormat.red)
              << "\nGreen Size: " << static_cast<u16> (_M_gFormat.green)
              << "\nBlue Size: "  << static_cast<u16> (_M_gFormat.blue)
              << "\nAlpha Size: " << static_cast<u16> (_M_gFormat.alpha)
              << "\nDepth size: " << static_cast<u16> (_M_gFormat.depth);

    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                           _M_pCfg.get<internal::fbconfig_pointer> (), GLX_CONFIG_CAVEAT, &value);

    switch (value)
    {
    case internal::none:
        std::cout << "\nGLX_CONFIG_CAVEAT: GLX_NONE";
        break;
    case GLX_SLOW_CONFIG:
        std::cout << "\nGLX_CONFIG_CAVEAT: GLX_SLOW_CONFIG";
        break;
    }

    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (), GLX_FBCONFIG_ID, &value);
    std::cout << "\nConfig ID: " << value;

    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_MAX_PBUFFER_WIDTH, &value);
    std::cout << "\nMax pbuffer width: " << value;
    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_MAX_PBUFFER_HEIGHT, &value);
    std::cout << "\nMax pbuffer height: " << value;
    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_MAX_PBUFFER_PIXELS, &value);
    std::cout << "\nMax pbuffer pixels: " << value;
    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_VISUAL_ID, &value);
    std::cout << "\nNative visual ID: " << value;
    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_SAMPLE_BUFFERS, &value);
    std::cout << "\nSample Buffers: " << value;
    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_SAMPLES, &value);
    std::cout << "\nSamples: " << value;
    ::glXGetFBConfigAttrib (_M_pLegacy.get<internal::display_pointer> (),
                            _M_pCfg.get<internal::fbconfig_pointer> (),
                            GLX_TRANSPARENT_TYPE, &value);

    switch (value)
    {
    case GLX_NONE:
        std::cout << "\nTransparent type: GLX_NONE" << std::endl;
        break;
    case  GLX_TRANSPARENT_RGB:
        std::cout << "\nTransparent type:  GLX_TRANSPARENT_RGB" << std::endl;
        break;
    case  GLX_TRANSPARENT_INDEX:
        std::cout << "\nTransparent type:  GLX_TRANSPARENT_INDEX" << std::endl;
        break;
    default:
        std::cout << "\nTransparent type: " << value << std::endl;
        break;
    }
}

// ====================================================

surface::surface (conf_reference gConf,
                  point2u size,
                  handle_type wnd,
                  surface_type type)
: _M_pConf   (gConf),
  _M_pHandle (internal::create_surface (gConf, size, type, wnd)),
  _M_pWnd    (_M_pHandle ? wnd : handle_type ()),
  _M_eType   (type)
{
    if (type == surface_type::double_buffer)
    {
        auto eFeatures = internal::convert_extensions (_M_pConf.legacy ().get<internal::display_pointer> ());

        if (eFeatures.test (config::feature::sync_control))
        {
            auto glXSwapIntervalEXT =
                reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>
                (glXGetProcAddress((GLubyte*)"glXSwapIntervalEXT"));

            glXSwapIntervalEXT (_M_pConf.legacy ().get<internal::display_pointer> (),
                                _M_pHandle.get<internal::surface_id> (),
                                1);
        }
        else if (eFeatures.test (config::feature::mesa_sync_control))
        {
            auto glXSwapIntervalMESA =
                reinterpret_cast<PFNGLXSWAPINTERVALMESAPROC>
                (glXGetProcAddress((GLubyte*)"glXSwapIntervalMESA"));

            glXSwapIntervalMESA (1);
        }
    }
}

surface::surface (surface const& obj)
: _M_pConf   (obj._M_pConf),
  _M_pHandle (internal::create_surface (_M_pConf, obj.size (), obj._M_eType, obj._M_pWnd)),
  _M_pWnd    (_M_pHandle ? obj._M_pWnd : nullptr),
  _M_eType   (obj._M_eType)
{ }

surface& surface::operator = (surface&& obj) noexcept
{
    if (this == &obj) return *this;

    if (_M_pHandle)
    {
        switch (type ())
        {
        case surface_type::drawable:
        case surface_type::double_buffer:
            ::glXDestroyWindow (_M_pConf.legacy ().get<internal::display_pointer> (),
                                _M_pHandle.get<internal::surface_id> ());
            break;
        case surface_type::back_buffer:
            ::glXDestroyPbuffer (_M_pConf.legacy ().get<internal::display_pointer> (),
                                 _M_pHandle.get<internal::pbuffer_id> ());
            break;
        case surface_type::pixmap:
            ::glXDestroyPixmap (_M_pConf.legacy ().get<internal::display_pointer> (),
                                _M_pHandle.get<internal::pixmap_id> ());
            break;
        }
    }

    _M_pConf       = obj._M_pConf  ;
    _M_pHandle     = obj._M_pHandle;
    _M_pWnd        = obj._M_pWnd   ;

    obj._M_pConf   = conf_value ();
    obj._M_pHandle = nullptr;
    obj._M_pWnd    = nullptr;

    return *this;
}

surface::~surface ()
{
    if (!_M_pHandle) return;

    switch (surface::type ())
    {
    case surface_type::drawable:
    case surface_type::double_buffer:
        ::glXDestroyWindow (_M_pConf.legacy ().get<internal::display_pointer> (),
                            _M_pHandle.get<internal::surface_id> ());
        break;
    case surface_type::back_buffer:
        ::glXDestroyPbuffer (_M_pConf.legacy ().get<internal::display_pointer> (),
                             _M_pHandle.get<internal::pbuffer_id> ());
        break;
    case surface_type::pixmap:
        ::glXDestroyPixmap (_M_pConf.legacy ().get<internal::display_pointer> (),
                            _M_pHandle.get<internal::pixmap_id> ());
        break;
    }
}

void surface::flush ()
{
    if (_M_eType == surface_type::double_buffer)
        ::glXSwapBuffers (_M_pConf.legacy ().get<internal::display_pointer> (),
                          _M_pHandle.get<internal::surface_id> ());
    else
        ::glFlush ();
}

void surface::paint_background (color clr)
{
    ::glClearColor (static_cast<float> (clr.red   ()) / 255.0f,
                    static_cast<float> (clr.green ()) / 255.0f,
                    static_cast<float> (clr.blue  ()) / 255.0f,
                    1.0f);

    ::glClear      (GL_COLOR_BUFFER_BIT);
}

point2u surface::size () const noexcept
{
    return internal::get_size (configuration (), _M_pHandle);
}

/// TODO: finish glx resizing
void surface::scale (point2u gSize)
{
    if (context_interface::current ())
    {
        if (context_interface::current ()->drawable ().get () == this &&
            context_interface::current ()->readable ().get () == this)
        {
            ::glMatrixMode (GL_PROJECTION);
            ::glPushMatrix ();
            ::glLoadIdentity ();

            ::glViewport (0, 0, gSize.x, gSize.y);

            ::glMatrixMode (GL_MODELVIEW);
            ::glPushMatrix ();
            ::glLoadIdentity ();

            if (_M_eType == surface_type::double_buffer)
                ::glXSwapBuffers (_M_pConf.legacy ().get<internal::display_pointer> (),
                                  _M_pHandle.get<internal::surface_id> ());
            else
                ::glFlush ();
        }
    }
}

// ====================================================

context::context (conf_reference conf, version_type const& version)
: _M_pConf        (conf),
  _M_pGC          (internal::create_gc (conf, version)),
  _M_pDrawTarget  (),
  _M_pReadTarget  (),
  _M_nVersion     (_M_pGC ? version : version_type ())
{ }

context::context (context const& obj)
: _M_pConf       (obj._M_pConf),
  _M_pGC         (internal::create_gc (_M_pConf, obj._M_nVersion)),
  _M_pDrawTarget (),
  _M_pReadTarget (),
  _M_nVersion    (_M_pGC ? obj._M_nVersion : version_type ())
{ }

context& context::operator = (context&& obj) noexcept
{
    if (this == &obj) return *this;

    if (_M_pGC)
    {
        if (active ()) release ();

        ::glXDestroyContext (configuration ().legacy ().get<internal::display_pointer> (),
                             _M_pGC.get<internal::context_pointer> ());
    }

    _M_pDrawTarget = obj._M_pDrawTarget;
    _M_pReadTarget = obj._M_pReadTarget;
    _M_nVersion    = obj._M_nVersion;
    _M_pConf       = obj._M_pConf;
    _M_pGC         = obj._M_pGC;

    obj._M_nVersion    = version ();
    obj._M_pGC         = nullptr;
    obj._M_pConf       = conf_value ();
    obj._M_pDrawTarget = nullptr;
    obj._M_pReadTarget = nullptr;

    return *this;
}

context& context::operator = (context const& obj)
{
    if (this == &obj) return *this;

    if (_M_pGC)
    {
        if (active ()) release ();

        ::glXDestroyContext (configuration ().legacy ().get<internal::display_pointer> (),
                             _M_pGC.get<internal::context_pointer> ());
    }

    _M_pGC = obj._M_pGC ? internal::create_gc (obj.configuration (), obj._M_nVersion) :
                          internal::context_pointer ();

    _M_pReadTarget = _M_pDrawTarget = nullptr;
    _M_nVersion    = obj._M_nVersion;
    _M_pConf       = obj._M_pConf   ;

    return *this;
}

context::~context () noexcept
{
    if (!_M_pGC) return;

    acquire (nullptr);

    ::glXDestroyContext (configuration ().legacy ().get<internal::display_pointer> (),
                         _M_pGC.get<internal::context_pointer> ());
}

resource_version context::platform_version () noexcept
{
    return internal::version ();
}

bool context::use (shared_surface pDraw, shared_surface pRead) noexcept
{
    if ((( pDraw and !pRead) or (pDraw and pDraw->device () != device_backend::gl)) or
        ((!pDraw and  pRead) or (pRead and pRead->device () != device_backend::gl)))
        return false;

    _M_pDrawTarget = pDraw;
    _M_pReadTarget = pRead;

    return true;
}

bool context::assign (shared_context cntxt) noexcept
{
    if (cntxt == nullptr) return false;

    auto local_cntxt = static_cast<context*> (cntxt.get ());

    static_cast<surface*> (local_cntxt->_M_pDrawTarget.get ())->_M_pContext = cntxt;
    static_cast<surface*> (local_cntxt->_M_pReadTarget.get ())->_M_pContext = cntxt;

    if (::glXMakeContextCurrent (local_cntxt->configuration ().legacy ().get<internal::display_pointer> (),
                                 local_cntxt->_M_pDrawTarget != nullptr ?
                                    local_cntxt->_M_pDrawTarget->handle ().get<internal::surface_id> () :
                                    internal::surface_id (),
                                 local_cntxt->_M_pReadTarget != nullptr ?
                                    local_cntxt->_M_pReadTarget->handle ().get<internal::surface_id> () :
                                    internal::surface_id (),
                                 local_cntxt->_M_pGC.get<internal::context_pointer> ()))
    {
        return true;
    }

    return false;
}

void context::release () noexcept
{
    if (active ())
    {
        ::glXMakeContextCurrent (configuration ().legacy ().get<internal::display_pointer> (),
                                 internal::surface_id (),
                                 internal::surface_id (),
                                 internal::context_pointer ());

        if (static_cast<surface*> (_M_pDrawTarget.get ())->_M_pContext.get () == this)
            static_cast<surface*> (_M_pDrawTarget.get ())->_M_pContext.reset ();

        if (static_cast<surface*> (_M_pReadTarget.get ())->_M_pContext.get () == this)
            static_cast<surface*> (_M_pReadTarget.get ())->_M_pContext.reset ();
    }
}

void context::flush () noexcept
{
    if (active ()) ::glFlush ();
}

void context::finish () noexcept
{
    if (active ()) ::glXWaitGL ();
}

} } } // namespace gl
