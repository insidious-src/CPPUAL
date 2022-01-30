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

#ifndef CPPUAL_BITFIELD_H_
#define CPPUAL_BITFIELD_H_
#ifdef __cplusplus

#include <cppual/meta.h>
#include <cppual/types.h>

#include <type_traits>

namespace cppual {

template <std::size_t LastBit>
struct minimum_type_helper
{
    typedef
        typename std::conditional<LastBit ==  0, void,
        typename std::conditional<LastBit <=  8, u8  ,
        typename std::conditional<LastBit <= 16, u16 ,
        typename std::conditional<LastBit <= 32, u32 ,
        typename std::conditional<LastBit <= 64, u64 ,
        void>::type>::type>::type>::type>::type  type;
};

// =========================================================

template <std::size_t Index, std::size_t Bits = 1>
class bitfield
{
private:
    enum { Mask = (1u << Bits) - 1u };
    typedef typename minimum_type_helper<Index + Bits>::type value_type;

public:
    template <class U>
    bitfield& operator = (U value)
    {
        _M_value = (_M_value & ~(Mask << Index)) | ((value & Mask) << Index);
        return *this;
    }

    operator      value_type() const { return (_M_value >> Index) & Mask;  }
    explicit   operator bool() const { return  _M_value & (Mask << Index); }
    bitfield&  operator ++  ()       { return *this = *this + 1;          }
    value_type operator ++  (int)    { value_type r = *this; ++*this; return r;    }
    bitfield&  operator --  ()       { return *this = *this - 1;          }
    value_type operator --  (int)    { value_type r = *this; --*this; return r;    }

private:
    value_type _M_value;
};

// =========================================================

template <std::size_t Index>
class bitfield<Index, 1>
{
private:
    enum
    {
        Bits = 1,
        Mask = 0x01
    };

    typedef typename minimum_type_helper<Index + Bits>::type value_type;

public:
    bitfield& operator = (bool value)
    {
        _M_value = (_M_value & ~(Mask << Index)) | (value << Index);
        return *this;
    }

    explicit operator bool() const
    { return _M_value & (Mask << Index); }

private:
    value_type _M_value;
};

// =========================================================

// function to reverse bits of a number
template <typename T,
          typename = typename std::enable_if<is_integer<T>::value>::type>
T reverse_bits(T n)
{
    T rev = 0;

    // traversing bits of 'n' from the right
    while (n > 0)
    {
        // bitwise left shift
        // 'rev' by 1
        rev <<= 1;

        // if current bit is '1'
        if ((n & 1) == 1)
            rev ^= 1;

        // bitwise right shift
        // 'n' by 1
        n >>= 1;
    }

    return rev;
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_BITFIELD_H_
