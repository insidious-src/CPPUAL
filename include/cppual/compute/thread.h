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

#ifndef CPPUAL_PROCESS_THREAD
#define CPPUAL_PROCESS_THREAD
#ifdef __cplusplus

#include <cppual/resource.h>
#include <cppual/functional.h>
#include <cppual/noncopyable.h>
#include <cppual/process/details.h>

#include <atomic>
#include <thread>

using std::atomic_size_t;
using std::atomic_bool;

namespace cppual { namespace compute {

enum class thread_priority : byte
{
    inherit = 0,
    idle,
    lowest,
    very_low,
    low,
    normal,
    high,
    very_high,
    highest
};

// =========================================================

namespace main_thread {

resource_handle handle () noexcept;
thread_priority priority ();
int			    set_priority (thread_priority priority);

} // namespace main_thread

// =========================================================

namespace this_thread {

resource_handle handle () noexcept;
void		    exit ();
int			    sleep_for (uint millisec);

} // namespace this_thread

// =========================================================

class thread final : public non_copyable
{
public:
    typedef function<void()> callable ;
    typedef std::size_t      size_type;

    class id final
    {
    public:
        id () noexcept : _M_handle () { }

        explicit
        id (thread_handle __id) : _M_handle (__id) { }

    private:
        friend class  thread  ;
        thread_handle _M_handle;

        static bool thread_handles_equal (resource_handle h1, resource_handle h2);

        friend bool
        operator == (thread::id, thread::id) noexcept;

        friend bool
        operator  < (thread::id, thread::id) noexcept;
    };

    thread () noexcept;
    ~thread ();

    static uint hardware_concurency () noexcept;
    int         set_priority (thread_priority priority);
    void        cancel ();
    void        detach ();
    void        join   ();
    //bool        tryJoinFor (uint millisec);

    bool start (callable&       func,
                bool            joinable   = true,
                thread_priority priority   = thread_priority::inherit,
                size_type       stack_size = 1048576U);

    inline size_type stack_size  () const noexcept { return _M_uStackSize;  }
    inline id        handle      () const noexcept { return _M_gId;         }
    inline bool      is_joinable () const noexcept { return _M_bIsJoinable; }

private:
    id          _M_gId        ;
    size_type   _M_uStackSize ;
    atomic_bool _M_bIsJoinable;
};

// =========================================================

inline bool
operator == (thread::id __x, thread::id __y) noexcept
{ return thread::id::thread_handles_equal (__x._M_handle, __y._M_handle); }

inline bool
operator < (thread::id __x, thread::id __y) noexcept
{ return __x._M_handle < __y._M_handle; }

inline bool
operator!=(thread::id __x, thread::id __y) noexcept
{ return !(__x == __y); }

inline bool
operator<=(thread::id __x, thread::id __y) noexcept
{ return !(__y < __x); }

inline bool
operator>(thread::id __x, thread::id __y) noexcept
{ return __y < __x; }

inline bool
operator>=(thread::id __x, thread::id __y) noexcept
{ return !(__x < __y); }

} } // Concurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_THREAD
