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

#ifndef CPPUAL_UI_FRAME_H_
#define CPPUAL_UI_FRAME_H_
#ifdef __cplusplus

#include <cppual/ui/controls/popup.h>
#include <cppual/ui/composite.h>

namespace cppual { namespace Ui {

enum class Visibility : unsigned char
{
    Hidden,
    Normal,
    Minimized,
    Maximized
};

class FrameView : public SkinnableView
{
public:
    typedef Graphics::Image image_type;

    enum Edge
    {
        Left        = 1 << 0,
        Top         = 1 << 1,
        Right       = 1 << 2,
        Bottom      = 1 << 3,
        TopLeft     = Left  | Top,
        TopRight    = Right | Top,
        BottomLeft  = Left  | Bottom,
        BottomRight = Right | Bottom
    };

    enum MouseState
    {
        MouseLeftDown  = 1 << 0,
        MouseRightDown = 1 << 1,
    };

    typedef BitSet<FrameView::MouseState> MouseStates;
    typedef BitSet<FrameView::Edge>       Edges;

    FrameView () noexcept;
    FrameView (FrameView&&) noexcept;
    FrameView (FrameView const&) noexcept;
    FrameView& operator = (FrameView&&) noexcept;
    FrameView& operator = (FrameView const&) noexcept;
    ~FrameView () noexcept;

    void stretch ();
    void unstretch ();
    void attach (View*);
    void detach ();
    void setIcon (image_type*);
    void setLabel (string const&);
    void setFlags (WindowFlags);

    FrameView (View*       parent,
               Rect const& target_rect,
               string&&    title,
               image_type* icon,
               WindowFlags flags = WindowHints);

    inline image_type* icon () const noexcept { return m_pIcon; }
    inline View*       attached () const noexcept { return m_pTarget; }
    inline PopupMenu*  menu () noexcept { return &m_gSysMenu; }
    inline WindowFlags flags () const noexcept { return m_gFrameFlags; }
    inline Visibility  visibility () const noexcept { return m_eVisibility; }

    inline bool isStretched () const noexcept
    { return m_eVisibility == Visibility::Maximized; }

protected:
    void onBeginSizeMove (Rect const&);
    void onEndSizeMove (Rect const&);

private:
    PushButton  m_gCloseBtn;
    PushButton  m_gMaxBtn;
    PushButton  m_gMinBtn;
    PushButton  m_gHelpBtn;
    PopupMenu   m_gSysMenu;
    string      m_gTitle;
    Rect        m_gIconRect;
    Rect        m_gFrameBtnsRect;
    image_type* m_pIcon;
    View*       m_pTarget;
    WindowFlags m_gFrameFlags;
    MouseStates m_gMouseStates;
    Edges       m_gHoldingEdges;
    Visibility  m_eVisibility;

    Rect calcFrameSize (Rect const& client_rect) noexcept;
    void onShow (bool);
    void onPaint (Rect const&);
    void onSize (point2u);
    void onMouseLeftDown (point2i);
    void onMouseLeftUp (point2i);
    void onMouseRightDown (point2i);
    void onMouseRightUp (point2i);
    void onMouseMove (point2i);
};

// =========================================================

// control window frame eighter using internal compositor
// or the window manager
class Frame
{
public:
    typedef FrameView::image_type image_type;

    void stretch ();
    void unstretch ();
    void attach (View*);
    void detach ();
    void setIcon (image_type*);
    void setLabel (string const&);

    image_type* icon () const noexcept;
    View*       attached () const noexcept;
    PopupMenu*  menu () noexcept;
    Visibility  visibility () const noexcept;
    bool        isStretched () const noexcept;

private:
    FrameView* m_pInternalFrame;
    bool       m_bIsUsingInternalCompositor;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_FRAME_H_
