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

#include <cppual/network/packet.h>

namespace cppual { namespace network {

void Packet::append (cvoid* pData, size_type uSize) noexcept
{
    if (pData && uSize > 0)
    {
        std::size_t start = _M_gData.size ();
        _M_gData.reserve (start + uSize);
        std::copy(static_cast<cu8*> (pData), static_cast<cu8*> (pData) + uSize, &_M_gData[start]);
    }
}

Packet& Packet::operator >> (bool& bData) noexcept
{
    u8 value = 0;
    if (*this >> value) bData = (value > 0);
    return *this;
}

Packet& Packet::operator >> (i8& nData) noexcept
{
    if (can_exchange (sizeof (nData)))
    {
        nData   = *reinterpret_cast<ci8*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (nData);
    }

    return *this;
}

Packet& Packet::operator >> (u8& uData) noexcept
{
    if (can_exchange (sizeof (uData)))
    {
        uData   = *reinterpret_cast<cu8*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (uData);
    }

    return *this;
}

Packet& Packet::operator >> (i16& nData) noexcept
{
    if (can_exchange (sizeof (nData)))
    {
        nData   = *reinterpret_cast<ci16*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (nData);
    }

    return *this;
}

Packet& Packet::operator >> (u16& uData) noexcept
{
    if (can_exchange (sizeof (uData)))
    {
        uData    = *reinterpret_cast<cu16*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (uData);
    }

    return *this;
}

Packet& Packet::operator >> (i32& nData) noexcept
{
    if (can_exchange (sizeof (nData)))
    {
        nData    = *reinterpret_cast<ci32*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (nData);
    }

    return *this;
}

Packet& Packet::operator >> (u32& uData) noexcept
{
    if (can_exchange (sizeof (uData)))
    {
        uData    = *reinterpret_cast<cu32*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (uData);
    }

    return *this;
}

Packet& Packet::operator >> (float& fData) noexcept
{
    if (can_exchange (sizeof (fData)))
    {
        fData    = *reinterpret_cast<cfloat*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (fData);
    }

    return *this;
}

Packet& Packet::operator >> (double& dData) noexcept
{
    if (can_exchange (sizeof (dData)))
    {
        dData    = *reinterpret_cast<cdouble*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (dData);
    }

    return *this;
}

Packet& Packet::operator >> (ldouble& dData) noexcept
{
    if (can_exchange (sizeof (dData)))
    {
        dData    = *reinterpret_cast<cu32*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (dData);
    }

    return *this;
}

Packet& Packet::operator >> (char* pData) noexcept
{
    // First extract string uLength
    u32 uLength = 0;
    *this >> uLength;

    if ((uLength > 0) and can_exchange (uLength))
    {
        // Then extract characters
        ::memcpy (pData, &_M_gData[_M_uPos], uLength);
        pData[uLength] = '\0';

        // Update reading position
        _M_uPos += uLength;
    }

    return *this;
}

Packet& Packet::operator >> (string& gData) noexcept
{
    // First extract string uLength
    u32 uLength = 0;
    *this >> uLength;

    gData.clear ();
    if ((uLength > 0) and can_exchange (uLength))
    {
        // Then extract characters
        gData.assign (&_M_gData[_M_uPos], uLength);

        // Update reading position
        _M_uPos += uLength;
    }

    return *this;
}

Packet& Packet::operator >> (wchar*) noexcept
{
    return *this;
}

Packet& Packet::operator >> (wstring&) noexcept
{
    return *this;
}

Packet& Packet::operator >> (char16*) noexcept
{
    return *this;
}

Packet& Packet::operator >> (u16string&) noexcept
{
    return *this;
}

Packet& Packet::operator >> (char32*) noexcept
{
    return *this;
}

Packet& Packet::operator >> (u32string&) noexcept
{
    return *this;
}

cvoid* Packet::on_send (size_type& uSize)
{
    uSize = size ();
    return  data ();
}

void Packet::on_receive (cvoid* pData, size_type uSize)
{
    append (pData, uSize);
}

bool Packet::can_exchange (size_type uSize) noexcept
{
    return _M_bIsValid && (_M_uPos + uSize <= _M_gData.size ());
}

} } // namespace Network
