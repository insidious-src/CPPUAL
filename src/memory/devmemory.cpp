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

#include <vulkan/vulkan.h>
#include <cppual/memory/devmemory.h>
#include <cppual/compute/device.h>

void* operator new (std::size_t /* size */, MemoryChunk& obj, std::size_t /* align */)
{
    using namespace cppual::Memory;

    if (!obj.device ().valid ()) throw memory_source_not_available ();

    //::VkDeviceMemory mem_obj = nullptr;

    // ::VkMemoryAllocateInfo nfo =
    // {
    //     size,
    //     align ? align : size,
    //     0,
    //     1,
    //     { 0 },
    //     0
    // };

    // if (::vkAllocateMemory (obj.device ().handle<::VkDevice> (), &nfo, mem_obj))
    //     throw std::bad_alloc ();

    //return mem_obj;
    return nullptr;
}

void operator delete (void* ptr, MemoryChunk& obj)
{
    using namespace cppual::Memory;
    const ::VkAllocationCallbacks* p = nullptr;

    if (!obj.device ().valid ()) throw memory_source_not_available ();
    //::vkFreeMemory (obj.device ().handle<::VkDevice> (), static_cast<::VkDeviceMemory> (ptr), p);
}

// =========================================================

namespace cppual { namespace Memory {

} } // namespace Compute
