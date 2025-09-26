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

#ifndef CPPUAL_ITERATOR_H_
#define CPPUAL_ITERATOR_H_
#ifdef __cplusplus

#include <cppual/concepts>

#include <iterator>

// ====================================================

namespace cppual {

// ====================================================

template <typename T>
class bidirectional_iterator
{
public:
    typedef bidirectional_iterator<T>       self_type        ;
    typedef std::remove_reference_t<T>      buf_type         ;
    typedef std::remove_const_t<buf_type>   buf_clean_type   ;
    typedef typename T::value_type          value_type       ;
    typedef value_type const                const_value      ;
    typedef typename T::pointer             pointer          ;
    typedef typename T::const_pointer       const_pointer    ;
    typedef typename T::reference           reference        ;
    typedef typename T::const_reference     const_reference  ;
    typedef typename T::difference_type     difference_type  ;
    typedef typename T::size_type           size_type        ;
    typedef size_type const                 const_size       ;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef std::conditional_t<std::is_const_v<buf_type>, const_value, value_type> elem_type;

    template <typename U>
    using self_type_t = bidirectional_iterator<U>;

    inline constexpr static const_size npos = const_size (-1);

    friend class bidirectional_iterator<std::remove_const_t<buf_type> const>;
    friend class bidirectional_iterator<std::remove_const_t<buf_type>>;

    constexpr elem_type& operator *  () const { return  (get ()[_M_uPos]); }
    constexpr elem_type* operator -> () const { return &(get ()[_M_uPos]); }

    constexpr bidirectional_iterator () noexcept = default;

    constexpr
    explicit
    bidirectional_iterator (buf_type& b, const_size p = const_size ()) noexcept
    : _M_pBuf (&b), _M_uPos (p)
    { }

    //! converting a const iterator to a non-const iterator
    constexpr
    bidirectional_iterator (self_type_t<buf_type const> const& other) noexcept
    : _M_pBuf (const_cast<buf_type*> (other._M_pBuf)), _M_uPos (other._M_uPos)
    { }

    //! converting a non-const iterator to a const iterator
    constexpr
    bidirectional_iterator (self_type_t<std::remove_const_t<buf_type>> const& other) noexcept
    : _M_pBuf (other._M_pBuf), _M_uPos (other._M_uPos)
    { }

    //! converting a non-const iterator to a const iterator
    constexpr
    self_type& operator = (self_type_t<std::remove_const_t<buf_type>> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_pBuf = other._M_pBuf;
        _M_uPos = other._M_uPos;

        return *this;
    }

    //! converting a const iterator to a non-const iterator
    constexpr
    self_type& operator = (self_type_t<buf_type const> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_pBuf = const_cast<buf_type*> (other._M_pBuf);
        _M_uPos = other._M_uPos;

        return *this;
    }

    constexpr self_type& operator ++ ()
    {
        ++_M_uPos;
        return *this;
    }

    constexpr self_type operator ++ (int)
    {
        self_type tmp (*this);
         ++_M_uPos;
        return tmp;
    }

    constexpr self_type& operator -- ()
    {
        --_M_uPos;
        return *this;
    }
    constexpr self_type operator -- (int)
    {
        self_type tmp (*this);
        --_M_uPos;
        return tmp;
    }

    constexpr self_type operator + (difference_type n) const
    {
        self_type tmp (*this);
        tmp._M_uPos += static_cast<size_type> (n);
        return tmp;
    }

    constexpr self_type& operator += (difference_type n)
    {
        _M_uPos += static_cast<size_type> (n);
        return *this;
    }

    constexpr self_type operator - (difference_type n) const
    {
        self_type tmp (*this);
        tmp._M_uPos -= static_cast<size_type> (n);
        return tmp;
    }

    constexpr self_type& operator -= (difference_type n)
    {
        _M_uPos -= static_cast<size_type> (n);
        return *this;
    }

    constexpr difference_type operator + (self_type const& other) const
    { return difference_type (_M_uPos + other._M_uPos); }

    constexpr difference_type operator - (self_type const& other) const
    { return difference_type (_M_uPos - other._M_uPos); }

    constexpr bool operator == (self_type const& other) const
    { return _M_uPos == other._M_uPos && _M_pBuf == other._M_pBuf; }

    constexpr bool operator != (self_type const& other) const
    { return !(*this == other); }

    constexpr bool operator > (self_type const& other) const
    { return _M_uPos == npos ? false : _M_uPos > other._M_uPos; }

    constexpr bool operator >= (self_type const& other) const
    { return _M_uPos == npos ? false : _M_uPos >= other._M_uPos; }

    constexpr bool operator < (self_type const& other) const
    { return _M_uPos == npos ? other._M_uPos == npos ? false : true : _M_uPos < other._M_uPos; }

    constexpr bool operator <= (self_type const& other) const
    { return _M_uPos == npos ? true : _M_uPos <= other._M_uPos; }

    constexpr buf_type& get () const noexcept { return *_M_pBuf; }
    consteval size_type pos () const noexcept { return  _M_uPos; }

private:
    buf_type* _M_pBuf { };
    size_type _M_uPos { };
};

// =========================================================

} // cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_ITERATOR_H_
