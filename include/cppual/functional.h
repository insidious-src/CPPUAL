/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <memory>
#include <cstring>
#include <type_traits>
#include <cppual/decl.h>
#include <cppual/concepts.h>

namespace cppual { namespace Internal {

class AnyClass;
using AnyMemFunc = void (Internal::AnyClass::*)();

constexpr std::size_t SingleMemFuncPtrSize
{ sizeof (void (AnyClass::*)()) };

template<class TOut, class TIn>
union HorribleUnion
{ TOut out; TIn in; };

template<class TOut, class TIn>
inline TOut horrible_cast (TIn mIn) noexcept
{
	HorribleUnion<TOut, TIn> u;
	static_assert (sizeof (TIn) == sizeof (u) and sizeof (TIn) == sizeof (TOut),
				   "Cannot use horrible_cast<>");
	u.in = mIn;
	return u.out;
}

template<class TOut, class TIn>
inline TOut unsafe_horrible_cast (TIn mIn) noexcept
{
	HorribleUnion<TOut, TIn> u;
	u.in = mIn;
	return u.out;
}

template <std::size_t N>
struct SimplifyMemFunc
{
	template <class X, class XFuncType, class AnyMemFunc>
	inline static AnyClass* convert (X const*, XFuncType, AnyMemFunc&) noexcept
	{
		static_assert (N - 100,
					   "Unsupported member function pointer on this compiler");
		return nullptr;
	}
};

template <>
struct SimplifyMemFunc <SingleMemFuncPtrSize>
{
	template<class X, class XFuncType, class AnyMemFunc>
	inline
	static
	AnyClass* convert (X const*    mPtrThis,
					   XFuncType   mFuncToBind,
					   AnyMemFunc& mFuncBound) noexcept
	{
		mFuncBound = reinterpret_cast<AnyMemFunc> (mFuncToBind);
		return reinterpret_cast<AnyClass*> (const_cast<X*> (mPtrThis));
	}
};

template <class TAnyMemFunc, class TStaticFunc>
struct ClosurePtr
{
private:
	AnyClass*  ptrThis     { nullptr };
	AnyMemFunc ptrFunction { nullptr };

public:
	constexpr ClosurePtr () noexcept = default;
	constexpr ClosurePtr (std::nullptr_t) noexcept { }

	constexpr ClosurePtr (ClosurePtr const& mClosurePtr) noexcept
	: ptrThis     { mClosurePtr.ptrThis     },
	  ptrFunction { mClosurePtr.ptrFunction }
	{ }

	inline ClosurePtr (ClosurePtr&& mClosurePtr) noexcept
	: ptrThis     { std::move (mClosurePtr.ptrThis)     },
	  ptrFunction { std::move (mClosurePtr.ptrFunction) }
	{ mClosurePtr = nullptr; }

	inline ClosurePtr& operator = (std::nullptr_t) noexcept
	{
		ptrThis     = nullptr;
		ptrFunction = nullptr;
		return *this;
	}

	inline ClosurePtr& operator = (ClosurePtr const& mRhs) noexcept
	{
		ptrThis     = mRhs.ptrThis;
		ptrFunction = mRhs.ptrFunction;
		return *this;
	}

	inline ClosurePtr& operator = (ClosurePtr&& mRhs) noexcept
	{
		ptrThis     = std::move (mRhs.ptrThis);
		ptrFunction = std::move (mRhs.ptrFunction);
		return *this;
	}

	template<class X, class XMemFunc>
	inline void bindMemFunc (X* mPtrThis, XMemFunc mFuncToBind) noexcept
	{
		ptrThis = SimplifyMemFunc<sizeof (mFuncToBind)>::convert
				  (mPtrThis, mFuncToBind, ptrFunction);
	}

	template <class DerivedClass, class ParentInvokerSig>
	inline void bindStaticFunc (ObjectType<DerivedClass>* mPtrParent,
								ParentInvokerSig          mStaticFuncInvoker,
								TStaticFunc               mFuncToBind) noexcept
	{
		static_assert (sizeof (AnyClass*) == sizeof (mFuncToBind),
					   "Cannot use horrible_cast");
		if (mFuncToBind == nullptr) ptrFunction = nullptr;
		else bindMemFunc (mPtrParent, mStaticFuncInvoker);
		ptrThis = horrible_cast<AnyClass*> (mFuncToBind);
	}

	constexpr bool operator == (std::nullptr_t) const noexcept
	{ return ptrThis == nullptr and ptrFunction == nullptr; }

	constexpr bool operator == (ClosurePtr const& mRhs) const noexcept
	{ return ptrThis == mRhs.ptrThis and ptrFunction == mRhs.ptrFunction; }

	constexpr bool operator == (TStaticFunc mPtr) const noexcept
	{
		return mPtr == nullptr ?
					*this == nullptr :
					mPtr == reinterpret_cast<TStaticFunc> (getStaticFunc ());
	}

	constexpr bool operator != (std::nullptr_t) const noexcept
	{ return !operator == (nullptr); }

	constexpr bool operator != (ClosurePtr const& mRhs) const noexcept
	{ return !operator == (mRhs); }

	constexpr bool operator != (TStaticFunc mPtr) const noexcept
	{ return !operator == (mPtr); }

	inline bool operator < (ClosurePtr const& mRhs) const
	{
		return ptrThis != mRhs.ptrThis ?
							  ptrThis < mRhs.ptrThis :
							  std::memcmp (&ptrFunction, &mRhs.ptrFunction,
										   sizeof (ptrFunction)) < 0;
	}

	inline bool operator > (ClosurePtr const& mRhs) const
	{ return mRhs < *this; }

	inline std::size_t hash () const noexcept
	{
		return reinterpret_cast<std::size_t> (ptrThis) ^
				Internal::unsafe_horrible_cast<std::size_t> (ptrFunction);
	}

	inline AnyClass* getPtrThis () const noexcept
	{ return ptrThis; }

	inline TAnyMemFunc getPtrFunction () const noexcept
	{ return reinterpret_cast<TAnyMemFunc> (ptrFunction); }

	inline TStaticFunc getStaticFunc () const noexcept
	{
		static_assert (sizeof (TStaticFunc) == sizeof (this),
					   "Cannot use horrible_cast");
		return horrible_cast<TStaticFunc> (this);
	}
};

template <typename TReturn, typename... TArgs>
class FastFuncImpl
{
protected:
	using GenericMemFn = TReturn (AnyClass::*)(TArgs...);
	using FuncSig      = TReturn (*)(TArgs...);
	using ClosureType  = ClosurePtr<GenericMemFn, FuncSig>;

	ClosureType closure;

private:
	template <typename X> using XFuncToBind = TReturn (X::*)(TArgs...);
	inline TReturn invokeStaticFunc (TArgs&&... mArgs) const
	{ return (*(closure.getStaticFunc ()))(std::forward<TArgs> (mArgs)...); }

	inline void bind (FuncSig mFuncToBind) noexcept
	{ closure.bindStaticFunc (this, &FastFuncImpl::invokeStaticFunc, mFuncToBind); }

	template<typename X, typename Y>
	inline void bind (Y* mPtrThis, XFuncToBind<X> mFuncToBind) noexcept
	{ closure.bindMemFunc (reinterpret_cast<const X*> (mPtrThis), mFuncToBind); }

public:
	constexpr FastFuncImpl () noexcept = default;
	constexpr FastFuncImpl (std::nullptr_t) noexcept { }

	constexpr FastFuncImpl (FastFuncImpl const& mImpl) noexcept
	: closure { mImpl.closure } { }

	inline FastFuncImpl (FastFuncImpl&& mImpl) noexcept
	: closure { std::move (mImpl.closure) } { }

	inline FastFuncImpl (FuncSig mFuncToBind) noexcept
	{ bind (mFuncToBind); }

	template <typename X, typename Y>
	inline FastFuncImpl (XFuncToBind<X> mFuncToBind, Y* mPtrThis) noexcept
	{ bind (mPtrThis, mFuncToBind); }

	inline void operator = (FastFuncImpl const& mImpl) noexcept
	{ closure = mImpl.closure; }

	inline void operator = (FastFuncImpl&& mImpl) noexcept
	{ closure = std::move (mImpl.closure); }

	inline void operator = (FuncSig mFuncToBind) noexcept
	{ bind (mFuncToBind); }

	inline TReturn operator () (TArgs&&... mArgs) const
	{
		return (closure.getPtrThis ()->*(closure.getPtrFunction ()))
				(std::forward<TArgs> (mArgs)...);
	}

	constexpr bool operator == (std::nullptr_t) const noexcept
	{ return closure == nullptr; }

	constexpr bool operator == (FastFuncImpl const& mImpl) const noexcept
	{ return closure == mImpl.closure; }

	constexpr bool operator == (FuncSig mFuncPtr) const noexcept
	{ return closure == mFuncPtr; }

	constexpr bool operator != (std::nullptr_t) const noexcept
	{ return !operator == (nullptr); }

	constexpr bool operator != (FastFuncImpl const& mImpl) const noexcept
	{ return !operator == (mImpl); }

	constexpr bool operator != (FuncSig mFuncPtr) const noexcept
	{ return !operator == (mFuncPtr); }

	constexpr bool operator  < (FastFuncImpl const& mImpl) const
	{ return closure < mImpl.closure; }

	constexpr bool operator  > (FastFuncImpl const& mImpl) const
	{ return closure > mImpl.closure; }
};

} // Internal

template <typename TSignature>
class Function;

template <typename TReturn, typename... TArgs>
class Function <TReturn(TArgs...)> : public Internal::FastFuncImpl<TReturn, TArgs...>
{
private:
	using BaseType = Internal::FastFuncImpl<TReturn, TArgs...>;
	std::shared_ptr<void> storage;

	template<typename T>
	inline static void deleter (void* mPtr)
	{ static_cast<T*> (mPtr)->~T (); operator delete (mPtr); }

public:
	using     BaseType::BaseType;
	constexpr Function () noexcept = default;

	template <typename TFunc,
			  typename =
			  typename std::enable_if
			  <!std::is_same<Function, typename std::decay<TFunc>::type>{}>::type
			  >
	inline Function (TFunc&& mFunc)
	: storage (operator new (sizeof (TFunc)),
			   deleter<typename std::decay<TFunc>::type>)
	{
		using FuncType = typename std::decay<TFunc>::type;
		new (storage.get ()) FuncType (std::forward<TFunc> (mFunc));
		this->closure.bindMemFunc (storage.get (), &FuncType::operator ());
	}
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
