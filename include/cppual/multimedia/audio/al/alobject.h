/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef CPPUAL_AUDIO_BASE_H_
#define CPPUAL_AUDIO_BASE_H_
#ifdef __cplusplus

#include <atomic>
#include <cppual/multimedia/audio/spatial.h>

using std::atomic_uint;

namespace cppual { namespace Audio { namespace AL {

class AudioObject
{
public:
    AudioObject (ObjectType type) noexcept;
    AudioObject (AudioObject&&) noexcept;
    AudioObject (AudioObject const&) noexcept;
    AudioObject& operator = (AudioObject&&) noexcept;
    AudioObject& operator = (AudioObject const&) noexcept;

    inline  AudioObject () noexcept : m_uObjId (), m_eObjType () { }
    inline ~AudioObject () noexcept { reset (); }

    inline ObjectType type () const noexcept { return m_eObjType; }
    inline uint       id () const noexcept { return m_uObjId; }
    inline bool       isValid () const noexcept { return m_uObjId; }

    inline static uint count () noexcept
    { return sm_uALObjCount.load (std::memory_order_consume); }

private:
    static atomic_uint sm_uALObjCount;
    uint                m_uObjId;
    ObjectType          m_eObjType;

    void reset () noexcept;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_BASE_H_
