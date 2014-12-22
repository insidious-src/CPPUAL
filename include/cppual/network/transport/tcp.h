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

#ifndef CPPUAL_NETWORK_TCP_H_
#define CPPUAL_NETWORK_TCP_H_
#ifdef __cplusplus

#include <cppual/network/transport/socket.h>
#include <cppual/network/address.h>
#include <cppual/network/packet.h>

namespace cppual { namespace Network {

struct TcpClient;
class  TcpListener;

class TcpStream final : public TransportSocket
{
public:
	typedef std::size_t size_type;
	typedef ssize_t     stream_size;

	TcpStream& operator << (Packet const&) noexcept; // send
	TcpStream& operator >> (Packet&) noexcept;       // receive
	TcpStream () noexcept;
	~TcpStream () noexcept;

	void disconnect () noexcept;

	inline Address getPeerIp () const noexcept { return m_gPeerAddr; }
	inline u16     getPeerPort () const noexcept { return m_nPeerPort; }

private:
	Address m_gPeerAddr;
	u16     m_nPeerPort;
	bool    m_bIsConnected;

	TcpStream (TcpStream&& stream) noexcept;
	TcpStream (Address const& address, u16 port) noexcept;
	friend struct TcpClient;
	friend class  TcpListener;
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_TCP_H_
