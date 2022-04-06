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

#include <cppual/network/address.h>

#include <sstream>

namespace cppual { namespace network {

address::address (string_type const&) noexcept
{
}

address::string_type address::to_string () const noexcept
{
    stringstream output;

    if (!is_v4 ())
    {
        for (auto i = 0U; i < 7; ++i) output << _M_uBytes[i] << ':';
        output << _M_uBytes[7];
    }
    else
    {
        for (auto i = 0U; i < 2; ++i) output << _M_uBytes[i] << '.';
        output << _M_uBytes[3];
    }

    return output.str ();
}

address address::local_address () noexcept
{
    return address ();
}

address address::public_address () noexcept
{
    return address ();
}

} } // namespace Network
