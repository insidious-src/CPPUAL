/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_XCB_WINDOW_H_
#define CPPUAL_PLATFORM_XCB_WINDOW_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <xcb/xcb.h>
#include <string>
#include <memory>
#include "xcb_def.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

namespace cppual { namespace Ui { namespace Xcb {

typedef std::string                                string_type;
typedef std::unique_ptr<::xcb_intern_atom_reply_t> intern_ptr ;

inline ::xcb_intern_atom_reply_t* internAtomHelper (display_type* conn,
                                                    string_type const& str,
                                                    bool only_if_exists = false)
{
    ::xcb_intern_atom_cookie_t cookie = ::xcb_intern_atom (conn, only_if_exists,
                                                           static_cast<u16> (str.length()),
                                                           str.c_str());

    return ::xcb_intern_atom_reply (conn, cookie, nullptr);
}

} } } // namespace Xcb

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_WINDOW_H_
