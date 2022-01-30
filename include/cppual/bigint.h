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

#ifndef CPPUAL_BIGINT_H_
#define CPPUAL_BIGINT_H_
#ifdef __cplusplus

#include <cppual/meta.h>
#include <cppual/types.h>

#include <type_traits>

namespace cppual {

// =========================================================

template <std::size_t N>
class bigint;

template <std::size_t N>
class biguint;

template <typename>
struct int_sequence;

// =========================================================

typedef bigint <128> i128;
typedef biguint<128> u128;

// =========================================================

template <>
struct int_sequence<int>
{
    i32 last  ;
    u32 third ;
    u32 second;
    u32 first ;
};

template <>
struct int_sequence<uint>
{
    u32 last  ;
    u32 third ;
    u32 second;
    u32 first ;
};

// =========================================================

template <>
class bigint<128>
{
public:

private:
    int_sequence<int> _M_sequence;
};

// =========================================================

template <>
class biguint<128>
{
public:

private:
    int_sequence<uint> _M_sequence;
};

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_BIGINT_H_

