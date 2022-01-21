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

#include <cppual/decl.h>

#include <type_traits>
#include <tuple>
#include <string>

namespace cppual {

template <typename... Args>
struct Select
{
    template <typename C, typename R>
    static constexpr auto Overload(R (C::* fn)(Args...)) noexcept -> decltype(fn)
    { return fn; }
};

// ===================================================

template <class... Args>
struct type_list
{
    template <std::size_t N>
    using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
};

// ===================================================

template <typename T, typename Tuple, std::size_t I = 0>
struct tuple_ref_index;

template <typename T, typename Base, typename... Tail, std::size_t I>
struct tuple_ref_index<T, std::tuple<Base, Tail...>, I>
    : std::conditional<std::is_base_of<Base, T>::value || std::is_same<Base, T>::value,
                       std::integral_constant<std::size_t, I>,
                       tuple_ref_index<T, std::tuple<Tail...>, I+1>
                       >::type
{ };

template <typename Derived, typename... Bases>
using base_from_derived =
        typename std::tuple_element<tuple_ref_index<Derived, std::tuple<Bases...>>::value,
                                                    std::tuple<Bases...>>::type;

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
struct is_integer : std::false_type
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

template <typename T, typename Allocator = typename T::allocator_type>
struct is_string : std::false_type
{ };

template <typename Allocator>
struct is_string<std::basic_string<char, std::char_traits<char>, Allocator>> : std::true_type
{ };

template <typename Allocator>
struct is_string<std::basic_string<char16, std::char_traits<char16>, Allocator>> : std::true_type
{ };

template <typename Allocator>
struct is_string<std::basic_string<char32, std::char_traits<char32>, Allocator>> : std::true_type
{ };

template <typename Allocator>
struct is_string<std::basic_string<wchar, std::char_traits<wchar>, Allocator>> : std::true_type
{ };

// ====================================================

template <typename T>
struct member_function_to_static
{ static_assert (std::is_same<T, T>::value, "template parameter T is not a member function"); };

template <typename T, typename Object, typename... Args>
struct member_function_to_static <T(Object::*)(Args...) const>
{ using type = T(*)(Args...); };

template <typename T, typename Object, typename... Args>
struct member_function_to_static <T(Object::*)(Args...)>
{ using type = T(*)(Args...); };

// ====================================================

template <typename...>
struct are_any_references : std::false_type
{ };

template <typename T, typename... TT>
struct are_any_references<T, TT...>
        : std::integral_constant<bool, std::is_reference<typename std::remove_cv<T>::type>{} ||
                                                         are_any_references<T, TT...>{}>
{ };

// ====================================================

template <typename...>
struct are_any_objects : std::false_type
{ };

template <typename T, typename... TT>
struct are_any_objects<T, TT...>
        : std::integral_constant<bool, (std::is_class<typename std::remove_pointer<T>::type>{} &&
                                       !std::is_pointer<T>{}) ||
                                        are_any_objects<T, TT...>{}
                                 >
{ };

// ====================================================

template <typename T, typename = typename std::enable_if<is_char<T>::value>::type>
constexpr unsigned const_hash(T const* input)
{
    return *input ? static_cast<uint>(*input) + 33 * const_hash(input + 1) : 5381;
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_META_PROGRAMMING_H_
