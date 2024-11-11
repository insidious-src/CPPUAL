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

namespace cppual {

// ====================================================

/// import needed concepts
using memory::allocator_t;

// ====================================================

class any_object;
typedef void(any_object::* any_member_fn)();
typedef void(* any_static_fn)();

template <typename>
class function;

template <typename R, typename... Args>
using static_fn = R(*)(Args...);

template <typename R, typename... Args>
using member_fn = R(any_object::*)(Args...);

template <typename>
struct function_traits;

// ====================================================

template <typename R, typename... Args>
struct function_traits <R(Args...)>
{
    typedef std::size_t size_type  ;
    typedef R           return_type;

    using type          = R(Args...);
    using function_type = function<R(Args...)>;

    /// arity is the number of arguments
    inline constexpr static size_type const arity = sizeof... (Args);

    template <size_type I = 0>
    struct arg
    {
        static_assert(I < arity, "argument index is out of range!");
        typedef std::tuple_element_t<I, std::tuple<Args...>> type;
    };

    template <size_type I = 0>
    using arg_t = arg<I>::type;
};

template <typename R, typename... Args>
struct function_traits <R(*)(Args...)> : function_traits<R(Args...)>
{
    typedef R(* fn_pointer)(Args...);
};

template <class_t C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...)> : function_traits<R(Args...)>
{
    typedef std::remove_cvref_t<C> object_type;
    typedef R(C::* fn_pointer)(Args...)       ;
};

template <class_t C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...) const> : function_traits<R(Args...)>
{
    typedef std::remove_cvref_t<C> object_type;
    typedef R(C::* fn_pointer)(Args...)  const;
};

template <auto F,
          typename = std::enable_if_t<std::is_function_v<std::remove_pointer_t<decltype (F)>> ||
                                      std::is_member_function_pointer_v<decltype (F)>>
          >
struct function_ptr_traits : function_traits<decltype (F)>
{
    inline static constexpr auto const value = F;

    constexpr static auto fn_signature () noexcept -> decltype (F)
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

template <std::size_t N>
struct simplify_mem_fn
{
    template <class_t C, typename XFuncType>
    constexpr
    static
    any_object* convert (C const*, XFuncType, any_member_fn&) noexcept
    {
        static_assert (N - 100, "unsupported member function pointer on this compiler!");
        return nullptr;
    }
};

template <>
struct simplify_mem_fn <sizeof (any_member_fn)>
{
    template <class_t C, member_function_t XFuncType>
    constexpr
    static
    any_object* convert (C const* pObj, XFuncType mFuncToBind, any_member_fn& mFuncBound) noexcept
    {
        mFuncBound = direct_cast<any_member_fn> (mFuncToBind);
        return direct_cast<any_object*> (const_cast<C*> (pObj));
    }
};

// ====================================================

template <member_function_t TMemFunc, static_function_t TStaticFunc>
class closure
{
public:
    typedef closure<TMemFunc, TStaticFunc> self_type ;
    typedef std::size_t                    size_type ;
    typedef any_object*                    pointer   ;
    typedef any_member_fn                  value_type;

    constexpr closure () noexcept
    : _M_pObj (),
      _M_fn   ()
    { }

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

    template <class_t C, member_function_t XMemFunc>
    constexpr void bind_mem_func (C* pObj, XMemFunc mFuncToBind) noexcept
    {
        _M_pObj = simplify_mem_fn<sizeof (mFuncToBind)>::convert (pObj, mFuncToBind, _M_fn);
    }

    template <class_t DerivedClass, typename ParentInvokerSig>
    constexpr void bind_static_func (DerivedClass*    mPtrParent,
                                     ParentInvokerSig mStaticFuncInvoker,
                                     TStaticFunc      mFuncToBind) noexcept
    {
        if (mFuncToBind == nullptr) _M_fn = nullptr;
        else bind_mem_func (mPtrParent, mStaticFuncInvoker);

        _M_pObj = direct_cast<pointer> (mFuncToBind);
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    { return _M_fn == nullptr || _M_pObj == nullptr; }

    constexpr bool operator == (self_type const& mRhs) const noexcept
    { return _M_pObj == mRhs._M_pObj && _M_fn == mRhs._M_fn; }

    constexpr bool operator == (TStaticFunc mPtr) const noexcept
    {
        return mPtr == nullptr ?
              *this == nullptr :
               mPtr == static_cast<TStaticFunc> (static_func ());
    }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !(*this == nullptr); }

    constexpr bool operator != (self_type const& mRhs) const noexcept
    { return !(*this == mRhs); }

    constexpr bool operator != (TStaticFunc mPtr) const noexcept
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

    constexpr TMemFunc function_ptr () const noexcept
    { return direct_cast<TMemFunc> (_M_fn); }

    constexpr TStaticFunc static_func () const noexcept
    { return direct_cast<TStaticFunc> (this); }

    template <typename>
    friend class function;

    template <typename>
    friend struct ::std::hash;

private:
    pointer    _M_pObj { };
    value_type _M_fn   { };
};

// ====================================================

//! reimplementation of impossibly fast delegates
template <typename R, typename... Args>
class SHARED_API function <R(Args...)> : public function_traits<R(Args...)>
{
public:
    typedef function <R(Args...)>                   self_type      ;
    typedef member_fn<R, Args...>                   member_fn_type ;
    typedef static_fn<R, Args...>                   static_fn_type ;
    typedef closure<member_fn_type, static_fn_type> closure_type   ;
    typedef std::shared_ptr<void>                   storage_type   ;
    typedef closure_type::pointer                   pointer        ;
    typedef closure_type::value_type                value_type     ;
    typedef closure_type* self_type::*              safe_bool      ;

    template <class_t C>
    using mem_fn_type = R(C::*)(Args...);

    template <class_t C>
    using const_mem_fn_type = R(C::*)(Args...) const;

    typedef R(& static_fn_ref)(Args...);

    constexpr function () noexcept
    : _M_closure (),
      _M_storage ()
    { }

    /// capture lambda constructor
    template <lambda_capture_t Callable,
              allocator_t A = memory::allocator<Callable>,
              typename    C = std::enable_if_t<!std::is_same_v<self_type, std::decay_t<Callable>>,
                                                std::decay_t<Callable>>
              >
    constexpr function (Callable&& callable, A const& ator = A (), LambdaCaptureType<Callable>* = nullptr)
    : _M_storage (memory::allocate_shared<void, C> (ator, std::move (callable)))
    {
        _M_closure.bind_mem_func (static_cast<C*> (_M_storage.get ()), &C::operator ());
    }

    /// non-capture lambda constructor
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!std::is_same_v<self_type, std::decay_t<Callable>>,
                                             std::decay_t<Callable>>
              >
    constexpr function (Callable&& callable, LambdaNonCaptureType<Callable>* = nullptr)
    : _M_storage ()
    {
        _M_closure.bind_mem_func (&callable, &C::operator ());
    }

    /// static function constructor
    constexpr function (static_fn_ref mFuncToBind) noexcept
    : _M_storage ()
    { bind (&mFuncToBind); }

    /// member function constructor
    template <class_t C>
    constexpr function (C& pObj, mem_fn_type<C> mFuncToBind) noexcept
    : _M_storage ()
    { bind (pObj, mFuncToBind); }

    /// const member function constructor
    template <class_t C>
    constexpr function (C& pObj, const_mem_fn_type<C> mFuncToBind) noexcept
    : _M_storage ()
    { bind (pObj, mFuncToBind); }

    /// callable object constructor
    template <class_t C, typename = std::enable_if_t<!std::is_same_v<self_type, C>>>
    constexpr function (C& pObj)
    : _M_storage ()
    {
        _M_closure.bind_mem_func (&pObj, &C::operator ());
    }

    /// callable object + storage move constructor
    template <class_t C, typename = std::enable_if_t<!std::is_same_v<self_type, C>>>
    constexpr function (C& obj, storage_type&& storage)
    : _M_storage (std::move (storage))
    {
        _M_closure.bind_mem_func (&obj, &C::operator ());
    }

    /// callable object + storage copy constructor
    template <class_t C, typename = std::enable_if_t<!std::is_same_v<self_type, C>>>
    constexpr function (C& obj, storage_type const& storage)
    : _M_storage (storage)
    {
        _M_closure.bind_mem_func (&obj, &C::operator ());
    }

    constexpr explicit function (std::nullptr_t) noexcept
    : _M_closure (),
      _M_storage ()
    { }

    constexpr function (self_type const& mImpl) noexcept
    : _M_closure { mImpl._M_closure },
      _M_storage { mImpl._M_storage }
    { }

    constexpr function (self_type&& mImpl) noexcept
    : _M_closure { std::move (mImpl._M_closure) },
      _M_storage { std::move (mImpl._M_storage) }
    { }

    constexpr self_type& operator = (self_type const& mImpl)
    {
        _M_closure = mImpl._M_closure;
        _M_storage = mImpl._M_storage;
        return *this;
    }

    constexpr self_type& operator = (self_type&& mImpl)
    {
        _M_closure = std::move (mImpl._M_closure);
        _M_storage = std::move (mImpl._M_storage);
        return *this;
    }

    constexpr self_type& operator = (std::nullptr_t) noexcept
    {
        _M_closure = nullptr;
        _M_storage.reset();
        return *this;
    }

    /// static function assignment operator
    constexpr self_type& operator = (static_fn_ref mFuncToBind) noexcept
    {
        bind (&mFuncToBind);
        _M_storage.reset ();
        return *this;
    }

    /// member function assignment operator
    template <class_t C>
    constexpr self_type& operator = (std::pair<C&, mem_fn_type<C>> const& pair) noexcept
    {
        bind (pair.first, pair.second);
        _M_storage.reset ();
        return *this;
    }

    /// const member function assignment operator
    template <class_t C>
    constexpr self_type& operator = (std::pair<C&, const_mem_fn_type<C>> const& pair) noexcept
    {
        bind (pair.first, pair.second);
        _M_storage.reset ();
        return *this;
    }

    /// callable object assignment operator
    template <class_t C,
              typename = std::enable_if_t<!std::is_same_v<self_type, std::decay_t<C>>>
              >
    constexpr self_type& operator = (C& pObj)
    {
        _M_closure.bind_mem_func (&pObj, &C::operator ());
        _M_storage.reset();

        return *this;
    }

    /// capture lambda assignment operator
    template <lambda_capture_t Callable,
              allocator_t A = memory::allocator<Callable>,
              typename    C = std::enable_if_t<!std::is_same_v<self_type, std::decay_t<Callable>>,
                                                std::decay_t<Callable>>,
              typename      = LambdaCaptureType<Callable>
              >
    constexpr self_type& operator = (std::pair<Callable&&, A const&> pair)
    {
        _M_storage = std::move (memory::allocate_shared<void, Callable>
                               (pair.second, std::move (pair.first)));

        _M_closure.bind_mem_func (static_cast<C*> (_M_storage.get ()), &C::operator ());

        return *this;
    }

    /// non-capture lambda assignment operator
    template <lambda_non_capture_t Callable,
              typename C = std::enable_if_t<!std::is_same_v<self_type, std::decay_t<Callable>>,
                                             std::decay_t<Callable>>,
              typename   = LambdaNonCaptureType<Callable>
              >
    constexpr self_type& operator = (Callable&& callable)
    {
        _M_closure.bind_mem_func (&callable, &C::operator ());
        _M_storage.reset ();
        return *this;
    }

    constexpr R operator () (Args&&... args) const
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
        return _M_storage == mImpl._M_storage ?
               _M_storage.get () == nullptr   ?
               _M_closure == mImpl._M_closure :
               _M_closure.object       () == mImpl._M_closure.object       () &&
               _M_closure.function_ptr () == mImpl._M_closure.function_ptr () :
               _M_closure == mImpl._M_closure;
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
    constexpr R invoke_static_func (Args... args) const
    { return (*(_M_closure.static_func ()))(std::forward<Args> (args)...); }

    constexpr void bind (static_fn_type mFuncToBind) noexcept
    { _M_closure.bind_static_func (this, &function::invoke_static_func, mFuncToBind); }

    template <class_t C>
    constexpr void bind (C& pObj, mem_fn_type<C> mFuncToBind) noexcept
    { _M_closure.bind_mem_func (const_cast<const C*> (&pObj), mFuncToBind); }

    template <class_t C>
    constexpr void bind (C& pObj, const_mem_fn_type<C> mFuncToBind) noexcept
    { _M_closure.bind_mem_func (const_cast<const C*> (&pObj), mFuncToBind); }

    template <typename>
    friend class function;

    template <typename>
    friend struct ::std::hash;

private:
    closure_type _M_closure { };
    storage_type _M_storage { };
};

// ====================================================

/// static function make_fn
template <typename R, typename... Args>
constexpr function<R(Args...)> make_fn (R (& static_fn)(Args...))
{
    return function<R(Args...)> (static_fn);
}

/// member function make_fn
template <class_t C, typename R, typename... Args>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* member_fn)(Args...))
{
    return function<R(Args...)> (obj, member_fn);
}

/// const member function make_fn
template <class_t C, typename R, typename... Args>
constexpr function<R(Args...)> make_fn (C& obj, R (C::* const_member_fn)(Args...) const)
{
    return function<R(Args...)> (obj, const_member_fn);
}

/// capture lambda make_fn
template <typename Callable,
          typename... Args ,
          typename    R = std::invoke_result_t<decltype (&std::decay_t<Callable>::operator ()), Args...>,
          allocator_t A = memory::allocator<Callable>,
          typename    C = std::enable_if_t<!std::is_same_v<function<R(Args...)>, std::decay_t<Callable>>,
                                            std::decay_t<Callable>
                                           >
          >
constexpr function<R(Args...)> make_fn (C&& callable,
                                        A const& ator = A (),
                                        LambdaCaptureType<Callable>* = nullptr)
{
    return function<R(Args...)> (std::forward<C> (callable), ator);
}

/// non-capture lambda make_fn
template <typename Callable,
          typename...  Args,
          typename R = std::invoke_result_t<decltype (&std::decay_t<Callable>::operator ()), Args...>,
          typename C = std::enable_if_t<!std::is_same_v<function<R(Args...)>, std::decay_t<Callable>>,
                                         std::decay_t<Callable>
                                        >
          >
constexpr function<R(Args...)> make_fn (C&& fn, LambdaNonCaptureType<Callable>* = nullptr)
{
    return function<R(Args...)> (std::forward<C> (fn));
}

/// callable object make_fn
template <class_t  C      ,
          typename... Args,
          typename R = std::invoke_result_t<decltype (&C::operator ()), Args...>,
          typename   = std::enable_if_t<!std::is_same_v<function<R(Args...)>, C>>
          >
constexpr function<R(Args...)> make_fn (C& obj)
{
    return function<R(Args...)> (obj);
}

/// callable object + storage move make_fn
template <class_t  C      ,
          typename... Args,
          typename R = std::invoke_result_t<decltype (&C::operator ()), Args...>,
          typename   = std::enable_if_t<!std::is_same_v<function<R(Args...)>, C>>
          >
constexpr function<R(Args...)> make_fn (C& obj, std::shared_ptr<void>&& storage)
{
    return function<R(Args...)> (obj, std::move (storage));
}

/// callable object + storage copy make_fn
template <class_t  C      ,
          typename... Args,
          typename R = std::invoke_result_t<decltype (&C::operator ()), Args...>,
          typename   = std::enable_if_t<!std::is_same_v<function<R(Args...)>, C>>
          >
constexpr function<R(Args...)> make_fn (C& obj, std::shared_ptr<void> const& storage)
{
    return function<R(Args...)> (obj, storage);
}

// ====================================================
// Delegate Concept
// ====================================================

template <typename T>
struct is_function : std::false_type
{
    static_assert (is_function<T>::value, "NOT a functional type!");
};

template <typename T>
struct is_function <std::function<T>> : std::true_type
{ };

template <typename T>
struct is_function <function<T>> : std::true_type
{ };

template <typename T>
inline constexpr bool const is_function_v = is_function<std::remove_cvref_t<T>>::value;

template <typename T>
using FunctionalType = std::enable_if_t<is_function_v<T>, T>;

/// delegate concept
template <typename T>
concept functional_t = is_function_v<T>;

// ====================================================

} // namespace cppual

// ====================================================

namespace std {

// ====================================================

template <typename S>
using fn_type = cppual::function<S>;

// ====================================================

template <typename R, typename... Args>
struct hash <fn_type<R(Args...)>>
{
    typedef std::size_t         size_type ;
    typedef fn_type<R(Args...)> value_type;

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
