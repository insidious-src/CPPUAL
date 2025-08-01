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

#ifndef CPPUAL_UNBOUND_INTERFACE_H_
#define CPPUAL_UNBOUND_INTERFACE_H_
#ifdef __cplusplus

#include <cppual/casts>
#include <cppual/types>
#include <cppual/meta_functional>
#include <cppual/meta_type>
#include <cppual/functional>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/memory_resource>
#include <cppual/concepts>
#include <cppual/string>
#include <cppual/abi>

#include <string_view>
#include <utility>
#include <tuple>
#include <array>

#ifdef DEBUG_MODE
#   include <iostream>
#endif

// ====================================================

namespace cppual {

// =========================================================

consteval std::size_t min_args_count (std::size_t const count = 10) noexcept
{
    return count <= 10 ? 10 : count <= 20 ? 20 : count <= 30 ? 30 : count;
}

// =========================================================

template <class_t C>
class function_proxy
{
public:
    typedef function_proxy<C> self_type          ;
    typedef remove_cptr_t <C> iface_type         ;
    typedef iface_type      * iface_pointer      ;
    typedef iface_type const* iface_const_pointer;
    typedef std::string_view  string_view        ;

    constexpr function_proxy (self_type &&)            noexcept = default;
    constexpr function_proxy (self_type const&)        noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <typename R, non_void_t... Args>
    inline self_type& operator = (function<R(Args...)> const& fn) noexcept
    {
        _M_iface->iface_type::template add_function<R> (_M_fn_name, fn);
        return *this;
    };

    template <typename R, non_void_t... Args>
    constexpr R operator () (R* = void_ptr (), Args... args) const
    { return _M_iface->iface_type::template invoke<R> (_M_fn_name, std::forward<Args> (args)...); }

    // template <non_void_t... Args>
    // constexpr void operator () (Args... args) const
    // { _M_iface->iface_type::template invoke<void> (_M_fn_name, std::forward<Args> (args)...); }

private:
    constexpr function_proxy (iface_type const& interface, string_view fn_name)
    : _M_iface   (&interface)
    , _M_fn_name ( fn_name  )
    { }

    function_proxy () = delete;
    friend iface_type;

private:
    iface_const_pointer _M_iface  ;
    string_view         _M_fn_name;
};



// ====================================================

template <std::size_t N = min_args_count ()>
class function_rtti
{
private:
    inline constexpr static decltype(N) max_arity = min_args_count (N);

public:
    typedef function_rtti<N>                 self_type  ;
    typedef decltype(N)                      size_type  ;
    typedef size_type const                  const_size ;
    typedef function<void()>                 any_fn_type;
    typedef abi::rtti                        rtti_type  ;
    typedef std::array<rtti_type, max_arity> array_type ;

    template <typename S>
    using fn_type = function<S>;

    template <std::size_t SZ>
    using self_type_t = function_rtti<SZ>;

    constexpr function_rtti () noexcept = default;

    template <std::size_t SZ,
              typename = std::enable_if_t<min_args_count (N) >= min_args_count (SZ)>
              >
    constexpr function_rtti (self_type_t<SZ>&& rh) noexcept
    : _M_fn        (rh._M_fn       )
    , _M_arg_types (rh._M_arg_types)
    , _M_ret_type  (rh._M_ret_type )
    {
        static_assert (min_args_count (N) >= min_args_count (SZ),
                       "function_rtti<N> can NOT be initialized with function_rtti<SZ> "
                       "where N < SZ! N must be greater or equal to SZ!");

        rh = self_type_t<SZ> (); // clear the moved object
    }

    template <std::size_t SZ,
              typename = std::enable_if_t<min_args_count (N) >= min_args_count (SZ)>
              >
    constexpr function_rtti (self_type_t<SZ> const& rh) noexcept
    : _M_fn        (rh._M_fn       )
    , _M_arg_types (rh._M_arg_types)
    , _M_ret_type  (rh._M_ret_type )
    {
        static_assert (min_args_count (N) >= min_args_count (SZ),
                       "function_rtti<N> can NOT be assigned from function_rtti<SZ> "
                       "where N < SZ! N must be greater or equal to SZ!");
    }

    template <std::size_t SZ,
              typename = std::enable_if_t<min_args_count (N) >= min_args_count (SZ)>
              >
    constexpr self_type& operator = (self_type_t<SZ>&& rh) noexcept
    {
        static_assert (min_args_count (N) >= min_args_count (SZ),
                       "function_rtti<N> can NOT be assigned from function_rtti<SZ> "
                       "where N < SZ! N must be greater or equal to SZ!");

        if (this == &rh) return *this; // self-assignment check

        rh = self_type_t<SZ> (); // clear the moved object

        _M_fn        = rh._M_fn       ;
        _M_arg_types = rh._M_arg_types;
        _M_ret_type  = rh._M_ret_type ;

        return *this;
    }

    template <std::size_t SZ,
              typename = std::enable_if_t<min_args_count (N) >= min_args_count (SZ)>
              >
    constexpr self_type& operator = (self_type_t<SZ> const& rh) noexcept
    {
        static_assert (min_args_count (N) >= min_args_count (SZ),
                       "function_rtti<N> can NOT be assigned from function_rtti<SZ> "
                       "where N < SZ! N must be greater or equal to SZ!");

        if (this == &rh) return *this; // self-assignment check

        _M_fn        = rh._M_fn       ;
        _M_arg_types = rh._M_arg_types;
        _M_ret_type  = rh._M_ret_type ;

        return *this;
    }

    consteval any_fn_type const&    function () const noexcept { return _M_fn       ; }
    consteval array_type  const&   arg_types () const noexcept { return _M_arg_types; }
    consteval rtti_type   const& return_type () const noexcept { return _M_ret_type ; }

    consteval rtti_type const& arg_type (const_size index) const
    {
        if (index >= min_args_count (N))
            throw std::out_of_range ("function_rtti::arg_type: index out of range!");

        return _M_arg_types[index];
    }

    consteval static size_type arity () noexcept
    { return min_args_count (N); }

    template <typename R, non_void_t... Args>
    constexpr self_type& operator = (fn_type<R(Args...)> const& fn) noexcept
    { return *this = self_type (fn); }

private:
    template <typename R, typename... Args>
    constexpr function_rtti (fn_type<R(Args...)> const& fn)
    : _M_fn        ( fn_cast<void()> (fn)      )
    , _M_arg_types { abi::rtti (type<Args>)... }
    , _M_ret_type  ( type<R>                   )
    { }

    template <size_type>
    friend class function_rtti;

    template <switch_value_t, functional_t...>
    friend class unbound_interface    ;
    friend class dyn_unbound_interface;

private:
    any_fn_type _M_fn        { };
    array_type  _M_arg_types { };
    rtti_type   _M_ret_type  { };
};

//! ====================================================
//! static compile-time !precongnitive reversed interface
//! ====================================================

template <switch_value_t K, functional_t... FNs>
class unbound_interface : public non_copyable
{
public:
    typedef unbound_interface<K, FNs...>          self_type             ;
    typedef self_type                             base_type             ;
    typedef cchar*                                char_ptr              ;
    typedef  uint                                 key_type              ;
    typedef cuint                                 const_key             ;
    typedef std::tuple<std::pair<K, FNs>...>      map_type              ;
    typedef std::size_t                           size_type             ;
    typedef size_type const                       const_size            ;
    typedef std::string_view                      string_view           ;
    typedef string                                string_type           ;
    typedef map_type::iterator                    iterator              ;
    typedef map_type::const_iterator              const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;
    typedef std::pair<iterator, bool>             iterator_pair         ;
    typedef abi::rtti                             rtti_type             ;
    typedef dyn_array<rtti_type>                  rtti_array            ;

    // ====================================================

    template <typename S>
    using fn_type = function<S>;

    template <typename, typename>
    struct fn_signatures_match;

    // ====================================================

    template <class_t C>
    using any_member_fn_type = any_fn_type<C, void()>;

    template <class_t C>
    using any_const_member_fn_type = any_fn_type<C, void() const>;

    template <class_t C, typename R, typename... Args>
    using member_fn_type = any_fn_type<C, R(Args...)>;

    template <class_t C, typename R, typename... Args>
    using const_member_fn_type = any_fn_type<C, R(Args...) const>;

    template <class_t C>
    using any_member_fn_pair = std::pair<key_type, any_member_fn_type<C>>;

    template <class_t C>
    using any_const_member_fn_pair = std::pair<key_type, any_const_member_fn_type<C>>;

    template <class_t C, typename R, typename... Args>
    using member_fn_pair = std::pair<key_type, any_fn_type<C, R(Args...)>>;

    template <class_t C, typename R, typename... Args>
    using const_member_fn_pair = std::pair<key_type, any_fn_type<C, R(Args...) const>>;

    // ====================================================

    inline constexpr static const_size npos = static_cast<size_type> (-1);

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

    //unbound_interface () = delete;
    inline virtual ~unbound_interface () = default;

    // ====================================================

    consteval function_proxy<self_type> operator [] (string_view fn_name) const
    {  return function_proxy<self_type> (*this, fn_name); }

    consteval static size_type size () noexcept
    { return sizeof... (FNs); }

protected:
    /// a pair of Ps... consists of const char* and cppual::function class
    template <pair_t... Ts>
    requires (sizeof... (Ts) <= size () &&
             (functional_pair_t<Ts, typename Ts::first_type, typename Ts::second_type> && ...))
    constexpr unbound_interface (Ts... pairs)
    : _M_fn_map { std::make_pair (char_hash<pairs.first> (), pairs.second)... }
    { }

    template <key_type K_, size_type I = 0>
    consteval size_type get_index () const noexcept
    {
        if constexpr (I < size () && std::get<I> (_M_fn_map).first == K_) return I;

        return (I + 1) < size () ? get_index<K_, I + 1> () : npos;
    }

private:
    template <typename R, typename... Args>
    constexpr void add_function (string_view const& fn_name, fn_type<R(Args...)> const& fn) noexcept
    {
        constexpr const_size I = get_index<char_hash (fn_name.data ())> ();

        static_assert (I != npos, "key NOT found!");
        return std::get<I> (_M_fn_map).second = fn;
    }

    template <typename R, typename... Args>
    constexpr void add_function (string_view const& fn_name, fn_type<R(Args...) const> const& fn) noexcept
    {
        constexpr const_size I = get_index<char_hash (fn_name.data ())> ();

        static_assert (I != npos, "key NOT found!");
        return std::get<I> (_M_fn_map).second = fn;
    }

    template <typename... Args>
    constexpr auto invoke (string_view const& fn_name, Args... args) const noexcept
    {
        constexpr const_size I = get_index<char_hash (fn_name.data ())> ();

        static_assert (I != npos, "key NOT found!");
        return std::get<I> (_M_fn_map).second (std::forward<Args> (args)...);
    }

    // ====================================================

    template <class_t>
    friend class function_proxy;

private:
    map_type _M_fn_map;
};

//! ====================================================
//! dynamic runtime !precongnitive reversed interface
//! ====================================================

class dyn_unbound_interface : public non_copyable
{

public:
    template <typename S>
    using fn_type = function<S>;

    template <typename, typename>
    struct fn_signatures_match;

    // ====================================================

    typedef uint  key_type ;
    typedef cuint const_key;

    // ====================================================

    struct key_hash
    {
        consteval key_type operator () (key_type key) const noexcept
        { return key; }
    };

    struct compare : public std::less<key_type>
    {
        //consteval bool operator () (key_type const& lh, key_type const& rh) const noexcept
        //{ return lh < rh; }
    };

    // ====================================================

    typedef dyn_unbound_interface                 self_type             ;
    typedef self_type                             base_type             ;
    typedef key_hash                              hash_type             ;
    typedef function_rtti<min_args_count ()>      mapped_type           ;
    typedef mapped_type const                     const_mapped          ;
    typedef std::pair<const_key, mapped_type>     value_type            ;
    typedef map<key_type, mapped_type, compare>   map_type              ;
    typedef map_type::allocator_type              allocator_type        ;
    typedef std::allocator_traits<allocator_type> alloc_traits          ;
    typedef alloc_traits::size_type               size_type             ;
    typedef size_type const                       const_size            ;
    typedef std::string_view                      string_view           ;
    typedef string                                string_type           ;
    typedef fn_type<void()>                       any_fn_type           ;
    typedef map_type::iterator                    iterator              ;
    typedef map_type::const_iterator              const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;
    typedef std::pair<iterator, bool>             iterator_pair         ;
    typedef abi::rtti                             rtti_type             ;
    typedef cchar*                                char_ptr              ;
    typedef void *                                void_ptr              ;

    // ====================================================

    template <size_type N>
    using rtti_array = std::array<rtti_type, min_args_count (N)>;

    // ====================================================

    template <class_t>
    friend class function_proxy;

    template <size_type>
    friend class function_rtti;

    // ====================================================

    template <class_t C>
    using any_member_fn_type = void (C::*)();

    template <class_t C>
    using any_const_member_fn_type = void (C::*)() const;

    template <class_t C, typename R, typename... Args>
    using member_fn_type = R (C::*)(Args...);

    template <class_t C, typename R, typename... Args>
    using const_member_fn_type = R (C::*)(Args...) const;

    template <class_t C>
    using any_member_fn_pair = std::pair<key_type, any_member_fn_type<C>>;

    template <class_t C>
    using any_const_member_fn_pair = std::pair<key_type, any_const_member_fn_type<C>>;

    template <class_t C, typename R, typename... Args>
    using member_fn_pair = std::pair<key_type, member_fn_type<C, R, Args...>>;

    template <class_t C, typename R, typename... Args>
    using const_member_fn_pair = std::pair<key_type, const_member_fn_type<C, R, Args...>>;

    // ====================================================

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

    //dyn_unbound_interface () = delete;
    inline virtual ~dyn_unbound_interface () = default;

    constexpr size_type size () const noexcept
    {  return _M_fn_map.size (); }

    // ====================================================

    constexpr function_proxy<self_type> operator [] (string_view const& fn_name) const noexcept
    {  return function_proxy<self_type> (*this, fn_name); }

    template <typename R, typename... Args>
    constexpr void add_function (string_view const& fn_name, fn_type<R(Args...)> const& fn)
    { add_function_base (fn_name, fn); }

    // ====================================================

    inline const_mapped& rtti (const_iterator const& it) const
    {
        if    (it == _M_fn_map.cend ()) throw std::runtime_error ("function_rtti NOT found!");
        return it->second;
    }

    inline mapped_type& rtti (iterator const& it)
    {
        if    (it == _M_fn_map.end ()) throw std::runtime_error ("function_rtti NOT found!");
        return it->second;
    }

    inline const_mapped& rtti (string_view const& fn_name) const
    {
        auto   it  = _M_fn_map.find (char_hash (fn_name.data ()));
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function_rtti NOT found!");
        return it->second;
    }

    inline mapped_type& rtti (string_view const& fn_name)
    {
        auto   it  = _M_fn_map.find (char_hash (fn_name.data ()));
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function_rtti NOT found!");
        return it->second;
    }

protected:
    /// a pair of Ps... consists of const char* and cppual::function class
    template <pair_t... Ps>
    requires (functional_pair_t<Ps,
                                typename Ps::first_type,
                                typename Ps::second_type> && ...)
    constexpr dyn_unbound_interface (Ps... pairs)
    : _M_rc     (sizeof... (Ps) * (sizeof (value_type) + sizeof (rtti_type) * (pairs.second.arity () + ... + 0))),
      _M_fn_map (allocator_type (_M_rc))
    {
        check_pairs_types<std::tuple<Ps...>> ();

        //_M_fn_map.reserve (sizeof... (Ps));

        add_all_member_functions (std::make_tuple (pairs...));

#       ifdef DEBUG_MODE
        std::cout << "unbound_interface size: " << sizeof... (Ps) * sizeof (value_type)
                  << " bytes\nstacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;
#       endif
    }

private:
    template <functional_t FN>
    inline void add_function_base (string_view const& fn_name, FN const& fn)
    {
        iterator_pair const pair = _M_fn_map.insert_or_assign (char_hash (fn_name.data ()), mapped_type (fn));

        if (!pair.second)
        {
            string error_text;

            error_text.reserve (39U + fn_name.size ());

            error_text.append ("insertion of member function '")
                      .append (fn_name.data (), fn_name.size ())
                      .append ("' failed!");

            throw std::runtime_error (error_text.c_str ());
        }
    }

    template <typename Tuple, size_type I = 0>
    constexpr void add_all_member_functions (Tuple const& fn_pairs)
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            add_function (std::get<I> (fn_pairs).first, std::get<I> (fn_pairs).second);

#           ifdef DEBUG_MODE
            std::cout << "stacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;
#           endif

            add_all_member_functions<Tuple, I + 1> (fn_pairs);
        }
    }

    template <typename Tuple, size_type I = 0>
    consteval static void check_pairs_types () noexcept
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            static_assert (std::is_same_v <typename selected_tuple_type<Tuple, I>::first_type, char_ptr> &&
                           is_functional_v<typename selected_tuple_type<Tuple, I>::second_type>,
                           "tuple element is NOT a pair of function name (const char*) and "
                           "member function pointer!");

            check_pairs_types<Tuple, I + 1> ();
        }
    }

    template <typename R,
              non_void_t... Args,
              rtti_type RetExp,
              rtti_array<sizeof... (Args)> ArgsExp>
    consteval static bool check_fn_signature () noexcept
    {
        return RetExp  == rtti_type (type<R>) &&
               ArgsExp == rtti_array<sizeof... (Args)> { type<Args>... };
    }

    template <typename R = void, typename... Args>
    constexpr R invoke (string_view const& fn_name, Args... args) const
    {
        auto const fn_rtti = rtti (fn_name);

        static_assert (check_fn_signature<R, Args..., fn_rtti.return_type (), fn_rtti.arg_types ()> (),
                       "function signature mismatch!");

        return (fn_cast<R(Args...)> (fn_rtti.function ()))(std::forward<Args> (args)...);
    }

private:
    memory::heap_resource _M_rc    ;
    map_type              _M_fn_map;
};

//! ====================================================
//! template adaptor for reversed interface
//! ====================================================

template <class_t T>
struct is_reverse_interface : public std::conditional_t<std::is_base_of_v<dyn_unbound_interface, T>,
                                                        std::true_type,
                                                        std::false_type
                                                        >
{ };

template <switch_value_t K, functional_t... FNs>
struct is_reverse_interface<unbound_interface<K, FNs...>> : public std::true_type
{ };

template <>
struct is_reverse_interface<dyn_unbound_interface> : public std::true_type
{ };

template <class_t T>
using is_reverse_interface_t = is_reverse_interface<T>::type;

template <class_t T>
inline constexpr bool const is_reverse_interface_v = is_reverse_interface<T>::value;

template <typename T>
concept reverse_interface_t = is_reverse_interface_v<T>;

// ====================================================

template <reverse_interface_t T>
class reverse_interface
{
public:
    typedef reverse_interface<T> self_type  ;
    typedef remove_cvrefptr_t<T> iface_type ;
    typedef std::size_t          size_type  ;
    typedef size_type const      const_size ;
    typedef std::string_view     string_view;
    typedef string               string_type;

    template <pair_t... Ps>
    constexpr reverse_interface (Ps... pairs) noexcept
    : _M_iface (pairs...)
    { }

    constexpr size_type size () const noexcept
    { return _M_iface.size (); }

    constexpr auto operator [] (string_view const& fn_name) const
    { return _M_iface[fn_name]; }

private:
    iface_type _M_iface;
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_UNBOUND_INTERFACE_H_
