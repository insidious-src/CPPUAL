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

namespace cppual { namespace ui {

namespace { namespace internal {



} } // anonymous namespace Internal

// =========================================================

window::window (view* parent,
                rect const& rect,
                string_type const& title,
                window::image_type* icon,
                u32 screen,
                resource_type* rc)
: view (parent, rect, screen, rc),
  _M_gFrame (),
  _M_pIcon ()
{
    set_title (title);
    set_icon (icon);
}

window::window ()
: view (),
  _M_gFrame (),
  _M_pIcon ()
{
}

window::window (window const& obj)
: view (obj),
  _M_gFrame (),
  _M_pIcon ()
{
}

window::~window ()
{
}

void window::show_minimized ()
{
    renderable_unsafe ()->map_minimized ();
}

void window::show_maximized ()
{
    renderable_unsafe ()->map_maximized ();
}

window& window::operator = (window const& obj)
{
    view::operator = (obj);
    return *this;
}

bool window::set_icon (window::image_type*)
{
    return false;
}

void window::set_title (string_type const& name)
{
    renderable_unsafe ()->set_title (name);
}

void window::set_fullscreen (bool bFullscreen)
{
    renderable_unsafe ()->set_fullscreen (bFullscreen);
}

void window::flash (uint count)
{
    renderable_unsafe ()->flash (count);
}

void window::show_in_taskbar (bool bVis)
{
    renderable_unsafe ()->set_visible_in_taskbar (bVis);
}

bool window::is_visible_in_taskbar () const
{
    return renderable_unsafe ()->is_visible_in_taskbar ();
}

void window::show_in_pager (bool bVis)
{
    renderable_unsafe ()->set_visible_in_pager (bVis);
}

bool window::is_visible_in_pager () const
{
    return renderable_unsafe ()->is_visible_in_pager ();
}

void window::set_shaded (bool bShaded)
{
    renderable_unsafe ()->set_shaded (bShaded);
}

bool window::is_shaded () const
{
    return renderable_unsafe ()->is_shaded ();
}

void window::keep_above (bool above)
{
    renderable_unsafe ()->keep_above (above);
}

bool window::is_above () const
{
    return renderable_unsafe ()->is_above ();
}

void window::keep_below (bool below)
{
    renderable_unsafe ()->keep_below (below);
}

bool window::is_below () const
{
    return renderable_unsafe ()->is_below ();
}

void window::restore ()
{
    if (renderable_unsafe ()->is_minimized ()) renderable_unsafe ()->set_minimized (false);
    else if (renderable_unsafe ()->is_maximized ()) renderable_unsafe ()->set_maximized (false);
}

void window::minimize ()
{
    renderable_unsafe ()->set_minimized (true);
}

void window::maximize ()
{
    renderable_unsafe ()->set_maximized (true);
}

void window::close ()
{

}

window::string_type window::title () const
{
    return renderable_unsafe ()->title ();
}

bool window::is_minimized () const noexcept
{
    return renderable_unsafe ()->is_minimized ();

    //return _M_gFrame && _M_gFrame->attached () == this && _M_gFrame->is_hidden ();
}

bool window::is_maximized () const noexcept
{
    return renderable_unsafe ()->is_maximized ();

    //return _M_gFrame && _M_gFrame->attached () == this && _M_gFrame->is_stretched ();
}

} } // namespace Ui
