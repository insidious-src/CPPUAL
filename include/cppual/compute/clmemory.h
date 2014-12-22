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

#ifndef CPPUAL_CL_MEMORY_H_
#define CPPUAL_CL_MEMORY_H_
#ifdef __cplusplus

#include <cppual/memory/memory.h>
#include <cppual/compute/context.h>

namespace cppual {

//template <>
//struct Disposable <Compute::Memory>
//{
//	inline static int32 reference (Disposable<Compute::Memory>* memory) noexcept
//	{ return ::clRetainMemObject (static_cast<cl_mem> (memory)); }

//	inline static void* operator new (std::size_t uSize,
//									  void*       pMemory,
//									  cl_context  pContext) noexcept
//	{
//		return ::clCreateBuffer (pContext, CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
//								 uSize, pMemory, nullptr);
//	}

//	inline static void operator delete (void* memory) noexcept
//	{ ::clReleaseMemObject (static_cast<cl_mem> (memory)); }
//};

namespace Compute {

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_CL_MEMORY_H_
