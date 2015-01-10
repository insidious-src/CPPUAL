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

#ifndef CPPUAL_NETWORK_TCP_LISTENER_H_
#define CPPUAL_NETWORK_TCP_LISTENER_H_
#ifdef __cplusplus

#include <cppual/network/transport/socket.h>
#include <cppual/network/transport/tcp.h>

namespace cppual { namespace Network {

class TcpListener final : public TransportSocket
{
public:
	TcpListener () = delete;
	TcpListener (Address const&) noexcept;

	bool accept (TcpStream&) noexcept;
	bool listen (u16 port) noexcept;

	bool isListening () const noexcept
	{ return m_bIsListening; }

private:
	Address m_gAddr;
	ushort  m_uPort;
	bool    m_bIsListening;
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_TCP_LISTENER_H_
