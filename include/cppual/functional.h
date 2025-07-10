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
#include <cppual/types.h>
#include <cppual/concepts>
#include <cppual/exception>
#include <cppual/meta_functional>
#include <cppual/memory/allocator.h>

#include <cstring>
#include <utility>
#include <tuple>
//#include <array>

#include <cassert>

namespace cppual {

// ====================================================

//! import needed concepts
using memory::allocator_t;

// ====================================================

class any_object;

template <typename>
struct function_traits;

template <typename, std::size_t>
class function;

// ====================================================

template <typename R, non_void_t... Args>
using any_static_fn = any_fn_type<void, R(Args...)>::type;

template <typename R, non_void_t... Args>
using any_member_fn = any_fn_type<any_object, R(Args...)>::type;

template <typename R, non_void_t... Args>
using any_const_member_fn = any_fn_type<any_object, R(Args...) const>::type;

// ====================================================

template <typename R, non_void_t... Args>
struct function_traits <R(Args...)>
{
    typedef std::size_t                 size_type  ;
    typedef size_type const             const_size ;
    typedef R                           result_type;

    using type = R(Args...);

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

template <typename R, non_void_t... Args>
struct function_traits <R(*)(Args...)> : function_traits<R(Args...)>
{
    using function_ref = R(&)(Args...);
    using function_ptr = R(*)(Args...);
};

template <class_t C, typename R, non_void_t... Args>
struct function_traits <R(C::*)(Args...)> : function_traits<R(Args...)>
{
    typedef remove_cref_t<C> object_type;
    using   function_ptr = R(C::*)(Args...)   ;
};

template <class_t C, typename R, non_void_t... Args>
struct function_traits <R(C::*)(Args...) const> : function_traits<R(Args...)>
{
    typedef remove_cref_t<C> object_type   ;
    using   function_ptr = R(C::*)(Args...) const;
};

template <auto F,
          typename = std::enable_if_t<std::is_function_v<std::remove_pointer_t<decltype (F)>> ||
                                      std::is_member_function_pointer_v<decltype (F)>>
          >
struct function_ptr_traits : function_traits<decltype (F)>
{
    inline constexpr static decltype (F) const value = F;
};

/// deduce callable signature
template <typename T>
using function_traits_t = function_traits<T>::function_type;

/// callable signature pointer -> value
template <auto F>
inline constexpr cbool function_ptr_traits_v = function_ptr_traits<F>::value;

// ====================================================

//! helper for constructing member function pointers
struct mem_fn_helper
{
    template <class_t C, typename R, typename... Args>
    using member_pair_t = std::pair<C*, R(C::*)(Args...)>;

    template <class_t D, class_t C, typename R, typename... Args>
    constexpr static member_pair_t<D, R, Args...> make (C* obj, R(C::* fn)(Args...)) noexcept
    {
        return member_pair_t<D, R, Args...> (obj_ptr_cast<D> (obj), mem_fn_cast<D> (fn));
    }

    template <class_t D, class_t C, typename R, typename... Args>
    constexpr static member_pair_t<D, R, Args...> make (C* obj, R(C::* fn)(Args...) const) noexcept
    {
        return member_pair_t<D, R, Args...> (obj_ptr_cast<D> (obj), mem_fn_cast<D> (fn));
    }
};

// ====================================================

//! closure class handling the function bindings
template <typename R, non_void_t... Args>
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
    using member_pair_t = std::pair<C*, R(C::*)(Args...)>;

private:
    constexpr closure () noexcept = default;

    //! make class trivially copyable & movable
    constexpr closure (self_type&&) noexcept = default;
    constexpr closure (self_type const&) noexcept = default;
    constexpr self_type& operator = (self_type&&) noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr explicit closure (std::nullptr_t) noexcept
    : _M_pObj (),
      _M_fn   ()
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

    //! static helper for member function binding
    template <class_t C>
    constexpr
    static
    member_pair_t<any_object> make_member_fn (C* obj, R(C::* fn)(Args...)) noexcept
    {
        return mem_fn_helper::make<any_object> (obj, fn);
    }

    template <class_t C>
    constexpr
    static
    member_pair_t<any_object> make_member_fn (C* obj, R(C::* fn)(Args...) const) noexcept
    {
        return mem_fn_helper::make<any_object> (obj, fn);
    }

    //! static helper for static function binding
    template <class_t C>
    constexpr
    static
    member_pair_t<any_object> make_static_fn (R(* fn)(Args...), R(C::* invoker)(Args...)) noexcept
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

    constexpr bool operator == (std::nullptr_t) const noexcept
    { return _M_fn == nullptr || _M_pObj == nullptr; }

    constexpr bool operator == (self_type const& mRhs) const noexcept
    { return _M_pObj == mRhs._M_pObj && _M_fn == mRhs._M_fn; }

    constexpr bool operator == (static_fn_type mPtr) const noexcept
    {
        return mPtr == nullptr ?
              *this == nullptr :
               mPtr == static_func ();
    }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !(*this == nullptr); }

    constexpr bool operator != (self_type const& mRhs) const noexcept
    { return !(*this == mRhs); }

    constexpr bool operator != (static_fn_type mPtr) const noexcept
    { return !(*this == mPtr); }

    constexpr bool operator < (self_type const& mRhs) const
    {
        return _M_pObj != mRhs._M_pObj ?
                               _M_pObj < mRhs._M_pObj :
                               std::memcmp (&_M_fn, &mRhs._M_fn, sizeof (_M_fn)) < 0;
    }

    constexpr bool operator > (self_type const& mRhs) const
    { return  *this > mRhs; }

    constexpr static_fn_type static_func () const noexcept
    { return direct_cast<static_fn_type> (_M_pObj); }

public:
    constexpr pointer object () const noexcept
    { return _M_pObj; }

    constexpr value_type function_ptr () const noexcept
    { return _M_fn; }

    template <typename, size_type>
    friend class cppual::function;

    template <typename>
    friend struct ::std::hash;

public:
    pointer    _M_pObj { };
    value_type _M_fn   { };
};

// ====================================================

//! reimplementation of impossibly fast delegates
template <std::size_t BYTES, typename R, non_void_t... Args>
class SHARED_API function <R(Args...), BYTES> : public function_traits<R(Args...)>
{
public:
    typedef function<R(Args...), BYTES> self_type        ;
    typedef function_traits<R(Args...)> base_type        ;
    typedef any_member_fn  <R, Args...> mem_fn_pointer   ;
    typedef any_static_fn  <R, Args...> static_fn_pointer;
    typedef closure        <R, Args...> closure_type     ;
    typedef std::array<uchar, BYTES>    storage_type     ;
    typedef std::size_t                 size_type        ;
    typedef size_type const             const_size       ;
    typedef closure_type::pointer       pointer          ;
    typedef closure_type::value_type    value_type       ;
    typedef closure_type* self_type::*  safe_bool        ;

    using static_fn_ref =  R(&)(Args...);

    template <size_type SZ>
    using self_type_t = function<R(Args...), lambda_calc_size (SZ)>;

    template <class_t C>
    using mem_fn_type = any_fn_type<C, R(Args...)>::type;

    template <class_t C>
    using const_mem_fn_type = any_fn_type<C, R(Args...) const>::type;

    template <class_t C>
    using mem_fn_pair = std::pair<C&, mem_fn_type<C>>;

    template <class_t C>
    using const_mem_fn_pair = std::pair<C&, const_mem_fn_type<C>>;

    inline constexpr static const_size lambda_size = lambda_calc_size (BYTES);

    constexpr function () noexcept = default;

    //! make function class trivially copyable
    constexpr function (self_type&&) noexcept = default;
    constexpr function (self_type const&) noexcept = default;
    constexpr self_type& operator = (self_type&&) noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr explicit function (std::nullptr_t) noexcept
    : _M_closure (nullptr)
    { }

    template <size_type SZ>
    constexpr function (self_type_t<SZ> const& rh_obj) noexcept
    : _M_closure (rh_obj._M_closure)
    {
        static_assert (lambda_size >= lambda_calc_size (SZ), "function storage size mismatch!");

        std::copy (rh_obj._M_storage.begin (), rh_obj._M_storage.end (), _M_storage.begin ());
    }

    template <size_type SZ>
    constexpr function (self_type_t<SZ>&& rh_obj) noexcept
    : _M_closure (std::move (rh_obj._M_closure))
    {
        static_assert (lambda_size >= lambda_calc_size (SZ), "function storage size mismatch!");

        std::move (rh_obj._M_storage.begin (), rh_obj._M_storage.end (), _M_storage.begin ());
    }

    //! static function constructor
    constexpr function (static_fn_ref fn)
    : _M_closure (closure_type::make_static_fn (&fn, &self_type::call_static_fn))
    { }

    //! member function constructor
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr function (C& obj, mem_fn_type<C> mem_fn)
    : _M_closure (closure_type::make_member_fn (&obj, mem_fn))
    { }

    //! const member function constructor
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr function (C& obj, const_mem_fn_type<C> mem_fn)
    : _M_closure (closure_type::make_member_fn (&obj, mem_fn))
    { }

    //! capture lambda constructor
    template <lambda_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaCaptureType<Callable>
              >
    constexpr function (Callable&& lambda, LambdaCaptureType<Callable>* = nullptr)
    {
        // block function types for lambda capture constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("lambda capture constructor cannot be used with function types!");
        }
        else
        {
            static_assert (sizeof (C) <= lambda_size,
                           "Capture lambda too large for static storage! "
                           "DON'T value copy captured objects bigger than 16 bytes! "
                           "Use references instead!");

            ::new (&get_storage<C> ()) C (std::move (lambda));

            bind (get_storage<C> (), callable_operator_v<Callable>);
        }
    }

    //! non-capture lambda constructor
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaNonCaptureType<Callable>
              >
    constexpr function (Callable&& callable, LambdaNonCaptureType<Callable>* = nullptr)
    {
        // block function types for callable class constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("callable class constructor cannot be used with function types!");
        }
        else
        {
            bind (callable, callable_operator_v<Callable>);
        }
    }

    //! TODO: non-capture lambda + callable class constructor
    //! callable class constructor
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr function (C& obj)
    {
        // block function types for callable class constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("callable class constructor cannot be used with function types!");
        }
        else
        {
            bind (obj, callable_operator_v<C>);
        }
    }

    template <size_type SZ>
    constexpr self_type& operator = (self_type_t<SZ> const& rh_obj) noexcept
    {
        static_assert (lambda_size >= lambda_calc_size (SZ), "function storage size mismatch!");

        if (this == &rh_obj) return *this;

        _M_closure = rh_obj._M_closure;
        std::copy (rh_obj._M_storage.begin (), rh_obj._M_storage.end (), _M_storage.begin ());

        return *this;
    }

    template <size_type SZ>
    constexpr self_type& operator = (self_type_t<SZ>&& rh_obj) noexcept
    {
        static_assert (lambda_size >= lambda_calc_size (SZ), "function storage size mismatch!");

        if (this == &rh_obj) return *this;

        _M_closure = std::move (rh_obj._M_closure);
        std::move (rh_obj._M_storage.begin (), rh_obj._M_storage.end (), _M_storage.begin ());

        return *this;
    }

    constexpr self_type& operator = (std::nullptr_t) noexcept
    {
        _M_closure = nullptr;
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

    //! const member function assignment operator
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr self_type& operator = (const_mem_fn_pair<C> const& pair)
    {
        bind (pair.first, pair.second);
        return *this;
    }

    //! capture lambda assignment operator
    template <lambda_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaCaptureType<Callable>
              >
    constexpr self_type& operator = (Callable&& callable)
    {
        // block function types for lambda capture constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("lambda capture constructor cannot be used with function types!");
        }
        else
        {
            static_assert (sizeof (C) <= lambda_size,
                          "Capture lambda too large for static storage! "
                          "DON'T value copy captured objects bigger than 16 bytes! "
                          "Use references instead!");

            new (&get_storage<C> ()) C (std::move (callable));

            bind (get_storage<C> (), callable_operator_v<Callable>);
        }

        return *this;
    }

    //! non-capture lambda assignment operator
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
              typename   = LambdaNonCaptureType<Callable>
              >
    constexpr self_type& operator = (Callable&& callable)
    {
        // block function types for callable class constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("callable class assignment cannot be used with function types!");
        }
        else
        {
            bind (callable, callable_operator_v<Callable>);
        }

        return *this;
    }

    //! TODO: non-capture lambda + callable class assignment operator
    //! callable object assignment operator
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr self_type& operator = (C& obj)
    {
        // block function types for callable class constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("callable class assignment cannot be used with function types!");
        }
        else
        {
            bind (obj, callable_operator_v<C>);
        }

        return *this;
    }

    constexpr R operator () (Args... args) const
    {
        return (object ()->*function_ptr ())(std::forward<Args> (args)...);
    }

    consteval static size_type arity () noexcept
    {
        return base_type::arity;
    }

    constexpr explicit operator safe_bool () const noexcept
    {
        return _M_closure != nullptr ? &self_type::_M_closure : nullptr;
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    {
        return _M_closure == nullptr;
    }

    constexpr bool operator == (self_type const& rhObj) const noexcept
    {
        return _M_closure == rhObj._M_closure && _M_storage == rhObj._M_storage;
    }

    constexpr bool operator == (static_fn_ref staticFnRef) const noexcept
    { return _M_closure == &staticFnRef; }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !operator == (nullptr); }

    constexpr bool operator != (self_type const& rhObj) const noexcept
    { return !operator == (rhObj); }

    constexpr bool operator != (static_fn_ref staticFnRef) const noexcept
    { return !(*this == staticFnRef); }

    constexpr bool operator  < (self_type const& rhObj) const
    { return _M_closure < rhObj._M_closure; }

    constexpr bool operator  > (self_type const& rhObj) const
    { return _M_closure > rhObj._M_closure; }

private:
    template <class_t C, typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
    constexpr C& get_storage () const
    {
        return *direct_cast<C*> (&_M_storage[0]);
    }

    constexpr R call_static_fn (Args... args)
    {
        return (*(_M_closure.static_func ()))(std::forward<Args> (args)...);
    }

    constexpr pointer object () const noexcept
    {
        return (function_ptr<self_type> () == &self_type::call_static_fn) ?
                object<any_object> () : _M_closure.object ();
    }

    template <class_t C>
    constexpr C* object () const noexcept
    {
        return direct_cast<C*> (this);
    }

    constexpr value_type function_ptr () const noexcept
    {
        return _M_closure.function_ptr ();
    }

    template <class_t C>
    constexpr auto function_ptr () const noexcept
    {
        return direct_cast<mem_fn_type<C>> (_M_closure.function_ptr ());
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

    constexpr function (pointer      const  obj,
                        value_type   const  fn ,
                        storage_type const& lsz = { }) noexcept
    : _M_closure (obj, fn)
    , _M_storage (lsz)
    { }

    template <typename, size_type>
    friend class function;

    template <typename S1, std::size_t N, typename S2, std::size_t M>
    friend constexpr function<S1, lambda_calc_size (N)> fn_cast (function<S2, M> const& fn) noexcept;

    template <typename>
    friend struct ::std::hash;

public:
    closure_type _M_closure;

    //! bytes for storage of capture lambda
    alignas (std::max_align_t) storage_type _M_storage;
};

// ====================================================

//! static function make_fn
template <typename R, non_void_t... Args>
constexpr function<R(Args...)> make_fn (R (& static_fn)(Args...))
{
    return function<R(Args...)> (static_fn);
}

//! member function make_fn
template <class_t  C        ,
          typename R        ,
          non_void_t... Args,
          typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* mem_fn)(Args...))
{
    return function<R(Args...)> (obj, mem_fn);
}

//! const member function make_fn
template <class_t  C        ,
          typename R        ,
          non_void_t... Args,
          typename = std::enable_if_t<!is_functional_v<std::decay_t<C>>>>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* const_mem_fn)(Args...) const)
{
    return function<R(Args...)> (obj, const_mem_fn);
}

//! capture lambda make_fn
template <lambda_capture_t Callable,
          typename... Args         ,
          typename    R = callable_return_t<Callable, Args...>,
          typename    C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
          std::size_t N = lambda_calc_size (sizeof (C)),
          typename      = LambdaCaptureType<Callable>
          >
constexpr function<R(Args...), N> make_fn (Callable&& callable, LambdaCaptureType<Callable>* ptr = nullptr)
{
    return function<R(Args...), N> (std::move (callable), ptr);
}

//! non-capture lambda make_fn
template <lambda_non_capture_t Callable,
          non_void_t... Args,
          typename      R = callable_return_t<Callable, Args...>,
          typename      C = std::enable_if_t<!is_functional_v<std::decay_t<Callable>>, std::decay_t<Callable>>,
          typename        = LambdaNonCaptureType<Callable>
          >
constexpr function<R(Args...)> make_fn (Callable&& callable, LambdaNonCaptureType<Callable>* ptr = nullptr)
{
    return function<R(Args...)> (std::move (callable), ptr);
}

//! callable object make_fn
template <callable_class_t C,
          non_void_t... Args,
          typename         R = callable_return_t<C, Args...>,
          typename           = std::enable_if_t<!is_functional_v<std::decay_t<C>>>
          >
constexpr function<R(Args...)> make_fn (C& obj)
{
    return function<R(Args...)> (obj);
}

// ====================================================

template <typename  Out,
          std::size_t N = lambda_calc_size (),
          typename   In,
          std::size_t M
          >
constexpr function <Out, lambda_calc_size (N)> fn_cast (function<In, M> const& fn_in) noexcept
{
    static_assert (N >= M, "function storage size mismatch!");
    constexpr const std::size_t SZ = lambda_calc_size (N);

    return function <Out, SZ> (fn_in._M_closure.object (),
                               direct_cast<typename function<Out, SZ>::value_type>
                              (fn_in._M_closure.function_ptr ()),
                               fn_in._M_storage);
}

// ====================================================

} // namespace cppual

// ====================================================

namespace std {

// ====================================================

using cppual::lambda_calc_size;

// ====================================================

template <typename R, typename... Args, std::size_t B>
struct hash <cppual::function<R(Args...), B>>
{
    typedef std::size_t                  size_type ;
    typedef cppual::function<R(Args...)> value_type;

    constexpr size_type operator () (value_type& fn) const noexcept
    {
        return cppual::direct_cast<size_type> (fn._M_closure._M_pObj) ^
               cppual::direct_cast<size_type> (fn._M_closure._M_fn  ) ;
    }
};

// ====================================================

template <size_t N = lambda_calc_size (), cppual::switch_value_t T, typename... Args, typename R>
constexpr auto make_pair (T const _case, Args... fn_args, R* = cppual::void_ptr ()) noexcept
{
    using  fn_type = cppual::function<R(Args...), N>;
    return pair<T, fn_type> (_case, cppual::make_fn (forward<Args> (fn_args)...));
}

template <size_t N = lambda_calc_size (), cppual::switch_value_t T, typename... Args>
constexpr auto make_pair (T const _case, Args... fn_args) noexcept
{
    using  fn_type = cppual::function<void(Args...), N>;
    return pair<T, fn_type> (_case, cppual::make_fn (forward<Args> (fn_args)...));
}

// ====================================================

} // namespace std

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
