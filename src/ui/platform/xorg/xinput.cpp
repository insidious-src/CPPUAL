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

#define  API_EXPORT
#include "xinput.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <cppual/ui/queue.h>
#include "xcb_event.h"
#include "xwindow.h"

namespace cppual { namespace ui {

XQueue::XQueue ()
: display_queue_interface (display_interface::primary ()->native ())
{ }

bool XQueue::set_window_events (window_type const& window, mask_type gFlags) noexcept
{
    if (window.connection ()->native () != display ())
        return false;

    u32 uMask = XCB_EVENT_MASK_NO_EVENT;

//    XCB_EVENT_MASK_EXPOSURE
//            | XCB_EVENT_MASK_STRUCTURE_NOTIFY
//            | XCB_EVENT_MASK_KEY_PRESS
//            | XCB_EVENT_MASK_KEY_RELEASE
//            | XCB_EVENT_MASK_BUTTON_PRESS
//            | XCB_EVENT_MASK_BUTTON_RELEASE
//            | XCB_EVENT_MASK_BUTTON_MOTION
//            | XCB_EVENT_MASK_ENTER_WINDOW
//            | XCB_EVENT_MASK_LEAVE_WINDOW
//            | XCB_EVENT_MASK_PROPERTY_CHANGE
//            | XCB_EVENT_MASK_FOCUS_CHANGE;

    if (gFlags)
    {
        // the order of all events must be exact
        if (gFlags.test (event_type::size))        uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                                                            XCB_EVENT_MASK_RESIZE_REDIRECT;
        if (gFlags.test (event_type::paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
        if (gFlags.test (event_type::key_pressed))  uMask |= XCB_EVENT_MASK_KEY_PRESS;
        if (gFlags.test (event_type::key_released)) uMask |= XCB_EVENT_MASK_KEY_RELEASE;
        if (gFlags.test (event_type::mbutton_down)) uMask |= XCB_EVENT_MASK_BUTTON_PRESS;
        if (gFlags.test (event_type::mbutton_up))   uMask |= XCB_EVENT_MASK_BUTTON_RELEASE;
        if (gFlags.test (event_type::step))        uMask |= XCB_EVENT_MASK_ENTER_WINDOW |
                                                            XCB_EVENT_MASK_LEAVE_WINDOW;
        if (gFlags.test (event_type::mouse_move)) uMask |= XCB_EVENT_MASK_POINTER_MOTION;
//        if (gFlags.test (event_type::KeyMap))      uMask |= XCB_EVENT_MASK_KEYMAP_STATE;
//        if (gFlags.test (event_type::Paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
        if (gFlags.test (event_type::visibility))  uMask |= XCB_EVENT_MASK_VISIBILITY_CHANGE;
        if (gFlags.test (event_type::destroy))     uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY;
//                                                            XCB_EVENT_MASK_RESIZE_REDIRECT;
        if (gFlags.test (event_type::property))    uMask |= XCB_EVENT_MASK_PROPERTY_CHANGE;
        if (gFlags.test (event_type::focus))       uMask |= XCB_EVENT_MASK_FOCUS_CHANGE;
    }

    ::xcb_change_window_attributes (display   ().get<xcb::display_type> (),
                                    window.handle ().get<xcb::handle_type>  (),
                                    XCB_CW_EVENT_MASK,
                                    &uMask);

    ::xcb_flush (display ().get<xcb::display_type> ());
    return true;
}

bool XQueue::pop_front (bool bWait) noexcept
{
    xcb::Event event (bWait ? ::xcb_wait_for_event (display ().get<xcb::display_type> ()) :
                              ::xcb_poll_for_event (display ().get<xcb::display_type> ()));

    if (!event.get ()) return false;

    event (display());
    return true;
}

int XQueue::poll (bool_type& poll)
{
    while (poll)
    {
        xcb::Event event (::xcb_wait_for_event (display ().get<xcb::display_type> ()));

        if (!event.get ()) continue;

        event (display());
    }

    return 0;
}

void XQueue::send (window_type const& window, event_type const& event)
{
    xcb::Event::base_type send_event = xcb::Event::toXcbEvent (event);

    ::xcb_send_event (display ().get<xcb::display_type> (),
                      false,
                      window.handle ().get<xcb::handle_type> (),
                      send_event.response_type & ~0x80,
                      reinterpret_cast<cchar*> (&send_event));

    ::xcb_flush (display ().get<xcb::display_type> ());
}

void XQueue::post (window_type const& window, event_type const& event)
{
    xcb::Event::base_type post_event = xcb::Event::toXcbEvent (event);

    ::xcb_send_event (display ().get<xcb::display_type> (),
                      false,
                      window.handle ().get<xcb::handle_type> (),
                      post_event.response_type & ~0x80,
                      reinterpret_cast<cchar*> (&post_event));

    ::xcb_flush (display ().get<xcb::display_type> ());
}

} } // namespace Input

#endif // OS_GNU_LINUX or OS_BSD
