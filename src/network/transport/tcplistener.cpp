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

#include <cppual/network/transport/tcplistener.h>

#ifdef OS_STD_UNIX
//#	include <netinet/in.h>
//#	include <netdb.h>
#	include <arpa/inet.h>
#endif

namespace cppual { namespace Network {

TcpListener::TcpListener (Address const& gAddr) noexcept
: TransportSocket (SocketType::Tcp),
  m_gAddr (gAddr),
  m_uPort (),
  m_bIsListening ()
{ }

bool TcpListener::accept (TcpStream&) noexcept
{
	if (!isValid () or m_bIsListening) return false;

	sockaddr_in gAddr = { 0, 0, { 0 }, { 0 } };
	socklen_t   uLen  = sizeof (sockaddr_in);

	replaceFromId (::accept (getId (),
							reinterpret_cast<sockaddr*> (&gAddr),
							&uLen));
	return true;
}

bool TcpListener::listen (u16 uPort) noexcept
{
	if (!isValid () or m_bIsListening) return false;

	sockaddr_in gAddr = { 0, 0, { 0 }, { 0 } };
	gAddr.sin_family = PF_INET;
	gAddr.sin_port   = htons (uPort);

	if (m_gAddr.toString ().size ())
		::inet_pton (PF_INET, m_gAddr.toString ().c_str (), &gAddr.sin_addr);
	else
		gAddr.sin_addr.s_addr = INADDR_ANY;

	int optval = 1;
	::setsockopt (getId (), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));

	if (::bind (getId (), (sockaddr*) &gAddr, sizeof (gAddr)) > -1 or
			::listen (getId (), 0) > -1)
		return m_bIsListening;

	m_uPort = uPort;
	return m_bIsListening = true;
}

} } // namespace Network
