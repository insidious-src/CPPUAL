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

#ifndef CPPUAL_COMPUTE_DEVICE_SURFACE_H_
#define CPPUAL_COMPUTE_DEVICE_SURFACE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>

using cppual::Graphics::IResource;
using cppual::Graphics::DeviceType;

namespace cppual { namespace Compute {

class Config;
class DeviceSurface;

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

class DeviceSurface : public Graphics::IPixelSurface
{
public:
    typedef Config const* conf_pointer;
    typedef Config const& conf_reference;
    typedef void*         pointer;

    DeviceSurface (DeviceSurface const&);
    DeviceSurface (DeviceSurface&&) noexcept = default;
    DeviceSurface (conf_reference, point2u size, Type type, value_type owner = value_type ());
    DeviceSurface& operator = (DeviceSurface&&) noexcept;
    ~DeviceSurface () noexcept;

    point2u size  () const noexcept;
    void    scale (point2u size);
    void    flush ();

    conf_reference config     () const noexcept { return *m_pConf;             }
    DeviceType     device     () const noexcept { return  DeviceType::Compute; }
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

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_DEVICE_SURFACE_H_
