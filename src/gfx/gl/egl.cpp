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

#include <iostream>
#include <EGL/egl.h>
#include <cppual/gfx/gl/egl.h>
#include "gldef.h"

namespace cppual { namespace Graphics { namespace GL {

namespace { namespace EGL { // optimize for internal unit usage

inline GFXVersion& version () noexcept
{
	static GFXVersion eglVersion { 0, 0 };
	return eglVersion;
}

// ====================================================

enum ErrorType
{
	Initialize,
	Terminate,
	MakeCurrent,
	Create,
	Destroy
};

template <ErrorType>
void error ();

template <>
inline void error <Initialize> ()
{
	switch (eglGetError ())
	{
	case EGL_BAD_DISPLAY:
		throw bad_display ("NOT an EGL display connection");
	case EGL_NOT_INITIALIZED:
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
	case EGL_BAD_DISPLAY:
		throw bad_display ("invalid display handle");
	case EGL_NOT_INITIALIZED:
		throw not_initialized ("EGL is NOT initialized for target m_pDisplay");
	case EGL_BAD_SURFACE:
		throw bad_surface ("draw or read is NOT an EGL surface");
	case EGL_BAD_CONTEXT:
		throw bad_context ("NOT an EGL rendering context");
	case EGL_BAD_MATCH:
		throw bad_match ("draw or read are NOT compatible with the context");
	case EGL_BAD_ACCESS:
		throw bad_access ("the context is already current to some other thread");
	case EGL_BAD_NATIVE_WINDOW:
		throw bad_window ("the window underlying either draw or read is no longer valid");
	case EGL_BAD_CURRENT_SURFACE:
		throw bad_surface ("previous context has unflushed commands "
						   "and the previous surface is no longer valid");
	case EGL_BAD_ALLOC:
		throw std::bad_alloc ();
	case EGL_CONTEXT_LOST:
		throw bad_context ("a power management event has occurred");
	}
}

template <>
inline void error <Create> ()
{
	switch (eglGetError ())
	{
	case EGL_BAD_MATCH:
		throw bad_match ("rendering api not bound");
		break;
	case EGL_BAD_DISPLAY:
		throw bad_display ("invalid display handle");
		break;
	case EGL_NOT_INITIALIZED:
		throw not_initialized ("EGL is NOT initialized for target display");
		break;
	case EGL_BAD_CONFIG:
		throw bad_config ("NOT an EGL frame buffer configuration, "
						  "or does not support the current rendering API");
		break;
	case EGL_BAD_NATIVE_WINDOW:
		throw bad_window ("the window attributes do not correspond to target config "
						  "or it does not support rendering to windows");
		break;
	case EGL_BAD_CONTEXT:
		throw bad_config ("shared context is not an EGL rendering context "
						  "of the same client API type as the newly created context");
	case EGL_BAD_ATTRIBUTE:
		throw bad_config ("attribute value is NOT recognized or out of range");
		break;
	case EGL_BAD_ALLOC:
		throw std::bad_alloc ();
		break;
	}
}

// ====================================================

inline void initialize (void* dsp)
{
	if (!eglInitialize (dsp,
						&EGL::version ().major,
						&EGL::version ().minor))
		error<Initialize> ();
}

// ====================================================

constexpr uint api (API eAPI) noexcept
{
	return eAPI == API::OpenGL ?
				EGL_OPENGL_API :
				eAPI == API::OpenGLES ?
					EGL_OPENGL_ES_API :	EGL_NONE;
}

constexpr int32 api_bits (API eAPI) noexcept
{
	return eAPI == API::OpenGL ?
				EGL_OPENGL_BIT :
				eAPI == API::OpenGLES ?
					EGL_OPENGL_ES_BIT | EGL_OPENGL_ES2_BIT : EGL_NONE;
}

// ====================================================

inline void* createSurface (Config const& gConf, IResource::value_type uWndHandle, bool bDouble)
{
	cint32 nSurfaceAttribs[]
	{
		EGL_RENDER_BUFFER, bDouble ? EGL_BACK_BUFFER : EGL_SINGLE_BUFFER,
		EGL_NONE
	};

	EGLSurface pSurface = eglCreateWindowSurface (gConf.display (),
												  gConf,
												  uWndHandle,
												  nSurfaceAttribs);

	if (!pSurface) error<Create> ();
	return pSurface;
}

inline void* createPBuffer (Config const& gConf, point2u gSize)
{

	cint32 nPBufferAttribs[] =
	{
		EGL_WIDTH,          static_cast<int32> (gSize.x),
		EGL_HEIGHT,         static_cast<int32> (gSize.y),
		EGL_COLORSPACE,     EGL_COLORSPACE_sRGB,
		EGL_ALPHA_FORMAT,   EGL_ALPHA_FORMAT_NONPRE,
		EGL_TEXTURE_FORMAT, EGL_TEXTURE_RGBA,
		EGL_TEXTURE_TARGET, EGL_TEXTURE_2D,
		EGL_NONE
	};


	EGLSurface m_pPixelBuffer = eglCreatePbufferSurface (gConf.display (),
														 gConf,
														 nPBufferAttribs);

	if (!m_pPixelBuffer) error<Create> ();
	return m_pPixelBuffer;
}

inline void* createPixmap (Config const& gConf)
{

//	cint32 nSurfaceAttribs[]
//	{
//		EGL_COLORSPACE,   EGL_COLORSPACE_sRGB,
//		EGL_ALPHA_FORMAT, EGL_ALPHA_FORMAT_NONPRE,
//		EGL_NONE
//	};


	EGLSurface m_pSurface = eglCreatePixmapSurface (gConf.display (),
													gConf,
													0,
													nullptr);

	if (!m_pSurface) error<Create> ();
	return m_pSurface;
}

inline EGLContext createGC (Config const&     gConf,
							GFXVersion const& version,
							void*             pShared)
{
	cint32 nContextAttribs[]
	{
		EGL_CONTEXT_CLIENT_VERSION, version.major,
		EGL_NONE
	};

	eglBindAPI (EGL::api (gConf.api ())) ?
					true : throw bad_parameter ("the specified worthless API "
												"is NOT supported by the EGL implementation");

	EGLContext pContext = eglCreateContext (gConf.display (),
											gConf,
											pShared,
											nContextAttribs);

	if (!pContext) error<Create> ();
	return pContext;
}

} } // anonymous namespace EGL

// ====================================================

Config::Config (controller dsp, format_type gFormat, API eAPI)
: m_pDisplay (eglGetDisplay (dsp.get<EGLNativeDisplayType> ())),
  m_pCfg (),
  m_gFormat (),
  m_eAPI (eAPI)
{
	if (!m_pDisplay) throw std::logic_error ("invalid display");

	int32 nNumConfigs = 0;

	cint32 nConfigAttribs[]
	{
		EGL_RED_SIZE,        gFormat.red,
		EGL_GREEN_SIZE,      gFormat.green,
		EGL_BLUE_SIZE,       gFormat.blue,
		EGL_ALPHA_SIZE,      gFormat.alpha,
		EGL_DEPTH_SIZE,      gFormat.depth,
		EGL_STENCIL_SIZE,    gFormat.stencil,
		EGL_SURFACE_TYPE,    gFormat.flags.hasBit (PixelFlag::Drawable) ?
					EGL_WINDOW_BIT : 0 | gFormat.flags.hasBit (PixelFlag::Bitmap) ?
						EGL_PBUFFER_BIT : 0,
		EGL_RENDERABLE_TYPE, EGL::api_bits (eAPI),
		EGL_NONE
	};

	EGL::initialize (m_pDisplay);

	eglGetConfigs (m_pDisplay, &m_pCfg, 1, &nNumConfigs);
	eglChooseConfig (m_pDisplay, nConfigAttribs, &m_pCfg, 1, &nNumConfigs);

	m_gFormat = toFormat ();
}

PixelFormat Config::toFormat () const
{
	if (!m_pDisplay or !m_pCfg) return PixelFormat ();

	int nAttribs[Count];

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
		ColorType::TRUE
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

Surface::Surface (Config const& gConf, value_type uElem, bool bDouble)
: m_pConf   (&gConf),
  m_pHandle (EGL::createSurface (gConf, uElem, bDouble)),
  m_pOwner  (m_pHandle ? uElem : nullptr),
  m_eType   (bDouble ? Type::DoubleBuffer : Type::Drawable)
{ }

Surface::Surface (Config const& gConf, point2u gSize)
: m_pConf   (&gConf),
  m_pHandle (EGL::createPBuffer (gConf, gSize)),
  m_pOwner  (),
  m_eType   (Type::PBuffer)
{ }

Surface::Surface (Config const& gConf)
: m_pConf   (&gConf),
  m_pHandle (EGL::createPixmap (gConf)),
  m_pOwner  (),
  m_eType   (Type::Pixmap)
{ }

Surface::Surface (Surface const& gObj)
: m_pConf   (gObj.m_pConf),
  m_pHandle (gObj.m_eType == Type::Pixmap ?
				 EGL::createPixmap (*m_pConf) : gObj.m_eType == Type::PBuffer ?
					 EGL::createPBuffer (*m_pConf, gObj.size ()) :
					 EGL::createSurface (*m_pConf, gObj.m_pOwner,
										 gObj.m_eType == Type::DoubleBuffer)),
  m_pOwner  (m_pHandle ? gObj.m_pOwner : nullptr),
  m_eType   (gObj.m_eType)
{ }

Surface& Surface::operator = (Surface&& gObj) noexcept
{
	if (this == &gObj) return *this;
	if (m_pHandle) eglDestroySurface (m_pConf->display (), m_pHandle);

	m_pConf        = gObj.m_pConf;
	m_pHandle      = gObj.m_pHandle;
	m_pOwner       = gObj.m_pOwner;
	gObj.m_pConf   = nullptr;
	gObj.m_pHandle = nullptr;
	gObj.m_pOwner  = nullptr;

	return *this;
}

Surface::~Surface () noexcept
{
	if (m_pHandle) eglDestroySurface (m_pConf->display (), m_pHandle);
}

point2u Surface::size () const noexcept
{
	if (!m_pHandle) return point2u ();

	EGLint size[2];

	if (eglQuerySurface (m_pConf->display (), m_pHandle, EGL_WIDTH, &size[0]) == EGL_FALSE)
		return point2u ();

	if (eglQuerySurface (m_pConf->display (), m_pHandle, EGL_HEIGHT, &size[1]) == EGL_FALSE)
		return point2u ();

	return { static_cast<u16> (size[0]), static_cast<u16> (size[1]) };
}

void Surface::flush ()
{
	if (m_eType == Type::DoubleBuffer)
		eglSwapBuffers (m_pConf->display (), m_pHandle);
	else
		glFlush ();
}

void Surface::scale (point2u gSize)
{
	if (!m_pHandle or !eglDestroySurface (m_pConf->display (), m_pHandle))
		return;

	m_pHandle = m_eType == Type::PBuffer ?
					EGL::createPBuffer (*m_pConf, gSize) :
					EGL::createSurface (*m_pConf, m_pOwner, m_eType == Type::DoubleBuffer);
}

// ====================================================

Context::Context (Config const&     gConf,
				  GFXVersion const& gVersion,
				  Context*          pShared)
: m_pConf (&gConf),
  m_pGC (EGL::createGC (gConf, gVersion, pShared ? pShared->m_pGC : nullptr)),
  m_pWriteTarget (),
  m_pReadTarget (),
  m_gVersion (m_pGC ? gVersion : GFXVersion ())
{ }

Context::Context (Context const& gObj)
: m_pConf (gObj.m_pConf),
  m_pGC (EGL::createGC (*m_pConf, gObj.m_gVersion, nullptr)),
  m_pWriteTarget (),
  m_pReadTarget (),
  m_gVersion (m_pGC ? gObj.m_gVersion : GFXVersion ())
{ }

Context& Context::operator = (Context&& gObj) noexcept
{
	if (this == &gObj) return *this;

	if (m_pGC)
	{
		release ();
		eglDestroyContext (m_pConf->display (), m_pGC);
	}

	m_pWriteTarget = gObj.m_pWriteTarget;
	m_pReadTarget  = gObj.m_pReadTarget;
	m_gVersion     = gObj.m_gVersion;
	m_pConf        = gObj.m_pConf;
	m_pGC          = gObj.m_pGC;

	gObj.m_gVersion     = GFXVersion ();
	gObj.m_pGC          = nullptr;
	gObj.m_pConf        = nullptr;
	gObj.m_pWriteTarget = nullptr;
	gObj.m_pReadTarget  = nullptr;

	return *this;
}

Context& Context::operator = (Context const&)
{
	return *this;
}

Context::~Context () noexcept
{
	if (!m_pGC) return;
	release ();
	eglDestroyContext (m_pConf->display (), m_pGC);
}

GFXVersion Context::platformVersion () noexcept
{
	return EGL::version ();
}

bool Context::use (reference gWrite, const_reference gRead) noexcept
{
	if (!eglMakeCurrent (m_pConf->display (),
						 gWrite.handle ().get<void*> (),
						 gRead .handle ().get<void*> (), m_pGC))
		EGL::error<EGL::MakeCurrent> ();

	m_pReadTarget  = &gRead;
	m_pWriteTarget = &gWrite;

	acquire (this);
	return true;
}

bool Context::use () noexcept
{
	if (!eglMakeCurrent (m_pConf->display (), nullptr, nullptr, m_pGC))
		EGL::error<EGL::MakeCurrent> ();

	m_pWriteTarget = nullptr;
	m_pReadTarget  = nullptr;

	acquire (this);
	return true;
}

void Context::flush () noexcept
{
	if (active ()) glFlush ();
}

void Context::finish () noexcept
{
	if (active ()) eglWaitGL ();
}

void Context::release () noexcept
{
	if (!active () or !eglMakeCurrent (m_pConf->display (), nullptr, nullptr, nullptr))
		return;

	acquire (nullptr);
}

API Context::bound ()
{
	switch (eglQueryAPI ())
	{
	case EGL_OPENGL_ES_API:
		return API::OpenGLES;
	case EGL_OPENGL_API:
		return API::OpenGL;
	default:
		return API::Unbound;
	}
}

} } } // namespace EGL
