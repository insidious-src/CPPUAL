/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

using std::string;

namespace cppual { namespace Ui {

bool Label::create (View*       pParent,
                    string const& gText,
                    Rect const&   gRect,
                    TextFormat)
{
    if (pParent and !pParent->valid ()) pParent = nullptr;

    if (createView (pParent, gRect, pParent ?
                      pParent->renderable_unsafe ()->screen () :
                      0))
    {
        m_gText = gText;
        return true;
    }

    return false;
}

void Label::setText (string const& gText)
{
    m_gText = gText;

    if (isValid ())
    {
        onPaint ();
        signalTextChanged.notify ();
    }
}

void Label::setFormat (TextFormat eFormat)
{
    if (m_eFormat != eFormat)
    {
        m_eFormat = eFormat;

        if (isValid ())
        {
            onPaint ();
            signalTextChanged.notify ();
        }
    }
}

void Label::onPaint ()
{
}

void Label::onEnable (bool)
{
}

void Label::onSize (Rect const&)
{
}

} } // Ui
