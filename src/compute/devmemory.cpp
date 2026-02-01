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

#include <cppual/compute/devmemory.h>

#include <cppual/compute/device.h>

// =========================================================

void* operator new (std::size_t size, cppual::memory::memory_resource& res)
{
    if (res.device ().valid () == false       ) throw cppual::compute::bad_device    ();
    if (res.device ().max_alloc_size () < size) throw cppual::compute::out_of_memory ();

    return res.allocate (size, alignof (std::max_align_t));
}

void* operator new [] (std::size_t size, cppual::memory::memory_resource& res)
{
    return ::operator new (size, res);
}

// =========================================================