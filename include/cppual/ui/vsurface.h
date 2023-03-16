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

#ifndef CPPUAL_GFX_VIRTUAL_SURFACE_H_
#define CPPUAL_GFX_VIRTUAL_SURFACE_H_
#ifdef __cplusplus

#include <cppual/ui/wm.h>
#include <cppual/gfx/gl/texture.h>
#include <cppual/string.h>

namespace cppual { namespace ui {

class SHARED_API proxy_renderable final : public platform_wnd_interface
{
public:
    proxy_renderable () = delete;
    proxy_renderable (platform_wnd_interface*) noexcept;
    proxy_renderable (proxy_renderable const&) noexcept;
    proxy_renderable (shared_window const& owner, rect const& rect) noexcept;
    proxy_renderable& operator = (platform_wnd_interface*) noexcept;
    proxy_renderable& operator = (proxy_renderable const&) noexcept;

    string title () const noexcept { return string (); }
    void   set_title (string const&) noexcept { }
    void   set_shaded (bool) noexcept { }
    bool   is_shaded () const noexcept { return false; }
    void   set_modal (bool) noexcept { }
    bool   is_modal () const noexcept { return false; }
    void   set_fullscreen (bool) noexcept { }
    bool   is_fullscreen () const noexcept { return false; }
    void   set_maximized (bool) noexcept { }
    bool   is_maximized () const noexcept { return false; }
    void   set_minimized (bool) noexcept { }
    bool   is_minimized () const noexcept { return false; }
    void   set_visible_in_taskbar (bool) noexcept { }
    bool   is_visible_in_taskbar () const noexcept { return false; }
    void   set_visible_in_pager (bool) noexcept { }
    bool   is_visible_in_pager () const noexcept { return false; }
    void   setMimimumSize (point2u) noexcept { }
    void   setMaximumSize (point2u) noexcept { }
    void   flash (uint) noexcept { }
    void   set_geometry (rect const&) noexcept;
    void   set_owner (const_pointer) noexcept;
    void   move (point2i) noexcept;
    void   map () noexcept;
    void   map_minimized () noexcept { }
    void   map_maximized () noexcept { }
    void   unmap () noexcept;
    void   keep_above (bool) { }
    bool   is_above () const { return false; }
    void   keep_below (bool) { }
    bool   is_below () const { return false; }

    window_flags flags () const { return window_flags (); }
    void        set_flags (window_flags) { }

    inline ~proxy_renderable    ()       noexcept { unmap ();            }
    inline weak_window owner    () const noexcept { return _M_pParent;    }
    inline rect        geometry () const noexcept { return _M_gRect;      }
    inline bool        is_mapped () const noexcept { return _M_bIsVisible; }

    inline u32 screen () const noexcept
    { return _M_pParent != nullptr ? _M_pParent->screen () : 0; }

    inline void raise () noexcept
    { if (_M_pParent != nullptr) _M_pParent->raise (); }

    inline void lower () noexcept
    { if (_M_pParent != nullptr) _M_pParent->lower (); }

private:
    shared_window _M_pParent;
    rect          _M_gRect;
    bool          _M_bIsVisible;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_VIRTUAL_SURFACE_H_
