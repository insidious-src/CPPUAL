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

#include <cppual/compute/task.h>
#include <thread>
#include <deque>

using std::thread;
using std::deque;

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

struct ThreadPoolInitializer
{
    typedef HostQueue::mutex_type mutex_type;
    typedef HostQueue::write_lock write_lock;
    typedef HostQueue::read_lock  read_lock ;

    ~ThreadPoolInitializer ();

    ThreadPoolInitializer () noexcept
    : threadMutex (),
      threads     ()
    { }

    mutex_type    threadMutex;
    deque<thread> threads    ;

};

// =========================================================

inline ThreadPoolInitializer::~ThreadPoolInitializer ()
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

} // anonymous namespace

// =========================================================

class Assign final
{
public:
    Assign (HostQueue& gTasks) : m_queue (gTasks)
    {
        m_queue.m_gQueueMutex.lock ();
        ++m_queue.m_uNumAssigned;
        m_queue.m_gQueueMutex.unlock ();
    }

    ~Assign ()
    {
        // isolation brackets
        {
            m_queue.m_gQueueMutex.lock ();
            --m_queue.m_uNumAssigned;
            m_queue.m_gQueueMutex.unlock ();
        }

        m_queue.m_gTaskCond.notify_all ();
    }

private:
    HostQueue& m_queue;
};

// =========================================================

void HostQueue::operator ()()
{
    Assign    assign (*this);
    call_type run;

    while (true)
    {
        // RAII lock
        {
            read_lock gReadLock (m_gQueueMutex);

            m_gTaskCond.wait (gReadLock, [&]
            {
                return Running > m_eState or !m_gTaskQueue.empty ();
            });

            // process a task if there is one scheduled,
            // otherwise return if the execution state
            // requires it
            if (Interrupted == m_eState or
                (m_gTaskQueue.empty () and Inactive == m_eState))
                break;

            run = std::move (m_gTaskQueue.front ());
            gReadLock.unlock ();

            // RAII lock
            {
                write_lock gWriteLock (m_gQueueMutex);
                m_gTaskQueue.pop_front ();
            }
        }

        run (); // run the aquired task if valid

        // RAII lock
        {
            write_lock gWriteLock (m_gQueueMutex);
            ++m_uNumCompleted;
        }

        m_gTaskCond.notify_all ();
    }
}

// =========================================================

bool ThreadPool::reserve (HostQueue& gTaskQueue, size_type uAddThreads, bool bDetached)
{
    if (uAddThreads == 0) return false;


    write_lock gLock (pool ().threadMutex);

    // add threads to the container and initialize them
    while (uAddThreads--)
    {
        // add and initialize a thread with HostQueue::operator ()
        pool ().threads.emplace_back (thread (gTaskQueue));
        if (bDetached) pool ().threads.back ().detach ();
    }

    return true;
}

// =========================================================

HostQueue::HostQueue (HostQueue const&)
{
}

HostQueue::HostQueue (HostQueue&&)
{

}

HostQueue& HostQueue::operator = (HostQueue&&)
{
    return *this;
}

HostQueue& HostQueue::operator = (HostQueue const&)
{
    return *this;
}

bool HostQueue::schedule (call_type const& callable)
{
    // RAII lock
    {
        write_lock gLock (m_gQueueMutex);
        if (m_uNumAssigned == 0) return false;

        // schedule task
        m_gTaskQueue.push_back (callable);
        m_eState = HostQueue::Running;
    }

    // wake a thread to receive the task
    m_gTaskCond.notify_one ();
    return true;
}

void HostQueue::quit (bool bInterrupt) noexcept
{
    // RAII lock
    {
        read_lock gLock (m_gQueueMutex);

        if (m_eState != Running) return;
        m_eState = bInterrupt ? Interrupted : Inactive;
    }

    // wake all threads to check the execution state
    m_gTaskCond.notify_all ();
}

void HostQueue::whenAnyFinish ()
{
    read_lock gLock (m_gQueueMutex);

    m_gTaskCond.wait (gLock, [this, num_completed = m_uNumCompleted]
    {
        return m_uNumAssigned == 0 or num_completed < m_uNumCompleted;
    });
}

void HostQueue::whenAllFinish ()
{
    read_lock gLock (m_gQueueMutex);

    m_gTaskCond.wait (gLock, [this, num_completed = m_uNumCompleted]
    {
        return m_uNumAssigned == 0 or
                (num_completed < m_uNumCompleted and m_gTaskQueue.empty ());
    });
}

void HostQueue::whenAllExit ()
{
    read_lock gLock (m_gQueueMutex);

    m_gTaskCond.wait (gLock, [&]
    {
        return m_uNumAssigned == 0;
    });
}

} } // namespace Concurency
