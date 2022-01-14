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

#ifndef CPPUAL_COMPUTE_MEMORY_H_
#define CPPUAL_COMPUTE_MEMORY_H_
#ifdef __cplusplus

#include <cppual/compute/object.h>
#include <cppual/noncopyable.h>

#include <new>

namespace cppual { namespace Compute {

struct memory_source_not_available : std::bad_alloc { };

// =========================================================

class MemoryChunk : public Object<ObjectType::Buffer>
{
public:
    typedef Device        device_type           ;
    typedef Device&       device_reference      ;
    typedef Device*       device_pointer        ;
    typedef Device const* device_const_pointer  ;
    typedef Device const& device_const_reference;

    enum class Type : unsigned char
    {
        Restricted,
        Specialized,
        Global
    };

    MemoryChunk (device_const_reference, size_type size);

    device_const_reference device () const noexcept
    { return *m_pDevice; }

private:
    device_const_pointer m_pDevice;
};

} } // namespace Compute

// =========================================================

using cppual::Compute::MemoryChunk;

void* operator new    (std::size_t size, MemoryChunk& source, std::size_t align = 0);
void  operator delete (void* ptr, MemoryChunk& source);

inline void* operator new[] (std::size_t size, MemoryChunk& source, std::size_t align = 0)
{ return ::operator new (size, source, align); }

inline void  operator delete[] (void* ptr, MemoryChunk& source)
{ ::operator delete (ptr, source); }

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_MEMORY_H_
