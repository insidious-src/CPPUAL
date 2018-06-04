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

#ifndef CPPUAL_UI_TOOLBAR_H_
#define CPPUAL_UI_TOOLBAR_H_

#include <cppual/ui/skin.h>
#include <cppual/ui/groups.h>
#include <cppual/common.h>

namespace cppual { namespace Ui {

enum class ToolStyle : unsigned char
{
    Flat,
    Modern
};

enum class ToolButton : unsigned char
{
    TextBesideIcon,
    IconAboveText,
    TextOnly,
    IconOnly
};

class ToolBar : public SkinnableView
{
public:
    enum ToolFlag
    {
        AutoHide    = 1 << 0,
        HotTracking = 1 << 1,
        Splitter    = 1 << 2,
        Movable     = 1 << 3,
        Draggable   = 1 << 4,
        Floatable   = 1 << 5
    };

    typedef BitSet<ToolFlag>        ToolFlags   ;
    typedef std::vector<ViewGroup*> group_vector;

    point2i iconSize () const;
    u8      getRowCount ();

    void setStyle (ToolStyle);
    void setSimple (bool);
    void setMovable (bool);
    void setDraggable (bool);
    void setFloatable (bool);
    void setButtonType (ToolButton);
    void setButtonStyle (ToolStyle);
    void setIconSize (point2i const&);
    void setMaxRows (u8);
    void setOrientation (Orientation);
    void setAutoHide (bool);
    void setHotTracking (bool);
    void enableSplitter (bool);
    void moveGroup (size_type pos, size_type new_pos);
    void maximizeGroup (size_type pos);
    void minimizeGroup (size_type pos);
    void pushChevron (); // auto hide
    bool isFloating () const;
    bool addGroup (ViewGroup&, size_type pos);

    inline ToolButton getButtonType () const noexcept
    { return m_eButtonType; }

    inline Orientation getOrientation () const noexcept
    { return m_eOrientation; }

    inline bool isMovable () const noexcept
    { return m_eToolFlags.test (ToolBar::Movable); }

    inline bool isDraggable () const noexcept
    { return m_eToolFlags.test (ToolBar::Draggable); }

    inline bool isFloatable () const noexcept
    { return m_eToolFlags.test (ToolBar::Floatable); }

    inline bool isAutoHide () const noexcept
    { return m_eToolFlags.test (ToolBar::AutoHide); }

    inline bool hasHotTracking () const noexcept
    { return m_eToolFlags.test (ToolBar::HotTracking); }

    inline bool hasSplitter () const noexcept
    { return m_eToolFlags.test (ToolBar::Splitter); }

    inline size_type getGroupCount () const noexcept
    { return m_gGroups.size (); }

    inline ViewGroup* getGroup (size_type uId) const noexcept
    { return m_gGroups[uId]; }

    Signal<void()> clicked;
    Signal<void()> doubleClicked;
    Signal<void()> rightClicked;
    Signal<void()> childResized;
    Signal<void()> groupAdded;
    Signal<void()> groupRemoved;
    Signal<void()> dragBegun;
    Signal<void()> dragEnded;
    Signal<void()> chevronPushed;
    Signal<void()> heightChanged;
    Signal<void()> splitterDragging;
    Signal<void()> orientationChanged;

private:
    group_vector m_gGroups;
    ToolButton   m_eButtonType;
    ToolFlags    m_eToolFlags;
    Orientation  m_eOrientation;

    void onPaint       ();
    void onEnable      (bool);
    void onSize        (Rect const&);
    void onMove        (Rect const&);
    void onGotFocus    ();
    void onFocusKilled ();
};

} } // Ui

#endif // CPPUAL_UI_TOOLBAR_H_
