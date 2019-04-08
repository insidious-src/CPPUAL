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

#ifndef CPPUAL_TYPES_H_
#define CPPUAL_TYPES_H_
#ifdef __cplusplus

#include <cppual/decl.h>

#include <cstddef>
#include <cstdint>

namespace cppual
{

// int typedefs
typedef std::int8_t int8;
typedef std::int_fast8_t fint8;
typedef const std::int8_t cint8;
typedef const std::int_fast8_t cfint8;
typedef std::uint8_t u8;
typedef std::uint_fast8_t fu8;
typedef const std::uint8_t cu8;
typedef const std::uint_fast8_t cfu8;

typedef std::int16_t int16;
typedef std::int_fast16_t fint16;
typedef const std::int16_t cint16;
typedef const std::int_fast16_t cfint16;
typedef std::uint16_t u16;
typedef std::uint_fast16_t fu16;
typedef const std::uint16_t cu16;
typedef const std::uint_fast16_t cfu16;

typedef std::int32_t int32;
typedef std::int_fast32_t fint32;
typedef const std::int32_t cint32;
typedef const std::int_fast32_t cfint32;
typedef std::uint32_t u32;
typedef std::uint_fast32_t fu32;
typedef const std::uint32_t cu32;
typedef const std::uint_fast32_t cfu32;

typedef std::int64_t int64;
typedef std::int_fast64_t fint64;
typedef const std::int64_t cint64;
typedef const std::int_fast64_t cfint64;
typedef std::uint64_t u64;
typedef std::uint_fast64_t fu64;
typedef const std::uint64_t cu64;
typedef const std::uint_fast64_t cfu64;

typedef std::uintptr_t uptr;
typedef const std::uintptr_t cuptr;
typedef std::ptrdiff_t ptrdiff;
typedef const std::ptrdiff_t cptrdiff;

template <std::size_t N>
class bigsigned
{

};

template <std::size_t N>
class bigunsigned
{

};

typedef bigsigned  <128> int128;
typedef bigunsigned<128> u128  ;

template<class Out, class In>
union cast_union
{
    Out out;
    In  in ;

    cast_union () = delete;
    constexpr cast_union (In in_t) noexcept : in (in_t) { }
};

template<class Out, class In>
constexpr Out direct_cast (In in) noexcept
{
    static_assert (sizeof (In) == sizeof (Out), "cannot use direct_cast");
    return cast_union<Out, In>(in).out;
}

template<class Out, class In>
inline Out unsafe_direct_cast (In in) noexcept
{ return cast_union<Out, In>(in).out; }

} // cppual

#endif // __cplusplus
#endif // CPPUAL_TYPES_H_
