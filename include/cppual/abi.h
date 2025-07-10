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
    typedef cchar*           value_type   ;
    typedef std::size_t      size_type    ;
    typedef size_type const  const_Size   ;
    typedef std::string_view string_type  ;
    typedef cchar*           const_pointer;

    // =========================================================

    consteval rtti (self_type &&) noexcept = default;
    consteval rtti (self_type const&) noexcept = default;

    rtti () noexcept = delete;

    self_type& operator = (self_type &&) noexcept = delete;
    self_type& operator = (self_type const&) noexcept = delete;

    template <typename U>
    consteval rtti (U& val) noexcept
    : _M_type_name (type_name (val))
    { }

    consteval string_type name () const noexcept
    { return _M_type_name; }

    consteval auto hash () const noexcept
    { return constexpr_char_hash (_M_type_name); }

public:
    template <typename U>
    consteval static string_type type_name (U&) noexcept
    {
        if      constexpr (std::is_same_v<U, char>) return "char";
        else if constexpr (std::is_same_v<U, char8_t>) return "char8_t";
        else if constexpr (std::is_same_v<U, char16_t>) return "char16_t";
        else if constexpr (std::is_same_v<U, char32_t>) return "char32_t";
        else if constexpr (std::is_same_v<U, wchar_t>) return "wchar_t";
        else if constexpr (std::is_same_v<U, bool>) return "bool";
        else if constexpr (std::is_same_v<U, int>) return "int";
        else if constexpr (std::is_same_v<U, float>) return "float";

        return "void";
    }

public:
    value_type _M_type_name;
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
