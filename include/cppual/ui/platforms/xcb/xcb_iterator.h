/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_XCB_ITERATOR_H_
#define CPPUAL_PLATFORM_XCB_ITERATOR_H_
#ifdef __cplusplus

#include <cppual/decl.h>

#include <iterator>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <xcb/xcb.h>

namespace cppual { namespace x {

class screen_iterator
{
public:
    typedef screen_iterator                self_type        ;
    typedef ::xcb_setup_t                  handle_type      ;
    typedef ::xcb_setup_t const            const_handle     ;
    typedef ::xcb_screen_iterator_t        value_type       ;
    typedef ::xcb_screen_iterator_t*       pointer          ;
    typedef ::xcb_screen_iterator_t&       reference        ;
    typedef ::xcb_screen_iterator_t const& const_reference  ;
    typedef std::forward_iterator_tag      iterator_category;

    constexpr reference operator *  () noexcept { return  _M_gPos; }
    constexpr pointer   operator -> () noexcept { return &_M_gPos; }

    constexpr
    screen_iterator () noexcept
    : _M_gPos { }
    { }

    inline
    screen_iterator (const_handle& setup) noexcept
    : _M_gPos (::xcb_setup_roots_iterator (&setup))
    { }

    constexpr
    screen_iterator (const_reference ref) noexcept
    : _M_gPos (ref)
    { }

    self_type& operator ++ ()
    {
        ::xcb_screen_next (&_M_gPos);
        return *this;
    }

    self_type operator ++ (int)
    {
        self_type tmp (*this);
        ::xcb_screen_next (&_M_gPos);
        return tmp;
    }

    constexpr reference pos () noexcept
    { return _M_gPos; }

    constexpr const_reference pos () const noexcept
    { return _M_gPos; }

    constexpr bool operator == (self_type const& other) const noexcept
    { return _M_gPos.index == other._M_gPos.index; }

    constexpr bool operator != (self_type const& other) const noexcept
    { return !(*this == other); }

    constexpr bool operator > (self_type const& other) const noexcept
    { return _M_gPos.index > other._M_gPos.index; }

    constexpr bool operator >= (self_type const& other) const noexcept
    { return _M_gPos.index >= other._M_gPos.index; }

    constexpr bool operator < (self_type const& other) const noexcept
    { return _M_gPos.index < other._M_gPos.index; }

    constexpr bool operator <= (self_type const& other) const noexcept
    { return _M_gPos.index <= other._M_gPos.index; }

private:
    value_type _M_gPos;
};

} } // namespace x

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_ITERATOR_H_
