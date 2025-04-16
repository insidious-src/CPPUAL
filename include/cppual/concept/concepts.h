/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
 *
 * This program is free software: you can redistribute it &&/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, ||
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY || FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_CONCEPTS_H_
#define CPPUAL_CONCEPTS_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/types.h>

#include <tuple>
#include <utility>
#include <variant>
#include <concepts>
#include <iterator>
#include <algorithm>
#include <functional>
#include <type_traits>

namespace cppual {

// ====================================================

//! is bool type
template <typename> struct is_bool : std::false_type { };
template <> struct is_bool<bool>   : std::true_type  { };

//! is bool value
template <typename T>
inline constexpr cbool is_bool_v = is_bool<T>::value;

// ===================================================

template <typename> struct is_char_helper  : public std::false_type { };
template <> struct is_char_helper<char>    : public std::true_type  { };
template <> struct is_char_helper<cchar>   : public std::true_type  { };
template <> struct is_char_helper<uchar>   : public std::true_type  { };
template <> struct is_char_helper<cuchar>  : public std::true_type  { };
template <> struct is_char_helper<char8>   : public std::true_type  { };
template <> struct is_char_helper<cchar8>  : public std::true_type  { };
template <> struct is_char_helper<char16>  : public std::true_type  { };
template <> struct is_char_helper<cchar16> : public std::true_type  { };
template <> struct is_char_helper<char32>  : public std::true_type  { };
template <> struct is_char_helper<cchar32> : public std::true_type  { };
template <> struct is_char_helper<wchar>   : public std::true_type  { };
template <> struct is_char_helper<cwchar>  : public std::true_type  { };

//! is char type (char, uchar, char8, char16, char32, wchar)
template <typename T>
struct is_char : public is_char_helper<T>
{ };

//! is char value
template <typename T>
inline constexpr cbool is_char_v = is_char<T>::value;

// ===================================================

template <typename> struct is_stream_helper : public std::false_type { };
template <> struct is_stream_helper<char>   : public std::true_type  { };
template <> struct is_stream_helper<cchar>  : public std::true_type  { };
template <> struct is_stream_helper<uchar>  : public std::true_type  { };
template <> struct is_stream_helper<cuchar> : public std::true_type  { };

//! is char type (char, uchar)
template <typename T>
struct is_stream_char : public is_stream_helper<T>
{ };

//! is char value
template <typename T>
inline constexpr cbool is_stream_char_v = is_stream_char<T>::value;

// ====================================================

//! is integer type
template <typename> struct is_integer  : std::false_type { };
template <> struct is_integer<ushort>  : std::true_type  { };
template <> struct is_integer<short>   : std::true_type  { };
template <> struct is_integer<uint>    : std::true_type  { };
template <> struct is_integer<int>     : std::true_type  { };
template <> struct is_integer<ulong>   : std::true_type  { };
template <> struct is_integer<long>    : std::true_type  { };
template <> struct is_integer<ulong64> : std::true_type  { };
template <> struct is_integer<long64>  : std::true_type  { };

//! is integer value
template <typename T>
inline constexpr cbool is_integer_v = is_integer<T>::value;

// ====================================================

//! is unsigned integer type
template <typename> struct is_signed_integer  : std::false_type { };
template <> struct is_signed_integer<short>  : std::true_type  { };
template <> struct is_signed_integer<int>    : std::true_type  { };
template <> struct is_signed_integer<long>   : std::true_type  { };
template <> struct is_signed_integer<long64> : std::true_type  { };

//! is unsigned integer value
template <typename T>
inline constexpr cbool is_signed_integer_v = is_signed_integer<T>::value;

// ====================================================

//! is unsigned integer type
template <typename> struct is_unsigned_integer  : std::false_type { };
template <> struct is_unsigned_integer<ushort>  : std::true_type  { };
template <> struct is_unsigned_integer<uint>    : std::true_type  { };
template <> struct is_unsigned_integer<ulong>   : std::true_type  { };
template <> struct is_unsigned_integer<ulong64> : std::true_type  { };

//! is unsigned integer value
template <typename T>
inline constexpr cbool is_unsigned_integer_v = is_unsigned_integer<T>::value;

// ====================================================

//! is unsigned  type
template <typename> struct is_unsigned  : std::false_type { };
template <> struct is_unsigned<uchar>   : std::true_type  { };
template <> struct is_unsigned<ushort>  : std::true_type  { };
template <> struct is_unsigned<uint>    : std::true_type  { };
template <> struct is_unsigned<ulong>   : std::true_type  { };
template <> struct is_unsigned<ulong64> : std::true_type  { };

//! is unsigned value
template <typename T>
inline constexpr cbool is_unsigned_v = is_unsigned<T>::value;

// ====================================================

//! is float type
template <typename> struct is_float  : std::false_type { };
template <> struct is_float<float>   : std::true_type  { };
template <> struct is_float<double>  : std::true_type  { };
template <> struct is_float<ldouble> : std::true_type  { };

//! is float value
template <typename T>
inline constexpr cbool is_float_v = is_float<T>::value;

template <typename T>
inline constexpr cbool is_pod_v = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

// ====================================================

template <typename T>
inline constexpr cbool is_compact_forwarded_v = !std::is_class_v<T> && sizeof (T) <= sizeof (uptr);

// ====================================================

template <typename T>
inline constexpr cbool is_ref_v = std::is_reference_v<T>;

template <typename T>
inline constexpr cbool is_cref_v = std::is_const_v<T> && std::is_reference_v<T>;

template <typename T>
inline constexpr cbool is_cvref_v = std::is_const_v    <T> &&
                                    std::is_volatile_v <T> &&
                                    std::is_reference_v<T>;

template <typename T>
inline constexpr cbool is_ptr_v = std::is_pointer_v<T>;

template <typename T>
inline constexpr cbool is_cptr_v = std::is_const_v<T> && std::is_pointer_v<T>;

template <typename T>
inline constexpr cbool is_cvptr_v = std::is_const_v   <T> &&
                                    std::is_volatile_v<T> &&
                                    std::is_pointer_v <T>;

template <typename T>
inline constexpr cbool is_crefptr_v = std::is_const_v    <T> &&
                                      std::is_pointer_v  <T> &&
                                      std::is_reference_v<T>;

template <typename T>
inline constexpr cbool is_cvrefptr_v = std::is_const_v    <T> &&
                                       std::is_volatile_v <T> &&
                                       std::is_pointer_v  <T> &&
                                       std::is_reference_v<T>;

// ====================================================

template <typename T>
using remove_ref_t = std::remove_reference_t<T>;

template <typename T>
using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

template <typename T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <typename T>
using remove_ptr_t = std::remove_pointer_t<T>;

template <typename T>
using remove_cptr_t = std::remove_const_t<std::remove_pointer_t<T>>;

template <typename T>
using remove_cvptr_t = std::remove_cv_t<std::remove_pointer_t<T>>;

template <typename T>
using remove_crefptr_t = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

template <typename T>
using remove_cvrefptr_t = std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

// ====================================================

template <typename>
struct member_function_to_static
{ };

template <class C, typename R, typename... Args>
struct member_function_to_static <R(C::*)(Args...) const>
{ typedef R(* type)(Args...); };

template <class C, typename R, typename... Args>
struct member_function_to_static <R(C::*)(Args...)>
{ typedef R(* type)(Args...); };

template <typename F>
using member_function_to_static_t = member_function_to_static<F>::type;

template <typename F, F MEM_FN>
inline constexpr auto const member_function_to_static_v =
    static_cast<member_function_to_static<F>::type> (MEM_FN);

// ====================================================

//! type property concepts
template <typename T>
concept const_t = std::is_const_v<T>;

template <typename T>
concept volatile_t = std::is_volatile_v<T>;

template <typename T>
concept standard_layout_t = std::is_standard_layout_v<T>;

template <typename T>
concept trivial_t = std::is_trivial_v<T>;

template <typename T>
concept pod_t = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

// ====================================================

//! basic type concepts
template <typename T>
concept fundamental_t = std::is_fundamental_v<T>;

template <typename T>
concept unsigned_t = is_unsigned_v<T>;

template <typename T>
concept integer_t = is_integer_v<T>;

template <typename T>
concept unsigned_integer_t = is_unsigned_integer_v<T>;

template <typename T>
concept signed_integer_t = is_signed_integer_v<T>;

template <typename T>
concept floating_point_t = is_float_v<T>;

template <typename T>
concept number_t = is_integer_v<T> || is_float_v<T>;

template <typename T>
concept char_t = is_char_v<T>;

template <typename T>
concept switch_value_t = is_integer_v<T> || std::is_enum_v<T> || is_char_v<T> || is_stream_char_v<T>;

template <typename T>
concept c_str_t = std::is_same_v<T, char*>;

template <typename T>
concept c_str_const_t = std::is_same_v<T, cchar*>;

template <typename T>
concept c_str8_t = std::is_same_v<T, char8*>;

template <typename T>
concept c_str8_const_t = std::is_same_v<T, cchar8*>;

template <typename T>
concept c_str16_t = std::is_same_v<T, char16*>;

template <typename T>
concept c_str16_const_t = std::is_same_v<T, cchar16*>;

template <typename T>
concept c_str32_t = std::is_same_v<T, char32*>;

template <typename T>
concept c_str32_const_t = std::is_same_v<T, cchar32*>;

template <typename T>
concept c_wstr_t = std::is_same_v<T, wchar*>;

template <typename T>
concept c_wstr_const_t = std::is_same_v<T, cwchar*>;

template <typename T>
concept stream_char_t = is_stream_char_v<T>;

template <typename T>
concept compact_t = !std::is_class_v<T> && sizeof (T) <= sizeof (uptr);

template <typename T>
concept non_void_t = !std::is_void_v<T>;

template <typename T>
concept function_t = std::is_function_v<std::remove_pointer_t<T>> ||
                     std::is_member_function_pointer_v<T>;

template <typename T>
concept static_function_t = std::is_function_v<std::remove_pointer_t<T>>;

template <typename T>
concept member_function_t = std::is_member_function_pointer_v<T>;

template <typename T>
concept non_function_t = !std::is_function_v<std::remove_pointer_t<T>> &&
                         !std::is_member_function_pointer_v<T>;

template <typename T>
concept void_callable_t = requires (T& t) { { t () } -> std::same_as<void>; };

// ====================================================

template <typename T>
concept callable_class_t = std::is_class_v<T> && requires (T&) { &std::decay_t<T>::operator (); };

template <typename T>
concept callable_object_t = requires (T&) { &std::decay_t<T>::operator (); };

//! lambda concepts
template <typename T>
concept lambda_non_capture_t = callable_object_t<T> && std::is_constructible_v<
        member_function_to_static_t<decltype (&std::decay_t<T>::operator())>, T>;

template <typename T>
concept lambda_capture_t = callable_object_t<T> && !std::is_constructible_v<
        member_function_to_static_t<decltype (&std::decay_t<T>::operator())>, T>;

template <typename T>
concept lambda_t = lambda_non_capture_t<T> || lambda_capture_t<T>;

template <typename T>
concept callable_non_capture_object_t = (std::is_class_v<T> &&
                                         requires (T&) { &std::decay_t<T>::operator (); }) ||
                                         lambda_non_capture_t<T>;

template <typename T>
concept callable_t = lambda_non_capture_t<T> ||
lambda_capture_t <T>                         ||
member_function_t<T>                         ||
static_function_t<T>                         ||
callable_object_t<T>;

//! type category concepts
template <typename T>
concept arithmetic_t = std::is_arithmetic_v<T>;

template <typename T>
concept scalar_t = std::is_scalar_v<T>;

template <typename T>
concept compound_t = std::is_compound_v<T>;

template <typename T>
concept reference_t = std::is_reference_v<T>;

template <typename T>
concept lvalue_reference_t = std::is_lvalue_reference_v<T>;

template <typename T>
concept rvalue_reference_t = std::is_rvalue_reference_v<T>;

template <typename T>
concept enum_t = std::is_enum_v<T>;

template <typename T>
concept array_t = std::is_array_v<T>;

template <typename T>
concept union_t = std::is_union_v<T>;

template <typename T>
concept null_ptr_t = std::is_null_pointer_v<T>;

template <typename T>
concept object_t = std::is_object_v<T>;

template <typename T>
concept class_t = std::is_class_v<T> && !std::is_const_v<T>;

template <typename T>
concept class_or_union_t = class_t<T> || union_t<T>;

template <typename T>
concept abstract_t = std::is_abstract_v<T>;

//! pointer concepts
template <typename T>
concept ptr_t = std::is_pointer_v<T>;

template <typename T>
concept member_ptr_t = std::is_member_pointer_v<T>;

template <typename T>
concept member_object_ptr_t = std::is_member_object_pointer_v<T>;

template <typename T>
concept void_ptr_t =  std::is_pointer_v<T> && std::is_void_v<std::remove_pointer_t<T>>;

template <typename T>
concept pair_t = requires
{
    typename T::first_type ;
    typename T::second_type;
};

template <typename P, typename K, typename FN>
concept fn_pair_t = pair_t<P>                             &&
                    switch_value_t<remove_cptr_t<K>>      &&
                   (std::is_function_v<FN>                ||
                    std::is_member_function_pointer_v<FN> ||
                    callable_class_t<FN>);


template <typename P, typename K, typename FN>
concept member_fn_pair_t = std::is_same_v<P, std::pair<K, FN>> &&
                           switch_value_t<   remove_cptr_t<K>> &&
                           std::is_member_function_pointer_v<FN>;

template <typename Tuple, typename... Args>
concept tuple_t = std::is_same_v<Tuple, std::tuple<Args...>>;

//! constructibility and destructibility concepts
template <typename T>
concept constructible_t = std::is_constructible_v<T>;

template <typename T>
concept default_constructible_t = std::is_default_constructible_v<T>;

template <typename T>
concept destructible_t = std::is_destructible_v<T>;

template <typename T>
concept trivially_destructible_t = std::is_trivially_destructible_v<T>;

template <typename T>
concept virtually_destructible_t = std::has_virtual_destructor_v<T>;

template <typename T>
concept copyable_movable_t = std::copyable<T> && std::movable<T>;

template <typename T>
concept copyable_t = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

template <typename T>
concept copy_constructible_t = std::is_copy_constructible_v<T>;

template <typename T>
concept copy_assignable_t = std::is_copy_assignable_v<T>;

template <typename T>
concept movable_t = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

template <typename T>
concept move_constructible_t = std::is_move_constructible_v<T>;

template <typename T>
concept move_assignable_t = std::is_move_assignable_v<T>;

template <typename T1, typename T2>
concept non_convertible_t = !std::convertible_to<T1, T2> && !std::convertible_to<T2, T1>;

//! numeric concepts
template <typename T>
concept addable_t       = requires (T a, T b) { { a + b } -> std::convertible_to<T>; };

template <typename T>
concept subtractable_t  = requires (T a, T b) { { a - b } -> std::convertible_to<T>; };

template <typename T>
concept multiplicable_t = requires (T a, T b) { { a * b } -> std::convertible_to<T>; };

template <typename T>
concept divisible_t     = requires (T a, T b) { { a / b } -> std::convertible_to<T>; };

//! range concepts
template <typename T>
concept range_t = std::ranges::range<T>;

template <typename T>
concept sized_range_t = std::ranges::sized_range<T>;

template <typename T>
concept bidirectional_range_t = std::ranges::bidirectional_range<T>;

template <typename T>
concept random_access_range_t = std::ranges::random_access_range<T>;

//! utility concepts
template <typename T>
concept optional_like_t = requires (T t)
{
    { t.has_value () } -> std::convertible_to<bool>;
    { *t } -> std::same_as<typename T::value_type&>;
    { t.value () } -> std::same_as<typename T::value_type&>;
};

template <typename T>
concept variant_like_t = requires
{
    typename std::variant_size<std::remove_cvref_t<T>>::type;
    typename std::variant_alternative_t<0, std::remove_cvref_t<T>>;
};

//! Comparison concepts
template <typename T>
concept equality_comparable_t = std::equality_comparable<T>;

template <typename T>
concept totally_ordered_t = std::totally_ordered<T>;

//! callable concepts
template <typename F, typename... Args>
concept predicate_t = std::regular_invocable<F, Args...> &&
                      std::convertible_to<std::invoke_result_t<F, Args...>, bool>;

template <typename F, typename T>
concept binary_predicate_t = predicate_t<F, T, T>;

template <typename F, typename T>
concept relation_t = binary_predicate_t<F, T>;

//! TODO: replace internal begin/end with std::begin/std::end functions
//! Complex concepts
template <typename T>
concept sortable_t = requires (T& t)
{
    std::begin (t);
    std::end   (t);
    std::sort  (std::begin (t), std::end (t));
};

//! concept for types that can be used as dictionary keys
template<typename T>
concept keyable_t = std::is_copy_constructible_v<T> && std::is_move_constructible_v<T> &&
requires (T const& a, T const& b)
{
    {      a == b       } -> std::convertible_to<bool>;
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

//! container concepts
template <typename T>
concept container_t = requires (T a, const T b)
{
    typename T::value_type;
    typename T::reference;
    typename T::const_reference;
    typename T::iterator;
    typename T::const_iterator;
    typename T::difference_type;
    typename T::size_type;

    { a.begin    () } -> std::same_as<typename T::iterator>;
    { a.end      () } -> std::same_as<typename T::iterator>;
    { b.begin    () } -> std::same_as<typename T::const_iterator>;
    { b.end      () } -> std::same_as<typename T::const_iterator>;
    { a.size     () } -> std::convertible_to<typename T::size_type>;
    { a.max_size () } -> std::convertible_to<typename T::size_type>;
    { a.empty    () } -> std::convertible_to<bool>;
};

template <typename T>
concept reversible_container_t = container_t<T> && requires (T a, T const b)
{
    typename T::reverse_iterator;
    typename T::const_reverse_iterator;

    { a.rbegin () } -> std::same_as<typename T::reverse_iterator>;
    { a.rend   () } -> std::same_as<typename T::reverse_iterator>;
    { b.rbegin () } -> std::same_as<typename T::const_reverse_iterator>;
    { b.rend   () } -> std::same_as<typename T::const_reverse_iterator>;
};

template <typename T>
concept sequence_container_t = container_t<T> && requires (T a, typename T::value_type v)
{
    { a.front     ( ) } -> std::same_as<typename T::reference>;
    { a.back      ( ) } -> std::same_as<typename T::reference>;
    { a.push_back (v) } -> std::same_as<void>;
    { a.pop_back  ( ) } -> std::same_as<void>;
};

// ====================================================

inline namespace type_traits {

template <typename T>
using FundamentalType = std::enable_if_t<std::is_fundamental_v<T>, T>;

template <typename T>
using IntegralType = std::enable_if_t<std::is_integral_v<T>, T>;

template <typename T>
using SignedIntegralType = std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, T>;

template <typename T>
using UnsignedIntegralType = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, T>;

template <typename T>
using FloatingPointType = std::enable_if_t<std::is_floating_point_v<T>, T>;

template <typename T>
using NonVoidType = std::enable_if_t<!std::is_void_v<T>, T>;

template <typename T>
using FunctionType = std::enable_if_t<std::is_function_v<std::remove_pointer_t<T>> ||
                                      std::is_member_function_pointer_v<T>, T>;

template <typename T>
using StaticFunctionType = std::enable_if_t<std::is_function_v<std::remove_pointer_t<T>>, T>;

template <typename T>
using MemberFunctionType = std::enable_if_t<std::is_member_function_pointer_v<T>, T>;

template <typename T>
using LambdaNonCaptureType = std::enable_if_t<std::is_constructible_v
<member_function_to_static_t<decltype(&std::decay_t<T>::operator())>, T>, T>;

template <typename T>
using LambdaCaptureType = std::enable_if_t<!std::is_constructible_v
<member_function_to_static_t<decltype(&std::decay_t<T>::operator())>, T>, T>;

template <typename T>
using CallableType = std::enable_if_t<
 std::is_constructible_v<
member_function_to_static_t<decltype(&std::decay_t<T>::operator())>, T> ||
!std::is_constructible_v<
member_function_to_static_t<decltype(&std::decay_t<T>::operator())>, T>, T>;

// ====================================================

template <typename T>
using ArithmeticType = std::enable_if_t<std::is_arithmetic_v<T>, T>;

template <typename T>
using ScalarType = std::enable_if_t<std::is_scalar_v<T>, T>;

template <typename T>
using CompoundType = std::enable_if_t<std::is_compound_v<T>, T>;

template <typename T>
using ReferenceType = std::enable_if_t<std::is_reference_v<T>, T>;

template <typename T>
using LValueReferenceType = std::enable_if_t<std::is_lvalue_reference_v<T>, T>;

template <typename T>
using RValueReferenceType = std::enable_if_t<std::is_rvalue_reference_v<T>, T>;

template <typename T>
using CharType = std::enable_if_t<is_char_v<T>, T>;

template <typename T>
using EnumType = std::enable_if_t<std::is_enum_v<T>, T>;

template <typename T>
using ArrayType = std::enable_if_t<std::is_array_v<T>, T>;

template <typename T>
using UnionType = std::enable_if_t<std::is_union_v<std::remove_cvref_t<T>>, T>;

template <typename T>
using NullPointer = std::enable_if_t<std::is_null_pointer_v<T>, T>;

template <typename T>
using ObjectType = std::enable_if_t<std::is_object_v<std::remove_cvref_t<T>>, T>;

template <typename T>
using ClassType = std::enable_if_t<std::is_class_v<std::remove_cvref_t<T>>, T>;

template <typename T>
using AbstractType = std::enable_if_t<std::is_abstract_v<T>, T>;

template <typename T>
using PtrType = std::enable_if_t<std::is_pointer_v<T>, T>;

template <typename T>
using MemberPtrType = std::enable_if_t<std::is_member_pointer_v<T>, T>;

template <typename T>
using MemberObjectPtrType = std::enable_if_t<std::is_member_object_pointer_v<T>, T>;

// ====================================================

template <typename T>
using ConstructibleType = std::enable_if_t<std::is_constructible_v<T>, T>;

template <typename T>
using DefaultConstructibleType = std::enable_if_t<std::is_default_constructible_v<T>, T>;

template <typename T>
using DestructibleType = std::enable_if_t<std::is_destructible_v<T>, T>;

template <typename T>
using TriviallyDestructibleType = std::enable_if_t<std::is_trivially_destructible_v<T>, T>;

template <typename T>
using VirtuallyDestructibleType = std::enable_if_t<std::has_virtual_destructor_v<T>, T>;

template <typename T>
using CopyableMovable = std::enable_if_t<
std::is_copy_constructible_v<T> &&
std::is_copy_assignable_v   <T> &&
std::is_move_constructible_v<T> &&
std::is_move_assignable_v   <T>, T
>;

template <typename T>
using CopyableType = typename
std::enable_if_t<std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>, T>;

template <typename T>
using CopyConstructibleType = std::enable_if_t<std::is_copy_constructible_v<T>, T>;

template <typename T>
using CopyAssignableType = std::enable_if_t<std::is_copy_assignable_v<T>, T>;

template <typename T>
using MovableType = typename
std::enable_if_t<std::is_move_constructible_v<T> && std::is_move_assignable_v<T>, T>;

template <typename T>
using MoveConstructibleType = std::enable_if_t<std::is_move_constructible_v<T>, T>;

template <typename T>
using MoveAssignableType = std::enable_if_t<std::is_move_assignable_v<T>, T>;

// ====================================================

template <typename T>
using ConstType = std::enable_if_t<std::is_const_v<T>, T>;

template <typename T>
using VolatileType = std::enable_if_t<std::is_volatile_v<T>, T>;

template <typename T>
using StandartLayoutType = std::enable_if_t<std::is_standard_layout_v<T>, T>;

template <typename T>
using TrivialType = std::enable_if_t<std::is_trivial_v<T>, T>;

template <typename T>
using PODType = std::enable_if_t<std::is_pod_v<T>, T>;

// ====================================================

//template <typename T>
//using Container = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using ReversibleContainer = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using SequenceContainer = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using AssociativeContainer = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using UnorderedAssociativeContainer = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using AllocatorAwareContainer = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Erasable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using DefaultInsertable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using CopyInsertable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using MoveInsertable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using EmplaceConstructible = std::enable_if_t<std::is_standard_layout_v<T>, T>;

// ====================================================

//template <typename T>
//using EqualityComparable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using LessThanComparable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Swappable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using ValueSwappable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Sortable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//template <typename T>
//using Mergeable =     std::enable_if_t<std::is_standard_layout_v<T>, T>;

} } // cppual

#endif // __cplusplus
#endif // CPPUAL_CONCEPTS_H_
