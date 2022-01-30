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

#include <cppual/network/transport/tcp.h>

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

TcpStream::TcpStream () noexcept
: TransportSocket (SocketType::Tcp)
{
}

TcpStream::TcpStream (TcpStream&&) noexcept
: TransportSocket (SocketType::Tcp)
{
}

TcpStream::TcpStream (Address const&, u16) noexcept
: TransportSocket (SocketType::Tcp)
{
}

TcpStream::~TcpStream () noexcept
{
}

void TcpStream::disconnect () noexcept
{
    if (_M_bIsConnected)
    {
        _M_bIsConnected = false;
    }
}

TcpStream& TcpStream::operator << (Packet const&) noexcept // send
{
    if (_M_bIsConnected)
    {
    }

    return *this;
}

TcpStream& TcpStream::operator >> (Packet&) noexcept // receive
{
    if (_M_bIsConnected)
    {
    }

    return *this;
}

} } // namespace Network
