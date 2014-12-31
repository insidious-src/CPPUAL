/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#include <functional>
#include <shared_mutex>
#include <condition_variable>
#include <cppual/function.h>
#include <cppual/circular_queue.h>
#include <cppual/noncopyable.h>
#include <cppual/object.h>
#include <cppual/types.h>

using std::function;
using std::shared_lock;
using std::lock_guard;
using std::condition_variable_any;
using std::shared_timed_mutex;

namespace cppual { namespace Compute {

class SerialQueue : public cppual::ThreadObject
{
public:
	typedef condition_variable_any   cv_type;
//	typedef function<void()>         call_type;
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

	inline SerialQueue () noexcept
	: m_gQueueMutex (),
	  m_gTaskQueue (),
	  m_eState (),
	  m_uNumPending (),
	  m_uNumAssigned (),
	  m_gSchedCond (),
	  m_gTaskCond ()
	{ }

	void schedule (call_type&&);
	void quit (bool interrupt = false) noexcept;
	void whenAnyFinish ();
	void whenAllFinish ();
	void whenAllExit ();

	inline size_type assigned () const noexcept
	{
		read_lock gLock (m_gQueueMutex);
		return m_uNumAssigned;
	}

	inline ~SerialQueue ()
	{ quit (true); }

	inline size_type pending () const
	{
		read_lock gLock (m_gQueueMutex);
		return m_uNumPending;
	}

	inline State state () const
	{
		read_lock gLock (m_gQueueMutex);
		return m_eState;
	}

	inline bool empty () const
	{
		read_lock gLock (m_gQueueMutex);
		return m_gTaskQueue.empty ();
	}

	// remove all tasks from the queue
	inline void clear ()
	{
		read_lock gLock (m_gQueueMutex);
		m_gTaskQueue.clear ();
	}

private:
	mutable mutex_type m_gQueueMutex;
	queue_type         m_gTaskQueue;
	State              m_eState;
	u16                m_uNumPending, m_uNumAssigned;
	cv_type            m_gSchedCond, m_gTaskCond;

	friend class Assigned;
	friend class Pending;
	friend class Worker;
};

// =========================================================

struct ThreadPool final : NonConstructible
{
	typedef SerialQueue::mutex_type mutex_type;
	typedef SerialQueue::write_lock write_lock;
	typedef SerialQueue::read_lock  read_lock;
	typedef fu16                    size_type;

	static size_type count (); // count running threads

	static bool reserve (SerialQueue& task_queue,
						 size_type    assign_num_threads = 1,
						 bool         detach_threads     = false);
};

// =========================================================

class SerialTaskBase : protected SerialQueue
{
public:
	inline bool ready () const
	{ return state () == SerialQueue::Running and empty () and pending (); }

	inline void when_any () { whenAnyFinish (); }
	inline void when_all () { whenAllFinish (); }
	inline void finish   () { quit (false); }
	inline bool isValid  () const noexcept { return assigned (); }

	inline bool reuse ()
	{
		if (state () > SerialQueue::Running and !assigned ())
			return ThreadPool::reserve (*this);
		return false;
	}

	inline ~SerialTaskBase ()
	{
		quit (true);
		whenAllExit ();
	}

	inline SerialTaskBase ()
	{ ThreadPool::reserve (*this); }
};

// continuation task
template <typename T>
class SerialTask : public SerialTaskBase
{
public:
	inline SerialTask () = default;

	inline bool get (T& value)
	{
		if (!ready ()) return false;
		value = std::move (m_value);
		return true;
	}

	inline T operator ()()
	{
		whenAllFinish ();
		return m_value;
	}

	template <typename Callable>
	inline SerialTask (Callable&& fn) : SerialTaskBase ()
	{ then (std::forward<Callable> (fn)); }

	template <typename Callable>
	inline SerialTask& then (Callable&& fn)
	{
		static_assert (!std::is_member_function_pointer<Callable>::value,
					   "use std::bind for member functions");

		auto newfn = fn;
		schedule ([newfn, this]() { m_value = std::move (newfn ()); });
		return *this;
	}

private:
	T m_value;
};

template <>
class SerialTask <void> : public SerialTaskBase
{
public:
	inline SerialTask () = default;
	inline void operator ()() { whenAllFinish (); }

	template <typename Callable>
	inline SerialTask (Callable&& fn) : SerialTaskBase ()
	{ then (std::forward<Callable> (fn)); }

	template <typename Callable>
	inline SerialTask& then (Callable&& fn)
	{
		static_assert (!std::is_member_function_pointer<Callable>::value,
					   "use std::bind for member functions");

		auto newfn = fn;
		schedule ([newfn, this] { newfn (); });
		return *this;
	}
};

} } // namespace Concurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_THREAD_POOL
