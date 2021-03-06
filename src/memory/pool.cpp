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

#include <cassert>
#include <cppual/memory/pool.h>

using namespace std;

namespace cppual { namespace Memory {

MonotonicResource::MonotonicResource (size_type  uBlkCount,
                                      size_type  uBlkSize,
                                      align_type uBlkAlign)
: m_gOwner    (*this),
  m_pBegin    (uBlkCount ? ::operator new (uBlkCount * uBlkSize) : nullptr),
  m_pEnd      (m_pBegin != nullptr ?
                           static_cast<math_pointer> (m_pBegin) + (uBlkCount * uBlkSize) :
                           nullptr),
  m_pFreeList (),
  m_uBlkSize  (uBlkSize),
  m_uBlkAlign (uBlkAlign),
  m_uBlkNum   ()
{
    initialize ();
}

MonotonicResource::MonotonicResource (MemoryResource& pOwner,
                                      size_type   uBlkCount,
                                      size_type   uBlkSize,
                                      align_type  uBlkAlign)
: m_gOwner ((uBlkCount * uBlkSize) > pOwner.max_size () ? *this : pOwner),
  m_pBegin (&m_gOwner != this ?
                             (uBlkCount and uBlkSize and uBlkAlign ?
                                  static_cast<pointer> (pOwner.allocate (
                                                            uBlkCount * uBlkSize,
                                                            alignof (size_type))) :
                                  nullptr) :
                            (uBlkCount? ::operator new (uBlkCount * uBlkSize) : nullptr)),
  m_pEnd (m_pBegin != nullptr ? static_cast<math_pointer> (m_pBegin) + (uBlkCount * uBlkSize) :
                                nullptr),
  m_pFreeList (),
  m_uBlkSize  (uBlkSize),
  m_uBlkAlign (uBlkAlign),
  m_uBlkNum   ()
{
    initialize ();
}

//MonotonicPool::MonotonicPool (SharedObject& gObj,
//                              size_type     uBlkCount,
//                              size_type     uBlkSize,
//                              align_type    uBlkAlign)
//: m_gOwner (this),
//  m_gSharedMem (gObj, uBlkCount * uBlkSize),
//  m_pBegin (static_cast<pointer> (m_gSharedMem.address ())),
//  m_pEnd (m_pBegin != nullptr ? m_pBegin + uBlkCount * uBlkSize : nullptr),
//  m_pFreeList (),
//  m_uBlkSize (uBlkSize),
//  m_uBlkAlign (uBlkAlign),
//  m_uBlkNum (true)
//{
//    initialize ();
//}

MonotonicResource::~MonotonicResource ()
{
    if (m_pBegin == nullptr) return;

    if (&m_gOwner != this) m_gOwner.deallocate (m_pBegin, capacity ());
    else ::operator delete (m_pBegin);
}

void MonotonicResource::initialize () noexcept
{
    if (m_pBegin)
    {
        size_type uAdjust = alignAdjust (m_pBegin, m_uBlkAlign);

        pointer* p = m_pFreeList =
                reinterpret_cast<pointer*> (static_cast<math_pointer> (m_pBegin) + uAdjust);

        m_uBlkNum = ((m_uBlkSize - uAdjust) / m_uBlkSize) - 1;

        for (size_type i = 0; i < m_uBlkNum; ++i)
        {
            *p = reinterpret_cast<pointer>  ( p + m_uBlkSize);
             p = reinterpret_cast<pointer*> (*p);
        }

        *p = nullptr;
        ++m_uBlkNum;
    }
}

void* MonotonicResource::do_allocate (size_type uSize, align_type uAlign) noexcept
{
    // check if size and alignment match with the ones from
    // the current pool instance
    if (!m_pFreeList or uSize != m_uBlkSize or uAlign != m_uBlkAlign)
        return nullptr;

    pointer p   = reinterpret_cast<pointer > ( m_pFreeList);
    m_pFreeList = reinterpret_cast<pointer*> (*m_pFreeList);

    return p;
}

void MonotonicResource::do_deallocate (void* p, size_type uSize, align_type)
{
    if (uSize != m_uBlkSize) throw std::length_error ("not equal to blocksize");
    if (m_pEnd <= p or p < m_pBegin) throw std::out_of_range ("pointer is outside the buffer");

    *(reinterpret_cast<pointer*> (p)) = reinterpret_cast<pointer> (m_pFreeList);
    m_pFreeList = reinterpret_cast<pointer*> (p);
}

void MonotonicResource::clear () noexcept
{
    m_pFreeList = reinterpret_cast<pointer*> (static_cast<math_pointer> (m_pBegin) +
                                              alignAdjust (m_pBegin, m_uBlkAlign));
}

} } // namespace Memory
