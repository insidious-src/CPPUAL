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

#include <cppual/network/transport/tcpclient.h>

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

namespace cppual { namespace network {

namespace { // internal unit optimization

inline bool resolve_host_name (cchar* hostname, ::in_addr* addr) noexcept
{
#   ifdef OS_STD_UNIX
    ::addrinfo* res;

    if (::getaddrinfo (hostname, nullptr, nullptr, &res) == 0)
    {
        *addr = reinterpret_cast<::sockaddr_in*> (res->ai_addr)->sin_addr;
        ::freeaddrinfo (res);
        return true;
    }
#   endif

    return false;
}

} // anonymous namespace

bool tcp_client::connect (tcp_stream& stream, address const& server, u16 port) noexcept
{
    ::sockaddr_in gAddr;

    gAddr.sin_family = AF_INET;
    gAddr.sin_port   = ::htons (port);

    if (resolve_host_name (server.to_string ().c_str (), &gAddr.sin_addr))
    {
#       ifdef OS_STD_UNIX
        ::inet_pton (PF_INET, server.to_string ().c_str (), &gAddr.sin_addr);
#       endif
    }

    if (stream.valid ())
        ::connect (stream.handle (),
                   reinterpret_cast<::sockaddr*> (&gAddr),
                   sizeof (::sockaddr_in));
    return true;
}

} } // namespace Network
