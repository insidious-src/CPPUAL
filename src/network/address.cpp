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

#include <cppual/network/address.h>
#include <sstream>

using std::string;

namespace cppual { namespace Network {

Address::Address (string_type const&) noexcept
{
}

Address::string_type Address::toString () const noexcept
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

    return output.str ();
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
