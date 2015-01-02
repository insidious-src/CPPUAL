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
#include <functional>
#include <type_traits>

namespace cppual { namespace Internal {

class AnyObject;
using AnyMemFunc = void (AnyObject::*)();

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
	inline static AnyObject* convert (X const*, XFuncType, AnyMemFunc&) noexcept
	{
		static_assert (N - 100,
					   "Unsupported member function pointer on this compiler");
		return nullptr;
	}
};

template <>
struct SimplifyMemFunc <sizeof (AnyMemFunc)>
{
	template <class X, class XFuncType, class MemFunc>
	inline
	static
	AnyObject* convert (X const*  mPtrThis,
						XFuncType mFuncToBind,
						MemFunc&  mFuncBound) noexcept
	{
		mFuncBound = reinterpret_cast<MemFunc> (mFuncToBind);
		return reinterpret_cast<AnyObject*> (const_cast<X*> (mPtrThis));
	}
};

template <class TMemFunc, class TStaticFunc>
struct Closure
{
public:
	constexpr Closure () noexcept = default;
	constexpr Closure (std::nullptr_t) noexcept { }

	constexpr Closure (Closure const& mClosure) noexcept
	: m_pObj { mClosure.m_pObj },
	  m_fn   { mClosure.m_fn   }
	{ }

	inline Closure (Closure&& mClosure) noexcept
	: m_pObj { std::move (mClosure.m_pObj) },
	  m_fn   { std::move (mClosure.m_fn)   }
	{ mClosure = nullptr; }

	inline Closure& operator = (std::nullptr_t) noexcept
	{
		m_pObj = nullptr;
		m_fn   = nullptr;
		return *this;
	}

	inline Closure& operator = (Closure const& mRhs) noexcept
	{
		m_pObj = mRhs.m_pObj;
		m_fn   = mRhs.m_fn;
		return *this;
	}

	inline Closure& operator = (Closure&& mRhs) noexcept
	{
		m_pObj = std::move (mRhs.m_pObj);
		m_fn   = std::move (mRhs.m_fn);
		return *this;
	}

	template <class X, class XMemFunc>
	inline void bindMemFunc (X* mPtrThis, XMemFunc mFuncToBind) noexcept
	{
		m_pObj = SimplifyMemFunc<sizeof (mFuncToBind)>::convert
				  (mPtrThis, mFuncToBind, m_fn);
	}

	template <class DerivedClass, class ParentInvokerSig>
	inline void bindStaticFunc (DerivedClass*    mPtrParent,
								ParentInvokerSig mStaticFuncInvoker,
								TStaticFunc      mFuncToBind) noexcept
	{
		static_assert (sizeof (AnyObject*) == sizeof (mFuncToBind),
					   "Cannot use horrible_cast");
		if (mFuncToBind == nullptr) m_fn = nullptr;
		else bindMemFunc (mPtrParent, mStaticFuncInvoker);
		m_pObj = horrible_cast<AnyObject*> (mFuncToBind);
	}

	constexpr bool operator == (std::nullptr_t) const noexcept
	{ return m_pObj == nullptr and m_fn == nullptr; }

	constexpr bool operator == (Closure const& mRhs) const noexcept
	{ return m_pObj == mRhs.m_pObj and m_fn == mRhs.m_fn; }

	constexpr bool operator == (TStaticFunc mPtr) const noexcept
	{
		return mPtr == nullptr ?
					*this == nullptr :
					mPtr  == reinterpret_cast<TStaticFunc> (getStaticFunc ());
	}

	constexpr bool operator != (std::nullptr_t) const noexcept
	{ return !operator == (nullptr); }

	constexpr bool operator != (Closure const& mRhs) const noexcept
	{ return !operator == (mRhs); }

	constexpr bool operator != (TStaticFunc mPtr) const noexcept
	{ return !operator == (mPtr); }

	inline bool operator < (Closure const& mRhs) const
	{
		return m_pObj != mRhs.m_pObj ?
							  m_pObj < mRhs.m_pObj :
							  std::memcmp (&m_fn, &mRhs.m_fn, sizeof (m_fn)) < 0;
	}

	inline bool operator > (Closure const& mRhs) const
	{ return mRhs < *this; }

	inline std::size_t hash () const noexcept
	{
		return reinterpret_cast<std::size_t> (m_pObj) ^
				Internal::unsafe_horrible_cast<std::size_t> (m_fn);
	}

	inline AnyObject* object () const noexcept
	{ return m_pObj; }

	inline TMemFunc getPtrFunction () const noexcept
	{ return reinterpret_cast<TMemFunc> (m_fn); }

	inline TStaticFunc getStaticFunc () const noexcept
	{
		static_assert (sizeof (TStaticFunc) == sizeof (this),
					   "Cannot use horrible_cast");
		return horrible_cast<TStaticFunc> (this);
	}

private:
	AnyObject* m_pObj { nullptr };
	AnyMemFunc m_fn   { nullptr };
};

template <typename TReturn, typename... TArgs>
class FunctionImpl
{
protected:
	using GenericMemFn = TReturn (AnyObject::*)(TArgs...);
	using FuncSig      = TReturn (*)(TArgs...);
	using ClosureType  = Closure<GenericMemFn, FuncSig>;

	ClosureType closure;

private:
	template <typename X>
	using MemFuncToBind = TReturn (X::*)(TArgs...);

	inline TReturn invokeStaticFunc (TArgs&&... mArgs) const
	{ return (*(closure.getStaticFunc ()))(std::forward<TArgs> (mArgs)...); }

	inline void bind (FuncSig mFuncToBind) noexcept
	{ closure.bindStaticFunc (this, &FunctionImpl::invokeStaticFunc, mFuncToBind); }

	template<typename X, typename Y>
	inline void bind (Y* mPtrThis, MemFuncToBind<X> mFuncToBind) noexcept
	{ closure.bindMemFunc (reinterpret_cast<const X*> (mPtrThis), mFuncToBind); }

public:
	constexpr FunctionImpl () noexcept = default;
	constexpr FunctionImpl (std::nullptr_t) noexcept { }

	constexpr FunctionImpl (FunctionImpl const& mImpl) noexcept
	: closure { mImpl.closure } { }

	inline FunctionImpl (FunctionImpl&& mImpl) noexcept
	: closure { std::move (mImpl.closure) } { }

	// static function constructor
	inline FunctionImpl (FuncSig mFuncToBind) noexcept
	{ bind (mFuncToBind); }

	// member function constructor
	template <typename X, typename Y>
	inline FunctionImpl (MemFuncToBind<X> mFuncToBind, Y* mPtrThis) noexcept
	{ bind (mPtrThis, mFuncToBind); }

	inline void operator = (FunctionImpl const& mImpl) noexcept
	{ closure = mImpl.closure; }

	inline void operator = (FunctionImpl&& mImpl) noexcept
	{ closure = std::move (mImpl.closure); }

	inline void operator = (FuncSig mFuncToBind) noexcept
	{ bind (mFuncToBind); }

	inline TReturn operator () (TArgs... mArgs) const
	{
#		ifdef DEBUG_MODE
		if (closure == nullptr) throw std::bad_function_call ();
#		endif
		return (closure.object ()->*(closure.getPtrFunction ()))
				(std::forward<TArgs> (mArgs)...);
	}

	constexpr bool operator == (std::nullptr_t) const noexcept
	{ return closure == nullptr; }

	constexpr bool operator == (FunctionImpl const& mImpl) const noexcept
	{ return closure == mImpl.closure; }

	constexpr bool operator == (FuncSig mFuncPtr) const noexcept
	{ return closure == mFuncPtr; }

	constexpr bool operator != (std::nullptr_t) const noexcept
	{ return !operator == (nullptr); }

	constexpr bool operator != (FunctionImpl const& mImpl) const noexcept
	{ return !operator == (mImpl); }

	constexpr bool operator != (FuncSig mFuncPtr) const noexcept
	{ return !operator == (mFuncPtr); }

	constexpr bool operator  < (FunctionImpl const& mImpl) const
	{ return closure < mImpl.closure; }

	constexpr bool operator  > (FunctionImpl const& mImpl) const
	{ return closure > mImpl.closure; }
};

} // Internal

template <typename TSignature>
class Function;

template <typename TReturn, typename... TArgs>
class Function <TReturn(TArgs...)> : public Internal::FunctionImpl<TReturn, TArgs...>
{
private:
	using BaseType = Internal::FunctionImpl<TReturn, TArgs...>;
	std::shared_ptr<void> storage;

	template<typename T>
	inline static void deleter (void* mPtr)
	{ static_cast<T*> (mPtr)->~T (); operator delete (mPtr); }

public:
	using     BaseType::BaseType;
	constexpr Function () noexcept = default;

	// lambda constructor
	template <typename TFunc>
	inline Function (TFunc&& mFunc)
	: storage (operator new (sizeof (TFunc)),
			   deleter<typename std::decay<TFunc>::type>)
	{
		using FuncType = typename std::decay<TFunc>::type;

		static_assert (!std::is_same<Function, FuncType>::value,
					   "function signature is the same as Function class");

		new (storage.get ()) FuncType (std::forward<TFunc> (mFunc));
		this->closure.bindMemFunc (storage.get (), &FuncType::operator ());
	}
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_FAST_FUNC_H_
