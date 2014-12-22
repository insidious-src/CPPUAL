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

#include <cppual/memory/linear.h>

using std::string;

namespace cppual { namespace Memory {

LinearAllocator::LinearAllocator (size_type uSize)
: //m_gSharedName (),
  m_uNumAlloc (),
  m_gOwner (*this),
  m_pBegin (uSize > 0 ? new u8[uSize] : nullptr),
  m_pEnd (m_pBegin != nullptr ? m_pBegin + uSize : nullptr),
  m_pMarker (m_pBegin),
  m_eMemoryType (MemoryType::Local)
{ }

LinearAllocator::LinearAllocator (IAllocator& pOwner, size_type uSize)
: //m_gSharedName (),
  m_uNumAlloc (),
  m_gOwner (uSize > pOwner.maxSize () ? *this : pOwner),
  m_pBegin (&m_gOwner != this ?
							 (uSize > 0 ?
								  static_cast<u8*> (pOwner.allocate (
														uSize, alignof (size_type))) :
								  nullptr) :
							 (uSize > 0 ? new u8[uSize] : nullptr)),
  m_pEnd (m_pBegin != nullptr ? m_pBegin + uSize : nullptr),
  m_pMarker (m_pBegin),
  m_eMemoryType (&m_gOwner != this ? m_gOwner.memoryType () :
									 MemoryType::Local)
{ }

//LinearAllocator::LinearAllocator (string& gName, size_type uSize)
//: //m_gSharedName (std::move (gName)),
//  m_uNumAlloc (),
//  m_gOwner (*this),
//  m_pBegin (uSize > 0 ? new (gName) u8[uSize] : nullptr),
//  m_pEnd (m_pBegin != nullptr ? m_pBegin + uSize : nullptr),
//  m_pMarker (m_pBegin),
//  m_eMemoryType (MemoryType::Shared)
//{ }

LinearAllocator::~LinearAllocator ()
{
	if (m_pBegin == nullptr) return;

	if (&m_gOwner != this) m_gOwner.deallocate (m_pBegin, size ());
	else if (m_eMemoryType == MemoryType::Local) delete[] m_pBegin;
}

void* LinearAllocator::allocate (size_type uSize, align_type uAlign) noexcept
{
	if (!uSize) return nullptr;
	size_type uAdjust = align (m_pMarker, uAlign);

	if ((m_pMarker + uAdjust + uSize) > m_pEnd)
		return nullptr;

	pointer pAlignedAddr = m_pMarker + uAdjust;
	m_pMarker = pAlignedAddr + uSize;

	++m_uNumAlloc;
	return pAlignedAddr;
}

void LinearAllocator::clear () noexcept
{
	m_pMarker   = m_pBegin;
	m_uNumAlloc = 0;
}

} } // Memory
