/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_WIN32_WINDOW_H_
#define CPPUAL_PLATFORM_WIN32_WINDOW_H_
#ifdef __cplusplus

#include <cppual/ui/wm.h>
#ifdef   OS_WINDOWS

namespace cppual { namespace Ui {

class DECL_EXPORT win32_window final : public platform_wnd_interface
{
public:
    win32_window  (Rect const& rect, u32 screen, IDisplay* display);
    ~win32_window ();

    string title () const;
    void   set_title (string const&) noexcept;
    void   set_shaded (bool) noexcept;
    bool   is_shaded () noexcept;
    void   set_modal (bool) noexcept;
    bool   is_modal () noexcept;
    void   set_fullscreen (bool) noexcept;
    bool   is_fullscreen () noexcept;
    void   set_maximized (bool) noexcept;
    bool   is_maximized () noexcept;
    void   set_minimized (bool) noexcept;
    bool   is_minimized () noexcept;
    void   set_visible_in_taskbar (bool) noexcept;
    bool   is_visible_in_taskbar () noexcept;
    void   set_visible_in_pager (bool) noexcept;
    bool   is_visible_in_pager () noexcept;
    void   flash (uint) noexcept;
    Rect   geometry () const;
    bool   is_mapped () const;
    void   set_owner (const_pointer);
    void   set_geometry (Rect const&);
    void   raise ();
    void   lower ();
    void   move (point2i);
    void   map ();
    void   unmap ();

    WindowFlags flags () const noexcept { return _M_eFlags; }
    void        set_flags (WindowFlags) noexcept;

    weak_window owner  () const noexcept { return shared_window (); }
    u32         screen () const noexcept { return 0; }

private:
    WindowFlags _M_eFlags;
};

} } // namespace Ui

#endif // OS_WINDOWS
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_WIN32_WINDOW_H_
