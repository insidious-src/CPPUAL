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

#include <cppual/decl>
#include <cppual/types>

#include <type_traits>
#include <functional>
#include <algorithm>
#include <iterator>
#include <concepts>
#include <utility>
#include <variant>
#include <tuple>

namespace cppual {

// ====================================================

//! max array size for function's capture lambda storage
template <std::size_t SZ = 32>
consteval decltype (SZ) lambda_calc_size () noexcept
{
    return SZ <=  32 ?  32 :
           SZ <=  48 ?  48 :
           SZ <=  64 ?  64 :
           SZ <=  96 ?  96 :
           SZ <= 128 ? 128 :
           SZ ;
}

template <std::size_t SZ = lambda_calc_size ()>
inline constexpr static auto const max_capture_size_v = lambda_calc_size<SZ> ();
inline constexpr static auto const max_default_size_v = lambda_calc_size     ();

// ====================================================

//! void concepts
template <typename T>
concept non_void = !std::is_void_v<T>;

// ====================================================

//! is bool type
template <non_void> struct is_bool : std::false_type { };
template <> struct is_bool<bool>   : std::true_type  { };

//! is bool value
template <non_void T>
inline constexpr cbool is_bool_v = is_bool<T>::value;

// ===================================================

template <non_void> struct is_char_helper : public std::false_type { };
template <> struct is_char_helper<char>     : public std::true_type  { };
template <> struct is_char_helper<cchar>    : public std::true_type  { };
template <> struct is_char_helper<uchar>    : public std::true_type  { };
template <> struct is_char_helper<cuchar>   : public std::true_type  { };
template <> struct is_char_helper<char8>    : public std::true_type  { };
template <> struct is_char_helper<cchar8>   : public std::true_type  { };
template <> struct is_char_helper<char16>   : public std::true_type  { };
template <> struct is_char_helper<cchar16>  : public std::true_type  { };
template <> struct is_char_helper<char32>   : public std::true_type  { };
template <> struct is_char_helper<cchar32>  : public std::true_type  { };
template <> struct is_char_helper<wchar>    : public std::true_type  { };
template <> struct is_char_helper<cwchar>   : public std::true_type  { };

//! is char type (char, uchar, char8, char16, char32, wchar)
template <non_void T>
struct is_char : public is_char_helper<T>
{ };

//! is char value
template <non_void T>
inline constexpr cbool is_char_v = is_char<T>::value;

// ===================================================

template <non_void> struct is_stream_helper : public std::false_type { };
template <> struct is_stream_helper<char>     : public std::true_type  { };
template <> struct is_stream_helper<cchar>    : public std::true_type  { };
template <> struct is_stream_helper<uchar>    : public std::true_type  { };
template <> struct is_stream_helper<cuchar>   : public std::true_type  { };

//! is char type (char, uchar)
template <non_void T>
struct is_stream_char : public is_stream_helper<T>
{ };

//! is char value
template <non_void T>
inline constexpr cbool is_stream_char_v = is_stream_char<T>::value;

// ====================================================

//! is integer type
template <non_void> struct is_integer : std::false_type { };
template <> struct is_integer<ushort>   : std::true_type  { };
template <> struct is_integer<short>    : std::true_type  { };
template <> struct is_integer<uint>     : std::true_type  { };
template <> struct is_integer<int>      : std::true_type  { };
template <> struct is_integer<ulong>    : std::true_type  { };
template <> struct is_integer<long>     : std::true_type  { };
template <> struct is_integer<ulong64>  : std::true_type  { };
template <> struct is_integer<long64>   : std::true_type  { };

//! is integer value
template <non_void T>
inline constexpr cbool is_integer_v = is_integer<T>::value;

// ====================================================

//! is unsigned integer type
template <non_void> struct is_signed_integer : std::false_type { };
template <> struct is_signed_integer<short>    : std::true_type  { };
template <> struct is_signed_integer<int>      : std::true_type  { };
template <> struct is_signed_integer<long>     : std::true_type  { };
template <> struct is_signed_integer<long64>   : std::true_type  { };

//! is unsigned integer value
template <non_void T>
inline constexpr cbool is_signed_integer_v = is_signed_integer<T>::value;

// ====================================================

//! is unsigned integer type
template <non_void> struct is_unsigned_integer   : std::false_type { };
template <> struct is_unsigned_integer<ushort>   : std::true_type  { };
template <> struct is_unsigned_integer<uint>     : std::true_type  { };
template <> struct is_unsigned_integer<ulong>    : std::true_type  { };
template <> struct is_unsigned_integer<ulong64>  : std::true_type  { };

//! is unsigned integer value
template <non_void T>
inline constexpr cbool is_unsigned_integer_v = is_unsigned_integer<T>::value;

// ====================================================

//! is unsigned  type
template <non_void> struct is_unsigned_type   : std::false_type { };
template <> struct is_unsigned_type<uchar>    : std::true_type  { };
template <> struct is_unsigned_type<ushort>   : std::true_type  { };
template <> struct is_unsigned_type<uint>     : std::true_type  { };
template <> struct is_unsigned_type<ulong>    : std::true_type  { };
template <> struct is_unsigned_type<ulong64>  : std::true_type  { };

//! is unsigned value
template <non_void T>
inline constexpr cbool is_unsigned_v = is_unsigned_type<T>::value;

// ====================================================

//! is float type
template <non_void> struct is_floating_point : std::false_type { };
template <> struct is_floating_point<float>    : std::true_type  { };
template <> struct is_floating_point<double>   : std::true_type  { };
template <> struct is_floating_point<ldouble>  : std::true_type  { };

//! is float value
template <non_void T>
inline constexpr cbool is_float_v = is_floating_point<T>::value;

template <non_void T>
inline constexpr cbool is_pod_v = std::is_trivial_v<T> && std::is_standard_layout_v<T>;

// ====================================================

template <non_void T>
inline constexpr cbool is_ref_v = std::is_reference_v<T>;

template <non_void T>
inline constexpr cbool is_cref_v = std::is_const_v<T> && std::is_reference_v<T>;

template <non_void T>
inline constexpr cbool is_cvref_v = std::is_const_v    <T> &&
                                    std::is_volatile_v <T> &&
                                    std::is_reference_v<T>;

template <non_void T>
inline constexpr cbool is_ptr_v = std::is_pointer_v<T>;

template <non_void T>
inline constexpr cbool is_cptr_v = std::is_const_v<T> && std::is_pointer_v<T>;

template <non_void T>
inline constexpr cbool is_cvptr_v = std::is_const_v   <T> &&
                                    std::is_volatile_v<T> &&
                                    std::is_pointer_v <T>;

template <non_void T>
inline constexpr cbool is_crefptr_v = std::is_const_v    <T> &&
                                      std::is_pointer_v  <T> &&
                                      std::is_reference_v<T>;

template <non_void T>
inline constexpr cbool is_cvrefptr_v = std::is_const_v    <T> &&
                                       std::is_volatile_v <T> &&
                                       std::is_pointer_v  <T> &&
                                       std::is_reference_v<T>;

// ====================================================
// Delegate Concept
// ====================================================

template <non_void>
struct is_const_function : std::false_type
{ };

template <typename R, non_void... Args>
struct is_const_function <R(Args...) const> : std::true_type
{ };

template <typename R, non_void... Args>
struct is_const_function <R(Args...) const volatile> : std::true_type
{ };

template <non_void>
struct is_non_const_function : std::false_type
{ };

template <typename R, non_void... Args>
struct is_non_const_function <R(Args...)> : std::true_type
{ };

template <typename R, non_void... Args>
struct is_non_const_function <R(Args...) volatile> : std::true_type
{ };

template <non_void T>
inline constexpr static cbool is_const_fn_sig_v = is_const_function<T>::value;

template <non_void T>
inline constexpr static cbool is_non_const_fn_sig_v = is_non_const_function<T>::value;

template <non_void T>
inline constexpr static cbool is_fn_sig_v = is_non_const_function<T>::value ||
                                            is_const_function    <T>::value;

template <typename T>
concept non_const_fn_sig = is_non_const_fn_sig_v<T>;

template <typename T>
concept const_fn_sig = is_const_fn_sig_v<T>;

template <typename T>
concept fn_sig = is_const_fn_sig_v<T> || is_non_const_fn_sig_v<T>;

// ====================================================

template <typename, typename>
struct fn_signatures_match;

template <typename R1, typename... Args1, typename R2, typename... Args2>
struct fn_signatures_match <R1(Args1...), R2(Args2...)>
{
    inline constexpr static cbool value = std::is_same_v<R1, R2>                 &&
                                          sizeof... (Args1) == sizeof... (Args2) &&
                                          (std::is_same_v<Args1, Args2> && ...);
};

template <typename U1, typename U2>
inline constexpr static cbool fn_signatures_match_v = fn_signatures_match<U1, U2>::value;

// ====================================================

template <non_void, std::size_t = max_default_size_v>
class function;

// ====================================================

template <non_void F>
struct is_functional_helper : public std::false_type
{ typedef F type; };

template <non_void T, std::size_t BYTES>
struct is_functional_helper <function<T, BYTES>> : public std::true_type
{ typedef function<T, BYTES> type; };

template <non_void T>
struct is_functional_helper <std::function<T>> : public std::true_type
{ typedef std::function<T> type; };

template <non_void F>
using is_functional = is_functional_helper<F>::type;

/// is functional -> value
template <non_void F>
inline constexpr cbool is_functional_v = is_functional_helper<F>::value;

/// delegate concept
template <typename T>
concept void_callable = requires (T& t) { { t () } -> std::same_as<void>; };

template <typename F>
concept functional = is_functional_v<F>;

template <typename F>
concept void_functional = functional<F> && void_callable<F>;

// ====================================================

template <non_void T>
struct is_copyable
{
    typedef T type;
    inline constexpr static cbool value = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

    static_assert (value, "T is NOT copyable!");
};

template <non_void T>
using is_copyable_t = is_copyable<T>::type;

template <non_void T>
inline constexpr static cbool is_copyable_v = is_copyable<T>::value;

template <non_void T>
struct is_movable
{
    typedef T type;
    inline constexpr static cbool value = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

    static_assert (value, "T is NOT movable!");
};

template <non_void T>
using is_movable_t = is_movable<T>::type;

template <non_void T>
inline constexpr static cbool is_movable_v = is_movable<T>::value;

// ====================================================

template <non_void T>
using remove_ref_t = std::remove_reference_t<T>;

template <non_void T>
using remove_cref_t = std::remove_const_t<std::remove_reference_t<T>>;

template <non_void T>
using remove_cvref_t = std::remove_cv_t<std::remove_reference_t<T>>;

template <non_void T>
using remove_ptr_t = std::remove_pointer_t<T>;

template <non_void T>
using remove_cptr_t = std::remove_const_t<std::remove_pointer_t<T>>;

template <non_void T>
using remove_cvptr_t = std::remove_cv_t<std::remove_pointer_t<T>>;

template <non_void T>
using remove_crefptr_t = std::remove_const_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

template <non_void T>
using remove_cvrefptr_t = std::remove_cv_t<std::remove_pointer_t<std::remove_reference_t<T>>>;

// =========================================================

template <typename T, non_void U = remove_cvref_t<T>>
struct compact_forwarded : std::conditional<(std::is_class_v<U> || std::is_union_v<U>) && !std::is_pointer_v<T>,
std::conditional_t<is_cref_v<T> && std::is_copy_constructible_v<U>,
                   U const&,
                   std::conditional_t<
                   is_ref_v<T> && std::is_move_constructible_v<U>,
                   U&&,
                   std::conditional_t<(sizeof (U) < sizeof (uptr)), U, U&>
                                      >
                   >,
                   std::conditional_t<std::is_fundamental_v<U>, U, U&&>
>
{ };

template <typename T>
using compact_forwarded_t = compact_forwarded<T>::type;

template <non_void T>
inline constexpr cbool is_compact_forwarded_v = !std::is_class_v<T> &&
                                                !std::is_union_v<T> &&
                                                sizeof (T) <= sizeof (uptr);

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
inline constexpr cbool member_function_to_static_v =
static_cast<member_function_to_static<F>::type> (MEM_FN);

// ====================================================

//! type property concepts
template <typename T>
concept is_const = std::is_const_v<T>;

template <typename T>
concept is_volatile = std::is_volatile_v<T>;

template <typename T>
concept standard_layout = std::is_standard_layout_v<T>;

template <typename T>
concept trivial = std::is_trivial_v<T>;

template <typename T>
concept pod = trivial<T> && standard_layout<T>;

// ====================================================

//! basic type concepts
template <typename T>
concept is_fundamental = std::is_fundamental_v<T>;

template <typename T>
concept is_unsigned = is_unsigned_v<T>;

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
concept pointer_t = std::is_pointer_v<T>;

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
concept function_t = std::is_function_v<std::remove_pointer_t<T>> ||
                     std::is_member_function_pointer_v<T>;

template <typename T>
concept static_function_t = std::is_function_v<std::remove_pointer_t<T>>;

template <typename T>
concept member_function_t = std::is_member_function_pointer_v<T>;

template <typename T>
concept non_function_t = !std::is_function_v<std::remove_pointer_t<T>> &&
                         !std::is_member_function_pointer_v<T>;

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

// ====================================================

template <callable_object_t C>
inline constexpr auto callable_operator_v = &std::decay_t<C>::operator ();

template <callable_object_t C, typename... Args>
using callable_return_t = std::invoke_result_t<decltype (&std::decay_t<C>::operator ()), Args...>;

// ====================================================

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
concept void_or_class_t = std::is_void_v<T> || std::is_class_v<T>;

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

    { std::declval<T>().first  } -> std::convertible_to<typename T::first_type >;
    { std::declval<T>().second } -> std::convertible_to<typename T::second_type>;
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

template <typename P, typename K, typename FN>
concept functional_pair_t = pair_t<P> && switch_value_t<remove_cptr_t<K>> && functional<FN>;

template <typename P, typename K, typename FN>
concept functional_str_pair_t = pair_t<P> && is_char_v<remove_cptr_t<K>> && functional<FN>;

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

// ====================================================

template <typename T>
class is_input_iterator : public
        std::conditional_t<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::input_iterator_tag>::value, std::true_type, std::false_type>
{ };

template <typename T>
inline constexpr cbool is_input_iterator_v = is_input_iterator<T>::value;

template <typename T>
concept input_iterator_t = is_input_iterator_v<T>;

// ====================================================

template <typename T>
class is_output_iterator : public
        std::conditional_t<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::output_iterator_tag>::value, std::true_type, std::false_type>
{ };

template <typename T>
inline constexpr cbool is_output_iterator_v = is_output_iterator<T>::value;

template <typename T>
concept output_iterator_t = is_output_iterator_v<T>;

// ====================================================

template <typename T>
class is_forward_iterator : public
        std::conditional_t<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::forward_iterator_tag>::value, std::true_type, std::false_type>
{ };

template <typename T>
inline constexpr cbool is_forward_iterator_v = is_forward_iterator<T>::value;

template <typename T>
concept forward_iterator_t = is_forward_iterator_v<T>;

// ====================================================

template <typename T>
class is_bidirectional_iterator : public
        std::conditional_t<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::bidirectional_iterator_tag>::value, std::true_type, std::false_type>
{ };

template <typename T>
inline constexpr cbool is_bidirectional_iterator_v = is_bidirectional_iterator<T>::value;

template <typename T>
concept bidirectional_iterator_t = is_bidirectional_iterator_v<T>;

// ====================================================

template <typename T>
class is_random_access_iterator : public
        std::conditional_t<std::is_convertible<typename
        std::iterator_traits<T>::iterator_category,
        std::random_access_iterator_tag>::value, std::true_type, std::false_type>
{ };

template <typename T>
inline constexpr cbool is_random_access_iterator_v = is_random_access_iterator<T>::value;

template <typename T>
concept random_access_iterator_t = is_random_access_iterator_v<T>;

// ====================================================

template <typename T>
class is_iterator : public
        std::conditional_t<is_input_iterator<T>::value ||
        is_output_iterator<T>::value                   ||
        is_forward_iterator<T>::value                  ||
        is_bidirectional_iterator<T>::value            ||
        is_random_access_iterator<T>::value, std::true_type, std::false_type>
{ };

template <typename T>
inline constexpr cbool is_iterator_v = is_iterator<T>::value;

template <typename T>
concept iterator_t = is_iterator_v<T>;

// ====================================================

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
    typename std::variant_size<remove_cref_t<T>>::type;
    typename std::variant_alternative_t<0, remove_cref_t<T>>;
};

template <typename T>
concept str_view_like_t =
requires (T& t)
{
    typename T::value_type ;
    typename T::traits_type;

    //{ t          } -> std::same_as<std::basic_string_view<typename T::value_type, typename T::traits_type>>;
    { t.size  () } -> std::convertible_to<std::size_t>;
    { t.data  () } -> std::convertible_to<typename T::value_type const*>;
    { t.begin () } -> std::convertible_to<typename T::value_type const*>;
    { t.end   () } -> std::convertible_to<typename T::value_type const*>;
    { t[0]       } -> std::convertible_to<typename T::value_type>;
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

template <typename F>
using FunctionalType = std::enable_if_t<is_functional_v<F>, F>;

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
using UnionType = std::enable_if_t<std::is_union_v<remove_cref_t<T>>, T>;

template <typename T>
using NullPointer = std::enable_if_t<std::is_null_pointer_v<T>, T>;

template <typename T>
using ObjectType = std::enable_if_t<std::is_object_v<remove_cref_t<T>>, T>;

template <typename T>
using ClassType = std::enable_if_t<std::is_class_v<remove_cref_t<T>>, T>;

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
using PODType = std::enable_if_t<std::is_trivial_v<T> && std::is_standard_layout_v<T>, T>;

// ====================================================

template <typename T>
using IteratorType = typename
std::enable_if_t<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::input_iterator_tag>::value or
std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::output_iterator_tag>::value or
std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::bidirectional_iterator_tag>::value, T>;

template <typename T>
using OutputIteratorType = typename
std::enable_if_t<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::output_iterator_tag>::value, T>;

template <typename T>
using InputIteratorType = typename
std::enable_if_t<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::input_iterator_tag>::value, T>;

template <typename T>
using ForwardIteratorType = typename
std::enable_if_t<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::forward_iterator_tag>::value>;

template <typename T>
using BidirectionalIteratorType = typename
std::enable_if_t<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::bidirectional_iterator_tag>::value>;

template <typename T>
using RandomAccessIteratorType = typename
std::enable_if_t<std::is_convertible<typename
std::iterator_traits<T>::iterator_category,
std::random_access_iterator_tag>::value>;

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
