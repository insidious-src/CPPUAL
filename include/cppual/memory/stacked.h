/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#ifndef CPPUAL_MEMORY_STACKED_ALLOCATOR_H_
#define CPPUAL_MEMORY_STACKED_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/memory_allocator>
#include <cppual/shared_memory>

// =========================================================

namespace cppual::memory {

// =========================================================

class SHARED_API stacked_resource final : public memory_resource
{
public:
    stacked_resource  (size_type capacity);
    stacked_resource  (pointer buffer, size_type capacity);
    stacked_resource  (memory_resource& rc, size_type capacity);
    stacked_resource  (shared_memory& shared_name, size_type size);
    ~stacked_resource ();

    constexpr void   clear  ()       noexcept {        _M_pMarker = _M_pBegin; }
    constexpr cvoid* marker () const noexcept { return _M_pMarker            ; }

    constexpr size_type max_size () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (_M_pEnd) -
                                       static_cast<math_pointer> (_M_pMarker));
    }

    constexpr size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (_M_pEnd) -
                                       static_cast<math_pointer> (_M_pBegin));
    }

    constexpr base_reference       owner ()       noexcept { return _M_gOwner      ; }
    constexpr base_const_reference owner () const noexcept { return _M_gOwner      ; }
    constexpr bool             is_shared () const noexcept { return _M_bIsMemShared; }

private:
    void* do_allocate   (size_type capacity, align_type align);
    void* do_reallocate (void* p, size_type old_size, size_type size, align_type align_size);
    void  do_deallocate (void* p, size_type capacity, align_type align);

    constexpr bool do_is_equal (abs_base_type const& gObj) const noexcept
    { return &gObj == this; }

private:
    memory_resource& _M_gOwner      ;
    pointer          _M_pMarker     ;
    pointer const    _M_pBegin      ;
    pointer const    _M_pEnd        ;
    cbool            _M_bIsMemShared;
};

// =========================================================

class SHARED_API dstacked_resource final : public memory_resource
{
public:
    dstacked_resource  (size_type capacity, size_type hint);
    dstacked_resource  (pointer buffer, size_type capacity, size_type hint);
    dstacked_resource  (memory_resource& rc, size_type capacity, size_type hint);
    dstacked_resource  (shared_memory& shared, size_type size, size_type hint);
    ~dstacked_resource ();

    size_type hint          ()          const noexcept { return _M_uHint        ; }
    void      set_hint      (size_type uHint) noexcept { _M_uHint = uHint       ; }
    cvoid*    bottom_marker ()          const noexcept { return _M_pBottomMarker; }
    cvoid*    top_marker    ()          const noexcept { return _M_pTopMarker   ; }

    constexpr base_reference       owner ()       noexcept { return _M_gOwner      ; }
    constexpr base_const_reference owner () const noexcept { return _M_gOwner      ; }
    constexpr bool             is_shared () const noexcept { return _M_bIsMemShared; }

    constexpr size_type max_size () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (_M_pBottomMarker) -
                                       static_cast<math_pointer> (_M_pTopMarker   ));
    }

    constexpr size_type capacity () const noexcept
    {
        return static_cast<size_type> (static_cast<math_pointer> (_M_pEnd) -
                                       static_cast<math_pointer> (_M_pBegin));
    }

    void clear () noexcept
    {
        _M_pTopMarker    = _M_pBegin;
        _M_pBottomMarker = _M_pEnd  ;
    }

private:
    pointer do_allocate   (size_type capacity, align_type align);
    pointer do_reallocate (pointer p, size_type old_size, size_type size, align_type align_size);
    void    do_deallocate (pointer p, size_type capacity, align_type align);

    constexpr bool do_is_equal (abs_base_type const& gObj) const noexcept
    { return &gObj == this; }

private:
    memory_resource& _M_gOwner       ;
    pointer          _M_pTopMarker   ;
    pointer          _M_pBottomMarker;
    pointer const    _M_pBegin       ;
    size_type        _M_uHint        ;
    pointer const    _M_pEnd         ;
    cbool            _M_bIsMemShared ;
};

// =========================================================

} // namespace Memory

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_
