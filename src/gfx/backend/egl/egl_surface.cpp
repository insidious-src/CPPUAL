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

#include "egl_surface.h"

#include <cppual/meta_string>
#include <cppual/gfx/gl/gldef.h>

#include <cstring>
#include <iostream>
//#include <algorithm>

#include <EGL/egl.h>

namespace cppual::gfx::gl {

// ====================================================

namespace { namespace internal { // optimize for internal unit usage

typedef ::EGLint                        value_type     ;
typedef ::EGLBoolean                    bool_type      ;
typedef ::EGLDisplay                    display_pointer;
typedef ::EGLSurface                    surface_pointer;
typedef ::EGLContext                    context_pointer;
typedef ::EGLNativeDisplayType          native_display ;
typedef context_interface::version_type version_type   ;

enum class API : u8
{
    Unbound = 0,
    OpenGL,
    OpenGLES
};

enum
{
    Width               = EGL_WIDTH,
    Height              = EGL_HEIGHT,
    Extensions          = EGL_EXTENSIONS,
    GLID                = EGL_OPENGL_API,
    GLESID              = EGL_OPENGL_ES_API,
    GLBit               = EGL_OPENGL_BIT,
    GLESBit             = EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT,
    BasicContextVersion = EGL_CONTEXT_CLIENT_VERSION,
    ContextMajorVersion = /*EGL_CONTEXT_MAJOR_VERSION_KHR*/ 0x3098,
    ContextMinorVersion = /*EGL_CONTEXT_MINOR_VERSION_KHR*/ 0x30FB,
    ContextProfile      = /*EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR*/ 0x30FD,
    BadDisplay          = EGL_BAD_DISPLAY,
    BadContext          = EGL_BAD_CONTEXT,
    BadSurface          = EGL_BAD_SURFACE,
    BadCurrentSurface   = EGL_BAD_CURRENT_SURFACE,
    BadNativeWindow     = EGL_BAD_NATIVE_WINDOW,
    BadConfig           = EGL_BAD_CONFIG,
    BadAttribute        = EGL_BAD_ATTRIBUTE,
    NoMatch             = EGL_BAD_MATCH,
    BadParam            = EGL_BAD_PARAMETER,
    NoAccess            = EGL_BAD_ACCESS,
    BadAlloc            = EGL_BAD_ALLOC,
    NotInitialized      = EGL_NOT_INITIALIZED,
    ContextLost         = EGL_CONTEXT_LOST,
    ANGLEFixedSize      = /*EGL_FIXED_SIZE_ANGLE*/ 0x3201,
    none                = EGL_NONE,
    IntTrue             = EGL_TRUE,
    IntFalse            = EGL_FALSE
};

enum class error_type : u8
{
    initialize,
    terminate,
    make_current,
    create,
    destroy,
    query_surface,
    get_configs,
    choose_config
};

constexpr version_type& version () noexcept
{
    static version_type eglVersion { 0, 0 };
    return eglVersion;
}

// ====================================================

template <error_type>
void error ();

template <>
constexpr void error <error_type::initialize> ()
{
    switch (::eglGetError ())
    {
    case BadDisplay:
        throw bad_display ("NOT an EGL display connection");
    case NotInitialized:
        throw not_initialized ("target display CANNOT be initialized");
    }
}

template <>
void error <error_type::make_current> ()
{
    switch (::eglGetError ())
    {
    case BadDisplay:
        throw bad_display ("invalid display handle");
    case NotInitialized:
        throw not_initialized ("EGL is NOT initialized for target _M_pDisplay");
    case BadSurface:
        throw bad_surface ("draw or read is NOT an EGL surface");
    case BadContext:
        throw bad_context ("NOT an EGL rendering context");
    case NoMatch:
        throw bad_match ("draw or read are NOT compatible with the context");
    case NoAccess:
        throw bad_access ("the context is already current to some other thread");
    case BadNativeWindow:
        throw bad_window ("the window underlying either draw or read is NO longer valid");
    case BadCurrentSurface:
        throw bad_surface ("previous context has unflushed commands "
                           "and the previous surface is NO longer valid");
    case BadAlloc:
        throw bad_alloc ("EGL failed to allocate resources for the requested operation");
    case ContextLost:
        throw bad_context ("a power management event has occurred");
    }
}

template <>
void error <error_type::create> ()
{
    switch (::eglGetError ())
    {
    case NoMatch:
        throw bad_match ("rendering api NOT bound");
    case BadDisplay:
        throw bad_display ("invalid display handle");
    case NotInitialized:
        throw not_initialized ("EGL is NOT initialized for target display");
    case BadConfig:
        throw bad_config ("NOT an EGL frame buffer configuration, "
                          "or does NOT support the current rendering API");
    case BadNativeWindow:
        throw bad_window ("the window attributes do NOT correspond to target config "
                          "or it does NOT support rendering to windows");
    case BadContext:
        throw bad_config ("shared context is NOT an EGL rendering context "
                          "of the same client API type as the newly created context");
    case BadAttribute:
        throw bad_attrib ("attribute value is NOT recognized or out of range");
    case BadAlloc:
        throw bad_alloc ("EGL failed to allocate resources for the requested operation");
    }
}

template <>
constexpr void error <error_type::destroy> ()
{
    switch (::eglGetError ())
    {
    case BadDisplay:
        throw bad_display ("invalid display handle");
    case NotInitialized:
        throw not_initialized ("EGL is NOT initialized for target display");
    case BadSurface:
        throw bad_surface ("invalid surface handle");
    }
}

template <>
void error <error_type::query_surface> ()
{
    switch (::eglGetError ())
    {
    case BadDisplay:
        throw bad_display ("invalid display handle");
    case NotInitialized:
        throw not_initialized ("EGL is NOT initialized for target display");
    case BadSurface:
        throw bad_surface ("invalid surface handle");
    case BadAttribute:
        throw bad_attrib ("attribute value is NOT recognized or out of range");
    }
}

template <>
constexpr void error <error_type::get_configs> ()
{
    switch (::eglGetError ())
    {
    case BadDisplay:
        throw bad_display ("display is NOT an EGL display connection");
    case NotInitialized:
        throw not_initialized ("display has NOT been initialized");
    case BadParam:
        throw std::logic_error ("num_config is NULL");
    }
}

template <>
constexpr void error <error_type::choose_config> ()
{
    switch (::eglGetError ())
    {
    case BadDisplay:
        throw bad_display ("display is NOT an EGL display connection");
    case NotInitialized:
        throw not_initialized ("display has NOT been initialized");
    case BadAttribute:
        throw bad_attrib ("attribute_list contains an invalid frame buffer configuration "
                          "attribute or an attribute value that is unrecognized or out of range");
    case BadParam:
        throw std::logic_error ("num_config is NULL");
    }
}

// ====================================================

constexpr bool_type initialize (display_pointer dsp)
{
    bool_type ret;

    if (!(ret = ::eglInitialize (dsp, &internal::version ().major, &internal::version ().minor)))
        error<error_type::initialize> ();

    if (internal::version ().minor < 4)
    {
        ::eglTerminate (dsp);
        throw std::logic_error ("the EGL implementation must be atleast 1.4 version");
    }

    return ret;
}

// ====================================================

constexpr uint api (API eAPI) noexcept
{
    return eAPI == API::OpenGL ? GLID :
                                 eAPI == API::OpenGLES ? GLESID : none;
}

constexpr value_type api_bits (API eAPI) noexcept
{
    return eAPI == API::OpenGL ? GLBit :
                                 eAPI == API::OpenGLES ? GLESBit : none;
}

// ====================================================

constexpr config::feature_types convert_extensions (display_pointer dsp)
{
    config::feature_types eFeatures;

    auto const extensions = ::eglQueryString (dsp, internal::Extensions);

    std::cout << extensions << std::endl;

    for (auto
         extension = extensions; extension++;
         extension = std::strchr (extension , ' '))
    {
        switch (char_hash (extension))
        {
        case char_hash ("EGL_KHR_surfaceless_context"):
            eFeatures += config::feature::surfaceless_context;
            break;
        case char_hash ("EGL_MESA_configless_context"):
            eFeatures += config::feature::configless_context;
            break;
        case char_hash ("EGL_CHROMIUM_sync_control"):
            eFeatures += config::feature::sync_control;
            break;
        case char_hash ("EGL_EXT_create_context_robustness"):
            eFeatures += config::feature::create_robust_context;
            break;
        case char_hash ("EGL_ANGLE_window_fixed_size"):
            eFeatures += config::feature::scalable_surface;
            break;
        case char_hash ("EGL_KHR_create_context"):
            eFeatures += config::feature::context_attributes_ext;
            break;
        }
    }

    return eFeatures;
}

constexpr surface_pointer create_drawable (config const&                   gConf,
                                        point2u                         /*gSize*/,
                                        resource_interface::handle_type uWndHandle,
                                        bool                            bDouble)
{
    value_type nSurfaceAttribs[9];

    nSurfaceAttribs[0] = EGL_RENDER_BUFFER;
    nSurfaceAttribs[1] = bDouble ? EGL_BACK_BUFFER : EGL_SINGLE_BUFFER;

    //nSurfaceAttribs[2] = internal::ANGLEFixedSize;
    //nSurfaceAttribs[3] = internal::IntTrue;
    //nSurfaceAttribs[2] = internal::Width;
    //nSurfaceAttribs[3] = static_cast<value_type> (gSize.x);
    //nSurfaceAttribs[4] = internal::Height;
    //nSurfaceAttribs[5] = static_cast<value_type> (gSize.y);
    nSurfaceAttribs[2] = internal::none;

    surface_pointer pSurface = ::eglCreateWindowSurface (gConf.display (),
                                                         gConf,
                                                         uWndHandle.get<::EGLNativeWindowType> (),
                                                         nSurfaceAttribs);

    if (!pSurface) error<error_type::create> ();
    return pSurface;
}

constexpr surface_pointer create_pbuffer (config const& gConf, point2u gSize)
{
    value_type const nPBufferAttribs[] =
    {
        internal::Width,         static_cast<value_type> (gSize.x),
        internal::Height,        static_cast<value_type> (gSize.y),
        EGL_COLORSPACE,     EGL_COLORSPACE_sRGB,
        EGL_ALPHA_FORMAT,   EGL_ALPHA_FORMAT_NONPRE,
        EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGBA,
        EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
        internal::none
    };

    surface_pointer _M_pPixelBuffer = ::eglCreatePbufferSurface (gConf.display (),
                                                                 gConf,
                                                                 nPBufferAttribs);

    if (!_M_pPixelBuffer) error<error_type::create> ();
    return _M_pPixelBuffer;
}

constexpr surface_pointer create_pixmap (config const& gConf)
{
    surface_pointer _M_pSurface = ::eglCreatePixmapSurface (gConf.display (),
                                                            gConf,
                                                            0,
                                                            nullptr);

    if (!_M_pSurface) error<error_type::create> ();
    return _M_pSurface;
}

constexpr surface_pointer create_surface (config const&         gConf,
                                       point2u               size,
                                       surface_type          type,
                                       surface::handle_type  owner)
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

constexpr context_pointer create_gc (config const& gConf, version_type version, void* pShared)
{
    value_type nContextAttribs[5];

    if (gConf.features ().test (config::feature::context_attributes_ext))
    {
        nContextAttribs[0] = internal::ContextMajorVersion;
        nContextAttribs[1] = version.major;
        nContextAttribs[2] = internal::ContextMinorVersion;
        nContextAttribs[3] = version.minor;
        nContextAttribs[4] = internal::none;
    }
    else
    {
        nContextAttribs[0] = internal::BasicContextVersion;
        nContextAttribs[1] = version.major;
        nContextAttribs[2] = internal::none;
    }

    if(!::eglBindAPI (api (API::OpenGL)))
        throw bad_parameter ("the specified API "
                             "is NOT supported by the EGL implementation");

    EGLContext pContext = ::eglCreateContext (gConf.display (),
                                              gConf,
                                              pShared,
                                              nContextAttribs);

    if   (!pContext) error<error_type::create> ();
    return pContext;
}

constexpr point2u get_size (display_pointer dsp, surface_pointer surface) noexcept
{
    value_type size[2];

    if (::eglQuerySurface (dsp, surface, internal::Width , &size[0]) == internal::IntFalse)
        error<error_type::query_surface> ();

    if (::eglQuerySurface (dsp, surface, internal::Height, &size[1]) == internal::IntFalse)
        error<error_type::query_surface> ();

    return { static_cast<u16> (size[0]), static_cast<u16> (size[1]) };
}

} } // anonymous namespace internal

// ====================================================

config::config (connection_type native, format_type gFormat)
: _M_pDisplay   (::eglGetDisplay (native.get<internal::native_display> ())),
  _M_pCfg       (),
  _M_gFormat    (),
  _M_eFeatures  ()
{
    if (!_M_pDisplay) throw std::logic_error ("invalid display");

    static const ::EGLint config_size = 10;

    internal::value_type nNumConfigs = internal::value_type ();
    ::EGLConfig ptr[config_size];

    internal::value_type const nConfigAttribs[]
    {
        EGL_RED_SIZE,        gFormat.red,
        EGL_GREEN_SIZE,      gFormat.green,
        EGL_BLUE_SIZE,       gFormat.blue,
        EGL_ALPHA_SIZE,      gFormat.alpha,
        EGL_DEPTH_SIZE,      gFormat.depth,
        EGL_STENCIL_SIZE,    gFormat.stencil,
        EGL_SURFACE_TYPE,    gFormat.flags.test (pixel_flag::drawable) ?
                    EGL_WINDOW_BIT : gFormat.flags.test (pixel_flag::palette) ?
                        EGL_PBUFFER_BIT : gFormat.flags.test (pixel_flag::bitmap) ?
                            EGL_PIXMAP_BIT : internal::value_type (),
        EGL_RENDERABLE_TYPE, internal::api_bits (internal::API::OpenGL),
        internal::none
    };

    static auto init = internal::initialize (_M_pDisplay);

    UNUSED (init);

    if (::eglGetConfigs   (_M_pDisplay, ptr, config_size, &nNumConfigs) == internal::IntFalse)
        internal::error<internal::error_type::get_configs> ();

    if (::eglChooseConfig (_M_pDisplay, nConfigAttribs, ptr, config_size, &nNumConfigs) == internal::IntFalse)
        internal::error<internal::error_type::choose_config> ();

    _M_pCfg = nNumConfigs > 0 ? ptr[0] : nullptr;

    _M_eFeatures = internal::convert_extensions (_M_pDisplay);
    _M_gFormat   = to_format ();

    if (_M_eFeatures.test (feature::sync_control)) ::eglSwapInterval (_M_pDisplay, 1);

#   ifdef DEBUG_MODE
    std::cout << ::eglQueryString (_M_pDisplay, internal::Extensions) << std::endl;
#   endif
}

pixel_format config::to_format () const
{
    if (!_M_pDisplay or !_M_pCfg) return pixel_format ();

    int nAttribs[attrib_count];

    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_RED_SIZE    , &nAttribs[red]);
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_GREEN_SIZE  , &nAttribs[green]);
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_BLUE_SIZE   , &nAttribs[blue]);
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_ALPHA_SIZE  , &nAttribs[alpha]);
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_DEPTH_SIZE  , &nAttribs[depth]);
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_STENCIL_SIZE, &nAttribs[stencil]);
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_SURFACE_TYPE, &nAttribs[surface_type]);

    return
    {
        (nAttribs[surface_type] & EGL_WINDOW_BIT) ? pixel_flag::drawable : 0 |
                (nAttribs[surface_type] & EGL_PBUFFER_BIT) ? pixel_flag::bitmap : 0,
        static_cast<u8> (nAttribs[red]),
        static_cast<u8> (nAttribs[green]),
        static_cast<u8> (nAttribs[blue]),
        static_cast<u8> (nAttribs[alpha]),
        static_cast<u8> (nAttribs[depth]),
        static_cast<u8> (nAttribs[stencil]),
        color_type::true_type
    };
}

config::int_type config::id () const
{
    if (!_M_pDisplay or !_M_pCfg) return -1;
    ::EGLint id;

    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_CONFIG_ID, &id);
    return id;
}

void config::print ()
{
    static ::EGLint value;

    std::cout << "Buffer Size: " << static_cast<u16> (_M_gFormat.depth + _M_gFormat.alpha);

    std::cout << "\nRed Size: "   << static_cast<u16> (_M_gFormat.red);
    std::cout << "\nGreen Size: " << static_cast<u16> (_M_gFormat.green);
    std::cout << "\nBlue Size: "  << static_cast<u16> (_M_gFormat.blue);
    std::cout << "\nAlpha Size: " << static_cast<u16> (_M_gFormat.alpha);
    std::cout << "\nDepth size: " << static_cast<u16> (_M_gFormat.depth);

    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_CONFIG_CAVEAT, &value);

    switch (value)
    {
    case internal::none:
        std::cout << "\nEGL_CONFIG_CAVEAT: EGL_NONE";
        break;
    case EGL_SLOW_CONFIG:
        std::cout << "\nEGL_CONFIG_CAVEAT: EGL_SLOW_CONFIG";
        break;
    }

    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_CONFIG_ID, &value);
    std::cout << "\nConfig ID: " << value;

    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_MAX_PBUFFER_WIDTH, &value);
    std::cout << "\nMax pbuffer width: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_MAX_PBUFFER_HEIGHT, &value);
    std::cout << "\nMax pbuffer height: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_MAX_PBUFFER_PIXELS, &value);
    std::cout << "\nMax pbuffer pixels: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_NATIVE_RENDERABLE, &value);
    std::cout << "\nNative renderable: " << (value ? "true" : "false");
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_NATIVE_VISUAL_ID, &value);
    std::cout << "\nNative visual ID: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_NATIVE_VISUAL_TYPE, &value);
    std::cout << "\nNative visual type: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_SAMPLE_BUFFERS, &value);
    std::cout << "\nSample Buffers: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_SAMPLES, &value);
    std::cout << "\nSamples: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_SURFACE_TYPE, &value);
    std::cout << "\nSurface type: " << value;
    ::eglGetConfigAttrib (_M_pDisplay, _M_pCfg, EGL_TRANSPARENT_TYPE, &value);
    std::cout << "\nTransparent type: " << value << std::endl;
}

config::~config ()
{
    //if (!_M_pDisplay || !_M_pCfg) return;

    //::eglTerminate (_M_pDisplay);
    //::eglReleaseThread ();
}

// ====================================================

surface::surface (conf_reference gConf, point2u size, handle_type wnd, surface_type type)
: _M_pConf   (gConf),
  _M_pHandle (internal::create_surface (gConf, size, type, wnd)),
  _M_pWnd    (_M_pHandle ? wnd : handle_type ()),
  _M_eType   (type)
{
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
        if(!::eglDestroySurface (connection (), handle ()))
            internal::error<internal::error_type::destroy>();
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
    destroy ();
}

void surface::flush ()
{
    if (_M_eType == surface_type::double_buffer)
    {
        ::eglSwapBuffers (connection (), handle ());
    }
    else
    {
        ::glFlush ();
    }
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
    return internal::get_size (configuration ().display (), _M_pHandle);
}

/// TODO: finish egl resizing
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
        }
    }
}

void surface::destroy ()
{
    if (!_M_pHandle) return;

    if(!::eglDestroySurface (connection (), handle ()))
        internal::error<internal::error_type::destroy>();
}

// ====================================================

context::context (conf_reference conf, version_type const& version, shared_context shared)
: _M_pConf        (conf),
  _M_pGC          (internal::create_gc (conf, version, shared ? shared->handle () : nullptr)),
  _M_pDrawTarget  (),
  _M_pReadTarget  (),
  _M_pShared      (shared),
  _M_nVersion     (_M_pGC ? version : version_type ())
{ }

context::context (context const& obj)
: _M_pConf       (obj._M_pConf),
  _M_pGC         (internal::create_gc (_M_pConf,
                                  obj._M_nVersion,
                                  obj._M_pShared ? obj._M_pShared->handle () : nullptr)),
  _M_pDrawTarget (),
  _M_pReadTarget (),
  _M_pShared     (obj._M_pShared),
  _M_nVersion    (_M_pGC ? obj._M_nVersion : version_type ())
{ }

context& context::operator = (context&& obj) noexcept
{
    if (this == &obj) return *this;

    if (_M_pGC)
    {
        if (active ()) release ();
        ::eglDestroyContext (config ().display (), _M_pGC);
    }

    _M_pDrawTarget = obj._M_pDrawTarget;
    _M_pReadTarget = obj._M_pReadTarget;
    _M_nVersion    = obj._M_nVersion;
    _M_pConf       = obj._M_pConf;
    _M_pShared     = obj._M_pShared;
    _M_pGC         = obj._M_pGC;

    obj._M_nVersion    = version ();
    obj._M_pGC         = nullptr;
    obj._M_pConf       = conf_value ();
    obj._M_pShared     = nullptr;
    obj._M_pDrawTarget = nullptr;
    obj._M_pReadTarget = nullptr;

    return *this;
}

context& context::operator = (context const& obj)
{
    if (this == &obj) return *this;

    if (_M_pGC)
    {
        release ();
        ::eglDestroyContext (config ().display (), _M_pGC);
    }

    _M_pGC = obj._M_pGC ? internal::create_gc (obj.configuration (),
                                          obj._M_nVersion,
                                          obj._M_pShared ? obj._M_pShared->handle () : nullptr) :
                          nullptr;

    _M_pReadTarget = _M_pDrawTarget = nullptr;
    _M_nVersion    = obj._M_nVersion;
    _M_pShared     = obj._M_pShared ;
    _M_pConf       = obj._M_pConf   ;

    return *this;
}

context::~context () noexcept
{
    if (!_M_pGC) return;

    acquire (nullptr);

    ::eglDestroyContext (configuration ().display (), _M_pGC);
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

    if (!::eglMakeCurrent (local_cntxt->configuration ().display (),
                           local_cntxt->_M_pDrawTarget != nullptr ?
                           local_cntxt->_M_pDrawTarget->handle ().get<internal::surface_pointer> () :
                           internal::surface_pointer (),
                           local_cntxt->_M_pReadTarget != nullptr ?
                           local_cntxt->_M_pReadTarget->handle ().get<internal::surface_pointer> () :
                           internal::surface_pointer (),
                           local_cntxt->_M_pGC))
        internal::error<internal::error_type::make_current> ();

    return true;
}

void context::release () noexcept
{
    if (!active ()) return;

    if (!::eglMakeCurrent (configuration ().display (), nullptr, nullptr, nullptr))
        internal::error<internal::error_type::make_current> ();

    if (static_cast<surface*> (_M_pDrawTarget.get ())->_M_pContext.get () == this)
        static_cast<surface*> (_M_pDrawTarget.get ())->_M_pContext.reset ();

    if (static_cast<surface*> (_M_pReadTarget.get ())->_M_pContext.get () == this)
        static_cast<surface*> (_M_pReadTarget.get ())->_M_pContext.reset ();
}

void context::flush () noexcept
{
    if (active ()) ::glFlush ();
}

void context::finish () noexcept
{
    if (active ()) ::eglWaitGL ();
}

} // namespace EGL
