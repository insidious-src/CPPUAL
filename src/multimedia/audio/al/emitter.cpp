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

#include <cppual/multimedia/audio/al/emitter.h>
#include "aldef.h"

namespace cppual { namespace audio { namespace al {

constexpr int convert_emitter_type (audio::emit_type eType) noexcept
{
    switch (eType)
    {
    case audio::emit_type::statically:
        return al::static_source;
    case audio::emit_type::streaming:
        return al::streaming_source;
    default:
        return al::undetermined_source;
    }
}

constexpr audio::emit_type convert_emitter_type (int nType) noexcept
{
    switch (nType)
    {
    case al::static_source:
        return audio::emit_type::statically;
    case al::streaming_source:
        return audio::emit_type::streaming;
    default:
        return audio::emit_type::undetermined;
    }
}

// ====================================================

emit_type sound_emitter::type () const noexcept
{
    int nType = 0;
    if (valid ()) ::alGetSourcei (handle (), al::current_emitter_type, &nType);
    return convert_emitter_type (nType);
}

void sound_emitter::set_relative_to_listener (bool bRel) noexcept
{
    if (valid ()) ::alSourcei (handle (), al::is_relative_to_listener, bRel);
}

bool sound_emitter::is_relative_to_listener () const noexcept
{
    int nIsRelative = 0;
    if (valid ()) ::alGetSourceiv (handle (), al::is_relative_to_listener, &nIsRelative);
    return nIsRelative == 1;
}

void sound_emitter::set_type (audio::emit_type eType) noexcept
{
    if (valid ())
        ::alSourcei (handle (), al::current_emitter_type, convert_emitter_type (eType));
}

void sound_emitter::set_min_volume (float fValue) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::min_volume, fValue);
}

float sound_emitter::min_volume () const noexcept
{
    float fValue = .0f;
    if (valid ()) ::alGetSourcef (handle (), al::min_volume, &fValue);
    return fValue;
}

void sound_emitter::set_max_volume (float fValue) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::max_volume, fValue);
}

float sound_emitter::max_volume () const noexcept
{
    float fValue = .0f;
    if (valid ()) ::alGetSourcef (handle (), al::max_volume, &fValue);
    return fValue;
}

void sound_emitter::set_reference_distance (float fValue) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::reference_distance, fValue);
}

float sound_emitter::reference_distance () const noexcept
{
    float fValue = .0f;
    if (valid ()) ::alGetSourcef (handle (), al::reference_distance, &fValue);
    return fValue;
}

void sound_emitter::set_cancelation (float fValue) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::rolloff_factor, fValue);
}

float sound_emitter::cancelation () const noexcept
{
    float fValue = .0f;
    if (valid ()) ::alGetSourcef (handle (), al::rolloff_factor, &fValue);
    return fValue;
}

void sound_emitter::set_max_distance (float fValue) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::max_distance, fValue);
}

float sound_emitter::max_distance () const noexcept
{
    float fValue = .0f;
    if (valid ()) ::alGetSourcef (handle (), al::max_distance, &fValue);
    return fValue;
}

void sound_emitter::set_position (point3f const& gValue) noexcept
{
    if (valid ()) ::alSourcefv (handle (), al::position, &gValue.x);
}

point3f sound_emitter::position () const noexcept
{
    point3f gValue;
    if (valid ()) ::alGetSourcefv (handle (), al::position, &gValue.x);
    return gValue;
}

void sound_emitter::set_velocity (point3f const& gValue) noexcept
{
    if (valid ()) ::alSourcefv (handle (), al::velocity, &gValue.x);
}

point3f sound_emitter::velocity () const noexcept
{
    point3f gValue;
    if (valid ()) ::alGetSourcefv (handle (), al::velocity, &gValue.x);
    return gValue;
}

void sound_emitter::set_direction (point3f const& gValue) noexcept
{
    if (valid ()) ::alSourcefv (handle (), al::direction, &gValue.x);
}

point3f sound_emitter::direction () const noexcept
{
    point3f gValue;
    if (valid ()) ::alGetSourcefv (handle (), al::direction, &gValue.x);
    return gValue;
}

void sound_emitter::set_cone_inner_angle (anglef gAngle) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::cone_inner_angle, gAngle.radians);
}

anglef sound_emitter::cone_inner_angle () const noexcept
{
    anglef gAngle;
    if (valid ()) ::alGetSourcef (handle (), al::cone_inner_angle, &gAngle.radians);
    return gAngle;
}

void sound_emitter::set_cone_outer_angle (anglef gAngle) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::cone_outer_angle, gAngle.radians);
}

anglef sound_emitter::cone_outer_angle () const noexcept
{
    anglef gAngle;
    if (valid ()) ::alGetSourcef (handle (), al::cone_outer_angle, &gAngle.radians);
    return gAngle;
}

void sound_emitter::set_cone_outer_volume (float fValue) noexcept
{
    if (valid ()) ::alSourcef (handle (), al::cone_outer_volume, fValue);
}

float sound_emitter::cone_outer_volume () const noexcept
{
    float fValue = .0f;
    if (valid ()) ::alGetSourcef (handle (), al::cone_outer_volume, &fValue);
    return fValue;
}

void sound_emitter::copy_emitter_state (sound_emitter const& gObj) noexcept
{
    point3f gValue;
    anglef  gAngle;
    float   fValue;
    int     nValue;

    ::alGetSourcei (gObj.handle (), al::current_emitter_type, &nValue);
    ::alSourcei (handle (), al::current_emitter_type, nValue);

    ::alGetSourceiv (gObj.handle (), al::is_relative_to_listener, &nValue);
    ::alSourcei (handle (), al::is_relative_to_listener, nValue);

    ::alGetSourcef (gObj.handle (), al::min_volume, &fValue);
    ::alSourcef (handle (), al::min_volume, fValue);

    ::alGetSourcef (gObj.handle (), al::max_volume, &fValue);
    ::alSourcef (handle (), al::max_volume, fValue);

    ::alGetSourcef (gObj.handle (), al::reference_distance, &fValue);
    ::alSourcef (handle (), al::reference_distance, fValue);

    ::alGetSourcef (gObj.handle (), al::rolloff_factor, &fValue);
    ::alSourcef (handle (), al::rolloff_factor, fValue);

    ::alGetSourcef (gObj.handle (), al::max_distance, &fValue);
    ::alSourcef (handle (), al::max_distance, fValue);

    ::alGetSourcefv (gObj.handle (), al::position, &gValue.x);
    ::alSourcefv (handle (), al::position, &gValue.x);

    ::alGetSourcefv (gObj.handle (), al::velocity, &gValue.x);
    ::alSourcefv (handle (), al::velocity, &gValue.x);

    ::alGetSourcefv (gObj.handle (), al::direction, &gValue.x);
    ::alSourcefv (handle (), al::direction, &gValue.x);

    ::alGetSourcef (gObj.handle (), al::cone_inner_angle, &gAngle.radians);
    ::alSourcef (handle (), al::cone_inner_angle, gAngle.radians);

    ::alGetSourcef (gObj.handle (), al::cone_outer_angle, &gAngle.radians);
    ::alSourcef (handle (), al::cone_outer_angle, gAngle.radians);

    ::alGetSourcef (gObj.handle (), al::cone_outer_volume, &fValue);
    ::alSourcef (handle (), al::cone_outer_volume, fValue);
}

} } } // namespace Audio
