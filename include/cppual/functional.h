/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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

#include <cppual/types.h>
#include <cppual/concepts.h>
#include <cppual/memory/allocator.h>
#include <cppual/exceptions/functional_exception.h>

#include <cstring>
#include <tuple>

namespace cppual {

class any_object;
typedef void (any_object::* any_member_fn)();
typedef void (* any_static_fn)();

template <typename TSignature>
class function;

template <typename T, typename... Args>
using static_fn = T (*)(Args...);

template <typename T, typename... Args>
using member_fn = T (any_object::*)(Args...);

template <typename T>
struct function_traits;

// ====================================================

template <std::size_t N>
struct simplify_mem_func
{
    template <class X, class XFuncType>
    constexpr
    static
    any_object* convert (X const*, XFuncType, any_member_fn&) noexcept
    {
        static_assert (N - 100, "unsupported member function pointer on this compiler");
        return nullptr;
    }
};

template <>
struct simplify_mem_func <sizeof (any_member_fn)>
{
    template <class X, class XFuncType>
    inline
    static
    any_object* convert (X const* pThis, XFuncType mFuncToBind, any_member_fn& mFuncBound) noexcept
    {
        mFuncBound = direct_cast<any_member_fn> (mFuncToBind);
        return reinterpret_cast<any_object*> (const_cast<X*> (pThis));
    }
};

// ====================================================

template <typename ReturnType, typename... Args>
struct function_traits<ReturnType(*)(Args...)>
{
    // arity is the number of arguments
    enum { arity = sizeof...(Args) };

    typedef ReturnType result_type;

    template <std::size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
{
    // arity is the number of arguments
    enum { arity = sizeof...(Args) };

    typedef ReturnType result_type;

    template <std::size_t i>
    struct arg
    {
        typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
    };
};

// ====================================================

template <class TMemFunc, class TStaticFunc>
class closure
{
public:
    typedef std::size_t   size_type ;
    typedef any_object*   pointer   ;
    typedef any_member_fn value_type;

    constexpr closure () noexcept
    : _M_pObj (),
      _M_fn   ()
    { }

    constexpr explicit closure (std::nullptr_t) noexcept
    : _M_pObj (),
      _M_fn   ()
    { }

    constexpr closure (closure const& mClosure) noexcept
    : _M_pObj { mClosure._M_pObj },
      _M_fn   { mClosure._M_fn   }
    { }

    inline closure (closure&& mClosure) noexcept
    : _M_pObj { std::move (mClosure._M_pObj) },
      _M_fn   { std::move (mClosure._M_fn  ) }
    { mClosure = nullptr; }

    inline closure& operator = (std::nullptr_t) noexcept
    {
        _M_pObj = nullptr;
        _M_fn   = nullptr;
        return  * this   ;
    }

    inline closure& operator = (closure const& mRhs) noexcept
    {
        if (this == &mRhs) return *this;

        _M_pObj = mRhs._M_pObj;
        _M_fn   = mRhs._M_fn  ;
        return  * this        ;
    }

    inline closure& operator = (closure&& mRhs) noexcept
    {
        if (this == &mRhs) return *this;

        _M_pObj = mRhs._M_pObj;
        _M_fn   = mRhs._M_fn  ;

        mRhs   = nullptr;
        return * this   ;
    }

    template <class X, class XMemFunc>
    inline void bind_mem_func (X* pThis, XMemFunc mFuncToBind) noexcept
    {
        _M_pObj = simplify_mem_func<sizeof (mFuncToBind)>::convert
                 (pThis, mFuncToBind, _M_fn);
    }

    template <class DerivedClass, class ParentInvokerSig>
    inline void bind_static_func (DerivedClass*    mPtrParent,
                                  ParentInvokerSig mStaticFuncInvoker,
                                  TStaticFunc      mFuncToBind) noexcept
    {
        static_assert (sizeof (pointer) == sizeof (mFuncToBind), "Cannot use direct_cast");

        if (mFuncToBind == nullptr) _M_fn = nullptr;
        else bind_mem_func (mPtrParent, mStaticFuncInvoker);

        _M_pObj = direct_cast<pointer> (mFuncToBind);
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    { return _M_fn == nullptr || _M_pObj == nullptr; }

    constexpr bool operator == (closure const& mRhs) const noexcept
    { return _M_pObj == mRhs._M_pObj && _M_fn == mRhs._M_fn; }

    constexpr bool operator == (TStaticFunc mPtr) const noexcept
    {
        return mPtr == nullptr ?
              *this == nullptr :
               mPtr == reinterpret_cast<TStaticFunc> (static_func ());
    }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !operator == (nullptr); }

    constexpr bool operator != (closure const& mRhs) const noexcept
    { return !operator == (mRhs)   ; }

    constexpr bool operator != (TStaticFunc mPtr) const noexcept
    { return !operator == (mPtr)   ; }

    inline bool operator < (closure const& mRhs) const
    {
        return _M_pObj != mRhs._M_pObj ?
                              _M_pObj < mRhs._M_pObj :
                              std::memcmp (&_M_fn, &mRhs._M_fn, sizeof (_M_fn)) < 0;
    }

    inline bool operator > (closure const& mRhs) const
    { return mRhs < *this; }

    constexpr size_type hash () const noexcept
    {
        return unsafe_direct_cast<size_type> (_M_pObj) ^
               unsafe_direct_cast<size_type> (_M_fn  ) ;
    }

    constexpr pointer object () const noexcept
    { return _M_pObj; }

    constexpr TMemFunc ptr_function () const noexcept
    { return direct_cast<TMemFunc> (_M_fn); }

    constexpr TStaticFunc static_func () const noexcept
    { return direct_cast<TStaticFunc> (this); }

private:
    pointer    _M_pObj { };
    value_type _M_fn   { };
};

// ====================================================

//! reimplementation of impossibly fast delegates
template <typename T, typename... Args>
class function <T(Args...)> final
{
public:
    typedef member_fn<T, Args...>                    any_mem_fn_type;
    typedef static_fn<T, Args...>                    static_fn_type ;
    typedef closure<any_mem_fn_type, static_fn_type> closure_type   ;
    typedef std::shared_ptr<void>                    storage_type   ;
    typedef typename closure_type::value_type        value_type     ;
    typedef typename closure_type::pointer           pointer        ;

    template <typename X>
    using mem_fn_type = T (X::*)(Args...);

    /// capture lambda constructor
    template <typename Callable,
              typename Allocator = memory::allocator<Callable>,
              typename =
              typename std::enable_if<!std::is_same<function, typename std::decay<Callable>::type>{}>::type
              >
    inline function (Callable&& mFunc,
                     Allocator const& ator = Allocator(),
                     LambdaCapturePtr<Callable> = nullptr)
    : _M_storage (memory::allocate_shared<Callable, void>(ator, std::forward<Callable> (mFunc)))
    {
        using FuncType = typename std::decay<Callable>::type;

        _M_closure.bind_mem_func (static_cast<FuncType*>(_M_storage.get ()), &FuncType::operator ());
    }

    /// callable constructor
    template <typename Callable,
              typename =
              typename std::enable_if<!std::is_same<function, typename std::decay<Callable>::type>{}>::type>
    inline function (Callable&& mFunc,
                     LambdaNonCapturePtr<Callable> = nullptr)
    : _M_storage ()
    {
        using FuncType = typename std::decay<Callable>::type;

        _M_closure.bind_mem_func (&mFunc, &FuncType::operator ());
    }

    template <typename Callable,
              typename =
              typename std::enable_if<!std::is_same<function, typename std::decay<Callable>::type>{}>::type
              >
    inline function (Callable&& mFunc, storage_type&& storage)
    : _M_storage (std::move(storage))
    {
        using FuncType = typename std::decay<Callable>::type;

        _M_closure.bind_mem_func (&mFunc, &FuncType::operator ());
    }

    constexpr function () noexcept
    : _M_closure (),
      _M_storage ()
    { }

    constexpr explicit function (std::nullptr_t) noexcept
    : _M_closure (),
      _M_storage ()
    { }

    constexpr function (function const& mImpl)
    : _M_closure { mImpl._M_closure },
      _M_storage { mImpl._M_storage }
    { }

    inline function (function&& mImpl) noexcept
    : _M_closure { std::move (mImpl._M_closure) },
      _M_storage { std::move (mImpl._M_storage) }
    { }

    /// static function constructor
    inline function (static_fn_type mFuncToBind) noexcept
    : _M_storage ()
    { bind (mFuncToBind); }

    /// member function constructor
    template <typename X, typename Object>
    inline function (Object* pThis, mem_fn_type<X> mFuncToBind) noexcept
    : _M_storage ()
    { bind (pThis, mFuncToBind); }

    inline function& operator = (function const& mImpl)
    {
        _M_closure = mImpl._M_closure;
        _M_storage = mImpl._M_storage;
        return *this;
    }

    inline function& operator = (function&& mImpl)
    {
        _M_closure = std::move (mImpl._M_closure);
        _M_storage = std::move (mImpl._M_storage);
        return *this;
    }

    inline function& operator = (std::nullptr_t) noexcept
    {
        _M_closure = nullptr;
        _M_storage.reset();
        return *this;
    }

    inline function& operator = (static_fn_type mFuncToBind) noexcept
    {
        bind (mFuncToBind);
        _M_storage.reset ();
        return *this;
    }

    inline T operator () (Args... mArgs) const
    {
        #ifdef DEBUG_MODE
        if (_M_closure == nullptr) throw bad_function_call ();
        #endif
        return (_M_closure.object ()->*(_M_closure.ptr_function ()))
                (std::forward<Args> (mArgs)...);
    }

    constexpr explicit operator bool () const noexcept
    {
        return _M_closure != nullptr;
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    {
        return _M_closure == nullptr;
    }

    constexpr bool operator == (function const& mImpl) const noexcept
    { return _M_closure == mImpl._M_closure && _M_storage == mImpl._M_storage; }

    constexpr bool operator == (static_fn_type mFuncPtr) const noexcept
    { return _M_closure == mFuncPtr; }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !operator == (nullptr); }

    constexpr bool operator != (function const& mImpl) const noexcept
    { return !operator == (mImpl); }

    constexpr bool operator != (static_fn_type mFuncPtr) const noexcept
    { return !operator == (mFuncPtr); }

    constexpr bool operator  < (function const& mImpl) const
    { return _M_closure < mImpl._M_closure; }

    constexpr bool operator  > (function const& mImpl) const
    { return _M_closure > mImpl._M_closure; }

private:
    inline T invoke_static_func (Args... mArgs) const
    { return (*(_M_closure.static_func ()))(std::forward<Args> (mArgs)...); }

    inline void bind (static_fn_type mFuncToBind) noexcept
    { _M_closure.bind_static_func (this, &function::invoke_static_func, mFuncToBind); }

    template<typename X, typename Object>
    inline void bind (Object* pThis, mem_fn_type<X> mFuncToBind) noexcept
    { _M_closure.bind_mem_func (const_cast<const Object*> (pThis), mFuncToBind); }

private:
    closure_type _M_closure { };
    storage_type _M_storage { };
};

// ====================================================
// Delegate Concept
// ====================================================

template <typename>
struct is_delegate_helper : std::false_type
{ };

template <typename T>
struct is_delegate_helper < std::function<T> > : std::true_type
{ };

template <typename T>
struct is_delegate_helper < function<T> > : std::true_type
{ };

template <typename T>
struct is_delegate : is_delegate_helper<T>
{
    static_assert (is_delegate_helper<T>::value, "NOT a functional type!");
};

template <typename T>
using DelegateType = typename std::enable_if<is_delegate<T>::value, T>::type;

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
