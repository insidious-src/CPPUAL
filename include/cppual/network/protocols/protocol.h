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

#ifndef CPPUAL_NETWORK_PROTOCOL_INTERFACE_H_
#define CPPUAL_NETWORK_PROTOCOL_INTERFACE_H_
#ifdef __cplusplus

#include <cppual/network/packet.h>
#include <cppual/noncopyable.h>

namespace cppual { namespace network {

// =========================================================

class protocol_context;

// =========================================================

class protocol : public non_copyable_virtual
{
public:
    protocol_context* create_context  ();
    void              add_upper_layer (protocol*);
    void              add_lower_layer (protocol*);
    protocol*         uppest_layer    () const;
    protocol*         lowest_layer    () const;

    virtual uint required_output_size        (uint max_input);
    virtual uint required_recyclable_streams (uint max_connections,
                                              uint max_concurrent_calls);

    virtual void start_session  (protocol_context&, packet& outgoing_packet) = 0;
    virtual bool read_data      (protocol_context&, packet& incoming_packet) = 0;
    virtual byte try_decode     (protocol_context&, packet&   output_packet) = 0;

    virtual byte encode_content (protocol_context&,
                                 packet&  input_packet,
                                 packet& output_packet) = 0;

    protocol* upper_layer () const noexcept { return _M_pUpperProt; }
    protocol* lower_layer () const noexcept { return _M_pLowerProt; }

private:
    protocol* _M_pUpperProt;
    protocol* _M_pLowerProt;
};

// =========================================================

} } // Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PROTOCOL_INTERFACE_H_
