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

#ifndef CPPUAL_PROCESS_DETAILS
#define CPPUAL_PROCESS_DETAILS
#ifdef __cplusplus

#include <cppual/decl>
#include <cppual/resource>

#ifdef OS_STD_POSIX
#   include <pthread.h>
#   include <unistd.h>
//#   include <errno.h>
//#   include <signal.h>
#elif defined (OS_WINDOWS)
#   include <windows.h>
#endif

namespace cppual::process {

#ifdef OS_STD_POSIX
typedef rc_handle<pid_t>  process_handle;
#elif defined (OS_WINDOWS)
typedef rc_handle<HANDLE> process_handle;
#endif

} //! namespace process

namespace cppual::compute {

#ifdef OS_STD_POSIX
typedef rc_handle<pthread_t>       thread_handle;
typedef rc_handle<pthread_mutex_t> mutex_handle ;
#elif defined (OS_WINDOWS)
typedef rc_handle<DWORD>  thread_handle;
typedef rc_handle<HANDLE> mutex_handle ;
#endif

struct task_traits final
{
#ifdef OS_WINDOWS
    rc_handle<HANDLE>          _M_pEvent;
#elif defined (OS_STD_POSIX)
    rc_handle<pthread_cond_t>  _M_gReady;
    rc_handle<pthread_mutex_t> _M_gLock ;
#endif
};

} //! namespace compute

#endif // __cplusplus
#endif // CPPUAL_PROCESS_DETAILS
