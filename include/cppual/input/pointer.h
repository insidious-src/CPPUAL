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

#ifndef CPPUAL_DEVICES_MOUSE_H_
#define CPPUAL_DEVICES_MOUSE_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/coord.h>

namespace cppual { namespace Input {

struct Touch;
struct Mouse;
struct MouseEvent;

// =========================================================

// specialized for touch gestures
class TouchEvent
{
public:
    typedef u16 size_type;

    enum class Type : unsigned char
    {
        Move,
        Press,
        Release
    };

    struct TouchData
    {
        size_type id;
        point2i   pos;
    };

private:
};

// =========================================================

class MouseQueue : NonConstructible
{
public:
    typedef MouseEvent event_type;

    static bool pop_front (event_type& next_event, bool wait = false) noexcept;
};

// =========================================================

class TouchQueue : NonConstructible
{
public:
    typedef TouchEvent event_type;

    static bool pop_front (event_type& next_event, bool wait = false) noexcept;
};

// =========================================================

struct Mouse
{
    typedef MouseQueue queue_type;

    enum
    {
        ButtonCount = 7
    };

    enum Button
    {
        Left     = 1 << 0,
        Right    = 1 << 1,
        Middle   = 1 << 2,
        XButton1 = 1 << 3,
        XButton2 = 1 << 4,
        XButton3 = 1 << 5,
        XButton4 = 1 << 6
    };

    static point2i position ();
    static void    setPosition (point2i pos);
    static bool    isButtonPressed (u8 btn);
    static bool    isConnected () noexcept;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_MOUSE_H_
