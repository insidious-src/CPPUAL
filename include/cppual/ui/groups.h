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

#ifndef CPPUAL_UI_GROUPS_H_
#define CPPUAL_UI_GROUPS_H_

#include <cppual/ui/view.h>
#include <cppual/ui/cmd.h>

namespace cppual { namespace ui {

// =========================================================

class data_group
{
};

// =========================================================

class view_group
{
public:
    typedef string  string_type ;
    typedef command command_type;

    enum class type : byte
    {
        action,
        separator,
        view
    };

    void set_label (string_type const& text);
    void add_separator (int pos = -1);
    bool add_view (view*, int pos = -1);
    bool add_command (command_type const&, int pos = -1);
    bool move_item (int pos, int new_pos);
    bool remove_item (int pos);

    type item_type (int pos) const;
    uint item_count () const;
};

// =========================================================

} } // namespace Ui

#endif // CPPUAL_UI_GROUPS_H_
