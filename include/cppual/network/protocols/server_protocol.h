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

#ifndef CPPUAL_NETWORK_PROTOCOL_SERVER_H_
#define CPPUAL_NETWORK_PROTOCOL_SERVER_H_
#ifdef __cplusplus

#include <cppual/network/protocols/protocol.h>
#include <cppual/containers.h>
#include <cppual/string.h>

namespace cppual { namespace network {

//================================================

class DataProtocol
{
public:
    typedef string       string_type;
    typedef vector<char> stream_type;
    typedef u32          size_type  ;

    enum Type
    {
        Upload       ,
        Download     ,
        Publish      ,
        Subscribe    ,
        Subscribed   ,
        Unsubscribe  ,
        MQTTPublish  ,
        MQTTSubscribe,
        MQTTUnsubscribe
    };

    struct PACKED Header
    {
        size_type firstStringSize  { };
        size_type secondStringSize { };
        size_type type             { };
        size_type dataSize         { };
    };

    DataProtocol(string_type const& firstString, string_type const& secondString, Type type = Type::Upload);
    DataProtocol(stream_type const& bytes);
    DataProtocol() = default;
    stream_type operator ()() const;

    size_type   first_str_size () const noexcept;
    size_type   second_str_size() const noexcept;
    size_type   data_size      () const noexcept;
    size_type   header_size    () const noexcept;
    size_type   size           () const noexcept;
    Type        type           () const noexcept;
    stream_type first_string   () const noexcept;
    stream_type second_string  () const noexcept;
    stream_type data           () const noexcept;

    void set_data(stream_type const&);
    void set_type(Type) noexcept;

private:
    Header      _M_gHeader;
    stream_type _M_gData  ;
};

static_assert (sizeof(DataProtocol::Header) == 16, "DataProtocol::Header is not 16 bytes!");

//================================================

class ServerProtocol : Protocol
{
public:
    void start_session  (ProtocolContext&, Packet& outgoing_packet);
    bool read_data      (ProtocolContext&, Packet& incoming_packet);
    byte try_decode     (ProtocolContext&, Packet& output_packet);
    byte encode_content (ProtocolContext&, Packet& input_packet, Packet& output_packet);

private:
};

//================================================

} } // Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PROTOCOL_SERVER_H_
