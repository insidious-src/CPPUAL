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

#include <cppual/compute/thread.h>

namespace cppual { namespace compute {

namespace { // optimize for internal unit usage

//bool setPosixAttrPriority (pthread_attr_t*, ThreadPriority)
//{
//    return false;
//}

//bool setPosixPriority (thread_handle, ThreadPriority)
//{
//    return false;
//}

enum result
{
    result_success,
    error_invalid ,
    error_denied  ,
    error_unknown
};

} // anonymous

// =========================================================

namespace main_thread {

static const resource_handle main_thread_handle = this_thread::handle ();

resource_handle handle () noexcept
{
    return main_thread::main_thread_handle;
}

thread_priority priority ()
{
    return thread_priority::normal;
}

int set_priority (thread_priority ePrio)
{
    UNUSED(ePrio);

#    if defined (OS_STD_POSIX) && !defined (OS_ANDROID)

    int nPrio = 0;

    switch (ePrio)
    {
    case thread_priority::idle:
        nPrio = 0;
        break;
    case thread_priority::lowest:
        nPrio = 1;
        break;
    case thread_priority::very_low:
        nPrio = 14;
        break;
    case thread_priority::low:
        nPrio = 30;
        break;
    case thread_priority::normal:
        nPrio = 49;
        break;
    case thread_priority::high:
        nPrio = 70;
        break;
    case thread_priority::very_high:
        nPrio = 84;
        break;
    case thread_priority::highest:
        nPrio = 99;
        break;
    case thread_priority::inherit:
    {
        sched_param gParam;
        ::pthread_getschedparam (main_thread::main_thread_handle, &nPrio, &gParam);
        nPrio = gParam.sched_priority;
    }
        break;
    }

    switch (::pthread_setschedprio (main_thread::main_thread_handle, nPrio))
    {
    case ENOTSUP: case EINVAL:
        return error_invalid;
    case EPERM:
        return error_denied;
    case 0:
        return result_success;
    }

#    elif defined (OS_WINDOWS)
#    endif

    return error_unknown;
}

} // namespace main_thread

// =========================================================

void this_thread::exit ()
{
#ifdef OS_WINDOWS
#elif defined (OS_STD_POSIX)
    ::pthread_exit (nullptr);
#endif
}

int this_thread::sleep_for (uint uMillisec)
{
#ifdef OS_WINDOWS
    ::Sleep (uMillisec);
    return static_cast<int>(uMillisec);
#elif defined (OS_STD_POSIX)
    return ::usleep (uMillisec * 1000);
#endif
}

resource_handle this_thread::handle () noexcept
{
#ifdef OS_WINDOWS
    return ::GetCurrentThreadId ();
#elif defined (OS_STD_POSIX)
    return ::pthread_self ();
#endif
}

// =========================================================

bool thread::id::thread_handles_equal (resource_handle pth1, resource_handle pth2)
{
#ifdef OS_WINDOWS
    return (pth1 == pth2);
#elif defined (OS_STD_POSIX)
    return ::pthread_equal (pth1.get<thread_handle> (), pth2.get<thread_handle> ());
#endif
}

// =========================================================

static void* bind (void* p)
{
    thread::callable pFunc = std::move (*static_cast<thread::callable*> (p));
    pFunc ();
    return nullptr;
}

uint thread::hardware_concurency () noexcept
{
    return std::thread::hardware_concurrency ();
}

// =========================================================

thread::thread () noexcept
: _M_gId (),
  _M_uStackSize (),
  _M_bIsJoinable ()
{ }

thread::~thread ()
{
    if (_M_bIsJoinable.load ()) join ();
}

bool thread::start (callable&       gFunc,
                    bool            bJoinable,
                    thread_priority ePrio,
                    size_type       uStackSize)
{
    UNUSED(ePrio);

#   ifdef OS_STD_POSIX

    ::pthread_attr_t attr { };
    //const ::sched_param gParam { 49 };
    if (::pthread_attr_init (&attr) != 0) return false;

    if (::pthread_attr_setstacksize (&attr, uStackSize) != 0 ||
            //ThreadPriority::Inherit == ePrio ? 0 :
            //::pthread_attr_setschedparam (&attr, &gParam) != 0 ||
            ::pthread_attr_setdetachstate (&attr, !bJoinable ?
                                           PTHREAD_CREATE_JOINABLE :
                                           PTHREAD_CREATE_DETACHED) != 0 ||
            ::pthread_create (&_M_gId._M_handle, &attr, &bind, &gFunc) != 0)
    {
        ::pthread_attr_destroy (&attr);
        return false;
    }

    ::pthread_attr_destroy (&attr);

#   elif defined (OS_WINDOWS)
#   endif

    _M_bIsJoinable = bJoinable ;
    _M_uStackSize  = uStackSize;

    set_priority (ePrio);
    if (!bJoinable) detach ();
    return true;
}

void thread::cancel ()
{
#   ifdef OS_WINDOWS
#   elif defined (OS_STD_POSIX) && !defined (OS_ANDROID)
    ::pthread_cancel (_M_gId._M_handle);
#   endif
}

void thread::join ()
{
#   ifdef OS_WINDOWS
#   elif defined (OS_STD_POSIX)
    ::pthread_join (_M_gId._M_handle, nullptr);
#   endif
}

void thread::detach ()
{
    if (_M_gId._M_handle and _M_bIsJoinable.load ())
    {
#       ifdef OS_WINDOWS
#       elif defined (OS_STD_POSIX)
        ::pthread_detach (_M_gId._M_handle);
#       endif

        _M_bIsJoinable.store (false);
    }
}

int thread::set_priority (thread_priority ePrio)
{
#   if defined (OS_STD_POSIX) && !defined (OS_ANDROID)

    int nPrio = 0;

    switch (ePrio)
    {
    case thread_priority::idle:
        nPrio = 0;
        break;
    case thread_priority::lowest:
        nPrio = 1;
        break;
    case thread_priority::very_low:
        nPrio = 14;
        break;
    case thread_priority::low:
        nPrio = 30;
        break;
    case thread_priority::normal:
        nPrio = 49;
        break;
    case thread_priority::high:
        nPrio = 70;
        break;
    case thread_priority::very_high:
        nPrio = 84;
        break;
    case thread_priority::highest:
        nPrio = 99;
        break;
    default:
    {
        ::sched_param gParam;
        ::pthread_getschedparam (_M_gId._M_handle, &nPrio, &gParam);
        nPrio = gParam.sched_priority;
    }
        break;
    }

    switch (::pthread_setschedprio (_M_gId._M_handle, nPrio))
    {
    case ENOTSUP: case EINVAL:
        return error_invalid;
    case EPERM:
        return error_denied;
    case 0:
        return result_success;
    }

#   elif defined (OS_WINDOWS)
    UNUSED(ePrio);
#   elif defined (OS_ANDROID)
    UNUSED(ePrio);
#   endif

    return error_unknown;
}

} } // Concurency
