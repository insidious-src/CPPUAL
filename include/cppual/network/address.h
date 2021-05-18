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

#ifndef CPPUAL_NETWORK_ADDRESS_H_
#define CPPUAL_NETWORK_ADDRESS_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/string.h>

#include <cstring>

namespace cppual { namespace Network {

class Address
{
private:
    constexpr static cu16 v4_mapped_prefix[12] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff };

public:
    typedef string string_type;

    Address () noexcept = default;
    Address (string_type const&) noexcept;
    Address (u8 byte1, u8 byte2, u8 byte3, u8 byte4) noexcept;
    string_type toString () const noexcept;
    bool        isValid  () const noexcept;

    static Address localAddress  () noexcept;
    static Address publicAddress () noexcept;

    bool isV4 () const noexcept
    { return std::memcmp (&m_uBytes[0], &v4_mapped_prefix[0], 12) == 0; }

    bool isMulticast () const noexcept
    { return isV4 () ? m_uBytes[12] == 224 : m_uBytes[0] == 0xff; }

    bool isLoopback () const noexcept
    {
        return isV4 () ? m_uBytes[12] == 127 :
                         m_uBytes[ 0] == 0 and m_uBytes[ 1] == 0 and
                         m_uBytes[ 2] == 0 and m_uBytes[ 3] == 0 and
                         m_uBytes[ 4] == 0 and m_uBytes[ 5] == 0 and
                         m_uBytes[ 6] == 0 and m_uBytes[ 7] == 0 and
                         m_uBytes[ 8] == 0 and m_uBytes[ 9] == 0 and
                         m_uBytes[10] == 0 and m_uBytes[11] == 0 and
                         m_uBytes[12] == 0 and m_uBytes[13] == 0 and
                         m_uBytes[14] == 0 and m_uBytes[15] == 1;
    }

    bool isBroadcast () const noexcept
    {
        return isV4 () and
               m_uBytes[12] == 0xff and m_uBytes[13] == 0xff and
               m_uBytes[14] == 0xff and m_uBytes[15] == 0xff;
    }

    friend bool operator != (Address const&, Address const&) noexcept;
    friend bool operator <  (Address const&, Address const&) noexcept;
    friend bool operator <= (Address const&, Address const&) noexcept;
    friend bool operator == (Address const&, Address const&) noexcept;
    friend bool operator >  (Address const&, Address const&) noexcept;
    friend bool operator >= (Address const&, Address const&) noexcept;

private:
    u8 m_uBytes[16];
};

std::istream& operator >> (std::istream& stream, Address& address);
std::ostream& operator << (std::ostream& stream, Address const& address);

inline bool operator != (Address const& gObj1, Address const& gObj2) noexcept
{
    return gObj1.isV4 () != gObj2.isV4 () ?
                                true :
                                gObj1.m_uBytes[ 0] != gObj2.m_uBytes[ 0] or
                                gObj1.m_uBytes[ 1] != gObj2.m_uBytes[ 1] or
                                gObj1.m_uBytes[ 2] != gObj2.m_uBytes[ 2] or
                                gObj1.m_uBytes[ 3] != gObj2.m_uBytes[ 3] or
                                gObj1.m_uBytes[ 4] != gObj2.m_uBytes[ 4] or
                                gObj1.m_uBytes[ 5] != gObj2.m_uBytes[ 5] or
                                gObj1.m_uBytes[ 6] != gObj2.m_uBytes[ 6] or
                                gObj1.m_uBytes[ 7] != gObj2.m_uBytes[ 7] or
                                gObj1.m_uBytes[ 8] != gObj2.m_uBytes[ 8] or
                                gObj1.m_uBytes[ 9] != gObj2.m_uBytes[ 9] or
                                gObj1.m_uBytes[10] != gObj2.m_uBytes[10] or
                                gObj1.m_uBytes[11] != gObj2.m_uBytes[11] or
                                gObj1.m_uBytes[12] != gObj2.m_uBytes[12] or
                                gObj1.m_uBytes[13] != gObj2.m_uBytes[13] or
                                gObj1.m_uBytes[14] != gObj2.m_uBytes[14] or
                                gObj1.m_uBytes[15] != gObj2.m_uBytes[15];
}

inline bool operator < (Address const& gObj1, Address const& gObj2) noexcept
{
    return gObj1.isV4 () != gObj2.isV4 () ?
                                false :
                                gObj1.m_uBytes[15] < gObj2.m_uBytes[15] or
                                gObj1.m_uBytes[14] < gObj2.m_uBytes[14] or
                                gObj1.m_uBytes[13] < gObj2.m_uBytes[13] or
                                gObj1.m_uBytes[12] < gObj2.m_uBytes[12] or
                                gObj1.m_uBytes[11] < gObj2.m_uBytes[11] or
                                gObj1.m_uBytes[10] < gObj2.m_uBytes[10] or
                                gObj1.m_uBytes[ 9] < gObj2.m_uBytes[ 9] or
                                gObj1.m_uBytes[ 8] < gObj2.m_uBytes[ 8] or
                                gObj1.m_uBytes[ 7] < gObj2.m_uBytes[ 7] or
                                gObj1.m_uBytes[ 6] < gObj2.m_uBytes[ 6] or
                                gObj1.m_uBytes[ 5] < gObj2.m_uBytes[ 5] or
                                gObj1.m_uBytes[ 4] < gObj2.m_uBytes[ 4] or
                                gObj1.m_uBytes[ 3] < gObj2.m_uBytes[ 3] or
                                gObj1.m_uBytes[ 2] < gObj2.m_uBytes[ 2] or
                                gObj1.m_uBytes[ 1] < gObj2.m_uBytes[ 1] or
                                gObj1.m_uBytes[ 0] < gObj2.m_uBytes[ 0];
}

inline bool operator > (Address const& gObj1, Address const& gObj2) noexcept
{
    return gObj1.isV4 () != gObj2.isV4 () ?
                                false :
                                gObj1.m_uBytes[15] > gObj2.m_uBytes[15] or
                                gObj1.m_uBytes[14] > gObj2.m_uBytes[14] or
                                gObj1.m_uBytes[13] > gObj2.m_uBytes[13] or
                                gObj1.m_uBytes[12] > gObj2.m_uBytes[12] or
                                gObj1.m_uBytes[11] > gObj2.m_uBytes[11] or
                                gObj1.m_uBytes[10] > gObj2.m_uBytes[10] or
                                gObj1.m_uBytes[ 9] > gObj2.m_uBytes[ 9] or
                                gObj1.m_uBytes[ 8] > gObj2.m_uBytes[ 8] or
                                gObj1.m_uBytes[ 7] > gObj2.m_uBytes[ 7] or
                                gObj1.m_uBytes[ 6] > gObj2.m_uBytes[ 6] or
                                gObj1.m_uBytes[ 5] > gObj2.m_uBytes[ 5] or
                                gObj1.m_uBytes[ 4] > gObj2.m_uBytes[ 4] or
                                gObj1.m_uBytes[ 3] > gObj2.m_uBytes[ 3] or
                                gObj1.m_uBytes[ 2] > gObj2.m_uBytes[ 2] or
                                gObj1.m_uBytes[ 1] > gObj2.m_uBytes[ 1] or
                                gObj1.m_uBytes[ 0] > gObj2.m_uBytes[ 0];
}

inline bool operator <= (Address const& gObj1, Address const& gObj2) noexcept
{ return gObj1 < gObj2 or gObj1 == gObj1; }

inline bool operator == (Address const& gObj1, Address const& gObj2) noexcept
{ return !(gObj1 != gObj2); }

inline bool operator >= (Address const& gObj1, Address const& gObj2) noexcept
{ return gObj1 > gObj2 or gObj1 == gObj1; }

// =========================================================

static_assert (std::is_pod<Address>::value, "Address must be a POD");

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_ADDRESS_H_
