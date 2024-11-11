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

#ifndef CPPUAL_AUDIO_FORMAT_H_
#define CPPUAL_AUDIO_FORMAT_H_
#ifdef __cplusplus

#include <cppual/types.h>

namespace cppual { namespace audio {

enum audio_format
{
    undefined =  0,
    WAV       =  1,
    MP3       =  2,
    FLAC      =  3,
    Amr       =  4,
    OGG       =  5,
    Opus      =  6,
    AIFF      =  7,
    AU        =  8,
    RAW       =  9,
    PAF       = 10,
    SVX       = 11,
    NIST      = 12,
    VOC       = 13,
    IRCAM     = 14,
    W64       = 15,
    Mat4      = 16,
    Mat5      = 17,
    PVF       = 18,
    XI        = 19,
    SDS       = 20,
    AVR       = 21,
    WaveX     = 22,
    SD2       = 23,
    CAF       = 24,
    WVE       = 25,
    MPC2K     = 26,
    RF64      = 27,
    custom
};

struct descriptor
{
    char RIFF[4];
    u32  size;

    /// file audio format name
    char format[4];
};

struct fmt_sub_chunk
{
    char chunk_id[4];
    u32  chunk_size;

    /// PCM = 1, values other than 1 indicate compression
    u16  audio_format;

    /// mono = 1, stereo = 2, etc.
    u16  num_channels;

    /// 8000, 44100 Hz
    u32  sample_rate;

    /// (sampleRate * numChannels * bitsPerSample) / 8
    u32  byte_rate;
    u16  block_align;

    /// 8 bits = 8, 16 bits = 16, etc.
    u16  bits_per_sample;
};

struct data_chunk
{
    /// contains the letters "data"
    char chunk_id[4];

    /// raw audio data size in bytes
    u32 size;
};

struct format_header
{
    descriptor    local_descriptor;
    fmt_sub_chunk fmt             ;
    data_chunk    data            ;
};

} } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_FORMAT_H_
