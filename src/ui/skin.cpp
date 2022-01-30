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

#include <cppual/ui/skin.h>
#include <cppual/ui/queue.h>

namespace cppual { namespace ui {

// =========================================================

bool skin_interface::set_default (skin_interface* pSkin) noexcept
{
    if (!pSkin) return false;

    event_queue::events ().winPaint (window_type (),
                                     input::paint_event (rect ()).data ().paint);
    return true;
}

skin_interface* skin_interface::get_default () noexcept
{
    return nullptr;
}

// =========================================================

skinnable_view::skinnable_view ()
: _M_pSkin ()
{
}

skinnable_view::skinnable_view (skinnable_view&& gObj)
: view (gObj),
  _M_pSkin (gObj._M_pSkin)
{
}

skinnable_view::skinnable_view (skinnable_view const& gObj)
: view (gObj),
  _M_pSkin (gObj._M_pSkin)
{
}

skinnable_view& skinnable_view::operator = (skinnable_view&& gObj)
{
    if (this == &gObj) return *this;
    return *this;
}

skinnable_view& skinnable_view::operator = (skinnable_view const& gObj)
{
    if (this == &gObj) return *this;
    return *this;
}

void skinnable_view::set_skin (skin_interface* pNewSkin) noexcept
{
    _M_pSkin = pNewSkin;
    if (valid ()) refresh ();
}

void skinnable_view::change_skin ()
{
    refresh ();
    on_skin_change ();
}

} } // namespace Ui
