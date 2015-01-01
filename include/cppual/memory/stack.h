/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

using std::string;

namespace cppual { namespace Memory {

class StackedAllocator final : Allocator
{
public:
	StackedAllocator (size_type size);
	StackedAllocator (Allocator& allocator, size_type size);
	//StackedAllocator (string& shared_name, size_type size);
	~StackedAllocator ();
	void* allocate (size_type size, align_type align) noexcept override;
	void  deallocate (void* p, size_type size) override;
	void  clear () noexcept;

	cvoid* marker () const noexcept
	{ return m_pCurMarker; }

	bool is_equal (Allocator const& gObj) const noexcept
	{ return &gObj == &m_gOwner; }

	size_type max_size () const noexcept
	{
		return static_cast<size_type> (static_cast<math_pointer> (m_pEnd) -
									   static_cast<math_pointer> (m_pCurMarker));
	}

	size_type count () const noexcept { return m_uNumAlloc; }

	size_type size () const noexcept
	{
		return static_cast<size_type> (static_cast<math_pointer> (m_pEnd) -
									   static_cast<math_pointer> (m_pBegin));
	}

	Allocator& owner     () const noexcept { return m_gOwner; }
	bool        is_shared () const noexcept { return m_bIsMemShared; }

private:
	size_type     m_uNumAlloc;
	Allocator&   m_gOwner;
	pointer       m_pCurMarker;
	pointer const m_pBegin;
	pointer const m_pEnd;
	cbool         m_bIsMemShared;
};

template <class T>
using StackedPolicy = AllocatorPolicy <T, StackedAllocator>;

// =========================================================

class DStackedAllocator final : public Allocator, NonCopyable
{
public:
	DStackedAllocator (size_type size, size_type marker_hint);
	DStackedAllocator (Allocator& allocator, size_type size, size_type marker_hint);
	//DStackedAllocator (string& shared, size_type size, size_type marker_hint);
	~DStackedAllocator ();

	void* allocate (size_type size, align_type align) noexcept override;
	void  deallocate (void* p, size_type size) override;
	void  clear () noexcept;

	size_type hint () const noexcept { return m_uHint; }
	void      setHint (size_type uHint) noexcept { m_uHint = uHint; }
	cvoid*    bottomMarker () const noexcept { return m_pBottomMarker; }
	cvoid*    topMarker () const noexcept { return m_pTopMarker; }

	bool is_equal (Allocator const& gObj) const noexcept
	{ return &gObj == &m_gOwner; }

	size_type max_size () const noexcept
	{
		return static_cast<size_type> (static_cast<math_pointer> (m_pBottomMarker) -
									   static_cast<math_pointer> (m_pTopMarker));
	}

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
	size_type     m_uNumAlloc;
	Allocator&   m_gOwner;
	pointer       m_pTopMarker;
	pointer       m_pBottomMarker;
	pointer const m_pBegin;
	size_type     m_uHint;
	pointer const m_pEnd;
	cbool         m_bIsMemShared;
};

template <class T>
using DStackedPolicy = AllocatorPolicy <T, DStackedAllocator>;

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_ALLOCATOR_H_
