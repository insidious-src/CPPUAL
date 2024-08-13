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

#ifndef CPPUAL_INTERFACE_H_
#define CPPUAL_INTERFACE_H_
#ifdef __cplusplus

#include <cppual/meta.h>
#include <cppual/types.h>
#include <cppual/functional.h>
#include <cppual/containers.h>
#include <cppual/noncopyable.h>
#include <cppual/memory/stacked.h>

#include <tuple>
#include <iostream>
#include <typeindex>
#include <string_view>

namespace cppual {

// ====================================================

class unbound_interface : public non_copyable_virtual
{
public:
    class function_proxy;
    class function_info ;

    template <typename Sign>
    using fn_type = function<Sign>;

    typedef std::size_t      size_type  ;
    typedef std::string_view string_type;
    typedef cchar*           key_type   ;
    typedef function_info    mapped_type;

    class key_hash
    {
    public:
        constexpr size_type operator () (key_type key) const noexcept
        {
            return constexpr_hash (key);
        }
    };

    typedef unbound_interface                              base_type    ;
    typedef fn_type<void()>                                any_fn_type  ;
    typedef std::pair<key_type, mapped_type>               pair_type    ;
    typedef unordered_map<key_type, mapped_type, key_hash> map_type     ;
    typedef map_type::iterator                             iterator     ;
    typedef std::type_index                                typeid_type  ;
    typedef vector<typeid_type>                            typeid_vector;

    template <class C>
    using any_member_fn_type = void (C::*)();

    template <class C>
    using any_const_member_fn_type = void (C::*)() const;

    template <class C, typename R, typename... Args>
    using member_fn_type = R (C::*)(Args...);

    template <class C, typename R, typename... Args>
    using const_member_fn_type = R (C::*)(Args...) const;

    template <class C>
    using any_member_fn_pair = std::pair<key_type const, any_member_fn_type<C>>;

    template <class C>
    using any_const_member_fn_pair = std::pair<key_type const, any_const_member_fn_type<C>>;

    template <class C, typename R, typename... Args>
    using member_fn_pair = std::pair<key_type const, member_fn_type<C, R, Args...>>;

    template <class C, typename R, typename... Args>
    using const_member_fn_pair = std::pair<key_type const, const_member_fn_type<C, R, Args...>>;

    template <typename R>
    struct return_type
    {
        typedef R type;
        inline constexpr static const type constexpr_value = type ();
        inline static const type value = type ();
    };

    template <typename R>
    using return_t = return_type<R>::type;

    template <typename R>
    inline static auto const return_v = return_type<R>::value;

    template <typename R>
    inline constexpr static auto const return_cv = return_type<R>::constexpr_value;

    class function_info
    {
    public:
        constexpr function_info (function_info const&) = default;
        inline    function_info& operator = (function_info const&) = default;

        constexpr any_fn_type const&        functor () const noexcept { return _M_fn          ; }
        constexpr any_fn_type&              functor ()       noexcept { return _M_fn          ; }
        constexpr typeid_vector const& args_typeids () const noexcept { return _M_args_typeids; }
        constexpr typeid_vector&       args_typeids ()       noexcept { return _M_args_typeids; }
        inline    typeid_type         return_typeid () const noexcept { return _M_ret_type    ; }

    private:
        function_info () = delete;

        template <class C, typename R, typename... Args>
        constexpr function_info (C* obj, R (C::* fn)(Args...))
        : _M_fn           (obj, direct_cast<any_member_fn_type<C>> (fn)),
          _M_args_typeids ({ typeid_type (typeid (Args))... },
                           typeid_vector::allocator_type (object_ptr_cast<base_type> (obj)->_M_rc)),
          _M_ret_type     (typeid (R))
        {
            static_assert (std::is_base_of_v<base_type, C>,
                           "object is NOT derived from unbound_interface!");
        }

        template <class C, typename R, typename... Args>
        constexpr function_info (C* obj, R (C::* fn)(Args...) const)
        : _M_fn           (obj, direct_cast<any_member_fn_type<C>> (fn)),
          _M_args_typeids ({ typeid_type (typeid (Args))... },
                           typeid_vector::allocator_type (object_ptr_cast<base_type> (obj)->_M_rc)),
          _M_ret_type     (typeid (R))
        {
            static_assert (std::is_base_of_v<base_type, C>,
                           "object is NOT derived from unbound_interface!");
        }

        friend class unbound_interface;

    private:
        any_fn_type   _M_fn          ;
        typeid_vector _M_args_typeids;
        typeid_type   _M_ret_type    ;
    };

    class function_proxy
    {
    public:
        constexpr function_proxy (function_proxy const&) = default;
        inline    function_proxy& operator = (function_proxy const&) = default;

        template <typename R = void, typename... Args>
        inline R operator () (Args&&... args, R const& = return_cv<R>) const
        {
            return std::move (_M_iface->call<R> (_M_fn_name, std::forward<Args> (args)...));
        }

        friend class unbound_interface;

    private:
        constexpr function_proxy (base_type const* interface, string_type const& fn_name)
        : _M_iface   (interface),
          _M_fn_name (fn_name.data ())
        { }

    private:
        base_type const* _M_iface  ;
        key_type         _M_fn_name;
    };

    template <typename R = void, typename... Args>
    inline R call (string_type const& fn_name, Args&&... args) const
    {
        auto it = _M_fn_map.find (fn_name.data ());

        if (!check_return_type<R> (fn_info (it).return_typeid ()))
        {
            throw std::runtime_error ("return type mismatch!");
        }

        if (!check_args_types<Args...> (fn_info (it).args_typeids ()))
        {
            throw std::runtime_error ("parameter type mismatch!");
        }

        return (static_cast<fn_type<R(Args...)> const&>
               (fn_info (it).functor ()))(std::forward<Args> (args)...);
    }

    constexpr function_proxy operator [] (string_type const& fn_name) const
    {
        return function_proxy (this, fn_name);
    }

    inline mapped_type const& fn_info (iterator const& it) const
    {
        if    (it == _M_fn_map.end ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    inline mapped_type& fn_info (iterator const& it)
    {
        if    (it == _M_fn_map.end ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    inline mapped_type const& fn_info (string_type const& fn_name) const
    {
        auto   it  = _M_fn_map.find (fn_name.data ());
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    inline mapped_type& fn_info (string_type const& fn_name)
    {
        auto   it  = _M_fn_map.find (fn_name.data ());
        if    (it == _M_fn_map.end  ()) throw std::runtime_error ("function NOT found!");
        return it->second;
    }

    constexpr size_type fn_count () const noexcept
    {
        return _M_fn_map.size ();
    }

    inline virtual ~unbound_interface () { }

protected:
    /// a pair of Ts... consists of const char* and
    /// member function pointer ( ex. void (object::*)() )
    template <typename... Ts>
    inline unbound_interface (Ts... pairs)
    : _M_rc (sizeof... (Ts) *  sizeof (map_type::value_type) +
            (sizeof... (Ts) * (sizeof (typeid_type)  * 10U)) +
            (sizeof (map_type::value_type) - sizeof (uptr))),
      _M_fn_map (map_type::allocator_type (_M_rc))
    {
        check_pairs_types<std::tuple<Ts...>> ();

        std::cout << "unbound_interface size: " << sizeof... (Ts) * sizeof (map_type::value_type)
                  << " bytes\nstacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;

        _M_fn_map.reserve (sizeof... (Ts));

        add_all_member_functions (std::make_tuple (pairs...));
    }

    template <class C, typename FN_PTR>
    inline void add_member_function_base (string_type const& fn_name, FN_PTR fn)
    {
        typedef std::pair<iterator, bool> iterator_pair;

        static_assert (std::is_base_of_v<base_type, C>,
                       "object is NOT derived from unbound_interface!");

        iterator_pair pair = _M_fn_map.emplace (fn_name.data (), mapped_type (pointer_cast<C*> (this), fn));

        if (!pair.second) std::cerr << "insertion of member function " << fn_name
                      << " failed!" << std::endl;
    }

    template <class C, typename R, typename... Args>
    inline void add_member_function (string_type const& fn_name, R (C::* fn)(Args...))
    {
        add_member_function_base<C> (fn_name, fn);
    }

    template <class C, typename R, typename... Args>
    inline void add_member_function (string_type const& fn_name, R (C::* fn)(Args...) const)
    {
        add_member_function_base<C> (fn_name, fn);
    }

private:
    template <typename Tuple, size_type I = 0>
    using selected_type = std::tuple_element_t<I, Tuple>;

    unbound_interface () = delete;

    template <typename Tuple, size_type I = 0>
    constexpr static void check_pairs_types () noexcept
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            static_assert (std::is_same_v<typename selected_type<Tuple, I>::first_type, key_type> &&
                           std::is_member_function_pointer_v<typename selected_type<Tuple, I>::second_type>,
                           "tuple element is NOT a pair of function name (const char*) and "
                           "member function pointer!");

            check_pairs_types<Tuple, I + 1> ();
        }
    }

    template <typename R>
    inline bool check_return_type (typeid_type const& expected_type) const
    {
        return typeid (R) == expected_type;
    }

    template <typename... Args>
    inline bool check_args_types (typeid_vector const& expected_types) const
    {
        typeid_vector actual_types { typeid_type (typeid (Args))... };
        return expected_types  ==  actual_types;
    }

    template <typename Tuple, size_type I = 0>
    inline void add_all_member_functions (Tuple const& fn_pairs)
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            add_member_function (std::get<I> (fn_pairs).first, std::get<I> (fn_pairs).second);

            std::cout << "stacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;

            add_all_member_functions<Tuple, I + 1> (fn_pairs);
        }
    }

    friend class function_info;

private:
    memory::stacked_resource _M_rc    ;
    map_type mutable         _M_fn_map;
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_INTERFACE_H_
