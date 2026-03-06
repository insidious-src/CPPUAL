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

#include <cppual/types>
#include <cppual/string>
#include <cppual/containers>
#include <cppual/noncopyable>

// =========================================================

namespace cppual::network {

// =========================================================

class packet : public non_copyable_virtual
{
public:
    typedef packet                  self_type    ;
    typedef char                    value_type   ;
    typedef value_type const        const_value  ;
    typedef std::size_t             size_type    ;
    typedef size_type const         const_size   ;
    typedef  void*                  pointer      ;
    typedef cvoid*                  const_pointer;
    typedef u32                     stream_size  ;
    typedef dyn_array<value_type>   stream_type  ;

    using safe_bool = bool (self_type::*) (size_type) const noexcept;

    void append (const_pointer data, size_type size_in_bytes) noexcept;

    self_type& operator << (cbool)              noexcept;
    self_type& operator << (ci8)                noexcept;
    self_type& operator << (cu8)                noexcept;
    self_type& operator << (ci16)               noexcept;
    self_type& operator << (cu16)               noexcept;
    self_type& operator << (ci32)               noexcept;
    self_type& operator << (cu32)               noexcept;
    self_type& operator << (ci64&)              noexcept;
    self_type& operator << (cu64&)              noexcept;
    self_type& operator << (cfloat&)            noexcept;
    self_type& operator << (cdouble&)           noexcept;
    self_type& operator << (cldouble&)          noexcept;
    self_type& operator << (stream_type const&) noexcept;
    self_type& operator << (string const&)      noexcept;
    self_type& operator << (u8string const&)    noexcept;
    self_type& operator << (u16string const&)   noexcept;
    self_type& operator << (u32string const&)   noexcept;
    self_type& operator << (wstring const&)     noexcept;
    self_type& operator << (fstring const&)     noexcept;
    self_type& operator << (fu8string const&)   noexcept;
    self_type& operator << (fu16string const&)  noexcept;
    self_type& operator << (fu32string const&)  noexcept;
    self_type& operator << (fwstring const&)    noexcept;

    self_type& operator >> (bool&)        noexcept;
    self_type& operator >> (i8&)          noexcept;
    self_type& operator >> (u8&)          noexcept;
    self_type& operator >> (i16&)         noexcept;
    self_type& operator >> (u16&)         noexcept;
    self_type& operator >> (i32&)         noexcept;
    self_type& operator >> (u32&)         noexcept;
    self_type& operator >> (i64&)         noexcept;
    self_type& operator >> (u64&)         noexcept;
    self_type& operator >> (float&)       noexcept;
    self_type& operator >> (double&)      noexcept;
    self_type& operator >> (ldouble&)     noexcept;
    self_type& operator >> (stream_type&) noexcept;
    self_type& operator >> (string&)      noexcept;
    self_type& operator >> (u8string&)    noexcept;
    self_type& operator >> (u16string&)   noexcept;
    self_type& operator >> (u32string&)   noexcept;
    self_type& operator >> (wstring&)     noexcept;
    self_type& operator >> (fstring&)     noexcept;
    self_type& operator >> (fu8string&)   noexcept;
    self_type& operator >> (fu16string&)  noexcept;
    self_type& operator >> (fu32string&)  noexcept;
    self_type& operator >> (fwstring&)    noexcept;

    constexpr size_type size () const noexcept
    { return _M_gData.size (); }

    constexpr const_pointer data () const noexcept
    { return !_M_gData.empty () ? _M_gData.data () : nullptr; }

    constexpr bool is_end_of_packet () const noexcept
    { return _M_uPos >= _M_gData.size (); }

    constexpr bool valid () const noexcept
    { return _M_gData.capacity () > 0; }

    constexpr operator safe_bool () const noexcept
    {
      return valid () ? &self_type::can_exchange : nullptr;
    }

    constexpr void flush () noexcept
    {
        _M_gData.clear ();
        _M_uPos = size_type ();
    }

protected:
    virtual const_pointer on_send (size_type& size);
    virtual void on_receive (const_pointer data, size_type size_in_bytes);

private:
    bool can_exchange (size_type size_in_bytes) const noexcept;

private:
    stream_type _M_gData;
    size_type   _M_uPos ;
};

// =========================================================

} //! namespace network

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_NETWORK_PACKET_H_
