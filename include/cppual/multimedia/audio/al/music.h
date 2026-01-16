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

#ifndef CPPUAL_AUDIO_MUSIC_H_
#define CPPUAL_AUDIO_MUSIC_H_
#ifdef __cplusplus

#include <cppual/multimedia/audio/al/alsource.h>

namespace cppual { namespace audio { namespace al {

struct SHARED_API sound_player final : public sound_source
{
public:
    using sound_source::sound_source;
    sound_player () noexcept;
    sound_player (sound_buffer&) noexcept;

    constexpr bool is_usable () const noexcept
    { return _M_pBuffer and _M_pBuffer->channel_count () > 1; }
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_MUSIC_H_
