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

#ifndef CPPUAL_UI_VIRTUAL_SURFACE_H_
#define CPPUAL_UI_VIRTUAL_SURFACE_H_
#ifdef __cplusplus

#include <cppual/ui/wm.h>
#include <cppual/string.h>

namespace cppual { namespace ui {

class SHARED_API proxy_renderable : public platform_wnd_interface
{
public:
    typedef proxy_renderable       self_type;
    typedef platform_wnd_interface base_type;

    proxy_renderable () = delete;
    proxy_renderable (base_type*) noexcept;
    proxy_renderable (self_type const&) noexcept;
    proxy_renderable (wnd_const_reference owner, rect const& rect) noexcept;
    proxy_renderable& operator = (base_type*) noexcept;
    proxy_renderable& operator = (self_type const&) noexcept;

    string_type title () const noexcept { return string_type (); }
    void        set_title (string_type const&) noexcept { }
    void        set_shaded (bool) noexcept { }
    bool        is_shaded () const noexcept { return false; }
    void        set_modal (bool) noexcept { }
    bool        is_modal () const noexcept { return false; }
    void        set_fullscreen (bool) noexcept { }
    bool        is_fullscreen () const noexcept { return false; }
    void        set_maximized (bool) noexcept { }
    bool        is_maximized () const noexcept { return false; }
    void        set_minimized (bool) noexcept { }
    bool        is_minimized () const noexcept { return false; }
    void        set_visible_in_taskbar (bool) noexcept { }
    bool        is_visible_in_taskbar () const noexcept { return false; }
    void        set_visible_in_pager (bool) noexcept { }
    bool        is_visible_in_pager () const noexcept { return false; }
    void        setMimimumSize (point2u) noexcept { }
    void        setMaximumSize (point2u) noexcept { }
    void        flash (uint) noexcept { }
    void        set_geometry (rect const&) noexcept;
    void        set_owner (wnd_const_reference) noexcept;
    void        move (point2i) noexcept;
    void        map () noexcept;
    void        map_minimized () noexcept { }
    void        map_maximized () noexcept { }
    void        unmap () noexcept;
    void        keep_above (bool) { }
    bool        is_above () const { return false; }
    void        keep_below (bool) { }
    bool        is_below () const { return false; }

    window_flags flags () const { return window_flags (); }
    void         set_flags (window_flags) { }

    constexpr ~proxy_renderable     ()       noexcept { unmap ();             }
    constexpr wnd_pointer owner     () const noexcept { return _M_pParent;    }
    constexpr rect        geometry  () const noexcept { return _M_gRect;      }
    constexpr bool        is_mapped () const noexcept { return _M_bIsVisible; }

    constexpr u32 screen () const noexcept
    { return _M_pParent != nullptr ? _M_pParent->screen () : 0; }

    constexpr void raise () noexcept
    { if (_M_pParent != nullptr) _M_pParent->raise (); }

    constexpr void lower () noexcept
    { if (_M_pParent != nullptr) _M_pParent->lower (); }

private:
    wnd_pointer _M_pParent   ;
    rect        _M_gRect     ;
    bool        _M_bIsVisible;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIRTUAL_SURFACE_H_
