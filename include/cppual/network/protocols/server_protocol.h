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

#ifndef CPPUAL_NETWORK_PROTOCOL_SERVER_H_
#define CPPUAL_NETWORK_PROTOCOL_SERVER_H_
#ifdef __cplusplus

#include <cppual/network/transport/tcplistener.h>
#include <cppual/network/packet.h>
#include <cppual/containers.h>
#include <cppual/string.h>

namespace cppual { namespace network {

//================================================

class data_protocol
{
public:
    typedef packet::value_type  value_type ;
    typedef value_type const    const_value;
    typedef string              string_type;
    typedef packet::stream_type stream_type;
    typedef packet::stream_size stream_size;

    enum msg_type
    {
        upload        ,
        download      ,
        publish       ,
        subscribe     ,
        subscribed    ,
        unsubscribe   ,
        mqtt_publish  ,
        mqtt_subscribe,
        mqtt_unsubscribe
    };

    struct PACKED header
    {
        stream_size firstStringSize  { };
        stream_size secondStringSize { };
        stream_size type             { };
        stream_size dataSize         { };
    };

    data_protocol(string_type const& firstString,
                  string_type const& secondString,
                  msg_type           type = msg_type::upload);

    data_protocol(stream_type const& bytes);
    data_protocol() = default;
    stream_type operator ()() const;

    stream_size first_str_size () const noexcept;
    stream_size second_str_size() const noexcept;
    stream_size data_size      () const noexcept;
    stream_size header_size    () const noexcept;
    stream_size size           () const noexcept;
    msg_type    type           () const noexcept;
    stream_type first_string   () const noexcept;
    stream_type second_string  () const noexcept;
    stream_type data           () const noexcept;

    void set_data(stream_type const&);
    void set_type(msg_type) noexcept;

private:
    header      _M_gHeader;
    stream_type _M_gData  ;
};

static_assert (sizeof(data_protocol::header) == 16, "data_protocol::header is not 16 bytes!");

//================================================

class server_protocol : tcp_listener
{
public:

private:
};

//================================================

} } // Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PROTOCOL_SERVER_H_
