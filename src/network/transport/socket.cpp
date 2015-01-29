/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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


#include <iostream>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <cppual/network/transport/socket.h>

namespace cppual { namespace Network {

struct SocketFlag
{
	enum Type
	{
		NonBlocking = O_NONBLOCK,
		NoDelay     = TCP_NODELAY,
		Broadcast   = SO_BROADCAST,
#ifdef OS_LEOPARD_X
		NoSigPipe   = SO_NOSIGPIPE
#endif
	};
};

socket_id const TransportSocket::sm_nInvalid = -1;

TransportSocket::TransportSocket (SocketType eProt) noexcept
: m_nId (create (eProt)),
  m_eProtocol (eProt),
  m_bIsBlocking (true)
{
	initSocket ();
}

TransportSocket::TransportSocket (TransportSocket&& gObj) noexcept
: m_nId (gObj.m_nId),
  m_eProtocol (gObj.m_eProtocol),
  m_bIsBlocking (gObj.m_bIsBlocking)
{
	gObj.m_nId = nullSocket ();
}

TransportSocket& TransportSocket::operator = (TransportSocket&& gObj) noexcept
{
	m_nId         = gObj.m_nId;
	m_eProtocol   = gObj.m_eProtocol;
	m_bIsBlocking = gObj.m_bIsBlocking;
	gObj.m_nId    = nullSocket ();
	return *this;
}

void TransportSocket::setBlocking (bool bBlock) noexcept
{
	if (m_nId != nullSocket ())
	{
		int nStatus = ::fcntl (m_nId, F_GETFL);

		if (bBlock) nStatus &= ~O_NONBLOCK;
		else nStatus |= O_NONBLOCK;

		::fcntl (m_nId, F_SETFL, nStatus);
		m_bIsBlocking = bBlock;
	}
}

socket_id TransportSocket::create (SocketType eProt) noexcept
{
	return ::socket (PF_INET,
					 eProt == SocketType::Tcp ? SOCK_STREAM : SOCK_DGRAM,
					 0);
}

void TransportSocket::replaceFromId (socket_id nId) noexcept
{
	if (nId   == nullSocket ()) return;
	if (m_nId != nullSocket ()) ::close (m_nId);
	m_nId = nId;
}

void TransportSocket::close () noexcept
{
	if (m_nId != nullSocket ()) ::close (m_nId);
}

void TransportSocket::initSocket () noexcept
{
	if (m_nId != nullSocket ())
	{
		if (m_eProtocol == SocketType::Tcp)
		{
#ifdef OS_STD_UNIX
			// Disable the Nagle algorithm (ie. removes buffering of TCP packets)
			int yes = 1;

			if (::setsockopt (m_nId,
							  IPPROTO_TCP,
							  TCP_NODELAY,
							  reinterpret_cast<char*> (&yes),
							  sizeof (int)) == -1)
			{
				std::cerr << "Failed to set socket option \"TCP_NODELAY\"; "
							 "All your TCP packets will be buffered" << std::endl;
			}
#	ifdef OS_LEOPARD_X
			// On Mac OS X, disable the SIGPIPE signal on disconnection
			if (::setsockopt (m_nId,
							  SOL_SOCKET,
							  SO_NOSIGPIPE,
							  reinterpret_cast<char*>(&yes),
							  sizeof (int)) == -1)
				std::cerr << "Failed to set socket option \"SO_NOSIGPIPE\"" << std::endl;
#	endif
		}
		else
		{
			// Enable broadcast by default for UDP sockets
			int yes = 1;

			if (::setsockopt (m_nId,
							  SOL_SOCKET,
							  SO_BROADCAST,
							  reinterpret_cast<char*> (&yes),
							  sizeof (int)) == -1)
				std::cerr << "Failed to enable broadcast on UDP socket" << std::endl;
		}
#endif
	}
}

} } // namespace Network
