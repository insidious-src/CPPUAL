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

#ifndef CPPUAL_AUDIO_RECORDER_H_
#define CPPUAL_AUDIO_RECORDER_H_
#ifdef __cplusplus


#include <cppual/multimedia/audio/al/aldevice.h>

namespace cppual { namespace audio { namespace al {

class SHARED_API sound_stream_recorder : public non_copyable
{
public:
    sound_stream_recorder () = delete;
    explicit sound_stream_recorder (capture_device&) noexcept;
    static bool is_available () noexcept;

    bool record      () noexcept;
    void stop        () noexcept;
    void set_samples (int samples) noexcept;

    inline capture_device& get_device ()       noexcept { return *_M_gDevice ; }
    inline int             samples    () const noexcept { return  _M_nSamples; }

private:
    capture_device* _M_gDevice     ;
    int             _M_nSamples    ;
    bool            _M_bIsRecording;
};

} } } // namespace Audio

#endif // __cplusplus
#endif // CPPUAL_AUDIO_RECORDER_H_
