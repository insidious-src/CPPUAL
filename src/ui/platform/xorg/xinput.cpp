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

#define  API_EXPORT
#include "xinput.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <cppual/ui/queue.h>
#include "xcb_event.h"
#include "xsurface.h"

namespace cppual { namespace Ui {

XQueue::XQueue ()
: IDisplayQueue (IDisplay::primary ()->native ())
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
        if (gFlags.test (event_type::Size))        uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY |
                                                            XCB_EVENT_MASK_RESIZE_REDIRECT;
        if (gFlags.test (event_type::Paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
        if (gFlags.test (event_type::KeyPressed))  uMask |= XCB_EVENT_MASK_KEY_PRESS;
        if (gFlags.test (event_type::KeyReleased)) uMask |= XCB_EVENT_MASK_KEY_RELEASE;
        if (gFlags.test (event_type::MButtonDown)) uMask |= XCB_EVENT_MASK_BUTTON_PRESS;
        if (gFlags.test (event_type::MButtonUp))   uMask |= XCB_EVENT_MASK_BUTTON_RELEASE;
        if (gFlags.test (event_type::Step))        uMask |= XCB_EVENT_MASK_ENTER_WINDOW |
                                                            XCB_EVENT_MASK_LEAVE_WINDOW;
        if (gFlags.test (event_type::MouseMove)) uMask |= XCB_EVENT_MASK_POINTER_MOTION;
//        if (gFlags.test (event_type::KeyMap))      uMask |= XCB_EVENT_MASK_KEYMAP_STATE;
//        if (gFlags.test (event_type::Paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
        if (gFlags.test (event_type::Visibility))  uMask |= XCB_EVENT_MASK_VISIBILITY_CHANGE;
        if (gFlags.test (event_type::Destroy))     uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY;
//                                                            XCB_EVENT_MASK_RESIZE_REDIRECT;
        if (gFlags.test (event_type::Property))    uMask |= XCB_EVENT_MASK_PROPERTY_CHANGE;
        if (gFlags.test (event_type::Focus))       uMask |= XCB_EVENT_MASK_FOCUS_CHANGE;
    }

    ::xcb_change_window_attributes (display   ().get<Xcb::display_type> (),
                                    window.id ().get<Xcb::handle_type>  (),
                                    XCB_CW_EVENT_MASK,
                                    &uMask);

    ::xcb_flush (display ().get<Xcb::display_type> ());
    return true;
}

bool XQueue::pop_front (event_type& gEvent, bool bWait) noexcept
{
    Xcb::Event pEv (bWait ? ::xcb_wait_for_event (display ().get<Xcb::display_type> ()) :
                               ::xcb_poll_for_event (display ().get<Xcb::display_type> ()));

    if (!pEv.event ()) return false;
    gEvent = pEv.toEvent ();
    return true;
}

int XQueue::poll (window_type const& window, bool_type& poll)
{
    while (poll)
    {
        Xcb::Event event (::xcb_wait_for_event (display ().get<Xcb::display_type> ()));

        if (!event.event () or event.window () != window.id ().get<Xcb::handle_type> ())
            continue;

        event (static_cast<XWindow const&>(window).destroyPtr());
    }

    return 0;
}

void XQueue::send (window_type const& window, event_type const& event)
{
    Xcb::Event::base_type send_event = Xcb::Event::toXcbEvent (event);

    ::xcb_send_event (display ().get<Xcb::display_type> (),
                      false,
                      window.id ().get<Xcb::handle_type> (),
                      send_event.response_type & ~0x80,
                      reinterpret_cast<cchar*> (&send_event));

    ::xcb_flush (display ().get<Xcb::display_type> ());
}

void XQueue::post (window_type const& window, event_type const& event)
{
    Xcb::Event::base_type post_event = Xcb::Event::toXcbEvent (event);

    ::xcb_send_event (display ().get<Xcb::display_type> (),
                      false,
                      window.id ().get<Xcb::handle_type> (),
                      post_event.response_type & ~0x80,
                      reinterpret_cast<cchar*> (&post_event));

    ::xcb_flush (display ().get<Xcb::display_type> ());
}

} } // namespace Input

#endif // OS_GNU_LINUX or OS_BSD
