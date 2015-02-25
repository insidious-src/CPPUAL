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

#include <mutex>
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
	typedef pointer  Config::*              safe_bool;
	typedef typename IResource::controller  controller;
	typedef typename IResource::format_type format_type;

	enum
	{
		Red,
		Green,
		Blue,
		Alpha,
		Depth,
		Stencil,
		SurfaceType,
		AttribCount
	};

	enum Feature
	{
		SyncControl          = 1 << 0,
		ContextAttributesExt = 1 << 1,
		ConfiglessContext    = 1 << 2,
		SurfacelessContext   = 1 << 3,
		CreateRobustContext  = 1 << 4,
		ScalableSurface      = 1 << 5
	};

	typedef BitSet<Feature> Features;

	int_type id    () const;
	void     print ();

	Config (controller  display = defaultDisplay (),
			format_type format  = format_type::default2D ());

	constexpr Config () noexcept
	: m_pDisplay (), m_pCfg (), m_gFormat (), m_eFeatures ()
	{ }

	constexpr Config (Config const&) noexcept = default;
	inline    Config& operator = (Config const&) noexcept = default;

	constexpr controller  display  ()   const noexcept { return m_pDisplay;  }
	constexpr Features    features ()   const noexcept { return m_eFeatures; }
	constexpr format_type format   ()   const noexcept { return m_gFormat;   }
	constexpr void*       operator ()() const noexcept { return m_pCfg;      }
	constexpr operator    void*    ()   const noexcept { return m_pCfg;      }

	constexpr explicit operator safe_bool () const noexcept
	{ return m_pCfg ? &Config::m_pCfg : nullptr; }

	constexpr static controller defaultDisplay  () noexcept
	{ return nullptr; }

	friend
	constexpr bool operator == (Config const& lh, Config const& rh) noexcept;

private:
	format_type toFormat () const;

private:
	pointer     m_pDisplay;
	pointer     m_pCfg;
	format_type m_gFormat;
	Features    m_eFeatures;
};

// ====================================================

constexpr bool operator == (Config const& lh, Config const& rh) noexcept
{ return lh.m_pDisplay == rh.m_pDisplay and lh.m_pCfg == rh.m_pCfg; }

constexpr bool operator != (Config const& lh, Config const& rh) noexcept
{ return !(lh == rh); }

// ====================================================

class Surface : public IPixelSurface
{
public:
	typedef Config const* conf_pointer;
	typedef Config const& conf_reference;
	typedef void*         pointer;

	Surface (Surface const&);
	Surface (Surface&&) noexcept = default;
	Surface (Config const&, point2u size, Type type, value_type owner = value_type ());
	Surface& operator = (Surface&&) noexcept;
	~Surface () noexcept;

	point2u size  () const noexcept;
	void    scale (point2u size);
	void    flush ();

	conf_reference config     () const noexcept { return *m_pConf;             }
	DeviceType     device     () const noexcept { return  DeviceType::GL;      }
	value_type     handle     () const noexcept { return  m_pHandle;           }
	controller     connection () const noexcept { return  m_pConf->display (); }
	format_type    format     () const noexcept { return  m_pConf->format  (); }
	Type           type       () const noexcept { return  m_eType;             }

private:
	conf_pointer m_pConf;
	pointer      m_pHandle;
	value_type   m_pOwner;
	Type         m_eType;
};

// ====================================================

class Context : public IDeviceContext
{
public:
	typedef Config const* conf_pointer;
	typedef Config const& conf_reference;

	Context (Context const&);
	Context& operator = (Context&&) noexcept;
	Context& operator = (Context const&);
	Context (Context&&) noexcept = default;
	~Context () noexcept;

	Context (Config     const& config,
			 GFXVersion const& version = defaultVersion (),
			 Context*          shared  = nullptr);

	static GFXVersion platformVersion () noexcept;

	bool use     (pointer, const_pointer) noexcept;
	bool assign  () noexcept;
	void flush   () noexcept;
	void finish  () noexcept;
	void release () noexcept;

	conf_reference config     () const noexcept { return *m_pConf;             }
	const_pointer  readable   () const noexcept { return  m_pReadTarget;       }
	pointer        drawable   () const noexcept { return  m_pDrawTarget;       }
	GFXVersion     version    () const noexcept { return  m_nVersion;          }
	DeviceType     device     () const noexcept { return  DeviceType::GL;      }
	value_type     handle     () const noexcept { return  m_pGC;               }
	controller     connection () const noexcept { return  m_pConf->display (); }
	format_type    format     () const noexcept { return  m_pConf->format  (); }

	static constexpr GFXVersion defaultVersion () noexcept
	{ return GFXVersion { 3, 0 }; }

private:
	conf_pointer  m_pConf;
	void*         m_pGC;
	pointer       m_pDrawTarget;
	const_pointer m_pReadTarget;
	Context*      m_pShared;
	GFXVersion    m_nVersion;
};

// ====================================================

class ContextMutex
{
public:
	ContextMutex (Context& context) noexcept
	: m_context (context)
	{ }

	void lock ()
	{
		m_mutex.lock ();
		m_context.assign ();
	}

	bool try_lock ()
	{
		return m_mutex.try_lock () ? m_context.assign (), true : false;
	}

	void unlock ()
	{
		m_context.release ();
		m_mutex.unlock ();
	}

private:
	std::mutex m_mutex;
	Context&   m_context;
};

} } } // namespace EGL

#endif // __cplusplus
#endif // CPPUAL_GFX_EGL_SURFACE_H_
