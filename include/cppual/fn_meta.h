/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
 *
 * This program is free software: you can redistribute it &&/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, ||
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY || FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_FUNCTIONAL_META_H_
#define CPPUAL_FUNCTIONAL_META_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/cast.h>
#include <cppual/types.h>
#include <cppual/functional.h>
#include <cppual/containers.h>
#include <cppual/string_helper.h>
#include <cppual/concept/concepts.h>

#include <span>
#include <array>
#include <tuple>
#include <utility>
#include <type_traits>

namespace cppual {

// ====================================================

//! forward declarations
class vtable_base;

// ====================================================

template <typename P, typename T, typename U>
concept switch_map_pair_t = switch_value_t<T> && void_functional_t<U> && std::same_as<P, std::pair<T, U>>;

// =========================================================

template <non_void_t P, std::size_t N>
struct consteval_bimap;

// =========================================================

/**
 ** @arg_t is a single function argument that is memory compact.
 ** if the argument type is smaller than or equal to 4 bytes then it's passed as const value,
 ** however if it's bigger than 4 bytes or is a class then it's passed as const reference
 **
 ** T and U are the same type except U has const, volatile and reference removed.
 **/
template <typename T, typename U = std::remove_cvref_t<T>>
using arg_t = std::conditional_t<sizeof (U) <= arch_32_bits_v &&
                                !std::is_class_v<U>, U const, U const&>;

// =========================================================

/// overload member function with the same name but specific arguments
template <typename... Args, class_t C, typename R>
consteval auto overload (R (C::* fn)(Args...)) noexcept -> decltype (fn)
{ return fn; }

/// overload const member function with the same name but specific arguments
template <typename... Args, class_t C, typename R>
consteval auto overload (R (C::* fn)(Args...) const) noexcept -> decltype (fn)
{ return fn; }

/// overload static function with the same name but specific arguments
template <typename... Args, typename R>
consteval auto overload (R (* fn)(Args...)) noexcept -> decltype (fn)
{ return fn; }

// ====================================================

template <typename T, std::size_t N>
struct tuple_repeat_helper
{
    template <std::size_t... I>
    static auto make_tuple (std::index_sequence<I...>)
    {
        return std::tuple<std::conditional_t<(I>=0), T, T>...>{};
    }

    using type = decltype (make_tuple (std::make_index_sequence<N>{}));
};

template <typename T, std::size_t N>
using tuple_repeat_t = typename tuple_repeat_helper<T, N>::type;

// ====================================================

template <switch_value_t K, void_functional_t V, std::size_t N>
struct consteval_bimap <std::pair<K, V>, N>
{
    typedef std::pair<K, V>               pair_type  ;
    typedef consteval_bimap<pair_type, N> self_type  ;
    typedef std::size_t                   size_type  ;
    typedef size_type const               const_size ;
    typedef std::remove_cvref_t<K>        key_type   ;
    typedef key_type const                const_key  ;
    typedef std::remove_cvref_t<V>        value_type ;
    typedef value_type const              const_value;

    consteval static size_type size () noexcept { return N; }

    static_assert (size () > 0, "switch_map is empty!");

    inline constexpr static const_size npos = const_size (-1);

    constexpr void operator () (const_key k) const noexcept
    { get_value (k)(); }

private:
    constexpr consteval_bimap (std::array<pair_type, size ()> const& pairs) noexcept
    : _M_pairs (pairs)
    { }

    constexpr consteval_bimap (std::initializer_list<pair_type> pairs) noexcept
    : _M_pairs (pairs)
    { }

    //! runtime to_index function
    constexpr size_type key_to_index (const_key k) const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].first == k) return i;
        return npos;
    }

    constexpr size_type value_to_index (const_value v) const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].second == v) return i;
        return npos;
    }

    //! compile-time to_index function
    template <const_key K_>
    consteval size_type key_to_index () const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].first == K_) return i;
        return npos;
    }

    template <const_value V_>
    consteval size_type value_to_index () const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].second == V_) return i;
        return npos;
    }

    constexpr value_type get_value (const_key k) const noexcept
    {
        auto i = key_to_index (k);
        if (i != npos) return _M_pairs[i].second;
        return value_type ();
    }

    template <const_key K_>
    consteval value_type get_value () const noexcept
    {
        constexpr const_size I = key_to_index<K_> ();
        static_assert (I != npos, "key not found in switch_map");
        return _M_pairs[I].second;
    }

    constexpr key_type get_key (const_value v) const noexcept
    {
        auto i = value_to_index (v);
        if (i != npos) return _M_pairs[i].first;
        return key_type ();
    }

    template <const_value V_>
    consteval key_type get_key () const noexcept
    {
        constexpr const_size I = value_to_index<V_> ();
        static_assert (I != npos, "value not found in switch_map");
        return _M_pairs[I].first;
    }

    template <switch_value_t T, void_functional_t U, std::size_t SZ>
    friend constexpr auto make_switch_map (std::array<std::pair<T, U>, SZ> const&) noexcept;

    template <switch_value_t T, void_functional_t U, std::size_t SZ>
    friend constexpr auto make_switch_map (std::initializer_list<pair_type>) noexcept;

    template <non_void_t T, non_void_t U, std::size_t SZ>
    friend constexpr auto make_consteval_bimap (std::array<std::pair<T, U>, SZ> const&) noexcept;

    template <non_void_t T, non_void_t U, std::size_t SZ>
    friend constexpr auto make_consteval_bimap (std::initializer_list<pair_type>) noexcept;

private:
    std::array<pair_type, size ()> const _M_pairs;
};

// ====================================================

template <switch_value_t K, void_functional_t V = function<void()>, std::size_t N>
constexpr auto make_switch_map (std::array<std::pair<K, V>, N> const& case_pairs) noexcept
{
    return consteval_bimap<std::pair<K, V>, N> (case_pairs);
}

// ====================================================

template <switch_value_t K, void_functional_t V = function<void()>, std::size_t N>
constexpr auto make_switch_map (std::initializer_list<std::pair<K, V>> case_pairs) noexcept
{
    return consteval_bimap<std::pair<K, V>, N> (case_pairs);
}

// ====================================================

template <non_void_t K, non_void_t V, std::size_t N>
constexpr auto make_consteval_bimap (std::array<std::pair<K, V>, N> const& pairs) noexcept
{
    return consteval_bimap<std::pair<K, V>, N> (pairs);
}

// ====================================================

template <non_void_t K, non_void_t V, std::size_t N>
constexpr auto make_consteval_bimap (std::initializer_list<std::pair<K, V>> pairs) noexcept
{
    return consteval_bimap<std::pair<K, V>, N> (pairs);
}

// ====================================================

template <non_void_t... Ts>
struct type_list
{

    typedef type_list<Ts...> self_type;
    typedef std::size_t      size_type;

    template <size_type N>
    using ic = std::integral_constant<size_type, N>;

    template <size_type... Ns>
    using iseq = std::index_sequence<Ns...>;

    template <size_type N>
    using make_iseq = std::make_index_sequence<N>;

    template <typename... Us>
    using self_type_t = type_list<Us...>;

    template <size_type N>
    using type = std::tuple_element_t<N, std::tuple<Ts...>>;

    consteval static size_type size () noexcept { return sizeof... (Ts); }

    template <size_type Idx>
    struct at
    {
        template <size_type N, typename U, typename... Us>
        struct access_helper : access_helper<N + 1, Us...>
        {
            static_assert (N < sizeof... (Us), "index out of bounds!");
        };

        template <typename U, typename... Us>
        struct access_helper<Idx, U, Us...>
        {
            static_assert (Idx < sizeof... (Us), "index out of bounds!");

            typedef U result;
        };

        typedef access_helper<0, Ts...>::result result;
    };

    template <typename... Us>
    struct push
    {
        typedef self_type_t<Ts..., Us...> result;
    };

    template <size_type Idx, typename U>
    struct set
    {
        template <size_type N>
        static auto at_helper (ic<N>)   -> typename at<N>::result;
        static auto at_helper (ic<Idx>) -> U;

        template <size_type... Ns>
        static auto set_helper (iseq<Ns...>) -> self_type_t<decltype (at_helper (ic<Ns> {}))...>;

        typedef decltype (set_helper (make_iseq<size ()> {})) result;
    };

    struct init
    {
        template <size_type... Ns>
        static auto helper (iseq<Ns...>) -> self_type_t<typename at<Ns>::result...>;

        typedef decltype (helper (make_iseq<size () - 1> {})) result;
    };
};

// ====================================================

template <typename... Ts, typename... Us>
consteval auto operator + (type_list<Ts...>, type_list<Us...>) noexcept
{
    return type_list<Ts..., Us...> { };
}

template <typename... Ts, typename... Us>
consteval bool operator == (type_list<Ts...>, type_list<Us...>) noexcept
{
    return std::is_same_v<type_list<Ts...>, type_list<Us...>>;
}

template <typename... Ts, typename... Us>
consteval bool operator != (type_list<Ts...>, type_list<Us...>) noexcept
{
    return !std::is_same_v<type_list<Ts...>, type_list<Us...>>;
}

// ====================================================

struct stateful_type_list
{
public:
    typedef std::size_t size_type;

private:
    template <size_type N>
    struct getter
    {
        friend consteval auto flag (getter);
    };

    template <typename T, size_type N>
    struct setter
    {
        friend consteval auto flag (getter<N>)
        {
            return type_list<T> { };
        }

        inline static constexpr size_type const value = N;
    };

public:
    template <typename T, size_type N = 0>
    consteval static size_type try_push ()
    {
        if constexpr (requires { flag (getter<N> {}); })
        {
            return try_push<T, N + 1> ();
        }
        else
        {
            return setter<T, N>::value;
        }
    }

    template <typename Unique, size_type N = 0>
    consteval static auto get ()
    {
        if constexpr (requires { flag (getter<N> {}); })
        {
            return flag (getter<N> {}) + get<Unique, N + 1> ();
        }
        else
        {
            return type_list { };
        }
    }
};

// ====================================================

template <typename Tag>
struct meta_counter
{
    typedef meta_counter<Tag> self_type;
    typedef std::size_t       size_type;

    template <typename U>
    using self_type_t = meta_counter<U>;

    template <size_type N>
    struct ident
    {
        friend constexpr size_type adl_lookup (ident<N>);
        inline constexpr static size_type const value = N;
    };

    // ==================================================== - -

    template <typename Ident>
    struct writer
    {
        friend constexpr size_type adl_lookup (Ident)
        { return Ident::value; }

        inline constexpr static size_type const value = Ident::value;
    };

    // ====================================================

    template <size_type N, int = adl_lookup (ident<N> {})>
    consteval static size_type value_reader (int, ident<N>)
    {
        return N;
    }

    template <size_type N>
    consteval static size_type value_reader (float, ident<N>, size_type R = value_reader (0, ident<N - 1> ()))
    {
        return R;
    }

    consteval static size_type value_reader (float, ident<0>)
    {
        return 0;
    }

    // ====================================================

    template <size_type Max = 64>
    consteval static size_type value (size_type R = value_reader (0, ident<Max> {}))
    {
        return R;
    }

    template <size_type N = 1, class H = self_type>
    consteval static size_type next (size_type R = writer<ident<N + H::value ()>>::value)
    {
        return R;
    }
};

// ====================================================

template <typename Tag>
struct meta_list
{
    typedef meta_list<Tag>          self_type;
    typedef meta_counter<self_type> counter  ;
    typedef counter::size_type      size_type;

    template <typename U>
    using self_type_t = meta_list<U>;

    template <size_type N, typename = void>
    struct ident
    {
        friend auto adl_lookup (ident<N>);

        inline constexpr static size_type const value = N;
    };

    template <typename Dummy>
    struct ident <0, Dummy>
    {
        friend auto adl_lookup (ident<0>)
        {
            return type_list { };
        }
    };

    // ====================================================

    template <typename Ident, typename Value>
    struct writer
    {
        friend auto adl_lookup (ident<Ident::value>)
        {
            return Value {};
        }

        inline constexpr static size_type const value = Ident::value;
    };

    // ====================================================

    template <typename State,
              typename     H = self_type,
              typename Ident = typename H::template ident<H::counter::next ()>
              >
    consteval static size_type push_state (size_type R = writer<Ident, State>::value)
    {
        return R;
    }

    // ====================================================

    template <typename H = self_type, size_type N = H::counter::value ()>
    using value_ident = typename H::template ident<N>;

    template <typename     H = self_type,
              typename Ident = typename H::template value_ident<>
              >
    using value = decltype (adl_lookup (Ident {}));

    // ====================================================

    template <typename... Ts, typename H = self_type>
    consteval static void push (size_type = push_state<
                                   typename H::template value<>::template push<Ts...>::result
                                   > ())
    { }

    template <typename H = self_type>
    consteval static void pop (
        size_type = push_state<typename H::template value<>::init::result> ())
    { }

    template <size_type Idx, class T, class H = self_type>
    consteval static void set (
        size_type = push_state<typename H::template value<>::template set<Idx, T>::result> ())
    { }
};

// ====================================================

// class vtable_entry_base {
// public:
//     virtual ~vtable_entry_base() = default;
//     virtual void invoke(void* obj, void* args, void* ret) const = 0;
//     virtual std::size_t get_type_id() const = 0;
// };

// template <typename R, typename... Args>
// class vtable_entry final : public vtable_entry_base {
//     using member_fn_type = R(*)(void*, void*, std::tuple<Args...>&);

// public:
//     explicit vtable_entry(member_fn_type fn_ptr) : fn(fn_ptr) {}

//     void invoke(void* obj, void* args, void* ret) const override {
//         auto& tuple_args = *static_cast<std::tuple<Args...>*>(args);
//         if constexpr (std::is_void_v<R>) {
//             fn(obj, nullptr, tuple_args);
//         } else {
//             *static_cast<R*>(ret) = fn(obj, nullptr, tuple_args);
//         }
//     }

//     std::size_t get_type_id() const override {
//         return typeid(R(Args...)).hash_code();
//     }

// private:
//     member_fn_type fn;
// };

// template <typename T, typename R, typename... Args>
// struct vtable_function {
//     template <R(T::*Fn)(Args...)>
//     static R invoke(void* obj, void*, std::tuple<Args...>& args) {
//         T* typed_obj = static_cast<T*>(obj);
//         if constexpr (std::is_void_v<R>) {
//             std::apply([typed_obj](Args... params) {
//                 (typed_obj->*Fn)(std::forward<Args>(params)...);
//             }, args);
//         } else {
//             return std::apply([typed_obj](Args... params) {
//                 return (typed_obj->*Fn)(std::forward<Args>(params)...);
//             }, args);
//         }
//     }
// };

// class vtable_base {
// protected:
//     std::vector<std::unique_ptr<vtable_entry_base>> _vtable;

//     template <typename Derived, typename R, typename... Args>
//     void register_function(R(Derived::* fn)(Args...)) {
//         using entry_type = vtable_entry<R, Args...>;
//         using fn_type = R(*)(void*, void*, std::tuple<Args...>&);

//         fn_type fn_ptr = &vtable_function<Derived, R, Args...>::template invoke<fn>;
//         _vtable.push_back(std::make_unique<entry_type>(fn_ptr));
//     }

//     template <typename R, typename... Args>
//     R dispatch(std::size_t index, Args&&... args) {
//         if (index >= _vtable.size()) {
//             throw std::out_of_range("Invalid vtable index");
//         }

//         auto args_tuple = std::make_tuple(std::forward<Args>(args)...);

//         if constexpr (std::is_void_v<R>) {
//             _vtable[index]->invoke(this, &args_tuple, nullptr);
//         } else {
//             R result;
//             _vtable[index]->invoke(this, &args_tuple, &result);
//             return result;
//         }
//     }
// };

// class vtable_base_impl : public vtable_base {
// public:
//     vtable_base_impl() {
//         // Register member functions directly
//         register_function(&vtable_base_impl::print_impl<int>);
//         register_function(&vtable_base_impl::print_to_string_impl<std::string>);
//     }

//     // Clean templated virtual function interfaces
//     template <typename... Args>
//     void print(Args&&... args) {
//         dispatch<void>(0, std::forward<Args>(args)...);
//     }

//     template <typename... Args>
//     std::string print_to_string(Args&&... args) {
//         return dispatch<std::string>(1, std::forward<Args>(args)...);
//     }

// private:
//     // Actual implementations as regular member functions
//     template <typename T>
//     void print_impl(T arg) {
//         std::cout << arg << std::endl;
//     }

//     template <typename T>
//     std::string print_to_string_impl(T arg) {
//         return std::to_string(arg) + "\n";
//     }
// };

// ====================================================

class vtable_base
{
protected:
    template <typename> struct virtual_function;

    typedef std::size_t                                             size_type       ;
    typedef std::aligned_storage_t<sizeof (void*), alignof (void*)> storage_type    ;
    typedef std::span<storage_type>                                 fn_ptrs_vec_type;
    typedef std::span<void(*)(vtable_base&, void*, void*, void*)>   vtable_vec_type ;
    typedef unordered_map<size_type, function<void(void*)>>         vtable_map_type ;

    // ====================================================

    template <class_t D>
    constexpr vtable_base (D* /* derived_class */)
        : _M_vtable  (create_vtable<D> (stateful_type_list::get<D> ())),
        _M_fn_ptrs (create_fn_ptrs   (stateful_type_list::get<D> ()))
    { }

    // ====================================================

    inline virtual ~vtable_base () = default;

    // ====================================================

    template <class_t D, typename R, typename... Args>
    constexpr void register_function (R(D::* fn)(Args...))
    {
        using fn_type = R(Args...);
        typedef decltype (fn) fn_pointer;

        size_type idx = stateful_type_list::try_push<virtual_function<fn_type>> ();
        ::new (&_M_fn_ptrs[idx]) fn_pointer (fn);

        _M_invoke[typeid (fn_type).hash_code ()] = [this, idx](void* args)
        {
            auto* tuple = static_cast<std::tuple<Args...>*> (args);

            std::apply ([this, idx](Args&&... args)
                       {
                           invoke_dispatch<R> (idx, std::forward<Args> (args)...);
                       },
                       std::move (*tuple));
        };
    }

    template <class_t D, typename R, typename... Args>
    constexpr void register_function (R(D::* fn)(Args...) const)
    {
        using fn_type = R(Args...);
        typedef decltype (fn) fn_pointer;

        size_type idx = stateful_type_list::try_push<virtual_function<fn_type>> ();
        ::new (&_M_fn_ptrs[idx]) fn_pointer (fn);

        _M_invoke[typeid (fn_type).hash_code ()] = [this, idx](void* args)
        {
            auto* tuple = static_cast<std::tuple<Args...>*> (args);

            std::apply ([this, idx](Args&&... args)
                       {
                           invoke_dispatch<R> (idx, std::forward<Args> (args)...);
                       },
                       std::move (*tuple));
        };
    }

    // ====================================================

    /// function to invoke stored member function pointer
    template <typename R, typename... Args>
    constexpr R invoke_dispatch (size_type idx, Args&&... args) const
    {
        auto args_tuple = std::forward_as_tuple (std::forward<Args> (args)...);

        if constexpr (std::is_void_v<R>)
        {
            _M_vtable[idx](*this, &_M_fn_ptrs[idx], &args_tuple, nullptr);
        }
        else
        {
            R result;

            _M_vtable[idx](*this, &_M_fn_ptrs[idx], &args_tuple, &result);
            return result;
        }
    }

    template <typename... Args>
    void invoke_fn (Args&&... args)
    {
        auto fn_hash = typeid (void(Args...)).hash_code ();

        if (auto it = _M_invoke.find (fn_hash); it != _M_invoke.end ())
        {
            auto args_tuple = std::make_tuple (std::forward<Args>(args)...);
            it->second (&args_tuple);
        }
    }

    //! specialization for member function signatures
    template <typename R, typename... Args>
    struct virtual_function <R(Args...)>
    {
        template <class_t D>
        inline static void invoke (vtable_base& base, void* fn_ptr, void* args_ptr, void* ret)
        {
            if (!args_ptr && sizeof... (Args) > 0)
            {
                throw std::invalid_argument ("arguments > 0 but args_ptr is NULLPTR!");
            }

            auto  fn         =  static_cast<R(D::*)(Args...)> (fn_ptr);
            auto& tuple_args = *static_cast<std::tuple<Args...>*> (args_ptr);
            auto& derived    =  class_cast<D> (base);

            if constexpr (std::is_void_v<R>)
            {
                std::apply ([&derived, fn](Args&&... args)
                {
                    (derived.*fn)(std::forward<Args> (args)...);
                },
                std::move (tuple_args));
            }
            else
            {
                if (!ret) throw std::logic_error ("return pointer is NULLPTR!");

                *static_cast<R*> (ret) = std::apply ([&derived, fn](Args&&... args) -> R
                {
                    return (derived.*fn)(std::forward<Args> (args)...);
                },
                std::move (tuple_args));
            }
        }
    };

private:
    template <class_t D, typename... Funcs>
    inline static auto create_vtable (type_list<Funcs...>)
    {
        constinit static std::array<void(*)(vtable_base&, void*, void*, void*), sizeof...(Funcs)>
            vtable { &Funcs::template invoke<D>... };
        return std::span { vtable };
    }
    template <typename... Funcs>
    inline static auto create_fn_ptrs (type_list<Funcs...>)
    {
        constinit static std::array<storage_type, sizeof... (Funcs)> storage;
        return std::span { storage };
    }

private:
    vtable_vec_type  const _M_vtable ;
    fn_ptrs_vec_type const _M_fn_ptrs;
    vtable_map_type        _M_invoke ;
};

// ====================================================

} // cppual

// ====================================================

namespace std {

// ====================================================

using cppual::LAMBDA_DEFAULT_MAX_SIZE;

template <std::size_t N = LAMBDA_DEFAULT_MAX_SIZE>
using delegate = cppual::function<void(), N>;

// ====================================================

template <size_t N = LAMBDA_DEFAULT_MAX_SIZE, cppual::switch_value_t T, typename... Args>
constexpr auto make_pair (T const _case, Args&&... fn_args) noexcept
{
    using fn_type = delegate<N>;

    return pair<T, fn_type> (_case, cppual::make_fn (forward<Args> (fn_args)...));
}

// ====================================================

} // std

#endif // __cplusplus
#endif // CPPUAL_FUNCTIONAL_META_H_