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

#ifndef CPPUAL_META_PROGRAMMING_H_
#define CPPUAL_META_PROGRAMMING_H_
#ifdef __cplusplus

#include <type_traits>
#include <cppual/decl.h>

namespace cppual {

template <typename... Args>
struct Select
{
    template <typename C, typename R>
    static constexpr auto Overload(R (C::* fn)(Args...)) noexcept -> decltype(fn)
    { return fn; }
};

// ===================================================

template <typename> struct is_char_helper : public std::false_type { };
template <> struct is_char_helper<char>   : public std::true_type  { };
template <> struct is_char_helper<uchar>  : public std::true_type  { };
template <> struct is_char_helper<char16> : public std::true_type  { };
template <> struct is_char_helper<char32> : public std::true_type  { };
template <> struct is_char_helper<wchar>  : public std::true_type  { };

template <typename T>
struct is_char : std::integral_constant<bool, (is_char_helper<typename
                                               std::remove_cv<T>::type>::value)>
{ };

// ====================================================

template <typename>
struct is_bool : std::false_type
{ };

template <>
struct is_bool<bool> : std::true_type
{ };

// ====================================================

template <typename>
struct is_integer : std:: false_type
{ };

template <>
struct is_integer<ushort> : std::true_type
{ };

template <>
struct is_integer<uint> : std::true_type
{ };

template <>
struct is_integer<ulong> : std::true_type
{ };

template <>
struct is_integer<ulong64> : std::true_type
{ };

template <>
struct is_integer<short> : std::true_type
{ };

template <>
struct is_integer<int> : std::true_type
{ };

template <>
struct is_integer<long> : std::true_type
{ };

template <>
struct is_integer<long64> : std::true_type
{ };

// ====================================================

template <typename T>
struct member_function_to_static
{ static_assert (std::is_same<T, T>::value, "template parameter T is not member function"); };

template <typename T, typename Object, typename... Args>
struct member_function_to_static <T(Object::*)(Args...) const>
{ using type = T(*)(Args...); };

template <typename T, typename Object, typename... Args>
struct member_function_to_static <T(Object::*)(Args...)>
{ using type = T(*)(Args...); };

} // cppual

#endif // __cplusplus
#endif // CPPUAL_META_PROGRAMMING_H_
