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

#ifndef CPPUAL_MEMORY_HEAP_ALLOCATOR_H_
#define CPPUAL_MEMORY_HEAP_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/string.h>
#include <cppual/memory/allocator.h>

namespace cppual { namespace memory {

class heap_resource final : public memory_resource
{
public:
    heap_resource   (size_type size);
    heap_resource   (memory_resource& allocator, size_type size);
    //heap_resource   (string& shared_name, size_type size);
    ~heap_resource  ();
    void clear () noexcept;

    size_type max_size () const noexcept
    { return _M_pFreeBlocks != nullptr ? _M_pFreeBlocks->size : 0; }

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (_M_pEnd  ) -
                                       static_cast<math_pointer> (_M_pBegin));
    }

    bool is_shared () const noexcept
    { return _M_bIsMemShared; }

    memory_resource& owner () const noexcept
    { return _M_gOwner; }

private:
    struct header     { size_type size, adjust;           };
    struct free_block { size_type size; free_block* next; };

    memory_resource& _M_gOwner      ;
    pointer const    _M_pBegin      ;
    pointer const    _M_pEnd        ;
    free_block*      _M_pFreeBlocks ;
    cbool            _M_bIsMemShared;

    void* do_allocate   (size_type size, align_type align)  noexcept;
    void  do_deallocate (void* p, size_type size, align_type align) ;

    bool do_is_equal (base_type const& gObj) const noexcept
    { return &gObj == &_M_gOwner; }
};

// =========================================================

class list_resource final : public memory_resource
{
public:
    struct header
    {
        size_type size;
        header*   next;
    };

    list_resource (size_type size);
    list_resource (memory_resource& allocator, size_type size);
    //list_resource (string& shared_name, size_type size);
    ~list_resource ();

    size_type max_size   () const noexcept;
    void      defragment () noexcept;
    void      clear      () noexcept;

    size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (_M_pEnd  ) -
                                      (static_cast<math_pointer> (_M_pBegin) +
                                       sizeof (header)));
    }

    bool is_shared () const noexcept
    { return _M_bIsMemShared; }

    memory_resource& owner () const noexcept
    { return _M_gOwner; }

private:
    memory_resource& _M_gOwner         ;
    pointer const    _M_pBegin         ;
    pointer const    _M_pEnd           ;
    header*          _M_pFirstFreeBlock;
    cbool            _M_bIsMemShared   ;

    void* do_allocate   (size_type size, align_type align)  noexcept;
    void  do_deallocate (void* p, size_type size, size_type align)  ;

    bool  do_is_equal   (base_type const& gObj) const noexcept
    { return &gObj == this; }
};

// =========================================================

template <class T>
using heap_allocator = allocator <T, heap_resource>;

template <class T>
using list_allocator = allocator <T, list_resource>;

// =========================================================

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_HEAP_ALLOCATOR_H_
