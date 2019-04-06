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

#include <cppual/ui/view.h>
#include <cppual/ui/manager.h>
#include <cppual/ui/vsurface.h>

#include <iostream>
#include <exception>
#include <unordered_map>

namespace cppual { namespace Ui {

namespace { namespace Internal {

typedef Memory::PolymorphicAllocator<std::pair<const uptr, View*>> map_allocator;
typedef Memory::PolymorphicAllocator<ProxyRenderable>              renderable_allocator;

typedef std::unordered_map<
                           uptr,
                           View*,
                           std::hash<uptr>,
                           std::equal_to<uptr>,
                           map_allocator
                           >
                           map_type;

inline map_type& map ()
{
    static map_type views_map (3);
    return views_map;
}

inline shared_window createRenderable (View* pParentObj, Rect const& gRect, u32 nScreen)
{
    return pParentObj ? std::allocate_shared<ProxyRenderable> (renderable_allocator(),
                                                               pParentObj->renderable ().lock (),
                                                               gRect) :
                        Platform::Factory::instance ()->createWindow (gRect, nScreen);
}

} } // anonymous namespace Internal

// =========================================================

View::View (View* pParentObj, Rect const& gRect, u32 nScreen, allocator_type const& gAtor)
: m_gChildrenList (gAtor),
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
            connect (EventQueue::events ().mouseMove,
                     [](EventQueue::window_type wnd, point2u pos)
            {
                Internal::map ()[wnd]->mouseMovedEvent (pos);
            });

            connect (EventQueue::events ().mousePress,
                    [](EventQueue::window_type wnd, event_type::MButtonData data)
            {
                Internal::map ()[wnd]->mousePressedEvent (data);
            });

            connect (EventQueue::events ().mouseRelease,
                    [](EventQueue::window_type wnd, event_type::MButtonData data)
            {
                Internal::map ()[wnd]->mouseReleasedEvent (data);
            });

            connect (EventQueue::events ().scroll,
                    [](EventQueue::window_type wnd, event_type::MWheelData data)
            {
                Internal::map ()[wnd]->mouseWheelEvent (data);
            });

            connect (EventQueue::events ().winPaint,
                    [](EventQueue::window_type wnd, event_type::PaintData data)
            {
                Internal::map ()[wnd]->paint (data.region);
            });

            connect (EventQueue::events ().winFocus,
                    [](EventQueue::window_type wnd, bool state)
            {
                Internal::map ()[wnd]->focusEvent (state);
            });

            connect (EventQueue::events ().winSize,
                    [](EventQueue::window_type wnd, point2u size)
            {
                Internal::map ()[wnd]->size (size);
            });

            connect (EventQueue::events ().winVisible,
                    [](EventQueue::window_type wnd, bool state)
            {
                Internal::map ()[wnd]->showEvent (state);
            });

            connect(EventQueue::events ().winStep,
                    [](EventQueue::window_type wnd, bool state)
            {
                Internal::map()[wnd]->enterLeaveEvent (state);
            });

            connect (EventQueue::events ().winProperty,
                    [](EventQueue::window_type wnd, event_type::PropertyData data)
            {
                switch (data.prop)
                {
                case 0:
                    Internal::map ()[wnd]->minMaxSizeEvent (point2u ());
                    break;
                default:
                    break;
                }
            });

            connect (EventQueue::events ().winDestroy,
                    [](EventQueue::window_type wnd)
            {
                Internal::map ()[wnd]->destroyEvent ();
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
                throw std::runtime_error ("failed to register view object");
            }

            IDisplayQueue::primary ()->
                    set_window_events (*m_pRenderable,
                                        event_type::Key   |
                                        event_type::Mouse |
                                        event_type::Window);
        }

    }

    if (pParentObj)
    {
        m_pRenderable->setOwner (pParentObj->renderable ().lock ());
        m_pParentObj = pParentObj;

        pParentObj->m_gChildrenList.push_back (this);
        m_gItFromParent = --pParentObj->m_gChildrenList.end ();
    }

    m_gStateFlags = View::Valid | View::Enabled;
}

View::View (View const& gObj) noexcept
: View (gObj.m_pParentObj,
        gObj.m_pRenderable->geometry (),
        gObj.m_pRenderable->screen   ())
{
    if (gObj.valid ())
    {
        if (!gObj.isEnabled ()) disable ();
        if (!gObj.isHidden  ()) show    ();
    }
}

View& View::operator = (View const& gObj) noexcept
{
    if (this == &gObj) return *this;

    if (gObj.valid ())
    {
        if (Internal::createRenderable (gObj.m_pParentObj,
                                        gObj.m_pRenderable->geometry (),
                                        gObj.m_pRenderable->screen   ()))
        {
            if (!gObj.isEnabled ()) disable ();
            if (!gObj.isHidden  ()) show    ();
        }
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
    paintEvent (gRect);

    if (m_gChildrenList.size ())
        for (View* pChild : m_gChildrenList) pChild->paint (Rect ());
}

void View::size (point2u gSize)
{
    sizeEvent (gSize);

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
        moveEvent (gPoint);
    }
}

void View::mouseMovedEvent (point2u)
{
#   ifdef DEBUG_MODE
    std::cout << "View::mouseMoved(point2u)\n";
#   endif
}

void View::mouseWheelEvent (event_type::MWheelData const&)
{
#   ifdef DEBUG_MODE
    std::cout << "View::mouseWheel(event_type::MWheelData const&)\n";
#   endif
}

void View::mousePressedEvent (event_type::MButtonData const&)
{
#   ifdef DEBUG_MODE
    std::cout << "View::mousePressed(event_type::MButtonData const&)\n";
#   endif
}

void View::mouseReleasedEvent (event_type::MButtonData const&)
{
#   ifdef DEBUG_MODE
    std::cout << "View::mouseReleased(event_type::MButtonData const&)\n";
#   endif
}

void View::setFocus ()
{
    if (m_gStateFlags.test (View::Valid))
    {
        //if (m_gChildrenList.size ())
            //for (pointer pChild : m_gChildrenList) pChild->setFocus ();
        m_pRenderable->raise ();
        focusEvent (true);
    }
}

void View::killFocus ()
{
    if (m_gStateFlags.test (View::Valid))
    {
        for (View* pChild : m_gChildrenList) pChild->killFocus ();
        focusEvent (false);
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
        paintEvent (geometry ());
        for (View* pChild : m_gChildrenList) pChild->refresh ();
    }
}

void View::destroyEvent()
{
#   ifdef DEBUG_MODE
    std::cout << "View::onDestroy()\n";
#   endif
}

void View::showEvent(bool)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onShow(bool)\n";
#   endif
}

void View::paintEvent(Rect const&)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onPaint(Rect const&)\n";
#   endif
}

void View::onEnable(bool)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onEnable(bool)\n";
#   endif
}

void View::sizeEvent(point2u)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onSize(point2u)\n";
#   endif
}

void View::moveEvent(point2i)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onMove(point2i)\n";
#   endif
}

void View::enterLeaveEvent(bool)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onEnterLeave(bool)\n";
#   endif
}

void View::beginSizeMoveEvent(Rect const&)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onBeginSizeMove(Rect const&)\n";
#   endif
}

void View::endSizeMoveEvent(Rect const&)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onEndSizeMove(Rect const&)\n";
#   endif
}

void View::minMaxSizeEvent(point2u)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onMinMaxSize(point2u)\n";
#   endif
}

void View::focusEvent(bool)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onFocus(bool)\n";
#   endif
}

void View::onParentSize(point2u)
{
#   ifdef DEBUG_MODE
    std::cout << "View::onParentSize(point2u)\n";
#   endif
}

} } // namespace Ui
