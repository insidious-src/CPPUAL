/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#include "w32backend.h"

#ifdef   OS_WINDOWS
#include <windows.h>
#include <iostream>

namespace cppual { namespace Ui {

namespace {

typedef ::HDC  display_type;
typedef ::HWND handle_type;

inline display_type get_display () noexcept
{
    static const display_type dc = ::GetDC (handle_type ());
    return dc;
}

inline handle_type desktop_window () noexcept
{
    static const handle_type desktop_wnd = ::GetDesktopWindow ();
    return desktop_wnd;
}

} // anonymous namespace

Win32Display::Win32Display () noexcept
: IDisplay (get_display (), get_display ())
{
}

Win32Display::~Win32Display () noexcept
{
    if (native () and ::ReleaseDC (desktop_window (), native ().get<display_type> ()) != 1)
        std::cerr << "failed to release display handle: " << name () << std::endl;
}

string Win32Display::name () const noexcept
{
    return "WINDISPLAY";
}

uint Win32Display::screenCount () const noexcept
{
    return 1;
}

} } // Ui

#endif // OS_WINDOWS
