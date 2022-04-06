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

tcp_stream::tcp_stream () noexcept
: transport_socket (socket_type::tcp)
{
}

tcp_stream::tcp_stream (tcp_stream&&) noexcept
: transport_socket (socket_type::tcp)
{
}

tcp_stream::tcp_stream (address const&, u16) noexcept
: transport_socket (socket_type::tcp)
{
}

tcp_stream::~tcp_stream () noexcept
{
}

void tcp_stream::disconnect () noexcept
{
    if (_M_bIsConnected)
    {
        _M_bIsConnected = false;
    }
}

void tcp_stream::start_session(protocol_context&, packet& /*outgoing_packet*/)
{

}

bool tcp_stream::read_data(protocol_context&, packet& /*incoming_packet*/)
{
    return false;
}

byte tcp_stream::try_decode(protocol_context&, packet& /*output_packet*/)
{
    return byte ();
}

byte tcp_stream::encode_content(protocol_context&, packet& /*input_packet*/, packet& /*output_packet*/)
{
    return byte ();
}

tcp_stream& tcp_stream::operator << (packet const&) noexcept // send
{
    if (_M_bIsConnected)
    {
    }

    return *this;
}

tcp_stream& tcp_stream::operator >> (packet&) noexcept // receive
{
    if (_M_bIsConnected)
    {
    }

    return *this;
}

} } // namespace Network
