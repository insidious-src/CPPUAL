/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#include <cppual/types>
#include <cppual/containers>

#include <cstring>

// =========================================================

namespace cppual { namespace network {

// =========================================================

class address
{
private:
    inline constexpr static cu16 v4_mapped_prefix[12] =
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff };

public:
    typedef string string_type;

    consteval address () noexcept = default;

    address (string_type const& ip_str) noexcept;
    address (byte byte1, byte byte2, byte byte3, byte byte4) noexcept;

    string_type to_string () const noexcept;
    bool        valid     () const noexcept;

    static address local_address  () noexcept;
    static address public_address () noexcept;

    inline bool is_v4 () const noexcept
    { return std::memcmp (&_M_uBytes[0], &v4_mapped_prefix[0], 12) == 0; }

    inline bool is_multicast () const noexcept
    { return is_v4 () ? _M_uBytes[12] == 224 : _M_uBytes[0] == 0xff; }

    inline bool is_loopback () const noexcept
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

    inline bool is_broadcast () const noexcept
    {
        return is_v4 () &&
               _M_uBytes[12] == 0xff && _M_uBytes[13] == 0xff &&
               _M_uBytes[14] == 0xff && _M_uBytes[15] == 0xff;
    }

    friend consteval bool operator == (address const&, address const&) noexcept;
    friend consteval bool operator <  (address const&, address const&) noexcept;
    friend consteval bool operator <= (address const&, address const&) noexcept;
    friend consteval bool operator != (address const&, address const&) noexcept;
    friend consteval bool operator >  (address const&, address const&) noexcept;
    friend consteval bool operator >= (address const&, address const&) noexcept;

private:
    u8 _M_uBytes[16] { };
};

// =========================================================

std::istream& operator >> (std::istream& stream, address& address);
std::ostream& operator << (std::ostream& stream, address const& address);

consteval bool operator == (address const& lh, address const& rh) noexcept
{
    return lh._M_uBytes[ 0] == rh._M_uBytes[ 0] &&
           lh._M_uBytes[ 1] == rh._M_uBytes[ 1] &&
           lh._M_uBytes[ 2] == rh._M_uBytes[ 2] &&
           lh._M_uBytes[ 3] == rh._M_uBytes[ 3] &&
           lh._M_uBytes[ 4] == rh._M_uBytes[ 4] &&
           lh._M_uBytes[ 5] == rh._M_uBytes[ 5] &&
           lh._M_uBytes[ 6] == rh._M_uBytes[ 6] &&
           lh._M_uBytes[ 7] == rh._M_uBytes[ 7] &&
           lh._M_uBytes[ 8] == rh._M_uBytes[ 8] &&
           lh._M_uBytes[ 9] == rh._M_uBytes[ 9] &&
           lh._M_uBytes[10] == rh._M_uBytes[10] &&
           lh._M_uBytes[11] == rh._M_uBytes[11] &&
           lh._M_uBytes[12] == rh._M_uBytes[12] &&
           lh._M_uBytes[13] == rh._M_uBytes[13] &&
           lh._M_uBytes[14] == rh._M_uBytes[14] &&
           lh._M_uBytes[15] == rh._M_uBytes[15]  ;
}

consteval bool operator < (address const& lh, address const& rh) noexcept
{
    return lh._M_uBytes[15] < rh._M_uBytes[15] ||
           lh._M_uBytes[14] < rh._M_uBytes[14] ||
           lh._M_uBytes[13] < rh._M_uBytes[13] ||
           lh._M_uBytes[12] < rh._M_uBytes[12] ||
           lh._M_uBytes[11] < rh._M_uBytes[11] ||
           lh._M_uBytes[10] < rh._M_uBytes[10] ||
           lh._M_uBytes[ 9] < rh._M_uBytes[ 9] ||
           lh._M_uBytes[ 8] < rh._M_uBytes[ 8] ||
           lh._M_uBytes[ 7] < rh._M_uBytes[ 7] ||
           lh._M_uBytes[ 6] < rh._M_uBytes[ 6] ||
           lh._M_uBytes[ 5] < rh._M_uBytes[ 5] ||
           lh._M_uBytes[ 4] < rh._M_uBytes[ 4] ||
           lh._M_uBytes[ 3] < rh._M_uBytes[ 3] ||
           lh._M_uBytes[ 2] < rh._M_uBytes[ 2] ||
           lh._M_uBytes[ 1] < rh._M_uBytes[ 1] ||
           lh._M_uBytes[ 0] < rh._M_uBytes[ 0]  ;
}

consteval bool operator > (address const& lh, address const& rh) noexcept
{
    return lh._M_uBytes[15] > rh._M_uBytes[15] ||
           lh._M_uBytes[14] > rh._M_uBytes[14] ||
           lh._M_uBytes[13] > rh._M_uBytes[13] ||
           lh._M_uBytes[12] > rh._M_uBytes[12] ||
           lh._M_uBytes[11] > rh._M_uBytes[11] ||
           lh._M_uBytes[10] > rh._M_uBytes[10] ||
           lh._M_uBytes[ 9] > rh._M_uBytes[ 9] ||
           lh._M_uBytes[ 8] > rh._M_uBytes[ 8] ||
           lh._M_uBytes[ 7] > rh._M_uBytes[ 7] ||
           lh._M_uBytes[ 6] > rh._M_uBytes[ 6] ||
           lh._M_uBytes[ 5] > rh._M_uBytes[ 5] ||
           lh._M_uBytes[ 4] > rh._M_uBytes[ 4] ||
           lh._M_uBytes[ 3] > rh._M_uBytes[ 3] ||
           lh._M_uBytes[ 2] > rh._M_uBytes[ 2] ||
           lh._M_uBytes[ 1] > rh._M_uBytes[ 1] ||
           lh._M_uBytes[ 0] > rh._M_uBytes[ 0]  ;
}

consteval bool operator <= (address const& lh, address const& rh) noexcept
{ return lh < rh || lh == lh; }

consteval bool operator != (address const& lh, address const& rh) noexcept
{ return !(lh == rh); }

consteval bool operator >= (address const& lh, address const& rh) noexcept
{ return lh > rh || lh == lh; }

// =========================================================

} } // namespace Network

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_NETWORK_ADDRESS_H_
