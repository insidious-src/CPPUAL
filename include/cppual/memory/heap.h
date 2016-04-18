/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

using std::string;

namespace cppual { namespace Memory {

class HeapAllocator final : public Allocator, NonCopyable
{
public:
    HeapAllocator (size_type size);
    HeapAllocator (Allocator& allocator, size_type size);
    //HeapAllocator (string& shared_name, size_type size);
    ~HeapAllocator ();

    void* allocate (size_type size, align_type align) noexcept override;
    void  deallocate (void* p, size_type size) override;
    void  clear () noexcept;

    bool is_equal (Allocator const& gObj) const noexcept
    { return &gObj == &m_gOwner; }

    size_type max_size () const noexcept
    { return m_pFreeBlocks != nullptr ? m_pFreeBlocks->size : 0; }

    size_type count () const noexcept
    { return m_uNumAlloc; }

    size_type size () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd) -
                                       static_cast<math_pointer> (m_pBegin));
    }

    bool is_shared () const noexcept
    { return m_bIsMemShared;  }

    Allocator& owner () const noexcept
    { return m_gOwner; }

private:
    struct Header    { size_type size, adjust; };
    struct FreeBlock { size_type size; FreeBlock* next; };

    size_type     m_uNumAlloc;
    Allocator&   m_gOwner;
    pointer const m_pBegin;
    pointer const m_pEnd;
    FreeBlock*    m_pFreeBlocks;
    cbool         m_bIsMemShared;
};

// =========================================================

class ListAllocator final : public Allocator, NonCopyable
{
public:
    struct Header;

    ListAllocator (size_type size);
    ListAllocator (Allocator& allocator, size_type size);
    //ListAllocator (string& shared_name, size_type size);
    ~ListAllocator ();

    size_type max_size () const noexcept;
    void      defragment () noexcept;
    void*     allocate (size_type size, align_type align) noexcept;
    void      deallocate (void* p, size_type size);
    void      clear () noexcept;

    bool is_equal (Allocator const& gObj) const noexcept
    { return &gObj == &m_gOwner; }

    size_type count () const noexcept
    { return m_uNumAlloc; }

    size_type size () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd) -
                                       static_cast<math_pointer> (m_pBegin));
    }

    bool is_shared () const noexcept
    { return m_bIsMemShared;  }

    Allocator& owner () const noexcept
    { return m_gOwner; }

private:
    pointer const m_pBegin;
    pointer const m_pEnd;
    Header*       m_pFirstFreeBlock;
    size_type     m_uNumAlloc;
    Allocator&   m_gOwner;
    cbool         m_bIsMemShared;
};

// =========================================================

template <class T>
using HeapPolicy = AllocatorPolicy <T, HeapAllocator>;

template <class T>
using ListPolicy = AllocatorPolicy <T, ListAllocator>;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_HEAP_ALLOCATOR_H_
