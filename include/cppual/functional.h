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

#include <cppual/casts>
#include <cppual/types>
#include <cppual/concepts>
#include <cppual/exception>
#include <cppual/meta_type>
#include <cppual/containers>
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

class any_object;

template <non_void> struct function_traits;
//template <fn_sig, std::size_t> class function;

// ====================================================

template <typename R, typename... Args>
using any_static_fn = fn_ptr_t<void, R(Args...)>;

template <typename R, typename... Args>
using any_member_fn = fn_ptr_t<any_object, R(Args...)>;

template <typename R, typename... Args>
using any_const_member_fn = fn_ptr_t<any_object, R(Args...) const>;

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

inline constexpr static const std::size_t def_arity_v = args_count ();
template <decltype (def_arity_v) N = def_arity_v>
inline constexpr static const decltype (N) arity_v = args_count (N);

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

    //! arity is the number of arguments
    inline constexpr static const_size arity = sizeof... (Args);

    template <size_type I = 0>
    struct arg
    {
        static_assert (I < arity, "argument index is out of range!");
        typedef selected_tuple_t<std::tuple<Args...>, I> type;
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
    using function_ptr = R(*)(Args...);
};

template <structure C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...)> : function_traits<R(Args...)>
{
    typedef remove_const_t<C>   object_type;
    using   function_ptr = R(C::*)(Args...);
};

template <structure C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...) const> : function_traits<R(Args...) const>
{
    typedef remove_const_t<C>         object_type;
    using   function_ptr = R(C::*)(Args...) const;
};

template <auto F, std::enable_if_t<function_like<decltype (F)>, int> = 0>
struct fn_ptr_traits : function_traits <decltype (F)>
{
    inline constexpr static const decltype (F) value = F;
};

//! deduce callable signature
template <fn_sig T>
using function_traits_t = function_traits<T>::sig_type;

//! callable signature pointer -> value
template <auto F>
inline constexpr cbool fn_ptr_traits_v = fn_ptr_traits<F>::value;

//! ====================================================

template <typename R1, typename... Args1, typename R2, typename... Args2>
consteval auto operator <=> (function_traits<R1(Args1...)>, function_traits<R2(Args2...)>) noexcept
{
    return are_same<R1, R2>                      &&
          (are_same<Args1, Args2> && ...)        &&
           sizeof... (Args1) <=> sizeof... (Args2);
}

//! ====================================================

template <typename R1, typename... Args1, typename R2, typename... Args2>
consteval auto operator <=> (function_traits<R1(Args1...) const>, function_traits<R2(Args2...) const>) noexcept
{
    return are_same<R1, R2>                      &&
          (are_same<Args1, Args2> && ...)        &&
           sizeof... (Args1) <=> sizeof... (Args2);
}

//! ====================================================

template <typename R1, typename... Args1, typename... Args2>
consteval auto operator <=> (function_traits<R1(Args1...)>, std::tuple<Args2...>) noexcept
{
    return sizeof... (Args1) <=> sizeof... (Args2) && (are_same<Args1, Args2> && ...);
}

//! ====================================================

template <typename R, typename... Args1, typename... Args2>
consteval auto operator <=> (function_traits<R(Args1...) const>, std::tuple<Args2...>) noexcept
{
    return sizeof... (Args1) <=> sizeof... (Args2) && (are_same<Args1, Args2> && ...);
}

//! ====================================================

template <typename R, typename... Args1, typename... Args2>
consteval auto operator <=> (std::tuple<Args1...>, function_traits<R(Args2...)>) noexcept
{
    return sizeof... (Args1) <=> sizeof... (Args2) && (are_same<Args1, Args2> && ...);
}

//! ====================================================

template <typename R, typename... Args1, typename... Args2>
consteval auto operator <=> (std::tuple<Args1...>, function_traits<R(Args2...) const>) noexcept
{
    return sizeof... (Args1) <=> sizeof... (Args2) && (are_same<Args1, Args2> && ...);
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
    typedef any_object const*         const_pointer ;
    typedef any_member_fn<R, Args...> value_type    ;
    typedef value_type const          const_value   ;
    typedef value_type                member_fn_type;
    typedef any_static_fn<R, Args...> static_fn_type;

    template <structure C>
    using member_pair_t = std::pair<C*, fn_ptr_t<C, R(Args...)>>;

    template <structure C>
    using const_member_pair_t = std::pair<C*, fn_ptr_t<C, R(Args...) const>>;

private:
    constexpr closure ()                               noexcept = default;
    constexpr closure (self_type &&)                   noexcept = default;
    constexpr closure (self_type const&)               noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    consteval closure (pointer const obj, value_type const fn) noexcept
    : _M_pObj (obj)
    , _M_fn   (fn )
    { }

    constexpr closure (std::pair<pointer, value_type> pair) noexcept
    : _M_pObj (pair.first )
    , _M_fn   (pair.second)
    { }

    constexpr self_type& operator = (null_ptr) noexcept
    {
        _M_pObj = nullptr;
        _M_fn   = nullptr;
        return  * this   ;
    }

    template <structure D, structure C>
    constexpr static member_pair_t<D> make (C* obj, fn_ptr_t<C, R(Args...)> fn) noexcept
    {
        return member_pair_t<D> (class_ptr_cast<D> (obj), mem_fn_cast<D> (fn));
    }

    template <structure D, structure C>
    constexpr static member_pair_t<D> make (C* obj, fn_ptr_t<C, R(Args...) const> fn) noexcept
    {
        return member_pair_t<D> (class_ptr_cast<D> (obj), mem_fn_cast<D> (fn));
    }

    //! static helper for member function binding
    template <structure C>
    constexpr
    static
    member_pair_t<any_object> make_member_fn (C* obj, fn_ptr_t<C, R(Args...)> fn) noexcept
    {
        return make<any_object> (obj, fn);
    }

    template <structure C>
    constexpr
    static
    member_pair_t<any_object> make_member_fn (C* obj, fn_ptr_t<C, R(Args...) const> fn) noexcept
    {
        return make<any_object> (obj, fn);
    }

    //! static helper for static function binding
    template <structure C>
    constexpr
    static
    member_pair_t<any_object> make_static_fn (R(* fn)(Args...), R(C::* invoker)(Args...)) noexcept
    {
        return member_pair_t<any_object> (direct_cast<pointer> (fn), direct_cast<value_type> (invoker));
    }

    template <structure C>
    constexpr
    static
    member_pair_t<any_object> make_static_fn (R(* fn)(Args...), R(C::* invoker)(Args...) const) noexcept
    {
        return member_pair_t<any_object> (direct_cast<pointer> (fn), direct_cast<value_type> (invoker));
    }

    //! non-constexpr functions for actual binding
    template <structure C>
    constexpr void bind_mem_func (C* obj, R(C::* fn)(Args...)) noexcept
    {
        auto [bound_obj, bound_fn] = make_member_fn (obj, fn);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

    template <structure C>
    constexpr void bind_mem_func (C* obj, R(C::* fn)(Args...) const) noexcept
    {
        auto [bound_obj, bound_fn] = make_member_fn (obj, fn);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

    template <structure C>
    constexpr void bind_static_func (R(* fn)(Args...), R(C::* invoker)(Args...)) noexcept
    {
        auto [bound_obj, bound_fn] = make_static_fn (fn, invoker);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

    template <structure C>
    constexpr void bind_static_func (R(* fn)(Args...), R(C::* invoker)(Args...) const) noexcept
    {
        auto [bound_obj, bound_fn] = make_static_fn (fn, invoker);

        _M_pObj = bound_obj;
        _M_fn   = bound_fn ;
    }

    constexpr pointer object () const noexcept
    { return _M_pObj; }

    constexpr value_type fn_ptr () const noexcept
    { return _M_fn; }

    constexpr static_fn_type static_func () const noexcept
    { return direct_cast<static_fn_type> (_M_pObj); }

    template <fn_sig, size_type>
    friend class function;

    template <typename>
    friend struct ::std::hash;

public:
    pointer    _M_pObj { };
    value_type _M_fn   { };
};

// ====================================================

template <typename R, typename... Args>
constexpr auto operator == (closure<R, Args...> const& lh, null_ptr) noexcept
{ return lh._M_fn == nullptr || lh._M_pObj == nullptr; }


template <typename R, typename... Args>
constexpr auto operator != (closure<R, Args...> const& lh, null_ptr) noexcept
{ return !(lh == nullptr); }

template <typename R, typename... Args>
constexpr auto operator == (null_ptr, closure<R, Args...> const& rh) noexcept
{ return nullptr == rh._M_fn || nullptr == rh._M_pObj; }

template <typename R, typename... Args>
constexpr auto operator != (null_ptr, closure<R, Args...> const& rh) noexcept
{ return !(nullptr == rh); }

template <typename R, typename... Args>
constexpr auto operator <=> (closure<R, Args...> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return lh.static_func () <=> &staticFnRef; }

template <typename R, typename... Args>
constexpr auto operator <=> (R(& staticFnRef)(Args...), closure<R, Args...> const& rh) noexcept
{ return &staticFnRef <=> rh.static_func (); }

template <typename R, typename... Args>
constexpr bool operator == (closure<R, Args...> const& lh, closure<R, Args...> const& rh) noexcept
{
    return lh._M_pObj == rh._M_pObj && lh._M_fn == rh._M_fn;
}

template <typename R, typename... Args>
constexpr auto operator <=> (closure<R, Args...> const& lh, closure<R, Args...> const& rh) noexcept
{
    if (auto cmp = lh._M_pObj <=> rh._M_pObj; cmp != 0) return cmp;
    return lh._M_fn <=> rh._M_fn;
}

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
    typedef std::array<byte, def_capture_size_v> storage_type     ;
    typedef closure_type* self_type::*           safe_bool        ;
    typedef base_type::return_type               return_type      ;

    template <size_type SZ>
    using self_type_t = function<R(Args...), max_capture_size_v<SZ>>;

    template <structure C>
    using const_mem_fn_type = fn_ptr_t<C, R(Args...) const>;

    template <structure C>
    using const_mem_fn_pair = std::pair<C&, const_mem_fn_type<C>>;

    //! make function class trivially copyable
    constexpr function ()                              noexcept = default;
    constexpr function (self_type &&)                  noexcept = default;
    constexpr function (self_type const&)              noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    //! member function constructor
    template <class_and_non_functional C>
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

    //! member function assignment operator
    template <class_and_non_functional C>
    constexpr self_type& operator = (const_mem_fn_pair<C> const& pair)
    {
        bind (pair.first, pair.second);
        return *this;
    }

    constexpr self_type& operator = (null_ptr) noexcept
    {
        _M_closure = nullptr;
        return *this;
    }

    constexpr return_type operator () (Args&&... args) const
    {
        return (object ()->*fn_ptr ())(std::forward<Args> (args)...);
    }

    template <class_and_non_functional C>
    constexpr void bind (C& obj, const_mem_fn_type<C> fn)
    {
        _M_closure.bind_mem_func (&obj, fn);
    }

    //! function arguments count
    consteval static size_type arity () noexcept
    { return base_type::arity; }

    //! lambda capture storage size in bytes
    consteval static size_type size () noexcept
    { return 0; }

    consteval static bool is_const () noexcept
    { return true; }

    consteval operator safe_bool () const noexcept
    { return _M_closure != nullptr ? &self_type::_M_closure : nullptr; }

private:
    consteval function (pointer const obj, value_type const fn) noexcept
    : _M_closure (obj, fn)
    { }

    constexpr pointer object () const noexcept
    {
        return _M_closure.object ();
    }

    constexpr value_type fn_ptr () const noexcept
    {
        return _M_closure.fn_ptr ();
    }

    template <structure C>
    constexpr const_mem_fn_type<C> fn_ptr () const noexcept
    {
        return direct_cast<const_mem_fn_type<C>> (_M_closure.fn_ptr ());
    }

    constexpr const_closure& get_closure () const noexcept
    {
        return _M_closure;
    }

    constexpr closure_type& get_closure () noexcept
    {
        return _M_closure;
    }

    //! friend functions & classes
    template <fn_sig, fn_sig, size_type>
    friend struct fn_cast_helper;

    template <fn_sig, size_type>
    friend class function;

public:
    closure_type _M_closure;
};

// ====================================================

//! reimplementation of impossibly fast delegates (const specialization)
template <typename R, typename... Args, decltype (def_capture_size_v) N>
class SHARED_API function <R(Args...), N> : public function<R(Args...) const>
{
public:
    typedef function<R(Args...), N>                 self_type        ;
    typedef function<R(Args...) const>              base_type        ;
    typedef base_type::value_type                   value_type       ;
    typedef base_type::pointer                      pointer          ;
    typedef base_type::size_type                    size_type        ;
    typedef base_type::const_size                   const_size       ;
    typedef closure<R, Args...>                     closure_type     ;
    typedef closure_type const                      const_closure    ;
    typedef std::array<byte, max_capture_size_v<N>> storage_type     ;
    typedef storage_type &                          storage_ref      ;
    typedef storage_type const&                     storage_const_ref;
    typedef storage_type * self_type::*             safe_bool        ;
    typedef base_type::return_type                  return_type      ;

    using base_type::operator ();
    using base_type::arity      ;
    using base_type::bind       ;
    using base_type::get_closure;

    using static_fn_ref = R(&)(Args...);

    template <size_type SZ>
    using self_type_t = function<R(Args...), max_capture_size_v<SZ>>;

    template <structure C>
    using mem_fn_type = fn_ptr_t<C, R(Args...)>;

    template <structure C>
    using mem_fn_pair = closure_type::template member_pair_t<C>;

    template <structure C>
    using const_mem_fn_type = fn_ptr_t<C, R(Args...) const>;

    template <structure C>
    using const_mem_fn_pair = closure_type::template const_member_pair_t<C>;

    //! lambda storage size in bytes
    consteval static size_type size () noexcept { return max_capture_size_v<N>; }

    //! make function class trivially copyable
    constexpr function ()                              noexcept = default;
    constexpr function (self_type &&)                  noexcept = default;
    constexpr function (self_type const&)              noexcept = default;
    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    //! static function constructor
    constexpr function (static_fn_ref fn)
    { bind (fn); }

    //! copy constructor
    template <size_type SZ>
    constexpr function (self_type_t<SZ> const& rh) noexcept
    : base_type (rh)
    {
        static_assert (size () >= max_capture_size_v<SZ>, "function storage size mismatch!");

        std::copy (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());
    }

    //! move constructor
    template <size_type SZ>
    constexpr function (self_type_t<SZ>&& rh) noexcept
    : base_type (std::move (rh))
    {
        static_assert (size () >= max_capture_size_v<SZ>, "function storage size mismatch!");

        std::move (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());
    }

    //! member function constructor
    template <class_and_non_functional C>
    constexpr function (C& obj, mem_fn_type<C> mem_fn)
    { bind (obj, mem_fn); }

    //! TODO: non-capture lambda + callable class constructor
    //! callable class constructor
    template <callable_class C> requires (!functional<C>)
    constexpr function (C& obj)
    { bind (obj, callable_operator_v<C>); }

    //! non-capture lambda constructor
    template <lambda_non_capture Callable>
    constexpr function (Callable&& callable, Callable* = nullptr)
    {
        bind (callable, callable_operator_v<Callable>);
    }

    //! capture lambda constructor
    template <lambda_capture Callable>
    constexpr function (Callable&& lambda, Callable* = nullptr)
    {
        typedef std::decay_t<Callable> C;

        static_assert (sizeof (C) <= size (),
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
        static_assert (size () >= max_capture_size_v<SZ>, "function storage size mismatch!");

        if (this == &rh) return *this;

        base_type::operator = (rh);
        std::copy (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());

        return *this;
    }

    //! move assignment operator
    template <size_type SZ>
    constexpr self_type& operator = (self_type_t<SZ>&& rh) noexcept
    {
        static_assert (size () >= max_capture_size_v<SZ>, "function storage size mismatch!");

        if (this == &rh) return *this;

        base_type::operator = (std::move (rh));
        std::move (rh._M_storage.begin (), rh._M_storage.end (), _M_storage.begin ());

        return *this;
    }

    //! static function assignment operator
    constexpr self_type& operator = (static_fn_ref fn) noexcept
    {
        bind (fn);
        return *this;
    }

    //! member function assignment operator
    template <class_and_non_functional C>
    constexpr self_type& operator = (mem_fn_pair<C> const& pair)
    {
        bind (pair.first, pair.second);
        return *this;
    }

    //! non-capture lambda assignment operator
    template <lambda_non_capture Callable>
    constexpr self_type& operator = (Callable&& callable)
    {
        bind (callable, callable_operator_v<Callable>);
        return *this;
    }

    //! capture lambda assignment operator
    template <lambda_capture Callable>
    constexpr self_type& operator = (Callable&& callable)
    {
        typedef std::decay_t<Callable> C;

        static_assert (sizeof (C) <= size (),
                      "capture lambda too large for static storage! "
                      "DON'T value copy captured objects bigger than 16 bytes! "
                      "Use references instead!");

        ::new (&get_storage<C> ()) C (std::move (callable));

        bind (get_storage<C> (), callable_operator_v<Callable>);
        return *this;
    }

    //! TODO: non-capture lambda + callable class assignment operator
    //! callable object assignment operator
    template <class_and_non_functional C> requires (callable_class<C>)
    constexpr self_type& operator = (C& obj)
    {
        bind (obj, callable_operator_v<C>);
        return *this;
    }

    constexpr self_type& operator = (null_ptr) noexcept
    {
        get_closure () = nullptr;
        return *this;
    }

    constexpr void bind (static_fn_ref fn) noexcept
    {
        get_closure ().bind_static_func (&fn, &self_type::call_static_fn);
    }

    template <class_and_non_functional C>
    constexpr void bind (C& obj, mem_fn_type<C> fn)
    {
        get_closure ().bind_mem_func (&obj, fn);
    }

    consteval static bool is_const () noexcept
    { return false; }

    consteval operator safe_bool () const noexcept
    { return get_closure () != nullptr ? &self_type::_M_storage : nullptr; }

private:
    consteval function (pointer      const  obj,
                        value_type   const  fn ,
                        storage_type const& lsz = storage_type ()) noexcept
    : base_type  (obj, fn)
    , _M_storage (lsz)
    { }

    constexpr return_type call_static_fn (Args... args)
    {
        return (*(get_closure ().static_func ()))(std::forward<Args> (args)...);
    }

    template <structure C>
    constexpr C* object () const noexcept
    {
        return direct_cast<C*> (this);
    }

    constexpr pointer object () const noexcept
    {
        return (fn_ptr<self_type > () == &self_type::call_static_fn) ?
                object<any_object> ()  : get_closure ().object ();
    }

    template <structure C>
    constexpr mem_fn_type<C> fn_ptr () const noexcept
    {
        return direct_cast<mem_fn_type<C>> (get_closure ().fn_ptr ());
    }

    constexpr value_type fn_ptr () const noexcept
    {
        return get_closure ().fn_ptr ();
    }

    constexpr storage_const_ref get_storage () const noexcept
    {
        return _M_storage;
    }

    constexpr storage_ref get_storage () noexcept
    {
        return _M_storage;
    }

    template <class_and_non_functional C>
    constexpr C& get_storage () const noexcept
    {
        return *direct_cast<C*> (_M_storage.data ());
    }

    //! friend functions & classes
    template <fn_sig, fn_sig, size_type>
    friend struct fn_cast_helper;

    template <fn_sig, size_type>
    friend class function;

public:
    //! bytes for storage of capture lambda
    storage_type _M_storage;
};

// ====================================================

template <typename FN, std::size_t SZ = def_capture_size_v>
constexpr bool operator == (function<FN, SZ> const& lh, null_ptr) noexcept
{ return lh._M_closure == nullptr; }

template <typename FN, std::size_t SZ = def_capture_size_v>
constexpr bool operator != (function<FN, SZ> const& lh, null_ptr) noexcept
{ return !(lh == nullptr); }

template <typename FN, std::size_t SZ = def_capture_size_v>
constexpr bool operator == (null_ptr, function<FN, SZ> const& rh) noexcept
{ return nullptr == rh._M_closure; }

template <typename FN, std::size_t SZ = def_capture_size_v>
constexpr bool operator != (null_ptr, function<FN, SZ> const& rh) noexcept
{ return !(nullptr == rh); }

template <typename R, typename... Args, std::size_t SZ>
constexpr auto operator <=> (function<R(Args...), SZ> const& lh, R(& staticFnRef)(Args...)) noexcept
{ return lh._M_closure <=> staticFnRef; }

template <typename R, typename... Args, std::size_t SZ = def_capture_size_v>
constexpr auto operator <=> (R(& staticFnRef)(Args...), function<R(Args...), SZ> const& rh) noexcept
{ return staticFnRef <=> rh._M_closure; }

template <typename R, typename... Args, std::size_t SZ1, std::size_t SZ2>
constexpr bool operator == (function<R(Args...), SZ1> const& lh,
                            function<R(Args...), SZ2> const& rh) noexcept
{ return lh._M_closure == rh._M_closure; }

template <typename R, typename... Args>
constexpr bool operator == (function<R(Args...) const> const& lh,
                            function<R(Args...) const> const& rh) noexcept
{ return lh._M_closure == rh._M_closure; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (function<R(Args...) const> const& lh,
                            function<R(Args...),   SZ> const& rh) noexcept
{ return lh._M_closure == rh._M_closure; }

template <typename R, typename... Args, std::size_t SZ>
constexpr bool operator == (function<R(Args...),   SZ> const& lh,
                            function<R(Args...) const> const& rh) noexcept
{ return lh._M_closure == rh._M_closure; }

template <typename FN, std::size_t SZ1, std::size_t SZ2>
constexpr auto operator <=> (function<FN, SZ1> const& lh, function<FN, SZ2> const& rh)
{ return lh._M_closure <=> rh._M_closure; }

//! ====================================================

//! static function make_fn
template <typename R, typename... Args>
constexpr function<R(Args...)> make_fn (R(& static_fn)(Args...)) noexcept
{
    return function<R(Args...)> (static_fn);
}

//! member function make_fn
template <class_and_non_functional C, typename R, typename... Args>
constexpr function<R(Args...)> make_fn (C& obj, R(C::* mem_fn)(Args...)) noexcept
{
    return function<R(Args...)> (obj, mem_fn);
}

//! const member function make_fn
template <class_and_non_functional C, typename R, typename... Args>
constexpr function<R(Args...) const> make_fn (C& obj, R(C::* const_mem_fn)(Args...) const) noexcept
{
    return function<R(Args...) const> (obj, const_mem_fn);
}

//! capture lambda make_fn
template <lambda_capture Callable,
          typename...    Args    ,
          typename       R = callable_return_t<Callable, Args...>,
          std::size_t    N = max_capture_size_v<sizeof (std::decay_t<Callable>)>
          >
constexpr function<R(Args...), N> make_fn (Callable&& callable, Callable* ptr = nullptr) noexcept
{
    return function<R(Args...), N> (std::move (callable), ptr);
}

//! non-capture lambda make_fn
template <lambda_non_capture Callable,
          typename... Args,
          typename R = callable_return_t<Callable, Args...>
          >
constexpr function<R(Args...)> make_fn (Callable&& callable, Callable* ptr = nullptr) noexcept
{
    return function<R(Args...)> (std::move (callable), ptr);
}

//! callable object make_fn
template <callable_class C,
          typename... Args,
          typename R = callable_return_t<C, Args...>,
          std::enable_if_t<!functional<std::decay_t<C>>, int> = 0
          >
constexpr function<R(Args...)> make_fn (C& obj) noexcept
{
    return function<R(Args...)> (obj);
}

//! ====================================================
//! function signature cast helpers
//! ====================================================

template <fn_sig, fn_sig, std::size_t = def_capture_size_v>
struct fn_cast_helper;

// ====================================================

template <typename R1, typename... Args1, typename R2, typename... Args2, std::size_t N>
struct fn_cast_helper <R1(Args1...), R2(Args2...), N>
{
    typedef decltype (N)    size_type ;
    typedef size_type const const_size;

    using type1    = R1(Args1...);
    using type2    = R2(Args2...);
    using fn_out_t = function<type1, max_capture_size_v<N>>;
    using fn_in_t  = function<type2, max_capture_size_v<N>>;

    consteval static fn_out_t fn_cast (fn_in_t const& fn_in) noexcept
    {
        return fn_out_t (fn_in.object (),
                         direct_cast<typename fn_out_t::value_type> (fn_in.fn_ptr ()),
                         fn_in.get_storage ());
    }
};

template <typename R1, typename... Args1, typename R2, typename... Args2>
struct fn_cast_helper <R1(Args1...) const, R2(Args2...) const>
{
    using type1    = R1(Args1...) const;
    using type2    = R2(Args2...) const;
    using fn_out_t = function<type1>   ;
    using fn_in_t  = function<type2>   ;

    consteval static fn_out_t fn_cast (fn_in_t const& fn_in) noexcept
    {
        return fn_out_t (fn_in.object (), direct_cast<typename fn_out_t::value_type> (fn_in.fn_ptr ()));
    }
};

template <typename R1, typename... Args1, typename R2, typename... Args2, std::size_t N>
struct fn_cast_helper <R1(Args1...) const, R2(Args2...), N>
{
    typedef decltype (N)    size_type ;
    typedef size_type const const_size;

    using type1    = R1(Args1...) const;
    using type2    = R2(Args2...)      ;
    using fn_out_t = function<type1>   ;
    using fn_in_t  = function<type2, max_capture_size_v<N>>;

    consteval static fn_out_t fn_cast (fn_in_t const& fn_in) noexcept
    {
        return fn_out_t (fn_in.object (), direct_cast<typename fn_out_t::value_type> (fn_in.fn_ptr ()));
    }
};

template <typename R1, typename... Args1, typename R2, typename... Args2, std::size_t N>
struct fn_cast_helper <R1(Args1...), R2(Args2...) const, N>
{
    using type1    = R1(Args1...)      ;
    using type2    = R2(Args2...) const;
    using fn_out_t = function<type1, max_capture_size_v<N>>;
    using fn_in_t  = function<type2>   ;

    consteval static fn_out_t fn_cast (fn_in_t const& fn_in) noexcept
    {
        return fn_out_t (fn_in.object (), direct_cast<typename fn_out_t::value_type> (fn_in.fn_ptr ()));
    }
};

// ====================================================

template <fn_sig Out, fn_sig In, std::size_t N>
consteval
function<Out, is_const_fn_sig_v<Out> ? def_capture_size_v : max_capture_size_v<N>>
fn_cast (function<In, N> const& fn_in) noexcept
{
    constexpr const decltype (N) SZ = is_const_fn_sig_v <Out> ?
                                      def_capture_size_v      :
                                      max_capture_size_v< N > ;

    return function<Out, SZ> (fn_cast_helper<Out, In, N>::fn_cast (fn_in));
}

// ====================================================

} //! namespace cppual

// ====================================================

namespace std {

// ====================================================

template <typename R, typename... Args, std::size_t N>
struct hash <cppual::function<R(Args...), N>>
{
    typedef decltype (N)                        size_type      ;
    typedef cppual::function<R(Args...)> const& const_reference;

    constexpr size_type operator () (const_reference fn) const noexcept
    {
        return direct_cast<size_type> (fn.object ()) ^
               direct_cast<size_type> (fn.fn_ptr ()) ;
    }
};

template <typename R, typename... Args, std::size_t N>
struct hash <cppual::function<R(Args...) const, N>>
{
    typedef decltype (N)                              size_type      ;
    typedef cppual::function<R(Args...) const> const& const_reference;

    constexpr size_type operator () (const_reference fn) const noexcept
    {
        return direct_cast<size_type> (fn.object ()) ^
               direct_cast<size_type> (fn.fn_ptr ()) ;
    }
};

// ====================================================

template <switch_value T, typename R, typename... Args, size_t SZ = def_capture_size_v>
constexpr
pair<T, cppual::function<R(Args...), max_capture_size_v<SZ>>>
make_pair (T const _case, cppual::function<R(Args...), max_capture_size_v<SZ>> const& fn) noexcept
{
    return pair<T, cppual::function<R(Args...), max_capture_size_v<SZ>>> (_case, fn);
}

template <switch_value T, typename R, typename... Args>
constexpr
pair<T, cppual::function<R(Args...) const>>
make_pair (T const _case, cppual::function<R(Args...) const> const& fn) noexcept
{
    return pair<T, cppual::function<void(Args...) const>> (_case, fn);
}

// ====================================================

} // namespace std

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_