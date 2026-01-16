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

#ifndef CPPUAL_DEVICES_EVENT_H
#define CPPUAL_DEVICES_EVENT_H
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/bitset.h>
#include <cppual/gfx/coord.h>
#include <cppual/input/keyboard.h>
#include <cppual/input/pointer.h>
#include <cppual/input/joystick.h>
#include <cppual/noncopyable.h>

namespace cppual::input {

struct system_message final
{
    enum type
    {
        quit,
        child,
        terminate
    };
};

// =========================================================

class event
{
public:
    typedef std::size_t size_type;

    enum bits
    {
        null               =       0,
        key_pressed        = 1 <<  0,
        key_released       = 1 <<  1,
        mbutton_down       = 1 <<  2,
        mbutton_up         = 1 <<  3,
        mwheel_step        = 1 <<  4,
        mouse_move         = 1 <<  5,
        touch_press        = 1 <<  6,
        touch_release      = 1 <<  7,
        touch_move         = 1 <<  8,
        joy_connect        = 1 <<  9,
        joy_disconnect     = 1 << 10,
        joy_button_press   = 1 << 11,
        joy_button_release = 1 << 12,
        joy_move           = 1 << 13,
        joy_trigger        = 1 << 14,
        joy_track_move     = 1 << 15,
        sys_message        = 1 << 16,
        paint              = 1 << 17,
        focus              = 1 << 18,
        step               = 1 << 19,
        size               = 1 << 20,
        visibility         = 1 << 21,
        property           = 1 << 22,
        destroy            = 1 << 23,

        key      = key_pressed   | key_released,
        mouse    = mbutton_down  | mbutton_up     | mouse_move | mwheel_step,
        touch    = touch_press   | touch_release  | touch_move,
        joystick = joy_connect   | joy_disconnect | joy_button_press | joy_button_release   |
                   joy_move      | joy_trigger    | joy_track_move,
        window   = paint | focus | size           | visibility     | property | destroy | step,
        all      = key   | mouse | touch          | window         | sys_message
    };

    typedef bitset<bits> bit_flags;

    struct base_data
    {
        size_type first ;
        size_type second;
    };

    struct key_data
    {
        keyboard::key      key ;
        keyboard::modifier mask;
    };

    struct mbutton_data
    {
        point2u pos   ;
        u8      button;
    };

    struct mwheel_data
    {
        point2u pos;
        i32   delta;
    };

    struct touch_data
    {
        i32     pid;
        point2u pos;
    };

    struct joy_button_data
    {
        size_type id    ;
        i32       button;
    };

    struct joy_axis_data
    {
        size_type id   ;
        i16       value;
        u8        axis ;
    };

    struct joy_trigger_data
    {
        size_type id       ;
        i16       threshold;
        u8        trigger  ;

    };

    struct joy_track_data
    {
        size_type id   ;
        point2i   pos  ;
        u8        track;
    };

    struct joy_plug_data
    {
        size_type id;
    };

    struct paint_data
    {
        rect region;
    };

    struct property_data
    {
        u32 prop ;
        i32 value;
    };

    union data_value
    {
        base_data        base        ;
        key_data         key_code    ;
        mbutton_data     mouse_button;
        point2u          position    ;
        mwheel_data      wheel       ;
        touch_data       touch       ;
        joy_button_data  joy_button  ;
        joy_axis_data    joy_axis    ;
        joy_plug_data    joy_plug    ;
        joy_trigger_data joy_trigger ;
        joy_track_data   joy_track   ;
        i32              message     ;
        paint_data       paint       ;
        point2u          size        ;
        bool             state       ;
        property_data    property    ;

        constexpr data_value () noexcept : base { } { }

        constexpr data_value (key_data const& gKeyData) noexcept
        : key_code (gKeyData)
        { }

        constexpr data_value (mbutton_data const& gKeyData) noexcept
        : mouse_button (gKeyData)
        { }
    };

    typedef data_value        value_type     ;
    typedef data_value const  const_value    ;
    typedef data_value&       reference      ;
    typedef data_value const& const_reference;

    constexpr event () noexcept : _M_data (), _M_type { } { }
    constexpr const_reference data () const noexcept { return _M_data; }
    constexpr bit_flags       type () const noexcept { return _M_type; }

    constexpr event (bits type, const_reference data = value_type()) noexcept
    : _M_data (data),
      _M_type (type)
    { }

protected:
    value_type _M_data;
    bit_flags  _M_type;
};

// =========================================================

struct message_event : public event
{
    inline
    message_event (int nMsg) noexcept
    : event   (event::sys_message)
    {
        _M_data.message = nMsg;
    }
};

// =========================================================

struct visibility_event : public event
{
    inline
    visibility_event (bool bVis) noexcept
    : event      (event::visibility)
    {
        _M_data.state = bVis;
    }
};

// =========================================================

struct paint_event : public event
{
    inline
    paint_event (rect gRect) noexcept
    : event (event::paint)
    {
        _M_data.paint.region = gRect;
    }
};

// =========================================================

struct size_event : public event
{
    inline
    size_event (point2u size) noexcept
    : event(event::size)
    {
        _M_data.size = size;
    }
};

// =========================================================

struct focus_event : public event
{
    inline
    focus_event (bool in) noexcept
    : event (event::focus)
    {
        _M_data.state = in;
    }
};

// =========================================================

struct step_event : public event
{
    inline
    step_event (bool in) noexcept
    : event(event::step)
    {
        _M_data.state = in;
    }
};

// =========================================================

struct property_event : public event
{
    inline
    property_event (u32 prop, i32 value) noexcept
    : event    (event::property)
    {
        _M_data.property.prop  = prop ;
        _M_data.property.value = value;
    }
};

// =========================================================

struct key_event : public event
{
    constexpr key_event (keyboard::key nKey,
                         keyboard::modifier uMask = keyboard::modifier::none,
                         bool is_pressed = true) noexcept
    : event (is_pressed ? event::key_pressed : event::key_released, { key_data { nKey, uMask } })
    { }
};

// =========================================================

struct key_press_event : public key_event
{
    constexpr key_press_event (keyboard::key nKey,
                               keyboard::modifier uMask = keyboard::modifier::none) noexcept
    : key_event (nKey, uMask, true)
    { }
};

// =========================================================

struct key_release_event : public key_event
{
    constexpr key_release_event (keyboard::key nKey,
                                 keyboard::modifier uMask = keyboard::modifier::none) noexcept
    : key_event (nKey, uMask, false)
    { }
};

// =========================================================

struct mouse_event : public event
{
    constexpr mouse_event (u8 nBtn, point2u gPos, bool is_down) noexcept
    : event (is_down ? event::mbutton_down : event::mbutton_up, { mbutton_data { gPos, nBtn } })
    { }

    constexpr mouse_event (point2u gPos) noexcept
    : event (event::mouse_move)
    {
        _M_data.position = gPos;
    }

    constexpr mouse_event (i32 nDelta, point2u gPos) noexcept
    : event (event::mwheel_step)
    {
        _M_data.wheel.delta = nDelta;
        _M_data.wheel.pos   = gPos;
    }
};

// =========================================================

struct mouse_press_event : public mouse_event
{
    constexpr mouse_press_event (u8 nBtn, point2u gPos) noexcept
    : mouse_event (nBtn, gPos, true)
    { }
};

// =========================================================

struct mouse_release_event : public mouse_event
{
    constexpr mouse_release_event (u8 nBtn, point2u gPos) noexcept
    : mouse_event (nBtn, gPos, false)
    { }
};

// =========================================================

struct mouse_move_event : public mouse_event
{
    inline
    mouse_move_event (point2u gPos) noexcept
    : mouse_event (gPos)
    { }
};

// =========================================================

struct wheel_event : public mouse_event
{
    inline
    wheel_event (i32 nDelta, point2u gPos) noexcept
    : mouse_event (nDelta, gPos)
    { }
};

// =========================================================

struct touch_event : public event
{
    inline
    touch_event (i32 pid, point2u gPos, bits action) noexcept
    : event (action)
    {
        _M_data.touch.pid = pid;
        _M_data.touch.pos = gPos;
    }
};

// =========================================================

struct touch_press_event : public touch_event
{
    inline
    touch_press_event (i32 pid, point2u gPos) noexcept
    : touch_event     (pid, gPos, event::touch_press)
    { }
};

// =========================================================

struct touch_release_event : public touch_event
{
    inline
    touch_release_event (i32 pid, point2u gPos) noexcept
    : touch_event       (pid, gPos, event::touch_release)
    { }
};

// =========================================================

struct touch_moved_event : public touch_event
{
    inline
    touch_moved_event (i32 pid, point2u gPos) noexcept
    : touch_event     (pid, gPos, event::touch_move)
    { }
};

// =========================================================

} // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_EVENT_H
