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

#ifndef CPPUAL_AUDIO_DEFINES_H_
#define CPPUAL_AUDIO_DEFINES_H_
#ifdef __cplusplus

#include <AL/al.h>
#include <AL/alext.h>

namespace cppual { namespace Audio { namespace AL {

enum
{
    // errors
    IllegalEnum             = AL_ILLEGAL_ENUM,
    IllegalCommand          = AL_ILLEGAL_COMMAND,
    Invalid                 = AL_INVALID,
    InvalidEnum             = AL_INVALID_ENUM,
    InvalidValue            = AL_INVALID_VALUE,
    InvalidName             = AL_INVALID_NAME,
    InvalidOperation        = AL_INVALID_OPERATION,
    OutOfMemory             = AL_OUT_OF_MEMORY,

    // operations
    CurrentDistanceModel    = AL_DISTANCE_MODEL,
    SpeedOfSound            = AL_SPEED_OF_SOUND,
    DopplerFactor           = AL_DOPPLER_FACTOR,
    DopplerVelocity         = AL_DOPPLER_VELOCITY,
    CurrentEmitterType      = AL_SOURCE_TYPE,
    CurrentEmitterState     = AL_SOURCE_STATE,
    IsRelativeToListener    = AL_SOURCE_RELATIVE,
    SoundInitial            = AL_INITIAL,
    SoundPlaying            = AL_PLAYING,
    SoundPaused             = AL_PAUSED,
    SoundStopped            = AL_STOPPED,
    Looping                 = AL_LOOPING,
    Volume                  = AL_GAIN,
    MinVolume               = AL_MIN_GAIN,
    MaxVolume               = AL_MAX_GAIN,
    ConeOuterVolume         = AL_CONE_OUTER_GAIN,
    MaxDistance             = AL_MAX_DISTANCE,
    Pitch                   = AL_PITCH,
    Position                = AL_POSITION,
    Direction               = AL_DIRECTION,
    Velocity                = AL_VELOCITY,
    Orientation             = AL_ORIENTATION,
    Buffer                  = AL_BUFFER,
    BuffersProcessed        = AL_BUFFERS_PROCESSED,
    BuffersQueued           = AL_BUFFERS_QUEUED,
    ConeOuterAngle          = AL_CONE_OUTER_ANGLE,
    ConeInnerAngle          = AL_CONE_INNER_ANGLE,
    SecOffset               = AL_SEC_OFFSET,
    SampleOffset            = AL_SAMPLE_OFFSET,
    ByteOffset              = AL_BYTE_OFFSET,
    RolloffFactor           = AL_ROLLOFF_FACTOR,
    ReferenceDistance       = AL_REFERENCE_DISTANCE,
    Frequency               = AL_FREQUENCY,
    Channels                = AL_CHANNELS,
    Bits                    = AL_BITS,
    Size                    = AL_SIZE,

    // formats
    Mono8                   = AL_FORMAT_MONO8,
    Mono16                  = AL_FORMAT_MONO16,
    Mono32                  = AL_FORMAT_MONO_FLOAT32,
    Stereo8                 = AL_FORMAT_STEREO8,
    Stereo16                = AL_FORMAT_STEREO16,
    Stereo32                = AL_FORMAT_STEREO_FLOAT32,
    Rear8                   = AL_FORMAT_REAR8,
    Rear16                  = AL_FORMAT_REAR16,
    Rear32                  = AL_FORMAT_REAR32,
    Quad8                   = AL_FORMAT_QUAD8,
    Quad16                  = AL_FORMAT_QUAD16,
    Quad32                  = AL_FORMAT_QUAD32,
    CH51_8                  = AL_FORMAT_51CHN8,
    CH51_16                 = AL_FORMAT_51CHN16,
    CH51_32                 = AL_FORMAT_51CHN32,
    CH61_8                  = AL_FORMAT_61CHN8,
    CH61_16                 = AL_FORMAT_61CHN16,
    CH61_32                 = AL_FORMAT_61CHN32,
    CH71_8                  = AL_FORMAT_71CHN8,
    CH71_16                 = AL_FORMAT_71CHN16,
    CH71_32                 = AL_FORMAT_71CHN32,
    Undetermined            = AL_UNDETERMINED,
    Static                  = AL_STATIC,
    Streaming               = AL_STREAMING,
    InverseDistance         = AL_INVERSE_DISTANCE,
    InverseDistanceClamped  = AL_INVERSE_DISTANCE_CLAMPED,
    LinearDistance          = AL_LINEAR_DISTANCE,
    LinearDistanceClamped   = AL_LINEAR_DISTANCE_CLAMPED,
    ExponentDistance        = AL_EXPONENT_DISTANCE,
    ExponentDistanceClamped = AL_EXPONENT_DISTANCE_CLAMPED,

    // primary
    Renderer                = AL_RENDERER,
    Vendor                  = AL_VENDOR,
    Version                 = AL_VERSION,
    Extensions              = AL_EXTENSIONS,
    Unused                  = AL_UNUSED
};

} } } // AL

#endif // __cplusplus
#endif // CPPUAL_AUDIO_DEFINES_H_
