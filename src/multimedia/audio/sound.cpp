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

#include <cppual/multimedia/audio/sound.h>
#include <cppual/types.h>

#include <locale>

#include <sndfile.h>

namespace cppual { namespace audio {

sound::sound (sound&& gObj) noexcept
: _M_gBuffer       (gObj._M_gBuffer)      ,
  _M_pSndDesc      (gObj._M_pSndDesc)     ,
  _M_nSampleCount  (gObj._M_nSampleCount) ,
  _M_nSampleRate   (gObj._M_nSampleRate)  ,
  _M_nChannelCount (gObj._M_nChannelCount),
  _M_gFlags        (gObj._M_gFlags)
{
    gObj._M_nSampleCount = gObj._M_nChannelCount = gObj._M_nSampleRate = 0;
    gObj._M_pSndDesc     = nullptr;
    gObj._M_gFlags       = 0;
}

sound& sound::operator = (sound&& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

sound::~sound () noexcept
{
    if (_M_pSndDesc) ::sf_close (static_cast<SNDFILE*> (_M_pSndDesc));
}

int sound::open_read_only (string const& gFile) noexcept
{
    close ();

    ::SF_INFO gInfo { 0, 0, 0, 0, 0, 0 };
    _M_pSndDesc = ::sf_open (gFile.c_str (), 0, &gInfo);

    if (_M_pSndDesc)
    {
        if (gInfo.channels > max_channels)
        {
            ::sf_close (static_cast<SNDFILE*> (_M_pSndDesc));
            return false;
        }

        _M_nChannelCount = gInfo.channels;
        _M_nSampleCount  = gInfo.frames;
        _M_nSampleRate   = gInfo.samplerate;

        if (gInfo.seekable) _M_gFlags += flag::seekable;
        _M_gFlags += flag::read;

        /// using format to save an int copy
        if (gInfo.format = on_open (); !gInfo.format)
        {
            close ();
            return gInfo.format;
        }

        return true;
    }

    return false;
}

int sound::open_read_only (cvoid*, size_type) noexcept
{
    close ();
    _M_gFlags = flag::read;
    return false;
}

int sound::open_writable (string_type const&, int, int) noexcept
{
    close ();
    _M_gFlags = flags::value_type(flag::read) | flags::value_type(flag::write);

    on_open ();
    return false;
}

bool sound::save (string_type const& gFileName) noexcept
{
    if (_M_pSndDesc)
    {
        on_save (gFileName);
        return true;
    }

    return false;
}

void sound::close () noexcept
{
    if (_M_pSndDesc)
    {
        on_close ();
        ::sf_close (static_cast<SNDFILE*> (_M_pSndDesc));

        _M_nSampleCount = _M_nSampleRate = _M_nChannelCount = 0;
        _M_gFlags = 0;
    }
}

void sound::seek (std::chrono::seconds) noexcept
{
    if (_M_gFlags.test (flag::seekable))
    {
    }
}

sound::string_type sound::attribute (attribute_type eType) const noexcept
{
    return ::sf_get_string (static_cast<::SNDFILE*> (_M_pSndDesc), eType);
}



} } // namespace Audio
