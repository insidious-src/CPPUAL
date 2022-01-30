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

#ifndef CPPUAL_UI_QUEUE_H
#define CPPUAL_UI_QUEUE_H
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/signal.h>
#include <cppual/noncopyable.h>
#include <cppual/input/event.h>
#include <cppual/ui/view.h>

#include <thread>
#include <atomic>
#include <memory>

namespace cppual { namespace ui {

class   display_queue_interface;
class   view;
typedef std::shared_ptr<display_queue_interface> shared_queue;

// =========================================================

class display_queue_interface : public non_copyable_virtual
{
public:
    typedef input::event             event_type     ;
    typedef connection_type          connection_type;
    typedef platform_wnd_interface          window_type    ;
    typedef std::atomic_bool         bool_type      ;
    typedef bitset<event_type::bits> mask_type      ;

    ~display_queue_interface();

    virtual bool set_window_events (window_type const&, mask_type)         = 0;
    virtual bool pop_front         (bool wait)                             = 0;
    virtual int  poll              (bool_type& poll)                       = 0;
    virtual void send              (window_type const&, event_type const&) = 0;
    virtual void post              (window_type const&, event_type const&) = 0;

    static shared_queue primary            () noexcept;
    static bool         has_valid_instance () noexcept;

    connection_type display () const noexcept { return _M_display; }
    bool            valid   () const noexcept { return _M_display; }

protected:
    constexpr display_queue_interface (connection_type display) noexcept
    : _M_display (display)
    { }

private:
    connection_type _M_display;
};

// =========================================================

class event_queue : public non_copyable
{
public:
    typedef display_queue_interface::event_type event_type  ;
    typedef resource_handle                     window_type ;
    typedef std::atomic_bool                    bool_type   ;
    typedef view                                control_type;

    struct event_signals final : non_copyable
    {
        signal<void(window_type, event_type::key_data)>      keyPress;
        signal<void(window_type, event_type::key_data)>      keyRelease;
        signal<void(window_type, event_type::mbutton_data)>  mousePress;
        signal<void(window_type, event_type::mbutton_data)>  mouseRelease;
        signal<void(window_type, point2u)>                   mouseMove;
        signal<void(window_type, event_type::mwheel_data)>   scroll;
        signal<void(window_type, event_type::touch_data)>    touchPress;
        signal<void(window_type, event_type::touch_data)>    touchRelease;
        signal<void(window_type, event_type::touch_data)>    touchMove;
        signal<void(window_type, i32)>                       sysMessage;
        signal<void(window_type, event_type::paint_data)>    winPaint;
        signal<void(window_type, point2u)>                   winSize;
        signal<void(window_type, bool)>                      winFocus;
        signal<void(window_type, bool)>                      winStep;
        signal<void(window_type, event_type::property_data)> winProperty;
        signal<void(window_type, bool)>                      winVisible;
        signal<void(window_type)>                            winDestroy;
    };

    inline static event_signals& events ()
    {
        static event_signals inst;
        return inst;
    }

    event_queue (shared_queue const& display_queue = display_queue_interface::primary ()) noexcept
    : queue     (display_queue),
      polling   (true)
    { }

    int poll ()
    {
        polling = true;
        return queue->poll (polling);
    }

    void send (control_type const& window, event_type const& event)
    { queue->send (*window.renderable ().lock ().get (), event); }

    void post (control_type const& window, event_type const& event)
    { queue->post (*window.renderable ().lock ().get (), event); }

    void quit ()
    { polling = false; }

    bool pop_front (bool wait)
    { return queue->pop_front (wait); }

    bool is_polling () const noexcept
    { return polling; }

private:
    shared_queue queue  ;
    bool_type    polling;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_UI_QUEUE_H
