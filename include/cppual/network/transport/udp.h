/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#ifndef CPPUAL_NETWORK_UDP_H_
#define CPPUAL_NETWORK_UDP_H_
#ifdef __cplusplus

#include <cppual/network/transport/socket.h>
#include <cppual/network/address.h>
#include <cppual/network/packet.h>
#include <cppual/string.h>

namespace cppual { namespace Network {

class UdpStream final : public virtual TransportSocket
{
public:
    typedef std::size_t size_type;
    typedef ssize_t     stream_size;

    UdpStream& operator << (Packet const&) noexcept;
    UdpStream& operator >> (Packet&) noexcept;
    UdpStream  () noexcept;
    ~UdpStream () noexcept;

    inline Address getPeerIp   () const noexcept { return m_gPeerAddr; }
    inline int     getPeerPort () const noexcept { return m_nPeerPort; }

    inline void setPeer (Address const& gAddr, u16 uPort) noexcept
    { m_gPeerAddr = gAddr; m_nPeerPort = uPort; }

    inline void setPeerIp (Address const& gAddr) noexcept
    { m_gPeerAddr = gAddr; }

    inline void setPeerPort (u16 uPort) noexcept
    { m_nPeerPort = uPort; }

private:
    Address m_gPeerAddr;
    u16     m_nPeerPort;

    UdpStream (UdpStream&& stream) noexcept;
    UdpStream (Address const& address, u16 port = AnyPort) noexcept;
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_UDP_H_
