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

#include <cppual/memory/heap.h>
#include <cppual/memory/mop.h>

namespace cppual { namespace Memory {

HeapResource::HeapResource (size_type uSize)
: m_gOwner (*this),
  m_pBegin (uSize >= sizeof (FreeBlock) ? ::operator new (uSize) : nullptr),
  m_pEnd (m_pBegin != nullptr ? address (m_pBegin, uSize) : nullptr),
  m_pFreeBlocks (reinterpret_cast<FreeBlock*> (m_pBegin)),
  m_bIsMemShared ()
{
    if (m_pFreeBlocks != nullptr)
    {
        m_pFreeBlocks->size = uSize;
        m_pFreeBlocks->next = nullptr;
    }
}

HeapResource::HeapResource (MemoryResource& pOwner, size_type uSize)
: m_gOwner (uSize > pOwner.max_size () ? *this : pOwner),
  m_pBegin (&m_gOwner != this ?
                             (uSize >= sizeof (FreeBlock) ?
                                  static_cast<pointer> (pOwner.allocate (
                                                            uSize, alignof (size_type))) :
                                  nullptr) :
                             (uSize > 0 ? ::operator new (uSize) : nullptr)),
  m_pEnd (m_pBegin != nullptr ? address (m_pBegin, uSize) : nullptr),
  m_pFreeBlocks (reinterpret_cast<FreeBlock*> (m_pBegin)),
  m_bIsMemShared (&m_gOwner != this ? m_gOwner.is_shared () : false)
{
    if (m_pFreeBlocks != nullptr)
    {
        m_pFreeBlocks->size = uSize;
        m_pFreeBlocks->next = nullptr;
    }
}

//HeapPool::HeapPool (string& gName, size_type uSize)
//: m_gSharedName (std::move (gName)),
//  m_gOwner (*this),
//  m_pBegin (uSize >= sizeof (FreeBlock) ? new (gName) u8[uSize] : nullptr),
//  m_pEnd (m_pBegin != nullptr ? address (m_pBegin, uSize) : nullptr),
//  m_pFreeBlocks (reinterpret_cast<FreeBlock*> (m_pBegin)),
//  m_bIsMemShared (true)
//{
//    if (m_pFreeBlocks != nullptr)
//    {
//        m_pFreeBlocks->size = uSize;
//        m_pFreeBlocks->next = nullptr;
//    }
//}

HeapResource::~HeapResource ()
{
    if (m_pBegin == nullptr) return;

    if      (&m_gOwner != this) m_gOwner.deallocate (m_pBegin, capacity ());
    else if (!m_bIsMemShared) ::operator delete (m_pBegin);
}

void* HeapResource::do_allocate (size_type uSize, align_type uAlign) noexcept
{
    static_assert (sizeof (Header) >= sizeof (FreeBlock), "Header is not big enough!");
    if (!m_pBegin or !uSize) return nullptr;

    // check free blocks
    FreeBlock* pPrevFreeBlock = nullptr;
    FreeBlock* pFreeBlock     = m_pFreeBlocks;

    while (pFreeBlock)
    {
        // calculate uAdjust needed to keep object correctly aligned
        size_type uAdjust = alignAdjustmentHeader (pFreeBlock,
                                                   uAlign,
                                                   sizeof (Header));

        // if allocation doesn't fit in this FreeBlock, try the next
        if(pFreeBlock->size < (uSize + uAdjust))
        {
            pPrevFreeBlock = pFreeBlock;
            pFreeBlock     = pFreeBlock->next;
            continue;
        }

        // if allocations in the remaining memory will be impossible
        if((pFreeBlock->size - uSize - uAdjust) <= sizeof (Header))
        {
            // increase allocation size instead of creating a new FreeBlock
            uSize = pFreeBlock->size;

            if(pPrevFreeBlock != nullptr) pPrevFreeBlock->next = pFreeBlock->next;
            else m_pFreeBlocks = pFreeBlock->next;
        }
        else
        {
            // else create a new FreeBlock containing remaining memory
            FreeBlock* pNextBlock = pFreeBlock + uSize + uAdjust;
            pNextBlock->size      = pFreeBlock->size - uSize - uAdjust;
            pNextBlock->next      = pFreeBlock->next;

            if(pPrevFreeBlock != nullptr)
                pPrevFreeBlock->next = pNextBlock;
            else
                m_pFreeBlocks = pNextBlock;
        }

        size_type uAlignedAddr = reinterpret_cast<size_type> (pFreeBlock) + uAdjust;

        Header* pHeader = reinterpret_cast<Header*> (uAlignedAddr - sizeof (Header));
        pHeader->size   = uSize + uAdjust;
        pHeader->adjust = uAdjust;

        return reinterpret_cast<void*> (uAlignedAddr);
    }

    // couldn't find free block large enough!
    return nullptr;
}

void HeapResource::do_deallocate (void* p, size_type uSize, align_type)
{
    if (m_pEnd <= p or p < m_pBegin) throw std::out_of_range ("pointer is outside the buffer");

    Header* pHeader = reinterpret_cast<Header*>
                      (static_cast<math_pointer> (p) - sizeof (Header));

    if (pHeader->size < uSize) throw std::length_error ("deallocate size doesn't match");
    uSize = pHeader->size;

    size_type uBlockStart = reinterpret_cast<size_type> (p) - pHeader->adjust;
    size_type uBlockEnd   = uBlockStart + uSize;
    size_type uBlockSize  = uSize;

    bool blockMerged = false;
    bool bSearch     = true;

    while (bSearch)
    {
        bSearch = false;

        FreeBlock* pPrevFreeBlock = nullptr;
        FreeBlock* pFreeBlock     = m_pFreeBlocks;

        while (pFreeBlock != nullptr)
        {
            if (reinterpret_cast<size_type> (pFreeBlock) + pFreeBlock->size == uBlockStart)
            {
                pFreeBlock->size += uBlockSize;

                uBlockStart = reinterpret_cast<size_type> (pFreeBlock);
                uBlockEnd   = uBlockStart + pFreeBlock->size;
                uBlockSize  = pFreeBlock->size;

                bSearch     = true;
                blockMerged = true;
                break;
            }
            else if (uBlockEnd == reinterpret_cast<size_type> (pFreeBlock))
            {
                FreeBlock* pNewFreeBlock = reinterpret_cast<FreeBlock*> (uBlockStart);
                pNewFreeBlock->next      = pFreeBlock->next;
                pNewFreeBlock->size      = uBlockSize + pFreeBlock->size;

                if(pFreeBlock == m_pFreeBlocks)
                    m_pFreeBlocks = pNewFreeBlock;
                else if(pPrevFreeBlock != pNewFreeBlock)
                    pPrevFreeBlock->next = pNewFreeBlock;

                uBlockStart = reinterpret_cast<size_type> (pNewFreeBlock);
                uBlockEnd   = uBlockStart + pNewFreeBlock->size;
                uBlockSize  = pNewFreeBlock->size;

                bSearch     = true;
                blockMerged = true;
                break;
            }

            pPrevFreeBlock = pFreeBlock;
            pFreeBlock     = pFreeBlock->next;
        }
    }

    if (!blockMerged)
    {
        FreeBlock* pBlock = reinterpret_cast<FreeBlock*> (reinterpret_cast<math_pointer>(p) - pHeader->adjust);
        pBlock->size      = uBlockSize;
        pBlock->next      = m_pFreeBlocks;
        m_pFreeBlocks     = pBlock;
    }
}

void HeapResource::clear () noexcept
{
    m_pFreeBlocks       = reinterpret_cast<FreeBlock*> (m_pBegin);
    m_pFreeBlocks->size = capacity ();
    m_pFreeBlocks->next = nullptr;
}

// =========================================================
// List Allocator
// =========================================================

inline ListResource::Header* header (void* p) noexcept
{
    return reinterpret_cast<ListResource::Header*>
            (reinterpret_cast<uptr> (p) - sizeof (ListResource::Header));
}

inline void* forward (ListResource::Header*    const hdr,
                      ListResource::Header*    const prev_hdr,
                      ListResource::size_type  const size,
                      ListResource::align_type const align) noexcept
{
    typedef ListResource::Header Header;

    // get aligned address from header pointer
    void* ptr = nextAlignedAddr (hdr, align);

    // determine the location of the new forwarding header
    Header* split_hdr = static_cast<Header*> (ptr) + size;

    split_hdr->size = hdr->size - size;
    split_hdr->next = hdr->next;

    if (prev_hdr) prev_hdr->next = split_hdr;
    return ptr;
}

inline ListResource::Header* find (ListResource::Header*          hdr,
                                   ListResource::Header*&         prev_hdr,
                                   ListResource::size_type  const size,
                                   ListResource::align_type const align) noexcept
{
    typedef ListResource::Header       Header ;
    typedef ListResource::math_pointer pointer;

    for (Header* pCurHeader = hdr; pCurHeader;
         prev_hdr = pCurHeader, pCurHeader = pCurHeader->next)
    {
        if (pCurHeader->size >= size and
                (static_cast<pointer> (nextAlignedAddr (pCurHeader, align)) + pCurHeader->size)
                 <= static_cast<pointer> (static_cast<void*> (pCurHeader + pCurHeader->size)))
            return pCurHeader;
    }

    // couldn't find free block large enough!
    return nullptr;
}

// =========================================================

ListResource::ListResource (size_type uSize)
: m_pBegin (uSize > sizeof (Header) ? ::operator new (uSize + sizeof (Header)) : nullptr),
  m_pEnd (m_pBegin != nullptr ? address (m_pBegin, uSize + sizeof (Header)) : nullptr),
  m_pFirstFreeBlock (static_cast<Header*> (m_pBegin)),
  m_gOwner (*this),
  m_bIsMemShared ()
{
    if (m_pFirstFreeBlock != nullptr)
    {
        m_pFirstFreeBlock->size = capacity () - sizeof (Header);
        m_pFirstFreeBlock->next = nullptr;
    }
}

ListResource::ListResource (MemoryResource& pOwner, size_type uSize)
: m_pBegin (uSize > sizeof (Header) and uSize <= pOwner.max_size () ?
                static_cast<pointer> (pOwner.allocate (uSize, alignof (Header))) :
                nullptr),
  m_pEnd (m_pBegin != nullptr ? address (m_pBegin, uSize) : nullptr),
  m_pFirstFreeBlock (static_cast<Header*> (m_pBegin)),
  m_gOwner (m_pBegin ? pOwner : *this),
  m_bIsMemShared (&m_gOwner != this ? m_gOwner.is_shared () : false)
{
    if (m_pFirstFreeBlock != nullptr)
    {
        m_pFirstFreeBlock->size = distance (m_pEnd, m_pBegin);
        m_pFirstFreeBlock->next = nullptr;
    }
}

ListResource::~ListResource ()
{
    if (m_pBegin == nullptr) return;

    if (&m_gOwner != this) m_gOwner.deallocate (m_pBegin, capacity ());
    else if (!m_bIsMemShared) ::operator delete (m_pBegin);
}

void ListResource::defragment () noexcept
{
    for (Header* pCurHeader = m_pFirstFreeBlock; pCurHeader;
         pCurHeader = pCurHeader->next)
    {
        for (Header* pItHeader = m_pFirstFreeBlock->next; pItHeader;
             pItHeader = pItHeader->next)
        {
            if ((pCurHeader + pCurHeader->size) == pItHeader)
            {
                pCurHeader->size = pItHeader->size + pCurHeader->size;
                pCurHeader->next = pItHeader->next;
            }
        }
    }
}

void* ListResource::do_allocate (size_type uSize, align_type uAlign) noexcept
{
    if (m_pFirstFreeBlock and uSize)
    {
        Header* pPrevHeader   = nullptr;
        Header* pTargetHeader = find (m_pFirstFreeBlock, pPrevHeader, uSize, uAlign);

        if (pTargetHeader)
            return forward (pTargetHeader, pPrevHeader, uSize, uAlign);
    }

    // couldn't find free block large enough!
    return nullptr;
}

void ListResource::do_deallocate (void* p, size_type uSize, align_type)
{
    if (m_pEnd <= p or p < m_pBegin) throw std::out_of_range ("pointer is outside the buffer");

    Header* pHeader = header (p);

    if (pHeader->size < uSize) throw std::length_error ("deallocate size doesn't match");
    uSize = pHeader->size;
}

void ListResource::clear () noexcept
{
    m_pFirstFreeBlock       = static_cast<Header*> (static_cast<void*> (m_pBegin));
    m_pFirstFreeBlock->size = distance (m_pEnd, m_pBegin);
    m_pFirstFreeBlock->next = nullptr;
}

ListResource::size_type ListResource::max_size () const noexcept
{
    size_type uMaxSize = 0;

    for (Header* pCurHeader = m_pFirstFreeBlock; pCurHeader; pCurHeader = pCurHeader->next)
        if (uMaxSize < pCurHeader->size) uMaxSize = pCurHeader->size;
    return uMaxSize;
}

} } // namespace Memory
