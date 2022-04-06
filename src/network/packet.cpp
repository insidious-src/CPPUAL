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

// =========================================================

void packet::append (cvoid* pData, size_type uSize) noexcept
{
    if (pData && uSize > 0)
    {
        _M_gData.reserve (_M_gData.size () + uSize);
        _M_gData.insert  (_M_gData.end (),
                          static_cast<cchar*> (pData),
                          static_cast<cchar*> (pData) + uSize);
    }
}

// =========================================================

packet& packet::operator << (cbool bData) noexcept
{
    append (&bData, sizeof (cbool));
    return *this;
}

packet& packet::operator << (ci8 nData) noexcept
{
    append (&nData, sizeof (ci8));
    return *this;
}

packet& packet::operator << (cu8 uData) noexcept
{
    append (&uData, sizeof (cu8));
    return *this;
}

packet& packet::operator << (ci16 nData) noexcept
{
    append (&nData, sizeof (ci16));
    return *this;
}

packet& packet::operator << (cu16 uData) noexcept
{
    append (&uData, sizeof (cu16));
    return *this;
}

packet& packet::operator << (ci32 nData) noexcept
{
    append (&nData, sizeof (ci32));
    return *this;
}

packet& packet::operator << (cu32 uData) noexcept
{
    append (&uData, sizeof (cu32));
    return *this;
}

packet& packet::operator << (ci64& nData) noexcept
{
    append (&nData, sizeof (ci64));
    return *this;
}

packet& packet::operator << (cu64& uData) noexcept
{
    append (&uData, sizeof (cu64));
    return *this;
}

packet& packet::operator << (cfloat& fData) noexcept
{
    append (&fData, sizeof (cfloat));
    return *this;
}

packet& packet::operator << (cdouble& dData) noexcept
{
    append (&dData, sizeof (cdouble));
    return *this;
}

packet& packet::operator << (cldouble& dData) noexcept
{
    append (&dData, sizeof (cldouble));
    return *this;
}

packet& packet::operator << (stream_type const& gData) noexcept
{
    cu32 size = static_cast<u32> (gData.size ());

    append (&size, sizeof (cu32));
    append (&gData, gData.size ());
    return *this;
}

packet& packet::operator << (string const& gData) noexcept
{
    cu32 size = static_cast<u32> (gData.size ());

    append (&size, sizeof (cu32));
    append (&gData, gData.size ());
    return *this;
}

packet& packet::operator << (wstring const& gData) noexcept
{
    cu32 size = static_cast<u32> (sizeof (wchar) * gData.size ());

    append (&size, sizeof (cu32));
    append (&gData, size);
    return *this;
}

packet& packet::operator << (u16string const& gData) noexcept
{
    cu32 size = static_cast<u32> (sizeof (char16) * gData.size ());

    append (&size, sizeof (cu32));
    append (&gData, size);
    return *this;
}

packet& packet::operator << (u32string const& gData) noexcept
{
    cu32 size = static_cast<u32> (sizeof (char32) * gData.size ());

    append (&size, sizeof (cu32));
    append (&gData, size);
    return *this;
}

// =========================================================

packet& packet::operator >> (bool& bData) noexcept
{
    u8 value = 0;
    if (*this >> value) bData = (value > 0);
    return *this;
}

packet& packet::operator >> (i8& nData) noexcept
{
    if (can_exchange (sizeof (nData)))
    {
        nData    = *direct_cast<ci8*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (nData);
    }

    return *this;
}

packet& packet::operator >> (u8& uData) noexcept
{
    if (can_exchange (sizeof (uData)))
    {
        uData    = *direct_cast<cu8*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (uData);
    }

    return *this;
}

packet& packet::operator >> (i16& nData) noexcept
{
    if (can_exchange (sizeof (nData)))
    {
        nData    = *direct_cast<ci16*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (nData);
    }

    return *this;
}

packet& packet::operator >> (u16& uData) noexcept
{
    if (can_exchange (sizeof (uData)))
    {
        uData    = *direct_cast<cu16*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (uData);
    }

    return *this;
}

packet& packet::operator >> (i32& nData) noexcept
{
    if (can_exchange (sizeof (nData)))
    {
        nData    = *direct_cast<ci32*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (nData);
    }

    return *this;
}

packet& packet::operator >> (u32& uData) noexcept
{
    if (can_exchange (sizeof (uData)))
    {
        uData    = *direct_cast<cu32*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (uData);
    }

    return *this;
}

packet& packet::operator >> (i64& nData) noexcept
{
    if (can_exchange (sizeof (nData)))
    {
        nData    = *direct_cast<ci64*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (nData);
    }

    return *this;
}

packet& packet::operator >> (u64& uData) noexcept
{
    if (can_exchange (sizeof (uData)))
    {
        uData    = *direct_cast<cu64*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (uData);
    }

    return *this;
}

packet& packet::operator >> (float& fData) noexcept
{
    if (can_exchange (sizeof (fData)))
    {
        fData    = *direct_cast<cfloat*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (fData);
    }

    return *this;
}

packet& packet::operator >> (double& dData) noexcept
{
    if (can_exchange (sizeof (dData)))
    {
        dData    = *direct_cast<cdouble*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (dData);
    }

    return *this;
}

packet& packet::operator >> (ldouble& dData) noexcept
{
    if (can_exchange (sizeof (dData)))
    {
        dData    = *direct_cast<cldouble*> (&_M_gData[_M_uPos]);
        _M_uPos +=  sizeof (dData);
    }

    return *this;
}

packet& packet::operator >> (stream_type& gData) noexcept
{
    // First extract string uLength
    u32 uLength = 0;
    *this >> uLength;

    gData.clear ();

    if ((uLength > 0) && can_exchange (uLength))
    {
        // Then extract characters
        gData.reserve (gData.size () + uLength);

        gData.insert  (gData.end (),
                       &_M_gData[_M_uPos],
                       &_M_gData[_M_uPos] + uLength);

        // Update reading position
        _M_uPos += uLength;
    }

    return *this;
}

packet& packet::operator >> (string& gData) noexcept
{
    // First extract string uLength
    u32 uLength = 0;
    *this >> uLength;

    gData.clear ();

    if ((uLength > 0) && can_exchange (uLength))
    {
        // Then extract characters
        gData.assign (&_M_gData[_M_uPos], uLength);

        // Update reading position
        _M_uPos += uLength;
    }

    return *this;
}

packet& packet::operator >> (wstring& gData) noexcept
{
    // First extract size
    u32 uLength = 0;
    *this >> uLength;

    gData.clear ();

    if ((uLength > 0) && can_exchange (uLength))
    {
        // Then extract characters
        gData.assign (direct_cast<wchar*> (&_M_gData[_M_uPos]), uLength / sizeof (wchar));

        // Update reading position
        _M_uPos += uLength;
    }

    return *this;
}

packet& packet::operator >> (u16string& gData) noexcept
{
    // First extract size
    u32 uLength = 0;
    *this >> uLength;

    gData.clear ();

    if ((uLength > 0) && can_exchange (uLength))
    {
        // Then extract characters
        gData.assign (direct_cast<char16*> (&_M_gData[_M_uPos]), uLength / sizeof (char16));

        // Update reading position
        _M_uPos += uLength;
    }

    return *this;
}

packet& packet::operator >> (u32string& gData) noexcept
{
    // First extract size
    u32 uLength = 0;
    *this >> uLength;

    gData.clear ();

    if ((uLength > 0) && can_exchange (uLength))
    {
        // Then extract characters
        gData.assign (direct_cast<char32*> (&_M_gData[_M_uPos]), uLength / sizeof (char32));

        // Update reading position
        _M_uPos += uLength;
    }

    return *this;
}

// =========================================================

cvoid* packet::on_send (size_type& uSize)
{
    uSize = size ();
    return  data ();
}

void packet::on_receive (cvoid* pData, size_type uSize)
{
    append (pData, uSize);
}

bool packet::can_exchange (size_type uSize) noexcept
{
    return _M_bIsValid && ((_M_uPos + uSize) < _M_gData.size ());
}

// =========================================================

} } // namespace Network
