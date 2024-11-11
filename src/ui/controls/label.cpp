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

#include <cppual/ui/controls/label.h>

namespace cppual { namespace ui {

label::label (view*            pParent,
              string_type const& gText,
              rect const&        gRect,
              text_format)
: skinnable_view(pParent, gRect, pParent ?
                pParent->renderable_unsafe ()->screen () : 0),
  _M_gText(gText)
{ }

void label::set_text (string_type const& gText)
{
    _M_gText = gText;

    if (valid ())
    {
        refresh ();
        text_changed();
    }
}

void label::set_format (text_format eFormat)
{
    if (_M_eFormat != eFormat)
    {
        _M_eFormat = eFormat;

        if (valid ())
        {
            refresh();
            text_changed();
        }
    }
}

void label::paint_event (rect const&)
{
}

void label::size_event (point2u)
{
}

void label::on_enable (bool)
{
}

} } // Ui
