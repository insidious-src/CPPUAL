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

#ifndef CPPUAL_AUDIO_SOUND_H_
#define CPPUAL_AUDIO_SOUND_H_
#ifdef __cplusplus

#include <array>
#include <chrono>
#include <string>
#include <cppual/flags.h>
#include <cppual/multimedia/audio/format.h>

using std::string;

namespace cppual { namespace Audio {

enum
{
    MaxChannels  = 7,
    BufferLength = 1024
};

enum class OutputFormat : unsigned char
{
    Mono,
    Stereo,
    Quad,
    Side,
    Rear,
    CH51,
    CH61,
    CH71
};

enum class SoundState : unsigned char
{
    Unavailable,
    Initial,
    Playing,
    Paused,
    Stopped
};

enum class SoundQuality : unsigned char
{
    Low    =  8,
    Medium = 16,
    High   = 32,
};

// ====================================================

class Sound
{
public:
    typedef std::array<double, BufferLength> buffer_type;
    typedef double                           value_type;
    typedef double*                          pointer;
    typedef double const*                    const_pointer;
    typedef int64                            size_type;

    enum Attribute
    {
        Title     = 0x01,
        Copyright = 0x02,
        Software  = 0x03,
        Artist    = 0x04,
        Comment   = 0x05,
        Date      = 0x06,
        Album     = 0x07,
        License   = 0x08,
        TrackNo   = 0x09,
        Genre     = 0x10
    };

    class Handle
    {

    };

    Sound (Sound&&) noexcept;
    Sound& operator = (Sound&&) noexcept;
    ~Sound () noexcept;

    string attribute (Attribute) const noexcept;
    int    openReadOnly (string const& file) noexcept;
    int    openReadOnly (cvoid* data, size_type byte_size) noexcept;
    int    openWritable (string const& file, int channels, int sample_rate) noexcept;
    bool   save (string const& file) noexcept;
    void   seek (std::chrono::seconds time_pos) noexcept;
    void   close () noexcept;

    const_pointer data         () const noexcept { return m_gBuffer.data ();     }
    size_type     sampleCount  () const noexcept { return m_nSampleCount;        }
    int           sampleRate   () const noexcept { return m_nSampleRate;         }
    int           channelCount () const noexcept { return m_nChannelCount;       }
    bool          isAvailable  () const noexcept { return m_pSndDesc != nullptr; }

    bool isSeekable () const noexcept
    { return m_gFlags.test (Sound::Seekable); }

    bool isReadable () const noexcept
    { return m_gFlags.test (Sound::Read); }

    bool isWritable () const noexcept
    { return m_gFlags.test (Sound::Write); }

    Sound () noexcept
    : m_gBuffer (),
      m_pSndDesc (),
      m_nSampleCount (),
      m_nSampleRate (),
      m_nChannelCount (),
      m_gFlags (nullptr)
    { }

protected:
    virtual int  onOpen () { return true; }
    virtual void onSave (string const&) { }
    virtual void onClose () { }

private:
    enum Flag
    {
        Read     = 1 << 0,
        Write    = 1 << 1,
        Seekable = 1 << 2
    };

    typedef BitSet<Sound::Flag> Flags;

private:
    buffer_type m_gBuffer;
    void*       m_pSndDesc;
    size_type   m_nSampleCount;
    int         m_nSampleRate, m_nChannelCount;
    Flags       m_gFlags;
};

} } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_SOUND_H_
