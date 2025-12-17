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

#ifndef CPPUAL_ABI_H_
#define CPPUAL_ABI_H_
#ifdef __cplusplus

#include <cppual/decl>
#include <cppual/types>
#include <cppual/string>
#include <cppual/concepts>
#include <cppual/meta_type>
#include <cppual/noncopyable>
#include <cppual/meta_functional>

//#include <source_location>
#include <version>

// =========================================================

namespace cppual::abi {

// =========================================================

class rtti
{
public:
    typedef rtti            self_type    ;
    typedef char            value_type   ;
    typedef cchar*          const_pointer;
    typedef std::size_t     size_type    ;
    typedef size_type const const_size   ;
    typedef fstring         string_type  ;
    typedef fstring_view    string_view  ;

    // =========================================================

    template <const_pointer STR = "void">
    using type_t =
        std::conditional_t<char_hash<STR> () == char_hash ("char"), char,
        std::conditional_t<char_hash<STR> () == char_hash ("unsigned char"), uchar,
        std::conditional_t<char_hash<STR> () == char_hash ("char8_t"), char8,
        std::conditional_t<char_hash<STR> () == char_hash ("char16_t"), char16,
        std::conditional_t<char_hash<STR> () == char_hash ("char32_t"), char32,
        std::conditional_t<char_hash<STR> () == char_hash ("wchar_t"), wchar,
        std::conditional_t<char_hash<STR> () == char_hash ("bool"), bool,
        std::conditional_t<char_hash<STR> () == char_hash ("short"), short,
        std::conditional_t<char_hash<STR> () == char_hash ("unsigned short"), ushort,
        std::conditional_t<char_hash<STR> () == char_hash ("int"), int,
        std::conditional_t<char_hash<STR> () == char_hash ("unsigned int"), uint,
        std::conditional_t<char_hash<STR> () == char_hash ("long"), long,
        std::conditional_t<char_hash<STR> () == char_hash ("unsigned long"), ulong,
        std::conditional_t<char_hash<STR> () == char_hash ("long long"), long64,
        std::conditional_t<char_hash<STR> () == char_hash ("unsigned long long"), ulong64,
        std::conditional_t<char_hash<STR> () == char_hash ("float"), float,
        std::conditional_t<char_hash<STR> () == char_hash ("double"), double,
        std::conditional_t<char_hash<STR> () == char_hash ("long double"), ldouble,
        void>>>>>>>>>>>>>>>>>>;

    // =========================================================

    constexpr rtti (self_type &&)                      noexcept = default;
    constexpr rtti (self_type const&)                  noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <typename U = void>
    consteval rtti (type_ptr_t<U> = type_v<U>) noexcept
    : _M_typename  (type_name_v<U>)
    { }

    template <typename U>
    constexpr self_type& operator = (type_ptr_t<U>) noexcept
    {
        _M_typename = type_name_v<U>;
        return *this;
    }

    consteval const_pointer name () const noexcept
    { return _M_typename; }

    constexpr size_type hash_code () const noexcept
    {  return char_hash (_M_typename); }

public:
    template <typename U = void>
    consteval static const_pointer type_name () noexcept
    {
        if      constexpr (std::same_as<U, char>) return "char";
        else if constexpr (std::same_as<U, cchar>) return "const char";
        else if constexpr (std::same_as<U, uchar>) return "unsigned char";
        else if constexpr (std::same_as<U, cuchar>) return "const unsigned char";
        else if constexpr (std::same_as<U, char8>) return "char8_t";
        else if constexpr (std::same_as<U, cchar8>) return "const char8_t";
        else if constexpr (std::same_as<U, char16>) return "char16_t";
        else if constexpr (std::same_as<U, cchar16>) return "const char16_t";
        else if constexpr (std::same_as<U, char32>) return "char32_t";
        else if constexpr (std::same_as<U, cchar32>) return "const char32_t";
        else if constexpr (std::same_as<U, wchar>) return "wchar_t";
        else if constexpr (std::same_as<U, cwchar>) return "const wchar_t";
        else if constexpr (std::same_as<U, bool>) return "bool";
        else if constexpr (std::same_as<U, cbool>) return "const bool";
        else if constexpr (std::same_as<U, short>) return "short int";
        else if constexpr (std::same_as<U, cshort>) return "const short int";
        else if constexpr (std::same_as<U, ushort>) return "unsigned short int";
        else if constexpr (std::same_as<U, cushort>) return "const unsigned short int";
        else if constexpr (std::same_as<U, int>) return "int";
        else if constexpr (std::same_as<U, cint>) return "const int";
        else if constexpr (std::same_as<U, uint>) return "unsigned int";
        else if constexpr (std::same_as<U, cuint>) return "const unsigned int";
        else if constexpr (std::same_as<U, long>) return "long int";
        else if constexpr (std::same_as<U, clong>) return "const long int";
        else if constexpr (std::same_as<U, ulong>) return "unsigned long int";
        else if constexpr (std::same_as<U, culong>) return "const unsigned long int";
        else if constexpr (std::same_as<U, long64>) return "long long int";
        else if constexpr (std::same_as<U, clong64>) return "const long long int";
        else if constexpr (std::same_as<U, ulong64>) return "unsigned long long int";
        else if constexpr (std::same_as<U, culong64>) return "const unsigned long long int";
        else if constexpr (std::same_as<U, float>) return "float";
        else if constexpr (std::same_as<U, cfloat>) return "const float";
        else if constexpr (std::same_as<U, double>) return "double";
        else if constexpr (std::same_as<U, cdouble>) return "const double";
        else if constexpr (std::same_as<U, ldouble>) return "long double";
        else if constexpr (std::same_as<U, cldouble>) return "const long double";
        else if constexpr (std::is_array_v<U>) return type_name_str_v<remove_array_t<U>> + "[]";
        else if constexpr (std::is_pointer_v<U>) return type_name_str_v<std::remove_pointer_t<U>> + " *";
        else if constexpr (std::is_member_function_pointer_v<U>) return "member function pointer";
        else if constexpr (std::is_function_v<U>) return "function";
        else if constexpr (std::is_class_v<U>) return "class";
        else if constexpr (std::is_union_v<U>) return "union";
        else if constexpr (std::is_enum_v<U>) return "enumeration";

        return "void";
    }

    template <typename U = void>
    consteval static string_view type_name_sv () noexcept
    { return type_name<U> (); }

    template <typename U>
    inline constexpr static const_pointer const type_name_v = type_name<U> ();

    template <typename U>
    inline constexpr static string_view const type_name_sv_v = type_name<U> ();

    template <typename U>
    inline constexpr static string_type const type_name_str_v = type_name<U> ();

public:
    const_pointer _M_typename { };
};

// =========================================================


constexpr bool operator == (rtti const& lh, rtti const& rh) noexcept
{
    return lh.hash_code () == rh.hash_code ();
}

constexpr bool operator != (rtti const& lh, rtti const& rh) noexcept
{
    return !(lh == rh);
}

// =========================================================

//! Type name extraction
template <rtti const Type>
using type_t = rtti::type_t<Type.name ()>;

//! Type convertability check and extraction - argument convertable on std::forward
template <typename From, rtti const To>
using type_convertable_t = std::enable_if_t<std::convertible_to<From, type_t<To>>, type_t<To>>;

//! Argument type extraction from ABI array
template <rtti const Array[], typename From, rtti::size_type I>
using arg_type_t = abi::type_convertable_t<From, Array[I]>;

// =========================================================

} // namespace abi

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_ABI_H_
