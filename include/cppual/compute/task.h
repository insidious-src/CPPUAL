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

#ifndef CPPUAL_PROCESS_TASK
#define CPPUAL_PROCESS_TASK
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/reactive.h>
#include <cppual/functional.h>
#include <cppual/containers.h>
#include <cppual/circular_queue.h>
#include <cppual/unbound_matrix.h>
#include <cppual/memory/allocator.h>
#include <cppual/concept/concepts.h>

//#include <atomic>

#ifdef DEBUG_MODE
#   include <iostream>
#endif

#include <tuple>
#include <future>
#include <optional>
#include <shared_mutex>
#include <condition_variable>

// =========================================================

namespace cppual::compute {

// =========================================================

template <typename T>
using fn_tpl_type = function<T>;

class host_queue;

template <non_void_t>
class host_task;

template <non_void_t>
class unbound_task;

template <non_void_t, typename...>
class unbound_task_adaptor;

using memory::allocator_t;

// =========================================================

class host_queue
{
public:
    typedef host_queue                   self_type    ;
    typedef std::condition_variable_any  cv_type      ;
    typedef fn_tpl_type<void()>          fn_type      ;
    typedef fn_type const                fn_const_type;
    typedef std::size_t                  size_type    ;
    typedef std::shared_mutex            mutex_type   ;
    typedef std::unique_lock<mutex_type> write_lock   ;
    typedef std::shared_lock<mutex_type> read_lock    ;
    typedef circular_queue<fn_type>      queue_type   ;

    enum state_type
    {
        interrupted = -1,
        inactive    =  0,
        canceled    =  1,
        running     =  2
    };

    host_queue (host_queue&&                 );
    host_queue (host_queue const&            );
    host_queue& operator = (host_queue&&     );
    host_queue& operator = (host_queue const&);

    inline host_queue () noexcept = default;
    ~host_queue ();

    bool schedule (fn_const_type& task_fn);
    bool schedule (fn_type&& task_fn);
    void quit     (bool interrupt = false) noexcept;

    /// work thread entry function (main)
    void thread_main ();

    void when_any_finish   () const;
    void when_first_finish () const;
    void when_all_finish   () const;
    void when_all_exit     () const;

    inline size_type num_assigned () const
    {
        read_lock lock (_M_gQueueMutex);
        return _M_uNumAssigned;
    }

    inline size_type num_finished () const
    {
        read_lock lock (_M_gQueueMutex);
        return _M_uNumCompleted;
    }

    inline state_type state () const
    {
        read_lock lock (_M_gQueueMutex);
        return _M_eState;
    }

    inline bool is_running () const
    {
        return state () == running;
    }

    inline bool is_inactive_or_interrupted () const
    {
        return state () <= inactive;
    }

    inline bool is_inactive () const
    {
        return state () == inactive;
    }

    inline bool is_interrupted () const
    {
        return state () == interrupted;
    }

    inline bool is_canceled () const
    {
        return state () == canceled;
    }

    inline bool empty () const
    {
        read_lock lock (_M_gQueueMutex);
        return _M_gTaskQueue.empty ();
    }

    inline void cancel () noexcept
    {
        write_lock lock (_M_gQueueMutex);
        _M_eState = canceled;
    }

    inline void revert_cancellation () noexcept
    {
        write_lock lock (_M_gQueueMutex);
        _M_eState = running;
    }

    /// remove all tasks from the queue
    inline void clear ()
    {
        write_lock lock (_M_gQueueMutex);
        _M_gTaskQueue.clear ();
    }

    friend class assign_queue;

private:
    void schedule_wait   ();
    void schedule_notify ();

    template <typename F,
              typename = std::enable_if_t<std::is_same_v<std::remove_cvref_t<F>, fn_type>>
              >
    FORCEINLINE bool schedule_push (F&& fn)
    {
        queue_type::iterator_pair pair;

        /// RAII scope
        {
            write_lock lock (_M_gQueueMutex);

            pair = _M_gTaskQueue.push_back (std::is_const_v<F> ?
                                            std::forward<fn_const_type> (fn) :
                                            std::move (fn));
        }

        if (pair.second)
        {
#           ifdef DEBUG_MODE
            read_lock lock (_M_gQueueMutex);

            std::cout << __FUNCTION__         << " :: task added. task count: "
                      << _M_gTaskQueue.size() << std::endl;
#           endif
        }
        else
        {
#           ifdef DEBUG_MODE
            std::cerr << __FUNCTION__ << " :: cannot schedule task, queue is not running!"
                      << std::endl;
#           endif

            return false;
        }

        return true;
    }

private:
    mutex_type mutable _M_gQueueMutex                    ;
    queue_type         _M_gTaskQueue                     ;
    size_type          _M_uNumAssigned                { };
    size_type          _M_uNumCompleted               { };
    cv_type    mutable _M_gTasksCond                     ;
    cv_type    mutable _M_gSchedCond                     ;
    state_type         _M_eState { state_type::inactive };

    template <non_void_t>
    friend class host_task;

    template <non_void_t>
    friend class unbound_task;
};

// =========================================================

namespace thread_pool
{
    typedef host_queue::mutex_type mutex_type;
    typedef host_queue::write_lock write_lock;
    typedef host_queue::read_lock  read_lock ;
    typedef host_queue::size_type  size_type ;

    bool reserve (host_queue& task_queue,
                  size_type   reserve_num_threads = 1,
                  bool        detach_threads      = false);
}

// =========================================================

/// host continuation task
template <non_void_t T>
class host_task : private host_queue
{
public:
    typedef host_task<T>              self_type      ;
    typedef host_queue                base_type      ;
    typedef std::remove_cvref_t<T>    value_type     ;
    typedef value_type const          const_value    ;
    typedef value_type&               reference      ;
    typedef value_type const&         const_reference;
    typedef value_type*               pointer        ;
    typedef value_type const*         const_pointer  ;
    typedef std::optional<value_type> result_type    ;
    typedef reactive<value_type>      reactive_type  ;
    typedef std::future <value_type>  future_type    ;
    typedef std::promise<value_type>  promise_type   ;

    inline self_type& when_any () const
    {
        when_any_finish ();
        return const_cast<self_type&> (*this);
    }

    inline self_type& when_first () const
    {
        when_first_finish ();
        return const_cast<self_type&> (*this);
    }

    inline self_type& when_all () const
    {
        when_all_finish ();
        return const_cast<self_type&> (*this);
    }

    inline void finish ()
    {
        quit (false);
    }

    inline host_task () : _M_value ()
    { thread_pool::reserve (*this); }

    inline bool valid () const noexcept
    { return num_assigned () == 1; }

    inline bool ready () const noexcept
    { return state () == running && empty () && num_finished () > 0; }

    inline void reuse ()
    {
        if (!valid ())
        {
            thread_pool::reserve (*this);

            _M_promise = promise_type ();
        }
    }

    inline result_type get () const noexcept
    {
        return ready () ? _M_value : std::nullopt;
    }

    inline value_type operator ()() const noexcept
    {
        when_all ();
        return ready () ? _M_value.get () : value_type ();
    }

    constexpr explicit operator reactive_type& () const noexcept
    {
        return _M_value;
    }

    template <class_t C, typename... Args>
    inline host_task (C& obj, value_type (C::* fn)(Args...), Args&&... args)
    : self_type ()
    { then (obj, fn, std::forward<Args> (args)...); }

    template <class_t C, typename... Args>
    inline host_task (C& obj, value_type (C::* fn)(Args...) const, Args&&... args)
    : self_type ()
    { then (obj, fn, std::forward<Args> (args)...); }

    template <typename... Args>
    inline host_task (value_type (* fn)(Args...), Args&&... args)
    : self_type ()
    { then (fn, std::forward<Args> (args)...); }

    /// non-capture lambda & callable object constructor
    template <lambda_non_capture_t Callable,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline host_task (Callable&& fn,
                      Args&&... args,
                      LambdaNonCaptureType<Callable>* = nullptr)
    : self_type ()
    { then (std::move (fn), std::forward<Args> (args)...); }

    /// capture lambda constructor
    template <lambda_capture_t Callable,
              allocator_t A = memory::allocator<Callable>,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline host_task (Callable&& fn,
                      Args&&... args,
                      A const&  ator = A (),
                      LambdaCaptureType<Callable>* = nullptr)
    : self_type ()
    { then (std::move (fn), ator, std::forward<Args> (args)...); }

    template <class_t C, typename... Args>
    inline self_type& then (C& obj, value_type (C::* fn)(Args...), Args&&... args)
    {
        schedule ([=, this, ... args = std::forward<Args> (args), &obj]
        {
            value_type value (std::move ((obj.*fn)(std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    template <class_t C, typename... Args>
    inline self_type& then (C& obj, value_type (C::* fn)(Args...) const, Args&&... args)
    {
        schedule ([=, this, ... args = std::forward<Args> (args), &obj]
        {
            value_type value (std::move ((obj.*fn)(std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    template <typename... Args>
    inline self_type& then (value_type (* fn)(Args...), Args&&... args)
    {
        schedule ([=, this, ... args = std::forward<Args> (args)]
        {
            value_type value (std::move ((*fn)(std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    /// non-capture lambda & callable object then
    template <lambda_non_capture_t Callable,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline self_type& then (Callable&& fn,
                            Args&&... args,
                            LambdaNonCaptureType<Callable>* = nullptr)
    {
        schedule ([=, this, ... args = std::forward<Args> (args)]
        {
            value_type value (std::move (fn (std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    /// capture lambda then
    template <lambda_capture_t Callable,
              allocator_t A = memory::allocator<Callable>,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline self_type& then (Callable&& fn,
                            Args&&... args,
                            A const& ator = A (),
                            LambdaCaptureType<Callable>* = nullptr)
    {
        schedule ([=, this, ... args = std::forward<Args> (args)]
        {
            value_type value (std::move (fn (std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        },
        ator);

        return *this;
    }

    template <typename Cond,
              typename If  ,
              typename Else,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<bool()>, std::decay_t<Cond>>       ||
                                          std::is_same_v<bool(*)(), Cond>>,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<value_type()>, std::decay_t<If>>   ||
                                          std::is_same_v<value_type(*)(), If>>,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<value_type()>, std::decay_t<Else>> ||
                                          std::is_same_v<value_type(*)(), Else>>
              >
    inline self_type& then_if_else (Cond& cond_fn, If& if_fn, Else& else_fn)
    {
        schedule ([=, this]
        {
            value_type value (std::move (cond_fn () ? if_fn () : else_fn ()));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    future_type get_future () const
    {
        if (ready ())
        {
            _M_promise.set_value (_M_value);
        }
        else
        {
            /// connect reactive member value with promise -> future value.
            /// when reactive member is changed the new value is emitted to promise -> future.
            _M_value << [this](reactive_type::arg_type value)
            {
                _M_promise.set_value (value);
            };
        }

        return std::move (_M_promise.get_future ());
    }

    template <non_void_t, typename...>
    friend class unbound_task_adaptor;

private:
    reactive_type _M_value  ;
    promise_type  _M_promise;
};

// =========================================================

/// host unbound continuation task
template <non_void_t T>
class unbound_task : private host_queue
{
public:
    typedef unbound_task<T>           self_type      ;
    typedef host_queue                base_type      ;
    typedef std::remove_cvref_t<T>    value_type     ;
    typedef value_type const          const_value    ;
    typedef value_type&               reference      ;
    typedef value_type const&         const_reference;
    typedef value_type*               pointer        ;
    typedef value_type const*         const_pointer  ;
    typedef std::optional<value_type> result_type    ;
    typedef reactive<value_type>      reactive_type  ;
    typedef std::future <value_type>  future_type    ;
    typedef std::promise<value_type>  promise_type   ;

    inline self_type& when_any () const
    {
        when_any_finish ();
        return const_cast<self_type&> (*this);
    }

    inline self_type& when_first () const
    {
        when_first_finish ();
        return const_cast<self_type&> (*this);
    }

    inline self_type& when_all () const
    {
        when_all_finish ();
        return const_cast<self_type&> (*this);
    }

    inline void finish ()
    {
        quit (false);
    }

    constexpr unbound_task () noexcept = default;

    inline bool valid () const noexcept
    { return num_assigned () == 1; }

    inline bool ready () const
    { return state () == running && empty () && num_finished () > 0; }

    inline result_type get () const noexcept
    {
        return ready () ? _M_value : std::nullopt;
    }

    inline value_type operator ()() const noexcept
    {
        when_all ();
        return ready () ? _M_value.value_ref () : value_type ();
    }

    constexpr explicit operator reactive_type& () const noexcept
    {
        return _M_value;
    }

    template <class_t C, typename... Args>
    inline unbound_task (C& obj, value_type (C::* fn)(Args...), Args&&... args)
    : self_type ()
    { then (obj, fn, std::forward<Args> (args)...); }

    template <class_t C, typename... Args>
    inline unbound_task (C& obj, value_type (C::* fn)(Args...) const, Args&&... args)
    : self_type ()
    { then (obj, fn, std::forward<Args> (args)...); }

    template <typename... Args>
    inline unbound_task (value_type (* fn)(Args...), Args&&... args)
    : self_type ()
    { then (fn, std::forward<Args> (args)...); }

    /// non-capture lambda & callable object constructor
    template <lambda_non_capture_t Callable,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline unbound_task (Callable&& fn,
                         Args&&... args,
                         LambdaNonCaptureType<Callable>* = nullptr)
    : self_type ()
    { then (std::forward<Callable> (fn), std::forward<Args> (args)...); }

    /// capture lambda constructor
    template <lambda_capture_t Callable,
              allocator_t A = memory::allocator<Callable>,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline unbound_task (Callable&& fn,
                         Args&&... args,
                         A const& ator = A (),
                         LambdaCaptureType<Callable>* = nullptr)
    : self_type ()
    { then (std::move (fn), ator, std::forward<Args> (args)...); }

    template <class_t C, typename... Args>
    inline self_type& then (C& obj, value_type (C::* fn)(Args...), Args&&... args)
    {
        schedule ([=, this, ... args = std::forward<Args> (args), &obj]
        {
            value_type value (std::move ((obj.*fn)(std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    template <class_t C, typename... Args>
    inline self_type& then (C& obj, value_type (C::* fn)(Args...) const, Args&&... args)
    {
        schedule ([=, this, ... args = std::forward<Args> (args), &obj]
        {
            value_type value (std::move ((obj.*fn)(std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    template <typename... Args>
    inline self_type& then (value_type (* fn)(Args...), Args&&... args)
    {
        schedule ([=, this, ... args = std::forward<Args> (args)]
        {
            value_type value (std::move ((*fn)(std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    /// non-capture lambda & callable objects then
    template <lambda_non_capture_t Callable,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline self_type& then (Callable&& fn,
                            Args&&... args,
                            LambdaNonCaptureType<Callable>* = nullptr)
    {
        schedule ([this, fn = std::forward<Callable> (fn), ... args = std::forward<Args> (args)]
        {
            value_type value (std::move (fn (std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    /// capture lambda then
    template <lambda_capture_t Callable,
              allocator_t A = memory::allocator<Callable>,
              typename... Args,
              typename = std::enable_if_t<!std::is_same_v<function<value_type(Args...)>,
                                                          std::decay_t<Callable>>>
              >
    inline self_type& then (Callable&& fn,
                            Args&&... args,
                            A const& ator = A (),
                            LambdaCaptureType<Callable>* = nullptr)
    {
        schedule (
            [this, fn = std::forward<Callable> (fn), ... args = std::forward<Args> (args)]
        {
            value_type value (std::move (fn (std::forward<Args> (args)...)));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        },
        ator);

        return *this;
    }

    template <typename Cond,
              typename If,
              typename Else,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<bool()>, std::decay_t<Cond>>       ||
                                          std::is_same_v<bool(*)(), Cond>>,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<value_type()>, std::decay_t<If>>   ||
                                          std::is_same_v<value_type(*)(), If>>,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<value_type()>, std::decay_t<Else>> ||
                                          std::is_same_v<value_type(*)(), Else>>
              >
    inline self_type& then_if_else (Cond&& cond_fn, If&& if_fn, Else&& else_fn)
    {
        schedule ([=, this]
        {
            value_type value (std::move (cond_fn () ? if_fn () : else_fn ()));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    template <typename Cond,
              typename If  ,
              typename Else,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<bool()>, std::decay_t<Cond>>       ||
                                          std::is_same_v<bool(*)(), Cond>>,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<value_type()>, std::decay_t<If>>   ||
                                          std::is_same_v<value_type(*)(), If>>,
              typename = std::enable_if_t<std::is_same_v<fn_tpl_type<value_type()>, std::decay_t<Else>> ||
                                          std::is_same_v<value_type(*)(), Else>>
              >
    inline self_type& then_if_else (Cond& cond_fn, If& if_fn, Else& else_fn)
    {
        schedule ([=, this]
        {
            value_type value (std::move (cond_fn () ? if_fn () : else_fn ()));

            write_lock lock (_M_gQueueMutex);
            _M_value = std::move (value);
        });

        return *this;
    }

    future_type get_future () const
    {
        if (ready ())
        {
            _M_promise.set_value (_M_value);
        }
        else
        {
            /// connect reactive member value with promise -> future value.
            /// when reactive member is changed the new value is emitted to promise -> future.
            _M_value << [this](reactive_type::arg_type value)
            {
                _M_promise.set_value (value);
            };
        }

        return std::move (_M_promise.get_future ());
    }

    template <non_void_t, typename...>
    friend class unbound_task_adaptor;

private:
    reactive_type _M_value  ;
    promise_type  _M_promise;
};

// =========================================================

/// Use a matrix to calculate multiple results in parallel
/// with only a single thread.
template <non_void_t M, typename... Ts>
class unbound_task_adaptor : private unbound_matrix<M, memory::allocator<M>>
{
public:
    typedef unbound_task_adaptor<M, Ts...>          self_type   ;
    typedef unbound_matrix<M, memory::allocator<M>> base_type   ;
    typedef base_type                               matrix_type ;
    typedef std::size_t                             size_type   ;
    typedef std::tuple<Ts...>                       values_tuple;
    typedef matrix_type::value_type                 value_type  ;

    int matrix_main ();

    inline unbound_task_adaptor ()
    : _M_gThreadTask (*this, &unbound_task_adaptor::matrix_main)
    { }

    constexpr static size_type num_threads () noexcept
    {
        return sizeof... (Ts);
    }

    /// same as num_threads member function
    constexpr static size_type size () noexcept
    {
        return num_threads ();
    }

private:
    host_task<int> _M_gThreadTask;
    values_tuple   _M_values     ;
};

// =========================================================

} // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_PROCESS_TASK
