/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#ifndef CPPUAL_DEVICES_JOYSTICK_H_
#define CPPUAL_DEVICES_JOYSTICK_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/gfx/coord.h>
#include <cppual/noncopyable.h>
#include <cppual/string.h>

namespace cppual { namespace input {

// =========================================================

struct joystick      ;
struct joystick_event;

// =========================================================

class joystick_queue : non_constructible
{
public:
    typedef joystick_event event_type;

    static bool pop_front (event_type& next_event, bool wait = false) noexcept;
};

// =========================================================

struct joystick : non_copyable_virtual
{
    typedef std::size_t size_type;

    enum
    {
        max_count    = 16,
        button_count = 32,
        axis_count   =  8
    };

    enum class axis : u8
    {
        x,
        y,
        z,
        r,
        u,
        v,
        pov_x,
        pov_y
    };

    enum class surface : u8
    {
        track_pad,
        track_ball,
        touch
    };

    virtual string    model_name () const = 0;
    virtual float     axis_position (axis) = 0;
    virtual size_type id () const = 0;
    virtual size_type btn_count () const = 0;
    virtual bool      valid () const = 0;
    virtual bool      is_button_pressed (int) = 0;
    virtual bool      has_axis (axis) const = 0;
    virtual bool      has_surface (surface) const = 0;
    virtual bool      has_motor () const = 0;
    virtual void      vibrate (ushort) noexcept = 0;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_JOYSTICK_H_
