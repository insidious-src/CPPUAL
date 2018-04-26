/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#include <iterator>

namespace cppual {

template <typename T>
using IteratorType = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::input_iterator_tag>::value or
std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::output_iterator_tag>::value, T>::type;

template <typename T>
using OutputIterator = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::output_iterator_tag>::value, T>::type;

template <typename T>
using InputIterator = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::input_iterator_tag>::value, T>::type;

template <typename T>
using ForwardIterator = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::forward_iterator_tag>::value>::type;

template <typename T>
using BidirectionalIterator = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::bidirectional_iterator_tag>::value>::type;

template <typename T>
using RandomAccessIterator = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::random_access_iterator_tag>::value>::type;

// ====================================================

template <typename T>
class CircularIterator
{
public:
    typedef T                               buf_type;
    typedef typename T::pointer             pointer;
    typedef typename T::const_pointer       const_pointer;
    typedef typename T::reference           reference;
    typedef typename T::const_reference     const_reference;
    typedef typename T::difference_type     difference_type;
    typedef typename T::size_type           size_type;
    typedef typename T::value_type          value_type;
    typedef CircularIterator<T>             self_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef typename std::conditional<std::is_const<buf_type>::value,
    value_type const, value_type>::type
    elem_type;

    friend class CircularIterator<buf_type const>;

    constexpr elem_type& operator *  () { return   get ()[m_uPos];  }
    constexpr elem_type* operator -> () { return &(get ()[m_uPos]); }

    constexpr CircularIterator () noexcept
    : m_pBuf (0), m_uPos (0)
    { }

    constexpr CircularIterator (buf_type& b, size_type p) noexcept
    : m_pBuf (&b), m_uPos (p)
    { }

    // Converting a non-const iterator to a const iterator
    constexpr
    CircularIterator (CircularIterator<typename std::remove_const<buf_type>::type>
                      const& other) noexcept
    : m_pBuf (other.m_pBuf), m_uPos (other.m_uPos)
    { }

    inline self_type& operator ++ ()
    {
        ++m_uPos;
        return *this;
    }

    inline self_type operator ++ (int)
    {
        self_type tmp (*this);
        ++m_uPos;
        return tmp;
    }

    inline self_type& operator -- ()
    {
        --m_uPos;
        return *this;
    }
    inline self_type operator -- (int)
    {
        self_type tmp (*this);
        --m_uPos;
        return tmp;
    }

    constexpr self_type operator + (difference_type n) const
    {
        self_type tmp (*this);
        tmp.m_uPos += n;
        return tmp;
    }

    inline self_type& operator += (difference_type n)
    {
        m_uPos += n;
        return *this;
    }

    constexpr self_type operator - (difference_type n) const
    {
        self_type tmp (*this);
        tmp.m_uPos -= n;
        return tmp;
    }

    inline self_type& operator -= (difference_type n)
    {
        m_uPos -= n;
        return *this;
    }

    constexpr difference_type operator - (self_type const& c) const
    { return difference_type (m_uPos - c.m_uPos); }

    constexpr bool operator == (self_type const& other) const
    { return m_uPos == other.m_uPos and m_pBuf == other.m_pBuf; }

    constexpr bool operator != (self_type const& other) const
    { return m_uPos != other.m_uPos or m_pBuf != other.m_pBuf; }

    constexpr bool operator > (self_type const& other) const
    { return m_uPos > other.m_uPos; }

    constexpr bool operator >= (self_type const& other) const
    { return m_uPos >= other.m_uPos; }

    constexpr bool operator < (self_type const& other) const
    { return m_uPos < other.m_uPos; }

    constexpr bool operator <= (self_type const& other) const
    { return m_uPos <= other.m_uPos; }

    constexpr buf_type& get () const noexcept
    { return *m_pBuf; }

    constexpr size_type pos () const noexcept
    { return m_uPos; }

private:
    buf_type* m_pBuf;
    size_type m_uPos;
};

// =========================================================

template <typename circular_buffer_iterator_t>
constexpr
circular_buffer_iterator_t
operator + (const typename circular_buffer_iterator_t::difference_type &a,
            const circular_buffer_iterator_t                           &b)
{ return circular_buffer_iterator_t (a) + b; }

template <typename circular_buffer_iterator_t>
constexpr
circular_buffer_iterator_t
operator - (const typename circular_buffer_iterator_t::difference_type &a,
            const circular_buffer_iterator_t                           &b)
{ return circular_buffer_iterator_t (a) - b; }

} // cppual

#endif // __cplusplus
#endif // CPPUAL_ITERATOR_H_
