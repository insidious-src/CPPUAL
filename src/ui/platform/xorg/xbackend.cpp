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

#define  API_EXPORT
#include "xbackend.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

//#include <cppual/ui/platforms/xcb/xcb_def.h>

#include <iostream>

namespace cppual { namespace ui {

namespace { // optimize for internal unit usage

inline xcb_display::handle_type x11_connection (cchar* pName) noexcept
{
    static xcb_display::handle_type pDisplay    = nullptr;
    static cchar*                   pCachedName = "";

    return pDisplay && pName == pCachedName ?
                                pDisplay :
                                pDisplay = ::XOpenDisplay (pCachedName = pName);
}

inline x::display_type* get_connection (cchar* pName) noexcept
{
    return ::XGetXCBConnection (x11_connection (pName).get<x::legacy_type> ());
}

} // anonymous namespace

xcb_display::xcb_display (string_type const& name)
: display_interface (get_connection (name.c_str()), x11_connection (name.c_str())),
  _M_gName (native () ? name : nullptr),
  _M_data  (native<x::display_type> ())
{
    if (!native ()) return;

    // take ownership of the event queue
    ::XSetEventQueueOwner (legacy<x::legacy_type> (), XCBOwnsEventQueue);

    if (::xcb_connection_has_error (native<x::display_type> ()))
    {
        std::cerr << "error when connecting to display: "
                  << name << std::endl;
    }

    _M_prevAtoms.reserve (15);
}

xcb_display::~xcb_display ()
{
    if (native ()) ::XCloseDisplay (legacy<x::legacy_type> ());
}

uint xcb_display::screen_count () const noexcept
{
    return static_cast<uint>
            (::xcb_setup_roots_length (::xcb_get_setup (native<x::display_type> ())));
}

void xcb_display::flush () noexcept
{
    ::xcb_flush (native<x::display_type> ());
}

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
