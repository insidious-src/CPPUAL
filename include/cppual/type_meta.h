/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#ifndef CPPUAL_TYPE_META_H_
#define CPPUAL_TYPE_META_H_
#ifdef __cplusplus

#include <cppual/casts>
#include <cppual/bitflags>
#include <cppual/concepts>

//#include <span>
//#include <array>
#include <tuple>
#include <limits>
//#include <vector>
//#include <utility>
//#include <optional>
#include <type_traits>

// ====================================================

namespace cppual {

// ====================================================

template <struct_or_class Tuple, std::size_t I>
using selected_tuple_t = std::tuple_element_t<I, Tuple>;

// ===================================================

template <typename T, struct_or_class Tuple, std::size_t I = 0>
struct tuple_ref_index;

template <typename T, typename Base, typename... Bases, std::size_t I>
struct tuple_ref_index<T, std::tuple<Base, Bases...>, I>
    : std::conditional_t<std::is_base_of_v<Base, T> || std::is_same_v<Base, T>,
                         std::integral_constant<decltype (I), I>,
                         tuple_ref_index<T, std::tuple<Bases...>, I + 1>
                         >
{ };

/// get base class from derived using a list of bases
template <struct_or_class Derived, struct_or_class... Bases>
using base_from_derived = selected_tuple_t
<std::tuple<Bases...>, tuple_ref_index<Derived, std::tuple<Bases...>>::value>;

//! ====================================================
//! Type pack and unpack. remove nth element in a
//! variadic template (ex. std::tuple) and return
//! the rest of the elements as a template class with
//! variadic arguments
//! ====================================================

template <typename...> struct pack { };

template <template <typename...> class T, typename Pack>
struct unpack;

template <template <typename...> class T, typename... Args>
struct unpack <T, pack<Args...>>
{
    typedef T<Args...> type;
};

template <typename T, typename Pack>
struct prepend;

template <typename T, typename... Args>
struct prepend <T, pack<Args...>>
{
    typedef pack<T, Args...> type;
};

template <std::size_t N, typename... Args>
struct remove_nth_helper;

template <std::size_t N, typename T, typename... Ts>
struct remove_nth_helper <N, T, Ts...> : prepend<T, typename remove_nth_helper<N - 1, Ts...>::type>
{ };

template <typename T, typename... Ts>
struct remove_nth_helper <0, T, Ts...>
{
    typedef pack<Ts...> type;
};

template <typename T, int N>
struct remove_nth;

template <template <typename...> class T, int N, typename... Args>
struct remove_nth <T<Args...>, N>
{
    typedef typename unpack
    <
        T,
        typename remove_nth_helper<N, Args...>::type
    >
    ::type type;
};

template <typename T, int N>
using remove_nth_t = typename remove_nth<T, N>::type;

// ====================================================

struct traits_enum
{
public:
    typedef enum class type : u16
    {
        void_type                        ,
        boolean                 = 1 <<  0,
        character               = 1 <<  1,
        integer                 = 1 <<  2,
        floating_point          = 1 <<  3,
        number                  = 1 <<  4,
        enumeration             = 1 <<  5,
        member_pointer          = 1 <<  6,
        member_function_pointer = 1 <<  7,
        static_function         = 1 <<  8, //! free function
        object                  = 1 <<  9,
        structure               = 1 << 10, //! class or struct
        union_type              = 1 << 11,
        reference               = 1 << 12,
        pointer                 = 1 << 13,
        array                   = 1 << 14,
        pod                     = 1 << 15
    }
    const const_type;

    typedef bitset<type> types;

private:
    template <typename T>
    consteval static bool is_type (const_type type_bits) noexcept
    {
        switch (type_bits)
        {
        case type::boolean: return is_bool_v<T>;
        case type::character: return is_char_v<T>;
        case type::integer: return is_integer_v<T>;
        case type::floating_point: return is_float_v<T>;
        case type::number: return is_integer_v<T> || is_float_v<T>;
        case type::enumeration: return std::is_enum_v<T>;
        case type::member_pointer: return std::is_member_pointer_v<T>;
        case type::member_function_pointer: return std::is_member_function_pointer_v<T>;
        case type::static_function: return std::is_function_v<T>;
        case type::object: return std::is_object_v<T>;
        case type::structure: return std::is_class_v<T>;
        case type::union_type: return std::is_union_v<T>;
        case type::reference: return std::is_reference_v<T>;
        case type::pointer: return std::is_pointer_v<T>;
        case type::array: return std::is_array_v<T>;
        case type::pod: return std::is_trivial_v<T> && std::is_standard_layout_v<T>;
        default: return std::is_void_v<T>;
        }
    }

public:
    template <type E, typename T>
    inline constexpr static cbool value = is_type<T> (E);
};

typedef traits_enum::type  traits_enum_t ;
typedef traits_enum::types traits_enum_ts;

template <traits_enum_t E, typename T>
inline constexpr bool const traits_enum_v = traits_enum::value<E, T>;

// ====================================================

template <integer T>
struct traits_type
{
    typedef T                type       ;
    typedef remove_cref_t<T> value_type ;
    typedef value_type const const_value;

    inline constexpr static const_value min_value = std::numeric_limits<value_type>::min ();
    inline constexpr static const_value max_value = std::numeric_limits<value_type>::max ();
};

// ====================================================

/// are all template arguments of the same type
template <typename...>
struct are_of_same_type : std::true_type
{ };

/// are all template arguments of the same type
template <typename T, typename R, typename... Ts>
struct are_of_same_type <T, R, Ts...> : std::false_type
{ };

/// are all template arguments of the same type
template <typename T, typename... Ts>
struct are_of_same_type <T, T, Ts...> : are_of_same_type<T, Ts...>
{ };

/// are all template arguments of the same type -> value
template <typename... Ts>
inline constexpr cbool are_of_same_type_v = are_of_same_type<Ts...>::value;

// =====================================================

/// are all template arguments of type
template <traits_enum_t, typename...>
struct are_all_of_type : std::true_type
{ };

/// are all template arguments of type
template <traits_enum_t E, typename T, typename... Ts>
struct are_all_of_type <E, T, Ts...>
{
    inline constexpr static cbool value = traits_enum_v<E, T> && are_all_of_type<E, Ts...>::value;
};

/// specialization for single type
template <traits_enum_t E, typename T>
struct are_all_of_type<E, T> : std::bool_constant<traits_enum_v<E, T>>
{ };

/// are all template arguments of type -> value
template <traits_enum_t E, typename... Ts>
inline constexpr cbool are_all_of_type_v = are_all_of_type<E, Ts...>::value;

// ====================================================

/// are there any types of (...) in the template argument list
template <traits_enum_t, typename...>
struct are_any_of_type : std::false_type
{ };

/// are there any types of (...) in the template argument list
template <traits_enum_t E, typename T, typename... Ts>
struct are_any_of_type <E, T, Ts...>
{
    inline constexpr static cbool value = traits_enum_v<E, T> || are_any_of_type<E, Ts...>::value;
};

/// specialization for single type
template <traits_enum_t E, typename T>
struct are_any_of_type<E, T> : std::bool_constant<traits_enum_v<E, T>>
{ };

/// are there any types of (...) in the template argument list -> value
template <traits_enum_t E, typename... Ts>
inline constexpr cbool are_any_of_type_v = are_any_of_type<E, Ts...>::value;

// ====================================================

template <array T>
constexpr std::size_t array_length (T val) noexcept
{
    return (sizeof (val) / sizeof (T));
}

template <array T>
constexpr std::size_t array_size (T val) noexcept
{
    return sizeof (val);
}

template <integer T>
constexpr u16 digit_count (T val) noexcept
{
    u16 uDigitCount = 0;

    for (int n = (val > 0 ? val : -val); n > 0; n /= 10) ++uDigitCount;
    return uDigitCount;
}

template <integer T>
constexpr bool is_power_of_two (T val) noexcept
{
    return val < 1 ? false : (val & (val - 1)) == 0;
}

// ====================================================

} // cppual

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_TYPE_META_H_
