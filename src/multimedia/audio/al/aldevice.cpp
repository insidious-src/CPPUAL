/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#include <cppual/multimedia/audio/al/aldevice.h>
#include <AL/alc.h>
#include "aldef.h"

using std::string;

namespace cppual { namespace Audio { namespace AL {

namespace { // optimize for internal unit usage

struct Internal final : NonConstructible
{ static thread_local Instance* currentContext; };

thread_local Instance* Internal::currentContext = nullptr;

} // anonymous

// ====================================================

inline int convertQuality (OutputFormat eFormat, SoundQuality eQuality) noexcept
{
    switch (eFormat)
    {
    case OutputFormat::Mono:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::Mono8;
        case SoundQuality::Medium:
            return AL::Mono16;
        case SoundQuality::High:
            return AL::Mono32;
        }
        break;
    case OutputFormat::Stereo:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::Stereo8;
        case SoundQuality::Medium:
            return AL::Stereo16;
        case SoundQuality::High:
            return AL::Stereo32;
        }
        break;
    case OutputFormat::Rear:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::Rear8;
        case SoundQuality::Medium:
            return AL::Rear16;
        case SoundQuality::High:
            return AL::Rear32;
        }
        break;
    case OutputFormat::Quad:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::Quad8;
        case SoundQuality::Medium:
            return AL::Quad16;
        case SoundQuality::High:
            return AL::Quad32;
        }
        break;
    case OutputFormat::CH51:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::CH51_8;
        case SoundQuality::Medium:
            return AL::CH51_16;
        case SoundQuality::High:
            return AL::CH51_32;
        }
        break;
    case OutputFormat::CH61:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::CH61_8;
        case SoundQuality::Medium:
            return AL::CH61_16;
        case SoundQuality::High:
            return AL::CH61_32;
        }
        break;
    case OutputFormat::CH71:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::CH71_8;
        case SoundQuality::Medium:
            return AL::CH71_16;
        case SoundQuality::High:
            return AL::CH71_32;
        }
    default:
        switch (eQuality)
        {
        case SoundQuality::Low:
            return AL::Stereo8;
        case SoundQuality::Medium:
            return AL::Stereo16;
        case SoundQuality::High:
            return AL::Stereo32;
        }
        break;
    }

    return 0;
}

inline DistanceModel convertDistanceModel (int nModel) noexcept
{
    switch (nModel)
    {
    case AL::InverseDistance:
        return DistanceModel::InverseDistance;
    case AL::InverseDistanceClamped:
        return DistanceModel::InverseDistanceClamped;
    case AL::LinearDistance:
        return DistanceModel::LinearDistance;
    case AL::LinearDistanceClamped:
        return DistanceModel::LinearDistanceClamped;
    case AL::ExponentDistance:
        return DistanceModel::ExponentDistance;
    case AL::ExponentDistanceClamped:
        return DistanceModel::ExponentDistanceClamped;
    default:
        return DistanceModel::Plain;
    }
}

inline int convertDistanceModel (DistanceModel eModel) noexcept
{
    switch (eModel)
    {
    case DistanceModel::Plain:
        return 0;
    case DistanceModel::InverseDistance:
        return AL::InverseDistance;
    case DistanceModel::InverseDistanceClamped:
        return AL::InverseDistanceClamped;
    case DistanceModel::LinearDistance:
        return AL::LinearDistance;
    case DistanceModel::LinearDistanceClamped:
        return AL::LinearDistanceClamped;
    case DistanceModel::ExponentDistance:
        return AL::ExponentDistance;
    default:
        return AL::ExponentDistanceClamped;
    }
}

// ====================================================

bool AudioDevice::isExtensionSupported (string const& gExt) noexcept
{
    return alIsExtensionPresent (gExt.c_str ());
}

bool AudioDevice::isExtensionPresent (string const& gExt) noexcept
{
    if (m_pDevObj)
        return alcIsExtensionPresent
                (static_cast<ALCdevice*> (m_pDevObj), gExt.c_str ());
    return false;
}

// ====================================================

PlaybackDevice::PlaybackDevice () noexcept
: AudioDevice ("Default",
               alcOpenDevice (nullptr),
               DeviceType::Playback)
{ }

PlaybackDevice::PlaybackDevice (string const& gName) noexcept
: AudioDevice (std::move (gName),
               alcOpenDevice (gName.c_str ()),
               DeviceType::Playback)
{ }

PlaybackDevice::~PlaybackDevice () noexcept
{
    if (m_pDevObj) alcCloseDevice (static_cast<ALCdevice*> (m_pDevObj));
}

// ====================================================

CaptureDevice::CaptureDevice () noexcept
: AudioDevice ("Default",
               alcCaptureOpenDevice (nullptr,
                                     22050,
                                     convertQuality (OutputFormat::Stereo,
                                                     SoundQuality::Low),
                                     0),
               DeviceType::Capture),
  m_eQuality (SoundQuality::Low),
  m_eFormat (OutputFormat::Stereo)
{
}

CaptureDevice::CaptureDevice (string const& gName,
                              uint          uFreq,
                              OutputFormat  eFormat,
                              SoundQuality  eQuality) noexcept
: AudioDevice (gName,
               alcCaptureOpenDevice (gName.c_str (),
                                     uFreq,
                                     convertQuality (eFormat, eQuality),
                                     0),
               DeviceType::Capture),
  m_eQuality (eQuality),
  m_eFormat (eFormat)
{ }

CaptureDevice::~CaptureDevice () noexcept
{
    if (m_pDevObj) alcCaptureCloseDevice (static_cast<ALCdevice*> (m_pDevObj));
}

// ====================================================

Instance::Instance (PlaybackDevice& gDevice, bool bMakeCurrent) noexcept
: m_gDevice (gDevice),
  m_pDevContext (gDevice.isValid () ?
                     alcCreateContext
                     (static_cast<ALCdevice*> (gDevice.object ()), nullptr) :
                     nullptr)
{
    if (m_pDevContext and bMakeCurrent and
            alcMakeContextCurrent (static_cast<ALCcontext*> (m_pDevContext)) == 1)
        Internal::currentContext = this;
}

Instance::~Instance () noexcept
{
    if (m_pDevContext)
    {
        invalidate ();
        alcDestroyContext (static_cast<ALCcontext*> (m_pDevContext));
    }
}

bool Instance::use () noexcept
{
    if (Internal::currentContext != this and
            alcMakeContextCurrent (static_cast<ALCcontext*> (m_pDevContext)) == 1)
    {
        Internal::currentContext = this;
        return true;
    }

    return false;
}

void Instance::invalidate () noexcept
{
    if (Internal::currentContext == this and alcMakeContextCurrent (nullptr) == 1)
        Internal::currentContext  = nullptr;
}

bool Instance::setCapability (int nCap, bool bEnable) noexcept
{
    Instance* pPrevContext = Internal::currentContext;

    if (m_pDevContext and use ())
    {
        bEnable ? alEnable (nCap) : alDisable (nCap);

        if (pPrevContext and pPrevContext != this)
        {
            if (alcMakeContextCurrent
                    (static_cast<ALCcontext*> (pPrevContext->m_pDevContext)) == 1)
                Internal::currentContext = pPrevContext;
        }

        return true;
    }

    return false;
}

bool Instance::hasCapability (int nCap) noexcept
{
    Instance* pPrevContext = Internal::currentContext;

    if (m_pDevContext and use ())
    {
        bool bHasCap = alIsEnabled (nCap) == 1;

        if (pPrevContext and pPrevContext != this)
        {
            if (alcMakeContextCurrent
                    (static_cast<ALCcontext*> (pPrevContext->m_pDevContext)) == 1)
                Internal::currentContext = pPrevContext;
        }

        return bHasCap;
    }

    return false;
}

void Instance::process () noexcept
{
    if (m_pDevContext) alcProcessContext (static_cast<ALCcontext*> (m_pDevContext));
}

void Instance::suspend () noexcept
{
    if (m_pDevContext) alcSuspendContext (static_cast<ALCcontext*> (m_pDevContext));
}

DistanceModel Instance::distanceModel () noexcept
{
    int nModel = 0;
    alGetIntegerv (AL::CurrentDistanceModel, &nModel);
    return convertDistanceModel (nModel);
}

void Instance::setDistanceModel (DistanceModel eModel) noexcept
{
    alDistanceModel (convertDistanceModel (eModel));
}

Instance* Instance::current () noexcept
{
    return Internal::currentContext;
}

string Instance::label (StringQuery) noexcept
{
    return { };
}

Instance::ext_list Instance::extensions () noexcept
{
    return { };
}

void Instance::setDopplerFactor (float) noexcept
{
}

float Instance::dopplerFactor () noexcept
{
    return .0f;
}

void Instance::setSpeedOfSound (float) noexcept
{
}

float Instance::speedOfSound () noexcept
{
    return .0f;
}

} } } // namespace Audio
