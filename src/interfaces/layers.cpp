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

#include <cppual/interfaces/layers.h>

// =========================================================

namespace cppual {

// =========================================================

void layer_interface::add_upper_node (layer_interface* layer)
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

void layer_interface::add_lower_node (layer_interface* layer)
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

layer_interface* layer_interface::uppest_node () const
{
    if (_M_pUpperProt != nullptr)
    {
        auto pUppestProt     = _M_pUpperProt;
        auto pPrevUppestProt = pUppestProt  ;

        while (pUppestProt != nullptr)
        {
            pPrevUppestProt = pUppestProt;
            pUppestProt     = pUppestProt->upper_node ();
        }

        return pPrevUppestProt;
    }

    return const_cast<layer_interface*> (this);
}

layer_interface* layer_interface::lowest_node () const
{
    if (_M_pLowerProt != nullptr)
    {
        auto pLowestProt     = _M_pLowerProt;
        auto pPrevLowestProt = pLowestProt  ;

        while (pLowestProt != nullptr)
        {
            pPrevLowestProt = pLowestProt;
            pLowestProt     = pLowestProt->lower_node ();
        }

        return pPrevLowestProt;
    }

    return const_cast<layer_interface*> (this);
}

// =========================================================

void list_interface::add_next_node (list_interface* layer)
{
    if (_M_pNextProt != nullptr)
    {
        auto const prev_upper_layer = _M_pNextProt;

        prev_upper_layer->_M_pPrevProt = layer;
        layer->_M_pNextProt = prev_upper_layer;
    }
    else
    {
        layer->_M_pNextProt = nullptr;
    }

    layer->_M_pPrevProt = this;
    _M_pNextProt = layer;
}

void list_interface::add_prev_node (list_interface* layer)
{
    if (_M_pPrevProt != nullptr)
    {
        auto const prev_lower_layer = _M_pPrevProt;

        prev_lower_layer->_M_pNextProt = layer;
        layer->_M_pPrevProt = prev_lower_layer;
    }
    else
    {
        layer->_M_pPrevProt = nullptr;
    }

    layer->_M_pNextProt = this;
    _M_pPrevProt = layer;
}

list_interface* list_interface::last_node () const
{
    if (_M_pNextProt != nullptr)
    {
        auto pLastProt     = _M_pNextProt;
        auto pPrevLastProt = pLastProt  ;

        while (pLastProt != nullptr)
        {
            pPrevLastProt = pLastProt;
            pLastProt     = pLastProt->next_node ();
        }

        return pPrevLastProt;
    }

    return const_cast<list_interface*> (this);
}

list_interface* list_interface::first_node () const
{
    if (_M_pPrevProt != nullptr)
    {
        auto pFirstProt     = _M_pPrevProt;
        auto pPrevFirstProt = pFirstProt  ;

        while (pFirstProt != nullptr)
        {
            pPrevFirstProt = pFirstProt;
            pFirstProt     = pFirstProt->prev_node ();
        }

        return pPrevFirstProt;
    }

    return const_cast<list_interface*> (this);
}

// =========================================================

} // cppual