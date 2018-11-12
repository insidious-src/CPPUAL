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

#ifndef CPPUAL_MEMORY_STATIC_ALLOCATOR_H_
#define CPPUAL_MEMORY_STATIC_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/noncopyable.h>
#include <cppual/memory/mop.h>
#include <cppual/memory/allocator.h>

namespace cppual { namespace Memory {

template <MemoryResource::size_type N>
class StaticPool final : public MemoryResource, NonCopyable
{
public:
    size_type capacity () const noexcept
    { return sizeof (m_pBuffer); }

    size_type max_size () const noexcept
    {
        return static_cast<size_type> ((m_pBuffer + sizeof (m_pBuffer)) -
                                       static_cast<math_pointer> (m_pMarker));
    }

    void      clear      () noexcept { m_pMarker = m_pBuffer;   }
    constexpr StaticPool () noexcept : m_pMarker  (m_pBuffer) { }

private:
    pointer           m_pMarker;
    alignas (uptr) u8 m_pBuffer[N];

    void* do_allocate   (size_type size, align_type align);
    void  do_deallocate (void* p, size_type size, align_type align);

    bool  do_is_equal   (memory_resource const& gObj) const noexcept
    { return &gObj == this; }
};

template <MemoryResource::size_type N>
inline void* StaticPool<N>::do_allocate (size_type n, align_type a)
{
    pointer const pMarker    = nextAlignedAddr (m_pMarker, a);
    pointer const pNewMarker = static_cast<math_pointer> (pMarker) + n;

    if (pNewMarker > m_pBuffer + sizeof (m_pBuffer)) throw std::bad_alloc ();

    m_pMarker = pNewMarker;
    return      pMarker   ;
}

template <MemoryResource::size_type N>
inline void StaticPool<N>::do_deallocate (void* p, size_type n, align_type)
{
    if (static_cast<math_pointer> (p) + n == m_pMarker)
        m_pMarker = p;
    else
        throw std::out_of_range ("pointer doesn't match the last element allocated");
}

template <class T, MemoryResource::size_type N>
using StaticAllocator = Allocator <T, StaticPool<N> >;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_STATIC_ALLOCATOR_H_
