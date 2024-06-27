/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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

#include <iostream>
#include <new>

void* operator new (std::size_t size, device_memory& obj)
{
    using namespace cppual;

    if (!obj.device ().valid ()) throw compute::bad_device ();
    if ( obj.device ().max_alloc_size () < size) throw std::bad_alloc ();

    return reinterpret_cast<void*> (1);
}

void operator delete (void* /*ptr*/, device_memory& obj)
{
    if (!obj.device ().valid ())
    {
        std::cerr << "compute device NOT valid!";
        return;
    }
}

// =========================================================

namespace cppual { namespace compute {

// void fn ()
// {
//     device_memory mem (device::host (), 512);

//     auto value = new (mem) int (5);
//     ::operator delete (value, sizeof (int), mem);
// }

} } // namespace Compute
