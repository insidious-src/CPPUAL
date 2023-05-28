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

#include <cppual/ui/controls/popup.h>

namespace cppual { namespace ui {

class popup_group final
{
};

// ====================================================

popup_menu::popup_menu ()
{
}

popup_menu::popup_menu (popup_menu&& gObj)
: skinnable_view (gObj)
{
}

popup_menu::popup_menu (popup_menu const& gObj)
: skinnable_view (gObj)
{
}

popup_menu& popup_menu::operator = (popup_menu&&)
{
    return *this;
}

popup_menu& popup_menu::operator = (popup_menu const&)
{
    return *this;
}

menu_item_type popup_menu::item_type (int)
{
    return menu_item_type::invalid;
}

popup_menu* popup_menu::sub_menu (int)
{
    return nullptr;
}

popup_menu::command_type* popup_menu::get_command (int)
{
    return nullptr;
}

popup_menu::size_type popup_menu::delay ()
{
    return 0;
}

bool popup_menu::create (point2i)
{
    return false;
}

bool popup_menu::add_command (command_type*, int)
{
    return false;
}

bool popup_menu::add_check (command_type*, int)
{
    return false;
}

bool popup_menu::add_radio (command_type*, int)
{
    return false;
}

bool popup_menu::add_sub_menu (popup_menu*, string const&, int)
{
    return false;
}

bool popup_menu::add_separator (int)
{
    return false;
}

bool popup_menu::add_group (int)
{
    return false;
}

bool popup_menu::add_to_group (int, view*)
{
    return false;
}

bool popup_menu::remove_from_group (int, view*)
{
    return false;
}

bool popup_menu::remove_item (int)
{
    return false;
}

void popup_menu::set_delay (view::size_type)
{
}

void popup_menu::set_position (point2i)
{
}

void popup_menu::set_group_name (int, string const&)
{
}

popup_menu::size_type popup_menu::default_delay ()
{
    return 300;
}

void popup_menu::set_default_delay (size_type)
{
}

void popup_menu::paint_event (rect const&)
{
}

void popup_menu::focus_event (bool is_focused)
{
    if (!is_focused) hide ();
}

void popup_menu::key_pressed_event (event_type::key_data const&)
{
}

void popup_menu::key_released_event (event_type::key_data const&)
{
}

} } // namespace Ui
