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

#ifndef CPPUAL_SIGNAL_H_
#define CPPUAL_SIGNAL_H_
#ifdef __cplusplus

#include <cppual/functional.h>
#include <cppual/circular_queue.h>

namespace cppual {

template <typename T,
		  typename = Memory::AllocatorPolicy<Function<T> >
		  >
class Signal;

template <typename T,
		  typename = Memory::AllocatorPolicy<Function<T> >
		  >
class ScopedConnection;

// =========================================================

template <typename TRetType, typename... TArgs, typename Allocator>
class Signal <TRetType(TArgs...), Allocator>
{
public:
	typedef typename Allocator::value_type            value_type;
	typedef typename Allocator::size_type             size_type;
	typedef typename Allocator::reference             reference;
	typedef typename Allocator::const_reference       const_reference;
	typedef AllocatorType<Allocator>                  allocator_type;
	typedef CircularQueue<value_type, allocator_type> container;
	typedef typename container::const_iterator        connection;

	enum { default_size = 10 };

	static bool atomic () noexcept { return false; }
	bool        empty  () const noexcept { return m_gSlots.empty (); }

	// emit signal to connected slots
	void operator () (TArgs... args)
	{ for (reference slot : m_gSlots) slot (std::forward<TArgs> (args)...); }

	Signal () noexcept
	: m_gSlots (default_size)
	{ }

	Signal (allocator_type const& gAlloc) noexcept
	: m_gSlots (default_size, gAlloc)
	{ }

	template <typename Call,
			  typename TRetType_,
			  typename... TArgs_,
			  typename Allocator_
			  >
	friend typename Signal<TRetType_(TArgs_...), Allocator_>::connection
	connect (Signal<TRetType_(TArgs_...), Allocator_>&,
			 Call&&,
			 bool);

	template <typename TClass,
			  typename TRetType_,
			  typename... TArgs_,
			  typename Allocator_
			  >
	friend typename Signal<TRetType_(TArgs_...), Allocator_>::connection
	connect (Signal<TRetType_(TArgs_...), Allocator_>&,
			 ObjectType<TClass>&,
			 TRetType_ (TClass::*)(TArgs_...),
			 bool);

	template <typename TClass,
			  typename TRetType_,
			  typename... TArgs_,
			  typename Allocator_
			  >
	friend void
	disconnect (Signal<TRetType_(TArgs_...), Allocator_>&,
				typename Signal<TRetType_(TArgs_...), Allocator_>::connection&);

private:
	container m_gSlots;
};

// =========================================================

template <typename Call,
		  typename TRetType,
		  typename... TArgs,
		  typename Allocator
		  >
inline
typename Signal<TRetType(TArgs...), Allocator>::connection
connect (Signal<TRetType(TArgs...), Allocator>& gSignal,
		 Call&& gFunc,
		 bool bTop = false)
{
	if (bTop)
	{
		gSignal.m_gSlots.push_front (std::forward<Call> (gFunc));
		return gSignal.m_gSlots.cbegin ();
	}

	gSignal.m_gSlots.push_back (std::forward<Call> (gFunc));
	return gSignal.m_gSlots.cend ();
}

template <typename TClass,
		  typename TRetType,
		  typename... TArgs,
		  typename Allocator
		  >
inline
typename Signal<TRetType(TArgs...), Allocator>::connection
connect (Signal<TRetType(TArgs...), Allocator>& gSignal,
		 ObjectType<TClass>& pObj,
		 TRetType (TClass::* fn)(TArgs...),
		 bool bTop = false)
{
	if (bTop)
	{
		gSignal.m_gSlots.push_front (fn, &pObj);
		return gSignal.m_gSlots.cbegin ();
	}

	gSignal.m_gSlots.push_back (fn, &pObj);
	return gSignal.m_gSlots.cend ();
}

// =========================================================

template <typename TRetType,
		  typename... TArgs,
		  typename Allocator
		  >
inline
void
disconnect (Signal<TRetType(TArgs...), Allocator>& gSignal,
			typename Signal<TRetType(TArgs...), Allocator>::connection& gConn)
{ gSignal.m_gSlots.erase (gConn); }

// =========================================================

template <typename T, typename... TArgs, typename Allocator>
class ScopedConnection <T(TArgs...), Allocator>
{
public:
	typedef Signal<T(TArgs...), Allocator>   signal_type;
	typedef typename signal_type::connection connection;
	typedef typename signal_type::func_type  func_type;

	ScopedConnection () = delete;
	ScopedConnection (ScopedConnection&&) = default;

	inline ~ScopedConnection ()
	{ if (m_fn != nullptr) disconnect (*m_signal, m_fn); }

	inline ScopedConnection (signal_type& gSignal,
							 func_type&&  gFunc,
							 bool         bTop = false)
	: m_signal (&gSignal),
	  m_fn (std::move (connect (gSignal, std::forward<func_type> (gFunc), bTop)))
	{ }

private:
	signal_type* m_signal;
	connection   m_fn;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_SIGNAL_H_
