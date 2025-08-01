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
#include <cppual/meta_functional>
#include <cppual/noncopyable>

#include <string_view>
//#include <typeinfo>
#include <version>

// =========================================================

namespace cppual::abi {

// =========================================================

class rtti
{
public:
    typedef rtti             self_type    ;
    typedef char             value_type   ;
    typedef std::size_t      size_type    ;
    typedef size_type const  const_Size   ;
    typedef std::string_view string_type  ;
    typedef cchar*           const_pointer;

    // =========================================================

    template <cchar* U = "void">
    using type_name_t = std::conditional_t<char_hash<U> () == char_hash ("char"), char,
        std::conditional_t<char_hash<U> () == char_hash ("unsigned char"), uchar,
        std::conditional_t<char_hash<U> () == char_hash ("char8_t"), char8_t,
        std::conditional_t<char_hash<U> () == char_hash ("char16_t"), char16_t,
        std::conditional_t<char_hash<U> () == char_hash ("char32_t"), char32_t,
        std::conditional_t<char_hash<U> () == char_hash ("wchar_t"), wchar_t,
        std::conditional_t<char_hash<U> () == char_hash ("bool"), bool,
        std::conditional_t<char_hash<U> () == char_hash ("short"), short,
        std::conditional_t<char_hash<U> () == char_hash ("unsigned short"), ushort,
        std::conditional_t<char_hash<U> () == char_hash ("int"), int,
        std::conditional_t<char_hash<U> () == char_hash ("unsigned int"), uint,
        std::conditional_t<char_hash<U> () == char_hash ("long"), long,
        std::conditional_t<char_hash<U> () == char_hash ("unsigned long"), ulong,
        std::conditional_t<char_hash<U> () == char_hash ("long long"), long64,
        std::conditional_t<char_hash<U> () == char_hash ("unsigned long long"), ulong64,
        std::conditional_t<char_hash<U> () == char_hash ("float"), float,
        std::conditional_t<char_hash<U> () == char_hash ("double"), double,
        std::conditional_t<char_hash<U> () == char_hash ("long double"), ldouble,
        void>>>>>>>>>>>>>>>>>>;

    // =========================================================

    consteval rtti (self_type &&)                      noexcept = default;
    consteval rtti (self_type const&)                  noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <typename U>
    constexpr self_type& operator = (var_type_t<U> val) noexcept
    {
        _M_type_name = type_name (val).data ();
        return *this;
    }

    template <typename U = void>
    consteval rtti (var_type_t<U> = type<void>) noexcept
    : _M_type_name (type_name_v<U>)
    { }

    consteval string_type name () const noexcept
    { return _M_type_name; }

    consteval uint hash () const noexcept
    {  return char_hash (_M_type_name); }

public:
    template <typename U = void>
    consteval static const_pointer type_name (var_type_t<U> = type<void>) noexcept
    {
        if      constexpr (std::is_same_v<U, char>) return "char";
        else if constexpr (std::is_same_v<U, uchar>) return "unsigned char";
        else if constexpr (std::is_same_v<U, char8_t>) return "char8_t";
        else if constexpr (std::is_same_v<U, char16_t>) return "char16_t";
        else if constexpr (std::is_same_v<U, char32_t>) return "char32_t";
        else if constexpr (std::is_same_v<U, wchar_t>) return "wchar_t";
        else if constexpr (std::is_same_v<U, bool>) return "bool";
        else if constexpr (std::is_same_v<U, short>) return "short";
        else if constexpr (std::is_same_v<U, ushort>) return "unsigned short";
        else if constexpr (std::is_same_v<U, int>) return "int";
        else if constexpr (std::is_same_v<U, uint>) return "unsigned int";
        else if constexpr (std::is_same_v<U, long>) return "long";
        else if constexpr (std::is_same_v<U, ulong>) return "unsigned long";
        else if constexpr (std::is_same_v<U, long64>) return "long long";
        else if constexpr (std::is_same_v<U, ulong64>) return "unsigned long long";
        else if constexpr (std::is_same_v<U, float>) return "float";
        else if constexpr (std::is_same_v<U, double>) return "double";
        else if constexpr (std::is_same_v<U, ldouble>) return "long double";

        return "void";
    }

    template <typename U>
    inline constexpr static const_pointer const type_name_v = type_name (type<U>);

public:
    const_pointer _M_type_name { };
};

// =========================================================

consteval bool operator == (rtti const& lh, rtti const& rh) noexcept
{
    return lh.hash () == rh.hash ();
}

consteval bool operator != (rtti const& lh, rtti const& rh) noexcept
{
    return !(lh == rh);
}

// =========================================================

} // namespace ABI

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_ABI_H_
