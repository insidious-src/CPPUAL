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

#ifndef CPPUAL_UI_HOTKEY_H_
#define CPPUAL_UI_HOTKEY_H_
#ifdef __cplusplus

#include <cppual/signal.h>
#include <cppual/input/event.h>

#include <atomic>

namespace cppual { namespace ui {

class SHARED_API hotkey
{
public:
    typedef input::sys_event::key_data value_type;
    typedef std::atomic_bool           bool_type ;

    hotkey () noexcept;
    hotkey (value_type const) noexcept;
    void set_key_code (value_type const) noexcept;

    inline value_type key_code () const noexcept
    { return _M_gKeyCode; }

    inline bool is_triggered () const noexcept
    { return _M_bIsTriggered.load (std::memory_order_consume); }

    signal<void()> signal_triggered;
    signal<void()> signal_released ;

private:
    void on_key_press (u8) noexcept;
    void on_key_release (u8) noexcept;
    void on_key_notify (value_type const) noexcept;

private:
    value_type _M_gKeyCode    ;
    bool_type  _M_bIsTriggered;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_HOTKEY_H_
