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

#ifndef CPPUAL_BITSET_H_
#define CPPUAL_BITSET_H_
#ifdef __cplusplus


#include <cppual/iterators/bitset_iterator.h>
#include <cppual/concept/concepts.h>
#include <cppual/containers.h>

#include <string_view>
#include <type_traits>
#include <cstddef>
#include <cassert>
#include <array>
#include <bit>

// =========================================================

namespace cppual {

// =========================================================

template <enum_t> class bitset;

// =========================================================

//! bitset reference proxy
template <enum_t T>
class reference_proxy
{
public:
    typedef reference_proxy           self_type  ;
    typedef bitset<T>                 bitset_type;
    typedef std::underlying_type_t<T> value_type ;

    constexpr reference_proxy (bitset_type& b, value_type const m)
    : bs (&b), mask (m)
    { }

    constexpr self_type& operator = (bool const x) noexcept
    { (x) ? bs->_M_flags |= mask : bs->_M_flags &= ~mask; return *this; }

    constexpr operator bool () const noexcept
    { return (bs->_M_flags & mask) != 0; }

private:
    bitset_type* bs  ;
    value_type   mask;
};

// =========================================================

template <enum_t T>
class bitset
{
public:
    typedef bitset<T>                                       self_type             ;
    typedef std::remove_cvref_t<T>                          enum_type             ;
    typedef std::size_t                                     size_type             ;
    typedef std::underlying_type_t<T>                       value_type            ;
    typedef std::string_view                                string_view           ;
    typedef string                                          string_type           ;
    typedef reference_proxy<T>                              ref_proxy_type        ;
    typedef set_bit_iterator<T>                             iterator              ;
    typedef set_bit_iterator<T const>                       const_iterator        ;
    typedef std::reverse_iterator<iterator>                 reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>           reverse_const_iterator;
    typedef std::array<char, (sizeof (value_type) * 8) + 1> str_vector_type       ;

    inline constexpr static size_type npos = static_cast<size_type> (-1);

    consteval bitset () noexcept = default;

    constexpr bitset (enum_type const eFlag) noexcept
    : _M_flags (static_cast<value_type> (eFlag))
    { }

    constexpr bitset (value_type const flags) noexcept
    : _M_flags (flags)
    { }

    template <integer_t U,
              typename = std::enable_if_t<!std::is_same_v<U, value_type> &&
                                          !std::is_same_v<U, int>>
              >
    constexpr bitset (U const eFlag) noexcept
    : _M_flags (static_cast<value_type> (eFlag))
    {
        static_assert (sizeof (U) <= sizeof (value_type),
                       "size of U is bigger than the size of value_type!");
    }

    constexpr bitset (self_type const& obj) noexcept
    : _M_flags (obj._M_flags)
    { }

    constexpr bitset (self_type&& obj) noexcept
    : _M_flags (std::move (obj._M_flags))
    { assert (obj._M_flags == value_type ()); }

    //! constexpr constructor from string_view
    constexpr bitset (string_view const& sv) noexcept
    : _M_flags ()
    {
        for (auto i = 0UL; i < sv.size () && i < size (); ++i)
        {
            if (sv[i] == '1') _M_flags |= static_cast<value_type> (1 << i);
        }
    }

    constexpr self_type& operator = (self_type const& obj) noexcept
    {
        if (this != &obj) _M_flags = obj._M_flags;
        return *this;
    }

    constexpr self_type& operator = (self_type&& obj) noexcept
    {
        if (this != &obj) _M_flags = std::move (obj._M_flags);
        return *this;
    }

    constexpr self_type& operator = (enum_type const eFlag) noexcept
    { _M_flags = static_cast<value_type> (eFlag); return *this; }

    constexpr self_type& operator = (value_type const nFlags) noexcept
    { _M_flags = nFlags; return *this; }

    template <integer_t U,
              typename = std::enable_if_t<!std::is_same_v<U, value_type> &&
                                          !std::is_same_v<U, int>>
              >
    constexpr self_type& operator = (U const nFlags) noexcept
    {
        static_assert (sizeof (U) <= sizeof (value_type), "size of U is bigger than the size of value_type!");
        _M_flags = static_cast<value_type> (nFlags); return *this;
    }

    constexpr void flip  (enum_type const eFlag) noexcept { _M_flags ^= static_cast<value_type> (eFlag); }
    constexpr void flip  () noexcept { _M_flags = _M_flags ^ _M_flags; }
    constexpr void reset () noexcept { _M_flags = value_type (); }

    constexpr operator value_type () const noexcept
    { return _M_flags; }

    constexpr value_type value () const noexcept
    { return _M_flags; }

    constexpr enum_type enum_value () const noexcept
    { return static_cast<enum_type> (_M_flags); }

    //! bitset size in bits
    consteval static size_type size () noexcept
    { return sizeof (value_type) * 8; }

    //! use std::popcount for counting set bits
    constexpr size_type set_count () const noexcept
    { return static_cast<size_type> (std::popcount (_M_flags)); }

    constexpr bool test (enum_type const eFlag) const noexcept
    { return (_M_flags & static_cast<value_type> (eFlag)) == static_cast<value_type> (eFlag); }

    constexpr bool test (value_type const flags) const noexcept
    { return (_M_flags & flags) == flags; }

    template <integer_t U,
              typename = std::enable_if_t<!std::is_same_v<U, value_type> &&
                                          !std::is_same_v<U, int>>
              >
    constexpr bool test (U const flags) const noexcept
    {
        static_assert (sizeof (U) <= sizeof (value_type), "size of U is bigger than the size of value_type!");
        return (_M_flags & static_cast<value_type> (flags)) == static_cast<value_type> (flags);
    }

    constexpr bool test (self_type const& gObj) const noexcept
    { return (_M_flags & gObj._M_flags) == gObj._M_flags; }

    consteval bool all () const noexcept
    { return _M_flags == ~static_cast<value_type> (0); }

    consteval bool any () const noexcept
    { return _M_flags != value_type (); }

    consteval bool none () const noexcept
    { return _M_flags == value_type (); }

    constexpr size_type count_trailing_zeros () const noexcept
    { return std::countr_zero (_M_flags); }

    constexpr size_type count_leading_zeros () const noexcept
    { return std::countl_zero (_M_flags); }

    constexpr size_type find_first_set () const noexcept
    { return std::countr_zero (_M_flags); }

    constexpr size_type find_last_set () const noexcept
    { return size () - 1 - std::countl_zero (_M_flags); }

    constexpr self_type& operator += (enum_type const eFlag) noexcept
    { _M_flags |= static_cast<value_type> (eFlag); return *this; }

    constexpr self_type& operator += (value_type const flags) noexcept
    { _M_flags |= flags; return *this; }

    template <integer_t U,
              typename = std::enable_if_t<!std::is_same_v<U, value_type> &&
                                          !std::is_same_v<U, int>>
              >
    constexpr self_type& operator += (U const flags) noexcept
    {
        static_assert (sizeof (U) <= sizeof (value_type), "size of U is bigger than the size of value_type!");
        _M_flags |= static_cast<value_type> (flags); return *this;
    }

    constexpr self_type& operator -= (enum_type const eFlag) noexcept
    { _M_flags &= ~static_cast<value_type> (eFlag); return *this; }

    constexpr self_type& operator -= (value_type const flags) noexcept
    { _M_flags &= ~flags; return *this; }

    template <integer_t U,
              typename = std::enable_if_t<!std::is_same_v<U, value_type> &&
                                          !std::is_same_v<U, int>>
              >
    constexpr self_type& operator -= (U const flags) noexcept
    {
        static_assert (sizeof (U) <= sizeof (value_type), "size of U is bigger than the size of value_type!");
        _M_flags &= ~static_cast<value_type> (flags); return *this;
    }

    //! bitwise operations
    constexpr self_type operator  & (self_type const& other) const noexcept
    { return  self_type (_M_flags & other._M_flags); }

    constexpr self_type operator  | (self_type const& other) const noexcept
    { return  self_type (_M_flags | other._M_flags); }

    constexpr self_type operator  ^ (self_type const& other) const noexcept
    { return  self_type (_M_flags ^ other._M_flags); }

    constexpr self_type operator  ~ () const noexcept
    { return  self_type (~_M_flags); }

    // =========================================================

    constexpr iterator        begin ()       noexcept { return iterator       (*this); }
    constexpr iterator        begin () const noexcept { return iterator       (*this); }
    constexpr const_iterator cbegin () const noexcept { return const_iterator (*this); }
    constexpr const_iterator cbegin ()       noexcept { return const_iterator (*this); }

    constexpr reverse_iterator  rbegin () noexcept
    { return reverse_iterator (*this, size () - 1); }

    constexpr reverse_iterator  rbegin () const noexcept
    { return reverse_iterator (*this, size () - 1); }

    constexpr reverse_const_iterator crbegin () const noexcept
    { return reverse_const_iterator (*this, size () - 1); }

    constexpr reverse_const_iterator crbegin () noexcept
    { return reverse_const_iterator (*this, size () - 1); }

    constexpr iterator        end   ()       noexcept { return iterator (*this, size ()); }
    constexpr iterator        end   () const noexcept { return iterator (*this, size ()); }
    constexpr const_iterator cend   () const noexcept { return const_iterator (*this, size ()); }
    constexpr const_iterator cend   ()       noexcept { return const_iterator (*this, size ()); }

    constexpr reverse_iterator rend () noexcept
    { return reverse_iterator (*this, npos); }

    constexpr reverse_iterator rend () const noexcept
    { return reverse_iterator (*this, npos); }

    constexpr reverse_const_iterator crend () const noexcept
    { return reverse_const_iterator (*this, npos); }

    constexpr reverse_const_iterator crend () noexcept
    { return reverse_const_iterator (*this, npos); }

    // =========================================================

    constexpr ref_proxy_type operator [] (enum_type const flag) const noexcept
    { return  ref_proxy_type (*this, static_cast<value_type> (flag)); }

    // =========================================================

    //! string conversion
    inline string_type to_string () const noexcept
    {
        string_type result;

        result.reserve (size ());

        for (auto i = 0UL; i < size (); ++i)
        {
            result[(size () - 1) - i] = test (static_cast<value_type> (1) << i) ? '1' : '0';
        }

        return result;
    }

    inline static self_type from_string (string_view const& str)
    {
        self_type result;

        for (size_type i = 0; i < str.size () && i < size (); ++i)
        {
            if (str[i] == '1') result = static_cast<enum_type> (static_cast<value_type> (1) << i);
        }

        return result;
    }

    //! compile-time utilities
    template <size_type N>
    constexpr static self_type low_bits () noexcept
    {
        static_assert (N <= size (), "N must NOT exceed bitset size!");

        if constexpr (N == size ())
            return self_type { static_cast<value_type> (~static_cast<value_type> (0)) };
        else
            return self_type { (static_cast<value_type> (1) << N) - 1 };
    }

    //! friend functions
    template <typename U>
    friend
    constexpr bool operator == (bitset<U> const&, U const) noexcept;

    template <typename U>
    friend
    constexpr bool operator == (bitset<U> const&, typename bitset<U>::value_type const) noexcept;

    template <typename U>
    friend
    constexpr bool operator == (typename bitset<U>::value_type const, bitset<U> const&) noexcept;

    template <typename U>
    friend
    constexpr bool operator == (bitset<U> const&, bitset<U> const&) noexcept;

    template <typename U>
    friend
    constexpr bitset<U> operator + (bitset<U> const&,
                                    typename bitset<U>::enum_type const) noexcept;

    template <typename U>
    friend
    constexpr bitset<U> operator + (bitset<U> const&,
                                    typename bitset<U>::value_type const) noexcept;

    template <typename U>
    friend
    constexpr bitset<U> operator - (bitset<U> const&,
                                    typename bitset<U>::enum_type const) noexcept;

    template <typename U>
    friend
    constexpr bitset<U> operator - (bitset<U> const&,
                                    typename bitset<U>::value_type const) noexcept;

private:
    value_type _M_flags { };
};

// =========================================================

template <typename T>
constexpr bitset<T> operator + (bitset<T> const& lh,
                                typename bitset<T>::enum_type const eFlag) noexcept
{ return lh._M_flags | static_cast<typename bitset<T>::value_type> (eFlag); }

template <typename T>
constexpr bitset<T> operator + (bitset<T> const& lh,
                                typename bitset<T>::value_type const flags) noexcept
{ return lh._M_flags | flags; }

template <typename T>
constexpr bitset<T> operator - (bitset<T> const& lh,
                                typename bitset<T>::enum_type const eFlag) noexcept
{ return lh._M_flags & ~static_cast<typename bitset<T>::value_type> (eFlag); }

template <typename T>
constexpr bitset<T> operator - (bitset<T> const& lh,
                                typename bitset<T>::value_type const flags) noexcept
{ return lh._M_flags & ~flags; }

// =========================================================

template <typename T>
constexpr bool operator == (bitset<T> const& lh, T const eFlag) noexcept
{ return lh._M_flags == eFlag; }

template <typename T>
constexpr bool operator == (bitset<T> const& lh, typename bitset<T>::value_type const flags) noexcept
{ return lh._M_flags == flags; }

template <typename T>
constexpr bool operator == (typename bitset<T>::value_type const flags, bitset<T> const& rh) noexcept
{ return flags == rh._M_flags; }

template <typename T>
constexpr bool operator == (bitset<T> const& lh, bitset<T> const& rh) noexcept
{ return lh._M_flags == rh._M_flags; }

template <typename T>
constexpr bool operator != (bitset<T> const& lh, T const eFlag) noexcept
{ return !(lh == eFlag); }

template <typename T>
constexpr bool operator != (bitset<T> const& lh, typename bitset<T>::value_type const flags) noexcept
{ return !(lh == flags); }

template <typename T>
constexpr bool operator != (typename bitset<T>::value_type const flags, bitset<T> const& rh) noexcept
{ return !(flags == rh); }

template <typename T>
constexpr bool operator != (bitset<T> const& lh, bitset<T> const& rh) noexcept
{ return !(lh == rh); }

// =========================================================

//! compile-time bit manipulation
template <enum_t T, std::remove_cvref_t<T>... Flags>
consteval bitset<T> make_bitset () noexcept
{
    return bitset<T> ((... | static_cast<bitset<T>::value_type> (Flags)));
}

// =========================================================

} // namespace cppual

// =========================================================

namespace std {

using cppual::enum_t;

template <enum_t T>
struct hash <cppual::bitset<T>>
{
    size_t operator () (cppual::bitset<T> const& bs) const noexcept
    { return std::hash<typename cppual::bitset<T>::value_type> { } (bs.value ()); }
};

} // namespace std

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_BITSET_H_
