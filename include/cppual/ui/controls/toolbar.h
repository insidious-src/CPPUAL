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

#ifndef CPPUAL_UI_TOOLBAR_H_
#define CPPUAL_UI_TOOLBAR_H_

#include <cppual/ui/skin.h>
#include <cppual/ui/groups.h>
#include <cppual/common.h>

namespace cppual { namespace ui {

// =========================================================

enum class tool_style : byte
{
    flat,
    modern
};

// =========================================================

enum class tool_button : byte
{
    text_beside_icon,
    icon_above_text,
    text_only,
    icon_only
};

// =========================================================

class tool_bar : public skinnable_view
{
public:
    enum tool_flag
    {
        auto_hide    = 1 << 0,
        hot_tracking = 1 << 1,
        splitter     = 1 << 2,
        movable      = 1 << 3,
        draggable    = 1 << 4,
        floatable    = 1 << 5
    };

    typedef bitset<tool_flag>   tool_flags      ;
    typedef vector<view_group*> group_vector    ;
    typedef orientation         orientation_type;

    point2u icon_size () const;
    u8      row_count ();

    void set_style (tool_style);
    void set_simple (bool);
    void set_movable (bool);
    void set_draggable (bool);
    void set_floatable (bool);
    void set_button_type (tool_button);
    void set_button_style (tool_style);
    void set_icon_size (point2u const&);
    void set_max_rows (u8);
    void set_orientation (orientation_type);
    void set_auto_hide (bool);
    void set_hot_tracking (bool);
    void enable_splitter (bool);
    void move_group (size_type pos, size_type new_pos);
    void maximize_group (size_type pos);
    void minimize_group (size_type pos);
    void push_chevron (); // auto hide
    bool is_floating () const;
    bool add_group (view_group&, size_type pos);

    inline tool_button button_type () const noexcept
    { return _M_eButtonType; }

    inline orientation_type orientation () const noexcept
    { return _M_eOrientation; }

    inline bool is_movable () const noexcept
    { return _M_eToolFlags.test (tool_bar::movable); }

    inline bool is_draggable () const noexcept
    { return _M_eToolFlags.test (tool_bar::draggable); }

    inline bool is_floatable () const noexcept
    { return _M_eToolFlags.test (tool_bar::floatable); }

    inline bool is_auto_hide () const noexcept
    { return _M_eToolFlags.test (tool_bar::auto_hide); }

    inline bool has_hot_tracking () const noexcept
    { return _M_eToolFlags.test (tool_bar::hot_tracking); }

    inline bool has_splitter () const noexcept
    { return _M_eToolFlags.test (tool_bar::splitter); }

    inline size_type group_count () const noexcept
    { return _M_gGroups.size (); }

    inline view_group* group (size_type uId) const noexcept
    { return _M_gGroups[uId]; }

public:
    signal<void()> clicked            ;
    signal<void()> double_clicked     ;
    signal<void()> right_clicked      ;
    signal<void()> child_resized      ;
    signal<void()> group_added        ;
    signal<void()> group_removed      ;
    signal<void()> drag_begun         ;
    signal<void()> drag_ended         ;
    signal<void()> chevron_pushed     ;
    signal<void()> height_changed     ;
    signal<void()> splitter_dragging  ;
    signal<void()> orientation_changed;

protected:
    virtual void paint_event (rect const&);
    virtual void size_event  (point2u);
    virtual void move_event  (point2i);
    virtual void focus_event (bool);
    virtual void on_enable   (bool);

private:
    group_vector     _M_gGroups     ;
    tool_button      _M_eButtonType ;
    tool_flags       _M_eToolFlags  ;
    orientation_type _M_eOrientation;
};

// =========================================================

} } // Ui

#endif // CPPUAL_UI_TOOLBAR_H_
