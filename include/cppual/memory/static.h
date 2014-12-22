/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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
#include <cppual/memory/memory.h>
#include <cppual/memory/allocator.h>

namespace cppual { namespace Memory {

template <Allocator::size_type N>
class StaticAllocator final : public Allocator, NonCopyable
{
public:
	void* allocate (size_type n, align_type = 0) override;
	void  deallocate (void* p, size_type n) override;

	size_type size () const noexcept
	{ return sizeof (m_pBuffer); }

	bool is_equal (Allocator const& gObj) const noexcept
	{ return &gObj == this; }

	size_type max_size () const noexcept
	{
		return static_cast<size_type> ((m_pBuffer + sizeof (m_pBuffer)) -
									   static_cast<math_pointer> (m_pMarker));
	}

	size_type count () const noexcept
	{ return m_uNumAlloc; }

	void clear () noexcept
	{
		m_pMarker   = m_pBuffer;
		m_uNumAlloc = 0;
	}

	constexpr StaticAllocator () noexcept
	: m_pMarker   (m_pBuffer),
	  m_uNumAlloc ()
	{ }

private:
	pointer           m_pMarker;
	size_type         m_uNumAlloc;
	alignas (uptr) u8 m_pBuffer[N];
};

template <Allocator::size_type N>
inline void* StaticAllocator<N>::allocate (size_type n, align_type a)
{
	pointer const pMarker    = nextAlignedAddr (m_pMarker, a);
	pointer const pNewMarker = static_cast<math_pointer> (pMarker) + n;

	if (pNewMarker > m_pBuffer + sizeof (m_pBuffer)) throw std::bad_alloc ();

	m_pMarker = pNewMarker;
	++m_uNumAlloc;
	return pMarker;
}

template <Allocator::size_type N>
inline void StaticAllocator<N>::deallocate (void* p, size_type n)
{
	if (static_cast<math_pointer> (p) + n == m_pMarker)
	{
		m_pMarker = p;
		--m_uNumAlloc;
	}
	else throw std::out_of_range ("pointer doesn't match the last element allocated");
}

template <class T, Allocator::size_type N>
using StaticPolicy = AllocatorPolicy <T, StaticAllocator<N> >;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_STATIC_ALLOCATOR_H_
