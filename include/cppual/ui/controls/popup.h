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

#ifndef CPPUAL_UI_CTL_POPUP_H_
#define CPPUAL_UI_CTL_POPUP_H_

#include <cppual/ui/cmd.h>
#include <cppual/ui/layout.h>
#include <cppual/ui/controls/button.h>

namespace cppual { namespace Ui {

enum class MenuItemType : unsigned char
{
    Invalid,
    Command,
    Separator,
    Parent,
    Radio,
    Check,
    Group
};

// ====================================================

class PopupMenu : public SkinnableView
{
public:
    union ItemData
    {
        Command  * action ;
        CheckBox * check  ;
        RadioBox * radio  ;
        PopupMenu* subMenu;
    };

    typedef string string_type;

    PopupMenu ();
    PopupMenu (PopupMenu&&);
    PopupMenu (PopupMenu const&);
    PopupMenu& operator = (PopupMenu&&);
    PopupMenu& operator = (PopupMenu const&);

    MenuItemType itemType (int pos);
    PopupMenu*   subMenu (int menu_pos);
    Command*     command (int action_pos);
    size_type    delay ();
    bool         create (point2i);
    bool         addCommand (Command*, int pos = -1);
    bool         addCheck (Command*, int pos = -1);
    bool         addRadio (Command*, int pos = -1);
    bool         addSubMenu (PopupMenu*, string const&, int pos = -1);
    bool         addSeparator (int pos = -1);
    bool         addGroup (int pos = -1);
    bool         addToGroup (int group_pos, View*);
    bool         removeFromGroup (int group_pos, View*);
    bool         removeItem (int pos);
    void         setDelay (size_type ms);
    void         setPosition (point2i);
    void         setGroupName (int group_pos, string const&);

    static size_type defaultDelay    ();
    static void      setDefaultDelay (size_type ms);

    inline size_type itemCount () const noexcept
    { return m_gItemList.size (); }

private:
    struct MenuItem
    {
        ItemData     data;
        MenuItemType type;
    };

    typedef std::vector<MenuItem*> vector_type;

private:
    void paintEvent (Rect const&);
    void onFocusKilled ();
    void onKeyPress (u8);
    void onKeyRelease (u8);

    vector_type m_gItemList;

};

} } // namespace Ui

#endif // CPPUAL_UI_CTL_POPUP_H_
