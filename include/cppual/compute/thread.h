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

// =========================================================

namespace cppual::compute {

// =========================================================

typedef enum class thread_priority : u8
{
    inherit  ,
    idle     ,
    lowest   ,
    very_low ,
    low      ,
    normal   ,
    high     ,
    very_high,
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
    typedef thread             self_type    ;
    typedef function<void()>   fn_type      ;
    typedef std::size_t        size_type    ;
    typedef std::atomic_size_t atomic_size_t;
    typedef std::atomic_bool   atomic_bool  ;

    class id final
    {
    public:
        typedef id            self_type  ;
        typedef thread_handle handle_type;

        constexpr id ()                                    noexcept = default;
        constexpr id (self_type &&)                        noexcept = default;
        constexpr id (self_type const&)                    noexcept = default;
        constexpr self_type& operator = (self_type &&)     noexcept = default;
        constexpr self_type& operator = (self_type const&) noexcept = default;

        constexpr id (handle_type h) noexcept
        : _M_handle  (h)
        { }

    private:
        static bool thread_handles_equal (resource_handle h1, resource_handle h2);

        friend
        constexpr bool operator == (self_type, self_type) noexcept;

        friend
        constexpr bool operator  < (self_type, self_type) noexcept;

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
    bool        try_join_for (uint millisec);

    bool start (fn_type&        func,
                bool            joinable   = true,
                thread_priority priority   = thread_priority::inherit,
                size_type       stack_size = 1048576U);

    constexpr size_type stack_size  () const noexcept { return _M_uStackSize;  }
    constexpr id        handle      () const noexcept { return _M_gId;         }
    constexpr bool      is_joinable () const noexcept { return _M_bIsJoinable; }

private:
    id          _M_gId         { };
    size_type   _M_uStackSize  { };
    atomic_bool _M_bIsJoinable { };
};

// =========================================================

constexpr bool operator == (thread::id x, thread::id y) noexcept
{ return thread::id::thread_handles_equal (x._M_handle, y._M_handle); }

constexpr bool operator < (thread::id x, thread::id y) noexcept
{ return x._M_handle < y._M_handle; }

constexpr bool operator != (thread::id x, thread::id y) noexcept
{ return !(x == y); }

constexpr bool operator <= (thread::id x, thread::id y) noexcept
{ return !(y < x); }

constexpr bool operator > (thread::id x, thread::id y) noexcept
{ return y < x; }

constexpr bool operator >= (thread::id x, thread::id y) noexcept
{ return !(x < y); }

} //! namespace compute

#endif // __cplusplus
#endif // CPPUAL_PROCESS_THREAD
