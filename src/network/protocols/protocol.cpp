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


#include <cppual/network/protocols/protocol.h>

namespace cppual { namespace network {

Protocol* Protocol::lowest_layer () const
{
    if (_M_pLowerProt != nullptr)
    {
        Protocol* pLowestProt = _M_pLowerProt->lower_layer ();

        while (pLowestProt != nullptr) pLowestProt = pLowestProt->lower_layer ();
        return pLowestProt;
    }

    return nullptr;
}

uint Protocol::required_output_size (uint)
{
    return 0;
}

uint Protocol::required_recyclable_streams (uint, uint)
{
    return 0;
}

} } // Network
