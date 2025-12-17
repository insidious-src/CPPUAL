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

#ifndef CPPUAL_BITFIELD_H_
#define CPPUAL_BITFIELD_H_
#ifdef __cplusplus

#include <cppual/types>

#include <type_traits>

namespace cppual {

template <std::size_t LastBit = 32>
struct minimum_type_helper
{
    typedef std::conditional_t<LastBit ==  0, void,
            std::conditional_t<LastBit <=  8, u8  ,
            std::conditional_t<LastBit <= 16, u16 ,
            std::conditional_t<LastBit <= 32, u32 ,
            std::conditional_t<LastBit <= 64, u64 ,
            void>>>>> type;
};

template <std::size_t LastBit = 32>
using minimum_type_t = minimum_type_helper<LastBit>::type;

// =========================================================

template <std::size_t Index, std::size_t Bits = 1>
class bitfield
{
private:
    enum { Mask = (1U << Bits) - 1U };

    typedef bitfield<Index, Bits>        self_type ;
    typedef minimum_type_t<Index + Bits> value_type;

public:
    template <class T>
    self_type& operator = (T value)
    {
        _M_value = (_M_value & ~(Mask << Index)) | ((value & Mask) << Index);
        return *this;
    }

    operator      value_type () const { return (_M_value >> Index) & Mask      ; }
    explicit   operator bool () const { return  _M_value & (Mask << Index)     ; }
    self_type& operator ++   ()       { return *this = *this + 1               ; }
    value_type operator ++   (int)    { value_type r = *this; ++*this; return r; }
    self_type& operator --   ()       { return *this = *this - 1               ; }
    value_type operator --   (int)    { value_type r = *this; --*this; return r; }

private:
    value_type _M_value;
};

// =========================================================

template <std::size_t Index>
class bitfield <Index, 1>
{
public:
    enum { Bits = 1, Mask = 0x01 };

    typedef bitfield<Index, Bits>        self_type ;
    typedef minimum_type_t<Index + Bits> value_type;

public:
    self_type& operator = (bool value)
    {
        _M_value = (_M_value & ~(Mask << Index)) | (value << Index);
        return *this;
    }

    explicit operator bool () const
    { return _M_value & (Mask << Index); }

private:
    value_type _M_value;
};

// =========================================================

//! function to reverse bits of a number
template <typename T, typename = std::enable_if_t<is_integer_v<T>>>
T reverse_bits (T n)
{
    T rev = 0;

    while (n > 0) // traversing bits of 'n' from the right
    {
        rev <<= 1; // bitwise left shift 'rev' by 1

        if ((n & 1) == 1) rev ^= 1; // if current bit is '1'

        n >>= 1; // bitwise right shift 'n' by 1
    }

    return rev;
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_BITFIELD_H_
