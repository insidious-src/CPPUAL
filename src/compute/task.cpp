/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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
#include <cppual/circular_queue.h>

#include <thread>
#include <iostream>

namespace cppual { namespace compute {

namespace { // optimize for internal unit usage

struct thread_pool_initializer final
{
    typedef host_queue::mutex_type mutex_type;
    typedef host_queue::write_lock write_lock;
    typedef host_queue::read_lock  read_lock ;

    inline thread_pool_initializer ()
    : threadMutex ( ),
      threads     (5)
    { }

    inline ~thread_pool_initializer ()
    {
        /// block thread reservation until all threads exit
        /// and clear the container
        read_lock gLock (threadMutex);

        if (!threads.empty ())
        {
            for (auto& gThread : threads) if (gThread.joinable ()) gThread.join ();
        }
    }

    mutex_type                  threadMutex;
    circular_queue<std::thread> threads    ;
};

// =========================================================

inline static thread_pool_initializer& pool () noexcept
{
    static thread_pool_initializer thread_pool;
    return thread_pool;
}

} // anonymous namespace

// =========================================================

class assign_queue final
{
public:
    typedef host_queue::write_lock write_lock;

    assign_queue (host_queue& gTasks) : _M_queue (gTasks)
    {
        /// isolation brackets
        {
            write_lock lock (_M_queue._M_gQueueMutex);

            _M_queue._M_eState = host_queue::running;
            ++_M_queue._M_uNumAssigned;
        }

        _M_queue._M_gTaskCond.notify_all ();
    }

    ~assign_queue ()
    {
        /// isolation brackets
        {
            write_lock lock (_M_queue._M_gQueueMutex);

            --_M_queue._M_uNumAssigned;
        }

        _M_queue._M_gTaskCond.notify_all ();
    }

private:
    host_queue& _M_queue;
};

// =========================================================

bool thread_pool::reserve (host_queue& gTaskQueue, size_type uAddThreads, bool bDetached)
{
    if (uAddThreads == 0) return false;

    write_lock gLock (pool ().threadMutex);

    /// add threads to the container and initialize them
    while (uAddThreads--)
    {
        /// add and initialize a thread with host_queue::operator ()
        pool ().threads.emplace_back (std::thread (gTaskQueue));
        if (bDetached) pool ().threads.back ().detach ();
    }

    return true;
}

// =========================================================

void host_queue::operator ()()
{
    assign_queue assign (*this);
    call_type    run;

    while (true)
    {
        /// RAII lock
        {
            read_lock gReadLock (_M_gQueueMutex);

            std::cout << "waiting for a task..." << std::endl;

            if (running == _M_eState and _M_gTaskQueue.empty ())
            {
                _M_gTaskCond.wait (gReadLock, [this]
                {
                    return running > _M_eState or !_M_gTaskQueue.empty ();
                });
            }

            std::cout << "check for queue state..." << std::endl;

            /// process a task if there is one scheduled,
            /// otherwise return if the execution state
            /// requires it
            if (running > _M_eState)
            {
                std::cout << "exit thread..." << std::endl;
                break;
            }

            std::cout << "task scheduled..." << std::endl;

            run = std::move (_M_gTaskQueue.front ());
        }

        /// RAII lock
        {
            write_lock gWriteLock (_M_gQueueMutex);
            _M_gTaskQueue.pop_front ();
        }

        /// run the aquired task if valid
        run ();

        /// RAII lock
        {
            write_lock gWriteLock (_M_gQueueMutex);
            ++_M_uNumCompleted;
        }

        _M_gTaskCond.notify_all ();
    }
}

host_queue::host_queue (host_queue const&)
{
}

host_queue::host_queue (host_queue&&)
{

}

host_queue& host_queue::operator = (host_queue&&)
{
    return *this;
}

host_queue& host_queue::operator = (host_queue const&)
{
    return *this;
}

bool host_queue::schedule (call_type const& callable)
{
    /// RAII lock
    {
        /// isolation brackets
        {
            read_lock gLock (_M_gQueueMutex);

            _M_gTaskCond.wait (gLock, [this]
            {
                return _M_eState == running;
            });
        }

        write_lock gLock (_M_gQueueMutex);

        /// schedule task
        _M_gTaskQueue.push_back (callable);
    }

    /// wake a thread to receive the task
    _M_gTaskCond.notify_one ();
    return true;
}

void host_queue::quit (bool bInterrupt) noexcept
{
    /// RAII lock
    {
        write_lock gLock (_M_gQueueMutex);

        if (_M_eState != running) return;
        _M_eState = bInterrupt ? interrupted : inactive;
    }

    /// wake all threads to check the execution state
    _M_gTaskCond.notify_all ();
}

void host_queue::when_any_finish ()
{
    read_lock gLock (_M_gQueueMutex);

    _M_gTaskCond.wait (gLock, [this, num_completed = _M_uNumCompleted]
    {
        return _M_uNumAssigned == 0 or num_completed < _M_uNumCompleted;
    });
}

void host_queue::when_all_finish ()
{
    read_lock gLock (_M_gQueueMutex);

    _M_gTaskCond.wait (gLock, [this, num_completed = _M_uNumCompleted]
    {
        return _M_uNumAssigned == 0 or
                (num_completed < _M_uNumCompleted and _M_gTaskQueue.empty ());
    });
}

void host_queue::when_all_exit ()
{
    read_lock gLock (_M_gQueueMutex);

    _M_gTaskCond.wait (gLock, [this]
    {
        return _M_uNumAssigned == 0;
    });
}

} } // namespace Compute
