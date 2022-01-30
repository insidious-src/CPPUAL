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

#ifndef CPPUAL_NETWORK_TCP_H_
#define CPPUAL_NETWORK_TCP_H_
#ifdef __cplusplus

#include <cppual/network/transport/socket.h>
#include <cppual/network/address.h>
#include <cppual/network/packet.h>

namespace cppual { namespace network {

class TcpStream : public virtual TransportSocket
{
public:
    typedef std::size_t size_type;
    typedef ssize_t     stream_size;

    /// send
    TcpStream& operator << (Packet const&) noexcept;

    /// receive
    TcpStream& operator >> (Packet&) noexcept;

    TcpStream  () noexcept;
    virtual ~TcpStream () noexcept;

    void disconnect () noexcept;

    virtual void start_session(ProtocolContext&, Packet& outgoing_packet);
    virtual bool read_data(ProtocolContext&, Packet& incoming_packet);
    virtual byte try_decode(ProtocolContext&, Packet& output_packet);
    virtual byte encode_content(ProtocolContext&, Packet& input_packet, Packet& output_packet);

    Address peer_address () const noexcept { return _M_gPeerAddr; }
    u16     peer_port    () const noexcept { return _M_nPeerPort; }

protected:
    TcpStream (TcpStream&& stream) noexcept;
    TcpStream (Address const& address, u16 port) noexcept;

private:
    Address _M_gPeerAddr    { };
    u16     _M_nPeerPort    { };
    bool    _M_bIsConnected { };

};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_TCP_H_
