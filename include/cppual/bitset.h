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

#include <cppual/containers>
#include <cppual/concepts>
#include <cppual/iterator>

#include <string_view>
#include <type_traits>
#include <iterator>
#include <cstddef>
#include <cassert>
#include <array>
#include <bit>

// =========================================================

namespace cppual {

// =========================================================

template <enumeration> class bitset;

// =========================================================

//! bitset reference proxy
template <enumeration T>
class reference_proxy
{
public:
    typedef reference_proxy<T>        self_type  ;
    typedef bitset<T>                 bitset_type;
    typedef std::underlying_type_t<T> int_type   ;
    typedef int_type                  const_int  ;

    constexpr reference_proxy (self_type &&)           noexcept = default;
    consteval reference_proxy (self_type const&)       noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    consteval reference_proxy (bitset_type& bitset, const_int mask) noexcept
    : _M_bs (&bitset), _M_mask (mask)
    { }

    constexpr self_type& operator = (cbool x) noexcept
    { x ? _M_bs->_M_flags |= _M_mask : _M_bs->_M_flags &= ~_M_mask; return *this; }

    consteval operator bool () const noexcept
    { return (_M_bs->_M_flags & _M_mask) != 0; }

private:
    reference_proxy () = delete;

private:
    bitset_type* _M_bs   { };
    int_type     _M_mask { };
};

// =========================================================

template <enumeration T>
class bitset
{
public:
    typedef bitset<T>                                     self_type             ;
    typedef remove_cvref_t<T>                             value_type            ;
    typedef value_type const                              const_value           ;
    typedef std::underlying_type_t<value_type>            int_type              ;
    typedef int_type const                                const_int             ;
    typedef std::string_view                              string_view           ;
    typedef string                                        string_type           ;
    typedef std::size_t                                   size_type             ;
    typedef size_type const                               const_size            ;
    typedef reference_proxy<T>                            ref_proxy_type        ;
    typedef set_bit_iterator<T>                           iterator              ;
    typedef set_bit_iterator<T const>                     const_iterator        ;
    typedef std::reverse_iterator<iterator>               reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>         reverse_const_iterator;
    typedef std::array<char, (sizeof (int_type) * 8) + 1> str_vector_type       ;

    inline constexpr static const_size npos = static_cast<size_type> (-1);

    constexpr bitset () noexcept = default;
    constexpr bitset (self_type&&) noexcept = default;
    constexpr bitset (self_type const&) noexcept = default;
    constexpr self_type& operator = (self_type&&) noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr bitset (const_value eFlag) noexcept
    : _M_flags (static_cast<int_type> (eFlag))
    { }

    constexpr bitset (const_int flags) noexcept
    : _M_flags (flags)
    { }

    template <integer U, std::enable_if_t<!are_same<U, int_type>, void>>
    constexpr bitset (U const eFlag) noexcept
    : _M_flags (static_cast<int_type> (eFlag))
    {
        static_assert (sizeof (U) <= sizeof (int_type),
                       "size of U is bigger than the size of int_type!");
    }

    //! constexpr constructor from string_view
    constexpr bitset (string_view const& sv) noexcept
    : _M_flags ()
    {
        for (auto i = 0U; i < sv.size () && i < size (); ++i)
        {
            if (sv[i] == '1') _M_flags |= static_cast<int_type> (1 << i);
        }
    }

    constexpr self_type& operator = (const_value eFlag) noexcept
    { _M_flags = static_cast<int_type> (eFlag); return *this; }

    constexpr self_type& operator = (const_int nFlags) noexcept
    { _M_flags = nFlags; return *this; }

    template <integer U,
              std::enable_if_t<!are_same<U, int_type> && !are_same<U, int>, void>
              >
    constexpr self_type& operator = (U const nFlags) noexcept
    {
        static_assert (sizeof (U) <= sizeof (int_type), "size of U is bigger than the size of int_type!");
        _M_flags = static_cast<int_type> (nFlags); return *this;
    }

    constexpr void flip  (const_value eFlag) noexcept { _M_flags ^= static_cast<int_type> (eFlag); }
    constexpr void flip  () noexcept { _M_flags = _M_flags ^ _M_flags; }
    constexpr void reset () noexcept { _M_flags = int_type (); }

    constexpr operator int_type () const noexcept
    { return _M_flags; }

    constexpr int_type value () const noexcept
    { return _M_flags; }

    constexpr value_type enum_value () const noexcept
    { return static_cast<value_type> (_M_flags); }

    //! bitset size in bits
    consteval static size_type size () noexcept
    { return sizeof (int_type) * 8; }

    //! use std::popcount for counting set bits
    constexpr size_type set_count () const noexcept
    { return static_cast<size_type> (std::popcount (_M_flags)); }

    constexpr bool test (const_value eFlag) const noexcept
    { return (_M_flags & static_cast<int_type> (eFlag)) == static_cast<int_type> (eFlag); }

    constexpr bool test (const_int flags) const noexcept
    { return (_M_flags & flags) == flags; }

    template <integer U,
              std::enable_if_t<!are_same<U, int_type> && !are_same<U, int>, void>
              >
    constexpr bool test (U const flags) const noexcept
    {
        static_assert (sizeof (U) <= sizeof (int_type), "size of U is bigger than the size of int_type!");
        return (_M_flags & static_cast<int_type> (flags)) == static_cast<int_type> (flags);
    }

    constexpr bool test (self_type const& gObj) const noexcept
    { return (_M_flags & gObj._M_flags) == gObj._M_flags; }

    consteval bool all () const noexcept
    { return _M_flags == ~static_cast<int_type> (0); }

    consteval bool any () const noexcept
    { return _M_flags != int_type (); }

    consteval bool none () const noexcept
    { return _M_flags == int_type (); }

    constexpr size_type count_trailing_zeros () const noexcept
    { return std::countr_zero (_M_flags); }

    constexpr size_type count_leading_zeros () const noexcept
    { return std::countl_zero (_M_flags); }

    constexpr size_type find_first_set () const noexcept
    { return std::countr_zero (_M_flags); }

    constexpr size_type find_last_set () const noexcept
    { return size () - 1 - count_leading_zeros (); }

    constexpr self_type& operator += (const_value eFlag) noexcept
    { _M_flags |= static_cast<int_type> (eFlag); return *this; }

    constexpr self_type& operator += (const_int flags) noexcept
    { _M_flags |= flags; return *this; }

    template <integer U,
              std::enable_if_t<!are_same<U, int_type> && !are_same<U, int>, void>
              >
    constexpr self_type& operator += (U const flags) noexcept
    {
        static_assert (sizeof (U) <= sizeof (int_type),
                      "size of U is bigger than the size of int_type!");

        _M_flags |= static_cast<int_type> (flags); return *this;
    }

    constexpr self_type& operator -= (const_value eFlag) noexcept
    { _M_flags &= ~static_cast<int_type> (eFlag); return *this; }

    constexpr self_type& operator -= (const_int flags) noexcept
    { _M_flags &= ~flags; return *this; }

    template <integer U,
              std::enable_if_t<!are_same<U, int_type> && !are_same<U, int>, void>
              >
    constexpr self_type& operator -= (U const flags) noexcept
    {
        static_assert (sizeof (U) <= sizeof (int_type),
                      "size of U is bigger than the size of int_type!");

        _M_flags &= ~static_cast<int_type> (flags); return *this;
    }

    //! bitwise operations
    consteval self_type operator  & (self_type const& other) const noexcept
    { return  self_type (_M_flags & other._M_flags); }

    consteval self_type operator  | (self_type const& other) const noexcept
    { return  self_type (_M_flags | other._M_flags); }

    consteval self_type operator  ^ (self_type const& other) const noexcept
    { return  self_type (_M_flags ^ other._M_flags); }

    consteval self_type operator  ~ () const noexcept
    { return  self_type (~_M_flags); }

    // =========================================================

    consteval iterator        begin ()       noexcept { return iterator       (*this); }
    consteval iterator        begin () const noexcept { return iterator       (*this); }
    consteval const_iterator cbegin () const noexcept { return const_iterator (*this); }
    consteval const_iterator cbegin ()       noexcept { return const_iterator (*this); }
    consteval iterator        end   ()       noexcept { return iterator       (*this, size ()); }
    consteval iterator        end   () const noexcept { return iterator       (*this, size ()); }
    consteval const_iterator cend   () const noexcept { return const_iterator (*this, size ()); }
    consteval const_iterator cend   ()       noexcept { return const_iterator (*this, size ()); }

    consteval reverse_iterator  rbegin () noexcept
    { return reverse_iterator (*this, size () - 1); }

    consteval reverse_iterator  rbegin () const noexcept
    { return reverse_iterator (*this, size () - 1); }

    consteval reverse_const_iterator crbegin () const noexcept
    { return reverse_const_iterator (*this, size () - 1); }

    consteval reverse_const_iterator crbegin () noexcept
    { return reverse_const_iterator (*this, size () - 1); }

    consteval reverse_iterator rend () noexcept
    { return reverse_iterator (*this, npos); }

    consteval reverse_iterator rend () const noexcept
    { return reverse_iterator (*this, npos); }

    consteval reverse_const_iterator crend () const noexcept
    { return reverse_const_iterator (*this, npos); }

    consteval reverse_const_iterator crend () noexcept
    { return reverse_const_iterator (*this, npos); }

    // =========================================================

    constexpr ref_proxy_type operator [] (const_value flag) const noexcept
    { return  ref_proxy_type (*this, static_cast<int_type> (flag)); }

    // =========================================================

    //! string conversion
    constexpr string_type to_string () const noexcept
    {
        string_type result;

        result.reserve (size ());

        for (auto i = 0UL; i < size (); ++i)
        {
            result[(size () - 1) - i] = test (static_cast<int_type> (1) << i) ? '1' : '0';
        }

        return result;
    }

    constexpr static self_type from_string (string_view const& str)
    {
        self_type result;

        for (size_type i = 0; i < str.size () && i < size (); ++i)
        {
            if (str[i] == '1') result += static_cast<value_type> (static_cast<int_type> (1) << i);
        }

        return result;
    }

    //! compile-time utilities
    template <size_type N>
    constexpr static self_type low_bits () noexcept
    {
        static_assert (N <= size (), "N must NOT exceed bitset size!");

        if constexpr (N == size ())
            return self_type (static_cast<int_type> (~static_cast<int_type> (0)));
        else
            return self_type ((static_cast<int_type> (1) << N) - 1);
    }

    //! friend functions
    template <enumeration U>
    friend
    constexpr bool operator == (bitset<U> const&, U const) noexcept;

    template <enumeration U>
    friend
    constexpr bool operator == (bitset<U> const&, typename bitset<U>::const_int) noexcept;

    template <enumeration U>
    friend
    constexpr bool operator == (typename bitset<U>::const_int, bitset<U> const&) noexcept;

    template <enumeration U>
    friend
    constexpr bool operator == (bitset<U> const&, bitset<U> const&) noexcept;

    template <enumeration U>
    friend
    constexpr bitset<U> operator + (bitset<U> const&,
                                    typename bitset<U>::const_value) noexcept;

    template <enumeration U>
    friend
    constexpr bitset<U> operator + (bitset<U> const&,
                                    typename bitset<U>::const_int) noexcept;

    template <enumeration U>
    friend
    constexpr bitset<U> operator - (bitset<U> const&,
                                    typename bitset<U>::const_value) noexcept;

    template <enumeration U>
    friend
    constexpr bitset<U> operator - (bitset<U> const&,
                                    typename bitset<U>::const_int) noexcept;

private:
    int_type _M_flags { };
};

// =========================================================

template <enumeration T>
constexpr bitset<T> operator + (bitset<T> const& lh,
                                typename bitset<T>::const_value eFlag) noexcept
{ return lh._M_flags | static_cast<typename bitset<T>::int_type> (eFlag); }

template <enumeration T>
constexpr bitset<T> operator + (bitset<T> const& lh,
                                typename bitset<T>::const_int flags) noexcept
{ return lh._M_flags | flags; }

template <enumeration T>
constexpr bitset<T> operator - (bitset<T> const& lh,
                                typename bitset<T>::const_value eFlag) noexcept
{ return lh._M_flags & ~static_cast<typename bitset<T>::int_type> (eFlag); }

template <enumeration T>
constexpr bitset<T> operator - (bitset<T> const& lh,
                                typename bitset<T>::const_int flags) noexcept
{ return lh._M_flags & ~flags; }

// =========================================================

template <enumeration U>
constexpr bool operator == (bitset<U> const& lh, bitset<U> const& rh) noexcept
{ return lh._M_flags == rh._M_flags; }

template <enumeration T>
constexpr auto operator <=> (bitset<T> const& lh, bitset<T> const& rh) noexcept
{ return lh._M_flags <=> rh._M_flags; }

template <enumeration T>
constexpr auto operator <=> (bitset<T> const& lh, T const eFlag) noexcept
{ return lh._M_flags <=> eFlag; }

template <enumeration T>
constexpr auto operator <=> (T const eFlag, bitset<T> const& lh) noexcept
{ return eFlag <=> lh._M_flags; }

template <enumeration T>
constexpr auto operator <=> (bitset<T> const& lh, typename bitset<T>::const_int flags) noexcept
{ return lh._M_flags <=> flags; }

template <enumeration T>
constexpr auto operator <=> (typename bitset<T>::const_int flags, bitset<T> const& rh) noexcept
{ return flags <=> rh._M_flags; }

// =========================================================

//! compile-time bit manipulation
template <auto... Flags,
          typename E = std::common_type_t<decltype (Flags)...>,
          std::enable_if_t<enumeration<E>, void>
          >
consteval bitset<E> make_bitset () noexcept
{
    return bitset<E> ((static_cast<E> (Flags) | ...));
}

// =========================================================

} // namespace cppual

// =========================================================

namespace std {

using cppual::enumeration;

template <enumeration T>
struct hash <cppual::bitset<T>>
{
    typedef size_t size_type;

    constexpr size_type operator () (cppual::bitset<T> const& bs) const noexcept
    { return std::hash<typename cppual::bitset<T>::int_type> { } (bs.value ()); }
};

} // namespace std

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_BITSET_H_
