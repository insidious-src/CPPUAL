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

#include <cstring>
#include <cppual/network/packet.h>

namespace cppual { namespace Network {

void Packet::append (cvoid* pData, size_type uSize) noexcept
{
    if (pData and (uSize > 0))
    {
        std::size_t start = m_gData.size ();
        m_gData.reserve (start + uSize);
        std::copy(static_cast<cu8*>(pData), static_cast<cu8*>(pData) + uSize, &m_gData[start]);
    }
}

Packet& Packet::operator >> (bool& bData) noexcept
{
    u8 value;
    if (*this >> value) bData = (value != 0);
    return *this;
}

Packet& Packet::operator >> (int8& nData) noexcept
{
    if (canExchange (sizeof (nData)))
    {
        nData   = *reinterpret_cast<cint8*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (nData);
    }

    return *this;
}

Packet& Packet::operator >> (u8& uData) noexcept
{
    if (canExchange (sizeof (uData)))
    {
        uData   = *reinterpret_cast<cu8*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (uData);
    }

    return *this;
}

Packet& Packet::operator >> (int16& nData) noexcept
{
    if (canExchange (sizeof (nData)))
    {
        nData   = *reinterpret_cast<cint16*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (nData);
    }

    return *this;
}

Packet& Packet::operator >> (u16& uData) noexcept
{
    if (canExchange (sizeof (uData)))
    {
        uData   = *reinterpret_cast<cu16*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (uData);
    }

    return *this;
}

Packet& Packet::operator >> (int32& nData) noexcept
{
    if (canExchange (sizeof (nData)))
    {
        nData   = *reinterpret_cast<cint32*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (nData);
    }

    return *this;
}

Packet& Packet::operator >> (u32& uData) noexcept
{
    if (canExchange (sizeof (uData)))
    {
        uData   = *reinterpret_cast<cu32*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (uData);
    }

    return *this;
}

Packet& Packet::operator >> (float& fData) noexcept
{
    if (canExchange (sizeof (fData)))
    {
        fData   = *reinterpret_cast<cfloat*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (fData);
    }

    return *this;
}

Packet& Packet::operator >> (double& dData) noexcept
{
    if (canExchange (sizeof (dData)))
    {
        dData   = *reinterpret_cast<cdouble*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (dData);
    }

    return *this;
}

Packet& Packet::operator >> (ldouble& dData) noexcept
{
    if (canExchange (sizeof (dData)))
    {
        dData   = *reinterpret_cast<cu32*> (&m_gData[m_uPos]);
        m_uPos +=  sizeof (dData);
    }

    return *this;
}

Packet& Packet::operator >> (char* pData) noexcept
{
    // First extract string uLength
    u32 uLength = 0;
    *this >> uLength;

    if ((uLength > 0) and canExchange (uLength))
    {
        // Then extract characters
        ::memcpy (pData, &m_gData[m_uPos], uLength);
        pData[uLength] = '\0';

        // Update reading position
        m_uPos += uLength;
    }

    return *this;
}

Packet& Packet::operator >> (std::string& gData) noexcept
{
    // First extract string uLength
    u32 uLength = 0;
    *this >> uLength;

    gData.clear ();
    if ((uLength > 0) and canExchange (uLength))
    {
        // Then extract characters
        gData.assign (&m_gData[m_uPos], uLength);

        // Update reading position
        m_uPos += uLength;
    }

    return *this;
}

Packet& Packet::operator >> (wchar*) noexcept
{
    return *this;
}

Packet& Packet::operator >> (std::wstring&) noexcept
{
    return *this;
}

Packet& Packet::operator >> (char16*) noexcept
{
    return *this;
}

Packet& Packet::operator >> (std::u16string&) noexcept
{
    return *this;
}

Packet& Packet::operator >> (char32*) noexcept
{
    return *this;
}

Packet& Packet::operator >> (std::u32string&) noexcept
{
    return *this;
}

cvoid* Packet::onSend (size_type& uSize)
{
    uSize = getSize ();
    return  getData ();
}

void Packet::onReceive (cvoid* pData, size_type uSize)
{
    append (pData, uSize);
}

bool Packet::canExchange (size_type uSize) noexcept
{
    return m_bIsValid and (m_uPos + uSize <= m_gData.size ());
}

} } // namespace Network
