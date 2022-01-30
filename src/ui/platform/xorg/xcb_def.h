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

#ifndef CPPUAL_PLATFORM_XCB_DEF_H_
#define CPPUAL_PLATFORM_XCB_DEF_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/types.h>

#include <memory>
#include <cstdlib>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <cppual/ui/platforms/xcb/xcb.hpp>

#include <xcb/xcb_icccm.h>
#include <X11/Xlib-xcb.h>

namespace cppual { namespace ui { namespace xcb {

typedef ::xcb_connection_t display_type;
typedef ::Display          legacy_type ;
typedef ::xcb_screen_t     screen_type ;
typedef ::xcb_window_t     handle_type ;
typedef ::xcb_atom_t       atom_type   ;

constexpr cu8 atom_format   = sizeof(atom_type) * 8;
constexpr cu8 string_format = sizeof(char) * 8;

// =========================================================

template <typename T>
struct handle_ptr : public std::shared_ptr<T>
{
    inline handle_ptr(T* handle)
    : std::shared_ptr<T> (handle, [](void* p) { std::free (p); })
    { }
};

} } } // namespace x

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_DEF_H_
