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

#ifndef CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
#define CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/string.h>
#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

namespace cppual { namespace Memory {

class PageResource final : public MemoryResource, NonCopyable
{
public:
    PageResource (size_type size);
    PageResource (MemoryResource* allocator, size_type size);
    //PageResource (string&& shared_name, size_type size);
    void clear    () noexcept;

    inline size_type count () const noexcept
    { return 0; }

    inline size_type capacity () const noexcept
    { return 0; }

    inline size_type max_size () const noexcept
    { return 0; }

private:
    void* do_allocate   (size_type size, align_type align)  noexcept;
    void  do_deallocate (void* p, size_type size, align_type align) ;

    bool  do_is_equal   (base_type const& gObj) const noexcept
    { return &gObj == this; }
};

// =========================================================

template <typename T>
using PageAllocator = Allocator<T, MemoryResource>;

} } // Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
