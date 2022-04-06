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

#ifndef CPPUAL_UI_CTL_BUTTON_H_
#define CPPUAL_UI_CTL_BUTTON_H_
#ifdef __cplusplus

#include <cppual/ui/skin.h>
#include <cppual/string.h>

namespace cppual { namespace ui {

// =========================================================

class abstract_button : public skinnable_view
{
public:
    void set_label (string_type const&);
    ~abstract_button () noexcept;

    inline string_type label () const noexcept
    { return _M_gLabel; }

protected:
    string _M_gLabel;
};

// =========================================================

class push_button : public abstract_button
{
public:
    push_button (view* parent, string const& text);
    void press ();
    void release ();

    inline bool is_pushed () const noexcept
    { return _M_bIsPushed; }

protected:
    void paint_event (rect const&);
    void size_event (point2u);
    void focus_event (bool);

private:
    bool _M_bIsPushed;
};

// =========================================================

class radio_box : public abstract_button
{
public:
    radio_box (view* parent, string const& text, bool selected = false);
    void select ();
    void clear ();

    inline bool is_selected () const noexcept
    { return _M_bIsSelected; }

protected:
    void paint_event (rect const&);
    void size_event (point2u);
    void focus_event (bool);

private:
    bool _M_bIsSelected;
};

// =========================================================

enum class check_state : byte
{
    unchecked,
    checked,
    intermediate
};

// =========================================================

class check_box : public abstract_button
{
public:
    check_box (view*         parent,
               string const& text  ,
               check_state   state = check_state::unchecked);

    void set_state (check_state);

    inline check_state state () const noexcept
    { return _M_eCheckState; }

protected:
    void paint_event (rect const&);
    void size_event  (point2u);
    void focus_event (bool);

private:
    check_state _M_eCheckState;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_CTL_BUTTON_H_
