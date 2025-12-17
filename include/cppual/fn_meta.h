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

#include <cppual/decl>
#include <cppual/casts>
#include <cppual/types>
#include <cppual/concepts>

#include <tuple>
#include <utility>
#include <optional>
#include <algorithm>
#include <type_traits>

#include <cassert>

// ====================================================

namespace cppual {

// ====================================================

//! forward declarations
template <pair_like P, std::size_t N>
struct consteval_bimap;

// ====================================================

//template <typename, std::size_t>
//class function;

// ====================================================

template <typename P, typename T, typename U>
concept bimap_pair = non_void<T> && non_void<U> && std::same_as<P, std::pair<T, U>>;

template <typename P, typename T, typename U>
concept switch_map_pair = switch_value<T> && void_functional<U> && std::same_as<P, std::pair<T, U>>;

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

template <void_or_class C, typename S>
struct any_fn_type;

template <class_t C, typename R, non_void... Args>
struct any_fn_type <C, R(Args...)>
{
    using type = R(C::*)(Args...);
};

template <class_t C, typename R, non_void... Args>
struct any_fn_type <C, R(Args...) const>
{
    using type = R(C::*)(Args...) const;
};

template <typename R, non_void... Args>
struct any_fn_type <void, R(Args...)>
{
    using type = R(*)(Args...);
};

template <typename R, non_void... Args>
struct any_fn_type <void, R(Args...) const>
{
    static_assert (false, "CANNOT have const function pointer without class type!");
};

template <void_or_class C, typename S>
using any_fn_t = any_fn_type<C, S>::type;

// =========================================================

/**
 ** @arg_t is a single function argument that is memory compact.
 ** if the argument type is smaller than or equal to 4 bytes then it's passed as value,
 ** however if it's bigger than 4 bytes or is a class then it's passed as const reference
 **
 ** T and U are the same type except U has reference and const removed.
 **/
template <typename T, typename U = remove_cref_t<T>>
using arg_t = std::conditional_t<sizeof (U) <= arch_32_bits_v && !std::is_class_v<U>, U, U const&>;

// =========================================================

template <typename T = void>
struct var_type
{
    typedef remove_cref_t<T>  value_type   ;
    typedef value_type      * pointer      ;
    typedef value_type const* const_pointer;

    inline constexpr static pointer value = nullptr;
};

template <typename T = void>
using type_ptr_t = var_type<T>::pointer;

template <typename T = void>
inline constexpr type_ptr_t<T   > type_v    = var_type<T   >::value;
inline constexpr type_ptr_t<void> default_v = var_type<void>::value;

template <typename T = void>
inline constexpr type_ptr_t<T   > ret         = type_v<T>;
inline constexpr type_ptr_t<void> default_ret = default_v;

// ====================================================

template <typename T, std::size_t N>
struct tuple_repeat_helper
{
    template <std::size_t... I>
    static auto make_tuple (std::index_sequence<I...>)
    {
        return std::tuple<std::conditional_t<I >= 0, T, T>...> { };
    }

    using type = decltype (make_tuple (std::make_index_sequence<N> { }));
};

template <typename T, std::size_t N>
using tuple_repeat_t = typename tuple_repeat_helper<T, N>::type;

// ====================================================

template <switch_value K, non_void V, std::size_t N>
struct consteval_bimap <std::pair<K, V>, N>
{
public:
    typedef std::pair<K, V>                       pair_type             ;
    typedef pair_type const                       const_pair            ;
    typedef consteval_bimap<pair_type, N>         self_type             ;
    typedef decltype (N)                          size_type             ;
    typedef size_type const                       const_size            ;
    typedef remove_cref_t<K>                      key_type              ;
    typedef key_type const                        const_key             ;
    typedef remove_cref_t<V>                      value_type            ;
    typedef value_type const                      const_value           ;
    typedef std::optional<key_type>               optional_key          ;
    typedef std::optional<value_type>             optional_value        ;
    typedef pair_type                             array_type[N]         ;
    typedef key_type   &                          key_reference         ;
    typedef value_type &                          value_reference       ;
    typedef const_key  &                          key_const_reference   ;
    typedef const_value&                          value_const_reference ;
    typedef pair_type *                           iterator              ;
    typedef pair_type const*                      const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;
    typedef std::pair<iterator, bool>             iterator_pair         ;

    consteval static size_type size () noexcept { return N; }

    static_assert (size () > 0, "consteval_bimap is empty!");
    static_assert (!std::same_as<key_type, value_type>, "key & value must NOT be of the same type!");
    static_assert (copyable_movable<key_type  > &&
                   copyable_movable<value_type>, "key or/and value are NOT copyable & movable!");

    inline constexpr static const_size npos = size_type (-1);

    //! make class trivially copyable & movable
    constexpr consteval_bimap (self_type &&) noexcept = default;
    consteval consteval_bimap (self_type const&) noexcept = default;
    constexpr self_type& operator = (self_type &&) noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr consteval_bimap (std::initializer_list<pair_type> pairs) noexcept
    {
        assert (pairs.size () <= size () && "bimap capacity NOT sufficient!");

        std::copy (pairs.begin (), pairs.end (), _M_pairs);
        std::fill (begin () + pairs.size (), end (), pair_type (const_key (), const_value ()));
    }

    constexpr auto operator () (const_key k) noexcept
    {
        static_assert (functional<value_type>, "value_type is NOT callable!");
        return get (k)();
    }

    consteval auto operator () (const_key k) const noexcept
    {
        static_assert (functional<value_type>, "value_type is NOT callable!");
        return get<k> ()();
    }

    consteval size_type count (const_key k) const noexcept
    { return get_index<k> () != npos ? 1 : 0; }

    constexpr value_reference       operator [] (const_key   k)       { return get (k); }
    constexpr value_const_reference operator [] (const_key   k) const { return get (k); }
    constexpr key_reference         operator [] (const_value v)       { return get (v); }
    constexpr key_const_reference   operator [] (const_value v) const { return get (v); }

    constexpr value_reference       at (const_key   k)       { return get (k); }
    constexpr value_const_reference at (const_key   k) const { return get (k); }
    constexpr key_reference         at (const_value v)       { return get (v); }
    constexpr key_const_reference   at (const_value v) const { return get (v); }

    template <const_key K_>
    consteval value_reference at () noexcept { return get<K_> (); }

    template <const_key K_>
    consteval value_const_reference at () const noexcept { return get<K_> (); }

    template <const_value V_>
    consteval key_reference at () noexcept { return get<V_> (); }

    template <const_value V_>
    consteval key_const_reference at () const noexcept { return get<V_> (); }

    template <const_key K_>
    consteval bool contains () const noexcept
    {
        return get_index<K_> () != npos;
    }

    constexpr bool contains (const_key k) const noexcept
    {
        return get_index (k) != npos;
    }

    template <const_value V_>
    consteval bool contains () const noexcept
    {
        return get_index<V_> () != npos;
    }

    constexpr bool contains (const_value v) const noexcept
    {
        return get_index (v) != npos;
    }

    constexpr iterator       begin   ()       noexcept { return &_M_pairs[0]     ; }
    consteval const_iterator begin   () const noexcept { return &_M_pairs[0]     ; }
    consteval const_iterator cbegin  () const noexcept { return &_M_pairs[0]     ; }
    constexpr iterator       end     ()       noexcept { return &_M_pairs[N]     ; }
    consteval const_iterator end     () const noexcept { return &_M_pairs[N]     ; }
    consteval const_iterator cend    () const noexcept { return &_M_pairs[N]     ; }
    constexpr iterator       rbegin  ()       noexcept { return &_M_pairs[N  - 1]; }
    consteval const_iterator rbegin  () const noexcept { return &_M_pairs[N  - 1]; }
    consteval const_iterator crbegin () const noexcept { return &_M_pairs[N  - 1]; }
    constexpr iterator       rend    ()       noexcept { return &_M_pairs[0] - 1 ; }
    consteval const_iterator rend    () const noexcept { return &_M_pairs[0] - 1 ; }
    consteval const_iterator crend   () const noexcept { return &_M_pairs[0] - 1 ; }

    //! empty () method to check if map is empty
    consteval bool empty () const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i] != pair_type ()) return false;
        return true;
    }

    //! size_used () method to return number of used slots
    consteval size_type size_used () const noexcept
    {
        size_type count = 0;

        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].first != key_type ()) ++count;

        return count;
    }

    //! full () method to check if map is full
    consteval bool full () const noexcept
    {
        return size_used () == size ();
    }

    constexpr iterator find (const_key k) noexcept
    {
        auto const i = get_index (k); return i != npos ? &_M_pairs[i] : end ();
    }

    constexpr const_iterator find (const_key k) const noexcept
    {
        auto const i = get_index (k); return i != npos ? &_M_pairs[i] : end ();
    }

    template <const_key K_>
    consteval const_iterator find () const noexcept
    {
        return contains<K_> () ? &_M_pairs[get_index<K_> ()] : end ();
    }

    constexpr iterator find (const_value v) noexcept
    {
        auto const i = get_index (v); return i != npos ? &_M_pairs[i] : end ();
    }

    constexpr const_iterator find (const_value v) const noexcept
    {
        auto const i = get_index (v); return i != npos ? &_M_pairs[i] : end ();
    }

    template <const_value V_>
    consteval const_iterator find () const noexcept
    {
        return contains<V_> () ? &_M_pairs[get_index<V_> ()] : end ();
    }

    constexpr iterator_pair emplace (const_key k, const_value v) noexcept
    {
        return (get_index (k) == npos && _M_pairs[N - 1].first == key_type ()) ?
                   iterator_pair (&(_M_pairs[N - 1] = pair_type (k, v)), true) :
                   iterator_pair (end (), false);
    }

    constexpr void erase (const_key k) noexcept
    {
        auto const i = get_index (k);

        if (i != npos) _M_pairs[i] = pair_type ();
    }

    template <const_key K_>
    consteval void erase () noexcept
    {
        constexpr auto const i = get_index<K_> ();

        if (i != npos) _M_pairs[i] = pair_type ();
    }

    constexpr void clear () noexcept
    {
        for (auto i = 0; i < size (); ++i) _M_pairs[i] = pair_type ();
    }

protected:
    //! run-time get_index function
    constexpr size_type get_index (const_key k) const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].first == k) return i;
        return npos;
    }

    //! run-time get_index function
    constexpr size_type get_index (const_value v) const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].second == v) return i;
        return npos;
    }

    //! compile-time get_index function
    template <const_key K_>
    consteval size_type get_index () const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].first == K_) return i;
        return npos;
    }

    //! compile-time get_index function
    template <const_value V_>
    consteval size_type get_index () const noexcept
    {
        for (size_type i = 0; i < size (); ++i) if (_M_pairs[i].second == V_) return i;
        return npos;
    }

    constexpr value_reference get (const_key k)
    {
        const_size i = get_index (k);
        if (i == npos) throw std::runtime_error ("key NOT found in consteval_bimap");
        return _M_pairs[i].second;
    }

    template <const_key K_>
    consteval value_reference get () noexcept
    {
        constexpr const_size I = get_index<K_> ();
        static_assert (I != npos, "key NOT found in consteval_bimap");
        return _M_pairs[I].second;
    }

    constexpr value_const_reference get (const_key k) const
    {
        const_size i = get_index (k);
        if (i == npos) throw std::runtime_error ("key NOT found in consteval_bimap");
        return _M_pairs[i].second;
    }

    template <const_key K_>
    consteval value_const_reference get () const noexcept
    {
        constexpr const_size I = get_index<K_> ();
        static_assert (I != npos, "key NOT found in consteval_bimap");
        return _M_pairs[I].second;
    }

    constexpr key_reference get (const_value v)
    {
        const_size i = get_index (v);
        if (i == npos) throw std::runtime_error ("value NOT found in consteval_bimap");
        return _M_pairs[i].first;
    }

    template <const_value V_>
    consteval key_reference get () noexcept
    {
        constexpr const_size I = get_index<V_> ();
        static_assert (I != npos, "value NOT found in consteval_bimap");
        return _M_pairs[I].first;
    }

    constexpr key_const_reference get (const_value v) const
    {
        const_size i = get_index (v);
        if (i == npos) throw std::runtime_error ("value NOT found in consteval_bimap");
        return _M_pairs[i].first;
    }

    template <const_value V_>
    consteval key_const_reference get () const noexcept
    {
        constexpr const_size I = get_index<V_> ();
        static_assert (I != npos, "value NOT found in consteval_bimap");
        return _M_pairs[I].first;
    }

    //! safe access with optional return types
    template <const_key K_>
    consteval optional_value get_optional () const noexcept
    {
        const_size i = get_index<K_> ();
        return (i != npos) ? optional_value { _M_pairs[i].second } : std::nullopt;
    }

    template <const_value V_>
    consteval optional_key get_optional () const noexcept
    {
        const_size i = get_index<V_> ();
        return (i != npos) ? optional_key { _M_pairs[i].first } : std::nullopt;
    }

    constexpr optional_value get_optional (const_key k) const noexcept
    {
        const_size i = get_index (k);
        return (i != npos) ? optional_value { _M_pairs[i].second } : std::nullopt;
    }

    constexpr optional_key get_optional (const_value v) const noexcept
    {
        const_size i = get_index (v);
        return (i != npos) ? optional_key { _M_pairs[i].first } : std::nullopt;
    }

private:
    array_type _M_pairs;
};

// ====================================================

template <switch_value K = int, functional V = function<void()>, std::size_t N = 0, pair_like... Ps>
requires (switch_map_pair<Ps, K, V> && ...)
constexpr consteval_bimap<std::pair<K, V>, sizeof... (Ps)>
make_switch_map (Ps&&... case_pairs) noexcept
{
    constexpr const decltype (N) SZ = N == 0 ? sizeof... (Ps) : N;

    static_assert (sizeof... (Ps) <= SZ, "bimap has insufficient capacity!");
    return consteval_bimap<std::pair<K, V>, sizeof... (Ps)> { std::forward<Ps> (case_pairs)... };
}

// ====================================================

template <non_void K, non_void V, std::size_t N = 0, pair_like... Ps>
requires (bimap_pair<Ps, K, V> && ...)
constexpr consteval_bimap<std::pair<K, V>, sizeof... (Ps)>
make_consteval_bimap (Ps&&... pairs) noexcept
{
    constexpr const decltype (N) SZ = N == 0 ? sizeof... (Ps) : N;

    static_assert (sizeof... (Ps) <= SZ, "bimap has insufficient capacity!");
    return consteval_bimap<std::pair<K, V>, sizeof... (Ps)> { std::forward<Ps> (pairs)... };
}

// ====================================================

template <non_void... Ts>
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

} // cppual

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_FUNCTIONAL_META_H_