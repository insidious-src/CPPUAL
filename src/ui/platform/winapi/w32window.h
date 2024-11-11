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

#ifndef CPPUAL_PLATFORM_WIN32_WINDOW_H_
#define CPPUAL_PLATFORM_WIN32_WINDOW_H_
#ifdef __cplusplus

#include <cppual/ui/wm.h>
#ifdef   OS_WINDOWS

namespace cppual { namespace ui {

class DECL_EXPORT win32_window final : public platform_wnd_interface
{
public:
    win32_window  (rect const& rect, u32 screen, shared_display display);
    ~win32_window ();

    string_type title () const;
    void   set_title (string_type const&) noexcept;
    void   set_shaded (bool) noexcept;
    bool   is_shaded () const noexcept;
    void   set_modal (bool) noexcept;
    bool   is_modal () const noexcept;
    void   set_fullscreen (bool) noexcept;
    bool   is_fullscreen () const noexcept;
    void   set_maximized (bool) noexcept;
    bool   is_maximized () const noexcept;
    void   set_minimized (bool) noexcept;
    bool   is_minimized () const noexcept;
    void   set_visible_in_taskbar (bool) noexcept;
    bool   is_visible_in_taskbar () const noexcept;
    void   set_visible_in_pager (bool) noexcept;
    bool   is_visible_in_pager () const noexcept;
    void   flash (uint) noexcept;
    rect   geometry () const;
    bool   is_mapped () const;
    void   set_owner (const_pointer);
    void   set_geometry (rect const&);
    void   raise ();
    void   lower ();
    void   move (point2i);
    void   map ();
    void   unmap ();
    void   map_minimized();
    void   map_maximized();
    void   keep_above(bool);
    bool   is_above() const;
    void   keep_below(bool);
    bool   is_below() const;

    window_flags flags () const noexcept { return _M_eFlags; }
    void         set_flags (window_flags) noexcept;

    weak_window  owner  () const noexcept { return shared_window (); }
    u32          screen () const noexcept { return 0; }

private:
    window_flags _M_eFlags;
};

} } // namespace Ui

#endif // OS_WINDOWS
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_WIN32_WINDOW_H_
