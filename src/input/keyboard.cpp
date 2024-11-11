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

#include <cppual/input/keyboard.h>

#ifdef   OS_WINDOWS
#   include <windows.h>
#endif // OS_WINDOWS

namespace cppual::input {

bool keyboard::is_key_pressed (int)
{
    return false;
}

bool keyboard::is_lock_active (lock)
{
    return false;
}

bool keyboard::is_connected () noexcept
{
    return true;
}

bool key_queue::pop_front (event_type&, bool) noexcept
{
    return false;
}

} // namespace Input
