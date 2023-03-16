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

#ifndef CPPUAL_AUDIO_AL_LISTENER_H_
#define CPPUAL_AUDIO_AL_LISTENER_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>

namespace cppual { namespace audio { namespace al { namespace listener {

void    SHARED_API reset () noexcept;
void    SHARED_API SHARED_API set_position (point3f const& position) noexcept;
point3f SHARED_API position () noexcept;
void    SHARED_API set_velocity (point3f const& velocity) noexcept;
point3f SHARED_API velocity () noexcept;
void    SHARED_API set_orientation (point3f const& at, point3f const& up = { 0, 1.f, 0 }) noexcept;
point3f SHARED_API orientation_at () noexcept;
point3f SHARED_API orientation_up () noexcept;
void    SHARED_API set_volume (float value) noexcept;
float   SHARED_API volume () noexcept;
bool    SHARED_API is_mute () noexcept;
void    SHARED_API mute () noexcept;
void    SHARED_API unmute () noexcept;

} } } } // namespace Listener

#endif // __cplusplus
#endif // CPPUAL_AUDIO_AL_LISTENER_H_
