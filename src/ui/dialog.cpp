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

#include <cppual/ui/dialog.h>

namespace cppual { namespace ui {

dialog& dialog::operator = (dialog&& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

dialog& dialog::operator = (dialog const& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

dialog::result dialog::get_result() noexcept
{
    return dialog::result::close;
}

void dialog::show_modal (ui::window* pOwner) noexcept
{
    if (valid ())
    {
        if (pOwner and pOwner->valid ())
        {
            _M_pOwnerWnd = pOwner;
            pOwner->disable ();
        }

        show ();
        get_focus ();
    }
}

void dialog::on_create ()
{
    on_init ();
}

void dialog::close_event ()
{
    on_end ();

    if (_M_pOwnerWnd)
    {
        _M_pOwnerWnd->enable ();
        _M_pOwnerWnd->get_focus ();
        _M_pOwnerWnd = nullptr;
    }
}

} } // namespace Ui
