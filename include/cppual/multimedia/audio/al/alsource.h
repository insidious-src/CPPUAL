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

#ifndef CPPUAL_AUDIO_SOURCE_H_
#define CPPUAL_AUDIO_SOURCE_H_
#ifdef __cplusplus

#include <mutex>
#include <deque>
#include <cppual/multimedia/audio/al/albuffer.h>

using std::mutex;

namespace cppual { namespace Audio { namespace AL {

class SoundSource : public Object
{
public:
    typedef std::deque<SoundBuffer*> queue_type;
    typedef std::size_t              size_type;

    SoundSource () noexcept;
    SoundSource (SoundBuffer&) noexcept;

    explicit SoundSource (SoundSource&&) noexcept;
    explicit SoundSource (SoundSource const&) noexcept;
    SoundSource& operator = (SoundSource&&) noexcept;
    SoundSource& operator = (SoundSource const&) noexcept;
    ~SoundSource () noexcept;

    SoundState state () const noexcept;

    void  play      () noexcept;
    void  replay    () noexcept;
    void  pause     () noexcept;
    void  stop      () noexcept;
    void  rewind    () noexcept;
    bool  isPlaying () const noexcept;

    void  setLooping (bool value) noexcept;
    bool  isLooping  () const noexcept;

    void  enqueue (SoundBuffer&) noexcept;
    bool  attach  (SoundBuffer&) noexcept;
    void  detach  () noexcept;

    void  setVolume (float value) noexcept;
    float getVolume () const noexcept;

    void  setPlayingSpeed  (float value) noexcept;
    float playingSpeed     () const noexcept;
    void  setPlayingOffset (std::chrono::seconds value) noexcept;
    int   playingOffset    () noexcept;
    void  setSampleOffset  (float value) noexcept;
    void  setByteOffset    (float value) noexcept;

    void  mute () noexcept;
    void  unmute () noexcept;

    inline SoundBuffer* buffer () const noexcept { return m_pBuffer;  }
    inline void         clear  ()       noexcept { m_gQueue.clear (); }

protected:
    queue_type    m_gQueue;
    mutex mutable m_gMutex;
    SoundBuffer*  m_pBuffer;
    size_type     m_uBufferSlot;
    float         m_fVolume;

    void onDetach () noexcept;
    friend class SoundBuffer;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_SOURCE_H_
