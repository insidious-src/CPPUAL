/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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
#include <cppual/ui/action.h>

using cppual::Text::string;

namespace cppual { namespace Ui {

class ComboBox : public SkinnableView
{
public:
    ComboBox ();
    ComboBox (ComboBox&&);
    ComboBox (ComboBox const&);
    ComboBox& operator = (ComboBox&&);
    ComboBox& operator = (ComboBox const&);

    uint getItemCount ();
    int  getCurItemIdx ();
    bool addItem (ActionItem*, int pos = -1);
    bool removeItem (int pos);
    bool isEditable () const;
    void setEditable (bool);
    void setTextLimit (size_type max_chars_shown);
    void setDropListWidth (int);
    void setItemIcon (int, IImage*);
    void setItemText (int, string const&);
    void setItemDesc (int, string const&);
    void setItemHeight (int);
    void showDropList ();
    void hideDropList ();
    void clear ();

    bool addItem (string const&   text,
                  string const&   desc,
                  IImage* icon = nullptr,
                  int             pos  = -1);

    Signal<void(int)>  signalItemIdxChanged;
    Signal<void(int)>  signalItemAdded;
    Signal<void(int)>  signalItemRemoved;
    Signal<void(int)>  signalItemTextChanged;
    Signal<void(int)>  signalItemIconChanged;
    Signal<void(int)>  signalItemDescChanged;
    Signal<void()>     signalDoubleClick;
    Signal<void(bool)> signalDropDown;

private:
    void onCreate ();
    void onShow (bool);
    void onPaint ();
    void onEnable (bool);
    void onSize (Rect const&);
    void onGotFocus ();
    void onFocusKilled ();
    void onKeyPress (int);
    void onKeyRelease (int);
};

} } // Ui

#endif // CPPUAL_UI_COMBO_H_
