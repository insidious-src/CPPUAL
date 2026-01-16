/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#ifndef CPPUAL_MEMORY_OPERATIONS_H_
#define CPPUAL_MEMORY_OPERATIONS_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/iterators/bi_iterator.h>

#include <cstddef>
#include <cstring>

namespace cppual { namespace memory {

// =========================================================

template <typename T>
constexpr std::size_t distance (T* const p1, T* const p2) noexcept
{
    return p1 >= p2 ?
                std::size_t (reinterpret_cast<uptr> (p1) - reinterpret_cast<uptr> (p2)) :
                std::size_t (reinterpret_cast<uptr> (p2) - reinterpret_cast<uptr> (p1));
}

template <typename T>
constexpr T* address (T* const p, std::size_t shift) noexcept
{ return reinterpret_cast<T*> (reinterpret_cast<uptr> (p) + shift); }

constexpr std::size_t aligned_size (std::size_t const n,
                                    std::size_t const align = alignof (uptr)) noexcept
{ return (n + (align - 1)) & ~(align - 1); }

constexpr std::size_t align_adjust (cvoid* pAddr, std::size_t const uAlign) noexcept
{
    std::size_t uAdjust  = uAlign - (reinterpret_cast<uptr> (pAddr) & (uAlign - 1));
    return      uAdjust == uAlign ? 0 : uAdjust;
}

constexpr void* next_aligned_addr (cvoid* pAddr, std::size_t const uAlign) noexcept
{
    return reinterpret_cast<void*>
            ((reinterpret_cast<uptr> (pAddr) + (uAlign - 1)) & ~(uAlign - 1));
}

constexpr std::size_t align_adjustment_header (cvoid*      addr,
                                            std::size_t align,
                                            std::size_t header_size) noexcept
{
    std::size_t uAdjust = align - (reinterpret_cast<uptr> (addr) & (align - 1));

    // already aligned
    if (uAdjust == align) uAdjust = 0;

    if (uAdjust < header_size)
    {
        header_size -= uAdjust;

        // increase adjustment to fit header
        uAdjust += align * (header_size / align);
        if (header_size % align > 0) uAdjust += align;
    }

    return uAdjust;
}

// =========================================================

} } // namespace Memory

#endif // __cplusplus
#endif // CPPUAL_MEMORY_OPERATIONS_H_
