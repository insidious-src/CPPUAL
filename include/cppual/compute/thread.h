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

#ifndef CPPUAL_PROCESS_THREAD
#define CPPUAL_PROCESS_THREAD
#ifdef __cplusplus

#include <cppual/resource>
#include <cppual/functional>
#include <cppual/noncopyable>
#include <cppual/process/details.h>

#include <atomic>
//#include <thread>

// =========================================================

namespace cppual::compute {

// =========================================================

typedef enum class thread_priority : u8
{
    inherit = 0,
    idle       ,
    lowest     ,
    very_low   ,
    low        ,
    normal     ,
    high       ,
    very_high  ,
    highest
}
const const_thread_priority;

// =========================================================

namespace main_thread {

resource_handle handle       () noexcept;
thread_priority priority     ();
int			    set_priority (thread_priority priority);

} //! namespace main_thread

// =========================================================

namespace this_thread {

resource_handle handle    () noexcept;
void		    exit      ();
int			    sleep_for (uint millisec);

} //! namespace this_thread

// =========================================================

class thread : public non_copyable
{
public:
    typedef thread           self_type;
    typedef function<void()> callable ;
    typedef std::size_t      size_type;

    using atomic_size_t = std::atomic_size_t;
    using atomic_bool   = std::atomic_bool  ;

    class id final
    {
    public:
        typedef id            self_type  ;
        typedef thread_handle handle_type;

        constexpr id () noexcept = default;

        constexpr id (handle_type id) noexcept
        : _M_handle (id)
        { }

    private:
        static bool thread_handles_equal (resource_handle h1, resource_handle h2);

        friend bool
        operator == (self_type, self_type) noexcept;

        friend bool
        operator  < (self_type, self_type) noexcept;

        friend class thread;

    private:
        handle_type _M_handle { };
    };

    thread  () noexcept;
    ~thread ();

    static uint hardware_concurency () noexcept;
    int         set_priority (thread_priority priority);
    void        cancel ();
    void        detach ();
    void        join   ();
    //bool        try_join_for (uint millisec);

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

inline bool operator == (thread::id x, thread::id y) noexcept
{ return thread::id::thread_handles_equal (x._M_handle, y._M_handle); }

inline bool operator < (thread::id x, thread::id y) noexcept
{ return x._M_handle < y._M_handle; }

inline bool operator != (thread::id x, thread::id y) noexcept
{ return !(x == y); }

inline bool operator <= (thread::id x, thread::id y) noexcept
{ return !(y < x); }

inline bool operator > (thread::id x, thread::id y) noexcept
{ return y < x; }

inline bool operator >= (thread::id x, thread::id y) noexcept
{ return !(x < y); }

} //! namespace compute

#endif // __cplusplus
#endif // CPPUAL_PROCESS_THREAD
