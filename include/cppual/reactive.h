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

#include <cppual/decl>
#include <cppual/signal>
#include <cppual/concepts>
#include <cppual/meta_functional>
#include <cppual/memory_allocator>

// =========================================================

namespace cppual {

// =========================================================

template <non_void T, allocator_like = memory::allocator<function<void(arg_t<T>)>>>
class reactive;

// =========================================================

typedef reactive<byte>    reactive_byte   ;
typedef reactive<bool>    reactive_bool   ;
typedef reactive<char>    reactive_char   ;
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

template <non_void T, allocator_like A>
class reactive : private signal<void(arg_t<T>), A>
{
public:
    typedef reactive<T, A>               self_type      ;
    typedef arg_t<T>                     arg_type       ;
    typedef signal<void(arg_type), A>    base_type      ;
    typedef memory::allocator_traits<A>  alloc_traits   ;
    typedef alloc_traits::allocator_type allocator_type ;
    typedef remove_cvref_t<T>            value_type     ;
    typedef value_type const             const_value    ;
    typedef value_type &                 reference      ;
    typedef value_type const&            const_reference;
    typedef value_type *                 pointer        ;
    typedef value_type const*            const_pointer  ;
    typedef base_type::value_type        fn_type        ;
    typedef base_type::static_fn_ref     static_fn_ref  ;

    static_assert (std::equality_comparable<value_type>, "value_type is NOT equality comparable!");
    static_assert (is_copyable_movable_v<value_type>, "value_type is NOT neighter copyable nor movable!");

    template <class_t C>
    using mem_fn_type = fn_ptr_t<C, void(arg_type)>;

    template <class_t C>
    using const_mem_fn_type = fn_ptr_t<C, void(arg_type) const>;

    constexpr reactive () noexcept   = default;
    constexpr reactive (self_type&&) = default;

    constexpr reactive (self_type const& rh) noexcept
    : base_type ( rh         )
    , _M_value  ( rh._M_value)
    , _M_pNext  (&rh         )
    { }

    constexpr reactive (value_type&& value, allocator_type const& ator = allocator_type ())
    : base_type (ator             )
    , _M_value  (std::move (value))
    , _M_pNext  (                 )
    { }

    constexpr reactive (const_reference value, allocator_type const& ator = allocator_type ())
    : base_type (ator )
    , _M_value  (value)
    , _M_pNext  (     )
    { }

    constexpr self_type& operator = (self_type const& rh)
    {
        if (this == &rh) return *this;

        self_type* reactive_signal = this;

        base_type::operator = (rh);

        _M_value =  rh._M_value;
        _M_pNext = &rh;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            if (next != _M_pNext) next->_M_value = _M_value;
            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        (*reactive_signal)(get ());

        return *this;
    }

    constexpr self_type& operator = (self_type&& rh)
    {
        if (this == &rh) return *this;

        base_type::operator = (std::move (rh));

        _M_pNext = rh._M_pNext;

        (*this)(_M_value = std::move (rh._M_value));

        return *this;
    }

    constexpr self_type& operator = (value_type&& value)
    {
        if (_M_value == value) return *this;

        self_type* reactive_signal = this;

        _M_value = std::move (value);

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            next->_M_value = _M_value;

            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        (*reactive_signal)(get ());

        return *this;
    }

    constexpr self_type& operator = (const_reference value)
    {
        if (_M_value == value) return *this;

        self_type* reactive_signal = this;

        _M_value = value;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            next->_M_value = _M_value;

            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        (*reactive_signal)(get ());

        return *this;
    }

    constexpr explicit operator const_reference () const noexcept
    { return _M_value; }

    constexpr const_reference get () const noexcept
    { return _M_value; }

    constexpr bool empty () const noexcept
    { return base_type::empty (); }

    constexpr void clear () noexcept
    { base_type::clear (); }

    /// reactive (signal/slot) connect
    constexpr self_type& operator << (fn_type&& fn) const
    {
        self_type* reactive_signal = this;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        *reactive_signal << std::move (fn);

        return *this;
    }

    /// reactive (signal/slot) connect
    constexpr self_type& operator << (fn_type const& fn) const
    {
        self_type* reactive_signal = this;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        *reactive_signal << fn;

        return *this;
    }

    /// reactive (signal/slot) connect
    template <callable C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator << (std::pair<C&, mem_fn_type<C>> pair) const
    {
        self_type* reactive_signal = this;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        connect (*this, pair.first, pair.second);

        return *this;
    }

    /// reactive (signal/slot) connect
    template <callable C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator << (std::pair<C&, const_mem_fn_type<C>> pair) const
    {
        self_type* reactive_signal = this;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        connect (*this, pair.first, pair.second);

        return *this;
    }

    /// reactive (signal/slot) connect
    constexpr self_type& operator << (static_fn_ref fn) const
    {
        self_type* reactive_signal = this;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        *reactive_signal << fn;

        return *this;
    }

    /// callable object reactive (signal/slot) connect
    template <callable C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator << (C& obj) const
    {
        static_assert (std::is_same_v<void, callable_return<C, arg_type>>,
                       "C::operator () return type is NOT void!");

        self_type* reactive_signal = this;

        for (self_type* next = _M_pNext; next != this; next = next->_M_pNext)
        {
            if (reactive_signal == this && !next->empty ()) reactive_signal = next;
        }

        *reactive_signal << obj;

        return *this;
    }

    template <non_void, allocator_like>
    friend class reactive;

private:
    value_type _M_value { };
    self_type* _M_pNext { };
};

// =========================================================

} // cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_REACTIVE_H_
