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

#ifndef CPPUAL_NETWORK_URI_H_
#define CPPUAL_NETWORK_URI_H_
#ifdef __cplusplus

#include <cppual/network/uri_parts.h>
#include <cppual/string.h>

namespace cppual { namespace Network {

class Uri
{
public:
    typedef string::value_type     value_type;
    typedef value_type const*           const_pointer;
    typedef std::size_t                 size_type;
    typedef string                 string_type;
    typedef string_type::const_iterator iterator;
    typedef string_type::const_iterator const_iterator;

    inline Uri () = default;
    inline Uri (Uri&&) noexcept = default;
    inline Uri (Uri const&) = default;
    inline Uri& operator = (Uri&&) = default;

    bool isValid    () const;
    bool isAbsolute () const;

    inline const_iterator begin () const noexcept
    { return m_gUri.begin (); }

    inline const_iterator end   () const noexcept
    { return m_gUri.end   (); }

    inline string_type protocol () const noexcept
    { return m_gUri.substr (0, m_uProtEnd); }

    inline string_type userInfo () const
    { return m_gUri.substr (m_uProtEnd + 1, m_uUserNfoEnd); }

    inline string_type host () const
    { return m_gUri.substr (m_uUserNfoEnd + 1, m_uHostEnd); }

    inline string_type port () const
    { return m_gUri.substr (m_uHostEnd + 1, m_uPortEnd); }

    inline string_type path () const
    { return m_gUri.substr (m_uPortEnd + 1, m_uPathEnd); }

    inline string_type query () const
    { return m_gUri.substr (m_uPathEnd + 1, m_uQueryEnd); }

    inline string_type fragment () const
    { return m_gUri.substr (m_uQueryEnd + 1, m_gUri.length () - 1); }

    template <typename T>
    inline Uri (T const&);

    inline const_pointer c_str () const noexcept
    { return m_gUri.c_str (); }

    inline operator string_type const& () const noexcept
    { return m_gUri;          }

    inline string_type toString () const
    { return string_type ();  }

    inline std::wstring toWString () const
    { return std::wstring ();      }

    inline std::u16string toU16string () const
    { return std::u16string ();    }

    inline std::u32string toU32string () const
    { return std::u32string ();    }

    void swap (Uri& gObj) noexcept
    {
        m_gUri.swap (gObj.m_gUri);
        std::swap_ranges (&m_uProtEnd, &m_uQueryEnd, &gObj.m_uProtEnd);
    }

private:
    string_type m_gUri;
    size_type   m_uProtEnd;
    size_type   m_uUserNfoEnd;
    size_type   m_uHostEnd;
    size_type   m_uPortEnd;
    size_type   m_uPathEnd;
    size_type   m_uQueryEnd;
};

bool operator == (Uri const& lhs, Uri const& rhs);
bool operator != (Uri const& lhs, Uri const& rhs);
bool operator <  (Uri const& lhs, Uri const& rhs);
bool operator <= (Uri const& lhs, Uri const& rhs);
bool operator >  (Uri const& lhs, Uri const& rhs);
bool operator >= (Uri const& lhs, Uri const& rhs);

std::ostream & operator << (std::ostream & os, Uri const& u);
std::wostream& operator << (std::wostream& os, Uri const& u);
std::istream & operator >> (std::istream & os, Uri& u);
std::wistream& operator >> (std::wistream& os, Uri& u);

} } // namespace Network

#endif // __cplusplus
#endif // CPPUAL_NETWORK_URI_H_
