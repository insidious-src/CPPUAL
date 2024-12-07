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

#include <cppual/cast.h>
#include <cppual/types.h>
#include <cppual/concept/concepts.h>
#include <cppual/memory/allocator.h>
#include <cppual/exceptions/functional_exception.h>

#include <functional>
#include <cstring>
#include <utility>
#include <tuple>
#include <array>
#include <bit>

#include <cassert>

namespace cppual {

// ====================================================

//! import needed concepts
using memory::allocator_t;

//! default max array size for function's capture lambda storage
inline constexpr std::size_t const LAMBDA_MAX_SIZE = 64;

// ====================================================

template <callable_object_t C, typename... Args>
using callable_return_t = std::invoke_result_t<decltype (&std::decay_t<C>::operator ()), Args...>;

template <callable_object_t C>
inline constexpr auto callable_operator_v = &std::decay_t<C>::operator ();

// ====================================================

class any_object;

template <typename... Args>
using any_static_fn = void(*)(Args...);

template <typename... Args>
using any_member_fn = void(any_object::*)(Args...);

template <typename... Args>
using any_const_member_fn = void(any_object::*)(Args...) const;

template <typename, std::size_t = LAMBDA_MAX_SIZE>
struct function_traits;

template <typename, std::size_t = LAMBDA_MAX_SIZE>
class function;

template <typename R, typename... Args>
using static_fn = R(*)(Args...);

template <typename R, typename... Args>
using member_fn = R(any_object::*)(Args...);

template <typename R, typename... Args>
using const_member_fn = R(any_object::*)(Args...) const;

// ====================================================

template <std::size_t BYTES, typename R, typename... Args>
struct function_traits <R(Args...), BYTES>
{
    typedef std::size_t                 size_type    ;
    typedef size_type const             const_size   ;
    typedef R                           result_type  ;
    typedef function<R(Args...), BYTES> function_type;

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

template <typename R, typename... Args>
struct function_traits <R(*)(Args...)> : function_traits<R(Args...)>
{
    using function_ref = R(&)(Args...);
    using function_ptr = R(*)(Args...);
};

template <class_t C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...)> : function_traits<R(Args...)>
{
    typedef std::remove_cvref_t<C> object_type;
    using   function_ptr = R(C::*)(Args...)   ;
};

template <class_t C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...) const> : function_traits<R(Args...)>
{
    typedef std::remove_cvref_t<C> object_type   ;
    using   function_ptr = R(C::*)(Args...) const;
};

template <auto F,
          typename = std::enable_if_t<std::is_function_v<std::remove_pointer_t<decltype (F)>> ||
                                      std::is_member_function_pointer_v<decltype (F)>>
          >
struct function_ptr_traits : function_traits<decltype (F)>
{
    inline static constexpr auto const value = F;

    consteval static auto fn_signature () noexcept -> decltype (F)
    {
        return value;
    }
};

/// deduce callable signature
template <typename T>
using function_traits_t = function_traits<T>::function_type;

/// callable signature pointer -> value
template <auto F>
using function_ptr_traits_v = function_ptr_traits<F>::value;

// ====================================================
// Delegate Concept
// ====================================================

template <typename F>
struct is_functional_helper : public std::false_type
{ typedef F type; };

template <non_void_t T, std::size_t BYTES>
struct is_functional_helper <function<T, BYTES>> : public std::true_type
{ typedef function<T, BYTES> type; };

template <non_void_t T>
struct is_functional_helper <std::function<T>> : public std::true_type
{ typedef std::function<T> type; };

template <typename F>
struct is_functional : public is_functional_helper<F>
{ };

template <typename F>
using is_functional_t = is_functional<F>::type;

/// is functional -> value
template <typename F>
inline constexpr bool const is_functional_v = is_functional<F>::value;

template <typename F>
using FunctionalType = std::enable_if_t<is_functional_v<F>, F>;

/// delegate concept
template <typename F>
concept functional_t = is_functional_v<F>;

// ====================================================

template <std::size_t N>
struct simplify_mem_fn
{
    template <class_t C, typename R, typename... Args>
    constexpr
        static
        any_object* convert (C*, R(C::*)(Args...), member_fn<R, Args...>&) noexcept
    {
        static_assert (N - 100, "unsupported member function pointer on this compiler!");
        return nullptr;
    }
};

template <>
struct simplify_mem_fn <sizeof (any_member_fn<>)>
{
    template <class_t C, typename R, typename... Args>
    constexpr
        static
        any_object* convert (C* pObj,
                R(C::* fnToBind)(Args...),
                member_fn<R, Args...>& mFuncBound) noexcept
    {
        mFuncBound = safe_member_cast<member_fn<R, Args...>> (fnToBind);
        return static_cast<any_object*> (static_cast<void*> (pObj));
    }

    template <class_t C, typename R, typename... Args>
    constexpr
    static
    any_object* convert (C* pObj,
                         R(C::* fnToBind)(Args...),
                         const_member_fn<R, Args...>& mFuncBound) noexcept
    {
        mFuncBound = safe_member_cast<const_member_fn<R, Args...>> (fnToBind);
        return static_cast<any_object*> (static_cast<void*> (pObj));
    }

    template <class_t C, typename R, typename... Args>
    constexpr
        static
    any_object* convert (C* pObj,
                         R(C::* fnToBind)(Args...) const,
                         const_member_fn<R, Args...>& mFuncBound) noexcept
    {
        mFuncBound = safe_member_cast<const_member_fn<R, Args...>> (fnToBind);
        return static_cast<any_object*> (static_cast<void*> (pObj));
    }

    template <class_t C, typename R, typename... Args>
    constexpr
    static
    any_object* convert (C* pObj,
                         R(C::* fnToBind)(Args...) const,
                         member_fn<R, Args...>& mFuncBound) noexcept
    {
        mFuncBound = safe_member_cast<member_fn<R, Args...>> (fnToBind);
        return static_cast<any_object*> (static_cast<void*> (pObj));
    }
};

// ====================================================

template <typename R, typename... Args>
class closure
{
public:
    typedef closure<R, Args...>   self_type ;
    typedef std::size_t           size_type ;
    typedef any_object*           pointer   ;
    typedef member_fn<R, Args...> value_type;

    typedef R(*          static_fn_type)(Args...);
    typedef R(any_object::* mem_fn_type)(Args...);

    constexpr closure () noexcept = default;

    constexpr explicit closure (std::nullptr_t) noexcept
    : _M_pObj (),
      _M_fn   ()
    { }

    constexpr closure (self_type const& mClosure) noexcept
    : _M_pObj { mClosure._M_pObj },
      _M_fn   { mClosure._M_fn   }
    { }

    constexpr closure (self_type&& mClosure) noexcept
    : _M_pObj { std::move (mClosure._M_pObj) },
      _M_fn   { std::move (mClosure._M_fn  ) }
    { mClosure = nullptr; }

    constexpr self_type& operator = (std::nullptr_t) noexcept
    {
        _M_pObj = nullptr;
        _M_fn   = nullptr;
        return  * this   ;
    }

    constexpr self_type& operator = (self_type const& mRhs) noexcept
    {
        if (this == &mRhs) return *this;

        _M_pObj = mRhs._M_pObj;
        _M_fn   = mRhs._M_fn  ;
        return  * this        ;
    }

    constexpr self_type& operator = (self_type&& mRhs) noexcept
    {
        if (this == &mRhs) return *this;

        _M_pObj = mRhs._M_pObj;
        _M_fn   = mRhs._M_fn  ;

        mRhs   = nullptr;
        return * this   ;
    }

    template <class_t C>
    constexpr void bind_mem_func (C* pObj, R(C::* fnToBind)(Args...)) noexcept
    {
        _M_pObj = simplify_mem_fn<sizeof (fnToBind)>::convert (pObj, fnToBind, _M_fn);
    }

    template <class_t C>
    constexpr void bind_mem_func (C* pObj, R(C::* fnToBind)(Args...) const) noexcept
    {
        _M_pObj = simplify_mem_fn<sizeof (fnToBind)>::convert (pObj, fnToBind, _M_fn);
    }

    template <class_t DerivedClass, typename ParentInvokerSig>
    constexpr void bind_static_func (DerivedClass*    mPtrParent,
                                     ParentInvokerSig mStaticFuncInvoker,
                                     static_fn_type   fnToBind) noexcept
    {
        if (fnToBind == nullptr) _M_fn = nullptr;
        else bind_mem_func (mPtrParent, mStaticFuncInvoker);

        _M_pObj = std::bit_cast<pointer> (fnToBind);
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

    inline bool operator < (self_type const& mRhs) const
    {
        return _M_pObj != mRhs._M_pObj ?
                               _M_pObj < mRhs._M_pObj :
                               std::memcmp (&_M_fn, &mRhs._M_fn, sizeof (_M_fn)) < 0;
    }

    inline bool operator > (self_type const& mRhs) const
    { return  *this > mRhs; }

    constexpr pointer object () const noexcept
    { return _M_pObj; }

    constexpr mem_fn_type function_ptr () const noexcept
    { return _M_fn; }

    constexpr static_fn_type static_func () const noexcept
    { return std::bit_cast<static_fn_type> (this); }

    template <typename, std::size_t>
    friend class function;

    template <typename>
    friend struct ::std::hash;

public:
    pointer    _M_pObj { };
    value_type _M_fn   { };
};

// ====================================================

//! reimplementation of impossibly fast delegates
template <std::size_t BYTES, typename R, typename... Args>
class SHARED_API function <R(Args...), BYTES> : public function_traits<R(Args...), BYTES>
{
public:
    typedef function <R(Args...), BYTES> self_type     ;
    typedef member_fn<R, Args...>        member_fn_type;
    typedef static_fn<R, Args...>        static_fn_type;
    typedef closure  <R, Args...>        closure_type  ;
    typedef std::array<uchar, BYTES>     storage_type  ;
    typedef std::size_t                  size_type     ;
    typedef size_type const              const_size    ;
    typedef closure_type::value_type     value_type    ;
    typedef closure_type::pointer        pointer       ;
    typedef closure_type* self_type::*   safe_bool     ;

    template <size_type N, typename S = R(Args...)>
    using self_type_t = function<S, N>;

    template <class_t C>
    using mem_fn_type = R(C::*)(Args...);

    template <class_t C>
    using const_mem_fn_type = R(C::*)(Args...) const;

    template <class_t C>
    using mem_fn_pair = std::pair<C&, mem_fn_type<C>>;

    template <class_t C>
    using const_mem_fn_pair = std::pair<C&, const_mem_fn_type<C>>;

    typedef R(& static_fn_ref)(Args...);

    inline constexpr static const_size storage_size = BYTES;

    constexpr function () noexcept = default;

    /// capture lambda constructor
    template <lambda_capture_t Callable,
             typename C = std::enable_if_t<!is_functional_v<Callable>, std::decay_t<Callable>>
              >
    constexpr function (Callable&& callable, LambdaCaptureType<Callable>* = nullptr)
    {
        // Block function types for lambda capture constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("lambda capture constructor cannot be used with function types!");
        }
        else
        {
            static_assert (sizeof (C) <= storage_size,
                           "Capture lambda too large for static storage! "
                           "DON'T value copy captured objects bigger than 16 bytes! "
                           "Use references instead!");

            ::new (&get_storage<C> ()) C (std::move (callable));

            bind (get_storage<C> (), callable_operator_v<C>);
        }
    }

    /// non-capture lambda constructor
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<Callable>, std::decay_t<Callable>>
              >
    constexpr function (Callable&& callable, LambdaNonCaptureType<Callable>* = nullptr)
    {
        bind (callable, callable_operator_v<C>);
    }

    /// static function constructor
    constexpr function (static_fn_ref fnToBind)
    {
        bind (&fnToBind);
    }

    /// member function constructor
    template <class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr function (C& obj, mem_fn_type<C> fnToBind)
    {
        bind (obj, fnToBind);
    }

    /// const member function constructor
    template <class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr function (C& obj, const_mem_fn_type<C> fnToBind)
    {
        bind (obj, fnToBind);
    }

    /// callable class constructor
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr function (C& obj)
    {
        // Block function types for callable class constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("callable class constructor cannot be used with function types!");
        }
        else
        {
            bind (obj, callable_operator_v<C>);
        }
    }

    // /// capture lambda + storage move constructor
    // template <lambda_capture_t Callable,
    //           typename = std::enable_if_t<!is_functional_v<Callable>>
    //           >
    // constexpr function (Callable&& obj, storage_type&& storage)
    // : _M_storage (std::move (storage))
    // {
    //     typedef std::decay_t<std::remove_cvref_t<decltype (obj)>> C;

    //     _M_closure.bind_mem_func (static_cast<C*> (_M_storage.get ()), callable_operator_v<C>);
    // }

    // /// capture lambda + storage copy constructor
    // template <lambda_capture_t Callable,
    //           typename = std::enable_if_t<!is_functional_v<Callable>>
    //           >
    // constexpr function (Callable&& obj, storage_type const& storage)
    // : _M_storage (storage)
    // {
    //     typedef std::decay_t<std::remove_cvref_t<decltype (obj)>> C;

    //     _M_closure.bind_mem_func (static_cast<C*> (_M_storage.get ()), callable_operator_v<C>);
    // }

    constexpr explicit function (std::nullptr_t) noexcept
    : _M_closure ()
    { }

    template <size_type N>
    constexpr function (self_type_t<N> const& rh_obj) noexcept
    : _M_closure (rh_obj._M_closure),
      _M_storage (rh_obj._M_storage)
    {
        if constexpr (!rh_obj._M_storage.empty ())
        {
            static_assert (storage_size <= sizeof (rh_obj._M_storage.size ()),
                          "function storage size mismatch!");
        }
    }

    template <size_type N>
    constexpr function (self_type_t<N>&& rh_obj) noexcept
    : _M_closure (std::move (rh_obj._M_closure)),
      _M_storage (std::move (rh_obj._M_storage))
    {
        if constexpr (!rh_obj._M_storage.empty ())
        {
            static_assert (storage_size <= sizeof (rh_obj._M_storage.size ()),
                          "function storage size mismatch!");
        }
    }

    constexpr function (std::function<R(Args...)> const& fn) noexcept
    {
        bind (fn.template target<R(*)(Args...)> ());
    }

    constexpr function (std::function<R(Args...)>&& fn) noexcept
    {
        bind (fn.template target<R(*)(Args...)> ());
        fn = nullptr;
    }

    template <size_type N>
    constexpr self_type& operator = (self_type_t<N> const& rh_obj) noexcept
    {
        if constexpr (!rh_obj._M_storage.empty ())
        {
            static_assert (storage_size <= sizeof (rh_obj._M_storage.size ()),
                          "function storage size mismatch!");
        }

        if (this == &rh_obj) return *this;

        _M_closure = rh_obj._M_closure;
        _M_storage = rh_obj._M_storage;

        return *this;
    }

    template <size_type N>
    constexpr self_type& operator = (self_type_t<N>&& rh_obj) noexcept
    {
        if constexpr (!rh_obj._M_storage.empty ())
        {
            static_assert (storage_size <= sizeof (rh_obj._M_storage.size ()),
                          "function storage size mismatch!");
        }

        if (this == &rh_obj) return *this;

        _M_closure = std::move (rh_obj._M_closure);
        _M_storage = std::move (rh_obj._M_storage);

        rh_obj._M_storage = { };

        return *this;
    }

    constexpr self_type& operator = (std::function<R(Args...)> const& fn) noexcept
    {
        bind (fn.template target<R(*)(Args...)> ());
        _M_storage = { };

        return *this;
    }

    constexpr self_type& operator = (std::function<R(Args...)>&& fn) noexcept
    {
        bind (fn.template target<R(*)(Args...)> ());
        _M_storage = { };

        fn = nullptr;
        return *this;
    }

    constexpr self_type& operator = (std::nullptr_t) noexcept
    {
        _M_closure = nullptr;
        _M_storage = { };

        return *this;
    }

    /// static function assignment operator
    constexpr self_type& operator = (static_fn_ref fnToBind) noexcept
    {
        _M_storage = { };

        bind (&fnToBind);
        return *this;
    }

    /// member function assignment operator
    template <class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator = (mem_fn_pair<C> const& pair)
    {
        _M_storage = { };

        bind (pair.first, pair.second);
        return *this;
    }

    /// const member function assignment operator
    template <class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator = (const_mem_fn_pair<C> const& pair)
    {
        _M_storage = { };

        bind (pair.first, pair.second);
        return *this;
    }

    /// callable object assignment operator
    template <callable_class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr self_type& operator = (C& obj)
    {
        // Block function types for callable class constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("callable class assignment cannot be used with function types!");
        }
        else
        {
            _M_storage = { };

            bind (obj, callable_operator_v<C>);
        }

        return *this;
    }

    /// capture lambda assignment operator
    template <lambda_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<Callable>, std::decay_t<Callable>>,
              typename   = LambdaCaptureType<Callable>
              >
    constexpr self_type& operator = (Callable&& callable)
    {
        // Block function types for lambda capture constructor
        if constexpr (is_functional_v<C>)
        {
            throw std::runtime_error ("lambda capture constructor cannot be used with function types!");
        }
        else
        {
            static_assert (sizeof (C) <= storage_size,
                          "Capture lambda too large for static storage! "
                          "DON'T value copy captured objects bigger than 16 bytes! "
                          "Use references instead!");

            ::new (&get_storage<C> ()) C (std::move (callable));

            bind (get_storage<C> (), callable_operator_v<C>);
        }

        return *this;
    }

    /// non-capture lambda assignment operator
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!is_functional_v<Callable>, std::decay_t<Callable>>,
              typename   = LambdaNonCaptureType<Callable>
              >
    constexpr self_type& operator = (Callable&& callable)
    {
        bind (callable, callable_operator_v<C>);
        return *this;
    }

    inline R operator () (Args&&... args) const
    {
#       ifdef DEBUG_MODE
        if (_M_closure == nullptr) throw bad_function_call ();
#       endif
        return (_M_closure.object ()->*_M_closure.function_ptr ())
               (std::forward<Args> (args)...);
    }

    constexpr explicit operator safe_bool () const noexcept
    {
        return _M_closure != nullptr ? &self_type::_M_closure : nullptr;
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    {
        return _M_closure == nullptr;
    }

    constexpr bool operator == (self_type const& mImpl) const noexcept
    {
        return _M_closure == mImpl._M_closure && _M_storage == mImpl._M_storage;
    }

    constexpr bool operator == (static_fn_ref mFuncPtr) const noexcept
    { return _M_closure == &mFuncPtr; }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !operator == (nullptr); }

    constexpr bool operator != (self_type const& mImpl) const noexcept
    { return !operator == (mImpl); }

    constexpr bool operator != (static_fn_ref mFuncPtr) const noexcept
    { return !(*this == mFuncPtr); }

    constexpr bool operator  < (self_type const& mImpl) const
    { return _M_closure < mImpl._M_closure; }

    constexpr bool operator  > (self_type const& mImpl) const
    { return _M_closure > mImpl._M_closure; }

protected:
    template <class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr C& get_storage () const
    {
        return *direct_cast<C*> (&_M_storage[0]);
    }

    constexpr R invoke_static_func (Args... args) const
    {
        return (*(_M_closure.static_func ()))(std::forward<Args> (args)...);
    }

    constexpr void bind (static_fn_type fnToBind) noexcept
    {
        _M_closure.bind_static_func (this, &function::invoke_static_func, fnToBind);
    }

    template <class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr void bind (C& obj, mem_fn_type<C> fnToBind)
    {
        _M_closure.bind_mem_func (&obj, fnToBind);
    }

    template <class_t C, typename = std::enable_if_t<!is_functional_v<C>>>
    constexpr void bind (C& obj, const_mem_fn_type<C> fnToBind)
    {
        _M_closure.bind_mem_func (&obj, fnToBind);
    }

    //! overload for binding functions with different signatures
    template <typename F, typename = std::enable_if_t<!is_functional_v<F>>>
    constexpr void bind (F&& fn, auto&&...)
    {
        if constexpr (is_functional_v<std::decay_t<F>>)
        {
            // Handle void() case
            //bind (fn, &fn_object::operator ());
            throw std::runtime_error ("function types can only be used with copy/move constructor "
                                      "and copy/move assigment operator!");
        }
        else
        {
            constexpr auto const lambda_capture = [fn = std::forward<F> (fn)](Args... args) -> R
            {
                if constexpr (std::is_void_v<R>) fn (std::forward<Args> (args)...);
                else return fn (std::forward<Args> (args)...);
            };

            typedef decltype (lambda_capture) lambda_type;

            static_assert (sizeof (lambda_type) <= storage_size,
                          "capture lambda too large for static storage!");

            ::new (&get_storage<lambda_type> ()) lambda_type (std::move (lambda_capture));

            // Handle general case with type conversion
            bind (get_storage<lambda_type> (), callable_operator_v<lambda_type>);
        }
    }

    template <typename, size_type>
    friend class function;

    template <typename>
    friend struct ::std::hash;

public:
    closure_type _M_closure { };

    //! bytes for storage of capture lambda
    alignas (std::max_align_t) storage_type _M_storage;
};

// ====================================================

/// static function make_fn
template <typename R, typename... Args>
constexpr function<R(Args...)> make_fn (R (& static_fn)(Args...))
{
    return function<R(Args...)> (static_fn);
}

/// member function make_fn
template <class_t C, typename R, typename... Args, typename = std::enable_if_t<!is_functional_v<C>>>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* member_fn)(Args...))
{
    return function<R(Args...)> (obj, member_fn);
}

/// const member function make_fn
template <class_t C, typename R, typename... Args, typename = std::enable_if_t<!is_functional_v<C>>>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* const_member_fn)(Args...) const)
{
    return function<R(Args...)> (obj, const_member_fn);
}

/// capture lambda make_fn
template <lambda_capture_t Callable,
          typename... Args ,
          typename    R = callable_return_t<Callable, Args...>,
          typename    C = std::enable_if_t<!is_functional_v<Callable>, std::decay_t<Callable>>,
          std::size_t N = sizeof (C) < LAMBDA_MAX_SIZE ? LAMBDA_MAX_SIZE : sizeof (C)
          >
constexpr function<R(Args...), N> make_fn (C&& callable, LambdaCaptureType<Callable>* = nullptr)
{
    return function<R(Args...), N> (std::forward<C> (callable));
}

/// non-capture lambda make_fn
template <lambda_non_capture_t Callable,
          typename...  Args,
          typename R = callable_return_t<Callable, Args...>,
          typename C = std::enable_if_t<!is_functional_v<Callable>, std::decay_t<Callable>>
          >
constexpr function<R(Args...)> make_fn (C&& fn, LambdaNonCaptureType<Callable>* = nullptr)
{
    return function<R(Args...)> (std::forward<C> (fn));
}

/// callable object make_fn
template <class_t  C      ,
          typename... Args,
          typename R = callable_return_t<C, Args...>,
          typename   = std::enable_if_t<!is_functional_v<C>>
          >
constexpr function<R(Args...)> make_fn (C& obj)
{
    return function<R(Args...)> (obj);
}

// /// callable object + storage move make_fn
// template <class_t  C      ,
//           typename... Args,
//           typename R = callable_return_t<C, Args...>,
//           typename   = std::enable_if_t<!is_functional_v<C>>
//           >
// constexpr function<R(Args...)> make_fn (C& obj, std::shared_ptr<void>&& storage)
// {
//     return function<R(Args...)> (obj, std::move (storage));
// }

// /// callable object + storage copy make_fn
// template <class_t  C      ,
//           typename... Args,
//           typename R = callable_return_t<C, Args...>,
//           typename   = std::enable_if_t<!is_functional_v<C>>
//           >
// constexpr function<R(Args...)> make_fn (C& obj, std::shared_ptr<void> const& storage)
// {
//     return function<R(Args...)> (obj, storage);
// }

// ====================================================

} // namespace cppual

// ====================================================

namespace std {

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

} // namespace std

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
