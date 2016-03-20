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

#include <vulkan/vulkan.h>
#include <cppual/memory/devmemory.h>
#include <cppual/compute/device.h>

void* operator new (std::size_t size, MemoryChunk& obj, std::size_t align)
{
    using namespace cppual::Memory;

    if (!obj.device ().valid ()) throw memory_source_not_available ();

    ::VK_GPU_MEMORY mem_obj = nullptr;

    ::VK_MEMORY_ALLOC_INFO nfo =
    {
        size,
        align ? align : size,
        0,
        1,
        { 0 },
        0
    };

    if (::vkAllocMemory (obj.device ().handle ().get<VK_DEVICE> (), &nfo, &mem_obj))
        throw std::bad_alloc ();
    return mem_obj;
}

void operator delete (void* ptr, MemoryChunk& obj)
{
    using namespace cppual::Memory;

    if (!obj.device ().valid ()) throw memory_source_not_available ();
    if (::vkFreeMemory (ptr))    throw std::out_of_range
            ("pointer is not an allocated memory block");
}

// =========================================================

namespace cppual { namespace Memory {

} } // namespace Compute
