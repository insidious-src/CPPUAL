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

#include <unordered_map>
#include <cppual/ui/window.h>
#include <cppual/ui/manager.h>

using std::make_pair;
using std::unordered_map;

namespace cppual { namespace Ui {

namespace { namespace Internal {



} } // anonymous namespace Internal

// =========================================================

Window::Window (View* parent,
                Rect const& rect,
                string const& title,
                Window::image_type*,
                u32 screen)
: View(parent, rect, screen)
{
    setTitle(title);
}

Window::Window ()
: View()
{
}

Window::~Window ()
{
}

void Window::setTitle (string_type const& strTitle)
{
    renderable_unsafe ()->setTitle (strTitle);
}

void Window::setFullscreen(bool bFullscreen)
{
    renderable_unsafe ()->setFullscreen (bFullscreen);
}

void Window::showInTaskbar (bool)
{
}

Window::string_type Window::title () const
{
    return renderable_unsafe ()->title ();
}

} } // namespace Ui
