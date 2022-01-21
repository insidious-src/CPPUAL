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

#ifndef CPPUAL_MEMORY_POOL_ALLOCATOR_H_
#define CPPUAL_MEMORY_POOL_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/memory/allocator.h>
#include <cppual/memory/shared.h>
#include <cppual/memory/mop.h>

namespace cppual { namespace Memory {

class MonotonicResource final : public MemoryResource, NonCopyable
{
public:
    /// local memory
    MonotonicResource (size_type  blk_count,
                       size_type  blk_size,
                       align_type blk_align);

    /// nested allocators
    MonotonicResource (MemoryResource& allocator,
                       size_type   blk_count,
                       size_type   blk_size,
                       align_type  blk_align);

    /// shared memory
    MonotonicResource (SharedObject& shared_obj,
                       size_type     blk_count,
                       size_type     blk_size,
                       align_type    blk_align);

    ~MonotonicResource ();
    void clear     () noexcept;

    size_type blockSize  () const noexcept { return m_uBlkSize ; }
    size_type blockAlign () const noexcept { return m_uBlkAlign; }
    size_type blockCount () const noexcept { return m_uBlkNum  ; }

    bool is_equal (base_type const& gObj) const noexcept
    { return &gObj == &m_gOwner;                   }

    size_type max_size () const noexcept
    { return m_pBegin != nullptr ? m_uBlkSize : 0; }

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (m_pEnd  ) -
                                      (static_cast<math_pointer> (m_pBegin) +
                                       alignAdjust (m_pBegin, m_uBlkAlign)));
    }

    bool is_shared () const noexcept
    { return false;    }

    MemoryResource& owner () const noexcept
    { return m_gOwner; }

private:
    MemoryResource&  m_gOwner;
    pointer const    m_pBegin;
    pointer const    m_pEnd;
    pointer*         m_pFreeList;
    size_type  const m_uBlkSize;
    align_type const m_uBlkAlign;
    size_type        m_uBlkNum;

    void  initialize    ()                                  noexcept;
    void* do_allocate   (size_type size, align_type align)  noexcept;
    void  do_deallocate (void* p, size_type size, align_type align) ;

    bool  do_is_equal   (base_type const& gObj) const noexcept
    { return &gObj == &m_gOwner; }
};

template <class T>
using MonotonicAllocator = Allocator <T, MonotonicResource>;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_POOL_ALLOCATOR_H_
