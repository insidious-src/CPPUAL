/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#include <memory>
#include <cstring>
#include <functional>
#include <tuple>

namespace cppual {

class AnyObject;
typedef void (AnyObject::* AnyMemberFn)();
typedef void (* AnyStaticFn)();

template <typename TSignature>
class Function;

template <typename T, typename... Args>
using StaticFn = T (*)(Args...);

template <typename T, typename... Args>
using MemberFn = T (AnyObject::*)(Args...);

template <typename T>
struct function_traits;

// ====================================================

template <std::size_t N>
struct SimplifyMemFunc
{
    template <class X, class XFuncType>
    inline static AnyObject* convert (X const*, XFuncType, AnyMemberFn&) noexcept
    {
        static_assert (N - 100,
                       "unsupported member function pointer on this compiler");
        return nullptr;
    }
};

template <>
struct SimplifyMemFunc <sizeof (AnyMemberFn)>
{
    template <class X, class XFuncType>
    inline
    static
    AnyObject* convert (X const*     pThis,
                        XFuncType    mFuncToBind,
                        AnyMemberFn& mFuncBound ) noexcept
    {
        mFuncBound = direct_cast<AnyMemberFn> (mFuncToBind)    ;
        return reinterpret_cast<AnyObject*> (const_cast<X*> (pThis));
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
class Closure
{
public:
    typedef std::size_t size_type ;
    typedef AnyObject*  pointer   ;
    typedef AnyMemberFn value_type;

    constexpr Closure () noexcept = default;
    constexpr explicit Closure (std::nullptr_t) noexcept { }

    constexpr Closure (Closure const& mClosure) noexcept
    : m_pObj { mClosure.m_pObj },
      m_fn   { mClosure.m_fn   }
    { }

    inline Closure (Closure&& mClosure) noexcept
    : m_pObj { std::move (mClosure.m_pObj) },
      m_fn   { std::move (mClosure.m_fn  ) }
    { mClosure = nullptr; }

    inline Closure& operator = (std::nullptr_t) noexcept
    {
        m_pObj = nullptr;
        m_fn   = nullptr;
        return * this   ;
    }

    inline Closure& operator = (Closure const& mRhs) noexcept
    {
        m_pObj = mRhs.m_pObj;
        m_fn   = mRhs.m_fn  ;
        return * this       ;
    }

    inline Closure& operator = (Closure&& mRhs) noexcept
    {
        m_pObj = std::move (mRhs.m_pObj);
        m_fn   = std::move (mRhs.m_fn  );
        return * this                   ;
    }

    template <class X, class XMemFunc>
    inline void bindMemFunc (X* pThis, XMemFunc mFuncToBind) noexcept
    {
        m_pObj = SimplifyMemFunc<sizeof (mFuncToBind)>::convert
                 (pThis, mFuncToBind, m_fn);
    }

    template <class DerivedClass, class ParentInvokerSig>
    inline void bindStaticFunc (DerivedClass*    mPtrParent,
                                ParentInvokerSig mStaticFuncInvoker,
                                TStaticFunc      mFuncToBind) noexcept
    {
        static_assert (sizeof (pointer) == sizeof (mFuncToBind),
                       "Cannot use direct_cast")         ;
        if (mFuncToBind == nullptr) m_fn = nullptr       ;
        else bindMemFunc (mPtrParent, mStaticFuncInvoker);
        m_pObj = direct_cast<pointer> (mFuncToBind)      ;
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    { return m_fn == nullptr and m_pObj == nullptr; }

    constexpr bool operator == (Closure const& mRhs) const noexcept
    { return m_pObj == mRhs.m_pObj and m_fn == mRhs.m_fn; }

    constexpr bool operator == (TStaticFunc mPtr) const noexcept
    {
        return mPtr == nullptr ?
              *this == nullptr :
               mPtr == reinterpret_cast<TStaticFunc> (getStaticFunc ());
    }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !operator == (nullptr); }

    constexpr bool operator != (Closure const& mRhs) const noexcept
    { return !operator == (mRhs)   ; }

    constexpr bool operator != (TStaticFunc mPtr) const noexcept
    { return !operator == (mPtr)   ; }

    inline bool operator < (Closure const& mRhs) const
    {
        return m_pObj != mRhs.m_pObj ?
                              m_pObj < mRhs.m_pObj :
                              std::memcmp (&m_fn, &mRhs.m_fn, sizeof (m_fn)) < 0;
    }

    inline bool operator > (Closure const& mRhs) const
    { return mRhs < *this; }

    inline size_type hash () const noexcept
    {
        return reinterpret_cast  <size_type> (m_pObj) ^
               unsafe_direct_cast<size_type> (m_fn  ) ;
    }

    inline pointer object () const noexcept
    { return m_pObj; }

    inline TMemFunc getPtrFunction () const noexcept
    { return direct_cast<TMemFunc> (m_fn); }

    inline TStaticFunc getStaticFunc () const noexcept
    { return direct_cast<TStaticFunc> (this); }

private:
    pointer    m_pObj { };
    value_type m_fn   { };
};

// ====================================================

//! reimplementation of impossibly fast delegates
template <typename T, typename... Args>
class Function <T(Args...)>
{
public:
    typedef MemberFn<T, Args...>                     any_mem_fn_type;
    typedef StaticFn<T, Args...>                     static_fn_type ;
    typedef Closure<any_mem_fn_type, static_fn_type> closure_type   ;
    typedef std::shared_ptr<void>                    storage_type   ;
    typedef typename closure_type::value_type        value_type     ;
    typedef typename closure_type::pointer           pointer        ;

    template <typename X>
    using mem_fn_type = T (X::*)(Args...);

    // capture lambda constructor
    template <typename Callable,
              typename Allocator = std::allocator<CallableType<Callable>>,
              typename =
              typename std::enable_if<!std::is_same<Function, CallableType<Callable>>{}>::type
              >
    inline Function (Callable&& mFunc,
                     Allocator  ator = Allocator (),
                     LambdaCapturePtr<Callable> = nullptr)
    : m_storage (ator.allocate (1), [=](void* mPtr)
    {
        static_cast<Allocator> (ator).destroy    (static_cast<CallableType<Callable>*> (mPtr));
        static_cast<Allocator> (ator).deallocate (static_cast<CallableType<Callable>*> (mPtr), 1);
    })
    {
        ator.construct (static_cast<CallableType<Callable>*> (m_storage.get ()), std::forward<Callable> (mFunc));
        m_closure.bindMemFunc (m_storage.get (), &CallableType<Callable>::operator ());
    }

    // callable constructor
    template <typename Callable,
              typename =
              typename std::enable_if<!std::is_same<Function, CallableType<Callable>>{}>::type>
    inline Function (Callable&& mFunc,
                     LambdaNonCapturePtr<Callable> = nullptr)
    {
        m_closure.bindMemFunc (&mFunc, &CallableType<Callable>::operator ());
    }

    template <typename Callable,
              typename =
              typename std::enable_if<!std::is_same<Function, CallableType<Callable>>{}>::type
              >
    inline Function (Callable&& mFunc, storage_type&& storage)
    : m_storage (std::move(storage))
    {
        m_closure.bindMemFunc (&mFunc, &CallableType<Callable>::operator ());
    }

    constexpr Function () noexcept = default;
    constexpr explicit Function (std::nullptr_t) noexcept { }

    constexpr Function (Function const& mImpl) noexcept
    : m_closure { mImpl.m_closure },
      m_storage { mImpl.m_storage }
    { }

    inline Function (Function&& mImpl) noexcept
    : m_closure { std::move (mImpl.m_closure) },
      m_storage { std::move (mImpl.m_storage) }
    { }

    // static function constructor
    inline Function (static_fn_type mFuncToBind) noexcept
    { bind (mFuncToBind); }

    // member function constructor
    template <typename X, typename Object>
    inline Function (Object* pThis, mem_fn_type<X> mFuncToBind) noexcept
    { bind (mFuncToBind, pThis); }

    inline Function& operator = (Function const& mImpl) noexcept
    {
        m_closure = mImpl.m_closure;
        m_storage = mImpl.m_storage;
        return *this;
    }

    inline Function& operator = (Function&& mImpl) noexcept
    {
        m_closure = std::move (mImpl.m_closure);
        m_storage = std::move (mImpl.m_storage);
        return *this;
    }

    inline Function& operator = (static_fn_type mFuncToBind) noexcept
    {
        bind (mFuncToBind);
        m_storage.reset ();
        return *this;
    }

    inline T operator () (Args&&... mArgs) const
    {
        #ifdef DEBUG_MODE
        if (m_closure == nullptr) throw std::bad_function_call ();
        #endif
        return (m_closure.object ()->*(m_closure.getPtrFunction ()))
                (std::forward<Args> (mArgs)...);
    }

    constexpr bool operator == (std::nullptr_t) const noexcept
    { return m_closure == nullptr; }

    constexpr bool operator == (Function const& mImpl) const noexcept
    { return m_closure == mImpl.m_closure; }

    constexpr bool operator == (static_fn_type mFuncPtr) const noexcept
    { return m_closure == mFuncPtr; }

    constexpr bool operator != (std::nullptr_t) const noexcept
    { return !operator == (nullptr); }

    constexpr bool operator != (Function const& mImpl) const noexcept
    { return !operator == (mImpl); }

    constexpr bool operator != (static_fn_type mFuncPtr) const noexcept
    { return !operator == (mFuncPtr); }

    constexpr bool operator  < (Function const& mImpl) const
    { return m_closure < mImpl.m_closure; }

    constexpr bool operator  > (Function const& mImpl) const
    { return m_closure > mImpl.m_closure; }

private:
    template <typename U>
    inline static void deleter (void* mPtr)
    { static_cast<U*> (mPtr)->~U (); }

    inline T invokeStaticFunc (Args... mArgs) const
    { return (*(m_closure.getStaticFunc ()))(std::forward<Args> (mArgs)...); }

    inline void bind (static_fn_type mFuncToBind) noexcept
    { m_closure.bindStaticFunc (this, &Function::invokeStaticFunc, mFuncToBind); }

    template<typename X, typename Object>
    inline void bind (mem_fn_type<X> mFuncToBind, Object* pThis) noexcept
    { m_closure.bindMemFunc (reinterpret_cast<const Object*> (pThis), mFuncToBind); }

private:
    closure_type m_closure;
    storage_type m_storage;
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
struct is_delegate_helper < Function<T> > : std::true_type
{ };

template <typename T>
struct is_delegate : is_delegate_helper<T>
{ };

template <typename T>
using DelegateType = typename
std::enable_if<is_delegate<T>::value, T>::type;

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
