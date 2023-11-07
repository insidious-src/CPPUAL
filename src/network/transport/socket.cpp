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

#include <cppual/network/transport/socket.h>
#include <cppual/flags.h>

#include <cstring>
#include <iostream>

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

// ====================================================

enum class socket_flag : int
{
#   ifdef OS_STD_UNIX
    non_blocking = O_NONBLOCK,
#   endif
    no_delay     = TCP_NODELAY,
    broadcast    = SO_BROADCAST,
#   ifdef OS_MACX
    no_sig_pipe  = SO_NOSIGPIPE
#   endif
};

typedef bitset<socket_flag> socket_flags;

// ====================================================

transport_socket::transport_socket (socket_type eProt) noexcept
: resource     (create_socket (eProt)),
  _M_eProtocol (eProt)
{
    init_socket ();
}

transport_socket::transport_socket (transport_socket&& gObj) noexcept
: resource     (std::move (gObj)),
  _M_eProtocol (gObj._M_eProtocol)
{
}

transport_socket::~transport_socket () noexcept
{
    close ();
}

transport_socket& transport_socket::operator = (transport_socket&& gObj) noexcept
{
    if (this == gObj) return *this;

    resource::operator = (std::move (gObj));

    _M_eProtocol = gObj._M_eProtocol;

    return *this;
}

void transport_socket::set_blocking (bool bBlock) noexcept
{
    if (valid ())
    {
#       ifdef OS_STD_UNIX
        socket_flags status_flags = ::fcntl (handle (), F_GETFL);

        if (bBlock) status_flags -= socket_flag::non_blocking;
        else        status_flags += socket_flag::non_blocking;

        ::fcntl (handle (), F_SETFL, status_flags);
#       endif
    }
}

bool transport_socket::is_blocking () const noexcept
{
    if (valid ())
    {
#       ifdef OS_STD_UNIX
        socket_flags status_flags = ::fcntl (handle (), F_GETFL);

        return !status_flags.test (socket_flag::non_blocking);
#       endif
    }

    return true;
}

transport_socket::socket_id transport_socket::create_socket (socket_type eProt) noexcept
{
    return ::socket (PF_INET,
                     eProt == socket_type::tcp ? SOCK_STREAM : SOCK_DGRAM,
                     0);
}

void transport_socket::replace_from_id (socket_id nId) noexcept
{
    close ();
    set_handle (nId);
}

void transport_socket::close () noexcept
{
#   ifdef OS_STD_UNIX
    if (valid ()) ::close (handle ());
#   elif defined OS_WINDOWS
    if (valid ()) ::closesocket (handle ());
#   endif

    set_handle (null_value);
}

void transport_socket::init_socket () noexcept
{
    if (valid ())
    {
        int yes = 1;

        if (_M_eProtocol == socket_type::tcp)
        {
#           ifdef OS_STD_UNIX
            /// Disable the Nagle algorithm (ie. removes buffering of TCP packets)
            if (::setsockopt (handle (),
                              IPPROTO_TCP,
                              int(socket_flag::no_delay),
                              reinterpret_cast<char*> (&yes),
                              static_cast<::socklen_t> (sizeof (int))) == -1)
            {
                std::cerr << "Failed to set socket option 'TCP_NODELAY'. "
                             "All your TCP packets will be buffered!" << std::endl;
            }
#           elif defined OS_MACX
            /// On Mac OS X, disable the SIGPIPE signal on disconnection
            if (::setsockopt (handle (),
                              SOL_SOCKET,
                              int(socket_flag::no_sig_pipe),
                              reinterpret_cast<char*>(&yes),
                              static_cast<::socklen_t> (sizeof (int))) == -1)
                std::cerr << "Failed to set socket option 'SO_NOSIGPIPE'" << std::endl;
#           endif
        }
        else
        {
            /// Enable broadcast by default for UDP sockets
            if (::setsockopt (handle (),
                              SOL_SOCKET,
                              int(socket_flag::broadcast),
                              reinterpret_cast<char*> (&yes),
                              static_cast<::socklen_t> (sizeof (int))) == -1)
                std::cerr << "Failed to enable broadcast on UDP socket" << std::endl;
        }
    }
}

} } // namespace Network
