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

#include <cppual/memory/pool.h>

#include <cassert>
#include <iostream>

namespace cppual { namespace memory {

uniform_pool_resource::uniform_pool_resource (size_type  uBlkCount,
                                              size_type  uBlkSize,
                                              align_type uBlkAlign)
: _M_gOwner (uBlkCount && uBlkSize ?
                 get_default_resource()->max_size() >= (uBlkCount * uBlkSize + max_align * 2) ?
                *get_default_resource() : *new_delete_resource() : *this),
  _M_pBegin (uBlkCount && uBlkSize ?
                    _M_gOwner.allocate (uBlkCount * uBlkSize + max_align * 2, uBlkAlign) : nullptr),
  _M_pEnd (_M_pBegin != nullptr ?
            static_cast<math_pointer> (_M_pBegin) + (uBlkCount * uBlkSize + max_align * 2) : nullptr),
  _M_pFreeList (),
  _M_uBlkSize  (uBlkSize),
  _M_uBlkAlign (uBlkAlign),
  _M_uBlkNum   (uBlkCount),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    initialize ();
}

uniform_pool_resource::uniform_pool_resource (pointer    buffer,
                                              size_type  uBlkCount,
                                              size_type  uBlkSize,
                                              align_type uBlkAlign)
: _M_gOwner (*this),
  _M_pBegin (buffer && uBlkCount && uBlkSize ? buffer : nullptr),
  _M_pEnd (_M_pBegin ? static_cast<math_pointer> (_M_pBegin) + (uBlkCount * uBlkSize) : nullptr),
  _M_pFreeList (),
  _M_uBlkSize  (uBlkSize),
  _M_uBlkAlign (uBlkAlign),
  _M_uBlkNum (uBlkCount),
  _M_bIsMemShared ()
{
    initialize ();
}

uniform_pool_resource::uniform_pool_resource (memory_resource& pOwner,
                                              size_type        uBlkCount,
                                              size_type        uBlkSize,
                                              align_type       uBlkAlign)
: _M_gOwner (uBlkCount && uBlkSize ?
                 (uBlkCount * uBlkSize + max_align * 2) > pOwner.max_size () ?
                     (uBlkCount * uBlkSize + max_align * 2) > get_default_resource()->max_size () ?
                         *new_delete_resource () : *get_default_resource () : pOwner : *this),
  _M_pBegin (uBlkCount && uBlkSize ?
                 _M_gOwner.allocate (uBlkCount * uBlkSize + max_align * 2, uBlkAlign) : nullptr),
  _M_pEnd (_M_pBegin != nullptr ?
            static_cast<math_pointer> (_M_pBegin) + (uBlkCount * uBlkSize + max_align * 2) : nullptr),
  _M_pFreeList (),
  _M_uBlkSize  (uBlkSize),
  _M_uBlkAlign (uBlkAlign),
  _M_uBlkNum   (uBlkCount),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (&_M_gOwner != &pOwner)
    {
        std::cerr << __func__
                  << " :: Specified owner cannot be assigned -> 'max_size' exceeded. "
                     "Using default memory resource instead." << std::endl;
    }

    initialize ();
}

uniform_pool_resource::~uniform_pool_resource ()
{
    if (_M_pBegin && &_M_gOwner != this) _M_gOwner.deallocate (_M_pBegin, capacity (), _M_uBlkAlign);
}

void uniform_pool_resource::initialize () noexcept
{
    if (_M_pBegin)
    {
        auto uAdjust = align_adjust (_M_pBegin, _M_uBlkAlign);

        auto p = _M_pFreeList =
                 reinterpret_cast<pointer*> (static_cast<math_pointer> (_M_pBegin) + uAdjust);

        _M_uBlkNum = ((_M_uBlkSize - uAdjust) / _M_uBlkSize) - 1;

        for (size_type i = 0; i < _M_uBlkNum; ++i)
        {
            *p = reinterpret_cast<pointer>  ( p + _M_uBlkSize);
             p = reinterpret_cast<pointer*> (*p);
        }

        *p = nullptr;
        ++_M_uBlkNum;
    }
}

void* uniform_pool_resource::do_allocate (size_type uSize, align_type uAlign)
{
    // check if size and alignment match with the ones from
    // the current pool instance
    if (!_M_pFreeList || uSize != _M_uBlkSize || uAlign != _M_uBlkAlign)
        return nullptr;

    auto p       = reinterpret_cast<pointer > ( _M_pFreeList);
    _M_pFreeList = reinterpret_cast<pointer*> (*_M_pFreeList);

    return p;
}

void uniform_pool_resource::do_deallocate (void* p, size_type uSize, align_type)
{
    if (uSize != _M_uBlkSize) throw std::length_error ("not equal to blocksize");
    if (_M_pEnd <= p || p < _M_pBegin) throw std::out_of_range ("pointer is outside the buffer");

    *(reinterpret_cast<pointer*> (p)) = reinterpret_cast<pointer> (_M_pFreeList);
    _M_pFreeList = reinterpret_cast<pointer*> (p);
}

void uniform_pool_resource::clear () noexcept
{
    _M_pFreeList = reinterpret_cast<pointer*> (static_cast<math_pointer> (_M_pBegin) +
                                               align_adjust (_M_pBegin, _M_uBlkAlign));
}

} } // namespace Memory
