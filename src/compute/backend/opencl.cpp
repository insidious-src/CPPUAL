/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_COMPUTE_BACKEND_OPENCL_H_
#define CPPUAL_COMPUTE_BACKEND_OPENCL_H_
#ifdef __cplusplus

#include <vector>
#include <cstring>
#include <CL/cl.h>
#include <type_traits>
#include <cppual/types.h>
#include <cppual/memory/stack.h>
#include <cppual/compute/device.h>
#include <cppual/compute/platform.h>
#include "../backend.h"

using std::vector;
using cppual::Memory::StackedAllocator;
using cppual::Memory::StackedPolicy;

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

struct DeviceType final : NonConstructible
{
    enum _Type
    {
        Default     = CL_DEVICE_TYPE_DEFAULT,
        CPU         = CL_DEVICE_TYPE_CPU,
        GPU         = CL_DEVICE_TYPE_GPU,
        Accelerator = CL_DEVICE_TYPE_ACCELERATOR,
        Custom      = CL_DEVICE_TYPE_CUSTOM,
        Any         = CL_DEVICE_TYPE_ALL
    };

    constexpr static Device::Type convert (_Type type) noexcept
    {
        return type == CPU         ? Device::CPU         :
               type == GPU         ? Device::GPU         :
               type == Accelerator ? Device::Accelerator :
               type == Custom      ? Device::Custom      :
                                     Device::Any;
    }

    constexpr static _Type convert (Device::Type type) noexcept
    {
        return type == Device::CPU         ? CPU         :
               type == Device::GPU         ? GPU         :
               type == Device::Accelerator ? Accelerator :
               type == Device::Custom      ? Custom      :
                                             Any;
    }
};

struct PlatformInfo final : NonConstructible
{
    enum _Info
    {
        Name    = CL_PLATFORM_NAME,
        Vendor  = CL_PLATFORM_VENDOR,
        Profile = CL_PLATFORM_PROFILE,
        Version = CL_PLATFORM_VERSION
    };

    constexpr static Platform::Info convert (_Info info) noexcept
    {
        return info == Name    ? Platform::Info::Name    :
               info == Vendor  ? Platform::Info::Vendor  :
               info == Profile ? Platform::Info::Profile :
                                 Platform::Info::Version;
    }

    constexpr static _Info convert (Platform::Info info) noexcept
    {
        return info == Platform::Info::Name    ? Name    :
               info == Platform::Info::Vendor  ? Vendor  :
               info == Platform::Info::Profile ? Profile :
                                                 Version;
    }
};

} // anonymous namespace

// =========================================================

class CL
{
public:
    enum
    {
        MaxUnits      = CL_DEVICE_MAX_COMPUTE_UNITS,
        MaxDimensions = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
        MaxGroupSize  = CL_DEVICE_MAX_WORK_GROUP_SIZE,
        MaxItemCount  = CL_DEVICE_MAX_WORK_ITEM_SIZES,
        GlobalMemSize = CL_DEVICE_GLOBAL_MEM_SIZE,
        CacheSize     = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
        CacheLineSize = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
        ConstMemSize  = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
        LocalMemSize  = CL_DEVICE_LOCAL_MEM_SIZE,

        PlatformExtensions = CL_PLATFORM_EXTENSIONS,
        DeviceExtensions   = CL_DEVICE_EXTENSIONS
    };

    typedef cl_uint                                       size_type;
    typedef cl_int                                        int_type;
    typedef cl_device_id                                  device_type;
    typedef cl_platform_id                                platform_type;
    typedef cl_event                                      event_type;
    typedef cl_kernel                                     kernel_type;
    typedef cl_program                                    program_type;
    typedef cl_context                                    context_type;
    typedef cl_mem                                        memory_type;
    typedef cl_command_queue                              queue_type;
    typedef std::underlying_type<DeviceType::_Type>::type type_size;

    static device_type handle (type_size type, u16 platform_type, size_type id);
    static size_type   count  (u16 uId);
};

// =========================================================

namespace { namespace Internal { // optimize for internal unit usage

struct Initializer
{
    typedef StackedAllocator                               allocator_type;
    typedef StackedPolicy<CL::device_type>                 cl_device_policy;
    typedef std::vector<CL::device_type, cl_device_policy> cl_device_vector;

    struct PlatformInfo
    {
        CL::platform_type handle;
        cl_device_vector  devices;
        CL::size_type     cpu_count;
        CL::size_type     gpu_count;
        CL::size_type     accel_count;
        CL::size_type     custom_count;

        PlatformInfo (allocator_type& gAtor)
        : devices (cl_device_policy (gAtor))
        { }
    };

    typedef StackedPolicy<PlatformInfo>                   cl_platform_policy;
    typedef std::vector<PlatformInfo, cl_platform_policy> cl_platform_vector;

    allocator_type     allocator;
    cl_platform_vector platforms;

    Initializer ();

};

inline Initializer& get () noexcept
{
    static Initializer init;
    return init;
}

inline CL::size_type init_num_platforms () noexcept
{
    CL::size_type n;
    ::clGetPlatformIDs (0, nullptr, &n);
    return n;
}

inline CL::size_type get_num_platforms () noexcept
{
    static CL::size_type n = init_num_platforms ();
    return n;
}

inline std::size_t size_of_platforms_data () noexcept
{
    CL::size_type n    = get_num_platforms ();
    std::size_t   size = n * sizeof (Initializer::PlatformInfo);

    CL::platform_type handles[n];
    ::clGetPlatformIDs (n, handles, nullptr);

    for (CL::size_type i = 0, x; i < n; ++i)
    {
        ::clGetDeviceIDs (handles[i], DeviceType::Any, 0, nullptr, &x);
        size += sizeof (CL::device_type) * x;
    }

    return size;
}

inline string infostr (u16 id, CL::size_type info) noexcept
{
    static thread_local std::size_t n = 0;
    static thread_local string      text;

    ::clGetPlatformInfo (get ().platforms[id].handle, info, 0, nullptr, &n);
    text.resize (n);
    ::clGetPlatformInfo (get ().platforms[id].handle, info, n, &text[0], nullptr);
    text.resize (--n);

    return std::move (text);
}

Initializer::Initializer ()
: allocator (size_of_platforms_data ()),
  platforms (get_num_platforms (), PlatformInfo (allocator), cl_platform_policy (allocator))
{
    CL::platform_type handles[platforms.size ()];

    ::clGetPlatformIDs (static_cast<CL::size_type> (platforms.size ()),
                        handles, nullptr);

    // generate information for each platform
    for (CL::size_type i = 0, pos = 0; i < platforms.size (); ++i, pos = 0)
    {
        platforms[i].handle = handles[i];

        // get device count
        ::clGetDeviceIDs (handles[i], DeviceType::CPU, 0, nullptr,
                          &platforms[i].cpu_count);
        ::clGetDeviceIDs (handles[i], DeviceType::GPU, 0, nullptr,
                          &platforms[i].gpu_count);
        ::clGetDeviceIDs (handles[i], DeviceType::Accelerator, 0, nullptr,
                          &platforms[i].accel_count);
        ::clGetDeviceIDs (handles[i], DeviceType::Custom, 0, nullptr,
                          &platforms[i].custom_count);

        platforms[i].devices.resize (platforms[i].cpu_count   +
                                     platforms[i].gpu_count   +
                                     platforms[i].accel_count +
                                     platforms[i].custom_count);

        // place device handles
        if (platforms[i].cpu_count)
            ::clGetDeviceIDs (handles[i], DeviceType::CPU, platforms[i].cpu_count,
                              &platforms[i].devices[pos], nullptr);

        if (platforms[i].gpu_count)
            ::clGetDeviceIDs (handles[i], DeviceType::GPU, platforms[i].gpu_count,
                              &platforms[i].devices[pos += platforms[i].cpu_count], nullptr);

        if (platforms[i].accel_count)
            ::clGetDeviceIDs (handles[i], DeviceType::Accelerator, platforms[i].accel_count,
                              &platforms[i].devices[pos += platforms[i].gpu_count], nullptr);

        if (platforms[i].custom_count)
            ::clGetDeviceIDs (handles[i], DeviceType::Custom, platforms[i].custom_count,
                              &platforms[i].devices[pos += platforms[i].accel_count], nullptr);
    }
}

} } // Internal anonymous namespace

// =========================================================

CL::device_type CL::handle (type_size eType, u16 uPfId, size_type uDevId)
{
    switch (eType)
    {
    case DeviceType::Any: case DeviceType::CPU:
        return Internal::get ().platforms[uPfId].devices[uDevId];
    case DeviceType::GPU:
        return Internal::get ().platforms[uPfId].devices[Internal::get ().platforms[uPfId].cpu_count +
               uDevId];
    case DeviceType::Accelerator:
        return Internal::get ().platforms[uPfId].devices[Internal::get ().platforms[uPfId].cpu_count +
               Internal::get ().platforms[uPfId].gpu_count +
               uDevId];
    case DeviceType::Custom:
        return Internal::get ().platforms[uPfId].devices[Internal::get ().platforms[uPfId].cpu_count +
               Internal::get ().platforms[uPfId].gpu_count   +
               Internal::get ().platforms[uPfId].accel_count +
               uDevId];
    default:
        return nullptr;
    }
}

// =========================================================

CL::size_type CL::count (u16 uId)
{
    if (Internal::get ().platforms.size () <= uId) throw bad_platform ();
    return static_cast<size_type> (Internal::get ().platforms[uId].devices.size ());
}

// =========================================================

u16 Platform::count () noexcept
{
    return static_cast<u16> (Internal::get ().platforms.size ());
}

bool Platform::available (cchar* pFeature, u16 uId)
{
    if (Internal::get ().platforms.size () <= uId) throw bad_platform ();

    static std::size_t uSize;

    if (::clGetPlatformInfo (Internal::get ().platforms[uId].handle, CL::PlatformExtensions,
                             0, nullptr, &uSize) != CL_SUCCESS or !uSize)
        return false;

    char text[uSize]; // VLA

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

    static std::size_t n;

    if (::clGetPlatformInfo (Internal::get ().platforms[uId].handle, CL_PLATFORM_VERSION,
                             0, nullptr, &n) != CL_SUCCESS)
        return .0;

    char text[n]; // VLA

    if (::clGetPlatformInfo (Internal::get ().platforms[uId].handle, CL_PLATFORM_VERSION,
                             n, &text[0], nullptr) != CL_SUCCESS)
        return .0;

    return static_cast<float> (std::atof (&text[0]));
}

string Platform::info (Info eType, u16 uId)
{
    if (Internal::get ().platforms.size () <= uId) throw bad_platform ();
    return std::move (Internal::infostr (uId, PlatformInfo::convert (eType)));
}

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_OPENCL_H_
