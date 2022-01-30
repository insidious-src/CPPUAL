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

#ifndef CPPUAL_UI_TAB_H_
#define CPPUAL_UI_TAB_H_

#include <cppual/signal.h>
#include <cppual/ui/skin.h>

using cppual::gfx::icon;

namespace cppual { namespace ui {

// =========================================================

class tab_bar : public skinnable_view
{
public:
    tab_bar ();
    tab_bar (tab_bar&&);
    tab_bar (tab_bar const&);
    tab_bar& operator = (tab_bar&&);
    tab_bar& operator = (tab_bar const&);

    enum class tab_flag : byte
    {
        movable      = 1 << 0,
        draggable    = 1 << 1,
        removable    = 1 << 2,
        close_button = 1 << 3,
        add_button   = 1 << 4,
        show_icons   = 1 << 5
    };

    typedef bitset<tab_flag> tab_flags;

    tab_bar (view* parent, rect const&);

    bool add_tab (string const&, icon* = nullptr, int pos = -1);
    bool remove_tab (int pos);
    bool move_tab (int pos, int new_pos);
    bool is_movable () const;
    bool is_draggable () const;
    bool is_removable () const;
    bool has_close_buttton () const;
    bool has_add_button () const;
    void set_movable (bool);
    void set_draggable (bool);
    void set_removable (bool);
    void set_tab_label (int pos, string const& text);
    void set_tab_icon (int pos, icon*);
    void set_add_button_flags ();
    void set_close_button_flags ();
    void enable_close_button (bool);
    void enable_add_button (bool);

public:
    signal<void(int)> tab_index_changed;
    signal<void(int)> tab_addded       ;
    signal<void(int)> tab_removed      ;

protected:
    virtual void show_event (bool);
    virtual void paint_event (rect const&);
    virtual void size_event (point2u);
    virtual void focus_event (bool);

    virtual void on_create ();

private:
    tab_flags _M_gTabFlags;
};

// =========================================================

class tab_view : public skinnable_view
{
public:
    tab_view ();
    tab_view (tab_view&&);
    tab_view (tab_view const&);
    tab_view& operator = (tab_view&&);
    tab_view& operator = (tab_view const&);

    view* get_tab_container (int) noexcept;

    tab_bar* get_tab_bar () noexcept
    { return &_M_TabBar; }

protected:
    void on_tab_index_change (int);
    void on_add_tab (int);
    void on_remove_tab (int);

private:
    tab_bar _M_TabBar;
};

// =========================================================

} } // Ui

#endif // CPPUAL_UI_TAB_H_
