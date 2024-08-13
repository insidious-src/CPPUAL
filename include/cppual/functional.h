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

// ====================================================

class any_object;
typedef void (any_object::* any_member_fn)();
typedef void (* any_static_fn)();

template <typename>
class function;

template <typename R, typename... Args>
using static_fn = R (*)(Args...);

template <typename R, typename... Args>
using member_fn = R (any_object::*)(Args...);

template <typename>
struct function_traits;

// ====================================================

template <typename R, typename... Args>
struct function_traits <R(Args...)>
{
    typedef std::size_t size_type  ;
    typedef R           return_type;

    using function_type = R(Args...);

    /// arity is the number of arguments
    constexpr static auto const arity = sizeof... (Args);

    template <size_type I = 0>
    struct arg
    {
        static_assert(I < arity, "invalid argument index");
        typedef typename std::tuple_element_t<I, std::tuple<Args...>> type;
    };

    template <size_type I = 0>
    using arg_t = arg<I>::type;
};

template <typename R, typename... Args>
struct function_traits <R(*)(Args...)> : function_traits<R(Args...)>
{
    using pointer = R(*)(Args...);
};

template <class C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...)> : function_traits<R(Args...)>
{
    typedef std::remove_cvref_t<C> object_type;

    using pointer = R(C::*)(Args...);
};

template <class C, typename R, typename... Args>
struct function_traits <R(C::*)(Args...) const> : function_traits<R(Args...)>
{
    typedef std::remove_cvref_t<C> object_type;

    using pointer = R(C::*)(Args...) const;
};

template <auto F,
          typename = std::enable_if_t<std::is_function_v<std::remove_pointer_t<decltype(F)>> ||
                                      std::is_member_function_pointer_v<decltype(F)>>
          >
struct function_ptr_traits : function_traits<decltype(F)>
{
    static constexpr auto const pointer_value = F;
};

// ====================================================

template <std::size_t N>
struct simplify_mem_fn
{
    template <class C, typename XFuncType>
    constexpr
    static
    any_object* convert (C const*, XFuncType, any_member_fn&) noexcept
    {
        static_assert (N - 100, "unsupported member function pointer on this compiler");
        return nullptr;
    }
};

template <>
struct simplify_mem_fn <sizeof (any_member_fn)>
{
    template <class C, typename XFuncType>
    inline
    static
    any_object* convert (C const* pThis, XFuncType mFuncToBind, any_member_fn& mFuncBound) noexcept
    {
        mFuncBound = direct_cast<any_member_fn> (mFuncToBind);
        return reinterpret_cast<any_object*> (const_cast<C*> (pThis));
    }
};

// ====================================================

template <typename TMemFunc, typename TStaticFunc>
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

    template <class C, typename XMemFunc>
    inline void bind_mem_func (C* pThis, XMemFunc mFuncToBind) noexcept
    {
        _M_pObj = simplify_mem_fn<sizeof (mFuncToBind)>::convert
                 (pThis, mFuncToBind, _M_fn);
    }

    template <class DerivedClass, typename ParentInvokerSig>
    inline void bind_static_func (DerivedClass*    mPtrParent,
                                  ParentInvokerSig mStaticFuncInvoker,
                                  TStaticFunc      mFuncToBind) noexcept
    {
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

    constexpr TMemFunc function_ptr () const noexcept
    { return direct_cast<TMemFunc> (_M_fn); }

    constexpr TStaticFunc static_func () const noexcept
    { return direct_cast<TStaticFunc> (this); }

private:
    pointer    _M_pObj { };
    value_type _M_fn   { };
};

// ====================================================

//! reimplementation of impossibly fast delegates
template <typename R, typename... Args>
class SHARED_API function <R(Args...)> : function_traits<R(Args...)>
{
public:
    typedef member_fn<R, Args...>                    any_mem_fn_type;
    typedef static_fn<R, Args...>                    static_fn_type ;
    typedef closure<any_mem_fn_type, static_fn_type> closure_type   ;
    typedef std::shared_ptr<void>                    storage_type   ;
    typedef closure_type::pointer                    pointer        ;
    typedef closure_type::value_type                 value_type     ;

    template <class C>
    using mem_fn_type = R (C::*)(Args...);

    template <class C>
    using const_mem_fn_type = R (C::*)(Args...) const;

    /// capture lambda constructor
    template <typename Callable,
              typename Allocator = memory::allocator<Callable>,
              typename = std::enable_if_t<!std::is_same<function, std::decay_t<Callable>>{}>
              >
    inline function (Callable&& mFunc,
                     Allocator const& ator = Allocator(),
                     LambdaCapturePtr<Callable> = nullptr)
    : _M_storage (memory::allocate_shared<void> (ator, std::forward<Callable> (mFunc)))
    {
        using FuncType = std::decay_t<Callable>;

        _M_closure.bind_mem_func (static_cast<FuncType*>(_M_storage.get ()), &FuncType::operator ());
    }

    /// non-capture lambda constructor
    template <typename Callable,
              typename = std::enable_if_t<!std::is_same<function, std::decay_t<Callable>>{}>
              >
    inline function (Callable&& mFunc,
                     LambdaNonCapturePtr<Callable> = nullptr)
    : _M_storage ()
    {
        using FuncType = std::decay_t<Callable>;

        _M_closure.bind_mem_func (&mFunc, &FuncType::operator ());
    }

    template <typename Callable,
              typename = std::enable_if_t<!std::is_same<function, typename std::decay_t<Callable>>{}>
              >
    inline function (Callable&& obj, storage_type&& storage)
    : _M_storage (std::move(storage))
    {
        using FuncType = std::decay_t<Callable>;

        _M_closure.bind_mem_func (&obj, &FuncType::operator ());
    }

    constexpr function () noexcept
    : _M_closure (),
      _M_storage ()
    { }

    /// static function constructor
    inline function (static_fn_type mFuncToBind) noexcept
    : _M_storage ()
    { bind (mFuncToBind); }

    /// member function constructor
    template <class C>
    inline function (C* pThis, mem_fn_type<C> mFuncToBind) noexcept
    : _M_storage ()
    { bind (pThis, mFuncToBind); }

    /// const member function constructor
    template <class C>
    inline function (C* pThis, const_mem_fn_type<C> mFuncToBind) noexcept
    : _M_storage ()
    { bind (pThis, mFuncToBind); }

    template <class C>
    inline function (C* pThis)
    : _M_storage ()
    {
        _M_closure.bind_mem_func (pThis, mem_fn_type<C> (&C::operator ()));
    }

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

    inline R operator () (Args&&... args) const
    {
        #ifdef DEBUG_MODE
        if (_M_closure == nullptr) throw bad_function_call ();
        #endif
        return (_M_closure.object ()->*(_M_closure.function_ptr ()))
               (std::forward<Args> (args)...);
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
    {
        return _M_storage == mImpl._M_storage ?
               _M_storage.get () == nullptr ?
               _M_closure == mImpl._M_closure :
               _M_closure.function_ptr () == mImpl._M_closure.function_ptr () :
               _M_closure == mImpl._M_closure;
    }

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

protected:
    inline R invoke_static_func (Args... mArgs) const
    { return (*(_M_closure.static_func ()))(std::forward<Args> (mArgs)...); }

    inline void bind (static_fn_type mFuncToBind) noexcept
    { _M_closure.bind_static_func (this, &function::invoke_static_func, mFuncToBind); }

    template <class C>
    inline void bind (C* pThis, mem_fn_type<C> mFuncToBind) noexcept
    { _M_closure.bind_mem_func (const_cast<const C*> (pThis), mFuncToBind); }

    template <class C>
    inline void bind (C* pThis, const_mem_fn_type<C> mFuncToBind) noexcept
    { _M_closure.bind_mem_func (const_cast<const C*> (pThis), mFuncToBind); }

private:
    closure_type _M_closure { };
    storage_type _M_storage { };
};

// ====================================================
// Delegate Concept
// ====================================================

template <typename T>
struct is_delegate : std::false_type
{
    static_assert (is_delegate<T>::value, "NOT a functional type!");
};

template <typename T>
struct is_delegate < std::function<T> > : std::true_type
{ };

template <typename T>
struct is_delegate < function<T> > : std::true_type
{ };

template <typename T>
inline constexpr auto const is_delegate_v = is_delegate<T>::value;

template <typename T>
using DelegateType = typename std::enable_if_t<is_delegate_v<T>, T>;

// ====================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
