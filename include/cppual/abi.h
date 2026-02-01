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

//#include <source_location>
//#include <version>

// =========================================================

namespace cppual { namespace abi {

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

    consteval rtti ()                                  noexcept = default;
    constexpr rtti (self_type &&)                      noexcept = default;
    constexpr rtti (self_type const&)                  noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <typename U = void>
    consteval rtti (type_ptr_t<U>) noexcept
    : _M_type_hash (char_hash<type_name<U> ()> ())
    { }

    template <typename U>
    constexpr self_type& operator = (type_ptr_t<U>) noexcept
    { _M_type_hash = char_hash<type_name<U> ()> (); return *this; }

    consteval const_pointer name () const noexcept
    { return type_name (_M_type_hash); }

    consteval size_type hash_code () const noexcept
    { return _M_type_hash; }

    template <typename U = void>
    consteval static const_pointer type_name () noexcept
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
        else if constexpr (array_like<U>) return type_name<remove_array_t<U>> () + "[]";
        else if constexpr (is_ref_v<U>) return type_name<remove_ref_t<U>> () + " &";
        else if constexpr (is_cref_v<U>) return type_name<remove_cref_t<U>> () + " const &";
        else if constexpr (is_refptr_v<U>) return type_name<remove_refptr_t<U>> () + " &*";
        else if constexpr (is_crefptr_v<U>) return type_name<remove_crefptr_t<U>> () + " const &*";
        else if constexpr (ptr<U>) return type_name<remove_ptr_t<U>> () + " *";
        else if constexpr (member_function<U>) return "member function pointer";
        else if constexpr (static_function<U>) return "function";
        else if constexpr (structure<U>) return "class";
        else if constexpr (union_like<U>) return "union";
        else if constexpr (enumeration<U>) return "enumeration";

        return "void";
    }

    // =========================================================

    consteval static const_pointer type_name (size_type hash) noexcept
    {
        switch (hash)
        {
        case char_hash (type_name<cvoid> ()): return type_name<cvoid> ();
        case char_hash (type_name<bool> ()): return type_name<bool> ();
        case char_hash (type_name<cbool> ()): return type_name<cbool> ();
        case char_hash (type_name<char> ()): return type_name<char> ();
        case char_hash (type_name<cchar> ()): return type_name<cchar> ();
        case char_hash (type_name<uchar> ()): return type_name<uchar> ();
        case char_hash (type_name<cuchar> ()): return type_name<cuchar> ();
        case char_hash (type_name<char8> ()): return type_name<char8> ();
        case char_hash (type_name<cchar8> ()): return type_name<cchar8> ();
        case char_hash (type_name<char16> ()): return type_name<char16> ();
        case char_hash (type_name<cchar16> ()): return type_name<cchar16> ();
        case char_hash (type_name<char32> ()): return type_name<char32> ();
        case char_hash (type_name<cchar32> ()): return type_name<cchar32> ();
        case char_hash (type_name<wchar> ()): return type_name<wchar> ();
        case char_hash (type_name<cwchar> ()): return type_name<cwchar> ();
        case char_hash (type_name<short> ()): return type_name<short> ();
        case char_hash (type_name<cshort> ()): return type_name<cshort> ();
        case char_hash (type_name<ushort> ()): return type_name<ushort> ();
        case char_hash (type_name<cushort> ()): return type_name<cushort> ();
        case char_hash (type_name<int> ()): return type_name<int> ();
        case char_hash (type_name<cint> ()): return type_name<cint> ();
        case char_hash (type_name<uint> ()): return type_name<uint> ();
        case char_hash (type_name<cuint> ()): return type_name<cuint> ();
        case char_hash (type_name<long> ()): return type_name<long> ();
        case char_hash (type_name<clong> ()): return type_name<clong> ();
        case char_hash (type_name<ulong> ()): return type_name<ulong> ();
        case char_hash (type_name<culong> ()): return type_name<culong> ();
        case char_hash (type_name<long64> ()): return type_name<long64> ();
        case char_hash (type_name<clong64> ()): return type_name<clong64> ();
        case char_hash (type_name<ulong64> ()): return type_name<ulong64> ();
        case char_hash (type_name<culong64> ()): return type_name<culong64> ();
        case char_hash (type_name<float> ()): return type_name<float> ();
        case char_hash (type_name<cfloat> ()): return type_name<cfloat> ();
        case char_hash (type_name<double> ()): return type_name<double> ();
        case char_hash (type_name<cdouble> ()): return type_name<cdouble> ();
        case char_hash (type_name<ldouble> ()): return type_name<ldouble> ();
        case char_hash (type_name<cldouble> ()): return type_name<cldouble> ();
        case char_hash ("[]"): return "[]";
        case char_hash (" *"): return " *";
        case char_hash ("member function pointer"): return "member function pointer";
        case char_hash ("function"): return "function";
        case char_hash ("class"): return "class";
        case char_hash ("union"): return "union";
        case char_hash ("enumeration"): return "enumeration";
        }

        return type_name<void> ();
    }

    // =========================================================

    template <const_pointer STR = "void">
    using type_t =
    std::conditional_t<char_hash<STR> () == char_hash (type_name<char> ()), char,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cchar> ()), cchar,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<uchar> ()), uchar,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cuchar> ()), cuchar,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<char8> ()), char8,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cchar8> ()), cchar8,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<char16> ()), char16,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cchar16> ()), cchar16,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<char32> ()), char32,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cchar32> ()), cchar32,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<wchar> ()), wchar,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cwchar> ()), cwchar,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<bool> ()), bool,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cbool> ()), cbool,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<short> ()), short,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cshort> ()), cshort,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<ushort> ()), ushort,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cushort> ()), cushort,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<int> ()), int,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cint> ()), cint,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<uint> ()), uint,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cuint> ()), cuint,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<long> ()), long,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<clong> ()), clong,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<ulong> ()), ulong,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<culong> ()), culong,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<long64> ()), long64,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<clong64> ()), clong64,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<ulong64> ()), ulong64,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<culong64> ()), culong64,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<float> ()), float,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cfloat> ()), cfloat,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<double> ()), double,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cdouble> ()), cdouble,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<ldouble> ()), ldouble,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cldouble> ()), cldouble,
    std::conditional_t<char_hash<STR> () == char_hash (type_name<cvoid> ()), cvoid,
    std::enable_if_t  <char_hash<STR> () == char_hash (type_name<void> ()), void
    >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>;

public:
    size_type _M_type_hash { char_hash (type_name<void> ()) };
};

// =========================================================

template <typename U>
inline constexpr static rtti::const_pointer const typename_v = rtti::type_name<U> ();

template <rtti::size_type Hash>
inline constexpr static rtti::const_pointer const typename_hash_v = rtti::type_name (Hash);

template <typename U>
inline constexpr static rtti::string_view const typename_sv_v = rtti::type_name<U> ();

template <rtti::size_type Hash>
inline constexpr static rtti::string_view const typename_hash_sv_v = rtti::type_name (Hash);

template <typename U>
inline constexpr static rtti::string_type const typename_str_v = rtti::type_name<U> ();

template <rtti::size_type Hash>
inline constexpr static rtti::string_type const typename_hash_str_v = rtti::type_name (Hash);

template <typename U>
inline constexpr static rtti::const_size type_hash_v = char_hash<rtti::type_name<U> ()> ();

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
    typedef abi::rtti                           rtti_type      ;
    typedef static_array<rtti_type, arity_v<N>> array_type     ;
    typedef array_type &                        array_ref      ;
    typedef array_type const&                   array_const_ref;

    template <fn_sig S, size_type SZ = def_capture_size_v>
    using fn_t = function<S, max_capture_size_v<SZ>>;

    template <size_type SZ>
    using self_type_t = function_rtti<arity_v<SZ>>;

    consteval static size_type arity () noexcept
    { return N; }

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

    constexpr fn_const_ref     function () const noexcept { return _M_fn       ; }
    constexpr array_const_ref arg_types () const noexcept { return _M_arg_types; }
    constexpr rtti_type     return_type () const noexcept { return _M_ret_type ; }

    template <size_type I>
    consteval rtti_type arg_type () const noexcept
    {
        static_assert (I < arity_v<N>, "index out of range!");
        return _M_arg_types[I];
    }

private:
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

    template <size_type SZ, typename R, typename... Args>
    friend
    constexpr
    self_type_t<sizeof... (Args)>
    make_fn_rtti (fn_t<R(Args...), SZ> const&) noexcept;

    template <typename R, typename... Args>
    friend
    constexpr
    self_type_t<sizeof... (Args)>
    make_fn_rtti (fn_t<R(Args...) const> const&) noexcept;

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
using type_t = rtti::type_t<Type.name ()>;

//! Type convertability check and extraction - argument convertable on std::forward
template <typename From, rtti const To>
using type_convertable_t = std::enable_if_t<std::convertible_to<From, type_t<To>>, type_t<To>>;

//! Argument type extraction from ABI array
template <rtti const Array[], typename From, rtti::size_type I = 0>
using arg_type_t = abi::type_convertable_t<From, Array[I]>;

// =========================================================

} // namespace abi

// ====================================================

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
