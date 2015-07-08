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

#ifndef CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#define CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#ifdef __cplusplus

#include <CL/cl.h>
#include <Vulkan/vk.h>
#include <type_traits>
#include <cppual/types.h>

namespace cppual { namespace Compute {

class CL
{
public:
    enum DeviceType
    {
        Default     = CL_DEVICE_TYPE_DEFAULT,
        Any         = CL_DEVICE_TYPE_ALL,
        CPU         = CL_DEVICE_TYPE_CPU,
        GPU         = CL_DEVICE_TYPE_GPU,
        Accelerator = CL_DEVICE_TYPE_ACCELERATOR,
        Custom      = CL_DEVICE_TYPE_CUSTOM
    };

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

    typedef cl_uint                                size_type;
    typedef cl_int                                 int_type;
    typedef cl_device_id                           device_type;
    typedef cl_platform_id                         platform_type;
    typedef cl_event                               event_type;
    typedef cl_kernel                              kernel_type;
    typedef cl_program                             program_type;
    typedef cl_context                             context_type;
    typedef cl_mem                                 memory_type;
    typedef cl_command_queue                       queue_type;
    typedef std::underlying_type<DeviceType>::type type_size;

    device_type handle (type_size type, u16 platform_type, size_type id);
};

// =========================================================

class Vulkan
{
};

// =========================================================

class DeviceRepository
{
public:
    typedef std::size_t size_type;

    enum class Type : bool
    {
        OpenCL,
        Vulkan
    };

private:

};

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_INTERFACE_H_