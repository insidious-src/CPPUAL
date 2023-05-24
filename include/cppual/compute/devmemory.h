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

#ifndef CPPUAL_COMPUTE_MEMORY_H_
#define CPPUAL_COMPUTE_MEMORY_H_
#ifdef __cplusplus

#include <cppual/compute/object.h>
#include <cppual/noncopyable.h>

#include <new>

namespace cppual { namespace compute {

// =========================================================

struct memory_source_not_available : std::bad_alloc { };

// =========================================================

class memory_chunk : public object<resource_type::buffer>
{
public:
    typedef device        device_type           ;
    typedef device&       device_reference      ;
    typedef device*       device_pointer        ;
    typedef device const* device_const_pointer  ;
    typedef device const& device_const_reference;

    memory_chunk (device_const_reference, size_type size);

    device_const_reference get_device () const noexcept
    { return *_M_pDevice; }

private:
    device_const_pointer _M_pDevice;
};

} } // namespace Compute

// =========================================================

using cppual::compute::memory_chunk;

// =========================================================

void* operator new    (std::size_t size, memory_chunk& source, std::size_t align = 0);
void  operator delete (void* ptr, memory_chunk& source);

inline void* operator new[] (std::size_t size, memory_chunk& source, std::size_t align = 0)
{ return ::operator new (size, source, align); }

inline void  operator delete[] (void* ptr, memory_chunk& source)
{ ::operator delete (ptr, source); }

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_MEMORY_H_
