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

#ifndef CPPUAL_GFX_EGL_SURFACE_H_
#define CPPUAL_GFX_EGL_SURFACE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>

namespace cppual { namespace Graphics { namespace GL {

class Config;
class Surface;
class Context;

struct bad_match       : public std::logic_error { using std::logic_error::logic_error; };
struct bad_display     : public std::logic_error { using std::logic_error::logic_error; };
struct bad_surface     : public std::logic_error { using std::logic_error::logic_error; };
struct bad_parameter   : public std::logic_error { using std::logic_error::logic_error; };
struct bad_window      : public std::logic_error { using std::logic_error::logic_error; };
struct bad_context     : public std::logic_error { using std::logic_error::logic_error; };
struct bad_config      : public std::logic_error { using std::logic_error::logic_error; };
struct bad_attrib      : public std::logic_error { using std::logic_error::logic_error; };
struct bad_access      : public std::logic_error { using std::logic_error::logic_error; };
struct context_lost    : public std::logic_error { using std::logic_error::logic_error; };
struct not_initialized : public std::logic_error { using std::logic_error::logic_error; };

// ====================================================

class Config
{
public:
	typedef void*                           pointer;
	typedef int32                           int_type;
	typedef pointer     Config::*           safe_bool;
	typedef typename IResource::controller  controller;
	typedef typename IResource::format_type format_type;

	enum
	{
		Red = 0,
		Green,
		Blue,
		Alpha,
		Depth,
		Stencil,
		SurfaceType,
		Count
	};

	int_type id () const;
	void     print ();

	Config (controller display,
			format_type const& = format_type::default2D ());

	constexpr Config () noexcept
	: m_pDisplay (), m_pCfg (), m_gFormat ()
	{ }

	constexpr Config (Config const&) noexcept = default;
	inline    Config& operator = (Config const&) noexcept = default;

	constexpr controller  display () const noexcept { return m_pDisplay; }
	constexpr format_type format  () const noexcept { return m_gFormat;  }
	constexpr operator    void*   () const noexcept { return m_pCfg;     }

	constexpr explicit operator safe_bool () const noexcept
	{ return m_pCfg ? &Config::m_pCfg : nullptr; }

	friend
	constexpr bool operator == (Config const& lh, Config const& rh) noexcept;

private:
	format_type toFormat () const;

private:
	pointer     m_pDisplay;
	pointer     m_pCfg;
	PixelFormat m_gFormat;
};

// ====================================================

constexpr bool operator == (Config const& lh, Config const& rh) noexcept
{ return lh.m_pDisplay == rh.m_pDisplay and lh.m_pCfg == rh.m_pCfg; }

constexpr bool operator != (Config const& lh, Config const& rh) noexcept
{ return !(lh == rh); }

// ====================================================

class Surface : public IPixelBuffer
{
public:
	typedef Config const* conf_pointer;
	typedef Config        conf_type;
	typedef void*         pointer;

	Surface () = delete;
	Surface (Surface const&);
	Surface (Surface&&) noexcept = default;
	Surface (Config const&, value_type window);
	Surface& operator = (Surface&&) noexcept;
	~Surface ();

	point2i size  () const noexcept;
	void    flush ();

	DeviceType  type       () const noexcept { return DeviceType::EGL; }
	value_type  handle     () const noexcept { return m_pHandle; }
	int         colormap   () const noexcept { return int (); }
	controller  connection () const noexcept { return m_pConf->display (); }
	format_type format     () const noexcept { return m_pConf->format  (); }
	conf_type   config     () const noexcept { return *m_pConf; }

private:
	conf_pointer m_pConf;
	pointer      m_pHandle;

	void dispose () noexcept;
};

// ====================================================

class Context : public IDeviceContext
{
public:
	typedef Config const* conf_pointer;

	enum API
	{
		Unbound = 0,
		OpenGL,
		OpenGLES
	};

	static bool bind  (API);
	static API  bound ();

	Context () = delete;
	Context (Context const&);
	Context& operator = (Context&&) noexcept;
	Context& operator = (Context const&);
	Context (Context&&) noexcept = default;
	~Context () noexcept;

	Context (Config     const& config,
			 GFXVersion const& version = defaultVersion (),
			 Context*          shared  = nullptr);

	static GFXVersion platformVersion () noexcept;
	static controller defaultDisplay  () noexcept;

	bool use (reference, const_reference) noexcept;
	bool use () noexcept;
	void flush () noexcept;
	void finish () noexcept;
	void release () noexcept;

	const_pointer readable   () const noexcept { return m_pReadTarget;       }
	pointer       writable   () const noexcept { return m_pWriteTarget;      }
	GFXVersion    version    () const noexcept { return m_gVersion;          }
	DeviceType    type       () const noexcept { return DeviceType::EGL;     }
	value_type    handle     () const noexcept { return m_pGC;               }
	controller    connection () const noexcept { return m_pConf->display (); }
	format_type   format     () const noexcept { return m_pConf->format ();  }

	static constexpr GFXVersion defaultVersion () noexcept
	{ return { 3, 0 }; }

	Config config () const noexcept
	{ return *m_pConf; }

private:
	conf_pointer  m_pConf;
	void*         m_pGC;
	pointer       m_pWriteTarget;
	const_pointer m_pReadTarget;
	GFXVersion    m_gVersion;
};

// ====================================================

template <typename CharT, typename Traits>
std::basic_ostream<CharT, Traits>&
operator << (std::basic_ostream<CharT, Traits>& os, Context::API api)
{
	switch (api)
	{
	case Context::OpenGL:
		return os << "OpenGL";
	case Context::OpenGLES:
		return os << "OpenGLES";
	default:
		return os << "none";
	}
}

} } } // namespace EGL

#endif // __cplusplus
#endif // CPPUAL_GFX_EGL_SURFACE_H_
