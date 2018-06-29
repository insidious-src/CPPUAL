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

#ifndef CPPUAL_UI_VIEW_H_
#define CPPUAL_UI_VIEW_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>
#include <cppual/circular_queue.h>
#include <cppual/ui/vsurface.h>
#include <cppual/input/event.h>

namespace cppual { namespace Ui {

class View
{
public:
    typedef Input::Event                         event_type;
    typedef Graphics::Element                    window_type;
    typedef Memory::Allocator<View*>             allocator_type;
    typedef CircularQueue<View*, allocator_type> container;
    typedef std::size_t                          size_type;

    typedef typename
    container::iterator iterator;

//    typedef typename
//    Signal<void(window_type, point2u)>
//    ::slot_type mouse_move_conn;

//    typedef typename
//    Signal<void(window_type, event_type::MButtonData)>
//    ::slot_type mouse_btn_conn;

    enum
    {
        DefaultWidth  = 800,
        DefaultHeight = 600
    };

    View (View const&) noexcept;
    View& operator = (View const&) noexcept;
    virtual ~View ();

    View (View*       parent,
          Rect const& rect,
          u32         screen    = 0,
          allocator_type const& = allocator_type ());

    void destroy ();
    void show ();
    void hide ();
    bool setParent (View* parent, point2i pos = point2i ());
    void setGeometry (Rect const&);
    void setMinimumSize (point2u);
    void setMaximumSize (point2u);
    void move (point2i);
    void destroyChildren ();
    void refresh ();
    void enable ();
    void disable ();
    void setFocus ();
    void killFocus ();

    inline weak_window      renderable () const noexcept { return m_pRenderable; }
    inline IPlatformWindow* renderable_unsafe () const noexcept { return m_pRenderable.get (); }
    inline point2u          minimumSize () const noexcept { return m_gMinSize; }
    inline point2u          maximumSize () const noexcept { return m_gMaxSize; }

    inline bool valid () const noexcept
    { return m_gStateFlags.test (View::Valid); }

    inline bool isEnabled () const noexcept
    { return m_gStateFlags.test (View::Enabled); }

    inline bool hasFocus () const noexcept
    { return m_gStateFlags.test (View::HasFocus); }

    inline bool isHidden () const noexcept
    { return !m_gStateFlags.test (View::Valid) or !m_pRenderable->isMapped (); }

    inline Rect geometry () const noexcept
    { return m_gStateFlags.test (View::Valid) ? m_pRenderable->geometry () : Rect (); }

    inline View () noexcept
    : View (nullptr, Rect (0, 0, DefaultWidth, DefaultHeight))
    { }

protected:
    virtual void onDestroy ();
    virtual void onShow (bool);
    virtual void onPaint (Rect const&);
    virtual void onEnable (bool);
    virtual void onSize (point2u);
    virtual void onMove (point2i);
    virtual void onBeginSizeMove (Rect const&);
    virtual void onEndSizeMove (Rect const&);
    virtual void onMinMaxSize (point2u);
    virtual void onFocus (bool);
    virtual void onParentSize (point2u);

private:
    enum StateFlag
    {
        Valid    = 1 << 0,
        HasFocus = 1 << 1,
        Enabled  = 1 << 2
    };

    typedef BitSet<StateFlag> StateFlags;

    void paint (Rect const&);
    void size (point2u);
    void mouseMoved (point2u);
    void mousePressed (event_type::MButtonData const&);
    void mouseReleased (event_type::MButtonData const&);
    void destroyResources ();
    void invalidate () noexcept;

private:
    container     m_gChildrenList;
    point2u       m_gMinSize, m_gMaxSize;
    shared_window m_pRenderable;
    iterator      m_gItFromParent;
    View*         m_pParentObj;
    StateFlags    m_gStateFlags;
};

// =========================================================

class Widget
{
public:
    typedef Memory::Allocator<Widget*>             allocator_type;
    typedef CircularQueue<Widget*, allocator_type> container_type;
    typedef std::size_t                            size_type     ;

    void destroy ();
    void show ();
    void hide ();
    bool setParent (View* parent, point2i pos = point2i ());
    void setGeometry (Rect const&);
    void setSize (point2u);
    void setMinimumSize (point2u);
    void setMaximumSize (point2u);
    void move (point2u);
    void refresh ();
    void enable ();
    void disable ();
    void setFocus ();
    void killFocus ();

    Widget (Widget*     parent,
            Rect const& rect,
            allocator_type const& = allocator_type ());

    virtual ~Widget ();

    point2u minimumSize () const noexcept { return m_gMinSize; }
    point2u maximumSize () const noexcept { return m_gMaxSize; }

    bool isEnabled () const noexcept
    { return m_gStateFlags.test (Widget::Enabled); }

    bool hasFocus () const noexcept
    { return m_gStateFlags.test (Widget::HasFocus); }

    bool isHidden () const noexcept
    { return !m_gBuffer.isMapped (); }

    Rect geometry () const noexcept
    { return m_gBuffer.geometry (); }

    string name () const noexcept
    { return m_gName; }

protected:
    virtual void onDestroy () { }
    virtual void onShow (bool) { }
    virtual void onPaint (Rect const&) { }
    virtual void onEnable (bool) { }
    virtual void onSize (point2u) { }
    virtual void onMove (point2u) { }
    virtual void onBeginSizeMove (Rect const&) { }
    virtual void onEndSizeMove (Rect const&) { }
    virtual void onFocus (bool) { }
    virtual void onParentSize (point2u) { }

private:
    enum StateFlag
    {
        HasFocus = 1 << 1,
        Enabled  = 1 << 2
    };

    typedef BitSet<StateFlag> StateFlags;

    void paint (Rect const&);

private:
    VirtualBuffer  m_gBuffer;
    container_type m_gChildren;
    Widget*        m_pParent;
    string         m_gName;
    point2u        m_gMinSize, m_gMaxSize;
    StateFlags     m_gStateFlags;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIEW_H_
