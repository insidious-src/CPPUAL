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

#ifndef CPPUAL_BYTE_H_
#define CPPUAL_BYTE_H_
#ifdef __cplusplus

#include <cppual/concept/concepts.h>
#include <cppual/types.h>
#include <cppual/casts>

#include <type_traits>
#include <cstddef>

// =========================================================

namespace cppual {

// =========================================================

template <non_void T, typename Out, typename = std::enable_if_t<is_stream_char_v<Out>>>
constexpr Out* binary_to_bytes (T value, Out* str, bool littleEndian = false) noexcept
{
    auto p = direct_cast<uchar*> (str);

    ptrdiff i;

    if(!littleEndian)
    {
        for (i = static_cast<std::ptrdiff_t> (sizeof (T) - 1); i >= 0; --i)
        {
            *p++ = *(direct_cast<uchar*> (&value) + i);
        }
    }
    else
    {
        for (i = 0; i < static_cast<std::ptrdiff_t> (sizeof (T)); ++i)
        {
            *p++ = *(direct_cast<uchar*> (&value) + i);
        }
    }

    return str;
}

// =========================================================

template <non_void T, typename In, typename = std::enable_if_t<is_stream_char_v<In>>>
constexpr T bytes_to_binary (In* str, bool littleEndian = false)
{
    typedef std::conditional_t<std::is_const_v<In>, cuchar, uchar> byte_type;

    ptrdiff i;
    T       x;

    if (!littleEndian)
    {
        for (i = (static_cast<std::ptrdiff_t> (sizeof (T)) - 1); i >= 0; --i)
        {
            *(direct_cast<uchar*> (&x) + i) = static_cast<byte_type> (*str++);
        }
    }
    else
    {
        for (i = 0; i < static_cast<std::ptrdiff_t> (sizeof (T)); ++i)
        {
            *(direct_cast<uchar*> (&x) + i) = static_cast<byte_type> (*str++);
        }
    }

    return x;
}

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_BYTE_H_


