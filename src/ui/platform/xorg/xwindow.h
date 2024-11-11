/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_X_SURFACE_H_
#define CPPUAL_PLATFORM_X_SURFACE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>
#include <cppual/ui/wm.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xbackend.h"

namespace cppual { namespace ui {

// =========================================================

class SHARED_API xcb_window final : public platform_wnd_interface
{
public:
    typedef x::display_data const& data_const_reference;

    xcb_window () = delete;
    xcb_window (rect const& rect, u32 screen, shared_display display) noexcept;
    ~xcb_window() noexcept;

    string_type title () const;
    void        set_title (string_type const&);
    void        set_shaded (bool);
    bool        is_shaded () const;
    void        set_modal (bool);
    bool        is_modal () const;
    void        set_fullscreen (bool);
    bool        is_fullscreen () const;
    void        set_maximized (bool);
    bool        is_maximized () const;
    void        set_minimized (bool);
    bool        is_minimized () const;
    void        set_visible_in_taskbar (bool);
    bool        is_visible_in_taskbar () const;
    void        set_visible_in_pager (bool);
    bool        is_visible_in_pager () const;
    void        flash (uint);
    rect        geometry () const;
    bool        is_mapped () const;
    void        set_owner (wnd_const_reference);
    void        set_geometry (rect const&);
    void        raise ();
    void        lower ();
    void        move (point2i);
    void        map ();
    void        map_minimized ();
    void        map_maximized ();
    void        unmap ();
    void        set_flags (window_flags);
    void        keep_above (bool);
    bool        is_above () const;
    void        keep_below (bool);
    bool        is_below () const;

    window_flags flags  () const noexcept { return _M_eFlags     ; }
    wnd_pointer  owner  () const noexcept { return  wnd_pointer(); }
    u32          screen () const noexcept { return _M_uScreen    ; }

    inline x::display_type* display () const noexcept
    { return connection ()->native<x::display_type> (); }

    inline data_const_reference data () const noexcept
    { return static_cast<xcb_display&> (*connection ()).data (); }

private:
    window_flags    _M_eFlags ;
    u32             _M_uScreen;
    x::screen_type* _M_pScreen;
};

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_X_SURFACE_H_
