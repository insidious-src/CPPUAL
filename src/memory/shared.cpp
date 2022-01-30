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

#include <cppual/memory/shared.h>
//#include <boost/interprocess/shared_memory_object.hpp>

#ifdef OS_GNU_LINUX
#   include "os/linux.h"
#elif defined (OS_MACX)
#   include "os/mac.h"
#elif defined (OS_AIX)
#   include "os/aix.h"
#elif defined (OS_SOLARIS)
#   include "os/solaris.h"
#elif defined (OS_BSD)
#   include "os/bsd.h"
#elif defined (OS_WINDOWS)
#   include "os/win.h"
#elif defined (OS_ANDROID)
#   include "os/android.h"
#elif defined (OS_IOS)
#   include "os/ios.h"
#endif

namespace cppual { namespace memory {

#if defined (OS_STD_UNIX) && !defined (OS_ANDROID)

constexpr int convert_state (State eState) noexcept
{
    return eState == State::ReadOnly ?
                O_RDONLY | O_EXCL : eState == State::ReadWrite ? O_RDWR | O_EXCL : O_EXCL;
}

constexpr int convert_mode (Mode eMode) noexcept
{
    return eMode == Mode::Create ? O_CREAT : 0;
}

#endif

shared_object::shared_object (string const& gName, Mode eMode, State eState)
: _M_gName (gName),
  _M_eMode (eMode),
  _M_eState (eState)
#if defined (OS_STD_UNIX) && !defined (OS_ANDROID)
  , _M_nId (::shm_open (gName.c_str (), convert_mode (eMode) | convert_state (eState), 0600u))
#endif
{ }

shared_object::~shared_object () noexcept
{
#   if defined (OS_STD_UNIX) && !defined (OS_ANDROID)
    if (_M_nId != -1) ::shm_unlink (_M_gName.c_str ());
#   endif
}

bool shared_object::truncate (size_type) noexcept
{
    return false;
}

shared_memory::shared_memory (shared_object& gObj, size_type uSize, bool bWritable)
: _M_gObject (gObj),
  _M_pRegion (),
  _M_uSize (gObj.valid () ? uSize : 0)
{
    if (gObj.valid ())
    {
        if (gObj.mode () != Mode::Open) gObj.truncate (uSize);

#       ifdef OS_STD_UNIX
        _M_pRegion = ::mmap (nullptr, uSize,
                             bWritable ? PROT_READ | PROT_WRITE : PROT_READ,
                             MAP_SHARED, gObj.id (), 0);
#       elif defined OS_WINDOWS
        bWritable = bWritable;
#       endif
    }
}

shared_memory::~shared_memory () noexcept
{
#   ifdef OS_STD_UNIX
    if (_M_pRegion) ::munmap (_M_pRegion, _M_uSize);
#   endif
}

} } // namespace Memory
