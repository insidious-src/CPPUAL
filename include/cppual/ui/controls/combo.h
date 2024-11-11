/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#ifndef CPPUAL_UI_COMBO_H_
#define CPPUAL_UI_COMBO_H_

#include <cppual/ui/skin.h>
#include <cppual/ui/cmd.h>


namespace cppual { namespace ui {

// =========================================================

class combo_box : public skinnable_view
{
public:
    typedef gfx::image_interface image_type  ;
    typedef command              command_type;

    combo_box ();
    combo_box (combo_box&&);
    combo_box (combo_box const&);
    combo_box& operator = (combo_box&&);
    combo_box& operator = (combo_box const&);

    uint item_count ();
    int  current_index ();
    bool add_item (command_type*, int pos = -1);
    bool remove_item (int pos);
    bool is_editable () const;
    void set_editable (bool);
    void set_text_limit (size_type max_chars_shown);
    void set_drop_list_width (int);
    void set_item_icon (int, image_type*);
    void set_item_text (int, string const&);
    void set_item_desc (int, string const&);
    void set_item_height (int);
    void show_drop_list ();
    void hide_drop_list ();
    void clear ();

    bool add_item (string_type const& text,
                   string_type const& desc,
                   image_type*        icon = nullptr,
                   int                pos  = -1);

public:
    signal<void(int)>  index_changed    ;
    signal<void(int)>  item_added       ;
    signal<void(int)>  item_removed     ;
    signal<void(int)>  item_text_changed;
    signal<void(int)>  item_icon_changed;
    signal<void(int)>  item_desc_changed;
    signal<void()>     double_clicked   ;
    signal<void(bool)> dropped_down     ;

protected:
    virtual void show_event         (bool);
    virtual void paint_event        (rect const&);
    virtual void size_event         (point2u);
    virtual void focus_event        (bool);
    virtual void key_pressed_event  (event_type::key_data const&);
    virtual void key_released_event (event_type::key_data const&);

    virtual void on_enable (bool);
};

// =========================================================

} } // Ui

#endif // CPPUAL_UI_COMBO_H_
