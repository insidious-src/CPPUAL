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

#include "xbackend.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <iostream>
#include <X11/Xlib-xcb.h>

namespace cppual { namespace Ui {

typedef ::xcb_connection_t display_type;
typedef ::Display          legacy_type;

namespace { namespace Xcb { // optimize for internal unit usage

inline Connection x11_connection (cchar* pName) noexcept
{
    static Connection pDisplay    = nullptr;
    static cchar*     pCachedName = "";

    return pDisplay and pName == pCachedName ?
                pDisplay : pDisplay = ::XOpenDisplay (pCachedName = pName);
}

inline Connection get_connection (cchar* pName) noexcept
{
    return ::XGetXCBConnection (x11_connection (pName).get<legacy_type> ());
}

} } // anonymous namespace Xcb

XDisplay::XDisplay (cchar* pName) noexcept
: IDisplay (Xcb::get_connection (pName), Xcb::x11_connection (pName)),
  m_gName  (native () ? pName : nullptr)
{
    if (!native ()) return;

    // take ownership of the event queue
    ::XSetEventQueueOwner (gl ().get<legacy_type> (), XCBOwnsEventQueue);

    if (::xcb_connection_has_error (native ().get<display_type> ()))
    {
        std::cout << "error when connecting to display: "
                  << pName << std::endl;
    }
}

XDisplay::~XDisplay ()
{
    if (native ()) ::XCloseDisplay (gl ().get<legacy_type> ());
}

uint XDisplay::screenCount () const noexcept
{
    return static_cast<uint>
            (::xcb_setup_roots_length (::xcb_get_setup (native ().get<display_type> ())));
}

void XDisplay::flush () noexcept
{
    ::xcb_flush (native ().get<display_type> ());
}

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
