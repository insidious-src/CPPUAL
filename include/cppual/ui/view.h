/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#include <cppual/string.h>
#include <cppual/resource>
#include <cppual/gfx/draw.h>
#include <cppual/input/event.h>
#include <cppual/ui/vsurface.h>
#include <cppual/circular_queue.h>
#include <cppual/memory_allocator>

namespace cppual::ui {

class SHARED_API view
{
public:
    typedef view                                       self_type     ;
    typedef input::event                               event_type    ;
    typedef memory::memory_resource                    resource_type ;
    typedef memory::allocator<self_type*>              allocator_type;
    typedef circular_queue<self_type*, allocator_type> view_container;
    typedef std::size_t                                size_type     ;
    typedef view_container::iterator                   iterator      ;
    typedef view_container::const_iterator             const_iterator;
    typedef std::string_view                           string_type   ;
    typedef platform_wnd_interface                     platform_wnd  ;
    typedef gfx::shared_surface                        surface_type  ;
    typedef gfx::shared_context                        context_type  ;
    typedef shared_window                              window_type   ;
    typedef resource_handle                            handle_type   ;

    enum
    {
        default_width  = 640,
        default_height = 480
    };

    view ();
    view (self_type &&);
    view (self_type const&);
    self_type& operator = (self_type &&);
    self_type& operator = (self_type const&);
    virtual ~view ();

    view (self_type*     parent,
          rect const&    rect,
          u32            screen = 0,
          resource_type* rc     = nullptr);

    void destroy ();
    void show ();
    void hide ();
    bool set_parent (self_type* parent, point2i pos = point2i ());
    void set_geometry (rect const&);
    void set_minimum_size (point2u);
    void set_maximum_size (point2u);
    void move (point2i);
    void destroy_children ();
    void refresh ();
    void update (rect const& region);
    void enable ();
    void disable ();
    void get_focus ();
    void kill_focus ();

    inline    window_type    renderable        () const noexcept { return _M_pRenderable; }
    constexpr platform_wnd*  renderable_unsafe () const noexcept { return _M_pRenderable.get (); }
    inline    handle_type    platform_handle   () const { return renderable ()->handle (); }
    inline    shared_display platform_display  () const { return renderable ()->connection (); }
    inline    surface_type   platform_surface  () const noexcept { return _M_pSurface; }
    inline    context_type   platform_context  () const noexcept { return _M_pContext; }
    constexpr point2u        minimum_size      () const noexcept { return _M_gMinSize; }
    constexpr point2u        maximum_size      () const noexcept { return _M_gMaxSize; }

    constexpr bool valid () const noexcept
    { return _M_gStateFlags.test (state_flag::is_valid); }

    constexpr bool is_enabled () const noexcept
    { return _M_gStateFlags.test (state_flag::enabled); }

    constexpr bool has_focus () const noexcept
    { return _M_gStateFlags.test (state_flag::focus); }

    constexpr bool is_hidden () const noexcept
    { return !_M_gStateFlags.test (state_flag::is_valid) or !_M_pRenderable->is_mapped (); }

    constexpr rect geometry () const noexcept
    { return _M_gStateFlags.test (state_flag::is_valid) ? _M_pRenderable->geometry () : rect (); }

    constexpr view_container& children () noexcept
    { return _M_gChildrenList; }

    constexpr view_container const& children () const noexcept
    { return _M_gChildrenList; }

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
    virtual void minimize_event (bool is_minimized);
    virtual void maximize_event (bool is_maximized);
    virtual void fullscreen_event (bool is_fullscreen);
    virtual void close_event ();

    virtual void on_enable (bool);
    virtual void on_parent_size (point2u);

private:
    enum class state_flag : u8
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
    view_container _M_gChildrenList        ;
    point2u        _M_gMinSize, _M_gMaxSize;
    window_type    _M_pRenderable          ;
    surface_type   _M_pSurface             ;
    context_type   _M_pContext             ;
    iterator       _M_gItFromParent        ;
    self_type*     _M_pParentObj           ;
    state_flags    _M_gStateFlags          ;
};

} // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIEW_H_
