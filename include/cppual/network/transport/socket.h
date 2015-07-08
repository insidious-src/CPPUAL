/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <string>
#include <vector>
#include <cppual/types.h>
#include <cppual/noncopyable.h>

using std::vector;
using std::string;
using std::wstring;
using std::u16string;
using std::u32string;

namespace cppual { namespace Network {

typedef int socket_id;

enum class SocketType : bool
{
    Tcp,
    Udp
};

// ====================================================

class TransportSocket : public NonCopyable
{
public:
    typedef socket_id const const_id;

    TransportSocket () = delete;
    TransportSocket (SocketType) noexcept;
    TransportSocket (TransportSocket&&) noexcept;
    TransportSocket& operator = (TransportSocket&&) noexcept;

    void setBlocking (bool) noexcept;

    enum
    {
        AnyPort = 0
    };

    inline ~TransportSocket () noexcept { close (); }
    inline socket_id  id () const noexcept { return m_nId; }
    inline SocketType type () const noexcept { return m_eProtocol; }
    inline bool       isValid () const noexcept { return m_nId != nullSocket (); }
    inline bool       isBlocking () const noexcept { return m_bIsBlocking; }

    static socket_id nullSocket () noexcept
    { return sm_nInvalid; }

protected:
    static socket_id create (SocketType) noexcept;
    void replaceFromId (socket_id) noexcept;
    void close () noexcept;

private:
    static const_id sm_nInvalid;
    socket_id        m_nId;
    SocketType       m_eProtocol;
    bool             m_bIsBlocking;

    void initSocket () noexcept;
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_TRANSPORT_H_
