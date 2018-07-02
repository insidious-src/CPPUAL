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

#ifndef CPPUAL_UI_QUEUE_H
#define CPPUAL_UI_QUEUE_H
#ifdef __cplusplus

#include <thread>
#include <atomic>
#include <memory>
#include <cppual/flags.h>
#include <cppual/noncopyable.h>
#include <cppual/input/event.h>
#include <cppual/ui/window.h>

namespace cppual { namespace Ui {

class   IDisplayQueue;
typedef std::shared_ptr<IDisplayQueue> shared_queue;

// =========================================================

class IDisplayQueue : public NonCopyableVirtual
{
public:
    typedef Input::Event             event_type     ;
    typedef Graphics::Connection     connection_type;
    typedef IPlatformWindow          window_type    ;
    typedef BitSet<event_type::Type> mask_type      ;

    virtual bool set_window_events (window_type const&, mask_type)         = 0;
    virtual bool pop_front         (event_type& receiver, bool wait)       = 0;
    virtual int  poll              (window_type const&, atomic_bool& poll) = 0;
    virtual void send              (window_type const&, event_type const&) = 0;
    virtual void post              (window_type const&, event_type const&) = 0;

    static IDisplayQueue* primary          () noexcept;
    static bool           hasValidInstance () noexcept;

    connection_type display () const noexcept { return m_display; }
    bool            valid   () const noexcept { return m_display; }

protected:
    constexpr IDisplayQueue (connection_type display) noexcept
    : m_display (display)
    { }

private:
    connection_type m_display;
};

// =========================================================

class EventQueue : public NonCopyable
{
public:
    typedef IDisplayQueue::event_type event_type  ;
    typedef Graphics::Element         window_type ;
    typedef std::atomic_bool          bool_type   ;
    typedef View                      control_type;

    struct Signals final : NonCopyable
    {
        Signal<void(window_type, event_type::KeyData)>      keyPress;
        Signal<void(window_type, event_type::KeyData)>      keyRelease;
        Signal<void(window_type, event_type::MButtonData)>  mousePress;
        Signal<void(window_type, event_type::MButtonData)>  mouseRelease;
        Signal<void(window_type, point2u)>                  mouseMove;
        Signal<void(window_type, event_type::MWheelData)>   scroll;
        Signal<void(window_type, event_type::TouchData)>    touchPress;
        Signal<void(window_type, event_type::TouchData)>    touchRelease;
        Signal<void(window_type, event_type::TouchData)>    touchMove;
        Signal<void(window_type, int32)>                    sysMessage;
        Signal<void(window_type, event_type::PaintData)>    winPaint;
        Signal<void(window_type, point2u)>                  winSize;
        Signal<void(window_type, bool)>                     winFocus;
        Signal<void(window_type, bool)>                     winStep;
        Signal<void(window_type, event_type::PropertyData)> winProperty;
        Signal<void(window_type, bool)>                     winVisible;
        Signal<void(window_type)>                           winDestroy;
    };

    static Signals& emit ()
    {
        static Signals event_signals;
        return event_signals;
    }

    EventQueue (IDisplayQueue* display_queue = IDisplayQueue::primary ()) noexcept
    : queue    (display_queue),
      polling  ()
    { }

    int poll (control_type const& window)
    {
        if (std::this_thread::get_id () != window.renderable_unsafe ()->thread_id ())
            throw std::logic_error ("the window was created on a different thread");

        polling = true;
        return queue->poll (*window.renderable_unsafe (), polling);
    }

    void send (control_type const& window, event_type const& event)
    { queue->send (*window.renderable_unsafe (), event); }

    void post (control_type const& window, event_type const& event)
    { queue->post (*window.renderable_unsafe (), event); }

    void quit () noexcept
    { polling = false; }

    bool pop_front (event_type& receiver, bool wait)
    { return queue->pop_front (receiver, wait); }

    bool isPolling () const noexcept
    { return polling.load (); }

private:
    IDisplayQueue* queue  ;
    bool_type      polling;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_UI_QUEUE_H
