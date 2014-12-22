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

#include <cppual/ui/controls/popup.h>

namespace cppual { namespace Ui {

class PopupGroup final
{
};

// ====================================================

PopupMenu::PopupMenu ()
{
}

PopupMenu::PopupMenu (PopupMenu&& gObj)
: SkinnableView (gObj)
{
}

PopupMenu::PopupMenu (PopupMenu const& gObj)
: SkinnableView (gObj)
{
}

PopupMenu& PopupMenu::operator = (PopupMenu&&)
{
	return *this;
}

PopupMenu& PopupMenu::operator = (PopupMenu const&)
{
	return *this;
}

MenuItemType PopupMenu::itemType (int)
{
	return MenuItemType::Invalid;
}

PopupMenu* PopupMenu::subMenu (int)
{
	return nullptr;
}

Command* PopupMenu::command (int)
{
	return nullptr;
}

PopupMenu::size_type PopupMenu::delay ()
{
	return 0;
}

bool PopupMenu::create (point2i)
{
	return false;
}

bool PopupMenu::addCommand (Command*, int)
{
	return false;
}

bool PopupMenu::addCheck (Command*, int)
{
	return false;
}

bool PopupMenu::addRadio (Command*, int)
{
	return false;
}

bool PopupMenu::addSubMenu (PopupMenu*, string const&, int)
{
	return false;
}

bool PopupMenu::addSeparator (int)
{
	return false;
}

bool PopupMenu::addGroup (int)
{
	return false;
}

bool PopupMenu::addToGroup (int, View*)
{
	return false;
}

bool PopupMenu::removeFromGroup (int, View*)
{
	return false;
}

bool PopupMenu::removeItem (int)
{
	return false;
}

void PopupMenu::setDelay (View::size_type)
{
}

void PopupMenu::setPosition (point2i)
{
}

void PopupMenu::setGroupName (int, string const&)
{
}

PopupMenu::size_type PopupMenu::defaultDelay ()
{
	return 0;
}

void PopupMenu::setDefaultDelay (size_type)
{
}

void PopupMenu::onPaint (Rect const&)
{
}

void PopupMenu::onFocusKilled ()
{
	hide ();
}

void PopupMenu::onKeyPress (u8)
{
}

void PopupMenu::onKeyRelease (u8)
{
}

} } // namespace Ui
