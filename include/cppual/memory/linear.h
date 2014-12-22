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

#ifndef CPPUAL_MEMORY_LINEAR_ALLOCATOR_H_
#define CPPUAL_MEMORY_LINEAR_ALLOCATOR_H_
#ifdef __cplusplus

#include <cppual/memory/memory.h>

using std::string;
using std::atomic_size_t;

namespace cppual { namespace Memory {

template <class>
class LinearPolicy;

class LinearAllocator final : public IAllocator
{
public:
	LinearAllocator (size_type size);
	LinearAllocator (IAllocator& allocator, size_type size);
	//LinearAllocator (string& shared_name, size_type size);
	~LinearAllocator ();

	void* allocate (size_type size, align_type align) noexcept override;
	void  clear () noexcept;

	inline void deallocate (void*, size_type) noexcept override
	{ }

	inline AllocatorType type () const noexcept
	{ return AllocatorType::Linear; }

	inline size_type maxSize () const noexcept
	{ return static_cast<size_type> (m_pEnd - m_pMarker); }

	inline bool isValid () const noexcept
	{ return m_pEnd != nullptr; }

	inline size_type count () const noexcept
	{ return m_uNumAlloc; }

	inline size_type size () const noexcept
	{ return static_cast<size_type> (m_pEnd - m_pBegin); }

	inline MemoryType memoryType () const noexcept
	{ return m_eMemoryType;  }

	inline IAllocator& owner () const noexcept
	{ return m_gOwner; }

private:
	//string      const m_gSharedName;
	size_type         m_uNumAlloc;
	IAllocator&       m_gOwner;
	pointer     const m_pBegin;
	pointer     const m_pEnd;
	pointer           m_pMarker;
	MemoryType  const m_eMemoryType;
};

} } // Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_LINEAR_ALLOCATOR_H_
