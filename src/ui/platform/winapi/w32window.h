/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_WIN32_WINDOW_H_
#define CPPUAL_PLATFORM_WIN32_WINDOW_H_
#ifdef __cplusplus

#include <cppual/ui/wm.h>
#ifdef   OS_WINDOWS

namespace cppual { namespace Ui {

class Win32Window final : public IWindow
{
public:
    Win32Window  (Rect const& rect, u32 screen, IDisplay* display);
    ~Win32Window ();

    string title () const;
    void   setTitle (string const&) noexcept;
    void   setShaded (bool) noexcept;
    bool   isShaded () noexcept;
    void   setModal (bool) noexcept;
    bool   isModal () noexcept;
    void   setFullscreen (bool) noexcept;
    bool   isFullscreen () noexcept;
    void   setMaximized (bool) noexcept;
    bool   isMaximized () noexcept;
    void   setMinimized (bool) noexcept;
    bool   isMinimized () noexcept;
    void   setVisibleInTaskbar (bool) noexcept;
    bool   isVisibleInTaskbar () noexcept;
    void   setVisibleInPager (bool) noexcept;
    bool   isVisibleInPager () noexcept;
    void   flash (uint) noexcept;
    Rect   geometry () const;
    bool   isMapped () const;
    void   setParent (const_reference, point2i);
    void   setGeometry (Rect const&);
    void   raise ();
    void   lower ();
    void   move (point2i);
    void   map ();
    void   unmap ();

    WindowFlags flags () const noexcept { return m_eFlags; }
    void        setFlags (WindowFlags) noexcept;

    weak_window parent () const noexcept { return shared_window (); }
    u32         screen () const noexcept { return 0; }

private:
    WindowFlags m_eFlags;
};

} } // namespace Ui

#endif // OS_WINDOWS
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_WIN32_WINDOW_H_
