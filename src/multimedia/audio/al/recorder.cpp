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

#include <cppual/multimedia/audio/al/recorder.h>

#include <AL/alc.h>

#include "aldef.h"

namespace cppual { namespace audio { namespace al {

bool sound_stream_recorder::record () noexcept
{
    if (_M_gDevice->valid () and !_M_bIsRecording)
    {
        ::alcCaptureStart (_M_gDevice->handle<ALCdevice*> ());
        return _M_bIsRecording = true;
    }

    return false;
}

void sound_stream_recorder::stop () noexcept
{
    if (_M_bIsRecording)
    {
        ::alcCaptureStop (_M_gDevice->handle<ALCdevice*> ());
        _M_bIsRecording = false;
    }
}

void sound_stream_recorder::set_samples (int nSamples) noexcept
{
    if (_M_gDevice->valid ())
    {
        ::alcCaptureSamples (_M_gDevice->handle<ALCdevice*> (),
                             nullptr,
                             nSamples);
        _M_nSamples = nSamples;
    }
}

sound_stream_recorder::sound_stream_recorder (capture_device& gDevice) noexcept
: _M_gDevice (&gDevice),
  _M_nSamples (),
  _M_bIsRecording ()
{ }

bool sound_stream_recorder::is_available () noexcept
{
    return (audio_device::is_extension_supported ("ALC_EXT_CAPTURE") or
            audio_device::is_extension_supported ("ALC_EXT_capture"));
}

} } } // namespace Audio
