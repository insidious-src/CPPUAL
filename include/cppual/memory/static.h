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

#ifndef CPPUAL_MEMORY_STATIC_ALLOCATOR_H_
#define CPPUAL_MEMORY_STATIC_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/memory/mop.h>
#include <cppual/memory/allocator.h>

#include <cstring>

namespace cppual { namespace memory {

template <memory_resource::size_type N>
class stacked_static_resource final : public memory_resource
{
public:
    constexpr static auto bytes = N + (max_align * 2);

    static_assert (N > 0, "N is Zero bytes!");

    size_type capacity () const noexcept
    { return sizeof (_M_pBuffer); }

    size_type max_size () const noexcept
    {
        return static_cast<size_type> ((_M_pBuffer + sizeof (_M_pBuffer)) -
                                       static_cast<math_pointer> (_M_pMarker));
    }

    void      clear () noexcept { _M_pMarker = _M_pBuffer; }
    constexpr stacked_static_resource () noexcept : _M_pMarker  (_M_pBuffer) { }

private:
    pointer _M_pMarker       ;
    byte    _M_pBuffer[bytes];

    void* do_allocate   (size_type bytes, align_type align_size);
    void* do_reallocate (void* p, size_type old_size, size_type new_size, align_type align_size);
    void  do_deallocate (void* p, size_type bytes, align_type align_size);

    bool  do_is_equal   (base_type const& gObj) const noexcept
    { return &gObj == this; }
};

template <memory_resource::size_type N>
void* stacked_static_resource<N>::do_allocate (size_type size, align_type align_size)
{
    if (!size) throw std::bad_alloc ();
    if (!align_size || align_size > max_align) align_size = alignof(uptr);

    auto max_bytes = max_size();
    auto pMarker   = _M_pMarker;

    if (!std::align (align_size, size, pMarker, max_bytes)) throw std::bad_alloc ();

    auto const pNewMarker = static_cast<math_pointer> (pMarker) + size;

    if (pNewMarker > (_M_pBuffer + sizeof (_M_pBuffer))) throw std::bad_alloc ();

    auto const align_shift = static_cast<byte>
            (static_cast<math_pointer>(pMarker) - static_cast<math_pointer>(_M_pMarker));

    if (align_shift > 0) *static_cast<math_pointer>(_M_pMarker) = align_shift;

    _M_pMarker = pNewMarker;
    return       pMarker   ;
}

template <memory_resource::size_type N>
void* stacked_static_resource<N>::do_reallocate (void*      p       ,
                                                 size_type  old_size,
                                                 size_type  size    ,
                                                 align_type align_size)
{
    if (p)
    {
        if (old_size && old_size == size) return p;
    }
    else
    {
        p = _M_pMarker;
        old_size = 0;
    }

    if (!align_size || align_size > max_align) align_size = alignof(uptr);

    auto new_p   = p;
    auto pMarker = static_cast<math_pointer>(new_p) + old_size;

    if (pMarker == _M_pMarker || (pMarker + *pMarker) == _M_pMarker)
    {
        auto max = max_size () + old_size;

        if (std::align (align_size, size, new_p, max)) throw std::bad_alloc ();

        auto const pNewMarker = static_cast<math_pointer> (new_p) + size;

        if (pNewMarker > (_M_pBuffer + sizeof (_M_pBuffer))) throw std::bad_alloc ();

        auto const align_shift = static_cast<byte>
                    (static_cast<math_pointer>(new_p) - static_cast<math_pointer>(p));

        if (old_size) std::memmove (new_p, p, old_size <= size ? old_size : size);

        if (align_shift > 0) *static_cast<math_pointer>(p) = align_shift;

        _M_pMarker = pNewMarker;
    }
    else
    {
        throw std::out_of_range ("pointer doesn't match the last element allocated!");
    }

    return new_p;
}

template <memory_resource::size_type N>
void stacked_static_resource<N>::do_deallocate (void* p, size_type bytes, align_type)
{
    auto pMarker = static_cast<math_pointer> (p) + bytes;

    if (pMarker == _M_pMarker || (pMarker + *pMarker) == _M_pMarker)
    {
        _M_pMarker = p;
    }
    else
    {
        throw std::out_of_range ("pointer doesn't match the last element allocated!");
    }
}

//! static allocator using StaticResource (allocates memory on cpu stack)
template <class T, memory_resource::size_type N>
using StaticAllocator = allocator <T, stacked_static_resource<N * sizeof (T)> >;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_STATIC_ALLOCATOR_H_
