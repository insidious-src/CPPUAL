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

void protocol::add_upper_layer (protocol* layer)
{
    if (_M_pUpperProt != nullptr)
    {
        auto const lower_layer = _M_pUpperProt;

        _M_pUpperProt = layer;
        _M_pUpperProt->lowest_layer ()->add_lower_layer (lower_layer);
    }
    else
    {
        _M_pUpperProt = layer;
    }
}

void protocol::add_lower_layer (protocol* layer)
{
    if (_M_pLowerProt != nullptr)
    {
        auto const lower_layer = _M_pLowerProt;

        _M_pLowerProt = layer;
        _M_pLowerProt->add_upper_layer (lower_layer);
    }
    else
    {
        _M_pLowerProt = layer;
    }
}

protocol* protocol::uppest_layer () const
{
    if (_M_pUpperProt != nullptr)
    {
        auto pUppestProt = _M_pUpperProt;

        while (pUppestProt != nullptr) pUppestProt = pUppestProt->upper_layer ();
        return pUppestProt;
    }

    return nullptr;
}

protocol* protocol::lowest_layer () const
{
    if (_M_pLowerProt != nullptr)
    {
        auto pLowestProt = _M_pLowerProt;

        while (pLowestProt != nullptr) pLowestProt = pLowestProt->lower_layer ();
        return pLowestProt;
    }

    return nullptr;
}

uint protocol::required_output_size (uint)
{
    return 0;
}

uint protocol::required_recyclable_streams (uint, uint)
{
    return 0;
}

} } // Network
