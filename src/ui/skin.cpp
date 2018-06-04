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

#include <cppual/ui/skin.h>
#include <cppual/ui/queue.h>
#include <functional>

using std::placeholders::_1;

namespace cppual { namespace Ui {

bool ISkin::setDefault (ISkin* pSkin) noexcept
{
    if (!pSkin) return false;

    EventQueue::emit ().winPaint (window_type (),
                                  Input::PaintEvent (Rect ()).data ().paint);
    return true;
}

ISkin* ISkin::getDefault () noexcept
{
    return nullptr;
}

// =========================================================

SkinEngine::SkinEngine (SkinnableView* pView) noexcept
: m_pView (pView)
{
}

// =========================================================

SkinnableView::SkinnableView ()
: m_gSkinEngine (this),
  m_pSkin ()
{
}

SkinnableView::SkinnableView (SkinnableView&& gObj)
: View (gObj),
  m_gSkinEngine (this),
  m_pSkin (gObj.m_pSkin)
{
}

SkinnableView::SkinnableView (SkinnableView const& gObj)
: View (gObj),
  m_gSkinEngine (this),
  m_pSkin (gObj.m_pSkin)
{
}

SkinnableView& SkinnableView::operator = (SkinnableView&& gObj)
{
    if (this == &gObj) return *this;
    return *this;
}

SkinnableView& SkinnableView::operator = (SkinnableView const& gObj)
{
    if (this == &gObj) return *this;
    return *this;
}

void SkinnableView::setSkin (ISkin* pNewSkin) noexcept
{
    m_pSkin = pNewSkin;
    if (valid ()) refresh ();
}

void SkinnableView::onCreate ()
{
    onCreatePost ();
//    m_gConnSkin = std::move (connect (Platform::EventQueue::events.visual,
//                                      this, &SkinnableView::changeSkin));
}

void SkinnableView::changeSkin ()
{
    refresh ();
    onSkinChange ();
}

} } // namespace Ui
