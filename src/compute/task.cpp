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

#include <cppual/compute/task.h>
#include <atomic>
#include <deque>

using std::atomic_uint_fast16_t;
using std::deque;

namespace cppual { namespace Compute {

namespace { namespace Internal { // optimize for internal unit usage

struct ThreadPoolInitializer
{
	typedef atomic_uint_fast16_t    atomic_u16;
	typedef SerialQueue::mutex_type mutex_type;
	typedef SerialQueue::write_lock write_lock;
	typedef SerialQueue::read_lock  read_lock;

	~ThreadPoolInitializer ();

	inline ThreadPoolInitializer () noexcept
	: threadMutex (),
	  threads     (),
	  threadCount ()
	{ }

	mutex_type    threadMutex;
	deque<thread> threads;
	atomic_u16    threadCount;

};

// =========================================================

ThreadPoolInitializer::~ThreadPoolInitializer ()
{
	// block thread reservation until all threads exit
	// and clear the container
	read_lock gLock (threadMutex);

	if (!threads.empty ())
	{
		for (thread& gThread : threads)
			if (gThread.joinable ()) gThread.join ();
	}
}

inline ThreadPoolInitializer& pool () noexcept
{
	static ThreadPoolInitializer thread_pool;
	return thread_pool;
}

} } // anonymous namespace Internal

// =========================================================

class Assigned final
{
public:
	inline Assigned (SerialQueue& gTasks) : m_queue (gTasks)
	{
		++Internal::pool ().threadCount;
		SerialQueue::read_lock gLock (m_queue.m_gQueueMutex);
		++m_queue.m_uNumAssigned;
	}

	inline ~Assigned ()
	{
		// RAII lock
		{
			SerialQueue::read_lock gLock (m_queue.m_gQueueMutex);
			--m_queue.m_uNumAssigned;
		}

		--Internal::pool ().threadCount;
		m_queue.m_gTaskCond.notify_all ();
	}

private:
	SerialQueue& m_queue;
};

// =========================================================

class Pending final
{
public:
	inline Pending (SerialQueue& gTasks) : m_queue (gTasks)
	{ ++m_queue.m_uNumPending; }

	inline ~Pending ()
	{ --m_queue.m_uNumPending; }

private:
	SerialQueue& m_queue;
};

// =========================================================

class Worker final
{
public:
	typedef SerialQueue::call_type  call_type;
	typedef SerialQueue::read_lock  read_lock;
	typedef SerialQueue::write_lock write_lock;

	constexpr Worker (SerialQueue& s)
	: m_queue (s)
	{ }

	void operator ()();

private:
	SerialQueue& m_queue;
};

// =========================================================

void Worker::operator ()()
{
	call_type gTask;
	Assigned  gAssigned (m_queue);

	while (true)
	{
		// RAII lock
		{
			read_lock gLock (m_queue.m_gQueueMutex);
			Pending   gPending (m_queue);

			m_queue.m_gSchedCond.wait (gLock, [&]
			{
				return SerialQueue::Running < m_queue.m_eState or
						!m_queue.m_gTaskQueue.empty ();
			});

			// process a task if there is one scheduled,
			// otherwise return if the execution state
			// requires it
			if (SerialQueue::Interrupted == m_queue.m_eState or
					(m_queue.m_gTaskQueue.empty () and
					 SerialQueue::Stopped == m_queue.m_eState))
				break;

			gTask = m_queue.m_gTaskQueue.front ();
			gLock.unlock ();

			// RAII lock
			{
				write_lock gLock (m_queue.m_gQueueMutex);
				m_queue.m_gTaskQueue.pop_front ();
			}
		}

		if (gTask != nullptr)
		{
			gTask (); // run the aquired task if valid
			m_queue.m_gTaskCond.notify_all ();
		}
	}
}

// =========================================================

ThreadPool::size_type ThreadPool::count ()
{
	return Internal::pool ().threadCount.load ();
}

bool ThreadPool::reserve (SerialQueue& gTaskQueue, size_type uAddThreads, bool bDetached)
{
	if (!uAddThreads) return false;

	// add threads to the container and initialize them
	write_lock gLock (Internal::pool ().threadMutex);

	while (uAddThreads--)
	{
		Internal::pool ().threads.emplace_back (thread (Worker (gTaskQueue)));
		if (bDetached) Internal::pool ().threads.back ().detach ();
	}

	return true;
}

// =========================================================

SerialQueue::SerialQueue (SerialQueue const&)
{
}

SerialQueue::SerialQueue (SerialQueue&&)
{

}

SerialQueue& SerialQueue::operator = (SerialQueue&&)
{
	return *this;
}

SerialQueue& SerialQueue::operator = (SerialQueue const&)
{
	return *this;
}

void SerialQueue::schedule (call_type&& gCallable)
{
	// RAII lock
	{
		write_lock gLock (m_gQueueMutex);
		if (!m_uNumAssigned) return;

		// schedule task
		m_gTaskQueue.push_back (std::forward<call_type> (gCallable));
		m_eState = SerialQueue::Running;
	}

	// wake a thread to receive the task
	m_gSchedCond.notify_one ();
}

void SerialQueue::quit (bool bInterrupt) noexcept
{
	// RAII lock
	{
		read_lock gLock (m_gQueueMutex);

		if (m_eState != SerialQueue::Running) return;
		m_eState = bInterrupt ? SerialQueue::Interrupted : SerialQueue::Stopped;
	}

	// wake all threads to check the execution state
	m_gSchedCond.notify_all ();
	m_gTaskCond.notify_all ();
}

void SerialQueue::whenAnyFinish ()
{
	read_lock gLock (m_gQueueMutex);

	if (m_eState == SerialQueue::Running or m_uNumAssigned > m_uNumPending)
		m_gTaskCond.wait (gLock);
}

void SerialQueue::whenAllFinish ()
{
	read_lock gLock (m_gQueueMutex);

	m_gTaskCond.wait (gLock, [&]
	{
		return m_eState == SerialQueue::Interrupted or
			   m_eState == SerialQueue::Initial     or
			   m_gTaskQueue.empty ();
	});
}

void SerialQueue::whenAllExit ()
{
	read_lock gLock (m_gQueueMutex);

	m_gTaskCond.wait (gLock, [&]
	{
		return !m_uNumAssigned;
	});
}

} } // namespace Concurency
