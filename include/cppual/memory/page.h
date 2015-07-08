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

#ifndef CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
#define CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

using std::string;

namespace cppual { namespace Memory {

class PageAllocator final : public Allocator, NonCopyable
{
public:
    PageAllocator (size_type size);
    PageAllocator (Allocator* allocator, size_type size);
    //PageAllocator (string&& shared_name, size_type size);

    void* allocate (size_type size, align_type align) noexcept;
    void  deallocate (void* p, size_type size) noexcept;
    void  clear () noexcept;

    inline size_type count () const noexcept
    { return 0; }

    inline size_type size () const noexcept
    { return 0; }

    inline size_type max_size () const noexcept
    { return 0; }
};

// =========================================================

template <typename T>
using PagePolicy = AllocatorPolicy<T, Allocator>;

} } // Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
