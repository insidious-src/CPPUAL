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

#include <vector>
#include <cstring>
#include <cppual/compute/platform.h>
#include <cppual/memory/stack.h>
#include "backend.h"

using std::vector;
using cppual::memory::StackedPool;
using cppual::memory::StackedPolicy;

namespace cppual { namespace Compute {

namespace { namespace Internal { // optimize for internal unit usage

struct Initializer
{
    typedef StackedPool                                 allocator_type;
    typedef StackedPolicy<CL::device_type>              device_policy;
    typedef std::vector<CL::device_type, device_policy> device_vector;

    struct PlatformInfo
    {
        CL::platform_type handle;
        device_vector     devices;
        CL::size_type     cpu_count;
        CL::size_type     gpu_count;
        CL::size_type     accel_count;
        CL::size_type     custom_count;

        PlatformInfo (allocator_type& gAtor)
        : devices (device_policy (gAtor))
        { }
    };

    typedef StackedPolicy<PlatformInfo>                platform_policy;
    typedef std::vector<PlatformInfo, platform_policy> platform_vector;

    allocator_type  allocator;
    platform_vector platforms;

    Initializer ();

};

constexpr Initializer& get () noexcept
{
    static Initializer init;
    return init;
}

constexpr CL::size_type init_num_platforms () noexcept
{
    CL::size_type n;
    ::clGetPlatformIDs (0, nullptr, &n);
    return n;
}

constexpr CL::size_type get_num_platforms () noexcept
{
    static CL::size_type n = init_num_platforms ();
    return n;
}

constexpr std::size_t size_of_platforms_data () noexcept
{
    CL::size_type n    = get_num_platforms ();
    std::size_t   size = n * sizeof (Initializer::PlatformInfo);

    CL::platform_type handles[n];
    ::clGetPlatformIDs (n, handles, nullptr);

    for (CL::size_type i = 0, x; i < n; ++i)
    {
        ::clGetDeviceIDs (handles[i], CL::Any, 0, nullptr, &x);
        size += sizeof (CL::device_type) * x;
    }

    return size;
}

constexpr string infostr (u16 id, CL::size_type info) noexcept
{
    static thread_local std::size_t n = 0;
    static thread_local string      text;

    ::clGetPlatformInfo (Internal::get ().platforms[id].handle, info, 0, nullptr, &n);
    text.resize (n);
    ::clGetPlatformInfo (Internal::get ().platforms[id].handle, info, n, &text[0], nullptr);
    text.resize (--n);

    return std::move (text);
}

constexpr CL::size_type infotype (Platform::Info eType) noexcept
{
    return eType == Platform::Info::Name ?
                CL_PLATFORM_NAME : eType == Platform::Info::Vendor ?
                    CL_PLATFORM_VENDOR : eType == Platform::Info::Profile ?
                        CL_PLATFORM_PROFILE : eType == Platform::Info::Version ?
                            CL_PLATFORM_VERSION : 0;
}

Initializer::Initializer ()
: allocator (size_of_platforms_data ()),
  platforms (get_num_platforms (), PlatformInfo (allocator), platform_policy (allocator))
{
    CL::platform_type handles[platforms.size ()];

    ::clGetPlatformIDs (static_cast<CL::size_type> (platforms.size ()),
                        handles, nullptr);

    // generate information for each platform
    for (CL::size_type i = 0, pos = 0; i < platforms.size (); ++i, pos = 0)
    {
        platforms[i].handle = handles[i];

        // get device count
        ::clGetDeviceIDs (handles[i], CL::CPU, 0, nullptr, &platforms[i].cpu_count);
        ::clGetDeviceIDs (handles[i], CL::GPU, 0, nullptr, &platforms[i].gpu_count);
        ::clGetDeviceIDs (handles[i], CL::Accelerator, 0, nullptr, &platforms[i].accel_count);
        ::clGetDeviceIDs (handles[i], CL::Custom, 0, nullptr, &platforms[i].custom_count);

        platforms[i].devices.resize (platforms[i].cpu_count   +
                                     platforms[i].gpu_count   +
                                     platforms[i].accel_count +
                                     platforms[i].custom_count);

        // place device handles
        if (platforms[i].cpu_count)
            ::clGetDeviceIDs (handles[i], CL::CPU, platforms[i].cpu_count,
                              &platforms[i].devices[pos], nullptr);

        if (platforms[i].gpu_count)
            ::clGetDeviceIDs (handles[i], CL::GPU, platforms[i].gpu_count,
                              &platforms[i].devices[pos += platforms[i].cpu_count], nullptr);

        if (platforms[i].accel_count)
            ::clGetDeviceIDs (handles[i], CL::Accelerator, platforms[i].accel_count,
                              &platforms[i].devices[pos += platforms[i].gpu_count], nullptr);

        if (platforms[i].custom_count)
            ::clGetDeviceIDs (handles[i], CL::Custom, platforms[i].custom_count,
                              &platforms[i].devices[pos += platforms[i].accel_count], nullptr);
    }
}

} } // Internal anonymous namespace

// =========================================================

CL::device_type CL::handle (type_size eType, u16 uPfId, size_type uDevId)
{
    switch (eType)
    {
    case CL::Any: case CL::CPU:
        return Internal::get ().platforms[uPfId].devices[uDevId];
    case CL::GPU:
        return Internal::get ().platforms[uPfId].devices[Internal::get ().platforms[uPfId].cpu_count +
               uDevId];
    case CL::Accelerator:
        return Internal::get ().platforms[uPfId].devices[Internal::get ().platforms[uPfId].cpu_count +
               Internal::get ().platforms[uPfId].gpu_count +
               uDevId];
    case CL::Custom:
        return Internal::get ().platforms[uPfId].devices[Internal::get ().platforms[uPfId].cpu_count +
               Internal::get ().platforms[uPfId].gpu_count   +
               Internal::get ().platforms[uPfId].accel_count +
               uDevId];
    default:
        return nullptr;
    }
}

// =========================================================

CL::size_type Device::count (u16 uId)
{
    if (Internal::get ().platforms.size () <= uId) throw bad_platform ();
    return static_cast<uint_type> (Internal::get ().platforms[uId].devices.size ());
}

// =========================================================

u16 Platform::count () noexcept
{
    return static_cast<u16> (Internal::get ().platforms.size ());
}

bool Platform::available (cchar* pFeature, u16 uId)
{
    if (Internal::get ().platforms.size () <= uId) throw bad_platform ();

    static std::size_t uSize = 0;

    if (::clGetPlatformInfo (Internal::get ().platforms[uId].handle, CL::PlatformExtensions,
                             0, nullptr, &uSize) != CL_SUCCESS or !uSize)
        return false;

    char text[uSize];

    if (::clGetPlatformInfo (Internal::get ().platforms[uId].handle, CL::PlatformExtensions,
                             uSize, &text[0], nullptr) != CL_SUCCESS)
        return false;

    text[--uSize] = 0;
    text[--uSize] = 0;

    for (cchar* pEnd = ::strchr (text, ' '), *pBegin = text; ;
         pEnd = ::strchr (pBegin = ++pEnd, ' '))
    {
        if (!pEnd) return !::strcmp (pBegin, pFeature) ? true : false;
        else
        {
            text[pEnd - text] = 0;
            if (!::strcmp (pBegin, pFeature)) return true;
        }
    }

    return false;
}

float Platform::version (u16 uId)
{
    if (Internal::get ().platforms.size () <= uId) throw bad_platform ();

    static std::size_t n = 0;

    if (::clGetPlatformInfo (Internal::get ().platforms[uId].handle, CL_PLATFORM_VERSION,
                             0, nullptr, &n) != CL_SUCCESS)
        return .0;

    char text[n];

    if (::clGetPlatformInfo (Internal::get ().platforms[uId].handle, CL_PLATFORM_VERSION,
                             n, &text[0], nullptr) != CL_SUCCESS)
        return .0;

    //return static_cast<float> (std::atof ());
    return 1.2f;
}

string Platform::info (Info eType, u16 uId)
{
    if (Internal::get ().platforms.size () <= uId) throw bad_platform ();
    return std::move (Internal::infostr (uId, Internal::infotype (eType)));
}

} } // namespace Compute
