/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cppual/ui/animation.h>

namespace cppual { namespace Ui {

float IAnimation::defaultSpeed = 1.0;

// =========================================================

void Motion::play (int nRepeat)
{
    if (!m_bIsPlaying.load () and m_pObject and m_pObject->valid ())
    {
        m_bIsPlaying.store (true);

        do
        {
        }
        while (nRepeat < 0 ? true : --nRepeat);

        m_bIsPlaying.store (false);
    }
}

void Motion::pause ()
{
}

void Motion::resume ()
{
}

// =========================================================

void Fade::play (int)
{
    if (!m_bIsPlaying and m_pObject and m_pObject->valid ())
    {
    }
}

void Fade::pause ()
{
}

void Fade::resume ()
{
}

// =========================================================

void Rotation::play (int)
{
    if (!m_bIsPlaying.load (std::memory_order_consume) and
            m_pObject and m_pObject->valid ())
    {
    }
}

void Rotation::pause ()
{
}

void Rotation::resume ()
{
}

// =========================================================

void Pulse::play (int)
{
    if (!m_bIsPlaying.load (std::memory_order_consume) and
            m_pObject and m_pObject->valid ())
    {
    }
}

void Pulse::pause ()
{
}

void Pulse::resume ()
{
}

} } // namespace Ui
