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

class tcp_stream : public virtual transport_socket
{
public:
    typedef std::size_t size_type  ;
    typedef ssize_t     stream_size;

    /// send
    tcp_stream& operator << (packet const&) noexcept;

    /// receive
    tcp_stream& operator >> (packet&) noexcept;

    tcp_stream  () noexcept;
    virtual ~tcp_stream () noexcept;

    void disconnect () noexcept;

    virtual void start_session(protocol_context&, packet& outgoing_packet);
    virtual bool read_data(protocol_context&, packet& incoming_packet);
    virtual byte try_decode(protocol_context&, packet& output_packet);
    virtual byte encode_content(protocol_context&, packet& input_packet, packet& output_packet);

    address peer_address () const noexcept { return _M_gPeerAddr; }
    u16     peer_port    () const noexcept { return _M_nPeerPort; }

protected:
    tcp_stream (tcp_stream&& stream) noexcept;
    tcp_stream (address const& address, u16 port) noexcept;

private:
    address _M_gPeerAddr    { };
    u16     _M_nPeerPort    { };
    bool    _M_bIsConnected { };

};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_TCP_H_
