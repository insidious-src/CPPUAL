/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cppual/memory/stack.h>
#include <cppual/memory/mop.h>

using std::string;

namespace cppual { namespace Memory {

StackedPool::StackedPool (size_type uSize)
: //m_gSharedName (),
  m_gOwner (*this),
  m_pCurMarker (uSize > 0 ? ::operator new (uSize) : nullptr),
  m_pBegin (m_pCurMarker),
  m_pEnd (m_pBegin != nullptr ? static_cast<math_pointer> (m_pBegin) + uSize : nullptr),
  m_bIsMemShared ()
{ }

StackedPool::StackedPool (Repository& pOwner, size_type uSize)
: //m_gSharedName (),
  m_gOwner (uSize > pOwner.max_size () ? *this : pOwner),
  m_pCurMarker (&m_gOwner != this ?
                             (uSize > 0 ?
                                  static_cast<pointer> (pOwner.allocate (
                                                            uSize, alignof (uptr))) :
                                  nullptr) :
                             (uSize > 0 ? ::operator new (uSize) : nullptr)),
  m_pBegin (m_pCurMarker),
  m_pEnd (m_pBegin != nullptr ? static_cast<math_pointer> (m_pBegin) + uSize : nullptr),

  m_bIsMemShared (&m_gOwner != this ? m_gOwner.is_shared () : false)
{ }

// not implemented
//StackedPool::StackedPool (string& gName, size_type uSize)
//: m_gSharedName (std::move (gName)),
//  m_gOwner (*this),
//  m_pCurMarker (m_pBegin),
//  m_pBegin (uSize > 0 ? new (gName) u8[uSize] : nullptr),
//  m_pEnd (m_pBegin != nullptr ? m_pBegin + uSize : nullptr),
//  m_bIsMemShared (true)
//{ }

StackedPool::~StackedPool ()
{
    if (m_pBegin == nullptr) return;

    if (&m_gOwner != this) m_gOwner.deallocate (m_pBegin, capacity ());
    else if (!m_bIsMemShared) ::operator delete (m_pBegin);
}

void* StackedPool::do_allocate (size_type uBytes, align_type uAlign) noexcept
{
    // check whether there is enough space to allocate
    if (!m_pBegin or !uBytes) return nullptr;

    size_type const uAdjust    = alignAdjust (m_pCurMarker, uAlign);
    pointer   const pNewMarker = static_cast<math_pointer> (m_pCurMarker) + uAdjust + uBytes;

    if (pNewMarker > m_pEnd) return nullptr;

    // shift the marker to the next available address
    // and increment the allocation count
    return m_pCurMarker = pNewMarker;
}

void StackedPool::do_deallocate (void* p, size_type uSize, align_type)
{
    if ((static_cast<math_pointer> (p) + uSize) != m_pCurMarker)
        throw std::out_of_range ("pointer doesn't match the last element");

    // shift the marker to the end of the previous allocated block
    // and decrement the allocation count
    m_pCurMarker = p;
}

// =========================================================

DStackedPool::DStackedPool (size_type uSize, size_type uHint)
: //m_gSharedName (),
  m_gOwner (*this),
  m_pTopMarker (uSize > 0 ? ::operator new[] (uSize) : nullptr),
  m_pBottomMarker (m_pTopMarker != nullptr ? static_cast<math_pointer> (m_pTopMarker) + uSize :
                                             nullptr),
  m_pBegin (m_pTopMarker),
  m_uHint (uHint),
  m_pEnd (m_pBottomMarker),
  m_bIsMemShared ()
{ }

DStackedPool::DStackedPool (Repository& pOwner,
                            size_type   uSize,
                            size_type   uHint)
: //m_gSharedName (),
  m_gOwner (uSize > pOwner.max_size () ? *this : pOwner),
  m_pTopMarker (&m_gOwner != this ?
                                 (uSize > 0 ?
                                      static_cast<pointer> (pOwner.allocate (
                                                                uSize, alignof (uptr))) :
                                      nullptr) :
                                 (uSize > 0 ? ::operator new[] (uSize) : nullptr)),
  m_pBottomMarker (m_pTopMarker != nullptr ? static_cast<math_pointer> (m_pTopMarker) + uSize :
                                             nullptr),
  m_pBegin (m_pTopMarker),
  m_uHint (uHint),
  m_pEnd (m_pBottomMarker),
  m_bIsMemShared (&m_gOwner != this ? m_gOwner.is_shared () : true)
{ }

//DStackedPool::DStackedPool (string&   gName,
//                                      size_type uSize,
//                                      size_type uHint)
//: m_gSharedName (std::move (gName)),
//  m_gOwner (*this),
//  m_pTopMarker (uSize > 0 ? new (gName) u8[uSize] : nullptr),
//  m_pBottomMarker (m_pTopMarker != nullptr ? m_pTopMarker + uSize : nullptr),
//  m_pBegin (m_pTopMarker),
//  m_uHint (uHint),
//  m_pEnd (m_pBottomMarker),
//  m_bIsMemShared (true)
//{ }

DStackedPool::~DStackedPool ()
{
    if (m_pBegin == nullptr) return;

    if (&m_gOwner != this) m_gOwner.deallocate (m_pBegin, capacity ());
    else if (!m_bIsMemShared) delete[] static_cast<math_pointer> (m_pBegin);
}

void* DStackedPool::do_allocate (size_type uBytes, align_type uAlign) noexcept
{
    // check whether there is enough space to allocate
    if (!m_pBegin or !uBytes) return nullptr;

    size_type const uAdjust       = alignAdjust (m_pTopMarker, uAlign);
    pointer   const pNewTopMarker = static_cast<math_pointer> (m_pTopMarker) + uAdjust + uBytes;

    if (pNewTopMarker > m_pBottomMarker) return nullptr;

    return uBytes < m_uHint ?
                m_pTopMarker = pNewTopMarker :
                               m_pBottomMarker = static_cast<math_pointer> (m_pBottomMarker) -
                                                 alignAdjust (m_pTopMarker, uAlign) - uBytes;
}

void DStackedPool::do_deallocate (void* p, size_type uBytes, align_type)
{
    // if the pointer belongs to the top side
    if ((static_cast<math_pointer> (p) + (uBytes = alignedSize (uBytes))) == m_pTopMarker)
        m_pTopMarker = p;
    // if the pointer belongs to the bottom side
    else if (p == m_pBottomMarker)
        m_pBottomMarker = static_cast<math_pointer> (p) + uBytes;
    else
        throw std::out_of_range ("pointer doesn't match the last element is both markers");
}

} } // namespace Memory
