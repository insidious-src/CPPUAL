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


#include <cppual/network/protocols/http.h>

namespace cppual { namespace network { namespace http {

void Client::start_session (ProtocolContext&, Packet& /*outgoing_packet*/)
{
}

bool Client::read_data (ProtocolContext&, Packet& /*incoming_packet*/)
{
    return false;
}

byte Client::try_decode (ProtocolContext&, Packet& /*output_packet*/)
{
    return byte ();
}

byte Client::encode_content (ProtocolContext&, Packet& /*input_packet*/, Packet& /*output_packet*/)
{
    return byte ();
}

} } } // Http
