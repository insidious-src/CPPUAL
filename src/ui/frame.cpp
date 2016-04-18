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

#include <cppual/ui/composite.h>
#include <cppual/ui/frame.h>
#include <cppual/mstring.h>

using std::string;
using std::bind;
using std::placeholders::_1;

namespace cppual { namespace Ui {

FrameView::FrameView () noexcept
: m_gCloseBtn (),
  m_gMaxBtn (),
  m_gMinBtn (),
  m_gHelpBtn (),
  m_gSysMenu (),
  m_gTitle (),
  m_gIconRect (),
  m_gFrameBtnsRect (),
  m_pIcon (),
  m_pTarget (),
  m_gFrameFlags (),
  m_gMouseStates (),
  m_gHoldingEdges (),
  m_eVisibility ()
{
}

FrameView::FrameView (FrameView&& gObj) noexcept
: SkinnableView (gObj)
{
}

FrameView::FrameView (FrameView const& gObj) noexcept
: SkinnableView (gObj)
{
}

FrameView& FrameView::operator = (FrameView&& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

FrameView& FrameView::operator = (FrameView const& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

FrameView::~FrameView () noexcept
{
}

void FrameView::stretch ()
{
    renderable ().lock ().get ()->setMaximized (true);
}

void FrameView::unstretch ()
{
    renderable ().lock ().get ()->setMaximized (false);
}

void FrameView::attach (View* pView)
{
    if (!pView or !pView->valid ()) return;

    if (!isUsingInternalCompositor ())
    {
        if (m_pTarget and m_pTarget->valid ())
        {
            m_pTarget->renderable_unsafe ()->setFlags
                    (m_pTarget->renderable_unsafe ()->flags () += WindowFlag::Frame);
        }
        else
        {
            m_pTarget->renderable_unsafe ()->setFlags
                    (m_pTarget->renderable_unsafe ()->flags () -= WindowFlag::Frame);
        }
    }

    m_pTarget = pView;
}

void FrameView::detach ()
{
    if (!m_pTarget) return;

    if (!isUsingInternalCompositor ())
    {
        m_pTarget->renderable_unsafe ()->setFlags
                (m_pTarget->renderable_unsafe ()->flags () -= Frame);
    }
    else
    {
    }

    m_pTarget = nullptr;
}

void FrameView::setIcon (image_type*)
{
}

void FrameView::setLabel (string const&)
{
}

void FrameView::setFlags (WindowFlags)
{
}

FrameView::FrameView (View*       pParent,
                      Rect const& gTargetRect,
                      string&&    gLabel,
                      image_type* pIcon,
                      WindowFlags gFlags)
: SkinnableView (pParent, calcFrameSize (gTargetRect)),
  m_gCloseBtn (),
  m_gMaxBtn (),
  m_gMinBtn (),
  m_gHelpBtn (),
  m_gSysMenu (),
  m_gTitle (gLabel),
  m_gIconRect (),
  m_gFrameBtnsRect (),
  m_pIcon (pIcon),
  m_pTarget (),
  m_gFrameFlags (gFlags),
  m_gMouseStates (),
  m_gHoldingEdges (),
  m_eVisibility ()
{
    if (valid ())
    {
        m_gCloseBtn.create (this, string ());
        m_gMinBtn.create (this, string ());
        if (gFlags.test (Maximize)) m_gMaxBtn.create (this, string ());
        if (gFlags.test (Help)) m_gHelpBtn.create (this, string ());
        if (pIcon) setIcon (pIcon);
        m_gTitle = std::forward<string> (gLabel);

        if (m_gSysMenu.create ({ m_gIconRect.left, m_gIconRect.top }))
        {
            m_gSysMenu.addCommand (nullptr);
            m_gSysMenu.addCommand (nullptr);
            m_gSysMenu.addSeparator ();
            m_gSysMenu.addSubMenu (nullptr, "Compositor");
            m_gSysMenu.addSeparator ();
            m_gSysMenu.addCommand (nullptr);
        }
    }
}

void FrameView::onShow (bool bVisible)
{
    if (!bVisible) m_gSysMenu.hide ();
}

void FrameView::onPaint (Rect const&)
{
}

void FrameView::onSize (point2u)
{
}

void FrameView::onMouseLeftDown (point2i gPos)
{
    m_gMouseStates += FrameView::MouseLeftDown;

    if (m_gFrameBtnsRect.contains (gPos))
    {
        // frame buttons
    }
    else if (m_gIconRect.contains (gPos))
    {
        m_gSysMenu.setPosition (point2i { m_gIconRect.left,
                                          m_gIconRect.bottom });
        m_gSysMenu.show ();
    }
    else
    {
        onBeginSizeMove (geometry ());
    }
}

void FrameView::onMouseLeftUp (point2i)
{
    m_gMouseStates -= FrameView::MouseLeftDown;
}

void FrameView::onMouseRightDown (point2i gPos)
{
    m_gMouseStates += FrameView::MouseRightDown;

    if (geometry ().contains (gPos) and
            !m_gIconRect.contains (gPos) and
            !m_gFrameBtnsRect.contains (gPos))
    {
        m_gSysMenu.setPosition (gPos);
        m_gSysMenu.show ();
    }
}

void FrameView::onMouseRightUp (point2i)
{
    m_gMouseStates -= FrameView::MouseRightDown;
}

void FrameView::onMouseMove (point2i)
{
    if (!m_gMouseStates.test (FrameView::MouseLeftDown)) return;

    switch (m_gHoldingEdges)
    {
    case FrameView::Left:
        break;
    case FrameView::Top:
        break;
    case FrameView::Right:
        break;
    case FrameView::Bottom:
        break;
    case FrameView::TopLeft:
        break;
    case FrameView::TopRight:
        break;
    case FrameView::BottomLeft:
        break;
    case FrameView::BottomRight:
        break;
    }
}

void FrameView::onBeginSizeMove (Rect const&)
{
}

void FrameView::onEndSizeMove (Rect const&)
{
}

Rect FrameView::calcFrameSize (Rect const& gRect) noexcept
{
    return gRect;
}

} } // namespace Ui
