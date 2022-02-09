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

#ifndef CPPUAL_MEMORY_POOL_ALLOCATOR_H_
#define CPPUAL_MEMORY_POOL_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/memory/allocator.h>
#include <cppual/memory/shared.h>
#include <cppual/memory/mop.h>

namespace cppual { namespace memory {

// =========================================================

class uniform_pool_resource final : public memory_resource
{
public:
    /// local memory
    uniform_pool_resource (size_type  blk_count,
                           size_type  blk_size,
                           align_type blk_align = alignof(uptr));

    /// local memory from allocated buffer (stack or heap)
    uniform_pool_resource (pointer    buffer,
                           size_type  blk_count,
                           size_type  blk_size,
                           align_type blk_align = alignof(uptr));

    /// nested allocators
    uniform_pool_resource (memory_resource& allocator,
                           size_type        blk_count,
                           size_type        blk_size,
                           align_type       blk_align = alignof(uptr));

    /// shared memory
    uniform_pool_resource (shared_object& shared_obj,
                           size_type     blk_count,
                           size_type     blk_size,
                           align_type    blk_align = alignof(uptr));

    ~uniform_pool_resource ();
    void clear () noexcept;

    size_type block_size  () const noexcept { return _M_uBlkSize ; }
    size_type block_align () const noexcept { return _M_uBlkAlign; }
    size_type block_count () const noexcept { return _M_uBlkNum  ; }

    size_type max_size () const noexcept
    { return _M_pBegin != nullptr ? _M_uBlkSize : 0; }

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (_M_pEnd  ) -
                                      (static_cast<math_pointer> (_M_pBegin) +
                                       align_adjust (_M_pBegin, _M_uBlkAlign)));
    }

    bool is_shared () const noexcept
    { return false; }

    memory_resource& owner () const noexcept
    { return _M_gOwner; }

private:
    void  initialize    () noexcept;
    void* do_allocate   (size_type size, align_type align);
    void  do_deallocate (void* p, size_type size, align_type align) ;

    bool  do_is_equal   (base_const_reference gObj) const noexcept
    { return &gObj == this; }

private:
    memory_resource& _M_gOwner      ;
    pointer const    _M_pBegin      ;
    pointer const    _M_pEnd        ;
    pointer*         _M_pFreeList   ;
    size_type  const _M_uBlkSize    ;
    align_type const _M_uBlkAlign   ;
    size_type        _M_uBlkNum     ;
    cbool            _M_bIsMemShared;
};

// =========================================================

template <typename T>
using uniform_pool_allocator = allocator <T, uniform_pool_resource>;

// =========================================================

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_POOL_ALLOCATOR_H_
