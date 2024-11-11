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

#include <cppual/ui/animation.h>

namespace cppual { namespace ui {

// =========================================================

void motion::play (int nRepeat)
{
    if (!_M_bIsPlaying.load (std::memory_order_consume) and _M_pObject and _M_pObject->valid ())
    {
        _M_bIsPlaying.store (true);

        do
        {
        }
        while (nRepeat < 0 ? true : --nRepeat);

        _M_bIsPlaying.store (false);
    }
}

void motion::pause ()
{
}

void motion::resume ()
{
}

// =========================================================

void fade::play (int)
{
    if (!_M_bIsPlaying.load (std::memory_order_consume) and _M_pObject and _M_pObject->valid ())
    {
    }
}

void fade::pause ()
{
}

void fade::resume ()
{
}

// =========================================================

void rotation::play (int)
{
    if (!_M_bIsPlaying.load (std::memory_order_consume) and
            _M_pObject and _M_pObject->valid ())
    {
    }
}

void rotation::pause ()
{
}

void rotation::resume ()
{
}

// =========================================================

void pulse::play (int)
{
    if (!_M_bIsPlaying.load (std::memory_order_consume) and
            _M_pObject and _M_pObject->valid ())
    {
    }
}

void pulse::pause ()
{
}

void pulse::resume ()
{
}

} } // namespace Ui
