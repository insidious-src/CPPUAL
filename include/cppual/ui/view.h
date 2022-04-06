/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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
#include <cppual/memory/allocator.h>
#include <cppual/string.h>

namespace cppual { namespace ui {

class view
{
public:
    typedef input::event                          event_type    ;
    typedef resource_handle                       window_type   ;
    typedef memory::allocator<view*>              allocator_type;
    typedef circular_queue<view*, allocator_type> container     ;
    typedef std::size_t                           size_type     ;
    typedef typename container::iterator          iterator      ;
    typedef string                                string_type   ;

    enum
    {
        default_width  = 800,
        default_height = 600
    };

    view ();
    view (view const&);
    view& operator = (view const&);
    virtual ~view ();

    view (view*       parent,
          rect const& rect,
          u32         screen    = 0,
          allocator_type const& = allocator_type ());

    void destroy ();
    void show ();
    void hide ();
    bool set_parent (view* parent, point2i pos = point2i ());
    void set_geometry (rect const&);
    void set_minimum_size (point2u);
    void set_maximum_size (point2u);
    void move (point2i);
    void destroy_children ();
    void refresh ();
    void enable ();
    void disable ();
    void set_focus ();
    void kill_focus ();

    inline weak_window             renderable () const noexcept { return _M_pRenderable; }
    inline platform_wnd_interface* renderable_unsafe () const noexcept { return _M_pRenderable.get (); }
    inline point2u                 minimum_size () const noexcept { return _M_gMinSize; }
    inline point2u                 maximum_size () const noexcept { return _M_gMaxSize; }

    inline bool valid () const noexcept
    { return _M_gStateFlags.test (view::is_valid); }

    inline bool is_enabled () const noexcept
    { return _M_gStateFlags.test (view::enabled); }

    inline bool has_focus () const noexcept
    { return _M_gStateFlags.test (view::focus); }

    inline bool is_hidden () const noexcept
    { return !_M_gStateFlags.test (view::is_valid) or !_M_pRenderable->is_mapped (); }

    inline rect geometry () const noexcept
    { return _M_gStateFlags.test (view::is_valid) ? _M_pRenderable->geometry () : rect (); }

protected:
    virtual void mouse_moved_event (point2u);
    virtual void mouse_wheel_event (event_type::mwheel_data const&);
    virtual void mouse_pressed_event (event_type::mbutton_data const&);
    virtual void mouse_released_event (event_type::mbutton_data const&);
    virtual void key_pressed_event(event_type::key_data const&);
    virtual void key_released_event(event_type::key_data const&);
    virtual void show_event (bool);
    virtual void paint_event (rect const&);
    virtual void size_event (point2u);
    virtual void move_event (point2i);
    virtual void enter_leave_event (bool);
    virtual void begin_size_move_event (rect const&);
    virtual void end_size_move_event (rect const&);
    virtual void min_max_size_event (point2u);
    virtual void focus_event (bool);
    virtual void help_event ();
    virtual void minimize_event ();
    virtual void maximize_event ();
    virtual void close_event ();

    virtual void on_enable (bool);
    virtual void on_parent_size (point2u);

private:
    enum state_flag
    {
        is_valid = 1 << 0,
        focus    = 1 << 1,
        enabled  = 1 << 2
    };

    typedef bitset<state_flag> state_flags;

    void paint (rect const&);
    void size (point2u);
    void destroy_resources ();
    void invalidate () noexcept;

private:
    container     _M_gChildrenList;
    point2u       _M_gMinSize, _M_gMaxSize;
    shared_window _M_pRenderable;
    iterator      _M_gItFromParent;
    view*         _M_pParentObj;
    state_flags   _M_gStateFlags;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIEW_H_
