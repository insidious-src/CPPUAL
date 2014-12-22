/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#ifdef OS_STD_UNIX
//#	include <netinet/in.h>
#	include <netdb.h>
#	include <arpa/inet.h>
#endif

namespace cppual { namespace Network {

bool resolveHostName (cchar* hostname, in_addr* addr) noexcept
{
	addrinfo* res;

	if (::getaddrinfo (hostname, nullptr, nullptr, &res) == 0)
	{
		*addr = reinterpret_cast<sockaddr_in*> (res->ai_addr)->sin_addr;
		::freeaddrinfo (res);
		return true;
	}

	return false;
}

bool TcpClient::connect (TcpStream& gStream, Address const& server, u16 port) noexcept
{
	sockaddr_in gAddr;

	gAddr.sin_family = AF_INET;
	gAddr.sin_port   = htons (port);

	if (resolveHostName (server.toString ().c_str (), &gAddr.sin_addr) != 0)
	{
		::inet_pton (PF_INET, server.toString ().c_str (), &gAddr.sin_addr);
	}

	if (gStream.isValid ())
		::connect (gStream.getId (),
				   reinterpret_cast<sockaddr*> (&gAddr),
				   sizeof (sockaddr_in));
	return true;
}

} } // namespace Network
