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
#include <cppual/functional>
#include <cppual/containers>
#include <cppual/type_meta.h>
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

template <typename P, typename K, typename FN>
concept member_fn_pair_t = std::is_same_v<P, std::pair<K, FN>> &&
                           is_char_v<remove_cptr_t<K>>         &&
                           std::is_member_function_pointer_v<FN>;

// ====================================================

class unbound_interface : public non_copyable_virtual
{
public:
    class function_proxy;
    class function_rtti ;

    template <typename S>
    using fn_type = function<S>;

    template <typename, typename>
    struct fn_signatures_match;

    // ====================================================

    typedef unbound_interface                               self_type             ;
    typedef self_type                                       base_type             ;
    typedef cchar*                                          key_type              ;
    typedef function_rtti                                   mapped_type           ;
    typedef std::pair<key_type const, mapped_type>          value_type            ;
    typedef char_hash                                       hash_type             ;
    typedef unordered_map<key_type, mapped_type, hash_type> map_type              ;
    typedef std::allocator_traits<map_type::allocator_type> alloc_traits          ;
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
    typedef vector<typeid_type>                             typeid_vector         ;

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

    class function_rtti
    {
    public:
        typedef function_rtti self_type;

        constexpr function_rtti (self_type const&)         = default;
        inline    function_rtti (self_type &&)             = default;
        inline    self_type& operator = (self_type &&)     = default;
        inline    self_type& operator = (self_type const&) = default;

        constexpr any_fn_type   const&       functor () const noexcept { return _M_fn          ; }
        constexpr typeid_vector const&  args_typeids () const noexcept { return _M_args_typeids; }
        constexpr typeid_type   const& return_typeid () const noexcept { return _M_ret_type    ; }

    private:
        function_rtti () = delete;

        //! template parameter C is a derived class from unbound_interface
        template <class_t C, typename R, typename... Args,
                  typename = std::enable_if_t<std::is_base_of_v<base_type, C>>
                  >
        constexpr function_rtti (C& obj, member_fn_type<C, R, Args...> fn)
        : _M_fn           (direct_cast<fn_type<void()>> (fn_type<R(Args...)> (obj, fn))),
          _M_args_typeids ({ typeid_type (typeid (Args))... },
                             typeid_vector::allocator_type (/*class_cast<base_type> (obj)._M_rc*/)),
          _M_ret_type     (typeid (R))
        { }

        //! template parameter C is a derived class from unbound_interface
        template <class_t C, typename R, typename... Args,
                  typename = std::enable_if_t<std::is_base_of_v<base_type, C>>
                  >
        constexpr function_rtti (C& obj, const_member_fn_type<C, R, Args...> fn)
        : _M_fn           (direct_cast<fn_type<void()>> (fn_type<R(Args...)> (obj, fn))),
          _M_args_typeids ({ typeid_type (typeid (Args))... },
                             typeid_vector::allocator_type (/*class_cast<base_type> (obj)._M_rc*/)),
          _M_ret_type     (typeid (R))
        { }

        friend class unbound_interface;

    private:
        any_fn_type   _M_fn          ;
        typeid_vector _M_args_typeids;
        typeid_type   _M_ret_type    ;
    };

    // ====================================================

    class function_proxy
    {
    public:
        typedef function_proxy self_type;

        constexpr function_proxy (self_type const&) noexcept = default;
        constexpr self_type& operator = (self_type const&) noexcept = default;

        template <typename R = void, typename... Args>
        constexpr R operator () (R* = ret<void>, Args... args) const
        {
            return _M_iface->call<R, Args...> (_M_fn_name, std::forward<Args> (args)...);
        }

        friend class unbound_interface;

    private:
        constexpr function_proxy (base_type const& interface, string_view const& fn_name)
        : _M_iface   (&interface     ),
          _M_fn_name (fn_name.data ())
        { }

    private:
        base_type const* _M_iface  ;
        key_type         _M_fn_name;
    };

    // ====================================================

    unbound_interface () = delete;
    inline virtual ~unbound_interface () = default;

    template <typename R, typename... Args>
    inline R call (string_view const& fn_name, Args... args) const
    {
        auto it = _M_fn_map.find (fn_name.data ());

        if (!check_return_type<R> (rtti (it).return_typeid ()))
        {
            throw std::runtime_error ("return type mismatch!");
        }

        if (!check_args_types<Args...> (rtti (it).args_typeids ()))
        {
            throw std::runtime_error ("argument type mismatch!");
        }

        return (static_cast<fn_type<R(Args...)> const&>
               (rtti (it).functor ()))(std::forward<Args> (args)...);
    }

    constexpr function_proxy operator [] (string_view const& fn_name) const
    {
        return function_proxy (*this, fn_name);
    }

    inline mapped_type const& rtti (const_iterator const& it) const
    {
        if    (it == _M_fn_map.cend ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    inline mapped_type& rtti (iterator const& it)
    {
        if    (it == _M_fn_map.end ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    inline mapped_type const& rtti (string_view const& fn_name) const
    {
        auto   it  = _M_fn_map.find (fn_name.data ());
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    inline mapped_type& rtti (string_view const& fn_name)
    {
        auto   it  = _M_fn_map.find (fn_name.data ());
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    constexpr size_type fn_count () const noexcept
    {
        return _M_fn_map.size ();
    }

protected:
    /// a pair of Ts... consists of const char* and
    /// member function pointer (ex. void(object::*)())
    template <typename... Ts>
    requires (member_fn_pair_t<Ts, key_type, typename Ts::second_type> && ...)
    inline unbound_interface (Ts... pairs)
    : _M_rc (sizeof... (Ts) *  sizeof (map_type::value_type) +
            (sizeof... (Ts) * (sizeof (typeid_type) *  10U))),
      _M_fn_map (map_type::allocator_type (_M_rc))
    {
        check_pairs_types<std::tuple<Ts...>> ();

#       ifdef DEBUG_MODE
        std::cout << "unbound_interface size: " << sizeof... (Ts) * sizeof (map_type::value_type)
                  << " bytes\nstacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;
#       endif

        _M_fn_map.reserve (sizeof... (Ts));

        add_all_member_functions (std::make_tuple (pairs...));
    }

private:
    template <typename Tuple, size_type I = 0>
    inline void add_all_member_functions (Tuple const& fn_pairs)
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
    constexpr static void check_pairs_types () noexcept
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            static_assert (std::is_same_v<typename selected_tuple_type<Tuple, I>::first_type, key_type> &&
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

    friend class function_rtti;

private:
    template <class_t C, member_function_t FN>
    inline void add_member_function_base (string_view const& fn_name, FN fn)
    {
        iterator_pair pair = _M_fn_map.try_emplace (fn_name.data (),
                                                    mapped_type (class_cast<C> (*this), fn));

        if (!pair.second)
        {
            string error_text;

            error_text.reserve (39U + fn_name.size ());

            error_text.append ("insertion of member function '")
                      .append (fn_name.data ())
                      .append ("' failed!");

            throw std::runtime_error (error_text.c_str ());
        }
    }

    template <class_t C, typename R, typename... Args>
    inline void add_member_function (string_view const& fn_name, R(C::* fn)(Args...))
    {
        add_member_function_base<C> (fn_name, fn);
    }

    template <class_t C, typename R, typename... Args>
    inline void add_member_function (string_view const& fn_name, R(C::* fn)(Args...) const)
    {
        add_member_function_base<C> (fn_name, fn);
    }

private:
    memory::stacked_resource _M_rc    ;
    map_type mutable         _M_fn_map;
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_UNBOUND_INTERFACE_H_
