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

#ifndef CPPUAL_AUDIO_SOURCE_H_
#define CPPUAL_AUDIO_SOURCE_H_
#ifdef __cplusplus

#include <cppual/multimedia/audio/al/albuffer.h>

#include <mutex>

namespace cppual { namespace audio { namespace al {

class SHARED_API sound_source : public object
{
public:
    typedef deque<sound_buffer*> queue_type;
    typedef std::size_t          size_type ;

    sound_source () noexcept;
    sound_source (sound_buffer&) noexcept;

    explicit sound_source (sound_source&&) noexcept;
    explicit sound_source (sound_source const&) noexcept;
    sound_source& operator = (sound_source&&) noexcept;
    sound_source& operator = (sound_source const&) noexcept;
    ~sound_source () noexcept;

    sound_state state () const noexcept;

    void  play       () noexcept;
    void  replay     () noexcept;
    void  pause      () noexcept;
    void  stop       () noexcept;
    void  rewind     () noexcept;
    bool  is_playing () const noexcept;

    void  set_looping (bool value) noexcept;
    bool  is_looping  () const noexcept;

    void  enqueue (sound_buffer&) noexcept;
    bool  attach  (sound_buffer&) noexcept;
    void  detach  () noexcept;

    void  set_volume (float value) noexcept;
    float volume () const noexcept;

    void  set_playing_speed  (float value) noexcept;
    float playing_speed      () const noexcept;
    void  set_playing_offset (std::chrono::seconds value) noexcept;
    int   playing_offset     () noexcept;
    void  set_sample_offset  (float value) noexcept;
    void  set_byte_offset    (float value) noexcept;

    void  mute   () noexcept;
    void  unmute () noexcept;

    inline sound_buffer* buffer () const noexcept { return _M_pBuffer;  }
    inline void          clear  ()       noexcept { _M_gQueue.clear (); }

protected:
    queue_type         _M_gQueue;
    std::mutex mutable _M_gMutex;
    sound_buffer*      _M_pBuffer;
    size_type          _M_uBufferSlot;
    float              _M_fVolume;

    void on_detach () noexcept;
    friend class sound_buffer;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_SOURCE_H_
