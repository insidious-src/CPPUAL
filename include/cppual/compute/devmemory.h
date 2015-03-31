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

#ifndef CPPUAL_COMPUTE_MEMORY_H_
#define CPPUAL_COMPUTE_MEMORY_H_
#ifdef __cplusplus

#include <stdexcept>
#include <cppual/compute/object.h>
#include <cppual/noncopyable.h>

namespace cppual { namespace Compute {

struct memory_source_not_available : std::bad_alloc { };

// =========================================================

class GlobalMemory : public Object
{
public:
    GlobalMemory (Device const&, size_type size);

    Device const& device () const noexcept
    { return *m_pDevice; }

private:
    Device const* m_pDevice;
};

} } // namespace Compute

// =========================================================

using cppual::Compute::GlobalMemory;

void* operator new    (std::size_t size, GlobalMemory& source, std::size_t align = 0);
void  operator delete (void* ptr, GlobalMemory& source);

inline void* operator new[] (std::size_t size, GlobalMemory& source, std::size_t align = 0)
{ return ::operator new (size, source, align); }

inline void  operator delete[] (void* ptr, GlobalMemory& source)
{ ::operator delete (ptr, source); }

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_MEMORY_H_
