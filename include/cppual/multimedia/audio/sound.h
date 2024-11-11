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

#ifndef CPPUAL_AUDIO_SOUND_H_
#define CPPUAL_AUDIO_SOUND_H_
#ifdef __cplusplus


#include <cppual/bitset.h>
#include <cppual/containers.h>
#include <cppual/multimedia/audio/format.h>
#include <cppual/string.h>

//#include <array>
#include <chrono>

namespace cppual { namespace audio {

enum
{
    max_channels  = 7,
    buffer_length = 1024
};

enum class output_format : u8
{
    mono,
    stereo,
    quad,
    side,
    rear,
    ch51,
    ch61,
    ch71
};

enum class sound_state : u8
{
    unavailable,
    initial,
    playing,
    paused,
    stopped
};

enum class sound_quality : u8
{
    low    =  8,
    medium = 16,
    high   = 32,
};

// ====================================================

class SHARED_API sound
{
public:
    typedef vector<uchar> buffer_type  ;
    typedef uchar         value_type   ;
    typedef uchar*        pointer      ;
    typedef cuchar*       const_pointer;
    typedef i64           size_type    ;
    typedef string        string_type  ;

    enum attribute_type
    {
        title     = 0x01,
        copyright = 0x02,
        software  = 0x03,
        artist    = 0x04,
        comment   = 0x05,
        date      = 0x06,
        album     = 0x07,
        license   = 0x08,
        track_no  = 0x09,
        genre     = 0x10
    };

    sound (sound&&) noexcept;
    sound& operator = (sound&&) noexcept;
    virtual ~sound () noexcept;

    string_type attribute (attribute_type) const noexcept;
    int  open_read_only (string_type const& file) noexcept;
    int  open_read_only (cvoid* data, size_type byte_size) noexcept;
    int  open_writable (string_type const& file, int channels, int sample_rate) noexcept;
    bool save  (string_type const& file) noexcept;
    void seek  (std::chrono::seconds time_pos) noexcept;
    void close () noexcept;

    const_pointer data          () const noexcept { return _M_gBuffer.data ();     }
    size_type     sample_count  () const noexcept { return _M_nSampleCount;        }
    int           sample_rate   () const noexcept { return _M_nSampleRate;         }
    int           channel_count () const noexcept { return _M_nChannelCount;       }
    bool          is_available  () const noexcept { return _M_pSndDesc != nullptr; }

    bool is_seekable () const noexcept
    { return _M_gFlags.test (flag::seekable); }

    bool is_readable () const noexcept
    { return _M_gFlags.test (flag::read); }

    bool is_writable () const noexcept
    { return _M_gFlags.test (flag::write); }

    sound () noexcept
    : _M_gBuffer       (),
      _M_pSndDesc      (),
      _M_nSampleCount  (),
      _M_nSampleRate   (),
      _M_nChannelCount (),
      _M_gFlags        ()
    { }

protected:
    virtual int  on_open  () { return true; }
    virtual void on_save  (string_type const &) {}
    virtual void on_close () { }

private:
    enum class flag : u8
    {
        read     = 1 << 0,
        write    = 1 << 1,
        seekable = 1 << 2
    };

    typedef bitset<flag> flags;

private:
    buffer_type _M_gBuffer;
    void*       _M_pSndDesc;
    size_type   _M_nSampleCount;
    int         _M_nSampleRate, _M_nChannelCount;
    flags       _M_gFlags;
};

} } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_SOUND_H_
