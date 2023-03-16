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

#include <cppual/ui/layout.h>

namespace cppual { namespace ui {

layout::layout   ()
: _M_gViewList   (),
  _M_pOwner      (),
  _M_uNumRows    (),
  _M_uNumColumns ()
{
}

layout::layout(layout&&)
{

}

layout::layout(layout const&)
{

}

layout& layout::operator = (layout&&)
{
    return *this;
}

layout& layout::operator = (layout const&)
{
    return *this;
}

void layout::on_size (rect const&)
{
}

void layout::set_view_metrics (view*, layout_metrics const&)
{
}

void layout::set_padding(const rect&)
{

}

bool layout::add_view (view*, layout_metrics const&)
{
    return false;
}

bool layout::remove_view (view*)
{
    return false;
}

} } // namespace Ui
