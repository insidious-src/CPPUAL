/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#ifndef CPPUAL_COMPUTE_CL_H_
#define CPPUAL_COMPUTE_CL_H_
#ifdef __cplusplus

#include <CL/cl.h>
#include <cppual/types.h>

namespace cppual { namespace Compute { namespace CL {

enum
{
	AllDevices    = CL_DEVICE_TYPE_ALL,
	MaxUnits      = CL_DEVICE_MAX_COMPUTE_UNITS,
	MaxDimensions = CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
	MaxGroupSize  = CL_DEVICE_MAX_WORK_GROUP_SIZE,
	MaxItemCount  = CL_DEVICE_MAX_WORK_ITEM_SIZES,
	GlobalMemSize = CL_DEVICE_GLOBAL_MEM_SIZE,
	CacheSize     = CL_DEVICE_GLOBAL_MEM_CACHE_SIZE,
	CacheLineSize = CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE,
	ConstMemSize  = CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE,
	LocalMemSize  = CL_DEVICE_LOCAL_MEM_SIZE
};

typedef cl_uint        size_type;
typedef cl_int         int_type;
typedef cl_device_id   device;
typedef cl_platform_id platform;
typedef u8             type_size;

device handle (type_size type, u16 platform, size_type id);

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_CL_H_
