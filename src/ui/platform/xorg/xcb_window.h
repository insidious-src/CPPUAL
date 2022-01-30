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

#ifndef CPPUAL_PLATFORM_XCB_WINDOW_H_
#define CPPUAL_PLATFORM_XCB_WINDOW_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <string>
#include <memory>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xcb_def.h"
#include "xcb_iterator.h"

#include <xcb/xcb.h>

namespace cppual { namespace ui { namespace xcb {

typedef string                                     string_type;
typedef handle_ptr<::xcb_intern_atom_reply_t>           intern_ptr ;
typedef handle_ptr<::xcb_get_property_reply_t>          prop_ptr   ;
typedef handle_ptr<::xcb_get_geometry_reply_t>          geo_ptr    ;
typedef handle_ptr<::xcb_get_window_attributes_reply_t> attrib_ptr ;

enum
{
    Move   = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
    Resize = Move | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
    Stack  = XCB_CONFIG_WINDOW_STACK_MODE
};

// =========================================================

inline intern_ptr internAtomHelper (display_type* conn,
                                    string_type const& str,
                                    bool only_if_exists = false)
{
    auto cookie = ::xcb_intern_atom (conn,
                                     only_if_exists,
                                     static_cast<u16> (str.length()),
                                     str.c_str());

    return intern_ptr(::xcb_intern_atom_reply (conn, cookie, nullptr));
}

// =========================================================

inline prop_ptr propAtomHelper (display_type* conn,
                                handle_type   wnd_id,
                                atom_type     property,
                                atom_type     type)
{
    auto cookie = ::xcb_get_property(conn,
                                     0, /* _delete */
                                     wnd_id,
                                     property,
                                     type,
                                     0,
                                     sizeof(atom_type));

    return prop_ptr(::xcb_get_property_reply(conn, cookie, nullptr));
}

// =========================================================

inline geo_ptr geometryHelper (display_type* conn, handle_type wnd_id)
{
    auto cookie = ::xcb_get_geometry(conn, wnd_id);

    return geo_ptr(::xcb_get_geometry_reply(conn, cookie, nullptr));
}

// =========================================================

inline attrib_ptr attribHelper (display_type* conn, handle_type wnd_id)
{
    auto cookie = ::xcb_get_window_attributes (conn, wnd_id);

    return attrib_ptr(::xcb_get_window_attributes_reply(conn, cookie, nullptr));
}

// =========================================================

inline screen_type* screenHandle (display_type* conn, u32& screen) noexcept
{
    ::xcb_setup_t const* pSetup = ::xcb_get_setup (conn);
    u32 nNum = static_cast<u32> (::xcb_setup_roots_length (pSetup));
    ScreenForwardIterator it (*pSetup);

    if (nNum < screen) screen = nNum - 1;
    else nNum = screen;

    for (u8 n = 1; n < screen; ++n) ++it;
    return it->data;
}

} } } // namespace x

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_WINDOW_H_
