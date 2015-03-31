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

#include <cppual/multimedia/audio/al/emitter.h>
#include "aldef.h"

namespace cppual { namespace Audio { namespace AL {

inline static int convertEmitterType (Audio::EmitType eType) noexcept
{
    switch (eType)
    {
    case Audio::EmitType::Undetermined:
        return AL::Undetermined;
    case Audio::EmitType::Static:
        return AL::Static;
    case Audio::EmitType::Streaming:
        return AL::Streaming;
    }

    return 0;
}

inline static Audio::EmitType convertEmitterType (int nType) noexcept
{
    switch (nType)
    {
    case AL::Static:
        return Audio::EmitType::Static;
    case AL::Streaming:
        return Audio::EmitType::Streaming;
    }

    return Audio::EmitType::Undetermined;
}

// ====================================================

SoundEmitter::SoundEmitter (SoundEmitter const& gObj) noexcept
: SoundSource (gObj)
{
    if (id () and gObj.id ()) copyEmitterState (gObj);
}

SoundEmitter& SoundEmitter::operator = (SoundEmitter const& gObj) noexcept
{
    if (id () and gObj.id ()) copyEmitterState (gObj);
    return *this;
}

EmitType SoundEmitter::getType () const noexcept
{
    int nType = 0;
    if (id ()) alGetSourcei (id (), AL::CurrentEmitterType, &nType);
    return convertEmitterType (nType);
}

void SoundEmitter::setRelativeToListener (bool bRel) noexcept
{
    if (id ()) alSourcei (id (), AL::IsRelativeToListener, bRel);
}

bool SoundEmitter::isRelativeToListener () const noexcept
{
    int nIsRelative = 0;
    if (id ()) alGetSourceiv (id (), AL::IsRelativeToListener, &nIsRelative);
    return nIsRelative == 1;
}

void SoundEmitter::setType (Audio::EmitType eType) noexcept
{
    if (id ())
        alSourcei (id (), AL::CurrentEmitterType, convertEmitterType (eType));
}

void SoundEmitter::setMinVolume (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::MinVolume, fValue);
}

float SoundEmitter::getMinVolume () const noexcept
{
    float fValue = .0f;
    if (id ()) alGetSourcef (id (), AL::MinVolume, &fValue);
    return fValue;
}

void SoundEmitter::setMaxVolume (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::MaxVolume, fValue);
}

float SoundEmitter::getMaxVolume () const noexcept
{
    float fValue = .0f;
    if (id ()) alGetSourcef (id (), AL::MaxVolume, &fValue);
    return fValue;
}

void SoundEmitter::setReferenceDistance (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::ReferenceDistance, fValue);
}

float SoundEmitter::getReferenceDistance () const noexcept
{
    float fValue = .0f;
    if (id ()) alGetSourcef (id (), AL::ReferenceDistance, &fValue);
    return fValue;
}

void SoundEmitter::setCancelation (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::RolloffFactor, fValue);
}

float SoundEmitter::getCancelation () const noexcept
{
    float fValue = .0f;
    if (id ()) alGetSourcef (id (), AL::RolloffFactor, &fValue);
    return fValue;
}

void SoundEmitter::setMaxDistance (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::MaxDistance, fValue);
}

float SoundEmitter::getMaxDistance () const noexcept
{
    float fValue = .0f;
    if (id ()) alGetSourcef (id (), AL::MaxDistance, &fValue);
    return fValue;
}

void SoundEmitter::setPosition (point3f const& gValue) noexcept
{
    if (id ()) alSourcefv (id (), AL::Position, &gValue.x);
}

point3f SoundEmitter::getPosition () const noexcept
{
    point3f gValue;
    if (id ()) alGetSourcefv (id (), AL::Position, &gValue.x);
    return gValue;
}

void SoundEmitter::setVelocity (point3f const& gValue) noexcept
{
    if (id ()) alSourcefv (id (), AL::Velocity, &gValue.x);
}

point3f SoundEmitter::getVelocity () const noexcept
{
    point3f gValue;
    if (id ()) alGetSourcefv (id (), AL::Velocity, &gValue.x);
    return gValue;
}

void SoundEmitter::setDirection (point3f const& gValue) noexcept
{
    if (id ()) alSourcefv (id (), AL::Direction, &gValue.x);
}

point3f SoundEmitter::getDirection () const noexcept
{
    point3f gValue;
    if (id ()) alGetSourcefv (id (), AL::Direction, &gValue.x);
    return gValue;
}

void SoundEmitter::setConeInnerAngle (anglef gAngle) noexcept
{
    if (id ()) alSourcef (id (), AL::ConeInnerAngle, gAngle.radians);
}

anglef SoundEmitter::getConeInnerAngle () const noexcept
{
    anglef gAngle;
    if (id ()) alGetSourcef (id (), AL::ConeInnerAngle, &gAngle.radians);
    return gAngle;
}

void SoundEmitter::setConeOuterAngle (anglef gAngle) noexcept
{
    if (id ()) alSourcef (id (), AL::ConeOuterAngle, gAngle.radians);
}

anglef SoundEmitter::getConeOuterAngle () const noexcept
{
    anglef gAngle;
    if (id ()) alGetSourcef (id (), AL::ConeOuterAngle, &gAngle.radians);
    return gAngle;
}

void SoundEmitter::setConeOuterVolume (float fValue) noexcept
{
    if (id ()) alSourcef (id (), AL::ConeOuterVolume, fValue);
}

float SoundEmitter::getConeOuterVolume () const noexcept
{
    float fValue = .0f;
    if (id ()) alGetSourcef (id (), AL::ConeOuterVolume, &fValue);
    return fValue;
}

void SoundEmitter::copyEmitterState (SoundEmitter const& gObj) noexcept
{
    point3f gValue;
    anglef  gAngle;
    float   fValue;
    int     nValue;

    alGetSourcei (gObj.id (), AL::CurrentEmitterType, &nValue);
    alSourcei (id (), AL::CurrentEmitterType, nValue);

    alGetSourceiv (gObj.id (), AL::IsRelativeToListener, &nValue);
    alSourcei (id (), AL::IsRelativeToListener, nValue);

    alGetSourcef (gObj.id (), AL::MinVolume, &fValue);
    alSourcef (id (), AL::MinVolume, fValue);

    alGetSourcef (gObj.id (), AL::MaxVolume, &fValue);
    alSourcef (id (), AL::MaxVolume, fValue);

    alGetSourcef (gObj.id (), AL::ReferenceDistance, &fValue);
    alSourcef (id (), AL::ReferenceDistance, fValue);

    alGetSourcef (gObj.id (), AL::RolloffFactor, &fValue);
    alSourcef (id (), AL::RolloffFactor, fValue);

    alGetSourcef (gObj.id (), AL::MaxDistance, &fValue);
    alSourcef (id (), AL::MaxDistance, fValue);

    alGetSourcefv (gObj.id (), AL::Position, &gValue.x);
    alSourcefv (id (), AL::Position, &gValue.x);

    alGetSourcefv (gObj.id (), AL::Velocity, &gValue.x);
    alSourcefv (id (), AL::Velocity, &gValue.x);

    alGetSourcefv (gObj.id (), AL::Direction, &gValue.x);
    alSourcefv (id (), AL::Direction, &gValue.x);

    alGetSourcef (gObj.id (), AL::ConeInnerAngle, &gAngle.radians);
    alSourcef (id (), AL::ConeInnerAngle, gAngle.radians);

    alGetSourcef (gObj.id (), AL::ConeOuterAngle, &gAngle.radians);
    alSourcef (id (), AL::ConeOuterAngle, gAngle.radians);

    alGetSourcef (gObj.id (), AL::ConeOuterVolume, &fValue);
    alSourcef (id (), AL::ConeOuterVolume, fValue);
}

} } } // namespace Audio
