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

#ifndef CPPUAL_AUDIO_DEFINES_H_
#define CPPUAL_AUDIO_DEFINES_H_
#ifdef __cplusplus

#include <AL/al.h>
#include <AL/alext.h>

namespace cppual { namespace audio { namespace al {

enum
{
    // errors
    illegal_enum              = AL_ILLEGAL_ENUM,
    illegal_command           = AL_ILLEGAL_COMMAND,
    invalid                   = AL_INVALID,
    invalid_enum              = AL_INVALID_ENUM,
    invalid_value             = AL_INVALID_VALUE,
    invalid_name              = AL_INVALID_NAME,
    invalid_operation         = AL_INVALID_OPERATION,
    out_of_memory             = AL_OUT_OF_MEMORY,

    // operations
    current_distance_model    = AL_DISTANCE_MODEL,
    speed_of_sound            = AL_SPEED_OF_SOUND,
    doppler_factor            = AL_DOPPLER_FACTOR,
    doppler_velocity          = AL_DOPPLER_VELOCITY,
    current_emitter_type      = AL_SOURCE_TYPE,
    current_emitter_state     = AL_SOURCE_STATE,
    is_relative_to_listener   = AL_SOURCE_RELATIVE,
    sound_initial             = AL_INITIAL,
    sound_playing             = AL_PLAYING,
    sound_paused              = AL_PAUSED,
    sound_stopped             = AL_STOPPED,
    looping                   = AL_LOOPING,
    volume                    = AL_GAIN,
    min_volume                = AL_MIN_GAIN,
    max_volume                = AL_MAX_GAIN,
    cone_outer_volume         = AL_CONE_OUTER_GAIN,
    max_distance              = AL_MAX_DISTANCE,
    pitch                     = AL_PITCH,
    position                  = AL_POSITION,
    direction                 = AL_DIRECTION,
    velocity                  = AL_VELOCITY,
    orientation               = AL_ORIENTATION,
    buffer                    = AL_BUFFER,
    buffers_processed         = AL_BUFFERS_PROCESSED,
    buffers_queued            = AL_BUFFERS_QUEUED,
    cone_outer_angle          = AL_CONE_OUTER_ANGLE,
    cone_inner_angle          = AL_CONE_INNER_ANGLE,
    sec_offset                = AL_SEC_OFFSET,
    sample_offset             = AL_SAMPLE_OFFSET,
    byte_offset               = AL_BYTE_OFFSET,
    rolloff_factor            = AL_ROLLOFF_FACTOR,
    reference_distance        = AL_REFERENCE_DISTANCE,
    frequency                 = AL_FREQUENCY,
    channels                  = AL_CHANNELS,
    bits                      = AL_BITS,
    size                      = AL_SIZE,

    // formats
    mono8                     = AL_FORMAT_MONO8,
    mono16                    = AL_FORMAT_MONO16,
    mono32                    = AL_FORMAT_MONO_FLOAT32,
    stereo8                   = AL_FORMAT_STEREO8,
    stereo16                  = AL_FORMAT_STEREO16,
    stereo32                  = AL_FORMAT_STEREO_FLOAT32,
    rear8                     = AL_FORMAT_REAR8,
    rear16                    = AL_FORMAT_REAR16,
    rear32                    = AL_FORMAT_REAR32,
    quad8                     = AL_FORMAT_QUAD8,
    quad16                    = AL_FORMAT_QUAD16,
    quad32                    = AL_FORMAT_QUAD32,
    ch51_8                    = AL_FORMAT_51CHN8,
    ch51_16                   = AL_FORMAT_51CHN16,
    ch51_32                   = AL_FORMAT_51CHN32,
    ch61_8                    = AL_FORMAT_61CHN8,
    ch61_16                   = AL_FORMAT_61CHN16,
    ch61_32                   = AL_FORMAT_61CHN32,
    ch71_8                    = AL_FORMAT_71CHN8,
    ch71_16                   = AL_FORMAT_71CHN16,
    ch71_32                   = AL_FORMAT_71CHN32,
    undetermined_source       = AL_UNDETERMINED,
    static_source             = AL_STATIC,
    streaming_source          = AL_STREAMING,
    inverse_distance          = AL_INVERSE_DISTANCE,
    inverse_distance_clamped  = AL_INVERSE_DISTANCE_CLAMPED,
    linear_distance           = AL_LINEAR_DISTANCE,
    linear_distance_clamped   = AL_LINEAR_DISTANCE_CLAMPED,
    exponent_distance         = AL_EXPONENT_DISTANCE,
    exponent_distance_clamped = AL_EXPONENT_DISTANCE_CLAMPED,

    // primary
    renderer                  = AL_RENDERER,
    vendor                    = AL_VENDOR,
    version                   = AL_VERSION,
    extensions                = AL_EXTENSIONS,
    unused                    = AL_UNUSED
};

} } } // AL

#endif // __cplusplus
#endif // CPPUAL_AUDIO_DEFINES_H_
