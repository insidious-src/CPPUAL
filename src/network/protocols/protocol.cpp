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


#include <cppual/network/protocols/protocol.h>

namespace cppual { namespace network {

void protocol::add_upper_layer (protocol* layer)
{
    if (_M_pUpperProt != nullptr)
    {
        auto const prev_upper_layer = _M_pUpperProt;

        prev_upper_layer->_M_pLowerProt = layer;
        layer->_M_pUpperProt = prev_upper_layer;
    }
    else
    {
        layer->_M_pUpperProt = nullptr;
    }

    layer->_M_pLowerProt = this;
    _M_pUpperProt = layer;
}

void protocol::add_lower_layer (protocol* layer)
{
    if (_M_pLowerProt != nullptr)
    {
        auto const prev_lower_layer = _M_pLowerProt;

        prev_lower_layer->_M_pUpperProt = layer;
        layer->_M_pLowerProt = prev_lower_layer;
    }
    else
    {
        layer->_M_pLowerProt = nullptr;
    }

    layer->_M_pUpperProt = this;
    _M_pLowerProt = layer;
}

protocol* protocol::uppest_layer () const
{
    if (_M_pUpperProt != nullptr)
    {
        auto pUppestProt     = _M_pUpperProt;
        auto pPrevUppestProt = pUppestProt  ;

        while (pUppestProt != nullptr)
        {
            pPrevUppestProt = pUppestProt;
            pUppestProt     = pUppestProt->upper_layer ();
        }

        return pPrevUppestProt;
    }

    return const_cast<protocol*> (this);
}

protocol* protocol::lowest_layer () const
{
    if (_M_pLowerProt != nullptr)
    {
        auto pLowestProt     = _M_pLowerProt;
        auto pPrevLowestProt = pLowestProt  ;

        while (pLowestProt != nullptr)
        {
            pPrevLowestProt = pLowestProt;
            pLowestProt     = pLowestProt->lower_layer ();
        }

        return pPrevLowestProt;
    }

    return const_cast<protocol*> (this);
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
