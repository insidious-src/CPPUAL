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

#ifndef CPPUAL_NETWORK_TRANSPORT_H_
#define CPPUAL_NETWORK_TRANSPORT_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/string.h>
#include <cppual/resource.h>
#include <cppual/noncopyable.h>
#include <cppual/network/protocols/protocol.h>

namespace cppual { namespace network {

// ====================================================

enum class socket_type : u8
{
    tcp,
    udp
};

// ====================================================

class transport_socket
    : public protocol,
      public resource<void, int, static_cast<resource_handle::value_type> (-1)>
{
public:
    typedef value_type       socket_id;
    typedef value_type const const_id ;

    transport_socket () = delete;
    virtual ~transport_socket () noexcept;
    transport_socket (socket_type) noexcept;
    transport_socket (transport_socket&&) noexcept;
    transport_socket& operator = (transport_socket&&) noexcept;

    void set_blocking (bool block) noexcept;
    bool is_blocking  ()     const noexcept;

    inline socket_type type  () const noexcept { return _M_eProtocol; }

protected:
    static socket_id create_socket (socket_type) noexcept;
    void   replace_from_id         (socket_id  ) noexcept;
    void   close                   ()            noexcept;

private:
    void init_socket () noexcept;

private:
    socket_type _M_eProtocol;
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_TRANSPORT_H_
