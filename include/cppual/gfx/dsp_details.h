/*
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

#ifndef CPPUAL_GFX_PLATFORM_DETAILS_H_
#define CPPUAL_GFX_PLATFORM_DETAILS_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/resource.h>

#include <type_traits>

namespace cppual { namespace Graphics {

typedef Handle Connection;

// =========================================================

class Element
{
public:
    typedef uptr  value_type;
    typedef void* pointer;

    Element () noexcept = default;

    template <typename T>
    constexpr T get () const noexcept
    { return static_cast<T> (reinterpret_cast<pointer> (m_handle)); }

    Element (pointer p) noexcept
    : m_handle (reinterpret_cast<value_type> (p))
    { }

    constexpr Element (value_type val) noexcept
    : m_handle (val)
    { }

    constexpr Element (std::nullptr_t) noexcept
    : m_handle ()
    { }

    constexpr operator value_type () const noexcept
    { return m_handle; }

    Element& operator = (std::nullptr_t) noexcept
    { m_handle = 0; return *this; }

    friend
    constexpr bool operator == (Element const&, Element const&) noexcept;

private:
    value_type m_handle;
};

// =========================================================

template <>
constexpr u32 Element::get<u32> () const noexcept
{ return static_cast<u32> (m_handle); }

template <>
constexpr int32 Element::get<int32> () const noexcept
{ return static_cast<int32> (m_handle); }

template <>
constexpr ulong_t Element::get<ulong_t> () const noexcept
{ return static_cast<ulong_t> (m_handle); }

constexpr bool operator == (Element const& elem1, Element const& elem2) noexcept
{ return elem1.m_handle == elem2.m_handle; }

constexpr bool operator != (Element const& elem1, Element const& elem2) noexcept
{ return !(elem1 == elem2); }

// =========================================================

static_assert (std::is_pod<Element>::value, "Graphics::Element is not POD");

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_PLATFORM_DETAILS_H_
