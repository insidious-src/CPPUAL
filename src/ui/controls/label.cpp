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

#include <cppual/ui/controls/label.h>

namespace cppual { namespace Ui {

Label::Label (View*            pParent,
              string_type const& gText,
              Rect const&        gRect,
              TextFormat)
: SkinnableView(pParent, gRect, pParent ?
                pParent->renderable_unsafe ()->screen () : 0),
  m_gText(gText)
{ }

void Label::setText (string const& gText)
{
    m_gText = gText;

    if (valid ())
    {
        refresh ();
        textChanged();
    }
}

void Label::setFormat (TextFormat eFormat)
{
    if (m_eFormat != eFormat)
    {
        m_eFormat = eFormat;

        if (valid ())
        {
            refresh();
            textChanged();
        }
    }
}

void Label::paintEvent (Rect const&)
{
}

void Label::onEnable (bool)
{
}

void Label::sizeEvent (point2u)
{
}

} } // Ui
