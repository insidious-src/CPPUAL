/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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
#include <cppual/containers.h>
#include <cppual/noncopyable.h>

namespace cppual { namespace network {

// =========================================================

class packet : public non_copyable_virtual
{
public:
    typedef char                  value_type ;
    typedef value_type const      const_value;
    typedef std::size_t           size_type  ;
    typedef u32                   stream_size;
    typedef dyn_array<value_type> stream_type;

    typedef bool (packet::* safe_bool)(size_type);

    void append (cvoid* data, size_type size_in_bytes) noexcept;

    packet& operator << (cbool)              noexcept;
    packet& operator << (ci8)                noexcept;
    packet& operator << (cu8)                noexcept;
    packet& operator << (ci16)               noexcept;
    packet& operator << (cu16)               noexcept;
    packet& operator << (ci32)               noexcept;
    packet& operator << (cu32)               noexcept;
    packet& operator << (ci64&)              noexcept;
    packet& operator << (cu64&)              noexcept;
    packet& operator << (cfloat&)            noexcept;
    packet& operator << (cdouble&)           noexcept;
    packet& operator << (cldouble&)          noexcept;
    packet& operator << (stream_type const&) noexcept;
    packet& operator << (string const&)      noexcept;
    packet& operator << (wstring const&)     noexcept;
    packet& operator << (u16string const&)   noexcept;
    packet& operator << (u32string const&)   noexcept;

    packet& operator >> (bool&)        noexcept;
    packet& operator >> (i8&)          noexcept;
    packet& operator >> (u8&)          noexcept;
    packet& operator >> (i16&)         noexcept;
    packet& operator >> (u16&)         noexcept;
    packet& operator >> (i32&)         noexcept;
    packet& operator >> (u32&)         noexcept;
    packet& operator >> (i64&)         noexcept;
    packet& operator >> (u64&)         noexcept;
    packet& operator >> (float&)       noexcept;
    packet& operator >> (double&)      noexcept;
    packet& operator >> (ldouble&)     noexcept;
    packet& operator >> (stream_type&) noexcept;
    packet& operator >> (string&)      noexcept;
    packet& operator >> (wstring&)     noexcept;
    packet& operator >> (u16string&)   noexcept;
    packet& operator >> (u32string&)   noexcept;

    constexpr size_type size () const noexcept
    { return _M_gData.size (); }

    constexpr cvoid* data () const noexcept
    { return !_M_gData.empty () ? _M_gData.data () : nullptr; }

    constexpr bool is_end_of_packet () const noexcept
    { return _M_uPos >= _M_gData.size (); }

    constexpr bool is_valid () const noexcept
    { return _M_gData.capacity () > 0; }

    constexpr operator safe_bool () const noexcept
    { return is_valid () ? &packet::can_exchange : nullptr; }

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
    stream_type _M_gData;
    size_type   _M_uPos ;
};

// =========================================================

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PACKET_H_
