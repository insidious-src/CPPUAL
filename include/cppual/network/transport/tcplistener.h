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

#ifndef CPPUAL_NETWORK_TCP_LISTENER_H_
#define CPPUAL_NETWORK_TCP_LISTENER_H_
#ifdef __cplusplus

#include <cppual/network/transport/tcp.h>

namespace cppual { namespace network {

class tcp_listener : public transport_socket
{
public:
    tcp_listener ();
    tcp_listener (address const& addr, u16 port) noexcept;

    virtual void start_session(protocol_context&, packet& outgoing_packet);
    virtual bool read_data(protocol_context&, packet& incoming_packet);
    virtual byte try_decode(protocol_context&, packet& output_packet);
    virtual byte encode_content(protocol_context&, packet& input_packet, packet& output_packet);

    bool accept (tcp_stream&) noexcept;
    bool listen (address const& addr, u16 port) noexcept;

    bool is_listening () const noexcept
    { return _M_bIsListening; }

private:
    address _M_gAddr        { };
    ushort  _M_uPort        { };
    bool    _M_bIsListening { };
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_TCP_LISTENER_H_
