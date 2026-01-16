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

#ifndef CPPUAL_GFX_GLX_SURFACE_H_
#define CPPUAL_GFX_GLX_SURFACE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>

//#include <mutex>

namespace cppual { namespace gfx { namespace gl {

// ====================================================

class config ;
class surface;
class context;

// ====================================================

class config
{
public:
    typedef config                              self_type      ;
    typedef resource_handle                     handle_type    ;
    typedef ulong                               value_type     ;
    typedef handle_type  config::*              safe_bool      ;
    typedef resource_interface::connection_type connection_type;
    typedef resource_interface::format_type     format_type    ;

    enum
    {
        red,
        green,
        blue,
        alpha,
        depth,
        stencil,
        surface_type,
        color_type,
        attrib_count
    };

    enum class feature : u8
    {
        sync_control           = 1 << 0,
        mesa_sync_control      = 1 << 1,
        context_attributes_ext = 1 << 2,
        configless_context     = 1 << 3,
        surfaceless_context    = 1 << 4,
        create_robust_context  = 1 << 5,
        scalable_surface       = 1 << 6
    };

    typedef bitset<feature> feature_types;

    config (connection_type native,
            connection_type legacy,
            format_type format = format_type::default2d ());

    constexpr config (self_type const&)                noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    value_type id    () const;
    void       print () const;


    /// null config constructor
    constexpr config () noexcept
    : _M_pNative     (),
      _M_pLegacy     (),
      _M_pCfg        (),
      _M_gFormat     (),
      _M_eFeatures   ()
    { }

    constexpr connection_type native   () const noexcept { return _M_pNative  ; }
    constexpr connection_type legacy   () const noexcept { return _M_pLegacy  ; }
    constexpr feature_types   features () const noexcept { return _M_eFeatures; }
    constexpr format_type     format   () const noexcept { return _M_gFormat  ; }
    constexpr handle_type     handle   () const noexcept { return _M_pCfg     ; }
    constexpr operator        void*    () const noexcept { return _M_pCfg     ; }

    constexpr explicit operator safe_bool () const noexcept
    { return _M_pCfg ? &config::_M_pCfg : nullptr; }

    friend
    consteval bool operator == (config const& lh, config const& rh) noexcept;

private:
    format_type to_format (handle_type fb_cfg) const;

private:
    handle_type   _M_pNative  ;
    handle_type   _M_pLegacy  ;
    handle_type   _M_pCfg     ;
    format_type   _M_gFormat  ;
    feature_types _M_eFeatures;
};

// ====================================================

consteval bool operator == (config const& lh, config const& rh) noexcept
{
    return lh._M_pNative   == rh._M_pNative &&
           lh._M_pLegacy   == rh._M_pLegacy &&
           lh._M_pCfg      == rh._M_pCfg    &&
           lh._M_gFormat   == rh._M_gFormat &&
           lh._M_eFeatures == rh._M_eFeatures;
}

consteval bool operator != (config const& lh, config const& rh) noexcept
{ return !(lh == rh); }

// ====================================================

class surface : public surface_interface
{
public:
    typedef surface       self_type     ;
    typedef config        conf_value    ;
    typedef config const* conf_pointer  ;
    typedef config const& conf_reference;


    surface (self_type &&) noexcept = default;
    surface (self_type const&);
    surface& operator = (self_type &&) noexcept;
    ~surface ();

    surface (conf_reference cfg ,
             point2u        size,
             handle_type    wnd ,
             surface_type   type = surface_type::double_buffer);

    void           set_parent (shared_surface /*parent*/) noexcept { }
    shared_surface parent     () const noexcept { return shared_surface (); }
    point2i        position   () const noexcept { return { }; }

    void    paint_background (color clr);
    point2u size  () const noexcept;
    void    scale (point2u size);
    void    flush ();

    conf_reference  configuration () const noexcept { return _M_pConf;           }
    device_backend  device        () const noexcept { return device_backend::gl; }
    handle_type     handle        () const noexcept { return _M_pHandle;         }
    connection_type connection    () const noexcept { return _M_pConf.native (); }
    format_type     format        () const noexcept { return _M_pConf.format (); }
    surface_type    type          () const noexcept { return _M_eType;           }
    shared_context  context       () const noexcept { return _M_pContext;        }

private:
    conf_value     _M_pConf   ;
    handle_type    _M_pHandle ;
    handle_type    _M_pWnd    ;
    surface_type   _M_eType   ;
    shared_context _M_pContext;

    friend class context;
};

// ====================================================

class context : public context_interface
{
public:
    typedef context       self_type     ;
    typedef config        conf_value    ;
    typedef config const* conf_pointer  ;
    typedef config const& conf_reference;


    context (self_type &&) noexcept = default;
    context (self_type const&);
    self_type& operator = (self_type &&) noexcept;
    self_type& operator = (self_type const&);
    ~context () noexcept;

    context (conf_reference conf, version_type const& version = default_version ());

    static version_type platform_version () noexcept;

    bool use     (shared_surface, shared_surface) noexcept;
    bool assign  (shared_context cntxt) noexcept;
    void flush   () noexcept;
    void finish  () noexcept;
    void release () noexcept;

    constexpr conf_reference configuration () const noexcept
    { return _M_pConf; }

    shared_surface  readable      () const noexcept { return  _M_pReadTarget;       }
    shared_surface  drawable      () const noexcept { return  _M_pDrawTarget;       }
    version_type    version       () const noexcept { return  _M_nVersion;          }
    device_backend  device        () const noexcept { return  device_backend::gl;   }
    handle_type     handle        () const noexcept { return  _M_pGC;               }
    connection_type connection    () const noexcept { return  _M_pConf.native  ();  }
    format_type     format        () const noexcept { return  _M_pConf.format  ();  }

    constexpr static version_type default_version () noexcept
    { return version_type { 3, 0 }; }

private:
    conf_value     _M_pConf      ;
    handle_type    _M_pGC        ;
    shared_surface _M_pDrawTarget;
    shared_surface _M_pReadTarget;
    version_type   _M_nVersion   ;
};

// ====================================================

} } } // namespace gl

#endif // __cplusplus
#endif // CPPUAL_GFX_GLX_SURFACE_H_
