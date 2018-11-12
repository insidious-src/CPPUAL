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

#include <cppual/multimedia/audio/al/alsource.h>
#include "aldef.h"

using std::mutex;

namespace cppual { namespace Audio { namespace AL {

inline int convertEmitterState (Audio::SoundState eState) noexcept
{
    switch (eState)
    {
    case Audio::SoundState::Unavailable:
        return 0;
    case Audio::SoundState::Initial:
        return AL::SoundInitial;
    case Audio::SoundState::Playing:
        return AL::SoundPlaying;
    case Audio::SoundState::Paused:
        return AL::SoundPaused;
    case Audio::SoundState::Stopped:
        return AL::SoundStopped;
    }

    return 0;
}

inline Audio::SoundState convertEmitterState (int nState) noexcept
{
    switch (nState)
    {
    case AL::SoundInitial:
        return Audio::SoundState::Initial;
    case AL::SoundPlaying:
        return Audio::SoundState::Playing;
    case AL::SoundPaused:
        return Audio::SoundState::Paused;
    case AL::SoundStopped:
        return Audio::SoundState::Stopped;
    }

    return Audio::SoundState::Unavailable;
}

// ====================================================

SoundSource::SoundSource () noexcept
: Object        (ObjectType::Source),
  m_gMutex      (),
  m_pBuffer     (),
  m_uBufferSlot (),
  m_fVolume     ()
{ }

SoundSource::SoundSource (SoundBuffer& gBuffer) noexcept
: Object        (ObjectType::Source),
  m_gMutex      (),
  m_pBuffer     (),
  m_uBufferSlot (gBuffer.m_gSources.size ()),
  m_fVolume     ()
{
    if (id () and gBuffer.isValid ())
    {
        m_pBuffer = &gBuffer;
        gBuffer.m_gSources.push_back (this); // attach
    }
}

SoundSource::SoundSource (SoundSource&& gObj) noexcept
: Object        (gObj),
  m_gMutex      (),
  m_pBuffer     (gObj.m_pBuffer),
  m_uBufferSlot (gObj.m_uBufferSlot),
  m_fVolume     (gObj.m_fVolume)
{
    gObj.m_pBuffer = nullptr;
    if (m_pBuffer) m_pBuffer->m_gSources[m_uBufferSlot] = this;
}

SoundSource::SoundSource (SoundSource const& gObj) noexcept
: Object        (gObj),
  m_gMutex      (),
  m_pBuffer     (gObj.m_pBuffer),
  m_uBufferSlot (gObj.m_pBuffer ? gObj.m_pBuffer->m_gSources.size () : 0),
  m_fVolume     (gObj.m_fVolume)
{
    setVolume       (gObj.getVolume    ());
    setLooping      (gObj.isLooping    ());
    setPlayingSpeed (gObj.playingSpeed ());
    if (m_pBuffer) m_pBuffer->m_gSources.push_back (this);
}

SoundSource& SoundSource::operator = (SoundSource&& gObj) noexcept
{
    m_pBuffer      = gObj.m_pBuffer;
    m_uBufferSlot  = gObj.m_uBufferSlot;
    m_fVolume      = gObj.m_fVolume;
    gObj.m_pBuffer = nullptr;

    if (m_pBuffer) m_pBuffer->m_gSources[m_uBufferSlot] = this;
    return *this;
}

SoundSource& SoundSource::operator = (SoundSource const& gObj) noexcept
{
    if (id ())
    {
        if (gObj.m_pBuffer and gObj.m_pBuffer->isValid ())
        {
            m_pBuffer     = gObj.m_pBuffer;
            m_uBufferSlot = gObj.m_pBuffer->m_gSources.size ();
            gObj.m_pBuffer->m_gSources.push_back (this);
        }

        setVolume       (gObj.getVolume    ());
        setLooping      (gObj.isLooping    ());
        setPlayingSpeed (gObj.playingSpeed ());
    }

    return *this;
}

SoundSource::~SoundSource () noexcept
{
    if (id ())
    {
        if (m_pBuffer)
        {
            stop ();
            m_pBuffer->m_gSources[m_uBufferSlot] = nullptr; // detach
        }
    }
}

SoundState SoundSource::state () const noexcept
{
    int nState = 0;
    alGetSourcei (id (), AL::CurrentEmitterState, &nState);
    return convertEmitterState (nState);
}

void SoundSource::play () noexcept
{
    int nState = 0;
    alGetSourcei (id (), AL::CurrentEmitterState, &nState);
    if (m_pBuffer and nState and nState != AL::SoundPlaying) alSourcePlay (id ());
}

void SoundSource::replay () noexcept
{
    int nState = 0;
    alGetSourcei (id (), AL::CurrentEmitterState, &nState);

    if (nState and m_pBuffer)
    {
        alSourceRewind (id ());
        if (nState != AL::SoundPlaying) alSourcePlay (id ());
    }
}

void SoundSource::pause () noexcept
{
    int nState = 0;
    alGetSourcei (id (), AL::CurrentEmitterState, &nState);
    if (nState == AL::SoundPlaying) alSourcePause (id ());
}

void SoundSource::stop () noexcept
{
    int nState = 0;
    alGetSourcei (id (), AL::CurrentEmitterState, &nState);

    if ((nState == AL::SoundPlaying or nState == AL::SoundPaused))
        alSourceStop (id ());
}

void SoundSource::rewind () noexcept
{
    if (m_pBuffer and m_pBuffer->isValid () and state () != SoundState::Initial)
        alSourceRewind (id ());
}

bool SoundSource::isPlaying () const noexcept
{
    int nState = 0;
    alGetSourcei (id (), AL::CurrentEmitterState, &nState);
    return nState == AL::SoundPlaying;
}

void SoundSource::setLooping (bool bLoop) noexcept
{
    if (id ()) alSourcei (id (), AL::Looping, bLoop);
}

bool SoundSource::isLooping () const noexcept
{
    int nIsLooping = 0;
    if (id ()) alGetSourcei (id (), AL::Looping, &nIsLooping);
    return nIsLooping == true;
}

bool SoundSource::attach (SoundBuffer& gBuffer) noexcept
{
    if (!id () or !gBuffer.isValid ()) return false;

    m_pBuffer     = &gBuffer;
    m_uBufferSlot =  gBuffer.m_gSources.size ();
    gBuffer.m_gSources.push_back (this); // attach
    return true;
}

void SoundSource::detach () noexcept
{
    if (m_pBuffer)
    {
        m_pBuffer->m_gSources[m_uBufferSlot] = nullptr;
        onDetach ();
    }
}

void SoundSource::onDetach () noexcept
{
    stop ();
    alSourcei (id (), AL::Buffer, 0);
    m_pBuffer = nullptr;
}

void SoundSource::setVolume (float fValue) noexcept
{
    if (id ())
    {
        alSourcef       (id (), AL::Volume, fValue);
        m_gMutex.lock   ();
        m_fVolume = fValue;
        m_gMutex.unlock ();
    }
}

float SoundSource::getVolume () const noexcept
{
    std::lock_guard<mutex> gLock (m_gMutex);
    return m_fVolume;
}

void SoundSource::setPlayingSpeed (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::Pitch, fValue);
}

float SoundSource::playingSpeed () const noexcept
{
    float fValue = .0f;
    if (id ()) alGetSourcef (id (), AL::Pitch, &fValue);
    return fValue;
}

void SoundSource::setPlayingOffset (std::chrono::seconds nValue) noexcept
{
    if (id ()) alSourcei (id (), AL::SecOffset, static_cast<int> (nValue.count ()));
}

int SoundSource::playingOffset () noexcept
{
    int nValue = 0;
    if (id ()) alGetSourcei (id (), AL::SecOffset, &nValue);
    return nValue;
}

void SoundSource::setSampleOffset (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::SampleOffset, fValue);
}

void SoundSource::setByteOffset (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::ByteOffset, fValue);
}

void SoundSource::mute () noexcept
{
    m_gMutex.lock ();
    if (m_fVolume > .0f) alListenerf (AL::Volume, .0f);
    m_gMutex.unlock ();
}

void SoundSource::unmute () noexcept
{
    m_gMutex.lock ();
    if (m_fVolume > .0f) alListenerf (AL::Volume, m_fVolume);
    m_gMutex.unlock ();
}

void SoundSource::enqueue (SoundBuffer& gBuffer) noexcept
{
    uint uBuffId = gBuffer.id ();
    m_gQueue.push_back (&gBuffer);
    alSourceQueueBuffers (id (), 1, &uBuffId);
}

} } } // namespace Audio
