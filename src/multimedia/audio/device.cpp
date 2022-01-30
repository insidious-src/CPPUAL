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

#include <cppual/multimedia/audio/device.h>

#include <regex>

namespace cppual { namespace audio {

namespace { namespace Internal { // optimize for internal unit usage



} } // anonymous namespace Internal

device::device (string_type const&, device_type)
{

}

bool device::is_extension_supported (string_type const&) noexcept
{
    return false;
}

bool device::is_extension_present (string_type const&) noexcept
{
    return false;
}

// ====================================================

//CaptureDevice::CaptureDevice (string const&,
//                              uint,
//                              OutputFormat,
//                              SoundQuality) noexcept
//{

//}

//CaptureDevice::CaptureDevice () noexcept
//{

//}

capture_device::~capture_device () noexcept
{

}

bool capture_device::busy () noexcept
{
    return false;
}

// ====================================================

//PlaybackDevice::PlaybackDevice () noexcept
//{

//}

//PlaybackDevice::PlaybackDevice (string const&) noexcept
//{

//}

playback_device::~playback_device () noexcept
{

}

// ====================================================

context::context (playback_device&, bool) noexcept
{

}

context::~context () noexcept
{

}

void context::use ()
{

}

void context::invalidate () noexcept
{

}

bool context::set_capability (int, bool) noexcept
{
    return false;
}

bool context::has_capability (int) noexcept
{
    return false;
}

void context::process () noexcept
{

}

void context::suspend () noexcept
{

}

} } // namespace Audio
