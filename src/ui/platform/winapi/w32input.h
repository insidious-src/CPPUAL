/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_PLATFORM_WIN32_INPUT_H_
#define CPPUAL_PLATFORM_WIN32_INPUT_H_
#ifdef __cplusplus

#include <cppual/ui/queue.h>
#ifdef   OS_WINDOWS

namespace cppual { namespace Ui {

struct DECL_EXPORT Win32Queue final : public IDisplayQueue
{
    Win32Queue () noexcept;
    bool set_window_events (IWindow const&, mask_type);
    bool pop_front         (event_type&, bool);
    int  poll              (IWindow const&, atomic_bool&);
    void send              (event_type const&);
    void post              (event_type const&);
};

} } // namespace Input

#endif // OS_WINDOWS
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_WIN32_INPUT_H_
