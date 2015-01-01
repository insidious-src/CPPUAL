/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef CPPUAL_CIRCULAR_QUEUE_H_
#define CPPUAL_CIRCULAR_QUEUE_H_
#ifdef __cplusplus

#include <cppual/types.h>

namespace cppual {

template <typename> class  simd_vector;
typedef simd_vector<  float>   float_v;
typedef simd_vector< double>  double_v;
typedef simd_vector<ldouble> ldouble_v;
typedef simd_vector< long64>   llong_v;
typedef simd_vector<ulong64>  ullong_v;
typedef simd_vector<   long>    long_v;
typedef simd_vector<  ulong>   ulong_v;
typedef simd_vector<    int>     int_v;
typedef simd_vector<   uint>    uint_v;
typedef simd_vector<  short>   short_v;
typedef simd_vector< ushort>  ushort_v;
typedef simd_vector<   char>    char_v;
typedef simd_vector<  uchar>   uchar_v;

template <typename T, std::size_t N>
class simd_vector
{
public:
	typedef T              value_type;
	typedef std::size_t    size_type;
	typedef simd_vector<T> self_type;

	constexpr simd_vector () noexcept = default;
	simd_vector (simd_vector const&);
	simd_vector& operator = (simd_vector const&);

	constexpr size_type size () const noexcept
	{ sizeof (m_data); }

private:
	value_type m_data[N];
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_CIRCULAR_QUEUE_H_
