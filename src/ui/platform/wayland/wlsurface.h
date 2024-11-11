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

#ifndef CPPUAL_UI_PLATFORM_WAYLAND_WLSURFACE_H
#define CPPUAL_UI_PLATFORM_WAYLAND_WLSURFACE_H

#include <cppual/ui/wm.h>
#include <cppual/gfx/draw.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <wayland-egl.h>
//#include <xdg-shell-client-protocol.h>
#include <xkbcommon/xkbcommon.h>

namespace cppual { namespace ui {

class wl_surface final : public platform_wnd_interface
{
public:
    wl_surface  () = delete;
    ~wl_surface () noexcept { }

    wl_surface (rect const& gRect, u32 nScreen, shared_display pDisplay) noexcept
    : platform_wnd_interface (pDisplay, handle_type ()),
      _M_gRect               (gRect),
      _M_uScreen             (nScreen)
    { }

    bool is_mapped     () const noexcept { return false; }
    void set_owner     (wnd_const_reference) noexcept { }
    void move          (point2i) noexcept { }
    void set_geometry  (rect const&) noexcept { }
    void raise         () noexcept { }
    void lower         () noexcept { }
    void map           () noexcept { }
    void map_minimized () noexcept { }
    void map_maximized () noexcept { }
    void unmap         () noexcept { }

    wnd_pointer owner    () const noexcept { return wnd_pointer(); }
    rect        geometry () const noexcept { return _M_gRect      ; }
    u32         screen   () const noexcept { return _M_uScreen    ; }

    window_flags flags () const { return window_flags (); }
    void set_flags (window_flags) { }

    string_type title () const { return string_type (); }
    void set_title (string_type const&) { }
    void set_shaded (bool) { }
    bool is_shaded () const { return false; }
    void set_modal (bool) { }
    bool is_modal () const { return false; }
    void set_fullscreen (bool) { }
    bool is_fullscreen () const { return false; }
    void set_maximized (bool) { }
    bool is_maximized () const { return false; }
    void set_minimized (bool) { }
    bool is_minimized () const { return false; }
    void set_visible_in_taskbar (bool) { }
    bool is_visible_in_taskbar () const { return false; }
    void set_visible_in_pager (bool) { }
    bool is_visible_in_pager () const { return false; }
    void flash (uint) { }
    void keep_above (bool) { }
    bool is_above () const { return false; }
    void keep_below (bool) { }
    bool is_below () const { return false; }

private:
    rect _M_gRect;
    u32  _M_uScreen;
};

} } // namespace Ui

#endif // OS_GNU_LINUX
#endif // CPPUAL_UI_PLATFORM_WAYLAND_WLSURFACE_H