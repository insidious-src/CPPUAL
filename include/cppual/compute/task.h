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

#ifndef CPPUAL_PROCESS_THREAD_POOL
#define CPPUAL_PROCESS_THREAD_POOL
#ifdef __cplusplus

#include <shared_mutex>
#include <condition_variable>
#include <cppual/functional.h>
#include <cppual/circular_queue.h>
#include <cppual/noncopyable.h>
#include <cppual/object.h>
#include <cppual/types.h>

using std::shared_lock;
using std::lock_guard;
using std::condition_variable_any;
using std::shared_timed_mutex;

namespace cppual { namespace Compute {

class SerialQueue : public cppual::ThreadObject
{
public:
	typedef condition_variable_any   cv_type;
	typedef Function<void()>         call_type;
	typedef fu16                     size_type;
	typedef shared_timed_mutex       mutex_type;
	typedef lock_guard<mutex_type>   write_lock;
	typedef shared_lock<mutex_type>  read_lock;
	typedef CircularQueue<call_type> queue_type;

	enum State
	{
		Initial = 0,
		Running,
		Stopped,
		Interrupted,
	};

	SerialQueue (SerialQueue&&);
	SerialQueue (SerialQueue const&);
	SerialQueue& operator = (SerialQueue&&);
	SerialQueue& operator = (SerialQueue const&);

	SerialQueue () noexcept
	: m_gQueueMutex  (),
	  m_gTaskQueue   (),
	  m_eState       (),
	  m_uNumPending  (),
	  m_uNumAssigned (),
	  m_gSchedCond   (),
	  m_gTaskCond    ()
	{ }

	void schedule (call_type&&);
	void quit     (bool interrupt = false) noexcept;

	void whenAnyFinish ();
	void whenAllFinish ();
	void whenAllExit   ();

	size_type assigned () const noexcept
	{
		read_lock gLock (m_gQueueMutex);
		return m_uNumAssigned;
	}

	~SerialQueue ()
	{
		quit (true);
		whenAllExit ();
	}

	size_type pending () const
	{
		read_lock gLock (m_gQueueMutex);
		return m_uNumPending;
	}

	State state () const
	{
		read_lock gLock (m_gQueueMutex);
		return m_eState;
	}

	bool empty () const
	{
		read_lock gLock (m_gQueueMutex);
		return m_gTaskQueue.empty ();
	}

	// remove all tasks from the queue
	void clear ()
	{
		read_lock gLock (m_gQueueMutex);
		m_gTaskQueue.clear ();
	}

	friend class Assigned;
	friend class Pending;
	friend class Worker;

private:
	mutable mutex_type m_gQueueMutex;
	queue_type         m_gTaskQueue;
	State              m_eState;
	u16                m_uNumPending, m_uNumAssigned;
	cv_type            m_gSchedCond,  m_gTaskCond;
};

// =========================================================

struct ThreadPool final : NonConstructible
{
	typedef SerialQueue::mutex_type mutex_type;
	typedef SerialQueue::write_lock write_lock;
	typedef SerialQueue::read_lock  read_lock;
	typedef SerialQueue::size_type  size_type;

	static size_type count (); // count running threads

	static bool reserve (SerialQueue& task_queue,
						 size_type    assign_num_threads = 1,
						 bool         detach_threads     = false);
};

// =========================================================

// continuation task
template <typename T>
class SerialTask : private SerialQueue
{
public:
	SerialTask ()
	{ ThreadPool::reserve (*this); }

	bool ready () const
	{ return state () == SerialQueue::Running and empty () and pending (); }

	void when_any () { whenAnyFinish (); }
	void when_all () { whenAllFinish (); }
	void finish   () { quit (false); }
	bool valid    () const noexcept { return assigned (); }

	bool reuse ()
	{
		if (state () > SerialQueue::Running and !assigned ())
			return ThreadPool::reserve (*this);
		return false;
	}

	inline bool get (T& value)
	{
		if (!ready ()) return false;
		value = std::move (m_value);
		return true;
	}

	inline T operator ()()
	{
		when_all ();
		return m_value;
	}

	template <class X, typename... Args>
	SerialTask (T (X::* fn)(Args...), X* obj, Args&&... args)
	{ then (fn, obj, std::forward<Args> (args)...); }

	template <typename Callable, typename... Args>
	SerialTask (Callable&& fn, Args&&... args)
	{ then (std::forward<Callable> (fn), std::forward<Args> (args)...); }

	template <class X, typename... Args>
	SerialTask& then (T (X::* fn)(Args...), X* obj, Args&&... args)
	{
		schedule ([fn, obj, args..., this]
		{
			m_value = std::move ((obj->*fn)(std::forward<Args> (args)...));
		});
		return *this;
	}

	template <typename Callable, typename... Args>
	SerialTask& then (Callable&& fn, Args&&... args)
	{
		schedule ([fn, args..., this]
		{
			m_value = std::move (fn (std::forward<Args> (args)...));
		});
		return *this;
	}

private:
	T m_value;
};

} } // namespace Concurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_THREAD_POOL
