/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#include <cppual/network/address.h>
#include <sstream>

using std::string;

namespace cppual { namespace Network {

cu16 Address::v4_mapped_prefix[12] =
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff };

Address::Address (string const&) noexcept
{
}

string Address::toString () const noexcept
{
    std::stringstream output;

    if (!isV4 ())
    {
        for (uint i = 0; i < 7; ++i) output << m_uBytes[i] << ':';
        output << m_uBytes[7];
    }
    else
    {
        for (uint i = 0; i < 2; ++i) output << m_uBytes[i] << '.';
        output << m_uBytes[3];
    }

    return std::move (output.str ());
}

Address Address::localAddress () noexcept
{
    return Address ();
}

Address Address::publicAddress () noexcept
{
    return Address ();
}

} } // namespace Network
