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

#include <cppual/concepts.h>
#include <cppual/functional.h>
#include <cppual/containers.h>
#include <cppual/circular_queue.h>
#include <cppual/unbound_matrix.h>
#include <cppual/types.h>

#include <deque>
#include <vector>
#include <shared_mutex>
#include <condition_variable>

namespace cppual { namespace compute {

class host_queue
{
public:
    typedef std::condition_variable_any  cv_type   ;
    typedef function<void()>             call_type ;
    typedef fu16                         size_type ;
    typedef std::shared_timed_mutex      mutex_type;
    typedef std::lock_guard<mutex_type>  write_lock;
    typedef std::shared_lock<mutex_type> read_lock ;
    typedef circular_queue<call_type>     queue_type;

    enum state_type
    {
        interrupted = -1,
        inactive,
        running
    };

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
        read_lock gLock (_M_gQueueMutex);
        return _M_uNumAssigned;
    }

    size_type num_finished () const
    {
        read_lock gLock (_M_gQueueMutex);
        return _M_uNumCompleted;
    }

    state_type state () const
    {
        read_lock gLock (_M_gQueueMutex);
        return _M_eState;
    }

    bool empty () const
    {
        read_lock gLock (_M_gQueueMutex);
        return _M_gTaskQueue.empty ();
    }

    // remove all tasks from the queue
    void clear ()
    {
        write_lock gLock (_M_gQueueMutex);
        _M_gTaskQueue.clear ();
    }

    friend class assign_queue;

private:
    mutable mutex_type _M_gQueueMutex;
    queue_type         _M_gTaskQueue;
    state_type         _M_eState { host_queue::inactive };
    size_type          _M_uNumAssigned { };
    size_type          _M_uNumCompleted { };
    cv_type            _M_gTaskCond;
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

//! host continuation task
template <typename T>
class host_task : private host_queue
{
public:
    static_assert (!std::is_void<T>::value, "T = void ... use std::async instead");

    typedef T value_type;

    void when_any () { when_any_finish (); }
    void when_all () { when_all_finish (); }
    void finish   () { quit (false);     }
    bool valid    () const noexcept { return !assigned (); }

    host_task () : _M_value ()
    { thread_pool::reserve (*this); }

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
        return _M_value;
    }

    template <class X, typename... Args>
    host_task (X* obj, value_type (X::* fn)(Args...), Args&&... args) : host_task ()
    { then (obj, fn, std::forward<Args> (args)...); }

    template <typename... Args>
    host_task (value_type (fn)(Args...), Args&&... args) : host_task ()
    { then (fn, std::forward<Args> (args)...); }

    template <typename Callable, typename... Args>
    host_task (Callable&& fn, Args&&... args) : host_task ()
    { then (std::forward<Callable> (fn), std::forward<Args> (args)...); }

    template <class X, typename... Args>
    host_task& then (X* obj, value_type (X::* fn)(Args...), Args&&... args)
    {
        schedule (call_type ([&]
        {
            _M_value = std::move ((obj->*fn)(std::forward<Args> (args)...));
        }));

        return *this;
    }

    template <typename... Args>
    host_task& then (value_type (fn)(Args...), Args&&... args)
    {
        schedule (call_type ([&]
        {
            _M_value = std::move (fn (std::forward<Args> (args)...));
        }));

        return *this;
    }

    template <typename U, typename... Args>
    host_task& then (U&& closure, Args&&... args)
    {
        schedule (call_type ([&]
        {
            _M_value = std::move
                      (closure.U::operator () (std::forward<Args> (args)...));
        }));

        return *this;
    }

private:
    value_type _M_value;
};

// =========================================================

template <typename T>
class unbound_task : private host_queue
{
public:
    static_assert (!std::is_void<T>::value, "T = void ... use std::async instead");

    typedef T                 value_type ;
    typedef vector<T>        vector_type;
    typedef unbound_matrix<T> matrix_type;

    template <typename F>
    using fn_vector_type = vector<function<F>>;

    void when_any () { when_any_finish (); }
    void when_all () { when_all_finish (); }
    void finish   () { quit (false);     }
    bool valid    () const noexcept { return !assigned (); }

    unbound_task ()
    { thread_pool::reserve (*this); }

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

    inline value_type operator ()()
    {
        when_all ();
        return _M_values;
    }

    template <class X>
    unbound_task (fn_vector_type<value_type (X::*)()> const& functions) : unbound_task ()
    { then (functions); }

    unbound_task (fn_vector_type<value_type (*)()> const& functions) : unbound_task ()
    { then (functions); }

    template <typename Callable>
    unbound_task (fn_vector_type<Callable&&> const& functions) : unbound_task ()
    { then (functions); }

    template <class X>
    unbound_task& then (fn_vector_type<value_type (X::*)()> const& functions)
    {
        schedule (call_type ([&]
        {
            matrix_type matrix;

            _M_values = std::move (matrix.process (functions));
        }));

        return *this;
    }

    unbound_task& then (fn_vector_type<value_type (*)()> const& functions)
    {
        schedule (call_type ([&]
        {
            matrix_type matrix;

            _M_values = std::move (matrix.process (functions));
        }));

        return *this;
    }

    template <typename U>
    unbound_task& then (fn_vector_type<CallableType<U>&&> const& functions)
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

} } // namespace Concurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_TASK
