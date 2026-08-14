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
#include <cppual/functional>
#include <cppual/noncopyable>
#include <cppual/meta_functional>

#include <source_location>
//#include <version>

// =========================================================

namespace cppual { namespace abi {

// =========================================================

class rtti
{
public:
    typedef rtti                    self_type    ;
    typedef char                    value_type   ;
    typedef cchar*                  const_pointer;
    typedef std::size_t             size_type    ;
    typedef size_type const         const_size   ;
    typedef fstring                 string_type  ;
    typedef fstring_view            string_view  ;
    typedef size_type self_type::*  safe_bool    ;

    consteval rtti ()                                  noexcept = default;
    constexpr rtti (self_type &&)                      noexcept = default;
    constexpr rtti (self_type const&)                  noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <typename U = void>
    consteval rtti (type_ptr_t<U>) noexcept
    : _M_type_hash (char_hash (name_of<U> ()))
    { }

    template <typename U>
    constexpr self_type& operator = (type_ptr_t<U>) noexcept
    { _M_type_hash = char_hash (name_of<U> ()); return *this; }

    consteval operator bool () const noexcept
    { return _M_type_hash != 0; }

    consteval explicit operator safe_bool () const noexcept
    { return _M_type_hash != 0 ? &self_type::_M_type_hash : nullptr; }

    consteval const_pointer name () const noexcept
    { return name_of (_M_type_hash); }

    consteval size_type hash_code () const noexcept
    { return _M_type_hash; }

    template <typename U = void>
    consteval static const_pointer name_of () noexcept
    {
        if      constexpr (are_same<U, cvoid>) return "const void";
        else if constexpr (are_same<U, char>) return "char";
        else if constexpr (are_same<U, cchar>) return "const char";
        else if constexpr (are_same<U, uchar>) return "unsigned char";
        else if constexpr (are_same<U, cuchar>) return "const unsigned char";
        else if constexpr (are_same<U, char8>) return "char8_t";
        else if constexpr (are_same<U, cchar8>) return "const char8_t";
        else if constexpr (are_same<U, char16>) return "char16_t";
        else if constexpr (are_same<U, cchar16>) return "const char16_t";
        else if constexpr (are_same<U, char32>) return "char32_t";
        else if constexpr (are_same<U, cchar32>) return "const char32_t";
        else if constexpr (are_same<U, wchar>) return "wchar_t";
        else if constexpr (are_same<U, cwchar>) return "const wchar_t";
        else if constexpr (are_same<U, bool>) return "bool";
        else if constexpr (are_same<U, cbool>) return "const bool";
        else if constexpr (are_same<U, short>) return "short int";
        else if constexpr (are_same<U, cshort>) return "const short int";
        else if constexpr (are_same<U, ushort>) return "unsigned short int";
        else if constexpr (are_same<U, cushort>) return "const unsigned short int";
        else if constexpr (are_same<U, int>) return "int";
        else if constexpr (are_same<U, cint>) return "const int";
        else if constexpr (are_same<U, uint>) return "unsigned int";
        else if constexpr (are_same<U, cuint>) return "const unsigned int";
        else if constexpr (are_same<U, long>) return "long int";
        else if constexpr (are_same<U, clong>) return "const long int";
        else if constexpr (are_same<U, ulong>) return "unsigned long int";
        else if constexpr (are_same<U, culong>) return "const unsigned long int";
        else if constexpr (are_same<U, long64>) return "long long int";
        else if constexpr (are_same<U, clong64>) return "const long long int";
        else if constexpr (are_same<U, ulong64>) return "unsigned long long int";
        else if constexpr (are_same<U, culong64>) return "const unsigned long long int";
        else if constexpr (are_same<U, float>) return "float";
        else if constexpr (are_same<U, cfloat>) return "const float";
        else if constexpr (are_same<U, double>) return "double";
        else if constexpr (are_same<U, cdouble>) return "const double";
        else if constexpr (are_same<U, ldouble>) return "long double";
        else if constexpr (are_same<U, cldouble>) return "const long double";
        else if constexpr (array_like<U>) return name_of<remove_array_t<U>> () + "[]";
        else if constexpr (is_ref_v<U>) return name_of<remove_ref_t<U>> () + " &";
        else if constexpr (is_cref_v<U>) return name_of<remove_cref_t<U>> () + " const &";
        else if constexpr (is_refptr_v<U>) return name_of<remove_refptr_t<U>> () + " &*";
        else if constexpr (is_crefptr_v<U>) return name_of<remove_crefptr_t<U>> () + " const &*";
        else if constexpr (ptr<U>) return name_of<remove_ptr_t<U>> () + " *";
        else if constexpr (member_function<U>) return "member function pointer";
        else if constexpr (static_function<U>) return "function";
        else if constexpr (structure<U>) return "class";
        else if constexpr (union_like<U>) return "union";
        else if constexpr (enumeration<U>) return "enumeration";

        return "void";
    }

    // =========================================================

    consteval static const_pointer name_of (size_type hash) noexcept
    {
        switch (hash)
        {
        case char_hash (name_of<cvoid> ()): return name_of<cvoid> ();
        case char_hash (name_of<bool> ()): return name_of<bool> ();
        case char_hash (name_of<cbool> ()): return name_of<cbool> ();
        case char_hash (name_of<char> ()): return name_of<char> ();
        case char_hash (name_of<cchar> ()): return name_of<cchar> ();
        case char_hash (name_of<uchar> ()): return name_of<uchar> ();
        case char_hash (name_of<cuchar> ()): return name_of<cuchar> ();
        case char_hash (name_of<char8> ()): return name_of<char8> ();
        case char_hash (name_of<cchar8> ()): return name_of<cchar8> ();
        case char_hash (name_of<char16> ()): return name_of<char16> ();
        case char_hash (name_of<cchar16> ()): return name_of<cchar16> ();
        case char_hash (name_of<char32> ()): return name_of<char32> ();
        case char_hash (name_of<cchar32> ()): return name_of<cchar32> ();
        case char_hash (name_of<wchar> ()): return name_of<wchar> ();
        case char_hash (name_of<cwchar> ()): return name_of<cwchar> ();
        case char_hash (name_of<short> ()): return name_of<short> ();
        case char_hash (name_of<cshort> ()): return name_of<cshort> ();
        case char_hash (name_of<ushort> ()): return name_of<ushort> ();
        case char_hash (name_of<cushort> ()): return name_of<cushort> ();
        case char_hash (name_of<int> ()): return name_of<int> ();
        case char_hash (name_of<cint> ()): return name_of<cint> ();
        case char_hash (name_of<uint> ()): return name_of<uint> ();
        case char_hash (name_of<cuint> ()): return name_of<cuint> ();
        case char_hash (name_of<long> ()): return name_of<long> ();
        case char_hash (name_of<clong> ()): return name_of<clong> ();
        case char_hash (name_of<ulong> ()): return name_of<ulong> ();
        case char_hash (name_of<culong> ()): return name_of<culong> ();
        case char_hash (name_of<long64> ()): return name_of<long64> ();
        case char_hash (name_of<clong64> ()): return name_of<clong64> ();
        case char_hash (name_of<ulong64> ()): return name_of<ulong64> ();
        case char_hash (name_of<culong64> ()): return name_of<culong64> ();
        case char_hash (name_of<float> ()): return name_of<float> ();
        case char_hash (name_of<cfloat> ()): return name_of<cfloat> ();
        case char_hash (name_of<double> ()): return name_of<double> ();
        case char_hash (name_of<cdouble> ()): return name_of<cdouble> ();
        case char_hash (name_of<ldouble> ()): return name_of<ldouble> ();
        case char_hash (name_of<cldouble> ()): return name_of<cldouble> ();
        case char_hash ("member function pointer"): return "member function pointer";
        case char_hash ("function"): return "function";
        case char_hash ("class"): return "class";
        case char_hash ("union"): return "union";
        case char_hash ("enumeration"): return "enumeration";
        }

        return name_of<void> ();
    }

    // =========================================================

    template <size_type Hash>
    consteval static const_pointer name_of () noexcept
    { return name_of (Hash); }

    // =========================================================

    template <const_pointer STR = "void">
    using type_of =
    std::conditional_t<char_hash<STR> () == char_hash (name_of<char> ()), char,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cchar> ()), cchar,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<uchar> ()), uchar,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cuchar> ()), cuchar,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<char8> ()), char8,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cchar8> ()), cchar8,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<char16> ()), char16,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cchar16> ()), cchar16,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<char32> ()), char32,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cchar32> ()), cchar32,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<wchar> ()), wchar,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cwchar> ()), cwchar,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<bool> ()), bool,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cbool> ()), cbool,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<short> ()), short,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cshort> ()), cshort,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<ushort> ()), ushort,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cushort> ()), cushort,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<int> ()), int,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cint> ()), cint,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<uint> ()), uint,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cuint> ()), cuint,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<long> ()), long,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<clong> ()), clong,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<ulong> ()), ulong,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<culong> ()), culong,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<long64> ()), long64,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<clong64> ()), clong64,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<ulong64> ()), ulong64,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<culong64> ()), culong64,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<float> ()), float,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cfloat> ()), cfloat,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<double> ()), double,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cdouble> ()), cdouble,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<ldouble> ()), ldouble,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cldouble> ()), cldouble,
    std::conditional_t<char_hash<STR> () == char_hash (name_of<cvoid> ()), cvoid,
    std::enable_if_t  <char_hash<STR> () == char_hash (name_of<void> ()), void
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>;

public:
    size_type _M_type_hash { };
};

// =========================================================

consteval bool operator == (rtti const& lh, rtti const& rh) noexcept
{
    return lh.hash_code () == rh.hash_code ();
}

consteval bool operator != (rtti const& lh, rtti const& rh) noexcept
{
    return !(lh == rh);
}

// ====================================================

template <std::size_t N = def_arity_v>
class function_rtti
{
public:
    typedef function_rtti<N>                    self_type      ;
    typedef decltype (N)                        size_type      ;
    typedef size_type const                     const_size     ;
    typedef function<void()>                    fn_type        ;
    typedef fn_type &                           fn_ref         ;
    typedef fn_type const&                      fn_const_ref   ;
    typedef rtti                                rtti_type      ;
    typedef static_array<rtti_type, arity_v<N>> array_type     ;
    typedef array_type &                        array_ref      ;
    typedef array_type const&                   array_const_ref;
    typedef fn_type* self_type::*               safe_bool      ;

    template <fn_sig S, size_type SZ = def_capture_size_v>
    using fn_t = function<S, max_capture_size_v<SZ>>;

    template <size_type SZ>
    using self_type_t = function_rtti<arity_v<SZ>>;

    constexpr function_rtti () noexcept
    : _M_fn ()
    , _M_arg_types ()
    , _M_ret_type  ()
    { }

    template <size_type SZ = N, std::enable_if_t<arity_v<N> >= arity_v<SZ>, void>>
    constexpr function_rtti (self_type_t<SZ>&& rh) noexcept
    : _M_fn        (std::move (rh._M_fn       ))
    , _M_arg_types (std::move (rh._M_arg_types))
    , _M_ret_type  (std::move (rh._M_ret_type ))
    { }

    template <size_type SZ = N, std::enable_if_t<arity_v<N> >= arity_v<SZ>, void>>
    constexpr function_rtti (self_type_t<SZ> const& rh) noexcept
    : _M_fn        (rh._M_fn       )
    , _M_arg_types (rh._M_arg_types)
    , _M_ret_type  (rh._M_ret_type )
    { }

    template <size_type SZ = N, std::enable_if_t<arity_v<N> >= arity_v<SZ>, void>>
    constexpr self_type& operator = (self_type_t<SZ>&& rh) noexcept
    {
        if (this == &rh) return *this; //! self-assignment check

        _M_fn        = std::move (rh._M_fn       );
        _M_arg_types = std::move (rh._M_arg_types);
        _M_ret_type  = std::move (rh._M_ret_type );

        return *this;
    }

    template <size_type SZ = N, std::enable_if_t<arity_v<N> >= arity_v<SZ>, void>>
    constexpr self_type& operator = (self_type_t<SZ> const& rh) noexcept
    {
        if (this == &rh) return *this; //! self-assignment check

        _M_fn        = rh._M_fn       ;
        _M_arg_types = rh._M_arg_types;
        _M_ret_type  = rh._M_ret_type ;

        return *this;
    }

    template <typename R, typename... Args>
    constexpr self_type& operator = (fn_t<R(Args...)> const& fn) noexcept
    { return *this = self_type (fn); }

    constexpr operator bool () const noexcept
    { return _M_fn != nullptr; }

    constexpr explicit operator safe_bool () const noexcept
    { return _M_fn != nullptr ? &self_type::_M_fn : nullptr; }

    consteval static size_type arity () noexcept
    { return N; }

    constexpr fn_const_ref           fn () const noexcept { return _M_fn       ; }
    constexpr array_const_ref arg_types () const noexcept { return _M_arg_types; }
    constexpr rtti_type     return_type () const noexcept { return _M_ret_type ; }

    template <size_type I>
    consteval rtti_type arg_type () const noexcept
    {
        static_assert (I < arity_v<N>, "index out of range!");
        return _M_arg_types[I];
    }

    template <typename R, typename... Args, size_type SZ = def_capture_size_v>
    constexpr function_rtti (fn_t<R(Args...), SZ> const& fn)
    : _M_fn        ( fn_cast<void()> (fn)        )
    , _M_arg_types { abi::rtti (type_v<Args>)... }
    , _M_ret_type  ( type_v<R>                   )
    { }

    template <typename R, typename... Args>
    constexpr function_rtti (fn_t<R(Args...) const> const& fn)
    : _M_fn        ( fn_cast<void()> (fn)        )
    , _M_arg_types { abi::rtti (type_v<Args>)... }
    , _M_ret_type  ( type_v<R>                   )
    { }

    template <size_type>
    friend class function_rtti;

private:
    fn_type    _M_fn       ;
    array_type _M_arg_types;
    rtti_type  _M_ret_type ;
};

// ====================================================

template <std::size_t N1, std::size_t N2>
constexpr auto operator <=> (function_rtti<N1> const& lh, function_rtti<N2> const& rh) noexcept
{
    return N1 <=> N2 && lh._M_fn <=> rh._M_fn;
}

// =========================================================

//! Type name extraction
template <rtti const Type>
using type_of = rtti::type_of<Type.name ()>;

//! Type convertability check and extraction - argument convertable on std::forward
template <typename From, rtti const To>
using type_convertable_to = std::enable_if_t<std::convertible_to<From, type_of<To>>, type_of<To>>;

// =========================================================

template <typename U>
inline constexpr static rtti::const_pointer const name_of_v = rtti::name_of<U> ();

template <typename U>
inline constexpr static rtti::string_view const name_of_sv_v = rtti::name_of<U> ();

template <typename U>
inline constexpr static rtti::string_type const name_of_str_v = rtti::name_of<U> ();

template <rtti::size_type Hash>
inline constexpr static rtti::const_pointer const name_of_hash_v = rtti::name_of (Hash);

template <rtti::size_type Hash>
inline constexpr static rtti::string_view const name_of_hash_sv_v = rtti::name_of (Hash);

template <rtti::size_type Hash>
inline constexpr static rtti::string_type const name_of_hash_str_v = rtti::name_of (Hash);

template <typename U>
inline constexpr static rtti::const_size type_hash_v = char_hash<rtti::name_of<U> ()> ();

// =========================================================

namespace array {

template <rtti const Array[], rtti::size_type I>
using type_of = type_of<Array[I]>;

} // namespace array

// =========================================================

namespace cstring {

template <rtti::const_pointer STR>
using type_of = rtti::type_of<STR>;

} // namespace cstring

// =========================================================

namespace convertable {

//! Argument type extraction from ABI array
template <rtti const Array[], typename From, rtti::size_type I>
using type_of = type_convertable_to<From, Array[I]>;

} // namespace convertable

// =========================================================

} // namespace abi

// =========================================================

template <std::size_t SZ = def_capture_size_v, typename R, typename... Args>
constexpr
abi::function_rtti<arity_v<sizeof... (Args)>>
make_fn_rtti (function<R(Args...), max_capture_size_v<SZ>> const& fn) noexcept
{
   return abi::function_rtti<arity_v<sizeof... (Args)>> (fn);
}

template <typename R, typename... Args>
constexpr
abi::function_rtti<arity_v<sizeof... (Args)>>
make_fn_rtti (function<R(Args...) const> const& fn) noexcept
{
   return abi::function_rtti<arity_v<sizeof... (Args)>> (fn);
}

// =========================================================

} //! namespace cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_ABI_H_
