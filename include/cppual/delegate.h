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

#ifndef CPPUAL_DELEGATE_H_
#define CPPUAL_DELEGATE_H_
#ifdef __cplusplus

#include <memory>
#include <cppual/decl.h>

namespace cppual {

template <typename> class Delegate;

// =========================================================

// lightweight delegate (doesn't support capturing lambdas)
template<typename T, typename... TArgs>
class Delegate<T (TArgs...)>
{
private:
	typedef T (* FuncType)(cvoid*, TArgs&&...);
	typedef cvoid* Delegate::* SafeBoolType;

	FuncType m_fpFunc;
	cvoid*	 m_pObj;

	template <typename, typename...>		friend class StaticFactory;
	template <class, typename, typename...> friend class MemberFactory;
	template <class, typename, typename...> friend class ConstMemberFactory;
	//template <typename, typename...>		friend class LambdaFactory;

	constexpr Delegate (FuncType fpFunc, cvoid* pObj = nullptr) noexcept
	: m_fpFunc (fpFunc), m_pObj (pObj) { }

public:
	constexpr Delegate () noexcept = default;
	constexpr Delegate (std::nullptr_t) : Delegate () { }
	inline ~Delegate () noexcept { }

	inline bool operator ! () const noexcept
	{ return m_fpFunc == nullptr and m_pObj == nullptr; }

	constexpr Delegate (Delegate const& gObj) noexcept
	: m_fpFunc (gObj.m_fpFunc), m_pObj (gObj.m_pObj) { }

	inline Delegate& operator = (Delegate const& gObj) noexcept
	{ m_pObj = gObj.m_pObj; m_fpFunc = gObj.m_fpFunc; return *this; }

	inline explicit operator SafeBoolType () const noexcept
	{ return m_fpFunc != nullptr and m_pObj != nullptr ? &Delegate::m_pObj : nullptr; }

	inline T operator () (TArgs&&... args) const
	{ return (*m_fpFunc)(m_pObj, std::forward<TArgs> (args)...); }
};

// disable comparison
template <typename T, typename... TArgs>
void operator == (Delegate<T (TArgs...)> const&,
				  Delegate<T (TArgs...)> const&);

template <typename T, typename... TArgs>
void operator != (Delegate<T (TArgs...)> const&,
				  Delegate<T (TArgs...)> const&);

// =========================================================

inline namespace Wrapper {

template <typename T, typename... TArgs>
class StaticFactory final
{
public:
	template <T (* FuncType)(TArgs...)>
	static constexpr Delegate<T (TArgs...)> bind () noexcept
	{
		static_assert (std::is_function
					   <
						 typename std::remove_pointer<decltype (FuncType)>::type
					   >
					   ::value, "FuncType is not a function pointer!");

		return Delegate<T (TArgs...)> (&StaticFactory::doWrap<FuncType>);
	}

private:
	template <T (* FuncType)(TArgs...)>
	static constexpr T doWrap (cvoid*, TArgs&&... args)
	{ return (*FuncType)(args...); }
};

// =========================================================

template <class TClass, typename T, typename... TArgs>
class MemberFactory final
{
public:
	template <T (TClass::* FuncType)(TArgs...)>
	static constexpr Delegate<T (TArgs...)> bind (TClass const& gObj) noexcept
	{
		static_assert (std::is_class<TClass>::value,
					   "the object is not of any class type!");
		static_assert (std::is_member_function_pointer<decltype (FuncType)>::value,
					   "FuncType is not a function pointer!");

		return nullptr != &gObj ?
							  Delegate<T (TArgs...)>
							  (&MemberFactory::doWrap<FuncType>, &gObj) :
							  nullptr;
	}

private:
	template <T (TClass::* FuncType)(TArgs...)>
	static constexpr T doWrap (cvoid* pObj, TArgs&&... args)
	{
		return (const_cast<TClass*>
				(static_cast<TClass const*> (pObj))->*FuncType)
				(args...);
	}
};

// =========================================================

template <class TClass, typename T, typename... TArgs>
class ConstMemberFactory final
{
public:
	template <T (TClass::* FuncType)(TArgs...) const>
	static constexpr Delegate<T (TArgs...)> bind (TClass const& gObj) noexcept
	{
		static_assert (std::is_class<TClass>::value,
					   "the object is not of any class type!");
		static_assert (std::is_member_function_pointer<decltype (FuncType)>::value,
					   "FuncType is not a function pointer!");

		return nullptr != &gObj ?
							  Delegate<T (TArgs...)>
							  (&ConstMemberFactory::doWrap<FuncType>, &gObj) :
							  nullptr;
	}

private:
	template <T (TClass::* FuncType)(TArgs...) const>
	static constexpr T doWrap (cvoid* pObj, TArgs&&... args)
	{ return (static_cast<TClass const*> (pObj)->*FuncType)(args...); }
};

// =========================================================

template <typename T, typename... TArgs>
constexpr StaticFactory<T, TArgs...>
getFactory (T (*)(TArgs...)) noexcept
{ return StaticFactory<T, TArgs...> (); }

template <class TClass, typename T, typename... TArgs>
constexpr MemberFactory<TClass, T, TArgs...>
getFactory (T (TClass::*)(TArgs...)) noexcept
{ return MemberFactory<TClass, T, TArgs...> (); }

template <class TClass, typename T, typename... TArgs>
constexpr ConstMemberFactory<TClass, T, TArgs...>
getFactory (T (TClass::*)(TArgs...) const) noexcept
{ return ConstMemberFactory<TClass, T, TArgs...> (); }

} } // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_DELEGATE_H_
