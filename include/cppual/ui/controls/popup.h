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

#ifndef CPPUAL_UI_CTL_POPUP_H_
#define CPPUAL_UI_CTL_POPUP_H_

#include <cppual/ui/cmd.h>
#include <cppual/ui/layout.h>
#include <cppual/ui/controls/button.h>

namespace cppual { namespace ui {

// =========================================================

enum class menu_item_type : u8
{
    invalid,
    command,
    separator,
    parent,
    radio,
    check,
    group
};

// ====================================================

class popup_menu : public skinnable_view
{
public:
    typedef command command_type;
    typedef string  string_type ;

    union item_data
    {
        command_type* action ;
        check_box*    check  ;
        radio_box*    radio  ;
        popup_menu*   subMenu;
    };

    popup_menu ();
    popup_menu (popup_menu&&);
    popup_menu (popup_menu const&);
    popup_menu& operator = (popup_menu&&);
    popup_menu& operator = (popup_menu const&);

    menu_item_type item_type (int pos);
    popup_menu*    sub_menu (int menu_pos);
    command_type*  get_command (int action_pos);
    size_type      delay ();
    bool           create (point2i);
    bool           add_command (command_type*, int pos = -1);
    bool           add_check (command_type*, int pos = -1);
    bool           add_radio (command_type*, int pos = -1);
    bool           add_sub_menu (popup_menu*, string_type const&, int pos = -1);
    bool           add_separator (int pos = -1);
    bool           add_group (int pos = -1);
    bool           add_to_group (int group_pos, view*);
    bool           remove_from_group (int group_pos, view*);
    bool           remove_item (int pos);
    void           set_delay (size_type ms);
    void           set_position (point2i);
    void           set_group_name (int group_pos, string_type const&);

    static size_type default_delay     ();
    static void      set_default_delay (size_type ms);

    inline size_type item_count () const noexcept
    { return _M_gItemList.size (); }

private:
    struct menu_item
    {
        item_data      data;
        menu_item_type type;
    };

    typedef vector<menu_item*> vector_type;

private:
    void paint_event (rect const&);
    void focus_event (bool);
    void key_pressed_event (event_type::key_data const&);
    void key_released_event (event_type::key_data const&);

    vector_type _M_gItemList;

};

// =========================================================

} } // namespace Ui

#endif // CPPUAL_UI_CTL_POPUP_H_
