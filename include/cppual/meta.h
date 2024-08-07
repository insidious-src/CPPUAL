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

#ifndef CPPUAL_META_PROGRAMMING_H_
#define CPPUAL_META_PROGRAMMING_H_
#ifdef __cplusplus

#include <cppual/decl.h>

#include <type_traits>
#include <tuple>
//#include <string>

namespace cppual {

/// overload member function with the same name but specific arguments
template <typename... Args, typename C, typename R>
constexpr auto overload (R (C::* fn)(Args...)) noexcept -> decltype (fn)
{ return fn; }

/// overload const member function with the same name but specific arguments
template <typename... Args, typename C, typename R>
constexpr auto overload (R (C::* fn)(Args...) const) noexcept -> decltype (fn)
{ return fn; }

// ===================================================

template <class... Args>
struct type_list
{
    template <std::size_t N>
    using type = typename std::tuple_element_t<N, std::tuple<Args...>>;
};

// ===================================================

template <typename T, typename Tuple, std::size_t I = 0>
struct tuple_ref_index;

template <typename T, typename Base, typename... Tail, std::size_t I>
struct tuple_ref_index<T, std::tuple<Base, Tail...>, I>
    : std::conditional_t<std::is_base_of_v<Base, T> || std::is_same_v<Base, T>,
                         std::integral_constant<std::size_t, I>,
                         tuple_ref_index<T, std::tuple<Tail...>, I + 1>
                         >
{ };

/// get base class from derived using a list of bases
template <typename Derived, typename... Bases>
using base_from_derived =
        typename std::tuple_element_t<tuple_ref_index<Derived, std::tuple<Bases...>>::value,
                                                      std::tuple<Bases...>>;

// ===================================================

template <typename> struct is_char_helper : public std::false_type { };
template <> struct is_char_helper<char>   : public std::true_type  { };
template <> struct is_char_helper<uchar>  : public std::true_type  { };
template <> struct is_char_helper<char16> : public std::true_type  { };
template <> struct is_char_helper<char32> : public std::true_type  { };
template <> struct is_char_helper<wchar>  : public std::true_type  { };

/// is char type (char, uchar, char16, char32, wchar)
template <typename T>
struct is_char : public is_char_helper<T>
{ };

/// is char type (char, uchar, char16, char32, wchar) -> value
template <typename T>
inline constexpr auto const is_char_v = is_char<T>::value;

// ====================================================

/// is bool type
template <typename> struct is_bool : std::false_type { };
template <> struct is_bool<bool>   : std::true_type  { };

/// is bool type -> value
template <typename T>
inline constexpr auto const is_bool_v = is_bool<T>::value;

// ====================================================

/// is integer type
template <typename> struct is_integer  : std::false_type { };
template <> struct is_integer<ushort>  : std::true_type  { };
template <> struct is_integer<uint>    : std::true_type  { };
template <> struct is_integer<ulong>   : std::true_type  { };
template <> struct is_integer<ulong64> : std::true_type  { };
template <> struct is_integer<short>   : std::true_type  { };
template <> struct is_integer<int>     : std::true_type  { };
template <> struct is_integer<long>    : std::true_type  { };
template <> struct is_integer<long64>  : std::true_type  { };

/// is integer type -> value
template <typename T>
inline constexpr auto const is_integer_v = is_integer<T>::value;

// ====================================================

/// is float type
template <typename> struct is_float  : std::false_type { };
template <> struct is_float<float>   : std::true_type  { };
template <> struct is_float<double>  : std::true_type  { };
template <> struct is_float<ldouble> : std::true_type  { };

/// is float type -> value
template <typename T>
inline constexpr auto const is_float_v = is_float<T>::value;

// ====================================================

template <typename>
struct member_function_to_static
{ };

template <typename T, typename C, typename... Args>
struct member_function_to_static <T(C::*)(Args...) const>
{ using type = T(*)(Args...); };

template <typename T, typename C, typename... Args>
struct member_function_to_static <T(C::*)(Args...)>
{ using type = T(*)(Args...); };

template <typename T>
using member_function_to_static_t = typename member_function_to_static<T>::type;

// ====================================================

/// arg all template arguments of the same type
template <typename...>
struct are_of_same_type : std::true_type
{ };

/// arg all template arguments of the same type
template <typename T, typename R, typename... Ts>
struct are_of_same_type <T, R, Ts...> : std::false_type
{ };

/// arg all template arguments of the same type
template <typename T, typename... Ts>
struct are_of_same_type <T, T, Ts...> : are_of_same_type<T, Ts...>
{ };

/// arg all template arguments of the same type -> value
template <typename... T>
inline constexpr auto const are_of_same_type_v = are_of_same_type<T...>::value;

// ====================================================

/// are there any references in the template argument list
template <typename...>
struct are_any_references : std::false_type
{ };

/// are there any references in the template argument list
template <typename T, typename... Ts>
struct are_any_references<T, Ts...>
        : std::integral_constant<bool, std::is_reference<typename std::remove_cv<T>::type>{} ||
                                                         are_any_references<T, Ts...>{}>
{ };

/// are there any references in the template argument list -> value
template <typename... T>
inline constexpr auto const are_any_references_v = are_any_references<T...>::value;

// ====================================================

/// are there any objects in the template argument list
template <typename...>
struct are_any_objects : std::false_type
{ };

/// are there any objects in the template argument list
template <typename T, typename... Ts>
struct are_any_objects<T, Ts...>
        : std::integral_constant<bool, (std::is_class<typename std::remove_pointer<T>::type>{} &&
                                       !std::is_pointer<T>{}) ||
                                        are_any_objects<T, Ts...>{}
                                 >
{ };

/// are there any objects in the template argument list -> value
template <typename... T>
inline constexpr auto const are_any_objects_v = are_any_objects<T...>::value;

// ====================================================

/// char string constexpr hash (ex. usage in switch cases or as a hash)
template <typename T, typename = typename std::enable_if_t<is_char_v<T>>>
constexpr unsigned constexpr_hash (T const* input)
{
    return *input ? static_cast<uint>(*input) + 33 * constexpr_hash(input + 1) : 5381;
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_META_PROGRAMMING_H_
