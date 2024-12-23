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

#ifndef CPPUAL_BITSET_ITERATOR_H_
#define CPPUAL_BITSET_ITERATOR_H_
#ifdef __cplusplus

#include <cppual/concept/concepts.h>

#include <type_traits>
#include <cstddef>

// =========================================================

namespace cppual {

// =========================================================

template <enum_t> class bitset;

// =========================================================

//! iterator for set bits
template <enum_t T>
class set_bit_iterator
{
public:
    typedef set_bit_iterator<T>             self_type        ;
    typedef bitset<T>                       buf_type         ;
    typedef std::remove_reference_t<T>      value_type       ;
    typedef std::underlying_type_t<T>       int_type         ;
    typedef value_type const                const_value      ;
    typedef int_type   const                const_int        ;
    typedef value_type const*               pointer          ;
    typedef value_type const&               reference        ;
    typedef std::size_t                     size_type        ;
    typedef std::ptrdiff_t                  difference_type  ;
    typedef std::bidirectional_iterator_tag iterator_category;

    template <enum_t U> using self_type_t = set_bit_iterator<U>;

    typedef std::conditional_t<std::is_const_v<value_type>, const_value, int_type> elem_type;

    inline constexpr static size_type const npos = size_type (-1);

    friend class set_bit_iterator<value_type const>;
    friend class set_bit_iterator<std::remove_const_t<value_type>>;

    consteval elem_type operator * () const noexcept
    { return static_cast<elem_type> (1 << _M_bit_pos); }

    set_bit_iterator () = delete;

    consteval explicit set_bit_iterator (buf_type& bitset, const_int pos = int_type ()) noexcept
    : _M_bs (&bitset), _M_bit_pos (pos)
    { }

    //! converting a const iterator to a non-const iterator
    consteval set_bit_iterator (self_type_t<const_value> const& other) noexcept
    : _M_bs (const_cast<buf_type*> (other._M_bs)), _M_bit_pos (other._M_bit_pos)
    { }

    //! converting a non-const iterator to a const iterator
    consteval set_bit_iterator (self_type_t<std::remove_const_t<value_type>> const& other) noexcept
    : _M_bs (other._M_bs), _M_bit_pos (other._M_bit_pos)
    { }

    //! converting a non-const iterator to a const iterator
    constexpr self_type& operator = (self_type_t<std::remove_const_t<value_type>> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_bs      = other._M_bs     ;
        _M_bit_pos = other._M_bit_pos;

        return *this;
    }

    //! converting a non-const iterator to a const iterator
    constexpr self_type& operator = (self_type_t<const_value> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_bs      = const_cast<buf_type*> (other._M_bs);
        _M_bit_pos = other._M_bit_pos;

        return *this;
    }

    constexpr self_type& operator ++ () noexcept
    {
        if (_M_bs && _M_bit_pos < (sizeof (int_type) * 8))
        {
            ++_M_bit_pos;
            find_next_set_bit ();
        }

        return *this;
    }

    constexpr self_type operator ++ (int) noexcept
    {
        self_type tmp (*this);

        if (_M_bs && _M_bit_pos < (sizeof (int_type) * 8))
        {
            ++_M_bit_pos;
            find_next_set_bit ();
        }

        return tmp;
    }

    constexpr self_type& operator -- () noexcept
    {
        if (_M_bs && int_type () <= _M_bit_pos)
        {
            --_M_bit_pos;
            find_prev_set_bit ();
        }

        return *this;
    }

    constexpr self_type operator -- (int) noexcept
    {
        self_type tmp (*this);

        if (_M_bs && int_type () <= _M_bit_pos)
        {
            --_M_bit_pos;
            find_prev_set_bit ();
        }

        return tmp;
    }

    template <typename U>
    friend constexpr bool operator == (self_type_t<U> const&, self_type_t<U> const&) noexcept;

private:
    constexpr void find_next_set_bit ()
    {
        while (_M_bit_pos < (sizeof (int_type) * 8) &&
              !_M_bs->test (static_cast<value_type> (1 << _M_bit_pos)))
        {
            ++_M_bit_pos;
        }
    }

    constexpr void find_prev_set_bit ()
    {
        while (int_type () <= _M_bit_pos && !_M_bs->test (static_cast<value_type> (1 >> _M_bit_pos)))
        {
            --_M_bit_pos;
        }
    }

private:
    buf_type* _M_bs      { };
    int_type  _M_bit_pos { };
};

// =========================================================

template <typename T>
constexpr bool operator == (set_bit_iterator<T> const& lh, set_bit_iterator<T> const& rh) noexcept
{
    return lh._M_bs == rh._M_bs && lh._M_bit_pos == rh._M_bit_pos;
}

template <typename T>
constexpr bool operator != (set_bit_iterator<T> const& lh, set_bit_iterator<T> const& rh) noexcept
{
    return !(lh == rh);
}

// =========================================================

} // namespace cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_BITSET_ITERATOR_H_
