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

#include <cppual/ui/controls/button.h>

namespace cppual { namespace ui {

void abstract_button::set_label (string_type const& gText)
{
    _M_gLabel = gText;
    if (valid () and is_hidden ()) refresh ();
}

abstract_button::~abstract_button () noexcept
{
}

// =========================================================

push_button::push_button (view*, string_type const&)
{
}

void push_button::press ()
{
}

void push_button::release ()
{
}

void push_button::paint_event (rect const&)
{
//    Rect gBtnRect (getSurface ()->getRect ());
//    IAbstractImage* pBackImage = _M_pSkin->getImage (SkinElement::Button, 0);
//    CPaintWorker cPainter (_M_pSurface, true);

//    if (!cPainter.IsActive ()) return;
//    cPainter.SetActiveObject (pBackImage);
//    cPainter.DrawImage (gBtnRect);
//    cPainter.SetActiveObject (&_M_gFont);
    //    cPainter.DrawText (gBtnRect, &_M_Label);
}

void push_button::size_event (point2u)
{
}

void push_button::focus_event (bool)
{
}

// =========================================================

radio_box::radio_box (view*, string_type const&, bool)
{
}

void radio_box::select ()
{
}

void radio_box::clear ()
{
}

void radio_box::paint_event (rect const&)
{
}

void radio_box::size_event (point2u)
{
}

void radio_box::focus_event (bool)
{
}

// =========================================================

check_box::check_box (view*, string_type const&, check_state)
{
}

void check_box::set_state (check_state)
{
}

void check_box::paint_event (rect const&)
{
}

void check_box::size_event (point2u)
{
}

void check_box::focus_event (bool)
{
}

} } // namespace Ui
