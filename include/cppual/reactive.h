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

#ifndef CPPUAL_REACTIVE_H_
#define CPPUAL_REACTIVE_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/signal.h>
#include <cppual/fn_meta.h>
#include <cppual/memory/allocator.h>
#include <cppual/concept/concepts.h>

// =========================================================

namespace cppual {

// =========================================================

using memory::allocator_t;

template <non_void_t T, allocator_t = signal_allocator<void(arg_t<T>)>>
class reactive;

// =========================================================

typedef reactive<byte>    reactive_byte   ;
typedef reactive<bool>    reactive_bool   ;
typedef reactive<char>    reactive_char   ;
typedef reactive<uchar>   reactive_uchar  ;
typedef reactive<char8>   reactive_char8  ;
typedef reactive<char16>  reactive_char16 ;
typedef reactive<char32>  reactive_char32 ;
typedef reactive<wchar>   reactive_wchar  ;
typedef reactive<short>   reactive_short  ;
typedef reactive<i16>     reactive_i16    ;
typedef reactive<ushort>  reactive_ushort ;
typedef reactive<u16>     reactive_u16    ;
typedef reactive<int>     reactive_int    ;
typedef reactive<i32>     reactive_i32    ;
typedef reactive<uint>    reactive_uint   ;
typedef reactive<u32>     reactive_u32    ;
typedef reactive<long>    reactive_long   ;
typedef reactive<i64>     reactive_i64    ;
typedef reactive<ulong>   reactive_ulong  ;
typedef reactive<u64>     reactive_u64    ;
typedef reactive<long64>  reactive_long64 ;
typedef reactive<ulong64> reactive_ulong64;
typedef reactive<float>   reactive_float  ;
typedef reactive<double>  reactive_double ;
typedef reactive<ldouble> reactive_ldouble;

// =========================================================

template <non_void_t T, allocator_t A>
class reactive : private signal<void(arg_t<T>), A>
{
public:
    typedef reactive<T, A>              self_type      ;
    typedef arg_t<T>                    arg_type       ;
    typedef signal<void(arg_type), A>   base_type      ;
    typedef std::allocator_traits<A>    traits_type    ;
    typedef traits_type::allocator_type allocator_type ;
    typedef std::remove_cvref_t<T>      value_type     ;
    typedef value_type const            const_value    ;
    typedef value_type &                reference      ;
    typedef value_type const&           const_reference;
    typedef value_type *                pointer        ;
    typedef value_type const*           const_pointer  ;
    typedef base_type::value_type       fn_type        ;

    constexpr reactive () noexcept        = default;
    inline    reactive (self_type&&)      = default;
    constexpr reactive (self_type const&) = default;

    constexpr reactive (value_type&& value, allocator_type const& ator = allocator_type ())
    : base_type (ator             ),
      _M_value  (std::move (value))
    { }

    constexpr reactive (const_reference value, allocator_type const& ator = allocator_type ())
    : base_type (ator ),
      _M_value  (value)
    { }

    inline self_type& operator = (self_type const& gObj)
    {
        if (this != &gObj) return *this;

        base_type::operator = (gObj);
        (*this)(_M_value = gObj._M_value);

        return *this;
    }

    inline self_type& operator = (self_type&& gObj)
    {
        if (this != &gObj) return *this;

        base_type::operator = (std::move (gObj));
        (*this)(_M_value = std::move (gObj._M_value));

        return *this;
    }

    inline self_type& operator = (value_type&& value)
    {
        if (&_M_value != &value) (*this)(_M_value = std::move (value));
        return *this;
    }

    inline self_type& operator = (const_reference value)
    {
        if (&_M_value != &value) (*this)(_M_value = value);
        return *this;
    }

    constexpr explicit operator const_reference () const noexcept
    { return _M_value; }

    constexpr const_reference get () const noexcept
    { return _M_value; }

    inline void  clear () noexcept
    { base_type::clear (); }

    /// reactive (signal/slot) connect
    inline self_type& operator << (fn_type&& fn) const
    {
        connect (*this, std::move (fn));
        return   *this;
    }

    /// reactive (signal/slot) connect
    inline self_type& operator << (fn_type const& fn) const
    {
        connect (*this, fn);
        return   *this;
    }

    /// reactive (signal/slot) connect
    inline self_type& operator << (void(& fn)(arg_type)) const
    {
        connect (*this, fn);
        return   *this;
    }

    /// callable object reactive (signal/slot) connect
    template <callable_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    inline self_type& operator << (C& obj) const
    {
        static_assert (std::is_same_v<void, callable_return_t<C, arg_type>>,
                       "C::operator () return type is NOT void!");

        connect (*this, obj);
        return   *this;
    }

    template <non_void_t, allocator_t>
    friend class reactive;

private:
    value_type _M_value { };
};

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_REACTIVE_H_
