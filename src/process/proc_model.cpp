/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cstdlib>
#include <stdexcept>
#include <cppual/process/proc_model.h>

#ifdef OS_STD_POSIX
#    include <spawn.h>
#endif

namespace cppual { namespace Process {

process_handle ThisProcess::handle () noexcept
{
#   ifdef OS_STD_POSIX
    return ::getpid ();
#   elif defined (OS_WINDOWS)
    return ::GetCurrentProcess ();
#   endif
}

process_handle create (cchar* path, char* args[])
{
#   ifdef OS_STD_POSIX
    process_handle id = 0;
    ::posix_spawn (&id, path, nullptr, nullptr, args, nullptr);
    return id;
#   elif defined OS_WINDOWS
    PROCESS_INFORMATION info;
    if (!::CreateProcessA (path, *args, nullptr, nullptr, true,
                           0, nullptr, nullptr, nullptr, &info))
        return process_handle ();
    return info.hProcess;
#   endif
}

int terminate (process_handle hProc)
{
    if (ThisProcess::handle () == hProc)
        throw std::logic_error ("using terminate on current process");

#   ifdef OS_STD_POSIX
    ::kill (hProc, SIGKILL);
#   elif defined OS_WINDOWS
    ::TerminateProcess (hProc, 0U);
#   endif

    return 0;
}

bool running (cchar* gName) noexcept
{
#   ifdef OS_STD_POSIX
    char command[32];
    std::sprintf (command, "pgrep %s > /dev/null", gName);
    return 0 == std::system (command);
#   elif defined OS_WINDOWS
    return gName;
#   endif
}

process_handle clone ()
{
#   ifdef OS_STD_POSIX
    return ::fork ();
#   elif defined OS_WINDOWS
    return 0;
#   endif
}

} } // namespace Process
