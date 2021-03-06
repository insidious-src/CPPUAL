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

#ifndef CPPUAL_MEMORY_HEAP_ALLOCATOR_H_
#define CPPUAL_MEMORY_HEAP_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/string.h>
#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

namespace cppual { namespace Memory {

class HeapResource final : public MemoryResource, NonCopyable
{
public:
    HeapResource   (size_type size);
    HeapResource   (MemoryResource& allocator, size_type size);
    //HeapPool   (string& shared_name, size_type size);
    ~HeapResource  ();
    void clear () noexcept;

    size_type max_size () const noexcept
    { return m_pFreeBlocks != nullptr ? m_pFreeBlocks->size : 0; }

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd  ) -
                                       static_cast<math_pointer> (m_pBegin));
    }

    bool is_shared () const noexcept
    { return m_bIsMemShared; }

    MemoryResource& owner () const noexcept
    { return m_gOwner; }

private:
    struct Header    { size_type size, adjust;          };
    struct FreeBlock { size_type size; FreeBlock* next; };

    MemoryResource& m_gOwner;
    pointer const   m_pBegin;
    pointer const   m_pEnd;
    FreeBlock*      m_pFreeBlocks;
    cbool           m_bIsMemShared;

    void* do_allocate   (size_type size, align_type align)  noexcept;
    void  do_deallocate (void* p, size_type size, align_type align) ;

    bool do_is_equal (MemoryResource const& gObj) const noexcept
    { return &gObj == &m_gOwner; }
};

// =========================================================

class ListResource final : public MemoryResource, NonCopyable
{
public:
    struct Header
    {
        size_type size;
        Header*   next;
    };

    ListResource (size_type size);
    ListResource (MemoryResource& allocator, size_type size);
    //ListPool (string& shared_name, size_type size);
    ~ListResource ();

    size_type max_size   () const noexcept;
    void      defragment () noexcept;
    void      clear      () noexcept;

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd  ) -
                                      (static_cast<math_pointer> (m_pBegin) +
                                       sizeof (Header)));
    }

    bool is_shared () const noexcept
    { return m_bIsMemShared;  }

    MemoryResource& owner () const noexcept
    { return m_gOwner; }

private:
    pointer const   m_pBegin;
    pointer const   m_pEnd;
    Header*         m_pFirstFreeBlock;
    MemoryResource& m_gOwner;
    cbool           m_bIsMemShared;

    void* do_allocate   (size_type size, align_type align)  noexcept;
    void  do_deallocate (void* p, size_type size, size_type align)  ;

    bool  do_is_equal   (MemoryResource const& gObj) const noexcept
    { return &gObj == &m_gOwner; }
};

// =========================================================

template <class T>
using HeapAllocator = Allocator <T, HeapResource>;

template <class T>
using ListAllocator = Allocator <T, ListResource>;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_HEAP_ALLOCATOR_H_
