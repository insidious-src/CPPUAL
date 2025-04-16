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

#include <cppual/cast.h>
#include <cppual/types.h>
#include <cppual/fn_meta.h>
#include <cppual/functional>
#include <cppual/containers>
#include <cppual/noncopyable.h>
#include <cppual/memory/heap.h>
#include <cppual/memory/stacked.h>
#include <cppual/concepts>

#include <string_view>
#include <typeindex>
#include <utility>
#include <tuple>

#ifdef DEBUG_MODE
#   include <iostream>
#endif

// ====================================================

namespace cppual {

//! ====================================================
//! static compile-time reversed interface
//! ====================================================

template <pair_t... Ps>
requires (member_fn_pair_t<Ps, typename Ps::first_type, typename Ps::second_type> && ...)
class unbound_interface : public non_copyable_virtual
{
public:
    class function_proxy;

    template <typename S>
    using fn_type = function<S>;

    template <typename, typename>
    struct fn_signatures_match;


    // ====================================================

    typedef unbound_interface<Ps...>                        self_type             ;
    typedef self_type                                       base_type             ;
    typedef cchar*                                          char_ptr              ;
    typedef  uint                                           key_type              ;
    typedef cuint                                           const_key             ;
    typedef std::tuple<Ps...>                               map_type              ;
    typedef std::size_t                                     size_type             ;
    typedef size_type const                                 const_size            ;
    typedef std::string_view                                string_view           ;
    typedef string                                          string_type           ;
    typedef map_type::iterator                              iterator              ;
    typedef map_type::const_iterator                        const_iterator        ;
    typedef std::reverse_iterator<iterator>                 reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>           reverse_const_iterator;
    typedef std::pair<iterator, bool>                       iterator_pair         ;
    typedef std::type_index                                 typeid_type           ;
    typedef dyn_array<typeid_type>                          typeid_vector         ;

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

    template <typename R>
    struct return_type
    {
        typedef R  type   ;
        typedef R* pointer;

        inline constexpr static pointer const value = nullptr;
    };

    template <typename R>
    using return_t = return_type<R>::type;

    template <typename R>
    using return_ptr = return_type<R>::pointer;

    template <typename R>
    inline constexpr static return_ptr<R> const ret = return_type<R>::value;

    // ====================================================

    inline constexpr static size_type const npos = static_cast<size_type> (-1);

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

    virtual string_view provides () const = 0;

    // ====================================================

    class function_proxy
    {
    public:
        typedef function_proxy self_type;

        constexpr function_proxy (self_type &&) noexcept = default;
        consteval function_proxy (self_type const&) noexcept = default;
        constexpr self_type& operator = (self_type &&) noexcept = default;
        constexpr self_type& operator = (self_type const&) noexcept = default;

        template <non_void_t... Args>
        constexpr auto operator () (Args... args) const noexcept
        {
            return _M_iface->call (_M_fn_name, std::forward<Args> (args)...);
        }

        template <pair_t...>
        friend class unbound_interface;

    private:
        constexpr function_proxy (base_type const& interface, string_view const& fn_name)
        : _M_iface   (&interface),
          _M_fn_name (fn_name   )
        { }

        function_proxy () = delete;

    private:
        base_type const* _M_iface  ;
        string_view      _M_fn_name;
    };

    // ====================================================

    unbound_interface () = delete;
    inline virtual ~unbound_interface () = default;

    template <key_type K_, size_type I = 0>
    consteval size_type get_index () const noexcept
    {
        if (I < size () && std::get<I> (_M_fn_map).first == K_) return I;

        return (I + 1) < size () ? get_index<K_, I + 1> () : npos;
    }

    template <class_t D = self_type, typename... Args>
    constexpr auto call (string_view const& fn_name, Args... args) const
    {
        constexpr const_size I = get_index<consteval_char_hash<fn_name.data ()>> ();

        static_assert (I != npos, "key NOT found!");
        return (class_cast<D> (*this).*std::get<I> (_M_fn_map).second)(std::forward<Args> (args)...);
    }

    constexpr function_proxy operator [] (string_view const& fn_name) const
    {
        return function_proxy (*this, fn_name);
    }

    consteval static size_type size () noexcept
    {
        return sizeof... (Ps);
    }

protected:
    /// a pair of Ps... consists of const char* and
    /// member function pointer (ex. void(object::*)())
    template <pair_t... Ts>
    requires (sizeof... (Ts) <= size () &&
             (member_fn_pair_t<Ts, typename Ts::first_type, typename Ts::second_type> && ...))
    consteval unbound_interface (Ts... pairs)
    : _M_fn_map { std::make_pair (consteval_char_hash<pairs.first> (), pairs.second)... }
    { }

private:
    map_type _M_fn_map;
};

//! ====================================================
//! dynamic runtime reversed interface
//! ====================================================

class dyn_unbound_interface : public non_copyable_virtual
{
public:
    class function_proxy;
    class function_rtti ;

    template <typename S>
    using fn_type = function<S>;

    template <typename, typename>
    struct fn_signatures_match;

    typedef cchar* char_ptr ;
    typedef uint   key_type ;
    typedef cuint  const_key;

    // ====================================================

    struct key_hash
    {
        constexpr key_type operator () (key_type key) const noexcept
        {
            return key;
        }
    };

    // ====================================================

    typedef dyn_unbound_interface                           self_type             ;
    typedef self_type                                       base_type             ;
    typedef key_hash                                        hash_type             ;
    typedef function_rtti                                   mapped_type           ;
    typedef mapped_type const                               const_mapped          ;
    typedef std::pair<const_key, mapped_type>               value_type            ;
    typedef unordered_map<key_type, mapped_type, hash_type> map_type              ;
    typedef map_type::allocator_type                        allocator_type        ;
    typedef std::allocator_traits<allocator_type>           alloc_traits          ;
    typedef alloc_traits::size_type                         size_type             ;
    typedef size_type const                                 const_size            ;
    typedef std::string_view                                string_view           ;
    typedef string                                          string_type           ;
    typedef fn_type<void()>                                 any_fn_type           ;
    typedef map_type::iterator                              iterator              ;
    typedef map_type::const_iterator                        const_iterator        ;
    typedef std::reverse_iterator<iterator>                 reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>           reverse_const_iterator;
    typedef std::pair<iterator, bool>                       iterator_pair         ;
    typedef std::type_index                                 typeid_type           ;
    typedef dyn_array<typeid_type>                          typeid_vector         ;

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

    template <typename R>
    struct return_type
    {
        typedef R  type   ;
        typedef R* pointer;

        inline constexpr static pointer const value = nullptr;
    };

    template <typename R>
    using return_t = return_type<R>::type;

    template <typename R>
    using return_ptr = return_type<R>::pointer;

    template <typename R>
    inline constexpr static return_ptr<R> const ret = return_type<R>::value;

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

    virtual string_view provides () const = 0;

    // ====================================================

    class function_rtti
    {
    public:
        typedef function_rtti self_type;

        constexpr function_rtti (self_type &&)             = default;
        constexpr function_rtti (self_type const&)         = default;
        inline    self_type& operator = (self_type &&)     = default;
        inline    self_type& operator = (self_type const&) = default;

        constexpr any_fn_type   const&    function () const noexcept { return _M_fn       ; }
        constexpr typeid_vector const&   arg_types () const noexcept { return _M_arg_types; }
        constexpr typeid_type   const& return_type () const noexcept { return _M_ret_type ; }

    private:
        //! template parameter C is a derived class from unbound_interface
        template <class_t C, typename R, typename... Args,
                  typename = std::enable_if_t<std::is_base_of_v<base_type, C>>
                  >
        constexpr function_rtti (C& obj, member_fn_type<C, R, Args...> fn)
        : _M_fn        (fn_cast<void()> (fn_type<R(Args...)> (obj, fn))),
          _M_arg_types ({ typeid_type (typeid (Args))... },
                          typeid_vector::allocator_type (class_cast<base_type> (obj)._M_rc)),
          _M_ret_type  (typeid (R))
        { }

        //! template parameter C is a derived class from unbound_interface
        template <class_t C, typename R, typename... Args,
                  typename = std::enable_if_t<std::is_base_of_v<base_type, C>>
                  >
        constexpr function_rtti (C& obj, const_member_fn_type<C, R, Args...> fn)
        : _M_fn        (fn_cast<void()> (fn_type<R(Args...)> (obj, fn))),
          _M_arg_types ({ typeid_type (typeid (Args))... },
                          typeid_vector::allocator_type (class_cast<base_type> (obj)._M_rc)),
          _M_ret_type  (typeid (R))
        { }

        function_rtti () = delete;
        friend class dyn_unbound_interface;

    private:
        any_fn_type   _M_fn       ;
        typeid_vector _M_arg_types;
        typeid_type   _M_ret_type ;
    };

    // ====================================================

    class function_proxy
    {
    public:
        typedef function_proxy self_type;

        constexpr function_proxy (self_type &&) noexcept = default;
        consteval function_proxy (self_type const&) noexcept = default;
        constexpr self_type& operator = (self_type &&) noexcept = default;
        constexpr self_type& operator = (self_type const&) noexcept = default;

        template <non_void_t... Args>
        constexpr void operator () (Args... args) const
        { _M_iface->call<void> (_M_fn_name, std::forward<Args> (args)...); }

        template <typename R, non_void_t... Args>
        constexpr R operator () (Args... args, R* = ret<void>) const
        { return _M_iface->call<R> (_M_fn_name, std::forward<Args> (args)...); }

    private:
        constexpr function_proxy (base_type const& interface, string_view const& fn_name)
        : _M_iface   (&interface),
          _M_fn_name (fn_name   )
        { }

        function_proxy () = delete;
        friend class dyn_unbound_interface;

    private:
        base_type const* _M_iface  ;
        string_view      _M_fn_name;
    };

    // ====================================================

    dyn_unbound_interface () = delete;
    inline virtual ~dyn_unbound_interface () = default;

    constexpr size_type size () const noexcept
    {  return _M_fn_map.size (); }

    // ====================================================

    constexpr function_proxy operator [] (string_view const& fn_name) const
    {  return function_proxy (*this, fn_name); }

    template <class_t C, typename R, typename... Args>
    constexpr void add_member_function (string_view const& fn_name, R(C::* fn)(Args...))
    { add_member_function_base<C> (fn_name, fn); }

    template <class_t C, typename R, typename... Args>
    constexpr void add_member_function (string_view const& fn_name, R(C::* fn)(Args...) const)
    { add_member_function_base<C> (fn_name, fn); }

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
        auto   it  = _M_fn_map.find (constexpr_char_hash (fn_name.data ()));
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function_rtti NOT found!");
        return it->second;
    }

    inline mapped_type& rtti (string_view const& fn_name)
    {
        auto   it  = _M_fn_map.find (constexpr_char_hash (fn_name.data ()));
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function_rtti NOT found!");
        return it->second;
    }

protected:
    /// a pair of Ps... consists of const char* and
    /// member function pointer (ex. void(object::*)())
    template <typename... Ps>
    requires (member_fn_pair_t<Ps, typename Ps::first_type, typename Ps::second_type> && ...)
    constexpr dyn_unbound_interface (Ps... pairs)
    : _M_rc     (sizeof... (Ps) * (sizeof (value_type) + sizeof (typeid_type) * 10)),
      _M_fn_map (allocator_type (_M_rc))
    {
        check_pairs_types<std::tuple<Ps...>> ();

        _M_fn_map.reserve (sizeof... (Ps));

        add_all_member_functions (std::make_tuple (pairs...));

#       ifdef DEBUG_MODE
        std::cout << "unbound_interface size: " << sizeof... (Ps) * sizeof (value_type)
                  << " bytes\nstacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;
#       endif
    }

private:
    template <typename Tuple, size_type I = 0>
    constexpr void add_all_member_functions (Tuple const& fn_pairs)
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            add_member_function (std::get<I> (fn_pairs).first, std::get<I> (fn_pairs).second);

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
            static_assert (std::is_same_v<typename selected_tuple_type<Tuple, I>::first_type, char_ptr> &&
                           std::is_member_function_pointer_v<typename selected_tuple_type<Tuple, I>::second_type>,
                           "tuple element is NOT a pair of function name (const char*) and "
                           "member function pointer!");

            check_pairs_types<Tuple, I + 1> ();
        }
    }

    template <typename R>
    constexpr static bool check_return_type (typeid_type const& expected_type) noexcept
    {
        return expected_type == typeid (R);
    }

    template <typename... Args>
    constexpr static bool check_args_types (typeid_vector const& expected_types) noexcept
    {
        return expected_types == typeid_vector { typeid_type (typeid (Args))... };
    }

    template <class_t C, member_function_t FN>
    inline void add_member_function_base (string_view const& fn_name, FN fn)
    {
        iterator_pair pair = _M_fn_map.try_emplace (constexpr_char_hash (fn_name.data ()),
                                                    mapped_type (class_cast<C> (*this), fn));

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

    template <typename R = void, non_void_t... Args>
    inline R call (string_view const& fn_name, Args... args) const
    {
        auto it = _M_fn_map.find (constexpr_char_hash (fn_name.data ()));

        if (!check_return_type<R> (rtti (it).return_type ()))
            throw std::runtime_error ("return type mismatch!");

        if (!check_args_types<Args...> (rtti (it).arg_types ()))
            throw std::runtime_error ("argument type mismatch!");

        return (fn_cast<R(Args...)> (rtti (it).function ()))(std::forward<Args> (args)...);
    }

    friend class function_rtti;

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

template <pair_t... Ps>
struct is_reverse_interface<unbound_interface<Ps...>> : public std::true_type
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

    constexpr string_view provides () const noexcept
    { return _M_iface.provides (); }

    constexpr auto operator [] (string_view const& fn_name) const
    {
        return _M_iface[fn_name];
    }

private:
    iface_type _M_iface;
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_UNBOUND_INTERFACE_H_
