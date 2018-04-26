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

#ifndef CPPUAL_AUDIO_AL_LISTENER_H_
#define CPPUAL_AUDIO_AL_LISTENER_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>

namespace cppual { namespace Audio { namespace AL { namespace Listener {

void    reset () noexcept;
void    setPosition (point3f const& position) noexcept;
point3f position () noexcept;
void    setVelocity (point3f const& velocity) noexcept;
point3f velocity () noexcept;
void    setOrientation (point3f const& at, point3f const& up = { 0, 1.f, 0 }) noexcept;
point3f orientationAt () noexcept;
point3f orientationUp () noexcept;
void    setVolume (float value) noexcept;
float   volume () noexcept;
bool    isMute () noexcept;
void    mute () noexcept;
void    unmute () noexcept;

} } } } // namespace Listener

#endif // __cplusplus
#endif // CPPUAL_AUDIO_AL_LISTENER_H_
