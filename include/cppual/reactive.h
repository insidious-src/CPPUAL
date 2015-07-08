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

#ifndef CPPUAL_REACTIVE_H_
#define CPPUAL_REACTIVE_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/signal.h>

namespace cppual {

template <typename>
class Reactive;

typedef Reactive<char>    reactive_char;
typedef Reactive<uchar>   reactive_uchar;
typedef Reactive<char16>  reactive_char16;
typedef Reactive<char32>  reactive_char32;
typedef Reactive<wchar>   reactive_wchar;
typedef Reactive<bool>    reactive_bool;
typedef Reactive<short>   reactive_short;
typedef Reactive<ushort>  reactive_ushort;
typedef Reactive<int>     reactive_int;
typedef Reactive<uint>    reactive_uint;
typedef Reactive<long>    reactive_long;
typedef Reactive<ulong>   reactive_ulong;
typedef Reactive<long64>  reactive_long64;
typedef Reactive<ulong64> reactive_ulong64;
typedef Reactive<float>   reactive_float;
typedef Reactive<double>  reactive_double;
typedef Reactive<ldouble> reactive_ldouble;

template <typename T>
class Reactive : Signal<void(T const&)>
{
public:
    typedef T        value_type;
    typedef T&       reference;
    typedef T const& const_reference;

    constexpr Reactive () noexcept = default;
    inline Reactive (Reactive&&) = default;
    inline Reactive (Reactive const&) = default;
    inline Reactive& operator = (Reactive&&) = default;

    inline Reactive (value_type&& value) : m_value (std::forward<T> (value)) { }
    inline Reactive (const_reference value) : m_value (value) { }

    inline Reactive& operator = (Reactive const& gObj)
    {
        if (this != &gObj)
        {
            Signal<void(T const&)>::operator = (gObj);
            m_value = gObj.m_value;
            *this (m_value);
        }

        return *this;
    }

    inline Reactive& operator = (value_type&& value)
    {
        m_value = std::forward<T> (value);
        *this (m_value);
        return *this;
    }

    inline Reactive& operator = (const_reference value)
    {
        m_value = value;
        *this (m_value);
        return *this;
    }

    constexpr operator const_reference () const noexcept
    { return m_value; }

private:
    T m_value;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_REACTIVE_H_
