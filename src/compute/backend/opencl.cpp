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


#include "opencl.h"

#include <cstring>
#include <memory>

namespace cppual { namespace Compute { namespace CL {

// =========================================================

namespace { // optimize for internal unit usage

} // anonymous namespace

// =========================================================



// =========================================================

ObjectHandle<device_type>::~ObjectHandle () noexcept
{
    if (handle ()) ::clReleaseDevice (handle ());
}

ObjectHandle<context_type>::~ObjectHandle () noexcept
{
    if (handle ()) ::clReleaseContext (handle ());
}

ObjectHandle<kernel_type>::~ObjectHandle () noexcept
{
    if (handle ()) ::clReleaseKernel (handle ());
}

ObjectHandle<program_type>::~ObjectHandle () noexcept
{
    if (handle ()) ::clReleaseProgram (handle ());
}

ObjectHandle<queue_type>::~ObjectHandle () noexcept
{
    if (handle ()) ::clReleaseCommandQueue (handle ());
}

ObjectHandle<memory_type>::~ObjectHandle () noexcept
{
    if (handle ()) ::clReleaseMemObject (handle ());
}

ObjectHandle<event_type>::~ObjectHandle () noexcept
{
    if (handle ()) ::clReleaseEvent (handle ());
}

// =========================================================

string Platform::info (Info nfo)
{
    std::size_t n = 0;
    string text;

    ::clGetPlatformInfo (handle (), nfo, 0, nullptr, &n);
    text.resize (n);
    ::clGetPlatformInfo (handle (), nfo, n, &text[0], nullptr);
    text.resize (--n);

    return text;
}

// =========================================================

bool Platform::has_extension (string_type const& feature)
{
    std::size_t uSize;

    if (::clGetPlatformInfo (handle (), PlatformExtensions,
                             0, nullptr, &uSize) != Success or !uSize)
        return false;

    auto text = string_type(uSize, 0);

    if (::clGetPlatformInfo (handle (), PlatformExtensions,
                             uSize, &text[0], nullptr) != Success)
        return false;

    text[--uSize] = 0;
    text[--uSize] = 0;

    auto const pos = text.find(feature);

    return pos != string_type::npos;
}

// =========================================================

Device::size_type Device::cache () const noexcept
{
    size_type value;

    if (::clGetDeviceInfo (handle(), CacheSize, sizeof (size_type), &value, nullptr) != Success)
        return size_type();

    return value;
}

Device::size_type Device::cacheLine () const noexcept
{
    u32 value;

    if (::clGetDeviceInfo (handle(), CacheLineSize, sizeof (u32), &value, nullptr) != Success)
        return u32();

    return value;
}

Device::size_type Device::localMemory () const noexcept
{
    size_type value;

    if (::clGetDeviceInfo (handle(), LocalMemSize, sizeof (size_type), &value, nullptr) != Success)
        return size_type();

    return value;
}

Device::size_type Device::constMemory () const noexcept
{
    size_type value;

    if (::clGetDeviceInfo (handle(), ConstMemSize, sizeof (size_type), &value, nullptr) != Success)
        return size_type();

    return value;
}

Device::size_type Device::globalMemory () const noexcept
{
    size_type value;

    if (::clGetDeviceInfo (handle(), GlobalMemSize, sizeof (size_type), &value, nullptr) != Success)
        return size_type();

    return value;
}

} } } // namespace CL
