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

#ifndef CPPUAL_REACTIVE_H_
#define CPPUAL_REACTIVE_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/signal.h>

namespace cppual {

// =========================================================

template <typename T,
          typename A = signal_allocator<void(T const&)>
          >
class reactive;

// =========================================================

typedef reactive<char>    reactive_char   ;
typedef reactive<uchar>   reactive_uchar  ;
typedef reactive<char16>  reactive_char16 ;
typedef reactive<char32>  reactive_char32 ;
typedef reactive<wchar>   reactive_wchar  ;
typedef reactive<bool>    reactive_bool   ;
typedef reactive<short>   reactive_short  ;
typedef reactive<ushort>  reactive_ushort ;
typedef reactive<int>     reactive_int    ;
typedef reactive<uint>    reactive_uint   ;
typedef reactive<long>    reactive_long   ;
typedef reactive<ulong>   reactive_ulong  ;
typedef reactive<long64>  reactive_long64 ;
typedef reactive<ulong64> reactive_ulong64;
typedef reactive<float>   reactive_float  ;
typedef reactive<double>  reactive_double ;
typedef reactive<ldouble> reactive_ldouble;

// =========================================================

template <typename T, typename A>
class reactive : signal<void(T const&), A>
{
public:
    static_assert (!std::is_void<T>::value, "T is void");

    typedef T        value_type     ;
    typedef T&       reference      ;
    typedef T const& const_reference;

    constexpr reactive () noexcept = default;
    inline reactive (reactive&&) = default;
    inline reactive (reactive const&) = default;
    inline reactive& operator = (reactive&&) = default;

    inline reactive (value_type&&    value) : _M_value (std::move (value)) { }
    inline reactive (const_reference value) : _M_value (value)             { }

    inline reactive& operator = (reactive const& gObj)
    {
        if (this != &gObj)
        {
            signal<void(T const&)>::operator = (gObj);
            _M_value = gObj._M_value;
            (*this)(_M_value);
        }

        return *this;
    }

    inline reactive& operator = (value_type&& value)
    {
        _M_value = std::move (value);
        (*this)(_M_value);
        return *this;
    }

    inline reactive& operator = (const_reference value)
    {
        _M_value = value;
        (*this)(_M_value);
        return *this;
    }

    constexpr operator const_reference () const noexcept
    { return _M_value; }

private:
    T _M_value;
};

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_REACTIVE_H_
