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

#ifndef CPPUAL_FAST_FUNC_H_
#define CPPUAL_FAST_FUNC_H_
#ifdef __cplusplus

#include <cppual/abi>
#include <cppual/casts>
#include <cppual/types>
#include <cppual/concepts>
#include <cppual/exception>
#include <cppual/meta_functional>
#include <cppual/memory_allocator>

#include <cassert>
#include <cstring>
#include <utility>
#include <tuple>
#include <array>

// ====================================================

namespace cppual {

// ====================================================

//! import needed concepts
using memory::allocator_t;

// ====================================================

class any_object;

template <non_void>
struct function_traits;

template <non_void, std::size_t>
class function;

// ====================================================

template <typename R, typename... Args>
using any_static_fn = any_fn_t<R(Args...)>;

template <typename R, typename... Args>
using any_member_fn = any_mem_fn_t<any_object, R(Args...)>;

template <typename R, typename... Args>
using any_const_member_fn = any_mem_fn_t<any_object, R(Args...) const>;

//! =========================================================

consteval std::size_t args_count (std::size_t const count = 5) noexcept
{
    return      count <=  5 ?
            5 : count <= 10 ?
           10 : count <= 15 ?
           15 : count <= 20 ?
           20 : count <= 25 ?
           25 : count <= 30 ?
           30 : count       ;
}

template <std::size_t N = 5>
inline constexpr static const decltype (N) max_args_v = args_count (N);

//! ====================================================

template <typename R, typename... Args>
struct function_traits <R(Args...)>
{
    typedef function_traits<R(Args...)> self_type  ;
    typedef std::size_t                 size_type  ;
    typedef size_type const             const_size ;
    typedef R                           return_type;
    typedef std::tuple<Args...>         args_type  ;

    using sig_type = R(Args...);

    /// arity is the number of arguments
    inline constexpr static const_size arity = sizeof... (Args);

    template <size_type I = 0>
    struct arg
    {
        static_assert (I < arity, "argument index is out of range!");
        typedef std::tuple_element_t<I, std::tuple<Args...>> type;
    };

    template <size_type I = 0>
    using arg_t = arg<I>::type;
};

template <typename R, typename... Args>
struct function_traits <R(Args...) const> : public function_traits<R(Args...)>
{
    typedef function_traits<R(Args...) const> self_type  ;
    typedef function_traits<R(Args...)>       base_type  ;
    typedef base_type::size_type              size_type  ;
    typedef size_type const                   const_size ;
    typedef base_type::return_type            return_type;
    typedef base_type::args_type              args_type  ;

    using sig_type = R(Args...) const;

    using base_type::arity;
    using base_type::arg  ;
    using base_type::arg_t;
};

template <typename R, typename... Args>
struct function_traits <R(*)(Args...)> : function_traits<R(Args...)>
{
    using function_ref = R(&)(Args...);
    using fn_ptr = R(*)(Args...);
};

template <class_t C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...)> : function_traits<R(Args...)>
{
    typedef remove_cref_t<C> object_type;
    using   fn_ptr = R(C::*)(Args...);
};

template <class_t C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...) const> : function_traits<R(Args...) const>
{
    typedef remove_cref_t<C> object_type;
    using   fn_ptr = R(C::*)(Args...) const;
};

template <auto F,
          typename = std::enable_if_t<std::is_function_v<std::remove_pointer_t<decltype (F)>> ||
                                      std::is_member_function_pointer_v<decltype (F)>>
          >
struct fn_ptr_traits : function_traits<decltype (F)>
{
    inline constexpr static decltype (F) const value = F;
};

/// deduce callable signature
template <typename T>
using function_traits_t = function_traits<T>::sig_type;

/// callable signature pointer -> value
template <auto F>
inline constexpr cbool fn_ptr_traits_v = fn_ptr_traits<F>::value;

//! ====================================================

template <typename R1, typename... Args1, typename R2, typename... Args2>
consteval bool operator == (function_traits<R1(Args1...)>, function_traits<R2(Args2...)>) noexcept
{
    return std::is_same_v<R1, R2>                 &&
           sizeof... (Args1) == sizeof... (Args2) &&
           ((std::is_same_v<Args1, Args2>)        && ...);
}

template <typename R1, typename... Args1, typename R2, typename... Args2>
consteval bool operator != (function_traits<R1(Args1...)>, function_traits<R2(Args2...)>) noexcept
{
    return !std::is_same_v<R1, R2>                 ||
            sizeof... (Args1) != sizeof... (Args2) ||
            ((!std::is_same_v<Args1, Args2>)       || ...);
}

//! ====================================================

template <typename R1, typename... Args1, typename R2, typename... Args2>
consteval bool operator == (function_traits<R1(Args1...) const>, function_traits<R2(Args2...) const>) noexcept
{
    return std::is_same_v<R1, R2>                 &&
           sizeof... (Args1) == sizeof... (Args2) &&
           ((std::is_same_v<Args1, Args2>)        && ...);
}

template <typename R1, typename... Args1, typename R2, typename... Args2>
consteval bool operator != (function_traits<R1(Args1...) const>, function_traits<R2(Args2...) const>) noexcept
{
    return !std::is_same_v<R1, R2>                 ||
            sizeof... (Args1) != sizeof... (Args2) ||
            ((!std::is_same_v<Args1, Args2>)       || ...);
}

//! ====================================================

template <typename R1, typename... Args1, typename... Args2>
consteval bool operator == (function_traits<R1(Args1...)>, std::tuple<Args2...>) noexcept
{
    return sizeof... (Args1) == sizeof... (Args2) && ((std::is_same_v<Args1, Args2>) && ...);
}

template <typename R1, typename... Args1, typename R2, typename... Args2>
consteval bool operator != (function_traits<R1(Args1...)>, std::tuple<Args2...>) noexcept
{
    return sizeof... (Args1) != sizeof... (Args2) || ((!std::is_same_v<Args1, Args2>) || ...);
}

//! ====================================================

template <typename R, typename... Args1, typename... Args2>
consteval bool operator == (function_traits<R(Args1...) const>, std::tuple<Args2...>) noexcept
{
    return sizeof... (Args1) == sizeof... (Args2) && ((std::is_same_v<Args1, Args2>) && ...);
}

template <typename R, typename... Args1, typename... Args2>
consteval bool operator != (function_traits<R(Args1...) const>, std::tuple<Args2...>) noexcept
{
    return sizeof... (Args1) != sizeof... (Args2) || ((!std::is_same_v<Args1, Args2>) || ...);
}

//! ====================================================

template <typename R, typename... Args1, typename... Args2>
consteval bool operator == (std::tuple<Args1...>, function_traits<R(Args2...)>) noexcept
{
    return sizeof... (Args1) == sizeof... (Args2) && ((std::is_same_v<Args1, Args2>) && ...);
}

template <typename R, typename... Args1, typename R2, typename... Args2>
consteval bool operator != (std::tuple<Args1...>, function_traits<R(Args2...)>) noexcept
{
    return sizeof... (Args1) != sizeof... (Args2) || ((!std::is_same_v<Args1, Args2>) || ...);
}

//! ====================================================

template <typename R, typename... Args1, typename... Args2>
consteval bool operator == (std::tuple<Args1...>, function_traits<R(Args2...) const>) noexcept
{
    return sizeof... (Args1) == sizeof... (Args2) && ((std::is_same_v<Args1, Args2>) && ...);
}

template <typename R, typename... Args1, typename... Args2>
consteval bool operator != (std::tuple<Args1...>, function_traits<R(Args2...) const>) noexcept
{
    return sizeof... (Args1) != sizeof... (Args2) || ((!std::is_same_v<Args1, Args2>) || ...);
}

//! ====================================================

//! closure class handling the function bindings
template <typename R, typename... Args>
class closure
{
public:
    typedef closure<R, Args...>       self_type     ;
    typedef std::size_t               size_type     ;
    typedef size_type const           const_size    ;
    typedef any_object*               pointer       ;
    typedef any_member_fn<R, Args...> value_type    ;
    typedef any_static_fn<R, Args...> static_fn_type;
    typedef value_type                member_fn_type;

    template <class_t C>
    using member_pair_t = std::pair<C*, any_mem_fn_t<C, R(Args...)>>;

    template <class_t C>
    using const_member_pair_t = std::pair<C*, any_mem_fn_t<C, R(Args...) const>>;

private:
    constexpr closure () noexcept = default;

    //! make class trivially copyable & movable
    constexpr closure (self_type&&) noexcept = default;
    constexpr closure (self_type const&) noexcept = default;
    constexpr self_type& operator = (self_type&&) noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr explicit closure (std::nullptr_t) noexcept
    : self_type ()
    { }

    constexpr closure (pointer const obj, value_type const fn) noexcept
    : _M_pObj (obj)
    , _M_fn   (fn )
    { }

    constexpr closure (std::pair<pointer, value_type> pair) noexcept
    : _M_pObj (pair.first )
    , _M_fn   (pair.second)
    { }

    constexpr self_type& operator = (std::nullptr_t) noexcept
    {
        _M_pObj = nullptr;
        _M_fn   = nullptr;
        return  * this   ;
    }

    template <class_t D, class_t C>
    constexpr static member_pair_t<D> make (C* obj, any_mem_fn_t<C, R(Args...)> fn) noexcept
    {
        return member_pair_t<D> (obj_ptr_cast<D> (obj), mem_fn_cast<D> (fn));
    }

    template <class_t D, class_t C>
    constexpr static member_pair_t<D> make (C* obj, any_mem_fn_t<C, R(Args...) const> fn) noexcept
    {
        return member_pair_t<D> (obj_ptr_cast<D> (obj), mem_fn_cast<D> (fn));
    }

    //! static helper for member function binding
    template <class_t C>
    constexpr
    static
    member_pair_t<any_object> make_member_fn (C* obj, any_mem_fn_t<C, R(Args...)> fn) noexcept
    {
        return make<any_object> (obj, fn);
    }

    template <class_t C>
    constexpr
    static
    member_pair_t<any_object> make_member_fn (C* obj, any_mem_fn_t<C, R(Args...) const> fn) noexcept
    {
        return make<any_object> (obj, fn);
    }

    //! static helper for static function binding
    template <class_t C>
    constexpr
    static
    member_pair_t<any_object> make_static_fn (R(* fn)(Args...), R(C::* invoker)(Args...)) noexcept
    {
        return member_pair_t<any_object> (direct_cast<pointer> (fn), direct_cast<value_type> (invoker));
    }

    template <class_t C>
    constexpr
    static
    member_pair_t<any_object> make_static_fn (R(* fn)(Args...), R(C::* invoker)(Args...) const) noexcept
    {
        return member_pair_t<any_object> (direct_cast<pointer> (fn), direct_cast<value_type> (invoker));
    }

    //! non-constexpr functions for actual binding
    template <class_t C>
    constexpr void bind_mem_func (C* obj, R(C::* fn)(Args...)) noexcept
    {
        auto [bound_obj, bound_fn] = make_member_fn (obj, fn);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

    template <class_t C>
    constexpr void bind_mem_func (C* obj, R(C::* fn)(Args...) const) noexcept
    {
        auto [bound_obj, bound_fn] = make_member_fn (obj, fn);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

    template <class_t C>
    constexpr void bind_static_func (R(* fn)(Args...), R(C::* invoker)(Args...)) noexcept
    {
        auto [bound_obj, bound_fn] = make_static_fn (fn, invoker);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

    template <class_t C>
    constexpr void bind_static_func (R(* fn)(Args...), R(C::* invoker)(Args...) const) noexcept
    {
        auto [bound_obj, bound_fn] = make_static_fn (fn, invoker);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

private:
    constexpr static_fn_type static_func () const noexcept
    { return direct_cast<static_fn_type> (_M_pObj); }

public:
    constexpr pointer object () const noexcept
    { return _M_pObj; }

    constexpr value_type fn_ptr () const noexcept
    { return _M_fn; }

    template <non_void, size_type>
    friend class function;

    template <typename>
    friend struct ::std::hash;

public:
    pointer    _M_pObj { };
    value_type _M_fn   { };
};

// ====================================================

template <typename R, typename... Args>
constexpr bool operator == (closure<R, Args...> const& lh, std::nullptr_t) noexcept
{ return lh._M_fn == nullptr || lh._M_pObj == nullptr; }

template <typename R, typename... Args>
constexpr bool operator == (std::nullptr_t, closure<R, Args...> const& rh) noexcept
{ return rh == nullptr; }

template <typename R, typename... Args>
constexpr bool operator == (closure<R, Args...> const& lh, closure<R, Args...> const& rh) noexcept
{ return lh._M_pObj == rh._M_pObj && lh._M_fn == rh._M_fn; }

template <typename R, typename... Args>
constexpr bool operator == (closure<R, Args...> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return lh.static_func () == &staticFnRef; }

template <typename R, typename... Args>
constexpr bool operator == (R(& staticFnRef)(Args...), closure<R, Args...> const& rh) noexcept
{ return rh == &staticFnRef; }

template <typename R, typename... Args>
constexpr bool operator != (closure<R, Args...> const& lh, std::nullptr_t) noexcept
{ return !(lh == nullptr); }

template <typename R, typename... Args>
constexpr bool operator != (std::nullptr_t, closure<R, Args...> const& rh) noexcept
{ return !(nullptr == rh); }

template <typename R, typename... Args>
constexpr bool operator != (closure<R, Args...> const& lh, closure<R, Args...> const& rh) noexcept
{ return !(lh == rh); }

template <typename R, typename... Args>
constexpr bool operator != (R(& staticFnRef)(Args...), closure<R, Args...> const& rh) noexcept
{ return !(staticFnRef = rh); }

template <typename R, typename... Args>
constexpr bool operator != (closure<R, Args...> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return !(lh == staticFnRef); }

template <typename R, typename... Args>
constexpr bool operator <=> (closure<R, Args...> const& lh, closure<R, Args...> const& rh)
{
    return lh._M_pObj <=> rh._M_pObj && lh._M_fn <=> rh._M_fn;
}

// template <typename R, typename... Args>
// constexpr bool operator < (closure<R, Args...> const& lh, closure<R, Args...> const& rh)
// {
//     return lh._M_pObj != rh._M_pObj ?
//                          lh._M_pObj < rh._M_pObj :
//                          std::memcmp (&lh._M_fn, &rh._M_fn, sizeof (lh._M_fn)) < 0;
// }

// template <typename R, typename... Args>
// constexpr bool operator > (closure<R, Args...> const& lh, closure<R, Args...> const& rh) noexcept
// {
//     return !(lh < rh) && lh != rh;
// }

//! ====================================================

//! reimplementation of impossibly fast delegates
template <typename R, typename... Args>
class SHARED_API function <R(Args...) const> : public function_traits<R(Args...) const>
{
public:
    typedef function<R(Args...) const>           self_type        ;
    typedef function_traits<R(Args...) const>    base_type        ;
    typedef any_member_fn  <R, Args...>          mem_fn_pointer   ;
    typedef any_static_fn  <R, Args...>          static_fn_pointer;
    typedef closure        <R, Args...>          closure_type     ;
    typedef closure_type const                   const_closure    ;
    typedef std::size_t                          size_type        ;
    typedef size_type const                      const_size       ;
    typedef closure_type::pointer                pointer          ;
    typedef closure_type::value_type             value_type       ;
    typedef std::array<byte, max_default_size_v> storage_type     ;
    typedef closure_type* self_type::*           safe_bool        ;

    template <size_type SZ>
    using self_type_t = function<R(Args...), max_capture_size_v<SZ>>;

    template <class_t C>
    using const_mem_fn_type = any_mem_fn_t<C, R(Args...) const>;

    template <class_t C>
    using const_mem_fn_pair = std::pair<C*, const_mem_fn_type<C>>;

    inline constexpr static const_size lambda_size = 0;

    //! make function class trivially copyable
    consteval function ()                              noexcept = default;
    constexpr function (self_type &&)                  noexcept = default;
    constexpr function (self_type const&)              noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr explicit function (std::nullptr_t) noexcept
    : self_type ()
    { }

    //! member function constructor
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr function (C& obj, const_mem_fn_type<C> mem_fn)
    : _M_closure (closure_type::make_member_fn (&obj, mem_fn))
    { }

    //! copy assignment operator
    template <size_type SZ>
    constexpr self_type& operator = (self_type_t<SZ> const& rh) noexcept
    {
        if (this == &rh) return *this;

        base_type::operator = (rh);
        _M_closure = rh._M_closure;

        return *this;
    }

    //! move assignment operator
    template <size_type SZ>
    constexpr self_type& operator = (self_type_t<SZ>&& rh) noexcept
    {
        if (this == &rh) return *this;

        base_type::operator = (std::move (rh));
        _M_closure = std::move (rh._M_closure);

        return *this;
    }

    constexpr self_type& operator = (std::nullptr_t) noexcept
    {
        _M_closure = nullptr;
        return *this;
    }

    //! member function assignment operator
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr self_type& operator = (const_mem_fn_pair<C> const& pair)
    {
        bind (pair.first, pair.second);
        return *this;
    }

    constexpr R operator () (Args... args) const
    {
        return (object ()->*fn_ptr ())(std::forward<Args> (args)...);
    }

    constexpr pointer object () const noexcept
    {
        return _M_closure.object ();
    }

    constexpr value_type fn_ptr () const noexcept
    {
        return _M_closure.fn_ptr ();
    }

    template <class_t C>
    constexpr const_mem_fn_type<C> fn_ptr () const noexcept
    {
        return direct_cast<const_mem_fn_type<C>> (_M_closure.fn_ptr ());
    }

    consteval static size_type arity () noexcept
    { return base_type::arity; }

    //! lambda capture storage size in bytes
    consteval static size_type size () noexcept
    { return lambda_size; }

    consteval static bool is_const () noexcept
    { return true; }

    consteval explicit operator safe_bool () const noexcept
    { return _M_closure != nullptr ? &self_type::_M_closure : nullptr; }

private:
    constexpr function (pointer const obj, value_type const fn) noexcept
    : _M_closure (obj, fn)
    { }

    constexpr const_closure& get_closure () const noexcept
    {
        return _M_closure;
    }

    constexpr closure_type& get_closure () noexcept
    {
        return _M_closure;
    }

    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr void bind (C& obj, const_mem_fn_type<C> fn)
    {
        _M_closure.bind_mem_func (&obj, fn);
    }

    //! friend functions & classes
    template <fn_sig, fn_sig, size_type>
    friend struct fn_cast_helper;

    template <non_void, size_type>
    friend class function;

public:
    closure_type _M_closure;
};

// ====================================================

//! reimplementation of impossibly fast delegates (const specialization)
template <typename R, typename... Args, std::size_t N>
class SHARED_API function<R(Args...), N> : public function<R(Args...) const>
{
public:
    typedef function<R(Args...), N>                 self_type    ;
    typedef function<R(Args...)  const>             base_type    ;
    typedef base_type::value_type                   value_type   ;
    typedef base_type::pointer                      pointer      ;
    typedef base_type::size_type                    size_type    ;
    typedef base_type::const_size                   const_size   ;
    typedef base_type::closure_type                 closure_type ;
    typedef base_type::const_closure                const_closure;
    typedef std::array<byte, max_capture_size_v<N>> storage_type ;
    typedef closure_type* self_type::*              safe_bool    ;

    using base_type::base_type  ;
    using base_type::operator=  ;
    using base_type::operator() ;
    using base_type::arity      ;
    using base_type::bind       ;
    using base_type::get_closure;
    using base_type::fn_ptr     ;
    using base_type::_M_closure ;

    using static_fn_ref = R(&)(Args...);

    template <size_type SZ>
    using self_type_t = function<R(Args...), max_capture_size_v<SZ>>;

    template <class_t C>
    using mem_fn_type = any_mem_fn_t<C, R(Args...)>;

    template <class_t C>
    using mem_fn_pair = closure_type::template member_pair_t<C>;

    template <class_t C>
    using const_mem_fn_type = any_mem_fn_t<C, R(Args...) const>;

    template <class_t C>
    using const_mem_fn_pair = closure_type::template const_member_pair_t<C>;

    inline constexpr static const_size lambda_size = max_capture_size_v<N>;

    //! static function constructor
    constexpr function (static_fn_ref fn)
    {
        bind (fn);
    }

    //! member function constructor
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr function (C& obj, mem_fn_type<C> mem_fn)
    {
        bind (obj, mem_fn);
    }

    //! TODO: non-capture lambda + callable class constructor
    //! callable class constructor
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr function (C& obj)
    {
        bind (obj, callable_operator_v<C>);
    }

    //! copy constructor
    template <size_type SZ>
    constexpr function (self_type_t<SZ> const& rh) noexcept
    : base_type (rh)
    {
        static_assert (lambda_size >= max_capture_size_v<SZ>, "function storage size mismatch!");

        std::copy (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());
    }

    constexpr function (base_type const& rh) noexcept
    : base_type (rh)
    { }

    //! move constructor
    template <size_type SZ>
    constexpr function (self_type_t<SZ>&& rh) noexcept
    : base_type (std::move (rh))
    {
        static_assert (lambda_size >= max_capture_size_v<SZ>, "function storage size mismatch!");

        std::move (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());
    }

    constexpr function (base_type&& rh) noexcept
    : base_type (std::move (rh))
    { }

    //! non-capture lambda constructor
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaNonCaptureType<Callable>
              >
    constexpr function (Callable&& callable, LambdaNonCaptureType<Callable>* = nullptr)
    {
        bind (callable, callable_operator_v<Callable>);
    }

    //! capture lambda constructor
    template <lambda_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaCaptureType<Callable>
              >
    constexpr function (Callable&& lambda, LambdaCaptureType<Callable>* = nullptr)
    {
        static_assert (sizeof (C) <= lambda_size,
                       "capture lambda too large for static storage! "
                       "DON'T value copy captured objects bigger than 16 bytes! "
                       "Use references instead!");

        ::new (&get_storage<C> ()) C (std::move (lambda));

        bind (get_storage<C> (), callable_operator_v<Callable>);
    }

    //! copy assignment operator
    template <size_type SZ>
    constexpr self_type& operator = (self_type_t<SZ> const& rh) noexcept
    {
        static_assert (lambda_size >= max_capture_size_v<SZ>, "function storage size mismatch!");

        if (this == &rh) return *this;

        base_type::operator = (rh);
        std::copy (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());

        return *this;
    }

    constexpr self_type& operator = (base_type const& rh) noexcept
    {
        if (this == &rh) return *this;

        base_type::operator = (rh);
        return *this;
    }

    //! move assignment operator
    template <size_type SZ>
    constexpr self_type& operator = (self_type_t<SZ>&& rh) noexcept
    {
        static_assert (lambda_size >= max_capture_size_v<SZ>, "function storage size mismatch!");

        if (this == &rh) return *this;

        base_type::operator = (std::move (rh));
        std::move (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());

        return *this;
    }

    constexpr self_type& operator = (base_type&& rh) noexcept
    {
        if (this == &rh) return *this;

        base_type::operator = (std::move (rh));
        return *this;
    }

    //! static function assignment operator
    constexpr self_type& operator = (static_fn_ref fn) noexcept
    {
        bind (fn);
        return *this;
    }

    //! member function assignment operator
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr self_type& operator = (mem_fn_pair<C> const& pair)
    {
        bind (pair.first, pair.second);
        return *this;
    }

    //! non-capture lambda assignment operator
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaNonCaptureType<Callable>
              >
    constexpr self_type& operator = (Callable&& callable)
    {
        bind (callable, callable_operator_v<Callable>);
        return *this;
    }

    //! capture lambda assignment operator
    template <lambda_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaCaptureType<Callable>
              >
    constexpr self_type& operator = (Callable&& callable)
    {
        static_assert (sizeof (C) <= lambda_size,
                      "capture lambda too large for static storage! "
                      "DON'T value copy captured objects bigger than 16 bytes! "
                      "Use references instead!");

        ::new (&get_storage<C> ()) C (std::move (callable));

        bind (get_storage<C> (), callable_operator_v<Callable>);
        return *this;
    }

    //! TODO: non-capture lambda + callable class assignment operator
    //! callable object assignment operator
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr self_type& operator = (C& obj)
    {
        bind (obj, callable_operator_v<C>);
        return *this;
    }

    constexpr R operator () (Args... args)
    {
        return (object ()->*fn_ptr ())(std::forward<Args> (args)...);
    }

    template <class_t C>
    constexpr C* object () const noexcept
    {
        return direct_cast<C*> (this);
    }

    template <class_t C>
    constexpr mem_fn_type<C> fn_ptr () noexcept
    {
        return direct_cast<mem_fn_type<C>> (_M_closure.fn_ptr ());
    }

private:
    constexpr R call_static_fn (Args... args) const
    {
        return (*(_M_closure.static_func ()))(std::forward<Args> (args)...);
    }

public:
    constexpr pointer object () const noexcept
    {
        return (fn_ptr<self_type> () == &self_type::call_static_fn) ?
                object<any_object> () : _M_closure.object ();
    }

    //! lambda storage size in bytes
    consteval static size_type size () noexcept
    { return lambda_size; }

    consteval static bool is_const () noexcept
    { return false; }

    consteval explicit operator safe_bool () const noexcept
    { return _M_closure != nullptr ? &self_type::_M_closure : nullptr; }

    //! friend functions & classes
    template <fn_sig, fn_sig, size_type>
    friend struct fn_cast_helper;

    template <non_void, size_type>
    friend class function;

private:
    constexpr function (pointer      const  obj,
                        value_type   const  fn ,
                        storage_type const& lsz = storage_type ()) noexcept
    : base_type  (obj, fn)
    , _M_storage (lsz)
    { }

    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr C& get_storage () const
    {
        return *direct_cast<C*> (&_M_storage[0]);
    }

    constexpr void bind (static_fn_ref fn) noexcept
    {
        _M_closure.bind_static_func (&fn, &self_type::call_static_fn);
    }

    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr void bind (C& obj, mem_fn_type<C> fn)
    {
        _M_closure.bind_mem_func (&obj, fn);
    }

    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr void bind (C& obj, const_mem_fn_type<C> fn)
    {
        _M_closure.bind_mem_func (&obj, fn);
    }

public:
    //! bytes for storage of capture lambda
    alignas (std::max_align_t) storage_type _M_storage;
};

// ====================================================

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (function<R(Args...), SZ> const& lh, std::nullptr_t) noexcept
{ return lh._M_closure == nullptr; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (function<R(Args...) const, SZ> const& lh, std::nullptr_t) noexcept
{ return lh._M_closure == nullptr; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (std::nullptr_t, function<R(Args...), SZ> const& lh) noexcept
{ return lh._M_closure == nullptr; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (std::nullptr_t, function<R(Args...) const, SZ> const& lh) noexcept
{ return lh._M_closure == nullptr; }

template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
constexpr bool operator == (function<R(Args...), SZ1> const& lh,
                            function<R(Args...), SZ2> const& rh) noexcept
{ return lh._M_closure == rh._M_closure && lh._M_storage == rh._M_storage; }

template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
constexpr bool operator == (function<R(Args...) const, SZ1> const& lh,
                            function<R(Args...) const, SZ2> const& rh) noexcept
{ return lh._M_closure == rh._M_closure && lh._M_storage == rh._M_storage; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (function<R(Args...), SZ> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return lh._M_closure == &staticFnRef; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (function<R(Args...) const, SZ> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return lh._M_closure == &staticFnRef; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (R(& staticFnRef)(Args...),
                            function<R(Args...), SZ> const& rh) noexcept
{ return rh._M_closure == &staticFnRef; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (R(& staticFnRef)(Args...),
                            function<R(Args...) const, SZ> const& rh) noexcept
{ return rh._M_closure == &staticFnRef; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (function<R(Args...), SZ> const& lh, std::nullptr_t) noexcept
{ return !(lh == nullptr); }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (function<R(Args...) const, SZ> const& lh, std::nullptr_t) noexcept
{ return !(lh == nullptr); }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (std::nullptr_t, function<R(Args...), SZ> const& rh) noexcept
{ return !(nullptr == rh); }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (std::nullptr_t, function<R(Args...)const, SZ> const& rh) noexcept
{ return !(nullptr == rh); }

template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
constexpr bool operator != (function<R(Args...), SZ1> const& lh,
                            function<R(Args...), SZ2> const& rh) noexcept
{ return !(lh == rh); }


template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
constexpr bool operator != (function<R(Args...) const, SZ1> const& lh,
                            function<R(Args...) const, SZ2> const& rh) noexcept
{ return !(lh == rh); }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (R(& staticFnRef)(Args...),
                            function<R(Args...), SZ> const& rh) noexcept
{ return !(staticFnRef == rh); }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (R(& staticFnRef)(Args...),
                            function<R(Args...) const, SZ> const& rh) noexcept
{ return !(staticFnRef == rh); }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (function<R(Args...), SZ> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return !(lh == staticFnRef); }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator != (function<R(Args...) const, SZ> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return !(lh == staticFnRef); }

template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
constexpr bool operator <=> (function<R(Args...), SZ1> const& lh,
                             function<R(Args...), SZ2> const& rh)
{ return lh._M_closure <=> rh._M_closure; }

// template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
// constexpr bool operator  < (function<R(Args...), SZ1> const& lh,
//                             function<R(Args...), SZ2> const& rh)
// { return lh._M_closure < rh._M_closure; }

// template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
// constexpr bool operator < (function<R(Args...) const, SZ1> const& lh,
//                             function<R(Args...) const, SZ2> const& rh)
// { return lh._M_closure < rh._M_closure; }

// template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
// constexpr bool operator > (function<R(Args...), SZ1> const& lh,
//                             function<R(Args...), SZ2> const& rh)
// { return lh._M_closure > rh._M_closure; }

// template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
// constexpr bool operator > (function<R(Args...) const, SZ1> const& lh,
//                             function<R(Args...) const, SZ2> const& rh)
// { return lh._M_closure > rh._M_closure; }

//! ====================================================

//! static function make_fn
template <typename R, typename... Args>
constexpr function<R(Args...)> make_fn (R (& static_fn)(Args...))
{
    return function<R(Args...)> (static_fn);
}

//! member function make_fn
template <class_t  C        ,
          typename R        ,
          typename... Args,
          typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* mem_fn)(Args...))
{
    return function<R(Args...)> (obj, mem_fn);
}

//! const member function make_fn
template <class_t  C        ,
          typename R        ,
          typename... Args,
          typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* const_mem_fn)(Args...) const)
{
    return function<R(Args...)> (obj, const_mem_fn);
}

template <class_t  C        ,
          typename R        ,
          typename... Args,
          typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
constexpr function<R(Args...) const> make_const_fn (C& obj, R (C::* const_mem_fn)(Args...) const)
{
    return function<R(Args...) const> (obj, const_mem_fn);
}

//! capture lambda make_fn
template <lambda_capture_t Callable,
          typename... Args         ,
          typename    R = callable_return_t<Callable, Args...>,
          typename    C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
          std::size_t N = lambda_calc_size<sizeof (C)> (),
          typename      = LambdaCaptureType<Callable>
          >
constexpr function<R(Args...), N> make_fn (Callable&& callable, LambdaCaptureType<Callable>* ptr = nullptr)
{
    return function<R(Args...), N> (std::move (callable), ptr);
}

//! non-capture lambda make_fn
template <lambda_non_capture_t Callable,
          typename... Args,
          typename R = callable_return_t<Callable, Args...>,
          typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
          typename   = LambdaNonCaptureType<Callable>
          >
constexpr function<R(Args...)> make_fn (Callable&& callable, LambdaNonCaptureType<Callable>* ptr = nullptr)
{
    return function<R(Args...)> (std::move (callable), ptr);
}

//! callable object make_fn
template <callable_class_t C,
          typename... Args,
          typename R = callable_return_t<C, Args...>,
          typename   = std::enable_if_t<!is_functional_v<std::decay_t<C>>>
          >
constexpr function<R(Args...)> make_fn (C& obj)
{
    return function<R(Args...)> (obj);
}

//! ====================================================
//! function signature cast helpers
//! ====================================================

template <fn_sig, fn_sig, std::size_t = max_default_size_v>
struct fn_cast_helper;

// ====================================================

template <typename R1, typename... Args1, typename R2, typename... Args2, std::size_t N>
struct fn_cast_helper <R1(Args1...), R2(Args2...), N>
{
    typedef decltype (N)    size_type ;
    typedef size_type const const_size;

    using type1 = R1(Args1...);
    using type2 = R2(Args2...);

    constexpr
    static
    function<type1, max_capture_size_v<N>>
    fn_cast (function<type2, N> const& fn_in) noexcept
    {
        constexpr const_size SZ = max_capture_size_v<N>;

        return function<type1, SZ>
              (fn_in._M_closure.object (),
               direct_cast<typename function<type1, SZ>::value_type>
              (fn_in._M_closure.fn_ptr ()),
               fn_in._M_storage);
    }
};

template <typename R1, typename... Args1, typename R2, typename... Args2, std::size_t N>
struct fn_cast_helper <R1(Args1...) const, R2(Args2...) const, N>
{
    typedef decltype (N)    size_type ;
    typedef size_type const const_size;

    using type1 = R1(Args1...) const;
    using type2 = R2(Args2...) const;

    constexpr
    static
    function<type1, max_default_size_v>
    fn_cast (function<type2, N> const& fn_in) noexcept
    {
        return function<type1, N>
              (fn_in._M_closure.object (),
               direct_cast<typename function<type1, N>::value_type>
              (fn_in._M_closure.fn_ptr ()));
    }
};

template <typename R1, typename... Args1, typename R2, typename... Args2, std::size_t N>
struct fn_cast_helper <R1(Args1...) const, R2(Args2...), N>
{
    typedef decltype (N)    size_type ;
    typedef size_type const const_size;

    using type1 = R1(Args1...) const;
    using type2 = R2(Args2...)      ;

    constexpr
    static
    function<type1, max_default_size_v>
    fn_cast (function<type2, N> const& fn_in) noexcept
    {
        constexpr const_size SZ = max_default_size_v;

        return function<type1, SZ>
              (fn_in._M_closure.object (),
               direct_cast<typename function<type1, SZ>::value_type>
              (fn_in._M_closure.fn_ptr ()));
    }
};

template <typename R1, typename... Args1, typename R2, typename... Args2, std::size_t N>
struct fn_cast_helper <R1(Args1...), R2(Args2...) const, N>
{
    using type1 = R1(Args1...)      ;
    using type2 = R2(Args2...) const;

    constexpr
    static
    function<type1, N>
    fn_cast (function<type2, N> const& fn_in) noexcept
    {
        return function<type1, N>
              (fn_in._M_closure.object (),
               direct_cast<typename function<type1, N>::value_type>
              (fn_in._M_closure.fn_ptr ()));
    }
};

// ====================================================

template <fn_sig Out, fn_sig In, std::size_t N>
constexpr
function<Out, is_const_fn_sig_v<Out> ? max_default_size_v : max_capture_size_v<N>>
fn_cast (function<In, N> const& fn_in) noexcept
{
    constexpr decltype (N) const SZ = is_const_fn_sig_v <Out> ?
                                      max_capture_size_v<   > :
                                      max_capture_size_v< N > ;

    return function<Out, SZ> (fn_cast_helper<Out, In, N>::fn_cast (fn_in));
}

//! ====================================================

template <std::size_t N = max_args_v<>>
class function_rtti
{
public:
    typedef function_rtti<N>                     self_type  ;
    typedef decltype (N)                         size_type  ;
    typedef size_type const                      const_size ;
    typedef function<void()>                     any_fn_type;
    typedef abi::rtti                            rtti_type  ;
    typedef std::array<rtti_type, max_args_v<N>> array_type ;

    template <non_void S, size_type SZ = max_default_size_v>
    using fn_t = function<S, max_capture_size_v<SZ>>;

    template <size_type SZ>
    using self_type_t = function_rtti<max_args_v<SZ>>;

    consteval static size_type arity () noexcept
    { return N; }

    constexpr function_rtti () noexcept = default;

    template <size_type SZ = N, typename = std::enable_if_t<max_args_v<N> >= max_args_v<SZ>>>
    constexpr function_rtti (self_type_t<SZ>&& rh) noexcept
    : _M_fn        (rh._M_fn       )
    , _M_arg_types (rh._M_arg_types)
    , _M_ret_type  (rh._M_ret_type )
    {
        rh = self_type_t<SZ> (); //! clear the moved object
    }

    template <size_type SZ = N, typename = std::enable_if_t<max_args_v<N> >= max_args_v<SZ>>>
    constexpr function_rtti (self_type_t<SZ> const& rh) noexcept
    : _M_fn        (rh._M_fn       )
    , _M_arg_types (rh._M_arg_types)
    , _M_ret_type  (rh._M_ret_type )
    { }

    template <size_type SZ = N, typename = std::enable_if_t<max_args_v<N> >= max_args_v<SZ>>>
    constexpr self_type& operator = (self_type_t<SZ>&& rh) noexcept
    {
        if (this == &rh) return *this; //! self-assignment check

        _M_fn        = rh._M_fn       ;
        _M_arg_types = rh._M_arg_types;
        _M_ret_type  = rh._M_ret_type ;

        rh = self_type_t<SZ> (); //! clear the moved object

        return *this;
    }

    template <size_type SZ = N, typename = std::enable_if_t<max_args_v<N> >= max_args_v<SZ>>>
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

    constexpr any_fn_type function    () const noexcept { return _M_fn       ; }
    constexpr array_type  arg_types   () const noexcept { return _M_arg_types; }
    constexpr rtti_type   return_type () const noexcept { return _M_ret_type ; }

    template <size_type I>
    consteval rtti_type arg_type () const noexcept
    {
        static_assert (I < max_args_v<N>, "function_rtti::arg_type: index out of range!");
        return _M_arg_types[I];
    }

private:
    template <typename R, typename... Args, size_type SZ = max_default_size_v>
    constexpr function_rtti (fn_t<R(Args...), SZ> const& fn)
    : _M_fn        ( fn_cast<void()> (fn)      )
    , _M_arg_types { abi::rtti (type<Args>)... }
    , _M_ret_type  ( type<R>                   )
    { }

    template <typename R, typename... Args>
    constexpr function_rtti (fn_t<R(Args...) const> const& fn)
    : _M_fn        ( fn_cast<void() const> (fn))
    , _M_arg_types { abi::rtti (type<Args>)... }
    , _M_ret_type  ( type<R>                   )
    { }

    template <size_type>
    friend class function_rtti;

    template <unsigned_integer_t, functional...>
    friend class unbound_interface    ;
    friend class dyn_unbound_interface;

    template <size_type SZ, typename R, typename... Args>
    friend
    constexpr
    self_type_t<sizeof... (Args)>
    make_fn_rtti (fn_t<R(Args...), SZ> const&) noexcept;

private:
    any_fn_type _M_fn       ;
    array_type  _M_arg_types;
    rtti_type   _M_ret_type ;
};

// ====================================================

template <std::size_t SZ = max_default_size_v, typename R, typename... Args>
constexpr
function_rtti<max_args_v<sizeof... (Args)>>
make_fn_rtti (function<R(Args...), max_capture_size_v<SZ>> const& fn) noexcept
{
    return function_rtti<max_args_v<sizeof... (Args)>> (fn);
}

template <typename R, typename... Args>
constexpr
function_rtti<max_args_v<sizeof... (Args)>>
make_fn_rtti (function<R(Args...) const> const& fn) noexcept
{
    return function_rtti<max_args_v<sizeof... (Args)>> (fn);
}

// ====================================================

} // namespace cppual

// ====================================================

namespace std {

using cppual::non_void          ;
using cppual::switch_value_t    ;
using cppual::max_default_size_v;
using cppual::max_capture_size_v;

// ====================================================

template <typename R, typename... Args, std::size_t N>
struct hash <cppual::function<R(Args...), N>>
{
    typedef decltype (N)                 size_type ;
    typedef cppual::function<R(Args...)> value_type;

    constexpr size_type operator () (value_type& fn) const noexcept
    {
        return cppual::direct_cast<size_type> (fn.object ()) ^
               cppual::direct_cast<size_type> (fn.fn_ptr ()) ;
    }
};

template <typename R, typename... Args, std::size_t N>
struct hash <cppual::function<R(Args...) const, N>>
{
    typedef decltype (N)                       size_type ;
    typedef cppual::function<R(Args...) const> value_type;

    constexpr size_type operator () (value_type& fn) const noexcept
    {
        return cppual::direct_cast<size_type> (fn.object ()) ^
               cppual::direct_cast<size_type> (fn.fn_ptr ()) ;
    }
};

// ====================================================

template <size_t N = max_default_size_v, switch_value_t T, typename... Args, typename R>
constexpr auto make_pair (T const _case, Args... fn_args, R* = cppual::void_ptr ()) noexcept
-> pair<T, cppual::function<R(Args...), N>>
{
    using  fn_type = cppual::function<R(Args...), N>;
    return pair<T, fn_type> (_case, cppual::make_fn (forward<Args> (fn_args)...));
}

template <size_t N = max_default_size_v, switch_value_t T, typename... Args>
constexpr auto make_pair (T const _case, Args... fn_args) noexcept
-> pair<T, cppual::function<void(Args...), N>>
{
    using  fn_type = cppual::function<void(Args...), N>;
    return pair<T, fn_type> (_case, cppual::make_fn (forward<Args> (fn_args)...));
}

// ====================================================

} // namespace std

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
