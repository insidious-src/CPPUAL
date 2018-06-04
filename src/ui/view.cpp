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

#include <unordered_map>
#include <cppual/ui/manager.h>
#include <cppual/ui/vsurface.h>
#include <cppual/ui/view.h>

namespace cppual { namespace Ui {

namespace { namespace Internal {

typedef std::unordered_map<uptr, View*> map_type;

inline map_type& map ()
{
    static map_type views_map (7);
    return views_map;
}

inline shared_window createRenderable (View* pParentObj, Rect const& gRect, u32 nScreen)
{
    return pParentObj ? shared_window (new ProxyRenderable (pParentObj->renderable ().lock (),
                                                            gRect)) :
                        Platform::Factory::instance ()->createWindow (gRect, nScreen);
}

} } // anonymous namespace Internal

// =========================================================

View::View (View* pParentObj, Rect const& gRect, u32 nScreen, allocator_type const& gAtor)
: m_gChildrenList (10, gAtor),
  m_gMinSize { 0, 0 },
  m_gMaxSize { 0, 0 },
  m_pRenderable (Internal::createRenderable (pParentObj, gRect, nScreen)),
  m_pParentObj  (),
  m_gStateFlags ()
{
    if (m_pRenderable == nullptr or !m_pRenderable->valid ())
    {
        m_pRenderable.reset ();
        throw std::logic_error ("failed to create renderable");
    }
    else
    {
        static bool bRegEvents = false;

        if (!bRegEvents)
        {
            connect (EventQueue::emit ().mouseMove,
             [](EventQueue::window_type wnd, point2u pos)
            {
                Internal::map ()[wnd]->mouseMoved (pos);
            });

            connect (EventQueue::emit ().mousePress,
                    [](EventQueue::window_type wnd, event_type::MButtonData data)
            {
                Internal::map ()[wnd]->mousePressed (data);
            });

            connect (EventQueue::emit ().mouseRelease,
                    [](EventQueue::window_type wnd, event_type::MButtonData data)
            {
                Internal::map ()[wnd]->mouseReleased (data);
            });

            connect (EventQueue::emit ().winPaint,
                    [](EventQueue::window_type wnd, event_type::PaintData data)
            {
                Internal::map ()[wnd]->paint (data.region);
            });

            connect (EventQueue::emit ().winFocus,
                    [](EventQueue::window_type wnd, bool state)
            {
                Internal::map ()[wnd]->onFocus (state);
            });

            connect (EventQueue::emit ().winSize,
                    [](EventQueue::window_type wnd, point2u size)
            {
                Internal::map ()[wnd]->size (size);
            });

            connect (EventQueue::emit ().winVisible,
                    [](EventQueue::window_type wnd, bool state)
            {
                Internal::map ()[wnd]->onShow (state);
            });

            connect (EventQueue::emit ().winProperty,
                    [](EventQueue::window_type wnd, event_type::PropertyData data)
            {
                switch (data.prop)
                {
                case 0:
                    Internal::map ()[wnd]->onMinMaxSize (point2u ());
                    break;
                default:
                    break;
                }
            });

            bRegEvents = true;
        }

        if (!pParentObj)
        {
            uptr key = m_pRenderable->id ();

            if (Internal::map ().count (key) == 1) Internal::map ()[key] = this;
            else if (!Internal::map ().emplace (std::make_pair (key, this)).second)
            {
                m_pRenderable.reset ();
                throw std::logic_error ("failed to register view object");
            }
        }

        IDisplayQueue::primary ()->
                set_window_events (*m_pRenderable,
                                     event_type::Key     |
                                     event_type::Mouse |
                                     event_type::Window);
    }

    if (pParentObj)
    {
        m_pRenderable->setOwner (pParentObj->renderable ().lock ());
        m_pParentObj = pParentObj;

        pParentObj->m_gChildrenList.push_back (this);
        m_gItFromParent = pParentObj->m_gChildrenList.end ();
    }

    m_gStateFlags = View::Valid | View::Enabled;
}

View::View (View const& gObj) noexcept
: View (gObj.m_pParentObj,
        gObj.m_pRenderable->geometry (),
        gObj.m_pRenderable->screen ())
{
    if (gObj.valid ())
    {
        if (!gObj.isEnabled ()) disable ();
        if (!gObj.isHidden  ()) show ();
    }
}

View& View::operator = (View const& gObj) noexcept
{
    if (this == &gObj) return *this;

    if (gObj.valid ())
    {
//        if (createView (gObj.m_pParentObj,
//                        gObj.m_pRenderable->geometry (),
//                        gObj.m_pRenderable->screen ()))
//        {
//            if (!gObj.isEnabled ()) disable ();
//            if (!gObj.isHidden  ()) show ();
//        }
    }

    return *this;
}

View::~View ()
{
    if (m_gStateFlags.test (View::Valid)) destroyResources ();
}

void View::destroyChildren ()
{
    if (m_gChildrenList.empty ()) return;

    for (View* pChild : m_gChildrenList) if (pChild != nullptr) pChild->destroy ();
    m_gChildrenList.clear ();
}

void View::destroyResources ()
{
    window_type uId = m_pRenderable->id ();

    // destroy all child virtual surfaces
    destroyChildren ();

    // if the object is a child then remove it from
    // the parent
    if (m_pParentObj) *m_gItFromParent = nullptr;

    // if the surface is unique it gets deleted
    m_pRenderable.reset ();

    Internal::map ().erase (uId);
}

void View::invalidate () noexcept
{
    m_pParentObj  = nullptr;
    m_gMinSize    = m_gMaxSize    = { 0, 0 };
    m_gStateFlags = m_gStateFlags = false;
}

void View::destroy ()
{
    if (!m_gStateFlags.test (View::Valid)) return;
    destroyResources ();
    invalidate ();
}

void View::show ()
{
    if (!valid ()) return;
    m_pRenderable->map ();
}

void View::hide ()
{
    if (!valid ()) return;
    m_pRenderable->unmap ();
}

void View::setMinimumSize (point2u gSize)
{
    if (m_gStateFlags.test (View::Valid))
    {
        //m_pRenderable->setMimimumSize (gSize);
        m_gMinSize = gSize;
    }
}

void View::setMaximumSize (point2u gSize)
{
    if (m_gStateFlags.test (View::Valid))
    {
        //m_pRenderable->setMaximumSize (gSize);
        m_gMinSize = gSize;
    }
}

bool View::setParent (View* pParentObj, point2i /*gPos*/)
{
    if (!m_gStateFlags.test (View::Valid) or m_pParentObj == pParentObj)
        return false;

    if (pParentObj)
    {
        if (m_pParentObj != nullptr) *m_gItFromParent = nullptr;
        m_pRenderable->setOwner (pParentObj->m_pRenderable);

        pParentObj->m_gChildrenList.push_back (this);
        //pParentObj->paint ();

        m_gItFromParent = std::move (pParentObj->m_gChildrenList.end ());
    }
    // pParentObj is invalid and m_pParentObj is NOT nullptr
    else
    {
        // recreate using physical surface
        m_pRenderable = Platform::Factory::instance ()->
                        createWindow (m_pRenderable->geometry (),
                                          m_pRenderable->screen (),
                                          m_pRenderable->connection ());
        if (m_pRenderable == nullptr) return false;

        *m_gItFromParent = nullptr;
    }

    m_pParentObj = pParentObj;
    return true;
}

void View::paint (Rect const& gRect)
{
    onPaint (gRect);

    if (m_gChildrenList.size ())
        for (View* pChild : m_gChildrenList) pChild->paint (Rect ());
}

void View::size (point2u gSize)
{
    onSize (gSize);

    if (m_gChildrenList.size ())
        for (View* pChild : m_gChildrenList) pChild->onParentSize (gSize);
}

void View::setGeometry (Rect const& gRect)
{
    if (m_gStateFlags.test (View::Valid))
    {
        Rect gNewRect (gRect);

        // width
        if (gNewRect.width () < m_gMinSize.x)
        {
            gNewRect.right = static_cast<Rect::value_type> (gNewRect.left + m_gMinSize.x);
        }
        else if (m_gMaxSize != point2u () and gNewRect.width () < m_gMaxSize.x)
        {
            gNewRect.right = static_cast<Rect::value_type> (gNewRect.left + m_gMinSize.x);
        }

        // height
        if (gNewRect.height () > m_gMinSize.y)
        {
            gNewRect.bottom = static_cast<Rect::value_type> (gNewRect.top + m_gMinSize.y);
        }
        else if (m_gMaxSize != point2u () and gNewRect.height () > m_gMaxSize.y)
        {
            gNewRect.bottom = static_cast<Rect::value_type> (gNewRect.top + m_gMinSize.y);
        }

        m_pRenderable->setGeometry (gNewRect);
    }
}

void View::move (point2i gPoint)
{
    if (m_gStateFlags.test (View::Valid))
    {
        m_pRenderable->move (gPoint);
        onMove (gPoint);
    }
}

void View::mouseMoved (point2u)
{
}

void View::mousePressed (event_type::MButtonData const&)
{
}

void View::mouseReleased (event_type::MButtonData const&)
{
}

void View::setFocus ()
{
    if (m_gStateFlags.test (View::Valid))
    {
        //if (m_gChildrenList.size ())
            //for (pointer pChild : m_gChildrenList) pChild->setFocus ();
        m_pRenderable->raise ();
        onFocus (true);
    }
}

void View::killFocus ()
{
    if (m_gStateFlags.test (View::Valid))
    {
        for (View* pChild : m_gChildrenList) pChild->killFocus ();
        onFocus (false);
    }
}

void View::enable ()
{
    if (m_gStateFlags.test (View::Valid))
        for (View* pChild : m_gChildrenList) pChild->enable ();
}

void View::disable ()
{
    if (m_gStateFlags.test (View::Valid))
        for (View* pChild : m_gChildrenList) pChild->disable ();
}

void View::refresh ()
{
    if (m_gStateFlags.test (View::Valid))
    {
        onPaint (geometry ());
        for (View* pChild : m_gChildrenList) pChild->refresh ();
    }
}

// =========================================================

void Widget::setSize(point2u)
{

}

void Widget::move (point2u gPoint)
{
    m_gBuffer.move (gPoint);
    onMove (gPoint);
}

void Widget::setFocus ()
{
    onFocus (true);

    if (m_gChildren.size ())
        for (Widget* pChild : m_gChildren) pChild->setFocus ();
}

void Widget::killFocus ()
{
    onFocus (false);

    if (m_gChildren.size ())
        for (Widget* pChild : m_gChildren) pChild->killFocus ();
}

void Widget::enable ()
{
    onEnable (true);
    for (Widget* pChild : m_gChildren) pChild->enable ();
}

void Widget::disable ()
{
    onEnable (false);
    for (Widget* pChild : m_gChildren) pChild->disable ();
}

void Widget::refresh ()
{
    onPaint (geometry ());
    for (Widget* pChild : m_gChildren) pChild->refresh ();
}

} } // namespace Ui
