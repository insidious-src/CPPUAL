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

#include <cppual/ui/window.h>
#include <cppual/ui/manager.h>

namespace cppual { namespace ui {

namespace { namespace internal {



} } // anonymous namespace Internal

// =========================================================

window::window (view* parent,
                rect const& rect,
                string_type const& title,
                window::image_type*,
                u32 screen)
: view(parent, rect, screen)
{
    set_title(title);
}



window::window ()
: view()
{
}

window::window(window const&)
: view()
{
}

window::~window ()
{
}

window& window::operator = (window const&)
{
    return *this;
}

bool window::set_icon(window::image_type*)
{
    return false;
}

void window::set_title (string_type const& strTitle)
{
    renderable_unsafe ()->setTitle (strTitle);
}

void window::set_fullscreen(bool bFullscreen)
{
    renderable_unsafe ()->setFullscreen (bFullscreen);
}

void window::flash(ushort)
{

}

void window::show_in_taskbar(bool)
{
}

void window::restore()
{

}

void window::minimize()
{

}

void window::maximize()
{

}

void window::close()
{

}

window::string_type window::title() const
{
    return renderable_unsafe ()->title ();
}

} } // namespace Ui
