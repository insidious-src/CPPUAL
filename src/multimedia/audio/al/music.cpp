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

#include <cppual/multimedia/audio/al/music.h>
#include <cppual/gfx/coord.h>

#include "aldef.h"

namespace cppual { namespace Audio { namespace AL {

inline void makePlayer (uint uId) noexcept
{
    point3f gValue { 0, 0, 0 };

    ::alSourcei  (uId, AL::IsRelativeToListener, true);
    ::alSourcefv (uId, AL::Position, &gValue.x);
    //::alSourcefv (getObjectId (), AL::Direction, &gValue.x);
}

SoundPlayer::SoundPlayer () noexcept
: SoundSource ()
{
    if (valid ()) makePlayer (id ());
}

SoundPlayer::SoundPlayer (SoundBuffer& gBuffer) noexcept
: SoundSource (gBuffer)
{
    if (valid ()) makePlayer (id ());
}

} } } // namespace Audio
