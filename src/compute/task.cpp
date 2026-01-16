/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#ifdef DEBUG_MODE
#   include <iostream>
#endif

namespace cppual::compute {

namespace { /// optimize for internal unit usage

struct thread_pool_initializer final
{
    typedef host_queue::mutex_type     mutex_type  ;
    typedef host_queue::write_lock     write_lock  ;
    typedef host_queue::read_lock      read_lock   ;
    typedef std::thread                value_type  ;
    typedef value_type                 thread_type ;
    typedef circular_queue<value_type> thread_queue;

    constexpr thread_pool_initializer ()
    : threadMutex (),
      threads     ()
    {
        threads.reserve (5);
    }

    constexpr ~thread_pool_initializer ()
    {
        /// block thread reservation until all threads exit
        /// and clear the container
        read_lock gLock (threadMutex);

        if (!threads.empty ())
        {
            for (auto& thread : threads) if (thread.joinable ()) thread.join ();
        }
    }

    mutex_type   threadMutex;
    thread_queue threads    ;
};

// =========================================================

constexpr static thread_pool_initializer& pool () noexcept
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
    typedef host_queue::read_lock  read_lock ;
    typedef host_queue::mutex_type mutex_type;

    assign_queue (host_queue& gTasks) : _M_queue (gTasks)
    {
        /// RAII scope
        {
            write_lock lock (_M_queue._M_gQueueMutex);

            _M_queue._M_eState = host_queue::running;

            ++_M_queue._M_uNumAssigned;
        }

        _M_queue._M_gSchedCond.notify_all ();
        _M_queue._M_gTasksCond.notify_all ();
    }

    ~assign_queue ()
    {
        /// RAII scope
        {
            write_lock lock (_M_queue._M_gQueueMutex);

            if (_M_queue._M_uNumAssigned > 0 && (--_M_queue._M_uNumAssigned) == 0)
            {
                _M_queue._M_eState = host_queue::inactive;
            }
        }

        _M_queue._M_gSchedCond.notify_all ();
        _M_queue._M_gTasksCond.notify_all ();
    }

private:
    host_queue& _M_queue;
};

// =========================================================

bool thread_pool::reserve (host_queue& gTaskQueue, size_type uAddThreads, bool bDetached)
{
    if (uAddThreads == 0) return false;

    write_lock lock (pool ().threadMutex);

    /// add threads to the container and initialize them
    while (uAddThreads-- > 0)
    {
        /// add and initialize a thread with host_queue::thread_main ()
        pool ().threads.emplace_back (host_queue::fn_type (gTaskQueue, &host_queue::thread_main));
        if (bDetached) pool ().threads.back ().detach ();
    }

    return true;
}

// =========================================================

host_queue::host_queue (host_queue const&)
{
}

host_queue::host_queue (host_queue&&)
{
}

host_queue::~host_queue ()
{
    quit (true);
    when_all_exit ();
}

host_queue& host_queue::operator = (host_queue const&)
{
    return *this;
}

host_queue& host_queue::operator = (host_queue&&)
{
    return *this;
}

void host_queue::thread_main ()
{
    assign_queue assign (*this);
    fn_type      run           ;

#   ifdef DEBUG_MODE
    std::cout << __FUNCTION__ << " :: thread started..." << std::endl;
#   endif

    while (!is_inactive_or_interrupted ())
    {
        /// RAII scope
        {
            read_lock lock (_M_gQueueMutex);

#           ifdef DEBUG_MODE
            std::cout << __FUNCTION__ << " :: waiting for a task..." << std::endl;
#           endif

            if (_M_eState == running && _M_gTaskQueue.empty ())
            {
                _M_gTasksCond.wait (lock, [this]
                {
#                   ifdef DEBUG_MODE
                    std::cout << __FUNCTION__ << " :: task count: " << _M_gTaskQueue.size () << std::endl;
#                   endif

                    return _M_eState != running || !_M_gTaskQueue.empty ();
                });
            }

#           ifdef DEBUG_MODE
            std::cout << __FUNCTION__ << " :: check for queue state..." << std::endl;
#           endif

            /// process a task if there is one scheduled,
            /// otherwise return if the execution state
            /// requires it

            switch (_M_eState)
            {
            case interrupted:
            case inactive:
#               ifdef DEBUG_MODE
                std::cout << __FUNCTION__ << " :: exit thread..." << std::endl;
#               endif
                continue;
            case canceled:
#               ifdef DEBUG_MODE
                std::cout << __FUNCTION__ << " :: task canceled..." << std::endl;
#               endif
                break;
            case running:
                if (!_M_gTaskQueue.empty ())
                {
                    run = _M_gTaskQueue.front ();

#                   ifdef DEBUG_MODE
                    if (run != nullptr) std::cout << __FUNCTION__ << " :: task scheduled..." << std::endl;
#                   endif
                }
                break;
            }

#           ifdef DEBUG_MODE
            std::cout << __FUNCTION__ << " :: task count: " << _M_gTaskQueue.size () << std::endl;
#           endif
        }

        /// RAII scope
        {
            write_lock lock (_M_gQueueMutex);

            if (run != nullptr) _M_gTaskQueue.pop_front ();
        }

        _M_gTasksCond.notify_all ();

        /// run the aquired task if valid
        if (run != nullptr)
        {
            run ();
            run = nullptr;

            /// RAII scope
            {
                write_lock lock (_M_gQueueMutex);

                if (_M_eState == running) ++_M_uNumCompleted;
            }
        }

        _M_gSchedCond.notify_all ();
        _M_gTasksCond.notify_all ();
    }
}

void host_queue::schedule_wait ()
{
    read_lock lock (_M_gQueueMutex);

    if (_M_eState <= inactive || _M_uNumAssigned == 0)
    {
        _M_gSchedCond.wait (lock, [this] { return _M_uNumAssigned > 0; });
    }
}

void host_queue::schedule_notify ()
{
    _M_gTasksCond.notify_one ();

#   ifdef DEBUG_MODE
    std::cout <<  __FUNCTION__ << " :: notified waiting thread" << std::endl;
#   endif
}

bool host_queue::schedule (fn_const_type& task_fn)
{
    if (task_fn == nullptr) return false;

    schedule_wait ();

    cbool is_scheduled = schedule_push (task_fn);

    if (is_scheduled) schedule_notify ();

    return is_scheduled;
}

bool host_queue::schedule (fn_type&& task_fn)
{
    if (task_fn == nullptr) return false;

    schedule_wait ();

    cbool is_scheduled = schedule_push (task_fn);

    if (is_scheduled) schedule_notify ();

    return is_scheduled;
}

void host_queue::quit (bool bInterrupt) noexcept
{
    if (is_running ())
    {
        write_lock lock (_M_gQueueMutex);

        _M_eState = bInterrupt ? interrupted : inactive;
    }

    /// wake all threads to check the execution state
    _M_gTasksCond.notify_all ();
}

void host_queue::when_any_finish () const
{
    read_lock lock (_M_gQueueMutex);

    _M_gTasksCond.wait (lock, [this, prev_num_completed = _M_uNumCompleted]
    {
        return prev_num_completed < _M_uNumCompleted || _M_uNumAssigned == 0;
    });
}

void host_queue::when_first_finish () const
{
    read_lock lock (_M_gQueueMutex);

    _M_gTasksCond.wait (lock, [this]
    {
        return _M_uNumCompleted >= 1 || _M_uNumAssigned  == 0;
    });
}

void host_queue::when_all_finish () const
{
    read_lock lock (_M_gQueueMutex);

    _M_gTasksCond.wait (lock, [this,
                               prev_num_completed = _M_uNumCompleted,
                               task_count         = _M_gTaskQueue.size ()]
    {
        return _M_uNumAssigned  == 0 ||
             ((_M_uNumCompleted  - prev_num_completed) >= task_count && _M_gTaskQueue.empty ()) ||
              (_M_uNumCompleted >= task_count && _M_gTaskQueue.empty ());
    });
}

void host_queue::when_all_exit () const
{
    read_lock lock (_M_gQueueMutex);

    _M_gTasksCond.wait (lock, [this]
    {
        return _M_uNumAssigned == 0;
    });
}

} // namespace Compute
