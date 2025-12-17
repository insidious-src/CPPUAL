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

// =========================================================

namespace cppual::memory { namespace { /// optimized for internal usage

// =========================================================

#if defined (OS_STD_UNIX) && !defined (OS_ANDROID)

constexpr int convert_state (state_type eState) noexcept
{
    return eState == state_type::read_only ?
                O_RDONLY | O_EXCL : eState == state_type::read_write ? O_RDWR | O_EXCL : O_EXCL;
}

constexpr int convert_mode (mode_type eMode) noexcept
{
    return eMode == mode_type::create ? O_CREAT : 0;
}

#endif

} //! anonymous namespace

// =========================================================

shared_object::shared_object (string_type const& gName, mode_type eMode, state_type eState)
:
  #if defined (OS_STD_UNIX) && !defined (OS_ANDROID)
  base_type (::shm_open (gName.c_str (), convert_mode (eMode) | convert_state (eState), 0600u))
  #endif
,  _M_gName  (gName )
,  _M_eMode  (eMode )
,  _M_eState (eState)
{ }

shared_object::~shared_object () noexcept
{
#   if defined (OS_STD_UNIX) && !defined (OS_ANDROID)
    if (valid ()) ::shm_unlink (_M_gName.c_str ());
#   endif
}

bool shared_object::truncate (size_type /*mem_size*/) noexcept
{
    return false;
}

// =========================================================

shared_memory::shared_memory (object_type& gObj, size_type uSize, bool bWritable)
:
  #ifdef OS_STD_UNIX
  base_type (::mmap (nullptr,
                     uSize,
                     bWritable ? PROT_READ | PROT_WRITE : PROT_READ,
                     MAP_SHARED, gObj.handle<int> (), 0))
  #elif defined OS_WINDOWS
  #endif
, _M_pObject   (&gObj)
, _M_uSize     (gObj.valid () ? uSize : 0)
, _M_bWritable (bWritable)
{
    if (gObj.valid ()) if (gObj.mode () != mode_type::open) gObj.truncate (uSize);
}

shared_memory::~shared_memory () noexcept
{
#   ifdef OS_STD_UNIX
    if (valid ()) ::munmap (handle<value_type> (), _M_uSize);
#   endif
}

                           // =========================================================

} // namespace Memory

// =========================================================
