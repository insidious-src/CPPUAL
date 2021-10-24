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

#include <cppual/compute/thread.h>
#include <thread>

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

//bool setPosixAttrPriority (pthread_attr_t*, ThreadPriority)
//{
//    return false;
//}

//bool setPosixPriority (thread_handle, ThreadPriority)
//{
//    return false;
//}

enum Result
{
    ResultSuccess,
    ErrorInvalid ,
    ErrorDenied  ,
    ErrorUnknown
};

} // anonymous

// =========================================================

namespace MainThread {

const thread_handle Handle = ThisThread::handle ();
thread_handle handle () noexcept { return MainThread::Handle; }

} // MainThread

ThreadPriority MainThread::priority ()
{
    return ThreadPriority::Normal;
}

int MainThread::setPriority (ThreadPriority ePrio)
{
    UNUSED(ePrio);

#    if defined (OS_STD_POSIX) && !defined (OS_ANDROID)

    int nPrio = 0;

    switch (ePrio)
    {
    case ThreadPriority::Idle:
        nPrio = 0;
        break;
    case ThreadPriority::Lowest:
        nPrio = 1;
        break;
    case ThreadPriority::VeryLow:
        nPrio = 14;
        break;
    case ThreadPriority::Low:
        nPrio = 30;
        break;
    case ThreadPriority::Normal:
        nPrio = 49;
        break;
    case ThreadPriority::High:
        nPrio = 70;
        break;
    case ThreadPriority::VeryHigh:
        nPrio = 84;
        break;
    case ThreadPriority::Highest:
        nPrio = 99;
        break;
    case ThreadPriority::Inherit:
    {
        sched_param gParam;
        pthread_getschedparam (MainThread::Handle, &nPrio, &gParam);
        nPrio = gParam.sched_priority;
    }
        break;
    }

    switch (pthread_setschedprio (MainThread::Handle, nPrio))
    {
    case ENOTSUP: case EINVAL:
        return ErrorInvalid;
    case EPERM:
        return ErrorDenied;
    case 0:
        return ResultSuccess;
    }

#    elif defined (OS_WINDOWS)
#    endif

    return ErrorUnknown;
}

// =========================================================

void ThisThread::exit ()
{
#ifdef OS_WINDOWS
#elif defined (OS_STD_POSIX)
    pthread_exit (nullptr);
#endif
}

int ThisThread::sleepFor (uint uMillisec)
{
#ifdef OS_WINDOWS
    return Sleep (uMillisec);
#elif defined (OS_STD_POSIX)
    return usleep (uMillisec * 1000);
#endif
}

thread_handle ThisThread::handle () noexcept
{
#ifdef OS_WINDOWS
    return GetCurrentThreadId ();
#elif defined (OS_STD_POSIX)
    return pthread_self ();
#endif
}

// =========================================================

bool Thread::Id::threadHandlesEqual (thread_handle pth1, thread_handle pth2)
{
#ifdef OS_WINDOWS
    return (pth1 == pth2);
#elif defined (OS_STD_POSIX)
    return pthread_equal (pth1, pth2);
#endif
}

// =========================================================

static void* bind (void* p)
{
    Thread::callable pFunc = std::move (*static_cast<Thread::callable*> (p));
    pFunc ();
    return nullptr;
}

uint Thread::hardwareConcurency () noexcept
{
    return std::thread::hardware_concurrency ();
}

// =========================================================

Thread::Thread () noexcept
: m_gId (),
  m_uStackSize (),
  m_bIsJoinable ()
{ }

Thread::~Thread ()
{
    if (m_bIsJoinable.load ()) join ();
}

bool Thread::start (callable&      gFunc,
                    bool           bJoinable,
                    ThreadPriority /*ePrio*/,
                    size_type      uStackSize)
{
#    ifdef OS_STD_POSIX

    pthread_attr_t attr { };
    //const sched_param gParam { 49 };
    if (pthread_attr_init (&attr) != 0) return false;

    if (pthread_attr_setstacksize (&attr, uStackSize) != 0 ||
            //ThreadPriority::Inherit == ePrio ? 0 :
            //pthread_attr_setschedparam (&attr, &gParam) != 0 ||
            pthread_attr_setdetachstate (&attr, !bJoinable ?
                                         PTHREAD_CREATE_JOINABLE :
                                         PTHREAD_CREATE_DETACHED) != 0 ||
            pthread_create (&m_gId.m_handle, &attr, &bind, &gFunc) != 0)
    {
        pthread_attr_destroy (&attr);
        return false;
    }

    pthread_attr_destroy (&attr);

#    elif defined (OS_WINDOWS)
#    endif

    m_bIsJoinable.store (bJoinable);
    m_uStackSize = uStackSize;
    if (!bJoinable) detach ();
    return true;
}

void Thread::cancel ()
{
#ifdef OS_WINDOWS
#elif defined (OS_STD_POSIX) && !defined (OS_ANDROID)
    pthread_cancel (m_gId.m_handle);
#endif
}

void Thread::join ()
{
#ifdef OS_WINDOWS
#elif defined (OS_STD_POSIX)
    pthread_join (m_gId.m_handle, nullptr);
#endif
}

void Thread::detach ()
{
    if (m_gId.m_handle and m_bIsJoinable.load ())
    {

#ifdef OS_WINDOWS
#elif defined (OS_STD_POSIX)
    pthread_detach (m_gId.m_handle);
#endif

    m_bIsJoinable.store (false);

    }
}

int Thread::setPriority (ThreadPriority ePrio)
{
    UNUSED(ePrio);

#    if defined (OS_STD_POSIX) && !defined (OS_ANDROID)

    int nPrio = 0;

    switch (ePrio)
    {
    case ThreadPriority::Idle:
        nPrio = 0;
        break;
    case ThreadPriority::Lowest:
        nPrio = 1;
        break;
    case ThreadPriority::VeryLow:
        nPrio = 14;
        break;
    case ThreadPriority::Low:
        nPrio = 30;
        break;
    case ThreadPriority::Normal:
        nPrio = 49;
        break;
    case ThreadPriority::High:
        nPrio = 70;
        break;
    case ThreadPriority::VeryHigh:
        nPrio = 84;
        break;
    case ThreadPriority::Highest:
        nPrio = 99;
        break;
    case ThreadPriority::Inherit:
    {
        sched_param gParam;
        pthread_getschedparam (m_gId.m_handle, &nPrio, &gParam);
        nPrio = gParam.sched_priority;
    }
        break;
    }

    switch (pthread_setschedprio (m_gId.m_handle, nPrio))
    {
    case ENOTSUP: case EINVAL:
        return ErrorInvalid;
    case EPERM:
        return ErrorDenied;
    case 0:
        return ResultSuccess;
    }

#    elif defined (OS_WINDOWS)
#    endif

    return ErrorUnknown;
}

} } // Concurency
