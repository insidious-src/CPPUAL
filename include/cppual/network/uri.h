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

#ifndef CPPUAL_NETWORK_URI_H_
#define CPPUAL_NETWORK_URI_H_
#ifdef __cplusplus

#include <cppual/network/uri_parts.h>
#include <cppual/string.h>

#define _LIBCPP_DISABLE_DEPRECATION_WARNINGS
#include <locale>
#include <codecvt>

namespace cppual { namespace network {

class uri
{
public:
    typedef string                      string_type   ;
    typedef string_type::value_type     value_type    ;
    typedef value_type const*           const_pointer ;
    typedef std::size_t                 size_type     ;
    typedef string_type::const_iterator iterator      ;
    typedef string_type::const_iterator const_iterator;

    inline uri () = default;
    inline uri (uri&&) noexcept = default;
    inline uri (uri const&) = default;
    inline uri& operator = (uri&&) = default;
    inline uri& operator = (uri const&) = default;

    bool valid       () const;
    bool is_absolute () const;

    inline const_iterator begin () const noexcept
    { return _M_gUri.begin (); }

    inline const_iterator end () const noexcept
    { return _M_gUri.end (); }

    inline string_type protocol () const noexcept
    { return _M_gUri.substr (0, _M_uProtEnd + 1); }

    inline string_type user_info () const
    { return _M_gUri.substr (_M_uProtEnd + 1, _M_uUserNfoEnd - _M_uProtEnd); }

    inline string_type host () const
    { return _M_gUri.substr (_M_uUserNfoEnd + 1, _M_uHostEnd - _M_uUserNfoEnd); }

    inline string_type port () const
    { return _M_gUri.substr (_M_uHostEnd + 1, _M_uPortEnd - _M_uHostEnd); }

    inline string_type path () const
    { return _M_gUri.substr (_M_uPortEnd + 1, _M_uPathEnd - _M_uPortEnd); }

    inline string_type query () const
    { return _M_gUri.substr (_M_uPathEnd + 1, _M_uQueryEnd - _M_uPathEnd); }

    inline string_type fragment () const
    { return _M_gUri.substr (_M_uQueryEnd + 1); }

    template <typename T>
    inline uri (T const&);

    inline const_pointer c_str () const noexcept
    { return _M_gUri.c_str (); }

    inline operator string_type const& () const noexcept
    { return _M_gUri; }

    inline string_type to_string () const
    {
        return _M_gUri;
    }

    inline wstring to_wstring () const
    {
        return std::wstring_convert<
                   std::codecvt_utf8<wchar_t>,
                   wchar_t,
                   memory::allocator<wchar_t>,
                   memory::allocator<char>
                   >().from_bytes(_M_gUri);
    }

    inline u16string to_u16string () const
    {
        return std::wstring_convert<
                   std::codecvt_utf8<char16_t>,
                   char16_t,
                   memory::allocator<char16_t>,
                   memory::allocator<char>
                   >().from_bytes(_M_gUri);
    }

    inline u32string to_u32string () const
    {
        return std::wstring_convert<
                   std::codecvt_utf8<char32_t>,
                   char32_t,
                   memory::allocator<char32_t>,
                   memory::allocator<char>
                   >().from_bytes(_M_gUri);
    }

    void swap (uri& gObj) noexcept
    {
        _M_gUri.swap (gObj._M_gUri);
        std::swap_ranges (&_M_uProtEnd, &_M_uQueryEnd, &gObj._M_uProtEnd);
    }

private:
    string_type _M_gUri        { };
    size_type   _M_uProtEnd    { };
    size_type   _M_uUserNfoEnd { };
    size_type   _M_uHostEnd    { };
    size_type   _M_uPortEnd    { };
    size_type   _M_uPathEnd    { };
    size_type   _M_uQueryEnd   { };
};

bool operator == (uri const& lhs, uri const& rhs);
bool operator != (uri const& lhs, uri const& rhs);
bool operator <  (uri const& lhs, uri const& rhs);
bool operator <= (uri const& lhs, uri const& rhs);
bool operator >  (uri const& lhs, uri const& rhs);
bool operator >= (uri const& lhs, uri const& rhs);

std::ostream & operator << (std::ostream & os, uri const& u);
std::wostream& operator << (std::wostream& os, uri const& u);
std::istream & operator >> (std::istream & os, uri& u);
std::wistream& operator >> (std::wistream& os, uri& u);

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_URI_H_
