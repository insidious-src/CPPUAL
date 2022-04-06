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

#include <cppual/memory/stacked.h>
#include <cppual/memory/mop.h>

#include <iostream>
#include <new>

namespace cppual { namespace memory {

// =========================================================

namespace {

constexpr memory_resource::difference_type to_diff (byte val) noexcept
{
    return static_cast<memory_resource::difference_type> (val);
}

constexpr byte to_byte (memory_resource::difference_type val) noexcept
{
    return static_cast<byte> (val);
}

constexpr memory_resource::math_pointer to_math_ptr (memory_resource::pointer p) noexcept
{
    return static_cast<memory_resource::math_pointer> (p);
}

} // anonymous namespace

// =========================================================

stacked_resource::stacked_resource (size_type uSize)
: _M_gOwner (uSize ? get_default_resource()->max_size() >= (uSize + max_adjust) ?
                    *get_default_resource() : *new_delete_resource() : *this),
  _M_pMarker (&_M_gOwner != this ? _M_gOwner.allocate (uSize + max_adjust, alignof (uptr)) : nullptr),
  _M_pBegin (_M_pMarker),
  _M_pEnd (_M_pBegin != nullptr ? static_cast<math_pointer> (_M_pBegin) + uSize + max_adjust : nullptr),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (!_M_pBegin) throw std::bad_alloc ();
}

stacked_resource::stacked_resource (pointer buffer, size_type uSize)
: _M_gOwner (*this),
  _M_pMarker (buffer && uSize ? buffer : nullptr),
  _M_pBegin (_M_pMarker),
  _M_pEnd (_M_pBegin != nullptr ? static_cast<math_pointer> (_M_pBegin) + uSize : nullptr),
  _M_bIsMemShared ()
{
    if (!_M_pBegin) throw std::bad_alloc ();
}

stacked_resource::stacked_resource (memory_resource& pOwner, size_type uSize)
: _M_gOwner (uSize ? (uSize + max_adjust) > pOwner.max_size () ?
                         (uSize + max_adjust) > get_default_resource()->max_size() ?
                            *new_delete_resource() : *get_default_resource() : pOwner : *this),
  _M_pMarker (&_M_gOwner != this ? _M_gOwner.allocate (uSize + max_adjust, alignof (uptr)) : nullptr),
  _M_pBegin (_M_pMarker),
  _M_pEnd (_M_pBegin != nullptr ? static_cast<math_pointer> (_M_pBegin) + uSize + max_adjust : nullptr),

  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (!_M_pBegin) throw std::bad_alloc ();

    if (&_M_gOwner != &pOwner)
    {
        std::cerr << __func__
                  << " :: Specified owner cannot be assigned -> 'max_size' exceeded. "
                     "Using default memory resource instead." << std::endl;
    }
}

stacked_resource::~stacked_resource ()
{
    if (_M_pBegin && &_M_gOwner != this) _M_gOwner.deallocate (_M_pBegin, capacity (), alignof (uptr));
}

void* stacked_resource::do_allocate (size_type uBytes, align_type uAlign)
{
    if (!_M_pBegin or !uBytes) throw std::bad_alloc ();
    if (!uAlign || uAlign > max_align) uAlign = alignof (uptr);

    auto max_bytes = max_size ();
    auto pMarker   = _M_pMarker;

    if (!std::align (uAlign, uBytes, pMarker, max_bytes)) throw std::bad_alloc ();

    auto const pNewMarker = to_math_ptr (pMarker) + uBytes;

    if (pNewMarker > _M_pEnd) throw std::bad_alloc ();

    auto const align_shift = to_byte (to_math_ptr (pMarker) - to_math_ptr (_M_pMarker));

    if (align_shift > 0) *to_math_ptr (_M_pMarker) = align_shift;

    _M_pMarker = pNewMarker;
    return       pMarker   ;
}

void* stacked_resource::do_reallocate(void* p, size_type old_size, size_type size, align_type align_size)
{
    if (p)
    {
        if (old_size && old_size == size) return p;
    }
    else
    {
        p = _M_pMarker;
        old_size = 0;
    }

    if (!align_size || align_size > max_align) align_size = alignof(uptr);

    auto new_p   = p;
    auto pMarker = to_math_ptr (new_p) + old_size;

    if (pMarker == _M_pMarker || (pMarker + *pMarker) == _M_pMarker)
    {
        auto max = max_size () + old_size;

        if (std::align (align_size, size, new_p, max)) throw std::bad_alloc ();

        auto const pNewMarker = static_cast<math_pointer> (new_p) + size;

        if (pNewMarker > _M_pEnd) throw std::bad_alloc ();

        auto const align_shift = to_byte (to_math_ptr (new_p) - to_math_ptr (p));

        if (old_size) std::memmove (new_p, p, old_size <= size ? old_size : size);

        if (align_shift > 0)
        {
            *to_math_ptr (p) = (pMarker + to_diff (*pMarker)) == _M_pMarker ?
                               *pMarker + align_shift : align_shift;
        }

        _M_pMarker = pNewMarker;
    }
    else
    {
        throw std::out_of_range ("pointer doesn't match the last element allocated!");
    }

    return new_p;
}

void stacked_resource::do_deallocate (void* p, size_type uSize, align_type)
{
    auto const pMarker = to_math_ptr (p) + uSize;

    if (pMarker == _M_pMarker || (pMarker + to_diff (*pMarker)) == _M_pMarker)
    {
        _M_pMarker = p;
    }
    else
    {
        std::cerr << "pointer diff: " << to_math_ptr (_M_pMarker) - pMarker
                  << std::endl;

        throw std::out_of_range ("pointer doesn't match the last element allocated!");
    }
}

// =========================================================

dstacked_resource::dstacked_resource (size_type uSize, size_type uHint)
: _M_gOwner (uSize ? get_default_resource()->max_size() >= (uSize + max_adjust) ?
                *get_default_resource() : *new_delete_resource() : *this),
  _M_pTopMarker (&_M_gOwner != this ? _M_gOwner.allocate (uSize + max_adjust, alignof (uptr)) : nullptr),
  _M_pBottomMarker (_M_pTopMarker != nullptr ?
            static_cast<math_pointer> (_M_pTopMarker) + uSize + max_adjust : nullptr),
  _M_pBegin (_M_pTopMarker),
  _M_uHint (uHint),
  _M_pEnd (_M_pBottomMarker),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (!_M_pBegin) throw std::bad_alloc ();
}

dstacked_resource::dstacked_resource (pointer buffer, size_type uSize, size_type uHint)
: _M_gOwner (*this),
  _M_pTopMarker (buffer && uSize ? buffer : nullptr),
  _M_pBottomMarker (_M_pTopMarker != nullptr ? static_cast<math_pointer> (_M_pTopMarker) + uSize : nullptr),
  _M_pBegin (_M_pTopMarker),
  _M_uHint (uHint),
  _M_pEnd (_M_pBottomMarker),
  _M_bIsMemShared ()
{
    if (!_M_pBegin) throw std::bad_alloc ();
}

dstacked_resource::dstacked_resource (memory_resource& pOwner, size_type uSize, size_type uHint)
: _M_gOwner (uSize ? uSize > pOwner.max_size () ?
                         (uSize + max_adjust) > get_default_resource()->max_size() ?
                                         *new_delete_resource() : *get_default_resource() : pOwner : *this),
  _M_pTopMarker (&_M_gOwner != this ? _M_gOwner.allocate (uSize + max_adjust, alignof (uptr)) : nullptr),
  _M_pBottomMarker (_M_pTopMarker != nullptr ?
            static_cast<math_pointer> (_M_pTopMarker) + uSize + max_adjust : nullptr),
  _M_pBegin (_M_pTopMarker),
  _M_uHint (uHint),
  _M_pEnd (_M_pBottomMarker),
  _M_bIsMemShared (&_M_gOwner != this ? _M_gOwner.is_shared () : false)
{
    if (!_M_pBegin) throw std::bad_alloc ();

    if (&_M_gOwner != &pOwner)
    {
        std::cerr << __func__
                  << " :: Specified owner cannot be assigned -> 'max_size' exceeded. "
                     "Using default memory resource instead." << std::endl;
    }
}

dstacked_resource::~dstacked_resource ()
{
    if (_M_pBegin && &_M_gOwner != this) _M_gOwner.deallocate (_M_pBegin, capacity (), alignof (uptr));
}

void* dstacked_resource::do_allocate (size_type uBytes, align_type uAlign)
{
    constexpr static auto align_uptr = static_cast<difference_type> (alignof (uptr));

    // check whether there is enough space to allocate
    if (!_M_pBegin or !uBytes) throw std::bad_alloc ();
    if (!uAlign || uAlign > max_align) uAlign = alignof (uptr);

    auto max = max_size ();

    if (uBytes < _M_uHint)
    {
        auto pMarker = _M_pTopMarker;

        if (!std::align (uAlign, uBytes, pMarker, max)) throw std::bad_alloc ();

        auto const pNewMarker = to_math_ptr (pMarker) + uBytes;

        if (pNewMarker > _M_pBottomMarker) throw std::bad_alloc ();

        auto const align_shift = to_byte (to_math_ptr (pMarker) - to_math_ptr (_M_pTopMarker));

        if (align_shift > 0) *to_math_ptr (_M_pTopMarker) = align_shift;

        _M_pTopMarker = pNewMarker;
        return          pMarker   ;
    }

    pointer    pNewBottomMarker          = static_cast<math_pointer> (_M_pBottomMarker) - uBytes;
    auto const pNewUnalignedBottomMarker = static_cast<math_pointer> (pNewBottomMarker);

    if (!std::align (uAlign, uBytes, pNewBottomMarker, max)) throw std::bad_alloc ();

    auto const align_shift = ((pNewUnalignedBottomMarker - to_math_ptr (pNewBottomMarker)) +
                               align_uptr) % align_uptr;

    pNewBottomMarker = pNewUnalignedBottomMarker - align_shift;

    if (pNewBottomMarker < _M_pTopMarker) throw std::bad_alloc();

    if (align_shift > 0) *(to_math_ptr (pNewBottomMarker) + uBytes) = to_byte (align_shift);

    return _M_pBottomMarker = pNewBottomMarker;
}

void* dstacked_resource::do_reallocate(void* p, size_type old_size, size_type size, align_type align_size)
{
    constexpr static auto align_uptr = static_cast<difference_type> (alignof (uptr));

    auto const old_p = p;

    if (p)
    {
        if (old_size && old_size == size) return p;
    }
    else
    {
        p = old_size < _M_uHint ? _M_pTopMarker : _M_pBottomMarker;
        old_size = 0;
    }

    if (!align_size || align_size > max_align) align_size = alignof(uptr);

    auto new_p   = p;
    auto pMarker = to_math_ptr (new_p) + old_size;

    if (pMarker == _M_pTopMarker                       ||
       (pMarker + to_diff (*pMarker)) == _M_pTopMarker ||
        p == _M_pBottomMarker                          ||
       (to_math_ptr (_M_pBottomMarker) + to_diff (*to_math_ptr (_M_pBottomMarker))) == p)
    {
        auto max = max_size () + old_size;

        byte align_shift = 0;

        if (old_size < _M_uHint && size >= _M_uHint)
        {
            _M_pTopMarker = p;
            p = new_p = (to_math_ptr (_M_pBottomMarker) - size);
        }
        else if (old_size >= _M_uHint && size < _M_uHint)
        {
            _M_pBottomMarker = pMarker;
            p = new_p = _M_pTopMarker;
        }
        else if (old_size < _M_uHint && size < _M_uHint)
        {
            align_shift = (pMarker + to_diff (*pMarker)) == _M_pTopMarker ? *pMarker : 0;
        }
        else if (old_size >= _M_uHint && size >= _M_uHint)
        {
            align_shift = (to_math_ptr (_M_pBottomMarker) +
                           to_diff (*to_math_ptr (_M_pBottomMarker))) == p ? *pMarker : 0;
        }

        if (std::align (align_size, size, new_p, max)) throw std::bad_alloc ();

        if (size < _M_uHint)
        {
            auto const pNewMarker = static_cast<math_pointer> (new_p) + size;

            if (pNewMarker > _M_pBottomMarker) throw std::bad_alloc ();

            align_shift += to_byte (to_math_ptr (new_p) - to_math_ptr (p));

            if (old_size) std::memmove (new_p, old_p, old_size <= size ? old_size : size);

            if (align_shift > 0) *to_math_ptr(p) = align_shift;

            _M_pTopMarker = pNewMarker;
        }
        else
        {
            auto const new_align_shift = to_byte (((to_math_ptr (p) - to_math_ptr(new_p)) +
                                                    align_uptr) % align_uptr);

            align_shift += new_align_shift;

            auto const pNewBottomMarker = to_math_ptr (p) - new_align_shift;

            if (pNewBottomMarker < _M_pTopMarker) throw std::bad_alloc();

            if (old_size) std::memmove (new_p, old_p, old_size <= size ? old_size : size);

            if (align_shift > 0) *(to_math_ptr (pNewBottomMarker) + size) = align_shift;

            _M_pBottomMarker = pNewBottomMarker;
        }
    }
    else
    {
        throw std::out_of_range ("pointer doesn't match the last element in both markers");
    }

    return new_p;
}

void dstacked_resource::do_deallocate (void* p, size_type uBytes, align_type)
{
    auto const pMarker = to_math_ptr (p) + uBytes;

    // if the pointer belongs to the top side
    if (pMarker == _M_pTopMarker || (pMarker + to_diff (*pMarker)) == _M_pTopMarker)
    {
        _M_pTopMarker = p;
    }
    // if the pointer belongs to the bottom side
    else if (p == _M_pBottomMarker ||
            (to_math_ptr (_M_pBottomMarker) + to_diff (*to_math_ptr (_M_pBottomMarker))) == p)
    {
        _M_pBottomMarker = pMarker;
    }
    else
    {
        throw std::out_of_range ("pointer doesn't match the last element in both markers");
    }
}

} } // namespace Memory
