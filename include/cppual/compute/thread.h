/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#ifndef CPPUAL_PROCESS_THREAD
#define CPPUAL_PROCESS_THREAD
#ifdef __cplusplus

#include <atomic>
#include <functional>
#include <cppual/noncopyable.h>
#include <cppual/process/details.h>

using std::atomic_size_t;
using std::atomic_bool;

namespace cppual { namespace Compute {

enum class ThreadPriority : char
{
	Inherit = 0,
	Idle,
	Lowest,
	VeryLow,
	Low,
	Normal,
	High,
	VeryHigh,
	Highest
};

// =========================================================

namespace MainThread {

thread_handle  handle () noexcept;
ThreadPriority priority ();
int			   setPriority (ThreadPriority priority);

} // MainThread

// =========================================================

namespace ThisThread {

thread_handle handle () noexcept;
void		  exit ();
int			  sleepFor (uint millisec);

} //ThisThread

// =========================================================

class Thread final : public NonCopyable
{
public:
	typedef std::function<void()> callable;
	typedef std::size_t           size_type;

	class Id final
	{
	public:
		Id () noexcept : m_handle () { }

		explicit
		Id (thread_handle __id) : m_handle (__id) { }

	private:
		friend class  Thread;
		thread_handle m_handle;

		static bool threadHandlesEqual (thread_handle h1,
										thread_handle h2);

		friend bool
		operator == (Thread::Id, Thread::Id) noexcept;

		friend bool
		operator  < (Thread::Id, Thread::Id) noexcept;
	};

	Thread () noexcept;
	~Thread ();

	static uint hardwareConcurency () noexcept;
	int         setPriority (ThreadPriority priority);
	void        cancel ();
	void        detach ();
	void        join ();
	//bool        tryJoinFor (uint millisec);

	bool start (callable&      func,
				bool           joinable   = true,
				ThreadPriority priority   = ThreadPriority::Inherit,
				size_type      stack_size = 1048576U);

	inline size_type stackSize () const noexcept { return m_uStackSize; }
	inline Id        id () const noexcept { return m_gId; }
	inline bool      isJoinable () const noexcept { return m_bIsJoinable; }

private:
	class Id    m_gId;
	size_type   m_uStackSize;
	atomic_bool m_bIsJoinable;
};

// =========================================================

inline bool
operator == (Thread::Id __x, Thread::Id __y) noexcept
{ return Thread::Id::threadHandlesEqual (__x.m_handle, __y.m_handle); }

inline bool
operator < (Thread::Id __x, Thread::Id __y) noexcept
{ return __x.m_handle < __y.m_handle; }

inline bool
operator!=(Thread::Id __x, Thread::Id __y) noexcept
{ return !(__x == __y); }

inline bool
operator<=(Thread::Id __x, Thread::Id __y) noexcept
{ return !(__y < __x); }

inline bool
operator>(Thread::Id __x, Thread::Id __y) noexcept
{ return __y < __x; }

inline bool
operator>=(Thread::Id __x, Thread::Id __y) noexcept
{ return !(__x < __y); }

} } // Concurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_THREAD