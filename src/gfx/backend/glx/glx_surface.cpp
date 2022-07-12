/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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
    //BadDisplay          = GLX_BAD_DISPLAY,
    BadContext          = GLX_BAD_CONTEXT,
    //BadSurface          = EGL_BAD_SURFACE,
    //BadCurrentSurface   = EGL_BAD_CURRENT_SURFACE,
    //BadNativeWindow     = EGL_BAD_NATIVE_WINDOW,
    //BadConfig           = EGL_BAD_CONFIG,
    //BadAttribute        = EGL_BAD_ATTRIBUTE,
    //NoMatch             = EGL_BAD_MATCH,
    //NoAccess            = EGL_BAD_ACCESS,
    //NoAlloc             = EGL_BAD_ALLOC,
    //NotInitialized      = EGL_NOT_INITIALIZED,
    //ContextLost         = EGL_CONTEXT_LOST,
    none                = GLX_NONE,
    TRUE                = 1,
    FALSE               = 0
};

enum class error_type : byte
{
    initialize,
    terminate,
    make_current,
    create,
    destroy
};

inline version_type& version () noexcept
{
    static version_type glVersion { 3, 0 };
    return glVersion;
}

// ====================================================

inline surface_id create_drawable (config const&                   gConf,
                                   point2u                         /*gSize*/,
                                   resource_interface::handle_type uWndHandle,
                                   bool                            /*bDouble*/)
{
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

    auto const screens = x::screen_setup (native.get<internal::conn_pointer> ());

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
    value_type prevVisualID = 0U, visualID = prevVisualID;

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

            /*std::cout << "\n\nvisual id: " << visual->visualid << '\n'
                      << "pixel_flags: " << static_cast<u16> (val_format.flags) << '\n'
                      << "red: " << static_cast<u16> (val_format.red) << '\n'
                      << "green: " << static_cast<u16> (val_format.green) << '\n'
                      << "blue: " << static_cast<u16> (val_format.blue) << '\n'
                      << "alpha: " << static_cast<u16> (val_format.alpha) << '\n'
                      << "depth: " << static_cast<u16> (val_format.depth) << '\n'
                      << "stencil: " << static_cast<u16> (val_format.stencil) << '\n'
                      << "type: " << static_cast<u16> (val_format.type) << std::endl;*/

            if (_M_gFormat == val_format)
            {
                visualID = visual->visualid;
                break;
            }

            prevVisualID = visual->visualid;
        }
    }

    if (!visualID)
    {
        if (!prevVisualID)
        {
            throw std::logic_error ("NO visualid available!");
        }
        else
        {
            _M_gFormat = to_format (fb_config);
        }
    }

    _M_pCfg = fb_config;
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
    //static int value;

    std::cout << "Buffer Size: " << static_cast<u16> (_M_gFormat.depth + _M_gFormat.alpha)
              << std::endl;

    std::cout << "Red Size: "   << static_cast<u16> (_M_gFormat.red)   << std::endl;
    std::cout << "Green Size: " << static_cast<u16> (_M_gFormat.green) << std::endl;
    std::cout << "Blue Size: "  << static_cast<u16> (_M_gFormat.blue)  << std::endl;
    std::cout << "Alpha Size: " << static_cast<u16> (_M_gFormat.alpha) << std::endl;
    std::cout << "Depth size: " << static_cast<u16> (_M_gFormat.depth) << std::endl;

    /*::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_CONFIG_CAVEAT, &value);

    switch (value)
    {
    case internal::none:
        std::cout << "EGL_CONFIG_CAVEAT: EGL_NONE" << std::endl;
        break;
    case EGL_SLOW_CONFIG:
        std::cout << "EGL_CONFIG_CAVEAT: EGL_SLOW_CONFIG" << std::endl;
        break;
    }

    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_CONFIG_ID, &value);
    std::cout << "Config ID: " << value << std::endl;

    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_MAX_PBUFFER_WIDTH, &value);
    std::cout << "Max pbuffer width: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_MAX_PBUFFER_HEIGHT, &value);
    std::cout << "Max pbuffer height: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_MAX_PBUFFER_PIXELS, &value);
    std::cout << "Max pbuffer pixels: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_NATIVE_RENDERABLE, &value);
    std::cout << "Native renderable: " << (value ? "true" : "false") << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_NATIVE_VISUAL_ID, &value);
    std::cout << "Native visual ID: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_NATIVE_VISUAL_TYPE, &value);
    std::cout << "Native visual type: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_SAMPLE_BUFFERS, &value);
    std::cout << "Sample Buffers: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_SAMPLES, &value);
    std::cout << "Samples: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_SURFACE_TYPE, &value);
    std::cout << "Surface type: " << value << std::endl;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_TRANSPARENT_TYPE, &value);*/
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
{ }

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

point2u surface::size () const noexcept
{
    return point2u ();
}

/// TODO: finish egl resizing
void surface::scale (point2u /*gSize*/)
{
    if (context_interface::current ())
    {
        auto drawable = context_interface::current ()->drawable ();
        auto readable = context_interface::current ()->readable ();

        if (drawable == this && readable == this)
        {
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

        ::glXDestroyContext (config ().legacy ().get<internal::display_pointer> (),
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

        ::glXDestroyContext (config ().legacy ().get<internal::display_pointer> (),
                             _M_pGC.get<internal::context_pointer> ());
    }

    _M_pGC = obj._M_pGC ? internal::create_gc (obj.config (), obj._M_nVersion) :
                          internal::context_pointer ();

    _M_pReadTarget = _M_pDrawTarget = nullptr;
    _M_nVersion    = obj._M_nVersion;
    _M_pConf       = obj._M_pConf   ;

    return *this;
}

context::~context () noexcept
{
    if (!_M_pGC) return;

    if (active ()) acquire (nullptr);

    ::glXDestroyContext (config ().legacy ().get<internal::display_pointer> (),
                         _M_pGC.get<internal::context_pointer> ());
}

resource_version context::platform_version () noexcept
{
    return internal::version ();
}

bool context::use (pointer pDraw, const_pointer pRead) noexcept
{
    if ((( pDraw and !pRead) or (pDraw and pDraw->device () != device_backend::gl)) or
        ((!pDraw and  pRead) or (pRead and pRead->device () != device_backend::gl)))
        return false;

    _M_pDrawTarget = pDraw;
    _M_pReadTarget = pRead;

    assign ();
    return true;
}

bool context::assign () noexcept
{
    if (::glXMakeContextCurrent (config ().legacy ().get<internal::display_pointer> (),
                                 _M_pDrawTarget != nullptr ?
                                 _M_pDrawTarget->handle ().get<internal::surface_id> () : internal::surface_id (),
                                 _M_pReadTarget != nullptr ?
                                 _M_pReadTarget->handle ().get<internal::surface_id> () : internal::surface_id (),
                                 _M_pGC.get<internal::context_pointer> ()))
    {
        acquire (this);
        return true;
    }

    return true;
}

void context::release () noexcept
{
    if (active ())
    {
        ::glXMakeContextCurrent (config ().legacy ().get<internal::display_pointer> (),
                                 internal::surface_id (),
                                 internal::surface_id (),
                                 internal::context_pointer ());
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
