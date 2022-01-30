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

#include <cppual/multimedia/audio/al/alsource.h>

#include "aldef.h"

using std::mutex;

namespace cppual { namespace audio { namespace al {

inline int convert_emitter_state (audio::sound_state eState) noexcept
{
    switch (eState)
    {
    case audio::sound_state::unavailable:
        return 0;
    case audio::sound_state::initial:
        return al::sound_initial;
    case audio::sound_state::playing:
        return al::sound_playing;
    case audio::sound_state::paused:
        return al::sound_paused;
    case audio::sound_state::stopped:
        return al::sound_stopped;
    }

    return 0;
}

inline audio::sound_state convert_emitter_state (int nState) noexcept
{
    switch (nState)
    {
    case al::sound_initial:
        return audio::sound_state::initial;
    case al::sound_playing:
        return audio::sound_state::playing;
    case al::sound_paused:
        return audio::sound_state::paused;
    case al::sound_stopped:
        return audio::sound_state::stopped;
    }

    return audio::sound_state::unavailable;
}

// ====================================================

sound_source::sound_source () noexcept
: object        (object_type::source),
  _M_gMutex      (),
  _M_pBuffer     (),
  _M_uBufferSlot (),
  _M_fVolume     ()
{ }

sound_source::sound_source (sound_buffer& gBuffer) noexcept
: object        (object_type::source),
  _M_gMutex      (),
  _M_pBuffer     (),
  _M_uBufferSlot (gBuffer._M_gSources.size ()),
  _M_fVolume     ()
{
    if (id () and gBuffer.valid ())
    {
        _M_pBuffer = &gBuffer;
        gBuffer._M_gSources.push_back (this); // attach
    }
}

sound_source::sound_source (sound_source&& gObj) noexcept
: object         (gObj),
  _M_gMutex      (),
  _M_pBuffer     (gObj._M_pBuffer),
  _M_uBufferSlot (gObj._M_uBufferSlot),
  _M_fVolume     (gObj._M_fVolume)
{
    gObj._M_pBuffer = nullptr;
    if (_M_pBuffer) _M_pBuffer->_M_gSources[_M_uBufferSlot] = this;
}

sound_source::sound_source (sound_source const& gObj) noexcept
: object         (gObj),
  _M_gMutex      (),
  _M_pBuffer     (gObj._M_pBuffer),
  _M_uBufferSlot (gObj._M_pBuffer ? gObj._M_pBuffer->_M_gSources.size () : 0),
  _M_fVolume     (gObj._M_fVolume)
{
    set_volume        (gObj.volume        ());
    set_looping       (gObj.is_looping    ());
    set_playing_speed (gObj.playing_speed ());
    if (_M_pBuffer) _M_pBuffer->_M_gSources.push_back (this);
}

sound_source& sound_source::operator = (sound_source&& gObj) noexcept
{
    _M_pBuffer      = gObj._M_pBuffer;
    _M_uBufferSlot  = gObj._M_uBufferSlot;
    _M_fVolume      = gObj._M_fVolume;
    gObj._M_pBuffer = nullptr;

    if (_M_pBuffer) _M_pBuffer->_M_gSources[_M_uBufferSlot] = this;
    return *this;
}

sound_source& sound_source::operator = (sound_source const& gObj) noexcept
{
    if (id ())
    {
        if (gObj._M_pBuffer and gObj._M_pBuffer->valid ())
        {
            _M_pBuffer     = gObj._M_pBuffer;
            _M_uBufferSlot = gObj._M_pBuffer->_M_gSources.size ();
            gObj._M_pBuffer->_M_gSources.push_back (this);
        }

        set_volume        (gObj.volume        ());
        set_looping       (gObj.is_looping    ());
        set_playing_speed (gObj.playing_speed ());
    }

    return *this;
}

sound_source::~sound_source () noexcept
{
    if (id ())
    {
        if (_M_pBuffer)
        {
            stop ();
            _M_pBuffer->_M_gSources[_M_uBufferSlot] = nullptr; // detach
        }
    }
}

sound_state sound_source::state () const noexcept
{
    int nState = 0;
    ::alGetSourcei (id (), al::current_emitter_state, &nState);
    return convert_emitter_state (nState);
}

void sound_source::play () noexcept
{
    int nState = 0;
    ::alGetSourcei (id (), al::current_emitter_state, &nState);
    if (_M_pBuffer and nState and nState != al::sound_playing) ::alSourcePlay (id ());
}

void sound_source::replay () noexcept
{
    int nState = 0;
    ::alGetSourcei (id (), al::current_emitter_state, &nState);

    if (nState and _M_pBuffer)
    {
        ::alSourceRewind (id ());
        if (nState != al::sound_playing) ::alSourcePlay (id ());
    }
}

void sound_source::pause () noexcept
{
    int nState = 0;
    ::alGetSourcei (id (), al::current_emitter_state, &nState);
    if (nState == al::sound_playing) ::alSourcePause (id ());
}

void sound_source::stop () noexcept
{
    int nState = 0;
    ::alGetSourcei (id (), al::current_emitter_state, &nState);

    if ((nState == al::sound_playing or nState == al::sound_paused))
        ::alSourceStop (id ());
}

void sound_source::rewind () noexcept
{
    if (_M_pBuffer and _M_pBuffer->valid () and state () != sound_state::initial)
        ::alSourceRewind (id ());
}

bool sound_source::is_playing () const noexcept
{
    int nState = 0;
    ::alGetSourcei (id (), al::current_emitter_state, &nState);
    return nState == al::sound_playing;
}

void sound_source::set_looping (bool bLoop) noexcept
{
    if (id ()) ::alSourcei (id (), al::looping, bLoop);
}

bool sound_source::is_looping () const noexcept
{
    int nIsLooping = 0;
    if (id ()) ::alGetSourcei (id (), al::looping, &nIsLooping);
    return nIsLooping == true;
}

bool sound_source::attach (sound_buffer& gBuffer) noexcept
{
    if (!id () or !gBuffer.valid ()) return false;

    _M_pBuffer     = &gBuffer;
    _M_uBufferSlot =  gBuffer._M_gSources.size ();
    gBuffer._M_gSources.push_back (this); // attach
    return true;
}

void sound_source::detach () noexcept
{
    if (_M_pBuffer)
    {
        _M_pBuffer->_M_gSources[_M_uBufferSlot] = nullptr;
        on_detach ();
    }
}

void sound_source::on_detach () noexcept
{
    stop ();
    ::alSourcei (id (), al::buffer, 0);
    _M_pBuffer = nullptr;
}

void sound_source::set_volume (float fValue) noexcept
{
    if (id ())
    {
        ::alSourcef     (id (), al::volume, fValue);
        _M_gMutex.lock   ();
        _M_fVolume = fValue;
        _M_gMutex.unlock ();
    }
}

float sound_source::volume () const noexcept
{
    std::lock_guard<mutex> gLock (_M_gMutex);
    return _M_fVolume;
}

void sound_source::set_playing_speed (float fValue) noexcept
{
    if (id ()) ::alSourcef (id (), al::pitch, fValue);
}

float sound_source::playing_speed () const noexcept
{
    float fValue = .0f;
    if (id ()) ::alGetSourcef (id (), al::pitch, &fValue);
    return fValue;
}

void sound_source::set_playing_offset (std::chrono::seconds nValue) noexcept
{
    if (id ()) ::alSourcei (id (), al::sec_offset, static_cast<int> (nValue.count ()));
}

int sound_source::playing_offset () noexcept
{
    int nValue = 0;
    if (id ()) ::alGetSourcei (id (), al::sec_offset, &nValue);
    return nValue;
}

void sound_source::set_sample_offset (float fValue) noexcept
{
    if (id ()) ::alSourcef (id (), al::sample_offset, fValue);
}

void sound_source::set_byte_offset (float fValue) noexcept
{
    if (id ()) ::alSourcef (id (), al::byte_offset, fValue);
}

void sound_source::mute () noexcept
{
    _M_gMutex.lock ();
    if (_M_fVolume > .0f) ::alListenerf (al::volume, .0f);
    _M_gMutex.unlock ();
}

void sound_source::unmute () noexcept
{
    _M_gMutex.lock ();
    if (_M_fVolume > .0f) ::alListenerf (al::volume, _M_fVolume);
    _M_gMutex.unlock ();
}

void sound_source::enqueue (sound_buffer& gBuffer) noexcept
{
    uint uBuffId = gBuffer.id ();
    _M_gQueue.push_back (&gBuffer);
    ::alSourceQueueBuffers (id (), 1, &uBuffId);
}

} } } // namespace Audio
