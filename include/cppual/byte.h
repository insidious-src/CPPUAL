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

#ifndef CPPUAL_BYTE_H_
#define CPPUAL_BYTE_H_
#ifdef __cplusplus

#include <cppual/types.h>

#include <cstddef>
#include <type_traits>

namespace cppual {

// =========================================================

template <typename T  ,
          typename Out,
          typename = typename std::enable_if<std::is_same<Out, char>::value ||
                                             std::is_same<Out, byte>::value>::type
          >
inline Out* binary_to_bytes (T value, Out* str, bool littleEndian = false) noexcept
{
    auto p = reinterpret_cast<byte*> (str);

    std::ptrdiff_t i;

    if(!littleEndian)
    {
        for (i = static_cast<std::ptrdiff_t> (sizeof (T) - 1); i >= 0; --i)
        {
            *p++ = *(reinterpret_cast<byte*> (&value) + i);
        }
    }
    else
    {
        for (i = 0; i < static_cast<std::ptrdiff_t> (sizeof (T)); ++i)
        {
            *p++ = *(reinterpret_cast<byte*> (&value) + i);
        }
    }

    return str;
}

// =========================================================

template <typename T ,
          typename In,
          typename = typename std::enable_if<std::is_same<In, char >::value ||
                                             std::is_same<In, cchar>::value ||
                                             std::is_same<In, byte >::value ||
                                             std::is_same<In, cbyte>::value>::type
          >
inline T bytes_to_binary (In* str, bool littleEndian = false)
{
    static_assert (!std::is_void<T>::value, "T cannot be void!");

    typedef typename std::conditional<std::is_const<In>::value, cbyte, byte>::type byte_type;

    std::ptrdiff_t i;
    T              x;

    if (!littleEndian)
    {
        for (i = (static_cast<std::ptrdiff_t> (sizeof (T)) - 1); i >= 0; --i)
        {
            *(reinterpret_cast<byte*> (&x) + i) = static_cast<byte_type> (*str++);
        }
    }
    else
    {
        for (i = 0; i < static_cast<std::ptrdiff_t> (sizeof (T)); ++i)
        {
            *(reinterpret_cast<byte*> (&x) + i) = static_cast<byte_type> (*str++);
        }
    }

    return x;
}

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_BYTE_H_


