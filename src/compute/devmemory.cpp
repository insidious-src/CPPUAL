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

using namespace cppual::compute;

void* operator new (std::size_t /* size */, memory_chunk& /*obj*/, std::size_t /* align */)
{
    //if (!obj.device ().valid ()) throw memory_source_not_available ();

    return reinterpret_cast<void*>(1);
}

void operator delete (void* /*ptr*/, memory_chunk& /*obj*/)
{

}

// =========================================================

namespace cppual { namespace compute {

} } // namespace Compute
