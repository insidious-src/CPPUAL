/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#ifndef CPPUAL_NETWORK_PROTOCOL_INTERFACE_H_
#define CPPUAL_NETWORK_PROTOCOL_INTERFACE_H_
#ifdef __cplusplus

#include <cppual/network/packet.h>

namespace cppual { namespace Network {

class ProtocolContext;

class IProtocol
{
public:
    ProtocolContext* createContext  ();
    bool             addLowerLayer  (IProtocol*);
    IProtocol*       getLowestLayer () const;

    virtual uint getRequiredOutputSize        (uint max_input);
    virtual uint getRequiredRecyclableStreams (uint max_connections,
                                               uint max_concurrent_calls);

    virtual void startSession  (ProtocolContext&, Packet& outgoing_packet) = 0;
    virtual bool readData      (ProtocolContext&, Packet& incoming_packet) = 0;
    virtual byte tryDecode     (ProtocolContext&, Packet&   output_packet) = 0;
    virtual byte encodeContent (ProtocolContext&,
                                Packet&  input_packet,
                                Packet& output_packet) = 0;


    IProtocol* getUpperLayer () const { return m_pUpperProt; }
    IProtocol* getLowerLayer () const { return m_pLowerProt; }

private:
    IProtocol* m_pUpperProt;
    IProtocol* m_pLowerProt;
};

} } // Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PROTOCOL_INTERFACE_H_
