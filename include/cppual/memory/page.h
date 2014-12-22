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

#ifndef CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
#define CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/memory/memory.h>

using std::string;

namespace cppual { namespace Memory {

class PageAllocator final : public IAllocator
{
public:
	PageAllocator (size_type size);
	PageAllocator (IAllocator* allocator, size_type size);
	PageAllocator (string&& shared_name, size_type size);

	void* allocate (size_type size, align_type align) noexcept;
	void  deallocate (void* p) noexcept;
	void  clear () noexcept;

	inline bool isValid () const noexcept
	{ return false; }

	inline size_type count () const noexcept
	{ return 0; }

	inline size_type size () const noexcept
	{ return 0; }

	inline MemoryType memoryType () const noexcept
	{ return MemoryType::Local;  }

	inline IAllocator* owner () const noexcept
	{ return this; }

	inline AllocatorType type () const noexcept
	{ return AllocatorType::Randomized; }

	inline size_type maxSize () const noexcept
	{ return 0; }
};

// =========================================================

template <class T>
class PagePolicy
{
public:
	typedef T				value_type;
	typedef T*				pointer;
	typedef T const*		const_pointer;
	typedef T&				reference;
	typedef T const&		const_reference;
	typedef std::size_t		size_type;
	typedef std::ptrdiff_t	difference_type;
	typedef std::false_type propagate_on_container_move_assignment;

	template <class U>
	struct rebind { typedef PagePolicy<U> other; };

	PagePolicy (PagePolicy const&) noexcept = default;
	PagePolicy& operator = (PagePolicy const&) = delete;

	inline pointer allocate (size_type n)
	{
		return static_cast<pointer>
				(m_gAtor.allocate (sizeof (T) * n, alignof (T) * n));
	}

	inline void deallocate (pointer p)
	{ m_gAtor.deallocate (p); }

	inline size_type max_size () const noexcept
	{ return m_gAtor.maxSize () / sizeof (T); }

	inline PageAllocator* get () const noexcept
	{ return &m_gAtor; }

	inline T* allocNew ()
	{ return new (allocate (1)) T; }

	inline T* allocNew (T const& gObj)
	{ return new (allocate (1)) T (gObj); }

	template <typename... TArgs>
	inline T* allocNew (TArgs... args)
	{ return new (allocate (1)) T (std::forward<TArgs> (args)...); }

	void freeDelete (T* pObj)
	{
		if (!pObj) return;
		pObj->~T ();
		m_gAtor.deallocate (pObj);
	}

	inline PagePolicy (PageAllocator& gObj) noexcept
	: m_gAtor (gObj)
	{ }

	template <class U>
	inline
	explicit
	PagePolicy (PagePolicy<U> const& gObj) noexcept
	: m_gAtor (gObj.m_gAtor)
	{ }

private:
	PageAllocator& m_gAtor;

	template <class U>
	friend class PagePolicy;

	template <class T1, class T2>
	friend
	bool operator == (PagePolicy<T1> const&, PagePolicy<T2> const&)
	noexcept;
};

// determines if memory from another
// allocator can be deallocated from this one
template <class T1, class T2>
inline bool operator == (PagePolicy<T1> const& lhAlloc,
						 PagePolicy<T2> const& rhAlloc) noexcept
{ return &lhAlloc.m_gAtor == &rhAlloc.m_gAtor; }

template <class T1, class T2>
inline bool operator != (PagePolicy<T1> const& lhAlloc,
						 PagePolicy<T2> const& rhAlloc) noexcept
{ return !(lhAlloc == rhAlloc); }

template <class T, class TAllocator>
inline
constexpr
bool operator == (PagePolicy<T> const&, TAllocator const&) noexcept
{ return false; }

template <class T, class TAllocator>
inline
constexpr
bool operator != (PagePolicy<T> const&, TAllocator const&) noexcept
{ return true; }

} } // Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_PAGE_ALLOCATOR_H_
