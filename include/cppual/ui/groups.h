/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef CPPUAL_UI_GROUPS_H_
#define CPPUAL_UI_GROUPS_H_

#include <cppual/ui/widget.h>
#include <cppual/ui/action.h>

using cppual::Text::string;

namespace cppual { namespace Ui {

class DataGroup
{
};

class ViewGroup
{
public:
	enum class ItemType : unsigned char
	{
		Action,
		Separator,
		View
	};

	void setLabel (string const& text);
	void addSeparator (int pos = -1);
	bool addView (View*, int pos = -1);
	bool addAction (ActionItem const&, int pos = -1);
	bool moveItem (int pos, int new_pos);
	bool removeItem (int pos);

	ItemType itemType (int pos) const;
	uint     itemCount () const;
};

} } // namespace Ui

#endif // CPPUAL_UI_GROUPS_H_
