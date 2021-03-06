﻿/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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
class is_input_iterator : public
        std::conditional<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::input_iterator_tag>::value, std::true_type, std::false_type>::type
{ };

template <typename T>
class is_output_iterator : public
        std::conditional<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::output_iterator_tag>::value, std::true_type, std::false_type>::type
{ };

template <typename T>
class is_forward_iterator : public
        std::conditional<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::forward_iterator_tag>::value, std::true_type, std::false_type>::type
{ };

template <typename T>
class is_BidirectionalIterator : public
        std::conditional<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::bidirectional_iterator_tag>::value, std::true_type, std::false_type>::type
{ };

template <typename T>
class is_random_access_iterator : public
        std::conditional<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::random_access_iterator_tag>::value, std::true_type, std::false_type>::type
{ };

template <typename T>
class is_iterator : public
        std::conditional<is_input_iterator<T>::value ||
        is_output_iterator<T>::value                 ||
        is_forward_iterator<T>::value                ||
        is_BidirectionalIterator<T>::value           ||
        is_random_access_iterator<T>::value, std::true_type, std::false_type>::type
{ };

// ====================================================

template <typename T>
using IteratorType = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::input_iterator_tag>::value or
std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::output_iterator_tag>::value or
std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::bidirectional_iterator_tag>::value, T>::type;

template <typename T>
using OutputIteratorType = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::output_iterator_tag>::value, T>::type;

template <typename T>
using InputIteratorType = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::input_iterator_tag>::value, T>::type;

template <typename T>
using ForwardIteratorType = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::forward_iterator_tag>::value>::type;

template <typename T>
using BidirectionalIteratorType = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::bidirectional_iterator_tag>::value>::type;

template <typename T>
using RandomAccessIteratorType = typename
std::enable_if<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::random_access_iterator_tag>::value>::type;

// ====================================================

template <typename T>
class BidirectionalIterator
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
    typedef BidirectionalIterator<T>        self_type;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef typename std::conditional<std::is_const<buf_type>::value,
    value_type const, value_type>::type
    elem_type;

    friend class BidirectionalIterator<buf_type const>;

    constexpr elem_type& operator *  () const { return   get ()[m_uPos];  }
    constexpr elem_type* operator -> () const { return &(get ()[m_uPos]); }

    constexpr BidirectionalIterator () noexcept
    : m_pBuf (0), m_uPos (0)
    { }

    constexpr BidirectionalIterator (buf_type& b, size_type p) noexcept
    : m_pBuf (&b), m_uPos (p)
    { }

    // Converting a non-const iterator to a const iterator
    constexpr
    BidirectionalIterator (BidirectionalIterator<typename std::remove_const<buf_type>::type>
                           const& other) noexcept
    : m_pBuf (other.m_pBuf), m_uPos (other.m_uPos)
    { }

    inline
    BidirectionalIterator& operator = (BidirectionalIterator<typename std::remove_const<buf_type>::type>
                                       const& other) noexcept
    {
        m_pBuf = other.m_pBuf;
        m_uPos = other.m_uPos;

        return *this;
    }

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

    inline self_type operator + (difference_type n) const
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

    inline self_type operator - (difference_type n) const
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

template <typename Iterator>
constexpr Iterator operator + (const typename Iterator::difference_type &a, Iterator const& b)
{ return Iterator (a) + b; }

template <typename Iterator>
constexpr Iterator operator - (const typename Iterator::difference_type &a, Iterator const& b)
{ return Iterator (a) - b; }

} // cppual

#endif // __cplusplus
#endif // CPPUAL_ITERATOR_H_
