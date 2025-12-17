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

#ifndef CPPUAL_DEVICES_MOUSE_H_
#define CPPUAL_DEVICES_MOUSE_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/noncopyable>
#include <cppual/gfx/coord.h>

// =========================================================

namespace cppual::input {

// =========================================================

struct touch_event;
struct mouse      ;
struct mouse_event;

// =========================================================

class mouse_queue : non_constructible
{
public:
    typedef mouse_event event_type;

    static bool pop_front (event_type& next_event, bool wait = false) noexcept;
};

// =========================================================

class touch_queue : non_constructible
{
public:
    typedef touch_event event_type;

    static bool pop_front (event_type& next_event, bool wait = false) noexcept;
};

// =========================================================

struct mouse
{
    typedef mouse_queue queue_type;

    enum
    {
        button_count = 7
    };

    enum button
    {
        left     = 1 << 0,
        right    = 1 << 1,
        middle   = 1 << 2,
        xbutton1 = 1 << 3,
        xbutton2 = 1 << 4,
        xbutton3 = 1 << 5,
        xbutton4 = 1 << 6
    };

    static point2i position ();
    static void    set_position (point2i pos);
    static bool    is_button_pressed (u8 btn);
    static bool    is_connected () noexcept;
};

} // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_MOUSE_H_
