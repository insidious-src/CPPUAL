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

#ifndef CPPUAL_FLAGS_H_
#define CPPUAL_FLAGS_H_
#ifdef __cplusplus

#include <cppual/types.h>

#include <type_traits>
#include <cstddef>

// =========================================================

namespace cppual {

// =========================================================

template <typename T>
class bitset
{
public:
    static_assert (std::is_enum_v<T>, "T is not an enumeration!");

    typedef std::remove_cv_t<T>                enum_type ;
    typedef ulong_t                            size_type ;
    typedef typename std::underlying_type_t<T> value_type;

    constexpr bitset () noexcept = default;
    constexpr bitset (std::nullptr_t) noexcept : _M_flags () { }

    void flip  (enum_type const eFlag) noexcept { _M_flags ^= static_cast<value_type> (eFlag); }
    void flip  () noexcept { _M_flags = _M_flags ^ _M_flags; }
    void reset () noexcept { _M_flags = value_type (); }

    constexpr operator value_type () const noexcept
    { return _M_flags; }

    /// bitset size in bits
    constexpr static size_type size () noexcept
    { return std::integral_constant<size_type, sizeof (value_type) * 8>::value; }

    constexpr bool test (enum_type const eFlag) const noexcept
    { return (_M_flags & static_cast<value_type> (eFlag)) == static_cast<value_type> (eFlag); }

    constexpr bool test (value_type const flags) const noexcept
    { return (_M_flags & flags) == flags; }

    constexpr bool test (bitset const& gObj) const noexcept
    { return (_M_flags & gObj._M_flags) == gObj._M_flags; }

    inline bitset& operator += (enum_type const eFlag) noexcept
    { _M_flags |= static_cast<value_type> (eFlag); return *this; }

    inline bitset& operator += (value_type const flags) noexcept
    { _M_flags |= flags; return *this; }

    inline bitset& operator -= (enum_type const eFlag) noexcept
    { _M_flags &= ~static_cast<value_type> (eFlag); return *this; }

    inline bitset& operator -= (value_type const flags) noexcept
    { _M_flags &= ~flags; return *this; }

    inline bitset& operator = (enum_type const eFlag) noexcept
    { _M_flags = static_cast<value_type> (eFlag); return *this; }

    inline bitset& operator = (value_type const nFlags) noexcept
    { _M_flags = nFlags; return *this; }

    constexpr bitset (enum_type const eFlag) noexcept
    : _M_flags (static_cast<value_type> (eFlag))
    { }

    constexpr bitset (value_type const flags) noexcept
    : _M_flags (flags)
    { }

    /// friend functions

    template <typename U>
    friend
    constexpr bool operator == (bitset<U> const&, U const) noexcept;

    template <typename U>
    friend
    constexpr bool operator == (bitset<U> const&, typename bitset<U>::value_type const) noexcept;

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
    value_type _M_flags;
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
constexpr bool operator == (bitset<T> const& lh,
                            typename bitset<T>::value_type const flags) noexcept
{ return lh._M_flags == flags; }

template <typename T>
constexpr bool operator == (bitset<T> const& lh, bitset<T> const& rh) noexcept
{ return lh._M_flags == rh._M_flags; }

template <typename T>
constexpr bool operator != (bitset<T> const& lh, T const eFlag) noexcept
{ return !(lh == eFlag); }

template <typename T>
constexpr bool operator != (bitset<T> const& lh,
                            typename bitset<T>::value_type const flags) noexcept
{ return !(lh == flags); }

template <typename T>
constexpr bool operator != (bitset<T> const& lh, bitset<T> const& rh) noexcept
{ return !(lh == rh); }

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FLAGS_H_
