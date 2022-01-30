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

#ifndef CPPUAL_NETWORK_PACKET_H_
#define CPPUAL_NETWORK_PACKET_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/string.h>
#include <cppual/noncopyable.h>
#include <cppual/containers.h>

namespace cppual { namespace network {

class Packet
{
public:
    typedef std::size_t  size_type  ;
    typedef vector<char> vector_type;
    typedef bool (Packet::* safe_bool)(size_type);

    virtual ~Packet() { }

    bool operator == (Packet const&) const = delete;
    bool operator != (Packet const&) const = delete;

    void append (cvoid* data, size_type size_in_bytes) noexcept;

    Packet& operator >> (bool&)      noexcept;
    Packet& operator >> (i8&)        noexcept;
    Packet& operator >> (u8&)        noexcept;
    Packet& operator >> (i16&)       noexcept;
    Packet& operator >> (u16&)       noexcept;
    Packet& operator >> (i32&)       noexcept;
    Packet& operator >> (u32&)       noexcept;
    Packet& operator >> (float&)     noexcept;
    Packet& operator >> (double&)    noexcept;
    Packet& operator >> (ldouble&)   noexcept;
    Packet& operator >> (char*)      noexcept;
    Packet& operator >> (string&)    noexcept;
    Packet& operator >> (wchar*)     noexcept;
    Packet& operator >> (wstring&)   noexcept;
    Packet& operator >> (char16*)    noexcept;
    Packet& operator >> (u16string&) noexcept;
    Packet& operator >> (char32*)    noexcept;
    Packet& operator >> (u32string&) noexcept;

    Packet& operator << (bool)             noexcept;
    Packet& operator << (i8)               noexcept;
    Packet& operator << (u8)               noexcept;
    Packet& operator << (i16)              noexcept;
    Packet& operator << (u16)              noexcept;
    Packet& operator << (i32)              noexcept;
    Packet& operator << (u32)              noexcept;
    Packet& operator << (float)            noexcept;
    Packet& operator << (double)           noexcept;
    Packet& operator << (ldouble)          noexcept;
    Packet& operator << (cchar*)           noexcept;
    Packet& operator << (string const&)    noexcept;
    Packet& operator << (cwchar*)          noexcept;
    Packet& operator << (wstring const&)   noexcept;
    Packet& operator << (cchar16*)         noexcept;
    Packet& operator << (u16string const&) noexcept;
    Packet& operator << (cchar32*)         noexcept;
    Packet& operator << (u32string const&) noexcept;

    constexpr size_type size () const noexcept
    { return _M_gData.size (); }

    constexpr cvoid* data () const noexcept
    { return !_M_gData.empty () ? &_M_gData[0] : nullptr; }

    constexpr bool is_end_of_packet () const noexcept
    { return _M_uPos >= _M_gData.size (); }

    constexpr operator safe_bool () const noexcept
    { return _M_bIsValid ? &Packet::can_exchange : nullptr; }

    inline void flush () noexcept
    {
        _M_gData.clear ();
        _M_uPos = 0;
    }

protected:
    virtual cvoid* on_send    (size_type& size);
    virtual void   on_receive (cvoid* data, size_type size_in_bytes);

private:
    bool can_exchange (size_type size_in_bytes) noexcept;

private:
    vector_type _M_gData   ;
    size_type   _M_uPos    ;
    bool        _M_bIsValid;
};

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PACKET_H_
