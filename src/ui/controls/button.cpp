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

#include <cppual/ui/controls/button.h>

namespace cppual { namespace Ui {

void AbstractButton::setLabel (string const& gText)
{
    m_gLabel = gText;
    if (valid () and isHidden ()) refresh ();
}

AbstractButton::~AbstractButton () noexcept
{
}

// =========================================================

bool PushButton::create (View*, string const&)
{
    return true;
}

void PushButton::press ()
{
}

void PushButton::release ()
{
}

void PushButton::onPaint (Rect const&)
{
//    Rect gBtnRect (getSurface ()->getRect ());
//    IAbstractImage* pBackImage = m_pSkin->getImage (SkinElement::Button, 0);
//    CPaintWorker cPainter (m_pSurface, true);

//    if (!cPainter.IsActive ()) return;
//    cPainter.SetActiveObject (pBackImage);
//    cPainter.DrawImage (gBtnRect);
//    cPainter.SetActiveObject (&m_gFont);
    //    cPainter.DrawText (gBtnRect, &m_Label);
}

void PushButton::onSize (point2u)
{
}

void PushButton::onFocus (bool)
{
}

// =========================================================

bool RadioBox::create (View*, string const&, bool)
{
    return false;
}

void RadioBox::select ()
{
}

void RadioBox::clear ()
{
}

void RadioBox::onPaint (Rect const&)
{
}

void RadioBox::onSize (point2u)
{
}

void RadioBox::onFocus (bool)
{
}

// =========================================================

bool CheckBox::create (View*, string const&, CheckState)
{
    return false;
}

void CheckBox::setCheckState (CheckState)
{
}

void CheckBox::onPaint (Rect const&)
{
}

void CheckBox::onSize (point2u)
{
}

void CheckBox::onFocus (bool)
{
}

} } // namespace Ui
