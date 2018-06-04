/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#define  API_EXPORT
#include "xbackend.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <iostream>
#include <X11/Xlib-xcb.h>

namespace cppual { namespace Ui {

namespace { namespace Xcb { // optimize for internal unit usage

typedef ::xcb_connection_t display_type;
typedef ::Display          legacy_type;

inline XDisplay::handle_type x11_connection (cchar* pName) noexcept
{
    static XDisplay::handle_type pDisplay    = nullptr;
    static cchar*                pCachedName = "";

    return pDisplay and pName == pCachedName ?
                pDisplay : pDisplay = ::XOpenDisplay (pCachedName = pName);
}

inline XDisplay::handle_type get_connection (cchar* pName) noexcept
{
    return ::XGetXCBConnection (x11_connection (pName).get<legacy_type> ());
}

} } // anonymous namespace Xcb

XDisplay::XDisplay (string_type const& name) noexcept
: IDisplay (Xcb::get_connection (name.c_str()), Xcb::x11_connection (name.c_str())),
  m_gName  (native () ? name : nullptr)
{
    if (!native ()) return;

    // take ownership of the event queue
    ::XSetEventQueueOwner (legacy<Xcb::legacy_type> (), XCBOwnsEventQueue);

    if (::xcb_connection_has_error (native<Xcb::display_type> ()))
    {
        std::cout << "error when connecting to display: "
                  << name << std::endl;
    }
}

XDisplay::~XDisplay ()
{
    if (native ()) ::XCloseDisplay (legacy<Xcb::legacy_type> ());
}

uint XDisplay::screenCount () const noexcept
{
    return static_cast<uint>
            (::xcb_setup_roots_length (::xcb_get_setup (native<Xcb::display_type> ())));
}

void XDisplay::flush () noexcept
{
    ::xcb_flush (native<Xcb::display_type> ());
}

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
