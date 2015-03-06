/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <type_traits>
#include <cppual/types.h>

namespace cppual { namespace Graphics {

class Connection
{
public:
	typedef void* pointer;

	inline    Connection () noexcept = default;
	constexpr Connection (pointer display) noexcept : m_handle (display) { }
	constexpr Connection (std::nullptr_t ) noexcept : m_handle ()        { }

	constexpr operator pointer () const noexcept
	{ return m_handle; }

	template <typename T>
	constexpr typename std::remove_pointer<T>::type* get () const noexcept
	{ return static_cast<typename std::remove_pointer<T>::type*> (m_handle); }

	friend
	constexpr bool operator == (Connection const&, Connection const&) noexcept;

private:
	pointer m_handle;
};

constexpr bool operator == (Connection const& conn1, Connection const& conn2) noexcept
{ return conn1.m_handle == conn2.m_handle; }

constexpr bool operator != (Connection const& conn1, Connection const& conn2) noexcept
{ return !(conn1 == conn2); }

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

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_PLATFORM_DETAILS_H_
