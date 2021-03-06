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

#ifndef CPPUAL_PROCESS_TASK
#define CPPUAL_PROCESS_TASK
#ifdef __cplusplus

#include <cppual/concepts.h>
#include <cppual/functional.h>
#include <cppual/circular_queue.h>
#include <cppual/unbound_matrix.h>
#include <cppual/types.h>

#include <deque>
#include <vector>
#include <shared_mutex>
#include <condition_variable>

using std::shared_lock;
using std::lock_guard;
using std::condition_variable_any;
using std::shared_timed_mutex;

namespace cppual { namespace Compute {

class HostQueue
{
public:
    typedef condition_variable_any   cv_type   ;
    typedef Function<void()>         call_type ;
    typedef fu16                     size_type ;
    typedef shared_timed_mutex       mutex_type;
    typedef lock_guard<mutex_type>   write_lock;
    typedef shared_lock<mutex_type>  read_lock ;
    typedef CircularQueue<call_type> queue_type;

    enum State
    {
        Interrupted = -1,
        Inactive,
        Running
    };

    HostQueue (HostQueue&&                 );
    HostQueue (HostQueue const&            );
    HostQueue& operator = (HostQueue&&     );
    HostQueue& operator = (HostQueue const&);

    HostQueue () noexcept
    : m_gQueueMutex   (),
      m_gTaskQueue    (),
      m_uNumAssigned  (),
      m_uNumCompleted (),
      m_gTaskCond     ()
    { }

    bool schedule (call_type const&);
    void quit     (bool interrupt = false) noexcept;
    void operator ()();

    void whenAnyFinish ();
    void whenAllFinish ();
    void whenAllExit   ();

    ~HostQueue ()
    {
        quit (true);
        whenAllExit ();
    }

    size_type assigned () const
    {
        read_lock gLock (m_gQueueMutex);
        return m_uNumAssigned;
    }

    size_type numFinished () const
    {
        read_lock gLock (m_gQueueMutex);
        return m_uNumCompleted;
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

    friend class Assign;

private:
    mutable mutex_type m_gQueueMutex;
    queue_type         m_gTaskQueue;
    State              m_eState { HostQueue::Inactive };
    size_type          m_uNumAssigned;
    size_type          m_uNumCompleted;
    cv_type            m_gTaskCond;
};

// =========================================================

namespace ThreadPool
{
    typedef HostQueue::mutex_type mutex_type;
    typedef HostQueue::write_lock write_lock;
    typedef HostQueue::read_lock  read_lock ;
    typedef HostQueue::size_type  size_type ;

    bool reserve (HostQueue& task_queue,
                  size_type  assign_num_threads = 1,
                  bool       detach_threads     = false);
}

// =========================================================

// host continuation task
template <typename T>
class HostTask : private HostQueue
{
public:
    static_assert (!std::is_void<T>::value, "T = void ... use std::async instead");

    typedef T value_type;

    void when_any () { whenAnyFinish (); }
    void when_all () { whenAllFinish (); }
    void finish   () { quit (false);     }
    bool valid    () const noexcept { return !assigned (); }

    HostTask ()
    { ThreadPool::reserve (*this); }

    bool ready () const
    { return state () == HostQueue::Running and empty (); }

    void reuse ()
    { if (!valid ()) ThreadPool::reserve (*this); }

    inline bool get (value_type& value)
    {
        if (!ready ()) return false;
        value = std::move (m_value);
        return true;
    }

    inline value_type operator ()()
    {
        when_all ();
        return m_value;
    }

    template <class X, typename... Args>
    HostTask (X* obj, value_type (X::* fn)(Args...), Args&&... args) : HostTask ()
    { then (obj, fn, std::forward<Args> (args)...); }

    template <typename... Args>
    HostTask (value_type (fn)(Args...), Args&&... args) : HostTask ()
    { then (fn, std::forward<Args> (args)...); }

    template <typename Callable, typename... Args>
    HostTask (Callable&& fn, Args&&... args) : HostTask ()
    { then (std::forward<Callable> (fn), std::forward<Args> (args)...); }

    template <class X, typename... Args>
    HostTask& then (X* obj, value_type (X::* fn)(Args...), Args&&... args)
    {
        schedule (call_type ([=]
        {
            m_value = std::move ((obj->*fn)(std::forward<Args> (args)...));
        }));
        return *this;
    }

    template <typename... Args>
    HostTask& then (value_type (fn)(Args...), Args&&... args)
    {
        schedule (call_type ([=]
        {
            m_value = std::move (fn (std::forward<Args> (args)...));
        }));
        return *this;
    }

    template <typename U, typename... Args>
    HostTask& then (U&& closure, Args&&... args)
    {
        schedule (call_type ([=]
        {
            m_value = std::move
                      (closure.U::operator () (std::forward<Args> (args)...));
        }));
        return *this;
    }

private:
    value_type m_value;
};

// =========================================================

template <typename T>
class UnboundTask : private HostQueue
{
public:
    static_assert (!std::is_void<T>::value, "T = void ... use std::async instead");

    typedef T                value_type ;
    typedef std::vector<T>   vector_type;
    typedef UnboundMatrix<T> matrix_type;

    template <typename F>
    using fn_vector_type = std::vector<Function<F>>;

    void when_any () { whenAnyFinish (); }
    void when_all () { whenAllFinish (); }
    void finish   () { quit (false);     }
    bool valid    () const noexcept { return !assigned (); }

    UnboundTask ()
    { ThreadPool::reserve (*this); }

    bool ready () const
    { return state () == HostQueue::Running and empty (); }

    void reuse ()
    { if (!valid ()) ThreadPool::reserve (*this); }

    inline bool get (vector_type& value)
    {
        if (!ready ()) return false;
        value = std::move (m_values);
        return true;
    }

    inline value_type operator ()()
    {
        when_all ();
        return m_values;
    }

    template <class X>
    UnboundTask (fn_vector_type<value_type (X::*)()> const& functions) : UnboundTask ()
    { then (functions); }

    UnboundTask (fn_vector_type<value_type (*)()> const& functions) : UnboundTask ()
    { then (functions); }

    template <typename Callable>
    UnboundTask (fn_vector_type<Callable&&> const& functions) : UnboundTask ()
    { then (functions); }

    template <class X>
    UnboundTask& then (fn_vector_type<value_type (X::*)()> const& functions)
    {
        schedule (call_type ([=]
        {
            matrix_type matrix;

            m_values = std::move (matrix.process (functions));
        }));

        return *this;
    }

    UnboundTask& then (fn_vector_type<value_type (*)()> const& functions)
    {
        schedule (call_type ([=]
        {
            matrix_type matrix;

            m_values = std::move (matrix.process (functions));
        }));

        return *this;
    }

    template <typename U>
    UnboundTask& then (fn_vector_type<CallableType<U>&&> const& functions)
    {
        schedule (call_type ([=]
        {
            matrix_type matrix;

            m_values = std::move (matrix.process (functions));
        }));

        return *this;
    }

private:
    vector_type m_values;
};

} } // namespace Concurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_TASK
