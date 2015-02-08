/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <cppual/gfx/gl/surface.h>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <EGL/egl.h>
#include <GL/glew.h>

namespace cppual { namespace Graphics { namespace GL {

namespace { namespace EGL { // optimize for internal unit usage

typedef EGLint               value_type;
typedef EGLDisplay           display_pointer;
typedef EGLSurface           surface_pointer;
typedef EGLContext           context_pointer;
typedef EGLNativeDisplayType native_display ;

enum class API
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
	ContextMajorVersion = 0x3098, /*EGL_CONTEXT_MAJOR_VERSION_KHR*/
	ContextMinorVersion = 0x30FB, /*EGL_CONTEXT_MINOR_VERSION_KHR*/
	ContextProfile      = 0x30FD, /*EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR*/
	BadDisplay          = EGL_BAD_DISPLAY,
	BadContext          = EGL_BAD_CONTEXT,
	BadSurface          = EGL_BAD_SURFACE,
	BadCurrentSurface   = EGL_BAD_CURRENT_SURFACE,
	BadNativeWindow     = EGL_BAD_NATIVE_WINDOW,
	BadConfig           = EGL_BAD_CONFIG,
	BadAttribute        = EGL_BAD_ATTRIBUTE,
	NoMatch             = EGL_BAD_MATCH,
	NoAccess            = EGL_BAD_ACCESS,
	NoAlloc             = EGL_BAD_ALLOC,
	NotInitialized      = EGL_NOT_INITIALIZED,
	ContextLost         = EGL_CONTEXT_LOST,
	ANGLEFixedSize      = 0x3201/*EGL_FIXED_SIZE_ANGLE*/,
	NONE                = EGL_NONE,
	IntTrue             = EGL_TRUE,
	IntFalse            = EGL_FALSE
};

enum ErrorType
{
	Initialize,
	Terminate,
	MakeCurrent,
	Create,
	Destroy
};

inline GFXVersion& version () noexcept
{
	static GFXVersion eglVersion { 0, 0 };
	return eglVersion;
}

// ====================================================

template <ErrorType>
void error ();

template <>
inline void error <Initialize> ()
{
	switch (eglGetError ())
	{
	case BadDisplay:
		throw bad_display ("NOT an EGL display connection");
	case NotInitialized:
		throw not_initialized ("target display CANNOT be initialized");
	default:
		throw std::bad_exception ();
	}
}

template <>
inline void error <MakeCurrent> ()
{
	switch (eglGetError ())
	{
	case BadDisplay:
		throw bad_display ("invalid display handle");
	case NotInitialized:
		throw not_initialized ("EGL is NOT initialized for target m_pDisplay");
	case BadSurface:
		throw bad_surface ("draw or read is NOT an EGL surface");
	case BadContext:
		throw bad_context ("NOT an EGL rendering context");
	case NoMatch:
		throw bad_match ("draw or read are NOT compatible with the context");
	case NoAccess:
		throw bad_access ("the context is already current to some other thread");
	case BadNativeWindow:
		throw bad_window ("the window underlying either draw or read is no longer valid");
	case BadCurrentSurface:
		throw bad_surface ("previous context has unflushed commands "
						   "and the previous surface is no longer valid");
	case NoAlloc:
		throw std::bad_alloc ();
	case ContextLost:
		throw bad_context ("a power management event has occurred");
	}
}

template <>
inline void error <Create> ()
{
	switch (eglGetError ())
	{
	case NoMatch:
		throw bad_match ("rendering api not bound");
		break;
	case BadDisplay:
		throw bad_display ("invalid display handle");
		break;
	case NotInitialized:
		throw not_initialized ("EGL is NOT initialized for target display");
		break;
	case BadConfig:
		throw bad_config ("NOT an EGL frame buffer configuration, "
						  "or does not support the current rendering API");
		break;
	case BadNativeWindow:
		throw bad_window ("the window attributes do not correspond to target config "
						  "or it does not support rendering to windows");
		break;
	case BadContext:
		throw bad_config ("shared context is not an EGL rendering context "
						  "of the same client API type as the newly created context");
	case BadAttribute:
		throw bad_config ("attribute value is NOT recognized or out of range");
		break;
	case NoAlloc:
		throw std::bad_alloc ();
		break;
	}
}

// ====================================================

inline void initialize (display_pointer dsp)
{
	if (!eglInitialize (dsp, &EGL::version ().major, &EGL::version ().minor))
		error<Initialize> ();

	if (EGL::version ().minor < 4)
	{
		eglTerminate (dsp);
		throw std::logic_error ("the EGL implementation must be atleast 1.4 version");
	}
}

// ====================================================

constexpr uint api (API eAPI) noexcept
{
	return eAPI == API::OpenGL ? GLID :
								 eAPI == API::OpenGLES ? GLESID : NONE;
}

constexpr value_type api_bits (API eAPI) noexcept
{
	return eAPI == API::OpenGL ? GLBit :
								 eAPI == API::OpenGLES ? GLESBit : NONE;
}

constexpr uint const_hash (cchar* input)
{
	return *input and *input != ' ' ?
									static_cast<uint> (*input) + 33 * const_hash (input + 1) :
									5381;
}

// ====================================================

inline Config::Features convertExtensions (display_pointer dsp)
{
	Config::Features eFeatures (nullptr);

	for (auto
		 extension = eglQueryString (dsp, EGL::Extensions) - 1; extension++;
		 extension = std::strchr (extension , ' '))
	{
		switch (const_hash (extension))
		{
		case const_hash ("EGL_KHR_surfaceless_context"):
			eFeatures += Config::SurfacelessContext;
			break;
		case const_hash ("EGL_MESA_configless_context"):
			eFeatures += Config::ConfiglessContext;
			break;
		case const_hash ("EGL_CHROMIUM_sync_control"):
			eFeatures += Config::SyncControl;
			break;
		case const_hash ("EGL_EXT_create_context_robustness"):
			eFeatures += Config::CreateRobustContext;
			break;
		case const_hash ("EGL_ANGLE_window_fixed_size"):
			eFeatures += Config::ScalableSurface;
			break;
		case const_hash ("EGL_KHR_create_context"):
			eFeatures += Config::ContextAttributesExt;
			break;
			break;
		}
	}

	return eFeatures;
}

inline surface_pointer createDrawable (Config const&         gConf,
									   point2u               gSize,
									   IResource::value_type uWndHandle,
									   bool                  bDouble)
{
	value_type nSurfaceAttribs[9];

	nSurfaceAttribs[0] = EGL_RENDER_BUFFER;
	nSurfaceAttribs[1] = bDouble ? EGL_BACK_BUFFER : EGL_SINGLE_BUFFER;

	if (gConf.features ().test (Config::ScalableSurface))
	{
		nSurfaceAttribs[2] = EGL::ANGLEFixedSize;
		nSurfaceAttribs[3] = EGL::IntTrue;
		nSurfaceAttribs[4] = EGL::Width;
		nSurfaceAttribs[5] = gSize.x;
		nSurfaceAttribs[6] = EGL::Height;
		nSurfaceAttribs[7] = gSize.y;
		nSurfaceAttribs[8] = EGL::NONE;
	}
	else
		nSurfaceAttribs[2] = EGL::NONE;

	surface_pointer pSurface = eglCreateWindowSurface (gConf.display (),
													   gConf,
													   uWndHandle.get<EGLNativeWindowType> (),
													   &nSurfaceAttribs[0]);

	if (!pSurface) error<Create> ();
	return pSurface;
}

inline surface_pointer createPBuffer (Config const& gConf, point2u gSize)
{
	value_type const nPBufferAttribs[] =
	{
		EGL::Width,         static_cast<value_type> (gSize.x),
		EGL::Height,        static_cast<value_type> (gSize.y),
		EGL_COLORSPACE,     EGL_COLORSPACE_sRGB,
		EGL_ALPHA_FORMAT,   EGL_ALPHA_FORMAT_NONPRE,
		EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGBA,
		EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
		EGL::NONE
	};

	surface_pointer m_pPixelBuffer = eglCreatePbufferSurface (gConf.display (),
															  gConf,
															  nPBufferAttribs);

	if (!m_pPixelBuffer) error<Create> ();
	return m_pPixelBuffer;
}

inline surface_pointer createPixmap (Config const& gConf)
{
	surface_pointer m_pSurface = eglCreatePixmapSurface (gConf.display (),
														 gConf,
														 0,
														 nullptr);

	if (!m_pSurface) error<Create> ();
	return m_pSurface;
}

inline surface_pointer createSurface (Config const&       gConf,
									  point2u             size,
									  Surface::Type       type,
									  Surface::value_type owner)
{
	switch (type)
	{
	case Surface::Type::BackBuffer:
		return EGL::createPBuffer (gConf, size);
	case Surface::Type::Pixmap:
		return EGL::createPixmap (gConf);
	default:
		return EGL::createDrawable (gConf, size, owner, type == Surface::Type::DoubleBuffer);
	}
}

inline context_pointer createGC (Config const& gConf, GFXVersion version, void* pShared)
{
	value_type nContextAttribs[5];

	if (gConf.features ().test (Config::ContextAttributesExt))
	{
		nContextAttribs[0] = EGL::ContextMajorVersion;
		nContextAttribs[1] = version.major;
		nContextAttribs[2] = EGL::ContextMinorVersion;
		nContextAttribs[3] = version.minor;
		nContextAttribs[4] = EGL::NONE;
	}
	else
	{
		nContextAttribs[0] = EGL::BasicContextVersion;
		nContextAttribs[1] = version.major;
		nContextAttribs[2] = EGL::NONE;
	}

	eglBindAPI (EGL::api (API::OpenGL)) ?
					true : throw bad_parameter ("the specified worthless API "
												"is NOT supported by the EGL implementation");

	EGLContext pContext = eglCreateContext (gConf.display (),
											gConf,
											pShared,
											&nContextAttribs[0]);

	if (!pContext) error<Create> ();
	return pContext;
}

inline point2u getSize (Config const& config, Surface::pointer surface) noexcept
{
	value_type size[2];

	if (eglQuerySurface (config.display (), surface, EGL::Width, &size[0]) == EGL::IntFalse)
		return point2u ();

	if (eglQuerySurface (config.display (), surface, EGL::Height, &size[1]) == EGL::IntFalse)
		return point2u ();

	return { static_cast<u16> (size[0]), static_cast<u16> (size[1]) };
}

} } // anonymous namespace EGL

// ====================================================

Config::Config (controller dsp, format_type gFormat)
: m_pDisplay  (eglGetDisplay (dsp.get<EGL::native_display> ())),
  m_pCfg      (),
  m_gFormat   (),
  m_eFeatures ()
{
	if (!m_pDisplay) throw std::logic_error ("invalid display");

	EGL::value_type nNumConfigs = 0;

	EGL::value_type const nConfigAttribs[]
	{
		EGL_RED_SIZE,        gFormat.red,
		EGL_GREEN_SIZE,      gFormat.green,
		EGL_BLUE_SIZE,       gFormat.blue,
		EGL_ALPHA_SIZE,      gFormat.alpha,
		EGL_DEPTH_SIZE,      gFormat.depth,
		EGL_STENCIL_SIZE,    gFormat.stencil,
		EGL_SURFACE_TYPE,    gFormat.flags.test (PixelFlag::Drawable) ?
					EGL_WINDOW_BIT : 0 | gFormat.flags.test (PixelFlag::Palette) ?
						EGL_PBUFFER_BIT : 0 | gFormat.flags.test (PixelFlag::Bitmap) ?
							EGL_PIXMAP_BIT : 0,
		EGL_RENDERABLE_TYPE, EGL::api_bits (EGL::API::OpenGL),
		EGL::NONE
	};

	EGL::initialize (m_pDisplay);

	eglGetConfigs   (m_pDisplay, &m_pCfg, 1, &nNumConfigs);
	eglChooseConfig (m_pDisplay, nConfigAttribs, &m_pCfg, 1, &nNumConfigs);

	m_eFeatures = EGL::convertExtensions (m_pDisplay);
	m_gFormat   = toFormat ();

	std::cout << eglQueryString (display (), EGL::Extensions) << std::endl;
}

PixelFormat Config::toFormat () const
{
	if (!m_pDisplay or !m_pCfg) return PixelFormat ();

	int nAttribs[AttribCount];

	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_RED_SIZE, &nAttribs[Red]);
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_GREEN_SIZE, &nAttribs[Green]);
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_BLUE_SIZE, &nAttribs[Blue]);
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_ALPHA_SIZE, &nAttribs[Alpha]);
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_DEPTH_SIZE, &nAttribs[Depth]);
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_STENCIL_SIZE, &nAttribs[Stencil]);
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_SURFACE_TYPE, &nAttribs[SurfaceType]);

	return
	{
		static_cast<u8> (nAttribs[Red]),
		static_cast<u8> (nAttribs[Green]),
		static_cast<u8> (nAttribs[Blue]),
		static_cast<u8> (nAttribs[Alpha]),
		static_cast<u8> (nAttribs[Depth]),
		static_cast<u8> (nAttribs[Stencil]),
		(nAttribs[SurfaceType] & EGL_WINDOW_BIT) ? PixelFlag::Drawable : 0 |
				(nAttribs[SurfaceType] & EGL_PBUFFER_BIT) ? PixelFlag::Bitmap : 0,
		ColorType::TrueType
	};
}

Config::int_type Config::id () const
{
	if (!m_pDisplay or !m_pCfg) return -1;
	EGLint id;

	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_CONFIG_ID, &id);
	return id;
}

void Config::print ()
{
	static int value;

	std::cout << "Buffer Size: " << static_cast<u16> (m_gFormat.depth + m_gFormat.alpha)
			  << std::endl;

	std::cout << "Red Size: "   << static_cast<u16> (m_gFormat.red)   << std::endl;
	std::cout << "Green Size: " << static_cast<u16> (m_gFormat.green) << std::endl;
	std::cout << "Blue Size: "  << static_cast<u16> (m_gFormat.blue)  << std::endl;
	std::cout << "Alpha Size: " << static_cast<u16> (m_gFormat.alpha) << std::endl;
	std::cout << "Depth size: " << static_cast<u16> (m_gFormat.depth) << std::endl;

	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_CONFIG_CAVEAT, &value);

	switch (value)
	{
	case EGL_NONE:
		std::cout << "EGL_CONFIG_CAVEAT: EGL_NONE\n";
		break;
	case EGL_SLOW_CONFIG:
		std::cout << "EGL_CONFIG_CAVEAT: EGL_SLOW_CONFIG\n";
		break;
	}

	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_CONFIG_ID, &value);
	std::cout << "Config ID: " << value << std::endl;

	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_MAX_PBUFFER_WIDTH, &value);
	std::cout << "Max pbuffer width: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_MAX_PBUFFER_HEIGHT, &value);
	std::cout << "Max pbuffer height: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_MAX_PBUFFER_PIXELS, &value);
	std::cout << "Max pbuffer pixels: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_NATIVE_RENDERABLE, &value);
	std::cout << "Native renderable: " << (value ? "true" : "false") << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_NATIVE_VISUAL_ID, &value);
	std::cout << "Native visual ID: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_NATIVE_VISUAL_TYPE, &value);
	std::cout << "Native visual type: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_SAMPLE_BUFFERS, &value);
	std::cout << "Sample Buffers: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_SAMPLES, &value);
	std::cout << "Samples: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_SURFACE_TYPE, &value);
	std::cout << "Surface type: " << value << std::endl;
	eglGetConfigAttrib (m_pDisplay, m_pCfg, EGL_TRANSPARENT_TYPE, &value);
}

// ====================================================

Surface::Surface (Config const& gConf, point2u size, Type type, value_type owner)
: m_pConf   (&gConf),
  m_pHandle (EGL::createSurface (gConf, size, type, owner)),
  m_pOwner  (m_pHandle ? owner : value_type ()),
  m_eType   (type)
{ }

Surface::Surface (Surface const& obj)
: m_pConf   (obj.m_pConf),
  m_pHandle (EGL::createSurface (*m_pConf, obj.size (), obj.m_eType, obj.m_pOwner)),
  m_pOwner  (m_pHandle ? obj.m_pOwner : nullptr),
  m_eType   (obj.m_eType)
{ }

Surface& Surface::operator = (Surface&& obj) noexcept
{
	if (this == &obj) return *this;
	if (m_pHandle) eglDestroySurface (config ().display (), m_pHandle);

	m_pConf        = obj.m_pConf;
	m_pHandle      = obj.m_pHandle;
	m_pOwner       = obj.m_pOwner;

	obj.m_pConf   = nullptr;
	obj.m_pHandle = nullptr;
	obj.m_pOwner  = nullptr;

	return *this;
}

Surface::~Surface () noexcept
{
	if (m_pHandle) eglDestroySurface (config ().display (), m_pHandle);
}

void Surface::flush ()
{
	if (m_eType == Type::DoubleBuffer)
		eglSwapBuffers (config ().display (), m_pHandle);
	else
		glFlush ();
}

point2u Surface::size () const noexcept
{
	return EGL::getSize (config (), m_pHandle);
}

void Surface::scale (point2u gSize)
{
	if (!m_pConf->features ().test (Config::ScalableSurface))
	{
		eglDestroySurface (config ().display (), m_pHandle);
		m_pHandle = EGL::createSurface (config (), gSize, m_eType, m_pOwner);
	}
}

// ====================================================

Context::Context (Config const& conf, GFXVersion const& version, Context* shared)
: m_pConf        (&conf),
  m_pGC          (EGL::createGC (conf, version, shared ? shared->m_pGC : nullptr)),
  m_pDrawTarget  (),
  m_pReadTarget  (),
  m_pShared      (shared),
  m_nVersion     (m_pGC ? version : GFXVersion ())
{ }

Context::Context (Context const& obj)
: m_pConf        (obj.m_pConf),
  m_pGC          (EGL::createGC (*m_pConf, obj.m_nVersion, obj.m_pShared)),
  m_pDrawTarget  (),
  m_pReadTarget  (),
  m_pShared      (obj.m_pShared),
  m_nVersion     (m_pGC ? obj.m_nVersion : GFXVersion ())
{ }

Context& Context::operator = (Context&& obj) noexcept
{
	if (this == &obj) return *this;

	if (m_pGC)
	{
		release ();
		eglDestroyContext (config ().display (), m_pGC);
	}

	m_pDrawTarget = obj.m_pDrawTarget;
	m_pReadTarget = obj.m_pReadTarget;
	m_nVersion    = obj.m_nVersion;
	m_pConf       = obj.m_pConf;
	m_pShared     = obj.m_pShared;
	m_pGC         = obj.m_pGC;

	obj.m_nVersion    = GFXVersion ();
	obj.m_pGC         = nullptr;
	obj.m_pConf       = nullptr;
	obj.m_pShared     = nullptr;
	obj.m_pDrawTarget = nullptr;
	obj.m_pReadTarget = nullptr;

	return *this;
}

Context& Context::operator = (Context const& obj)
{
	if (this == &obj) return *this;

	if (m_pGC)
	{
		release ();
		eglDestroyContext (config ().display (), m_pGC);
	}

	m_pGC = obj.m_pGC ? EGL::createGC (obj.config (), obj.m_nVersion, obj.m_pShared) :
						 nullptr;

	m_pReadTarget = m_pDrawTarget = nullptr;
	m_nVersion    = obj.m_nVersion;
	m_pShared     = obj.m_pShared;
	m_pConf       = obj.m_pConf;

	return *this;
}

Context::~Context () noexcept
{
	if (!m_pGC) return;
	release ();
	eglDestroyContext (config ().display (), m_pGC);
}

GFXVersion Context::platformVersion () noexcept
{
	return EGL::version ();
}

bool Context::use (pointer pDraw, const_pointer pRead) noexcept
{
	if ((( pDraw and !pRead) or (pDraw and pDraw->device () != DeviceType::GL)) or
		((!pDraw and  pRead) or (pRead and pRead->device () != DeviceType::GL)))
		return false;

	m_pDrawTarget = pDraw;
	m_pReadTarget = pRead;

	if (active ()) assign ();
	return true;
}

bool Context::assign () noexcept
{
	if (!m_pDrawTarget and !m_pConf->features ().test (Config::SurfacelessContext))
		return false;

	if (!eglMakeCurrent (config ().display (),
						 m_pDrawTarget != nullptr ?
						 m_pDrawTarget->handle ().get<EGL::surface_pointer> () : nullptr,
						 m_pReadTarget != nullptr ?
						 m_pReadTarget->handle ().get<EGL::surface_pointer> () : nullptr,
						 m_pGC))
		EGL::error<EGL::MakeCurrent> ();

	acquire (this);
	return true;
}

void Context::release () noexcept
{
	if (!active () or !eglMakeCurrent (config ().display (), nullptr, nullptr, nullptr))
		return;

	acquire (nullptr);
}

void Context::flush () noexcept
{
	if (active ()) glFlush ();
}

void Context::finish () noexcept
{
	if (active ()) eglWaitGL ();
}

} } } // namespace EGL