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

#ifndef CPPUAL_NETWORK_PROTOCOL_HTTP_H_
#define CPPUAL_NETWORK_PROTOCOL_HTTP_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/network/protocols/protocol.h>

namespace cppual { namespace network { namespace http {

class Request final
{
};

class Response final
{
};

class Client : public protocol
{
    enum class ConnectionType : u8
    {
        Plain,
        SSL,
        TLS,
        Mixed,
        Custom
    };

    void start_session  (protocol_context&, packet& outgoing_packet);
    bool read_data      (protocol_context&, packet& incoming_packet);
    byte try_decode     (protocol_context&, packet& output_packet);
    byte encode_content (protocol_context&, packet& input_packet, packet& output_packet);
    int  version       ();

private:
    ConnectionType _M_eConnType;
};

} } } // Http

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PROTOCOL_HTTP_H_
