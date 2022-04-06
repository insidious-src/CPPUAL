/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
 *
 * This program is free software: you can redistribute it &&/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, ||
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY || FITNESS FOR A PARTICULAR PURPOSE.  See the
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

namespace cppual { namespace network {

class address
{
private:
    constexpr static cu16 v4_mapped_prefix[12] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff };

public:
    typedef string string_type;

    constexpr address () noexcept = default;
    address (string_type const&) noexcept;
    address (u8 byte1, u8 byte2, u8 byte3, u8 byte4) noexcept;
    string_type to_string () const noexcept;
    bool        valid () const noexcept;

    static address local_address  () noexcept;
    static address public_address () noexcept;

    bool is_v4 () const noexcept
    { return std::memcmp (&_M_uBytes[0], &v4_mapped_prefix[0], 12) == 0; }

    bool is_multicast () const noexcept
    { return is_v4 () ? _M_uBytes[12] == 224 : _M_uBytes[0] == 0xff; }

    bool is_loopback () const noexcept
    {
        return is_v4 () ? _M_uBytes[12] == 127 :
                          _M_uBytes[ 0] == 0 && _M_uBytes[ 1] == 0 &&
                          _M_uBytes[ 2] == 0 && _M_uBytes[ 3] == 0 &&
                          _M_uBytes[ 4] == 0 && _M_uBytes[ 5] == 0 &&
                          _M_uBytes[ 6] == 0 && _M_uBytes[ 7] == 0 &&
                          _M_uBytes[ 8] == 0 && _M_uBytes[ 9] == 0 &&
                          _M_uBytes[10] == 0 && _M_uBytes[11] == 0 &&
                          _M_uBytes[12] == 0 && _M_uBytes[13] == 0 &&
                          _M_uBytes[14] == 0 && _M_uBytes[15] == 1;
    }

    bool is_broadcast () const noexcept
    {
        return is_v4 () &&
               _M_uBytes[12] == 0xff && _M_uBytes[13] == 0xff &&
               _M_uBytes[14] == 0xff && _M_uBytes[15] == 0xff;
    }

    friend bool operator != (address const&, address const&) noexcept;
    friend bool operator <  (address const&, address const&) noexcept;
    friend bool operator <= (address const&, address const&) noexcept;
    friend bool operator == (address const&, address const&) noexcept;
    friend bool operator >  (address const&, address const&) noexcept;
    friend bool operator >= (address const&, address const&) noexcept;

private:
    byte _M_uBytes[16] { };
};

std::istream& operator >> (std::istream& stream, address& address);
std::ostream& operator << (std::ostream& stream, address const& address);

inline bool operator != (address const& gObj1, address const& gObj2) noexcept
{
    return gObj1.is_v4 () != gObj2.is_v4 () ?
                                true :
                                gObj1._M_uBytes[ 0] != gObj2._M_uBytes[ 0] ||
                                gObj1._M_uBytes[ 1] != gObj2._M_uBytes[ 1] ||
                                gObj1._M_uBytes[ 2] != gObj2._M_uBytes[ 2] ||
                                gObj1._M_uBytes[ 3] != gObj2._M_uBytes[ 3] ||
                                gObj1._M_uBytes[ 4] != gObj2._M_uBytes[ 4] ||
                                gObj1._M_uBytes[ 5] != gObj2._M_uBytes[ 5] ||
                                gObj1._M_uBytes[ 6] != gObj2._M_uBytes[ 6] ||
                                gObj1._M_uBytes[ 7] != gObj2._M_uBytes[ 7] ||
                                gObj1._M_uBytes[ 8] != gObj2._M_uBytes[ 8] ||
                                gObj1._M_uBytes[ 9] != gObj2._M_uBytes[ 9] ||
                                gObj1._M_uBytes[10] != gObj2._M_uBytes[10] ||
                                gObj1._M_uBytes[11] != gObj2._M_uBytes[11] ||
                                gObj1._M_uBytes[12] != gObj2._M_uBytes[12] ||
                                gObj1._M_uBytes[13] != gObj2._M_uBytes[13] ||
                                gObj1._M_uBytes[14] != gObj2._M_uBytes[14] ||
                                gObj1._M_uBytes[15] != gObj2._M_uBytes[15];
}

inline bool operator < (address const& gObj1, address const& gObj2) noexcept
{
    return gObj1.is_v4 () != gObj2.is_v4 () ?
                                false :
                                gObj1._M_uBytes[15] < gObj2._M_uBytes[15] ||
                                gObj1._M_uBytes[14] < gObj2._M_uBytes[14] ||
                                gObj1._M_uBytes[13] < gObj2._M_uBytes[13] ||
                                gObj1._M_uBytes[12] < gObj2._M_uBytes[12] ||
                                gObj1._M_uBytes[11] < gObj2._M_uBytes[11] ||
                                gObj1._M_uBytes[10] < gObj2._M_uBytes[10] ||
                                gObj1._M_uBytes[ 9] < gObj2._M_uBytes[ 9] ||
                                gObj1._M_uBytes[ 8] < gObj2._M_uBytes[ 8] ||
                                gObj1._M_uBytes[ 7] < gObj2._M_uBytes[ 7] ||
                                gObj1._M_uBytes[ 6] < gObj2._M_uBytes[ 6] ||
                                gObj1._M_uBytes[ 5] < gObj2._M_uBytes[ 5] ||
                                gObj1._M_uBytes[ 4] < gObj2._M_uBytes[ 4] ||
                                gObj1._M_uBytes[ 3] < gObj2._M_uBytes[ 3] ||
                                gObj1._M_uBytes[ 2] < gObj2._M_uBytes[ 2] ||
                                gObj1._M_uBytes[ 1] < gObj2._M_uBytes[ 1] ||
                                gObj1._M_uBytes[ 0] < gObj2._M_uBytes[ 0];
}

inline bool operator > (address const& gObj1, address const& gObj2) noexcept
{
    return gObj1.is_v4 () != gObj2.is_v4 () ?
                                false :
                                gObj1._M_uBytes[15] > gObj2._M_uBytes[15] ||
                                gObj1._M_uBytes[14] > gObj2._M_uBytes[14] ||
                                gObj1._M_uBytes[13] > gObj2._M_uBytes[13] ||
                                gObj1._M_uBytes[12] > gObj2._M_uBytes[12] ||
                                gObj1._M_uBytes[11] > gObj2._M_uBytes[11] ||
                                gObj1._M_uBytes[10] > gObj2._M_uBytes[10] ||
                                gObj1._M_uBytes[ 9] > gObj2._M_uBytes[ 9] ||
                                gObj1._M_uBytes[ 8] > gObj2._M_uBytes[ 8] ||
                                gObj1._M_uBytes[ 7] > gObj2._M_uBytes[ 7] ||
                                gObj1._M_uBytes[ 6] > gObj2._M_uBytes[ 6] ||
                                gObj1._M_uBytes[ 5] > gObj2._M_uBytes[ 5] ||
                                gObj1._M_uBytes[ 4] > gObj2._M_uBytes[ 4] ||
                                gObj1._M_uBytes[ 3] > gObj2._M_uBytes[ 3] ||
                                gObj1._M_uBytes[ 2] > gObj2._M_uBytes[ 2] ||
                                gObj1._M_uBytes[ 1] > gObj2._M_uBytes[ 1] ||
                                gObj1._M_uBytes[ 0] > gObj2._M_uBytes[ 0];
}

inline bool operator <= (address const& gObj1, address const& gObj2) noexcept
{ return gObj1 < gObj2 || gObj1 == gObj1; }

inline bool operator == (address const& gObj1, address const& gObj2) noexcept
{ return !(gObj1 != gObj2); }

inline bool operator >= (address const& gObj1, address const& gObj2) noexcept
{ return gObj1 > gObj2 || gObj1 == gObj1; }

// =========================================================

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_ADDRESS_H_
