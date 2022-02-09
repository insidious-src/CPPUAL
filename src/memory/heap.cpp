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

#include <cppual/memory/heap.h>
#include <cppual/memory/mop.h>

#include <iostream>

namespace cppual { namespace memory {

heap_resource::heap_resource (size_type uSize)
: _M_gOwner (uSize > sizeof (free_block) ? get_default_resource()->max_size() >= uSize ?
                           *get_default_resource() : *new_delete_resource() : *this),
  _M_pBegin (&_M_gOwner != this ?
                 _M_gOwner.allocate (uSize + max_align * 2, alignof (uptr)) : nullptr),
  _M_pEnd (_M_pBegin != nullptr ?
            static_cast<math_pointer> (_M_pBegin) + uSize + max_align * 2 : nullptr),
  _M_pFreeBlocks (reinterpret_cast<free_block*> (_M_pBegin)),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (_M_pFreeBlocks != nullptr)
    {
        _M_pFreeBlocks->size = uSize;
        _M_pFreeBlocks->next = nullptr;
    }
}

heap_resource::heap_resource (pointer buffer, size_type uSize)
: _M_gOwner (*this),
  _M_pBegin (buffer && uSize > sizeof (free_block) ? buffer : nullptr),
  _M_pEnd (_M_pBegin != nullptr ? static_cast<math_pointer> (buffer) + uSize : nullptr),
  _M_pFreeBlocks (reinterpret_cast<free_block*> (_M_pBegin)),
  _M_bIsMemShared ()
{
    if (_M_pFreeBlocks != nullptr)
    {
        _M_pFreeBlocks->size = uSize;
        _M_pFreeBlocks->next = nullptr;
    }
    else
    {
        throw std::bad_alloc ();
    }
}

heap_resource::heap_resource (memory_resource& pOwner, size_type uSize)
: _M_gOwner (uSize > sizeof(free_block) ? uSize > pOwner.max_size () ?
                                          uSize > get_default_resource()->max_size() ?
                            *new_delete_resource() : *get_default_resource() : pOwner : *this),
  _M_pBegin (&_M_gOwner != this ?
                 _M_gOwner.allocate (uSize + max_align * 2, alignof (uptr)) : nullptr),
  _M_pEnd (_M_pBegin != nullptr ?
            static_cast<math_pointer> (_M_pBegin) + uSize + max_align * 2 : nullptr),
  _M_pFreeBlocks (static_cast<free_block*> (_M_pBegin)),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (_M_pFreeBlocks != nullptr)
    {
        _M_pFreeBlocks->size = uSize;
        _M_pFreeBlocks->next = nullptr;
    }

    if (&_M_gOwner != &pOwner)
    {
        std::cerr << __func__
                  << " :: Specified owner cannot be assigned -> 'max_size' exceeded. "
                     "Using default memory resource instead." << std::endl;
    }
}

heap_resource::~heap_resource ()
{
    if (_M_pBegin && &_M_gOwner != this) _M_gOwner.deallocate (_M_pBegin, capacity (), alignof (uptr));
}

void* heap_resource::do_allocate (size_type uSize, align_type uAlign) noexcept
{
    static_assert (sizeof (header) >= sizeof (free_block), "Header is not big enough!");
    if (!_M_pBegin or !uSize) return nullptr;

    // check free blocks
    free_block* pPrevFreeBlock = nullptr;
    free_block* pFreeBlock     = _M_pFreeBlocks;

    while (pFreeBlock)
    {
        // calculate uAdjust needed to keep object correctly aligned
        size_type uAdjust = align_adjustment_header (pFreeBlock, uAlign, sizeof (header));

        // if allocation doesn't fit in this FreeBlock, try the next
        if(pFreeBlock->size < (uSize + uAdjust))
        {
            pPrevFreeBlock = pFreeBlock;
            pFreeBlock     = pFreeBlock->next;
            continue;
        }

        // if allocations in the remaining memory will be impossible
        if((pFreeBlock->size - uSize - uAdjust) <= sizeof (header))
        {
            // increase allocation size instead of creating a new FreeBlock
            uSize = pFreeBlock->size;

            if(pPrevFreeBlock != nullptr) pPrevFreeBlock->next = pFreeBlock->next;
            else _M_pFreeBlocks = pFreeBlock->next;
        }
        else
        {
            // else create a new FreeBlock containing remaining memory
            free_block* pNextBlock = pFreeBlock + uSize + uAdjust;
            pNextBlock->size      = pFreeBlock->size - uSize - uAdjust;
            pNextBlock->next      = pFreeBlock->next;

            if(pPrevFreeBlock != nullptr)
                pPrevFreeBlock->next = pNextBlock;
            else
                _M_pFreeBlocks = pNextBlock;
        }

        auto uAlignedAddr = reinterpret_cast<uptr> (pFreeBlock) + uAdjust;

        header* pHeader = reinterpret_cast<header*> (uAlignedAddr - sizeof (header));
        pHeader->size   = uSize + uAdjust;
        pHeader->adjust = uAdjust;

        return reinterpret_cast<void*> (uAlignedAddr);
    }

    // couldn't find free block large enough!
    return nullptr;
}

void heap_resource::do_deallocate (void* p, size_type uSize, align_type)
{
    if (_M_pEnd <= p or p < _M_pBegin) throw std::out_of_range ("pointer is outside the buffer");

    header* pHeader = reinterpret_cast<header*>
                      (static_cast<math_pointer> (p) - sizeof (header));

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

        free_block* pPrevFreeBlock = nullptr;
        free_block* pFreeBlock     = _M_pFreeBlocks;

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
                free_block* pNewFreeBlock = reinterpret_cast<free_block*> (uBlockStart);
                pNewFreeBlock->next      = pFreeBlock->next;
                pNewFreeBlock->size      = uBlockSize + pFreeBlock->size;

                if(pFreeBlock == _M_pFreeBlocks)
                    _M_pFreeBlocks = pNewFreeBlock;
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
        free_block* pBlock = reinterpret_cast<free_block*> (reinterpret_cast<math_pointer>(p) - pHeader->adjust);
        pBlock->size      = uBlockSize;
        pBlock->next      = _M_pFreeBlocks;
        _M_pFreeBlocks     = pBlock;
    }
}

void heap_resource::clear () noexcept
{
    _M_pFreeBlocks       = reinterpret_cast<free_block*> (_M_pBegin);
    _M_pFreeBlocks->size = capacity ();
    _M_pFreeBlocks->next = nullptr;
}

// =========================================================
// List Allocator
// =========================================================

inline list_resource::header* shift_to_header (void* p) noexcept
{
    return reinterpret_cast<list_resource::header*>
            (reinterpret_cast<uptr> (p) - sizeof (list_resource::header));
}

inline void* forward_alloc (list_resource::header*    const hdr,
                            list_resource::header*    const prev_hdr,
                            list_resource::size_type  const size,
                            list_resource::align_type const align) noexcept
{
    typedef list_resource::header header;

    // get aligned address from header pointer
    void* ptr = next_aligned_addr (hdr, align);

    // determine the location of the new forwarding header
    header* split_hdr = static_cast<header*> (ptr) + size;

    split_hdr->size = hdr->size - size;
    split_hdr->next = hdr->next;

    if (prev_hdr) prev_hdr->next = split_hdr;
    return ptr;
}

inline list_resource::header* find (list_resource::header*          hdr,
                                    list_resource::header*&         prev_hdr,
                                    list_resource::size_type  const size,
                                    list_resource::align_type const align) noexcept
{
    typedef list_resource::header       header ;
    typedef list_resource::math_pointer pointer;

    for (header* pCurHeader = hdr; pCurHeader; prev_hdr = pCurHeader, pCurHeader = pCurHeader->next)
    {
        if (pCurHeader->size >= size &&
                (static_cast<pointer> (next_aligned_addr (pCurHeader, align)) + pCurHeader->size)
                 <= static_cast<pointer> (static_cast<void*> (pCurHeader + pCurHeader->size)))
            return pCurHeader;
    }

    // couldn't find free block large enough!
    return nullptr;
}

// =========================================================

list_resource::list_resource (size_type uSize)
: _M_gOwner (uSize > sizeof (header) ? get_default_resource ()->max_size () >= uSize + max_align * 2 ?
                                      *get_default_resource () : *new_delete_resource () : *this),
  _M_pBegin (&_M_gOwner != this ?
                 _M_gOwner.allocate (uSize + max_align * 2, alignof (header)) : nullptr),
  _M_pEnd (_M_pBegin != nullptr ?
            static_cast<math_pointer> (_M_pBegin) + uSize + max_align * 2 : nullptr),
  _M_pFirstFreeBlock (static_cast<header*> (_M_pBegin)),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (_M_pFirstFreeBlock != nullptr)
    {
        _M_pFirstFreeBlock->size = capacity () - sizeof (header);
        _M_pFirstFreeBlock->next = nullptr;
    }
}

list_resource::list_resource (pointer buffer, size_type uSize)
: _M_gOwner(*this),
  _M_pBegin (buffer && uSize > sizeof (header) ? buffer : nullptr),
  _M_pEnd (_M_pBegin != nullptr ? static_cast<math_pointer> (_M_pBegin) + uSize : nullptr),
  _M_pFirstFreeBlock (static_cast<header*> (_M_pBegin)),
  _M_bIsMemShared ()
{
    if (_M_pFirstFreeBlock != nullptr)
    {
        _M_pFirstFreeBlock->size = capacity () - sizeof (header);
        _M_pFirstFreeBlock->next = nullptr;
    }
    else
    {
        throw std::bad_alloc ();
    }
}

list_resource::list_resource (memory_resource& pOwner, size_type uSize)
: _M_gOwner (uSize > sizeof (header) ? (uSize + max_align * 2) >  pOwner.max_size () ?
                                       (uSize + max_align * 2) >  get_default_resource ()->max_size () ?
                                       *new_delete_resource () : *get_default_resource () : pOwner : *this),
  _M_pBegin (&_M_gOwner != this ?
                 _M_gOwner.allocate (uSize + max_align * 2, alignof (header)) : nullptr),
  _M_pEnd (_M_pBegin != nullptr ?
          static_cast<math_pointer> (_M_pBegin) + uSize + max_align * 2 : nullptr),
  _M_pFirstFreeBlock (static_cast<header*> (_M_pBegin)),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (_M_pFirstFreeBlock != nullptr)
    {
        _M_pFirstFreeBlock->size = distance (_M_pEnd, _M_pBegin);
        _M_pFirstFreeBlock->next = nullptr;
    }

    if (&_M_gOwner != &pOwner)
    {
        std::cerr << __func__
                  << " :: Specified owner cannot be assigned -> 'max_size' exceeded. "
                     "Using default memory resource instead." << std::endl;
    }
}

list_resource::~list_resource ()
{
    if (_M_pBegin && &_M_gOwner != this) _M_gOwner.deallocate (_M_pBegin, capacity ());
}

void list_resource::defragment () noexcept
{
    for (header* pCurHeader = _M_pFirstFreeBlock; pCurHeader;
         pCurHeader = pCurHeader->next)
    {
        for (header* pItHeader = _M_pFirstFreeBlock->next; pItHeader;
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

void* list_resource::do_allocate (size_type uSize, align_type uAlign) noexcept
{
    if (_M_pFirstFreeBlock and uSize)
    {
        header* pPrevHeader   = nullptr;
        header* pTargetHeader = find (_M_pFirstFreeBlock, pPrevHeader, uSize, uAlign);

        if (pTargetHeader)
            return forward_alloc (pTargetHeader, pPrevHeader, uSize, uAlign);
    }

    // couldn't find free block large enough!
    return nullptr;
}

void list_resource::do_deallocate (void* p, size_type uSize, align_type /*uAlign*/)
{
    if (!_M_pFirstFreeBlock || _M_pEnd <= p || p < _M_pBegin)
        throw std::out_of_range ("pointer is outside the buffer");

    auto pHeader = shift_to_header (p);
    auto bFound  = false;

    if (pHeader->size != uSize) throw std::length_error ("deallocate size doesn't match");

    header* pPrevHdr = _M_pFirstFreeBlock;

    for (header* pCurHdr = pPrevHdr; pCurHdr; pPrevHdr = pCurHdr, pCurHdr = pCurHdr->next)
    {
        if (pCurHdr == pHeader)
        {
            pPrevHdr->next = pHeader->next;
            bFound = true;
        }
    }

    if (bFound) pPrevHdr->next = pHeader;
    else throw std::logic_error("pointer NOT found!");
}

void list_resource::clear () noexcept
{
    _M_pFirstFreeBlock->size = distance (_M_pEnd, _M_pBegin);
    _M_pFirstFreeBlock->next = nullptr;
}

list_resource::size_type list_resource::max_size () const noexcept
{
    size_type uMaxSize = 0;

    for (header* pCurHeader = _M_pFirstFreeBlock; pCurHeader; pCurHeader = pCurHeader->next)
        if (uMaxSize < pCurHeader->size) uMaxSize = pCurHeader->size;
    return uMaxSize;
}

} } // namespace Memory
