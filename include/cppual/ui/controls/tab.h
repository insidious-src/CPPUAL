/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#ifndef CPPUAL_UI_TAB_H_
#define CPPUAL_UI_TAB_H_

#include <cppual/ui/skin.h>

using cppual::Text::string;

namespace cppual { namespace Ui {

class TabBar : public SkinnableView
{
public:
    TabBar ();
    TabBar (TabBar&&);
    TabBar (TabBar const&);
    TabBar& operator = (TabBar&&);
    TabBar& operator = (TabBar const&);

    enum TabFlag
    {
        Movable,
        Draggable,
        Removable,
        CloseButton,
        AddButton,
        ShowIcons
    };

    typedef FlagSet<TabFlag, u8> TabFlags;

    bool create (View* parent, Rect const&);
    bool addTab (string const&, IImage* = nullptr, int pos = -1);
    bool removeTab (int pos);
    bool moveTab (int pos, int new_pos);
    bool isMovable () const;
    bool isDraggable () const;
    bool isRemovable () const;
    bool hasCloseButtton () const;
    bool hasAddButton () const;
    void setMovable (bool);
    void setDraggable (bool);
    void setRemovable (bool);
    void setTabLabel (int pos, string const& text);
    void setTabIcon (int pos, IImage*);
    void setAddButtonFlags ();
    void setCloseButtonFlags ();
    void enableCloseButton (bool);
    void enableAddButton (bool);

    Signal<void(int)> signalTabIdxChanged;
    Signal<void(int)> signalTabAddded;
    Signal<void(int)> signalTabRemoved;

private:
    TabFlags m_gTabFlags;

    void onCreate ();
    void onShow (bool);
    void onPaint ();
    void onSize (Rect const&);
    void onGotFocus ();
    void onFocusKilled ();
};

class TabView : public SkinnableView
{
public:
    TabView ();
    TabView (TabView&&);
    TabView (TabView const&);
    TabView& operator = (TabView&&);
    TabView& operator = (TabView const&);

    View* getTabContainer (int);
    bool    create (View* parent, Rect const&);

    TabBar* getTabBar () noexcept
    { return &m_TabBar; }

private:
    TabBar m_TabBar;
    function<void(int)> m_gSlotTabIdx;
    function<void(int)> m_gSlotAddTab;
    function<void(int)> m_gSlotRemoveTab;

    void onTabIdxChange (int);
    void onAddTab (int);
    void onRemoveTab (int);
};

} } // Ui

#endif // CPPUAL_UI_TAB_H_
