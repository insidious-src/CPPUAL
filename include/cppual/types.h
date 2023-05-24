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

#ifndef CPPUAL_TYPES_H_
#define CPPUAL_TYPES_H_
#ifdef __cplusplus

#include <cppual/decl.h>

#include <cstddef>
#include <cstdint>

namespace cppual {

// int typedefs
typedef std::int8_t i8;
typedef std::int_fast8_t fi8;
typedef const std::int8_t ci8;
typedef const std::int_fast8_t cfi8;
typedef std::uint8_t u8;
typedef std::uint_fast8_t fu8;
typedef const std::uint8_t cu8;
typedef const std::uint_fast8_t cfu8;

typedef std::int16_t i16;
typedef std::int_fast16_t fi16;
typedef const std::int16_t ci16;
typedef const std::int_fast16_t cfi16;
typedef std::uint16_t u16;
typedef std::uint_fast16_t fu16;
typedef const std::uint16_t cu16;
typedef const std::uint_fast16_t cfu16;

typedef std::int32_t i32;
typedef std::int_fast32_t fi32;
typedef const std::int32_t ci32;
typedef const std::int_fast32_t cfi32;
typedef std::uint32_t u32;
typedef std::uint_fast32_t fu32;
typedef const std::uint32_t cu32;
typedef const std::uint_fast32_t cfu32;

typedef std::int64_t i64;
typedef std::int_fast64_t fi64;
typedef const std::int64_t ci64;
typedef const std::int_fast64_t cfi64;
typedef std::uint64_t u64;
typedef std::uint_fast64_t fu64;
typedef const std::uint64_t cu64;
typedef const std::uint_fast64_t cfu64;

typedef std::intptr_t intptr;
typedef const std::intptr_t cintptr;
typedef std::uintptr_t uptr;
typedef const std::uintptr_t cuptr;
typedef std::ptrdiff_t ptrdiff;
typedef const std::ptrdiff_t cptrdiff;

template <class Out, class In>
union cast_union
{
    Out out;
    In  in ;

    cast_union () = delete;
    constexpr cast_union (In val) noexcept : in (val) { }
};

template <class Out, class In>
constexpr Out direct_cast (In val) noexcept
{
    static_assert (sizeof (In) == sizeof (Out), "The sizes of In & Out are not equal!");
    return cast_union<Out, In> (val).out;
}

template <class Out, class In>
constexpr Out unsafe_direct_cast (In val) noexcept
{
    return cast_union<Out, In> (val).out;
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_TYPES_H_
