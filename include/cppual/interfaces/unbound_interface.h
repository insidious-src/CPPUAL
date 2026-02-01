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

#include <cppual/abi>
#include <cppual/decl>
#include <cppual/types>
#include <cppual/casts>
#include <cppual/string>
#include <cppual/concepts>
#include <cppual/array_map>
#include <cppual/meta_type>
#include <cppual/functional>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/meta_string>
#include <cppual/meta_functional>
#include <cppual/memory_resource>
#include <cppual/memory_allocator>

#include <string_view>
#include <utility>
#include <tuple>
//#include <array>

#ifdef DEBUG_MODE
#   include <iostream>
#endif

// =========================================================

namespace cppual {

// =========================================================

template <functional...>
class unbound_interface;
class dyn_unbound_interface;

// =========================================================

typedef std::size_t unbound_key_t;

template <typename T>
concept unbound_key = are_same<T, unbound_key_t>;

// =========================================================

template <structure C>
class function_proxy
{
public:
    typedef function_proxy<C> self_type          ;
    typedef cchar*            const_pointer      ;
    typedef std::size_t       size_type          ;
    typedef size_type const   const_size         ;
    typedef unbound_key_t     key_type           ;
    typedef key_type  const   const_key          ;
    typedef fstring_view      string_view        ;
    typedef remove_cvref_t<C> iface_type         ;
    typedef iface_type      * iface_pointer      ;
    typedef iface_type const* iface_const_pointer;
    typedef iface_type      & iface_ref          ;
    typedef iface_type const& iface_const_ref    ;

    constexpr function_proxy (self_type &&)            noexcept = default;
    constexpr function_proxy (self_type const&)        noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    template <typename R, typename... Args>
    constexpr self_type& operator = (function<R(Args...) const> const& fn) noexcept
    {
        _M_iface->iface_type::template add_function<R> (_M_name, fn);
        return *this;
    };

    template <typename R, typename... Args>
    constexpr self_type& operator = (function<R(Args...)> const& fn) noexcept
    {
        _M_iface->iface_type::template add_function<R> (_M_name, fn);
        return *this;
    };

    template <typename R, typename... Args>
    constexpr R operator () (R*, Args... args) const
    {
        return _M_iface->iface_type::template invoke<R> (_M_name, std::forward<Args> (args)...);
    }

    template <typename... Args>
    constexpr void operator () (Args... args) const
    {
        _M_iface->iface_type::template invoke<void> (_M_name, std::forward<Args> (args)...);
    }

    constexpr string_view name () const noexcept
    { return _M_name; }

    constexpr iface_const_pointer interface () const noexcept
    { return _M_iface; }

private:
    constexpr function_proxy (iface_const_ref interface, string_view fn_name) noexcept
        : _M_iface (&interface)
        , _M_name  (fn_name   )
    { }

    consteval explicit function_proxy (iface_const_ref interface, const_pointer fn_name) noexcept
    : _M_iface (&interface)
    , _M_name  (fn_name   )
    { }

private:
    iface_const_pointer _M_iface { };
    string_view         _M_name  { };

    function_proxy () = delete;
    friend iface_type;
};

//! =======================================================
//! static compile-time !precongnitive reversed interface
//! =======================================================

template <functional... FNs>
class unbound_interface : public non_copyable_virtual
{
public:
    typedef unbound_interface<FNs...>               self_type             ;
    typedef self_type                               base_type             ;
    typedef function_proxy<self_type>               fn_proxy_type         ;
    typedef cchar*                                  char_ptr              ;
    typedef fn_proxy_type::size_type                size_type             ;
    typedef size_type const                         const_size            ;
    typedef unbound_key_t                           key_type              ;
    typedef key_type  const                         const_key             ;
    typedef std::tuple<std::pair<key_type, FNs>...> map_type              ;
    typedef ptrdiff                                 difference_type       ;
    typedef fstring_view                            string_view           ;
    typedef fstring                                 string_type           ;
    typedef map_type::iterator                      iterator              ;
    typedef map_type::const_iterator                const_iterator        ;
    typedef std::reverse_iterator<iterator>         reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>   reverse_const_iterator;
    typedef std::pair<iterator, bool>               iterator_pair         ;

    // ====================================================

    template <typename S, size_type SZ = def_capture_size_v>
    using fn_t = function<S, max_capture_size_v<SZ>>;

    template <size_type I>
    using return_t = selected_tuple_t<map_type, I>::second_type::return_type;

    template <structure C>
    using any_member_fn_t = fn_ptr_t<C, void()>;

    template <structure C>
    using any_const_member_fn_t = fn_ptr_t<C, void() const>;

    template <structure C, fn_sig S>
    using member_fn_t = fn_ptr_t<C, S>;

    template <structure C>
    using any_mem_fn_pair = std::pair<key_type, any_member_fn_t<C>>;

    template <structure C>
    using any_const_mem_fn_pair = std::pair<key_type, any_const_member_fn_t<C>>;

    template <structure C, typename R, typename... Args>
    using mem_fn_pair = std::pair<key_type, member_fn_t<C, R(Args...)>>;

    template <structure C, typename R, typename... Args>
    using const_mem_fn_pair = std::pair<key_type, member_fn_t<C, R(Args...) const>>;

    // ====================================================

    inline constexpr static const_size npos = static_cast<size_type> (-1);

    // ====================================================

    unbound_interface () = delete;

    // ====================================================

    template <key_type... Ks> requires (sizeof... (Ks) >= 2)
    consteval bool contains () const noexcept
    {
        return ((get_index<Ks> () != npos) && ...);
    }

    template <char_ptr... Ks> requires (sizeof... (Ks) >= 2)
    consteval bool contains () const noexcept
    {
        return ((get_index<Ks> () != npos) && ...);
    }

    template <unbound_key... Ks> requires (sizeof... (Ks) >= 2)
    constexpr bool contains (Ks... keys) const noexcept
    {
        return ((get_index (keys) != npos) && ...);
    }

    template <c_const_str... Ks> requires (sizeof... (Ks) >= 2)
    constexpr bool contains (Ks... keys) const noexcept
    {
        return ((get_index (keys) != npos) && ...);
    }

    // ====================================================

    template <key_type K_>
    consteval bool contains () const noexcept
    {
        return get_index<K_> () != npos;
    }

    template <char_ptr K_>
    consteval bool contains () const noexcept
    {
        return get_index<K_> () != npos;
    }

    constexpr bool contains (key_type k) const noexcept
    {
        return get_index (k) != npos;
    }
    constexpr bool contains (char_ptr k) const noexcept
    {
        return get_index (k) != npos;
    }

    // ====================================================

    consteval fn_proxy_type operator [] (string_view const& fn_name) const noexcept
    {  return fn_proxy_type (*this, fn_name); }

    // ====================================================

    consteval static size_type size () noexcept
    { return sizeof... (FNs); }

protected:
    //! a pair of Ps... consists of const char* and cppual::function class
    template <pair_like... Ts>
    requires (sizeof... (Ts) <= size () &&
             (functional_str_pair<Ts, typename Ts::first_type, typename Ts::second_type> && ...))
    consteval unbound_interface (Ts... pairs)
    : _M_fn_map { std::make_pair (char_hash (pairs.first), pairs.second)... }
    { }

    // ====================================================

    template <key_type K>
    consteval size_type get_index () const noexcept
    {
        constexpr const_size I = K % size ();

        return std::get<I> (_M_fn_map).first == K ? I : npos;
    }

    template <char_ptr STR_>
    consteval size_type get_index () const noexcept
    {
        constexpr const_key  K = char_hash<STR_> ();
        constexpr const_size I = K % size ();

        return std::get<I> (_M_fn_map).first == K ? I : npos;
    }

    template <string_view STR_>
    consteval size_type get_index () const noexcept
    {
        constexpr const_key  K = char_hash<STR_.data ()> ();
        constexpr const_size I = K % size ();

        return std::get<I> (_M_fn_map).first == K ? I : npos;
    }

    constexpr size_type get_index (const_key k) const noexcept
    {
        constexpr const_size I = k % size ();

        return std::get<I> (_M_fn_map).first == k ? I : npos;
    }

    constexpr size_type get_index (char_ptr k) const noexcept
    {
        const_key key = char_hash (k);
        constexpr const_size I = key % size ();

        return std::get<I> (_M_fn_map).first == key ? I : npos;
    }

    constexpr size_type get_index (string_view k) const noexcept
    {
        const_key key = char_hash (k.data ());
        constexpr const_size I = key % size ();

        return std::get<I> (_M_fn_map).first == key ? I : npos;
    }

private:
    template <typename R, typename... Args>
    constexpr void add_function (string_view const& fn_name, fn_t<R(Args...) const> const& fn) noexcept
    {
        constexpr const_size I = get_index<fn_name.data ()> ();

        if (I == npos) std::get<I> (_M_fn_map) = std::pair { char_hash (fn_name.data ()), fn };
    }

    template <size_type SZ = def_capture_size_v, typename R, typename... Args>
    constexpr void add_function (string_view const& fn_name, fn_t<R(Args...), SZ> const& fn) noexcept
    {
        constexpr const_size I = get_index<fn_name.data ()> ();

        if (I == npos) std::get<I> (_M_fn_map) = std::pair { char_hash (fn_name.data ()), fn };
    }

    template <typename R, typename... Args>
    constexpr void add_function (char_ptr fn_name, fn_t<R(Args...) const> const& fn) noexcept
    {
        constexpr const_size I = get_index<fn_name> ();

        if (I == npos) std::get<I> (_M_fn_map) = std::pair { char_hash (fn_name), fn };
    }

    template <size_type SZ = def_capture_size_v, typename R, typename... Args>
    constexpr void add_function (char_ptr fn_name, fn_t<R(Args...), SZ> const& fn) noexcept
    {
        constexpr const_size I = get_index<fn_name> ();

        if (I == npos) std::get<I> (_M_fn_map) = std::pair { char_hash (fn_name), fn };
    }

    // ====================================================

    template <typename R = void, typename... Args, c_const_str STR>
    consteval R invoke (STR fn_name, Args&&... args) const noexcept
    {
        constexpr const_size I = get_index<fn_name> ();

        typedef
        callable_return_t<typename selected_tuple_t<map_type, I>::second_type, Args...>
        return_type;

        static_assert (std::convertible_to<return_type, R>, "return type mismatch!");

        static_assert (I != npos, "key NOT found!");
        return std::get<I> (_M_fn_map).second (std::forward<Args> (args)...);
    }

    // ====================================================

    template <structure>
    friend class function_proxy;

private:
    map_type _M_fn_map;
};

//! ====================================================
//! dynamic runtime !precongnitive reversed interface
//! ====================================================

class dyn_unbound_interface : public non_copyable_virtual
{

public:
    template <fn_sig S, std::size_t SZ = def_capture_size_v>
    using fn_t = function<S, max_capture_size_v<SZ>>;

    // ====================================================

    typedef dyn_unbound_interface                 self_type             ;
    typedef self_type                             base_type             ;
    typedef function_proxy<self_type>             fn_proxy_type         ;
    typedef fn_t<void()>                          fn_type               ;
    typedef fn_proxy_type::size_type              size_type             ;
    typedef size_type const                       const_size            ;
    typedef unbound_key_t                         key_type              ;
    typedef key_type const                        const_key             ;
    typedef abi::function_rtti<>                  mapped_type           ;
    typedef mapped_type const                     const_mapped          ;
    typedef std::pair<key_type, mapped_type>      value_type            ;
    typedef value_type const                      const_value           ;
    typedef dyn_index_map<key_type, mapped_type>  container_type        ;
    typedef container_type::allocator_type        allocator_type        ;
    typedef std::allocator_traits<allocator_type> alloc_traits          ;
    typedef ptrdiff                               difference_type       ;
    typedef fstring_view                          string_view           ;
    typedef string                                string_type           ;
    typedef container_type::iterator              iterator              ;
    typedef container_type::const_iterator        const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;
    typedef mapped_type::array_type               array_type            ;
    typedef mapped_type::array_ref                array_ref             ;
    typedef mapped_type::array_const_ref          array_const_ref       ;
    typedef std::pair<iterator, bool>             iterator_pair         ;
    typedef abi::rtti                             rtti_type             ;
    typedef rtti_type const                       const_rtti            ;
    typedef cchar*                                char_ptr              ;
    typedef void *                                void_ptr              ;

    // ====================================================

    template <structure>
    friend class function_proxy;

    template <size_type>
    friend class function_rtti;

    // ====================================================

    template <structure C>
    using any_member_fn_t = fn_ptr_t<C, void()>;

    template <structure C>
    using any_const_member_fn_t = fn_ptr_t<C, void() const>;

    template <structure C, fn_sig S>
    using member_fn_t = fn_ptr_t<C, S>;

    template <structure C>
    using any_mem_fn_pair = std::pair<key_type, any_member_fn_t<C>>;

    template <structure C>
    using any_const_mem_fn_pair = std::pair<key_type, any_const_member_fn_t<C>>;

    template <structure C, typename R, typename... Args>
    using mem_fn_pair = std::pair<key_type, member_fn_t<C, R(Args...)>>;

    template <structure C, typename R, typename... Args>
    using const_mem_fn_pair = std::pair<key_type, member_fn_t<C, R(Args...) const>>;

    // ====================================================

    inline constexpr static const_size npos = static_cast<size_type> (-1);

    // ====================================================

    consteval fn_proxy_type operator [] (string_view const& fn_name) const noexcept
    {  return fn_proxy_type (*this, fn_name); }

    // ====================================================

    constexpr size_type size () const noexcept
    {  return _M_fn_map.size (); }

    template <typename R, typename... Args>
    constexpr void add_function (string_view const& fn_name, fn_t<R(Args...) const> const& fn) noexcept
    {
        _M_fn_map[fn_name].second = make_fn_rtti (fn);
    }

    template <size_type SZ = def_capture_size_v, typename R, typename... Args>
    constexpr void add_function (string_view const& fn_name, fn_t<R(Args...), SZ> const& fn) noexcept
    {
        _M_fn_map[fn_name].second = make_fn_rtti (fn);
    }

    // ====================================================

    constexpr const_mapped& fn_rtti (string_view const& fn_name) const noexcept
    {
        return _M_fn_map[fn_name].second;
    }

    constexpr mapped_type& fn_rtti (string_view const& fn_name) noexcept
    {
        return _M_fn_map[fn_name].second;
    }

    template <char_ptr FN_NAME>
    constexpr const_mapped& fn_rtti () const noexcept
    {
        return _M_fn_map[FN_NAME].second;
    }

    template <char_ptr FN_NAME>
    constexpr mapped_type& fn_rtti () noexcept
    {
        return _M_fn_map[FN_NAME].second;
    }

    // ====================================================

    template <char_ptr Name>
    constexpr rtti_type return_type () const noexcept
    {
        return _M_fn_map[Name].second.return_type ();
    }

    constexpr rtti_type return_type (char_ptr Name) const noexcept
    {
        return _M_fn_map[Name].second.return_type ();
    }

    template <key_type K>
    constexpr rtti_type return_type () const noexcept
    {
        return _M_fn_map[K].second.return_type ();
    }

    constexpr rtti_type return_type (key_type k) const noexcept
    {
        return _M_fn_map[k].second.return_type ();
    }

    template <string_view K>
    constexpr rtti_type return_type () const noexcept
    {
        return _M_fn_map[K].second.return_type ();
    }

    constexpr rtti_type return_type (string_view k) const noexcept
    {
        return _M_fn_map[k].second.return_type ();
    }

    template <char_ptr Name>
    constexpr array_const_ref arg_types () const noexcept
    {
        return _M_fn_map[Name].second.arg_types ();
    }

    constexpr array_const_ref arg_types (char_ptr Name) const noexcept
    {
        return _M_fn_map[Name].second.arg_types ();
    }

    template <key_type K>
    constexpr array_const_ref arg_types () const noexcept
    {
        return _M_fn_map[K].second.arg_types ();
    }

    constexpr array_const_ref arg_types (key_type k) const noexcept
    {
        return _M_fn_map[k].second.arg_types ();
    }

    template <string_view K>
    constexpr array_const_ref arg_types () const noexcept
    {
        return _M_fn_map[K].second.arg_types ();
    }

    constexpr array_const_ref arg_types (string_view k) const noexcept
    {
        return _M_fn_map[k].second.arg_types ();
    }

    // ====================================================

    template <unbound_key... Keys>
    consteval bool contains (Keys... keys) const noexcept
    {
        return ((contains<keys> ()) && ...);
    }

    template <c_const_str... Keys>
    consteval bool contains (Keys... keys) const noexcept
    {
        return ((contains<keys> ()) && ...);
    }

    template <key_type K>
    consteval bool contains () const noexcept
    {
        return _M_fn_map[K].first == K;
    }

    template <char_ptr STR>
    consteval bool contains () const noexcept
    {
        return _M_fn_map[STR].first == char_hash<STR> ();
    }

    constexpr bool contains (key_type k) const noexcept
    {
        return _M_fn_map[k].first == k;
    }

    constexpr bool contains (char_ptr str) const noexcept
    {
        return _M_fn_map[str].first == char_hash (str);
    }

    template <typename R = void, typename... Args, size_type... Is, c_const_str STR>
    constexpr R invoke (STR       fn_name,
                        Args&&... args   ,
                        std::index_sequence<Is...> = std::index_sequence_for<Args...> ())
    {
        constexpr const_size      Key           = char_hash<fn_name> ();
        constexpr const_rtti      return_type_v = return_type<Key>   ();
        constexpr array_const_ref arg_types_v   = arg_types  <Key>   ();

        static_assert (arg_types_v.size () <= sizeof...(Args), "function argument count mismatch!");

        typedef abi::type_convertable_t<R, return_type_v> (fn_sign)
               (abi::arg_type_t<arg_types_v.data (), Args, Is>...);

        return (fn_cast<fn_sign> (_M_fn_map[Key].second.function ()))(std::forward<Args> (args)...);
    }

protected:
    //! a pair of Ps... consists of const char* and cppual::function class
    template <pair_like... Ps>
    requires (functional_str_pair<Ps, typename Ps::first_type, typename Ps::second_type> && ...)
    constexpr dyn_unbound_interface (Ps... pairs)
    : _M_rc     (sizeof... (Ps) * sizeof (value_type)),
      _M_fn_map (sizeof... (Ps), allocator_type (_M_rc))
    {
        check_pairs_types<0, Ps...> ();

        add_all_member_functions (std::make_tuple (pairs...));

#       ifdef DEBUG_MODE
        std::cout << "unbound_interface size: " << sizeof... (Ps) * sizeof (value_type)
                  << " bytes\nstacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;
#       endif
    }

private:
    template <size_type I = 0, pair_like... Ps>
    requires (functional_str_pair<Ps, typename Ps::first_type, typename Ps::second_type> && ...)
    constexpr void add_all_member_functions (std::tuple<Ps...> const& fn_pairs)
    {
        if constexpr (I < sizeof... (Ps))
        {
            add_function (std::get<I> (fn_pairs).first, std::get<I> (fn_pairs).second);

#           ifdef DEBUG_MODE
            std::cout << "stacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;
#           endif

            add_all_member_functions<I + 1, Ps...> (fn_pairs);
        }
    }

    template <size_type I, pair_like... Ps>
    requires (functional_str_pair<Ps, typename Ps::first_type, typename Ps::second_type> && ...)
    consteval static void check_pairs_types () noexcept
    {
        typedef std::tuple<Ps...> tuple_type;

        if constexpr (I < sizeof... (Ps))
        {
            static_assert (std::is_same_v <typename selected_tuple_t<tuple_type, I>::first_type, char_ptr> &&
                           is_functional_v<typename selected_tuple_t<tuple_type, I>::second_type>,
                           "tuple element is NOT a pair of function name (const char*) and "
                           "function class!");

            check_pairs_types<I + 1, Ps...> ();
        }
    }

private:
    memory::stacked_resource _M_rc    ;
    container_type           _M_fn_map;
};

//! ====================================================
//! template adaptor for reversed interface
//! ====================================================

template <structure T>
struct is_reverse_interface : public std::conditional_t<std::is_base_of_v<dyn_unbound_interface, T>,
                                                        std::true_type,
                                                        std::false_type
                                                        >
{ };

template <functional... FNs>
struct is_reverse_interface <unbound_interface<FNs...>> : public std::true_type
{ };

template <>
struct is_reverse_interface <dyn_unbound_interface> : public std::true_type
{ };

template <structure T>
using is_reverse_interface_t = is_reverse_interface<T>::type;

template <structure T>
inline constexpr cbool is_reverse_interface_v = is_reverse_interface<T>::value;

template <typename T>
concept reverse_interface_t = is_reverse_interface_v<T>;

// ====================================================

template <reverse_interface_t T>
class reverse_interface
{
public:
    typedef reverse_interface<T>       self_type    ;
    typedef remove_cvrefptr_t<T>       iface_type   ;
    typedef std::size_t                size_type    ;
    typedef size_type const            const_size   ;
    typedef std::string_view           string_view  ;
    typedef string                     string_type  ;
    typedef function_proxy<iface_type> fn_proxy_type;

    template <pair_like... Ps>
    constexpr reverse_interface (Ps... pairs) noexcept
    : _M_iface (pairs...)
    { }

    constexpr size_type size () const noexcept
    { return _M_iface.size (); }

    constexpr fn_proxy_type operator [] (string_view const& fn_name) const noexcept
    {  return fn_proxy_type (_M_iface, fn_name); }

private:
    iface_type _M_iface;
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_UNBOUND_INTERFACE_H_
