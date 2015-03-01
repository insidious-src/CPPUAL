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

#include <regex>
#include <cppual/multimedia/audio/device.h>

namespace cppual { namespace Audio {

namespace { namespace Internal { // optimize for internal unit usage



} } // anonymous namespace Internal

Device::Device (string const&, Type)
{

}

bool Device::isExtensionSupported (string const&) noexcept
{
	return false;
}

bool Device::isExtensionPresent (string const&) noexcept
{
	return false;
}

// ====================================================

//CaptureDevice::CaptureDevice (string const&,
//							  uint,
//							  OutputFormat,
//							  SoundQuality) noexcept
//{

//}

//CaptureDevice::CaptureDevice () noexcept
//{

//}

CaptureDevice::~CaptureDevice () noexcept
{

}

bool CaptureDevice::busy () noexcept
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

PlaybackDevice::~PlaybackDevice () noexcept
{

}

// ====================================================

Context::Context (PlaybackDevice&, bool) noexcept
{

}

Context::~Context () noexcept
{

}

void Context::use ()
{

}

void Context::invalidate () noexcept
{

}

bool Context::setCapability (int, bool) noexcept
{
	return false;
}

bool Context::hasCapability (int) noexcept
{
	return false;
}

void Context::process () noexcept
{

}

void Context::suspend () noexcept
{

}

} } // namespace Audio
