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

#ifndef CPPUAL_AUDIO_BUFFER_H_
#define CPPUAL_AUDIO_BUFFER_H_
#ifdef __cplusplus

#include <cppual/multimedia/audio/sound.h>
#include <cppual/multimedia/audio/al/aldevice.h>

#include <cppual/common.h>
#include <cppual/string.h>

namespace cppual { namespace audio { namespace al {

class sound_source;

// ====================================================

class SHARED_API sound_buffer : public object, public sound
{
public:
    typedef dyn_array<sound_source*>    vector_type   ;
    typedef vector_type::const_iterator const_iterator;
    typedef std::size_t                 size_type     ;
    friend  class                       sound_source  ;

    sound_buffer () noexcept;
    sound_buffer (sound_buffer&&) noexcept;
    sound_buffer& operator = (sound_buffer&&) noexcept;
    ~sound_buffer () noexcept;

    int frequency () const noexcept;
    int size () const noexcept;
    int bits () const noexcept;
    int duration () const noexcept;

    constexpr instance* context () const noexcept
    { return _M_pContext; }

private:
    vector_type _M_gSources;
    instance*   _M_pContext;

    int on_open () noexcept;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_BUFFER_H_
