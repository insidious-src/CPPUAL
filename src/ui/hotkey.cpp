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

#include <cppual/ui/hotkey.h>

namespace cppual { namespace ui {

hotkey::hotkey () noexcept
{
}

hotkey::hotkey (value_type const) noexcept
{
}

void hotkey::set_key_code (value_type const gCode) noexcept
{
    _M_gKeyCode = gCode;
}

void hotkey::on_key_press (u8 nKey) noexcept
{
    if (!_M_bIsTriggered.load (std::memory_order_consume) and
            _M_gKeyCode.key == nKey)
    {
        _M_bIsTriggered.store (true, std::memory_order_relaxed);
        signal_triggered ();
    }
}

void hotkey::on_key_release (u8 nKey) noexcept
{
    if (_M_bIsTriggered.load (std::memory_order_consume) and
            _M_gKeyCode.key == nKey)
    {
        _M_bIsTriggered.store (false, std::memory_order_relaxed);
        signal_released ();
    }
}

void hotkey::on_key_notify (value_type const gCode) noexcept
{
    if (_M_bIsTriggered.load (std::memory_order_consume))
    {
        if (_M_gKeyCode.key == gCode.key)
        {
            _M_bIsTriggered.store (true, std::memory_order_relaxed);
            signal_triggered ();
        }
    }
    else
    {
        if (_M_gKeyCode.key == gCode.key)
        {
            _M_bIsTriggered.store (false, std::memory_order_relaxed);
            signal_released ();
        }
    }
}

} } // namespace Ui
