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
#endif

namespace cppual { namespace Memory {

#if defined (OS_STD_UNIX) && !defined (OS_ANDROID)

inline int flags1 (State eState) noexcept
{
    switch (eState)
    {
    case State::ReadOnly:
        return O_RDONLY | O_EXCL;
    case State::ReadWrite:
        return O_RDWR | O_EXCL;
    default:
        return O_EXCL;
    }
}

constexpr int flags (Mode eMode) noexcept
{
    return eMode == Mode::Create ? O_CREAT : 0;
}

#endif

SharedObject::SharedObject (string const& gName, Mode eMode, State eState)
: m_gName (gName),
  m_eMode (eMode),
  m_eState (eState)
#if defined (OS_STD_UNIX) && !defined (OS_ANDROID)
  , m_nId (::shm_open (gName.c_str (), flags (eMode) | flags1 (eState), 0600u))
#endif
{ }

SharedObject::~SharedObject () noexcept
{
#   if defined (OS_STD_UNIX) && !defined (OS_ANDROID)
    if (m_nId != -1) ::shm_unlink (m_gName.c_str ());
#   endif
}

bool SharedObject::truncate (size_type) noexcept
{
    return false;
}

SharedRegion::SharedRegion (SharedObject& gObj, size_type uSize, bool bWritable)
: m_gObject (gObj),
  m_pRegion (),
  m_uSize (gObj.isValid () ? uSize : 0)
{
    if (gObj.isValid ())
    {
        if (gObj.mode () != Mode::Open) gObj.truncate (uSize);

#       ifdef OS_STD_UNIX
        m_pRegion = ::mmap (nullptr, uSize,
                            bWritable ? PROT_READ | PROT_WRITE : PROT_READ,
                            MAP_SHARED, gObj.id (), 0);
#       elif defined OS_WINDOWS
        bWritable = bWritable;
#       endif
    }
}

SharedRegion::~SharedRegion () noexcept
{
#   ifdef OS_STD_UNIX
    if (m_pRegion) ::munmap (m_pRegion, m_uSize);
#   endif
}

} } // namespace Memory
