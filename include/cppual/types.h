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

#ifndef CPPUAL_TYPES_H_
#define CPPUAL_TYPES_H_
#ifdef __cplusplus

#include <cstddef>
#include <cstdint>

namespace cppual {

// =========================================================

/// int typedefs
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

// =========================================================

template <std::size_t Bits = 64>
inline constexpr const std::size_t arch_bits_v     = Bits / 8        ;
inline constexpr const std::size_t arch_16_bits_v  = arch_bits_v<16> ;
inline constexpr const std::size_t arch_32_bits_v  = arch_bits_v<32> ;
inline constexpr const std::size_t arch_64_bits_v  = arch_bits_v<64> ;
inline constexpr const std::size_t arch_128_bits_v = arch_bits_v<128>;

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_TYPES_H_
