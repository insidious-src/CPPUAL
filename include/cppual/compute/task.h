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

#ifndef CPPUAL_PROCESS_TASK
#define CPPUAL_PROCESS_TASK
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/concepts.h>
#include <cppual/functional.h>
#include <cppual/containers.h>
#include <cppual/circular_queue.h>
#include <cppual/unbound_matrix.h>

//#include <deque>
//#include <vector>
#include <atomic>
#include <iostream>
#include <shared_mutex>
#include <condition_variable>

// =========================================================

namespace cppual { namespace compute {

// =========================================================

class host_queue
{
public:
    typedef std::condition_variable_any  cv_type   ;
    typedef function<void()>             call_type ;
    typedef fu16                         size_type ;
    typedef std::shared_mutex            mutex_type;
    typedef std::scoped_lock<mutex_type> write_lock;
    typedef std::shared_lock<mutex_type> read_lock ;
    typedef circular_queue  <call_type > queue_type;

    enum state_type
    {
        interrupted = -1,
        inactive    =  0,
        running     =  1
    };

    typedef std::atomic<state_type> atomic_state;

    host_queue (host_queue&&                 );
    host_queue (host_queue const&            );
    host_queue& operator = (host_queue&&     );
    host_queue& operator = (host_queue const&);

    host_queue () noexcept = default;

    bool schedule (call_type const&);
    void quit     (bool interrupt = false) noexcept;
    void operator ()();

    void when_any_finish ();
    void when_all_finish ();
    void when_all_exit   ();

    ~host_queue ()
    {
        quit (true);
        when_all_exit ();
    }

    size_type assigned () const
    {
        read_lock lock (_M_gQueueMutex);
        return _M_uNumAssigned;
    }

    size_type num_finished () const
    {
        read_lock lock (_M_gQueueMutex);
        return _M_uNumCompleted;
    }

    state_type state () const
    {
        return _M_eState;
    }

    bool empty () const
    {
        read_lock lock (_M_gQueueMutex);
        return _M_gTaskQueue.empty ();
    }

    // remove all tasks from the queue
    void clear ()
    {
        write_lock lock (_M_gQueueMutex);
        _M_gTaskQueue.clear ();
    }

    friend class assign_queue;

private:
    mutex_type mutable _M_gQueueMutex;
    queue_type         _M_gTaskQueue;
    atomic_state       _M_eState { state_type::inactive };
    size_type          _M_uNumAssigned { };
    size_type          _M_uNumCompleted { };
    cv_type            _M_gTaskCond;
    cv_type            _M_gSchedCond;

    template <typename>
    friend class host_task;
};

// =========================================================

namespace thread_pool
{
    typedef host_queue::mutex_type mutex_type;
    typedef host_queue::write_lock write_lock;
    typedef host_queue::read_lock  read_lock ;
    typedef host_queue::size_type  size_type ;

    bool reserve (host_queue& task_queue,
                  size_type   assign_num_threads = 1,
                  bool        detach_threads     = false);
}

// =========================================================

/// host continuation task
template <typename T>
class host_task : private host_queue
{
public:
    typedef std::remove_cv_t<T> value_type;

    static_assert (!std::is_void_v<T>, "T = void... use std::async instead");

    void when_any () { when_any_finish (); }
    void when_all () { when_all_finish (); }
    void finish   () { quit       (false); }

    host_task () : _M_value ()
    { thread_pool::reserve (*this); }

    bool valid () const noexcept
    { return !assigned (); }

    bool ready () const
    { return state () == host_queue::running and empty (); }

    void reuse ()
    { if (!valid ()) thread_pool::reserve (*this); }

    inline bool get (value_type& value)
    {
        if (!ready ()) return false;
        value = std::move (_M_value);
        return true;
    }

    inline value_type operator ()()
    {
        when_all ();
        std::cout << "host_task :: All tasks completed, returning value" << std::endl;
        return _M_value;
    }

    template <class C, typename... Args>
    host_task (C* obj, value_type (C::* fn)(Args...), Args&&... args) : host_task ()
    { then (obj, fn, std::forward<Args> (args)...); }

    template <typename... Args>
    host_task (value_type (* fn)(Args...), Args&&... args) : host_task ()
    { then (fn, std::forward<Args> (args)...); }

    template <typename Callable, typename... Args>
    host_task (Callable&& fn, Args&&... args) : host_task ()
    { then (std::forward<Callable> (fn), std::forward<Args> (args)...); }

    template <class C, typename... Args>
    host_task& then (C* obj, value_type (C::* fn)(Args...), Args&&... args)
    {
        schedule (call_type ([this, obj, fn, &args...]
        {
            _M_value = std::move ((obj->*fn)(std::forward<Args> (args)...));
        }));

        return *this;
    }

    template <typename... Args>
    host_task& then (value_type (* fn)(Args...), Args&&... args)
    {
        schedule (call_type ([this, fn, &args...]
        {
            _M_value = std::move (fn (std::forward<Args> (args)...));
        }));

        return *this;
    }

    template <typename Callable, typename... Args>
    host_task& then (Callable&& fn, Args&&... args)
    {
        schedule (call_type ([this, fn, &args...]
        {
            //_M_value = std::move (fn.Callable::operator () (std::forward<Args> (args)...));
            _M_value = std::move (fn (std::forward<Args> (args)...));
            std::cout << "host_task :: Task completed, value: " << _M_value << std::endl;
        }));

        return *this;
    }

private:
    value_type _M_value;
};

// =========================================================

/// host unbound continuation task
///
/// Use a matrix to calculate multiple results in parallel
/// with only a single thread.
template <typename T>
class unbound_task : private host_queue
{
public:
    typedef std::remove_cv_t<T>                 value_type    ;
    typedef vector<value_type>                  vector_type   ;
    typedef unbound_matrix<value_type>          matrix_type   ;
    typedef vector<function<void(value_type&)>> fn_vector_type;

    static_assert (!std::is_void_v<T>, "T = void... use std::async instead");

    void when_any () { when_any_finish (); }
    void when_all () { when_all_finish (); }
    void finish   () { quit       (false); }

    unbound_task ()
    { thread_pool::reserve (*this); }

    bool valid () const noexcept
    { return !assigned (); }

    bool ready () const
    { return state () == host_queue::running and empty (); }

    void reuse ()
    { if (!valid ()) thread_pool::reserve (*this); }

    inline bool get (vector_type& value)
    {
        if (!ready ()) return false;
        value = std::move (_M_values);
        return true;
    }

    inline vector_type operator ()()
    {
        when_all ();
        return _M_values;
    }

    unbound_task (fn_vector_type const& functions) : unbound_task ()
    { then (functions); }

    template <class C>
    unbound_task& then (fn_vector_type const& functions)
    {
        schedule (call_type ([&]
        {
            matrix_type matrix;

            _M_values = std::move (matrix.process (functions));
        }));

        return *this;
    }

private:
    vector_type _M_values;
};

// =========================================================

} } // namespace Concurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_TASK
