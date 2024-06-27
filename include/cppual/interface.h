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
#include <cppual/functional.h>
#include <cppual/containers.h>
#include <cppual/noncopyable.h>
#include <cppual/memory/stacked.h>

#include <tuple>
#include <iostream>

namespace cppual {

// ====================================================

class unbound_interface : public non_copyable_virtual
{
public:
    struct key_hash;

    template <typename S>
    using fn_type = function<S>;

    typedef unbound_interface                            base_type  ;
    typedef std::size_t                                  size_type  ;
    typedef fn_type<void()>                              any_fn_type;
    typedef unordered_map<cchar*, any_fn_type, key_hash> map_type   ;

    template <class C>
    using any_member_fn_type = void (C::*)();

    template <class C>
    using any_const_member_fn_type = void (C::*)() const;

    template <class C, typename R, typename... Args>
    using member_fn_type = R (C::*)(Args...);

    template <class C, typename R, typename... Args>
    using const_member_fn_type = R (C::*)(Args...) const;

    template <class C>
    using any_member_fn_pair = std::pair<cchar* const, any_member_fn_type<C>>;

    template <class C>
    using any_const_member_fn_pair = std::pair<cchar* const, any_const_member_fn_type<C>>;

    template <class C, typename R, typename... Args>
    using member_fn_pair = std::pair<cchar* const, member_fn_type<C, R, Args...>>;

    template <class C, typename R, typename... Args>
    using const_member_fn_pair = std::pair<cchar* const, const_member_fn_type<C, R, Args...>>;

    struct key_hash
    {
        constexpr size_type operator () (cchar* key) const noexcept
        {
            return constexpr_hash (key);
        }
    };

    template <typename R = void, typename... Args>
    inline R call (cchar* fn_name, Args... args) const
    {
        return (static_cast<fn_type<R(Args...)>&> (_M_fn[fn_name]))(std::forward<Args> (args)...);
    }

    template <typename R = void, typename... Args>
    inline R call (cchar* fn_name, Args... args)
    {
        return (static_cast<fn_type<R(Args...)>&> (_M_fn[fn_name]))(std::forward<Args> (args)...);
    }

    constexpr size_type interface_fn_count () const noexcept
    {
        return _M_fn.size ();
    }

    unbound_interface () = delete;
    inline virtual ~unbound_interface () { }

protected:
    template <typename... Ts>
    inline unbound_interface (Ts... pairs)
        : _M_rc (sizeof... (Ts) * sizeof (map_type::value_type) +
                (sizeof (map_type::value_type) - sizeof (uptr))),
      _M_fn (map_type::allocator_type (_M_rc))
    {
        check_types<std::tuple<Ts...>> ();

        std::cout << "unbound_interface size: " << sizeof... (Ts) * sizeof (map_type::value_type)
                  << " bytes\nstacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;

        auto list = std::make_tuple (pairs...);

        _M_fn.reserve (sizeof... (Ts));

        add_all_member_functions (list);
    }

    template <class C, typename R, typename... Args>
    inline void add_member_function (cchar* fn_name, R (C::* fn)(Args...))
    {
        static_assert (std::is_base_of_v<unbound_interface, C>,
                       "object is NOT derived from unbound_interface!");

        auto pair = _M_fn.try_emplace (fn_name,
                                       any_fn_type (this, direct_cast<any_member_fn_type<C>> (fn)));

        if (!pair.second) std::cerr << "insertion of member function " << fn_name
                                    << " failed!" << std::endl;
    }

    template <class C, typename R, typename... Args>
    inline void add_member_function (cchar* fn_name, R (C::* fn)(Args...) const)
    {
        static_assert (std::is_base_of_v<unbound_interface, C>,
                       "object is NOT derived from unbound_interface!");

        auto pair = _M_fn.try_emplace (fn_name,
                                       any_fn_type (this, direct_cast<any_const_member_fn_type<C>> (fn)));

        if (!pair.second) std::cerr << "insertion of member function \'" << fn_name
                                    << "\' failed!" << std::endl;
    }

private:
    template <typename Tuple, size_type I>
    using selected_type = std::tuple_element_t<I, Tuple>;

    template <typename Tuple, std::size_t I = 0>
    constexpr static void check_types () noexcept
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            static_assert (std::is_same_v<typename selected_type<Tuple, I>::first_type, map_type::key_type> &&
                           std::is_member_function_pointer_v<typename selected_type<Tuple, I>::second_type>,
                           "tuple element is NOT a pair of function name (const char*) and "
                           "member function pointer!");

            check_types<Tuple, I + 1> ();
        }
    }

    template <typename Tuple, std::size_t I = 0>
    inline void add_all_member_functions (Tuple const& list)
    {
        if constexpr (I < std::tuple_size_v<Tuple>)
        {
            add_member_function (std::get<I> (list).first, std::get<I> (list).second);

            std::cout << "stacked_resource max size: " << _M_rc.max_size () << " bytes" << std::endl;

            add_all_member_functions<Tuple, I + 1> (list);
        }
    }

private:
    memory::stacked_resource _M_rc;
    map_type mutable         _M_fn;
};

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_INTERFACE_H_
