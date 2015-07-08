/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <cppual/decl.h>

#ifdef OS_STD_POSIX
    #include <pthread.h>
    #include <unistd.h>
    #include <errno.h>
    #include <signal.h>
#elif defined (OS_WINDOWS)
    #include <Windows.h>
#endif

namespace cppual { namespace Process {

#ifdef OS_STD_POSIX
typedef pid_t  process_handle;
#elif defined (OS_WINDOWS)
typedef HANDLE process_handle;
#endif

} // Process

namespace Compute {

#ifdef OS_STD_POSIX
//typedef pthread_t       thread_id;
typedef pthread_t       thread_handle;
typedef pthread_mutex_t mutex_object;
typedef pid_t           process_handle;
#elif defined (OS_WINDOWS)
//typedef DWORD  thread_id;
typedef HANDLE thread_handle;
typedef HANDLE mutex_object;
typedef HANDLE process_handle;
#endif

struct TaskTraits final
{
#ifdef OS_WINDOWS
    HANDLE          m_pEvent;
#elif defined (OS_STD_POSIX)
    pthread_cond_t  m_gReady;
    pthread_mutex_t m_gLock;
#endif
};

} } // namespace Cuncurency

#endif // __cplusplus
#endif // CPPUAL_PROCESS_DETAILS
