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

#include <cppual/network/transport/tcplistener.h>

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

tcp_listener::tcp_listener()
: transport_socket (socket_type::tcp)
{

}

tcp_listener::tcp_listener (address const& gAddr, u16 uPort) noexcept
: transport_socket (socket_type::tcp)
{
    listen (gAddr, uPort);
}

bool tcp_listener::accept (tcp_stream&) noexcept
{
    if (!valid () or _M_bIsListening) return false;

    //::sockaddr_in gAddr = { 0, 0, { 0 }, { 0 } };
    ::sockaddr_in gAddr = ::sockaddr_in { };
    ::socklen_t   uLen  = sizeof (::sockaddr_in);

    replace_from_id (::accept (handle (),
                               reinterpret_cast<::sockaddr*> (&gAddr),
                               &uLen));
    return true;
}

bool tcp_listener::listen (address const& addr, u16 uPort) noexcept
{
    if (!valid () or _M_bIsListening) return false;

#   ifdef OS_STD_UNIX
    ::sockaddr_in gAddr = { 0, 0, { 0 }, { 0 } };
    gAddr.sin_family  = PF_INET;
    gAddr.sin_port    = ::htons (uPort);

    if (addr.to_string ().size ())
        ::inet_pton (PF_INET, addr.to_string ().c_str (), &gAddr.sin_addr);
    else
        gAddr.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    ::setsockopt (handle (), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));

    if (::bind (handle (), (::sockaddr*) &gAddr, sizeof (gAddr)) == -1 || ::listen (handle (), 0) == -1)
        return _M_bIsListening;
#   endif

    _M_gAddr = addr ;
    _M_uPort = uPort;

    return _M_bIsListening = true;
}

void tcp_listener::start_session(protocol_context&, packet& /*outgoing_packet*/)
{

}

bool tcp_listener::read_data(protocol_context&, packet& /*incoming_packet*/)
{
    return false;
}

byte tcp_listener::try_decode(protocol_context&, packet& /*output_packet*/)
{
    return byte ();
}

byte tcp_listener::encode_content(protocol_context&, packet& /*input_packet*/, packet& /*output_packet*/)
{
    return byte ();
}

} } // namespace Network
