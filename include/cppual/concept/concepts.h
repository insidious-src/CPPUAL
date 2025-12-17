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
#include <string_view>
#include <functional>
#include <algorithm>
#include <iterator>
#include <concepts>
//#include <utility>
//#include <variant>
#include <tuple>

// ====================================================

namespace std::pmr { class memory_resource; } //! namespace std::pmr

// ====================================================

namespace cppual {

// ====================================================

namespace memory { class memory_resource; } //! namespace memory

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
           SZ <= 144 ? 144 :
           SZ <= 160 ? 160 :
           SZ <= 176 ? 176 :
           SZ <= 192 ? 192 :
           SZ;
}


inline constexpr static const std::size_t def_capture_size_v = lambda_calc_size ();

template <decltype (def_capture_size_v) SZ = def_capture_size_v>
inline constexpr static decltype (SZ) const max_capture_size_v = lambda_calc_size<SZ> ();

// ====================================================

//! void concepts
template <typename T>
concept void_t = std::is_void_v<std::remove_pointer_t<T>> && !std::is_pointer_v<T>;

template <typename T>
concept non_void = !std::is_void_v<std::remove_pointer_t<T>> || std::is_pointer_v<T>;

// ====================================================

//! is bool type
template <non_void> struct is_bool : std::false_type { };
template <> struct is_bool<bool>   : std::true_type  { };

//! is bool value
template <non_void T>
inline constexpr cbool is_bool_v = is_bool<T>::value;

// ===================================================

template <non_void> struct is_byte_helper : public std::false_type { };
template <> struct is_byte_helper<uchar > : public std::true_type  { };
template <> struct is_byte_helper<cuchar> : public std::true_type  { };

//! is byte (unsigned char)
template <non_void T>
struct is_byte : public is_byte_helper<T>
{ };

//! is byte value
template <non_void T>
inline constexpr cbool is_byte_v = is_byte<T>::value;

// ===================================================

template <non_void> struct is_char_helper  : public std::false_type { };
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
template <non_void T>
struct is_char : public is_char_helper<T>
{ };

//! is char value
template <non_void T>
inline constexpr cbool is_char_v = is_char<T>::value;

// ===================================================

template <non_void> struct is_stream_helper : public std::false_type { };
template <> struct is_stream_helper<char>   : public std::true_type  { };
template <> struct is_stream_helper<cchar>  : public std::true_type  { };
template <> struct is_stream_helper<uchar>  : public std::true_type  { };
template <> struct is_stream_helper<cuchar> : public std::true_type  { };

//! is char type (char, uchar)
template <non_void T>
struct is_stream_char : public is_stream_helper<T>
{ };

//! is char value
template <non_void T>
inline constexpr cbool is_stream_char_v = is_stream_char<T>::value;

// ====================================================

//! is integer type
template <non_void> struct is_integer   : std::false_type { };
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

template <non_void, std::size_t = def_capture_size_v>
class function;

// ====================================================

template <non_void T>
struct is_copyable
{
    typedef T type;
    inline constexpr static cbool value = std::is_copy_constructible_v<T> &&
                                          std::is_copy_assignable_v<T>;

    static_assert (value, "T is NOT copyable!");
};

template <non_void T>
using is_copyable_t = is_copyable<T>::type;

template <non_void T>
inline constexpr static cbool is_copyable_v = is_copyable<T>::value;

// ====================================================

template <non_void T>
struct is_movable
{
    typedef T type;
    inline constexpr static cbool value = std::is_move_constructible_v<T> &&
                                          std::is_move_assignable_v<T>;

    static_assert (value, "T is NOT movable!");
};

template <non_void T>
using is_movable_t = is_movable<T>::type;

template <non_void T>
inline constexpr static cbool is_movable_v = is_movable<T>::value;

// ====================================================

template <non_void T>
struct is_copyable_movable
{
    typedef T type;
    inline constexpr static cbool value = std::is_copy_constructible_v<T> &&
                                          std::is_copy_assignable_v<T>    &&
                                          std::is_move_constructible_v<T> &&
                                          std::is_move_assignable_v<T>;

    static_assert (value, "T is NOT copyable nor movable!");
};

template <non_void T>
using is_copyable_movable_t = is_copyable_movable<T>::type;

template <non_void T>
inline constexpr static cbool is_copyable_movable_v = is_copyable_movable<T>::value;

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

template <typename T>
using remove_array_t = std::remove_extent_t<T>;

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

// =========================================================

template <typename> struct remove_fn_const   ;
template <typename> struct remove_fn_volatile;
template <typename> struct remove_fn_cv      ;
template <typename> struct add_fn_const      ;
template <typename> struct add_fn_volatile   ;
template <typename> struct add_fn_cv         ;

template <typename R, typename... ArgTypes>
struct remove_fn_const<R(ArgTypes...) const>  { using type = R(ArgTypes...); };

template <typename R, typename... ArgTypes>
struct add_fn_const<R(ArgTypes...)>  { using type = R(ArgTypes...) const; };

template <typename R, typename... ArgTypes>
struct remove_fn_volatile<R(ArgTypes...) volatile>  { using type = R(ArgTypes...); };

template <typename R, typename... ArgTypes>
struct add_fn_volatile<R(ArgTypes...)>  { using type = R(ArgTypes...) volatile; };

template <typename R, typename... ArgTypes>
struct remove_fn_cv<R(ArgTypes...) const volatile>  { using type = R(ArgTypes...); };

template <typename R, typename... ArgTypes>
struct add_fn_cv<R(ArgTypes...)>  { using type = R(ArgTypes...) const volatile; };

template <typename S>
using remove_fn_const_t = typename remove_fn_const<S>::type;

template <typename S>
using add_fn_const_t = typename add_fn_const<S>::type;

template <typename S>
using remove_fn_volatile_t = typename remove_fn_volatile<S>::type;

template <typename S>
using add_fn_volatile_t = typename add_fn_volatile<S>::type;

template <typename S>
using remove_fn_cv_t = typename remove_fn_cv<S>::type;

template <typename S>
using add_fn_cv_t = typename add_fn_cv<S>::type;

// ====================================================

template <non_void>
struct member_function_to_static_helper
{ };

template <typename C, typename R, typename... Args>
struct member_function_to_static_helper <R(C::*)(Args...) const>
{ using type = R(*)(Args...); };

template <typename C, typename R, typename... Args>
struct member_function_to_static_helper <R(C::*)(Args...)>
{ using type = R(*)(Args...); };

template <non_void F>
using member_function_to_static_t = member_function_to_static_helper<F>::type;

template <typename F, F MEM_FN>
inline constexpr cbool member_function_to_static_t_v =
static_cast<member_function_to_static_helper<F>::type> (MEM_FN);

// ====================================================

//! basic type concepts
template <typename T>
concept constant = std::is_const_v<T>;

template <typename T>
concept variable = !std::is_const_v<T>;

template <typename T>
concept fundamental = std::is_fundamental_v<T>;

template <typename T>
concept unsigned_t = is_unsigned_v<T>;

template <typename T>
concept integer = is_integer_v<T>;

template <typename T>
concept unsigned_integer = is_unsigned_integer_v<T>;

template <typename T>
concept signed_integer = is_signed_integer_v<T>;

template <typename T>
concept floating_point = is_float_v<T>;

template <typename T>
concept number = is_integer_v<T> || is_float_v<T>;

template <typename T>
concept char_t = is_char_v<T>;

template <typename T>
concept ptr = std::is_pointer_v<T>;

template <typename T>
concept integer_or_ptr = is_integer_v<T> || std::is_pointer_v<T>;

template <typename T>
concept class_or_ptr = std::is_pointer_v<T> || std::is_class_v<T>;

template <typename T>
concept switch_value = is_integer_v<T> || std::is_enum_v<T> || is_char_v<T> || is_stream_char_v<T>;

template <typename T>
concept c_str = std::is_same_v<T, char*>;

template <typename T>
concept c_const_str = std::is_same_v<T, cchar*>;

template <typename T>
concept c_str8 = std::is_same_v<T, char8*>;

template <typename T>
concept c_const_str8 = std::is_same_v<T, cchar8*>;

template <typename T>
concept c_str16 = std::is_same_v<T, char16*>;

template <typename T>
concept c_const_str16 = std::is_same_v<T, cchar16*>;

template <typename T>
concept c_str32 = std::is_same_v<T, char32*>;

template <typename T>
concept c_const_str32 = std::is_same_v<T, cchar32*>;

template <typename T>
concept c_wstr = std::is_same_v<T, wchar*>;

template <typename T>
concept c_const_wstr = std::is_same_v<T, cwchar*>;

template <typename T>
concept c_tstr = c_str  <T>  ||
                 c_str8 <T>  ||
                 c_str16<T>  ||
                 c_str32<T>  ||
                 c_wstr <T>;

template <typename T>
concept c_const_tstr = c_const_str  <T> ||
                       c_const_str8 <T> ||
                       c_const_str16<T> ||
                       c_const_str32<T> ||
                       c_const_wstr <T>;

template <typename T>
concept stream_char = is_stream_char_v<T>;

template <typename T>
concept compact = !std::is_class_v<T> && sizeof (T) <= sizeof (uptr);

template <typename T>
concept function_t = std::is_function_v<remove_ptr_t<T>> ||
                     std::is_member_function_pointer_v<T>;

template <typename T>
concept static_function = std::is_function_v<remove_ptr_t<T>>;

template <typename T>
concept member_function = std::is_member_function_pointer_v<T>;

template <typename T>
concept non_function = !std::is_function_v<remove_ptr_t<T>> &&
                       !std::is_member_function_pointer_v<T>;

// ====================================================

//! type property concepts
template <typename T>
concept volatile_type = std::is_volatile_v<T>;

template <typename T>
concept standard_layout = std::is_standard_layout_v<T>;

template <typename T>
concept trivial = std::is_trivial_v<T>;

template <typename T>
concept pod = trivial<T> && standard_layout<T>;

// ====================================================

template <typename T>
concept callable_class = std::is_class_v<T> && requires (T&) { &std::decay_t<T>::operator (); };

//template <typename T>
//concept callable_object = requires (T&) { &std::decay_t<T>::operator (); };

//! lambda concepts
template <typename T>
concept lambda_non_capture = callable_class<T> && std::is_constructible_v<
        member_function_to_static_t<decltype (&std::decay_t<T>::operator())>, T>;

template <typename T>
concept lambda_capture = callable_class<T> && !std::is_constructible_v<
        member_function_to_static_t<decltype (&std::decay_t<T>::operator())>, T>;

template <typename T>
concept lambda = lambda_non_capture<T> || lambda_capture<T>;

template <typename T>
concept callable_non_capture_object = (std::is_class_v<T> && callable_class<T>) ||
                                       lambda_non_capture<T>;

template <typename T>
concept callable = lambda_non_capture<T> ||
                   lambda_capture    <T> ||
                   member_function   <T> ||
                   static_function   <T> ||
                   callable_class    <T> ;

// ====================================================

template <callable_class C>
inline constexpr auto callable_operator_v = &std::decay_t<C>::operator ();

template <callable_class C, typename... Args>
using callable_return = std::invoke_result_t<decltype (callable_operator_v<C>), Args...>;

// ====================================================

//! type category concepts
template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept scalar = std::is_scalar_v<T>;

template <typename T>
concept compound = std::is_compound_v<T>;

template <typename T>
concept reference_t = std::is_reference_v<T>;

template <typename T>
concept lvalue_reference = std::is_lvalue_reference_v<T>;

template <typename T>
concept rvalue_reference = std::is_rvalue_reference_v<T>;

template <typename T>
concept enumeration = std::is_enum_v<T>;

template <typename T>
concept array = std::is_array_v<T>;

template <typename T>
concept union_t = std::is_union_v<T>;

template <typename T>
concept null_ptr = std::is_null_pointer_v<T>;

template <typename T>
concept object = std::is_object_v<T>;

template <typename T>
concept struct_or_class = std::is_class_v<T>;

template <typename T>
concept class_t = struct_or_class<T>;

template <typename T>
concept class_or_union = struct_or_class<T> || union_t<T>;

template <typename T>
concept void_or_class = void_t<T> || std::is_class_v<T>;

template <typename T>
concept abstract = std::is_abstract_v<T>;

//! pointer concepts
template <typename T>
concept member_ptr = std::is_member_pointer_v<T>;

template <typename T>
concept member_object_ptr = std::is_member_object_pointer_v<T>;

template <typename T>
concept void_pointer = std::is_pointer_v<T> && std::is_void_v<remove_ptr_t<T>>;

template <typename T>
concept pair_like = requires (T& t)
{
    typename T::first_type ;
    typename T::second_type;

    { t.first  } -> std::convertible_to<typename T::first_type >;
    { t.second } -> std::convertible_to<typename T::second_type>;
};

template <typename P, typename K, typename FN>
concept fn_pair = pair_like<P>                          &&
                  switch_value<remove_cptr_t<K>>        &&
                 (std::is_function_v<FN>                ||
                  std::is_member_function_pointer_v<FN> ||
                  callable_class<FN>);


template <typename P, typename K, typename FN>
concept member_fn_pair = std::is_same_v<P, std::pair<K, FN>> &&
                         switch_value<remove_cptr_t<K>>      &&
                         std::is_member_function_pointer_v<FN>;

// ====================================================

template <struct_or_class F>
struct is_functional_helper : public std::false_type
{ typedef F type; };

template <fn_sig T, std::size_t BYTES>
struct is_functional_helper <function<T, BYTES>> : public std::true_type
{ typedef function<T, BYTES> type; };

template <fn_sig T>
struct is_functional_helper <std::function<T>> : public std::true_type
{ typedef std::function<T> type; };

template <struct_or_class F>
using is_functional = is_functional_helper<F>::type;

/// is functional -> value
template <struct_or_class F>
inline constexpr cbool is_functional_v = is_functional_helper<F>::value;

/// delegate concept
template <typename T>
concept void_callable = requires (T& t) { { t () } -> std::convertible_to<void>; };

template <typename F>
concept functional = is_functional_v<F>;

template <typename F>
concept void_functional = functional<F> && void_callable<F>;

template <typename T>
concept class_and_non_functional = struct_or_class<T> && !is_functional_v<std::decay_t<T>>;

// ====================================================

template <typename P, typename K, typename FN>
concept functional_switch_pair = pair_like<P> && switch_value<remove_cptr_t<K>> && functional<FN>;

template <typename P, typename K, typename FN>
concept functional_str_pair = pair_like<P> && std::is_same_v<K, cchar*> && functional<FN>;

template <typename Tuple, typename... Args>
concept tuple_like = std::is_same_v<Tuple, std::tuple<Args...>>;

//! constructibility and destructibility concepts
template <typename T>
concept constructible = std::is_constructible_v<T>;

template <typename T>
concept default_constructible = std::is_default_constructible_v<T>;

template <typename T>
concept destructible = std::is_destructible_v<T>;

template <typename T>
concept trivially_destructible = std::is_trivially_destructible_v<T>;

template <typename T>
concept virtually_destructible = std::has_virtual_destructor_v<T>;

template <typename T>
concept copyable_movable = std::copyable<T> && std::movable<T>;

template <typename T>
concept copyable = std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>;

template <typename T>
concept copy_constructible = std::is_copy_constructible_v<T>;

template <typename T>
concept copy_assignable = std::is_copy_assignable_v<T>;

template <typename T>
concept movable = std::is_move_constructible_v<T> && std::is_move_assignable_v<T>;

template <typename T>
concept move_constructible = std::is_move_constructible_v<T>;

template <typename T>
concept move_assignable = std::is_move_assignable_v<T>;

template <typename T1, typename T2>
concept non_convertible_to = !std::convertible_to<T1, T2> && !std::convertible_to<T2, T1>;

//! numeric concepts
template <typename T>
concept addable = requires (T a, T b) { { a + b } -> std::same_as<T>; };

template <typename T>
concept subtractable = requires (T a, T b) { { a - b } -> std::same_as<T>; };

template <typename T>
concept multiplicable = requires (T a, T b) { { a * b } -> std::same_as<T>; };

template <typename T>
concept divisible = requires (T a, T b) { { a / b } -> std::same_as<T>; };

//! range concepts
template <typename T>
concept range = std::ranges::range<T>;

template <typename T>
concept sized_range = std::ranges::sized_range<T>;

template <typename T>
concept bidirectional_range = std::ranges::bidirectional_range<T>;

template <typename T>
concept random_access_range = std::ranges::random_access_range<T>;

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
concept input_iterator = is_input_iterator_v<T>;

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
concept output_iterator = is_output_iterator_v<T>;

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
concept forward_iterator = is_forward_iterator_v<T>;

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
concept bidirectional_iterator = is_bidirectional_iterator_v<T>;

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
concept random_access_iterator = is_random_access_iterator_v<T>;

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
concept common_iterator = is_iterator_v<T>;

// ====================================================

//! utility concepts
template <typename T>
concept optional_like = requires (T& t)
{
    {  t.has_value () } -> std::same_as<bool>                 ;
    { *t              } -> std::same_as<typename T::reference>;
    {  t.value     () } -> std::same_as<typename T::reference>;
};

template <typename T>
concept variant_like = requires
{
    typename std::variant_size<remove_cref_t<T>>::type;
    typename std::variant_alternative_t<0, remove_cref_t<T>>;
};

template <typename T>
concept str_view_like = requires (T& t)
{
    typename T::value_type ;
    typename T::traits_type;

    { t.size  ()     } -> std::same_as<std::size_t>              ;
    { t.compare ("") } -> std::same_as<int>                      ;
    { t.empty ()     } -> std::same_as<bool>                     ;
    { t.substr ("")  } -> std::same_as<std::string_view>         ;
    { t.data  ()     } -> std::same_as<typename T::const_pointer>;
    { t.begin ()     } -> std::same_as<typename T::iterator>     ;
    { t.end   ()     } -> std::same_as<typename T::iterator>     ;
    { t[0]           } -> std::same_as<typename T::reference>    ;
};

//! Comparison concepts
template <typename T>
concept equality_comparable = std::equality_comparable<T>;

template <typename T>
concept totally_ordered = std::totally_ordered<T>;

//! callable concepts
template <typename F, typename... Args>
concept predicate = std::regular_invocable<F, Args...> &&
                    std::convertible_to<std::invoke_result_t<F, Args...>, bool>;

template <typename F, typename T>
concept binary_predicate = predicate<F, T, T>;

template <typename F, typename T>
concept relation = binary_predicate<F, T>;

//! TODO: replace internal begin/end with std::begin/std::end functions
//! Complex concepts
template <typename T>
concept sortable = requires (T& t)
{
    std::begin (t);
    std::end   (t);
    std::sort  (std::begin (t), std::end (t));
};

//! concept for types that can be used as dictionary keys
template<typename T>
concept keyable = std::is_copy_constructible_v<T> && std::is_move_constructible_v<T> &&
requires (T const& a, T const& b)
{
    {      a == b       } -> std::same_as<bool>       ;
    { std::hash<T>{}(a) } -> std::same_as<std::size_t>;
};

//! container concepts
template <typename T>
concept container = requires (T& a, T const& b)
{
    typename T::value_type;
    typename T::reference;
    typename T::const_reference;
    typename T::iterator;
    typename T::const_iterator;
    typename T::difference_type;
    typename T::size_type;

    { a.begin    () } -> std::same_as<typename T::iterator>      ;
    { a.end      () } -> std::same_as<typename T::iterator>      ;
    { b.begin    () } -> std::same_as<typename T::const_iterator>;
    { b.end      () } -> std::same_as<typename T::const_iterator>;
    { a.size     () } -> std::same_as<typename T::size_type>     ;
    { a.max_size () } -> std::same_as<typename T::size_type>     ;
    { a.empty    () } -> std::same_as<bool>                      ;
};

template <typename T>
concept reversible_container = container<T> && requires (T& a, T const& b)
{
    typename T::reverse_iterator      ;
    typename T::const_reverse_iterator;

    { a.rbegin () } -> std::convertible_to<typename T::reverse_iterator>      ;
    { a.rend   () } -> std::convertible_to<typename T::reverse_iterator>      ;
    { b.rbegin () } -> std::convertible_to<typename T::const_reverse_iterator>;
    { b.rend   () } -> std::convertible_to<typename T::const_reverse_iterator>;
};

template <typename T>
concept sequence_container = container<T> && requires (T& t, typename T::value_type v)
{
    { t.front     ( ) } -> std::same_as<typename T::reference>;
    { t.back      ( ) } -> std::same_as<typename T::reference>;
    { t.push_back (v) } -> std::same_as<void>                 ;
    { t.pop_back  ( ) } -> std::same_as<void>                 ;
    { t[0]            } -> std::same_as<typename T::reference>;
};

//! =========================================================
//! memory_resource concept
//! =========================================================

template <class_t T>
struct is_memory_resource :
public std::conditional_t<std::is_base_of_v<memory::memory_resource  , T> ||
                          std::is_base_of_v<std::pmr::memory_resource, T>,
                          std::true_type,
                          std::false_type
                          >
{ static_assert (is_memory_resource<T>::value, "T is NOT derived from memory_resource!"); };

template <>
struct is_memory_resource <memory::memory_resource> : public std::true_type
{ };

template <>
struct is_memory_resource <std::pmr::memory_resource> : public std::true_type
{ };

//! is_memory_resource -> value
template <struct_or_class T>
inline constexpr cbool is_memory_resource_v = is_memory_resource<T>::value;

//! memory_resource concept
template <typename T>
concept memory_resource_t = is_memory_resource_v<T>;

//! ====================================================
//! OLD STYLE CONCEPTS AS TYPE TRAITS
//! ====================================================

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
//using Mergeable = std::enable_if_t<std::is_standard_layout_v<T>, T>;

//! memory_resource concept type
template <class_t T>
using MemoryResourceType = std::enable_if_t<is_memory_resource_v<T>, T>;

// ====================================================

} } // cppual

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_CONCEPTS_H_
