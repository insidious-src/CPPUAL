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

#include <cppual/multimedia/audio/al/music.h>
#include <cppual/gfx/coord.h>

#include "aldef.h"

namespace cppual { namespace audio { namespace al {

inline void make_player (uint uId) noexcept
{
    point3f gValue (.0f, .0f, .0f);

    ::alSourcei  (uId, al::is_relative_to_listener, true);
    ::alSourcefv (uId, al::position, &gValue.x);
    //::alSourcefv (getObjectId (), AL::Direction, &gValue.x);
}

sound_player::sound_player () noexcept
: sound_source ()
{
    if (valid ()) make_player (handle ());
}

sound_player::sound_player (sound_buffer& gBuffer) noexcept
: sound_source (gBuffer)
{
    if (valid ()) make_player (handle ());
}

} } } // namespace Audio
