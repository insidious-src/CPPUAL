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
#include "xwindow.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

namespace cppual { namespace ui {

// =========================================================

xcb_window::xcb_window (rect const& gRect, u32 nScreen, shared_display pDisplay) noexcept
: platform_wnd_interface (pDisplay, ::xcb_generate_id (pDisplay->native<x::display_type> ())),
  _M_eFlags  (WindowHints),
  _M_uScreen (nScreen),
  _M_pScreen (x::screen_handle (display (), _M_uScreen))
{
    if (valid ())
    {
        ::xcb_create_window (display (),
                             XCB_COPY_FROM_PARENT, // depth
                             handle<x::handle_type> (),
                             _M_pScreen->root, // parent
                             gRect.left,
                             gRect.top,
                             gRect.width (),
                             gRect.height (),
                             1U, // border width
                             XCB_WINDOW_CLASS_INPUT_OUTPUT, // class
                             _M_pScreen->root_visual,
                             0U, // mask
                             nullptr); // value list

        x::change_property (display (),
                            XCB_PROP_MODE_REPLACE,
                            handle<x::handle_type> (),
                            data ().WM_PROTOCOLS,
                            XCB_ATOM_ATOM,
                            x::atom_format,
                            1U,
                            &(data ().WM_DELETE_WINDOW));
    }
}

xcb_window::~xcb_window () noexcept
{
    x::destroy_window (display (), handle<x::handle_type> ());
}

rect xcb_window::geometry () const
{
    auto reply = x::get_geometry (display (), handle<x::handle_type> ());

    return rect (reply->x, reply->y, reply->width, reply->height);
}

void xcb_window::set_geometry (rect const& gRect)
{
    cu32 uRect[4] =
    {
        static_cast<u32> (gRect.left),
        static_cast<u32> (gRect.top),
        static_cast<u32> (gRect.width ()),
        static_cast<u32> (gRect.height ())
    };

    x::configure_window (display (),
                         handle<x::handle_type> (),
                         x::Resize,
                         uRect);
}

void xcb_window::raise ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_ABOVE };

    x::configure_window (display (),
                         handle<x::handle_type> (),
                         x::Stack,
                         uMode);
}

void xcb_window::lower ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_BELOW };

    x::configure_window (display (),
                         handle<x::handle_type> (),
                         x::Stack,
                         uMode);
}

void xcb_window::set_owner (shared_window const& pParent)
{
    rect rect = geometry ();

    x::reparent_window (display (),
                        handle<x::handle_type> (),
                        pParent != nullptr ?
                                            pParent->handle<x::handle_type> () :
                                            _M_pScreen->root,
                        rect.left, rect.top);
}

void xcb_window::move (point2i gPos)
{
    cu32 uPoint[2] = { static_cast<u32> (gPos.x), static_cast<u32> (gPos.y) };

    x::configure_window (display (),
                         handle<x::handle_type> (),
                         x::Move,
                         uPoint);
}

void xcb_window::map ()
{
    x::map_window (display (), handle<x::handle_type> ());

    ::xcb_flush (display ());
}

void xcb_window::map_minimized ()
{
    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ().WM_CHANGE_STATE;
    payload.format         = x::atom_format;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = data ().WM_CHANGE_STATE_ICONIC;

    x::map_window (display (), handle<x::handle_type> ());

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    ::xcb_flush (display ());
}

void xcb_window::map_maximized ()
{
    if (_M_eFlags.test (window_flag::Maximize))
    {
        x::client_msg_event_type payload { };

        payload.response_type  = XCB_CLIENT_MESSAGE;
        payload.type           = data ()._NET_WM_STATE;
        payload.format         = x::atom_format;
        payload.window         = handle<x::handle_type> ();

        payload.data.data32[0] = x::atom_state::add;
        payload.data.data32[1] = data ()._NET_WM_STATE_MAXIMIZED_HORZ;
        payload.data.data32[2] = data ()._NET_WM_STATE_MAXIMIZED_VERT;

        x::map_window (display (), handle<x::handle_type> ());

        x::send_event (display (),
                       x::propagate,
                       _M_pScreen->root,
                       static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                         XCB_EVENT_MASK_RESIZE_REDIRECT       |
                                         XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                       direct_cast<cchar*> (&payload));
    }
    else
    {
        x::map_window (display (), handle<x::handle_type> ());
    }

    ::xcb_flush (display ());
}

void xcb_window::unmap ()
{
    x::unmap_window (display (), handle<x::handle_type> ());

    ::xcb_flush (display ());
}

void xcb_window::set_flags (window_flags flags)
{
    _M_eFlags = flags;
}

void xcb_window::keep_above (bool bAbove)
{
    if (bAbove)
    {
        x::client_msg_event_type payload_below { };
        x::client_msg_event_type payload_above { };

        // ===============================

        payload_below.response_type  = XCB_CLIENT_MESSAGE;
        payload_below.type           = data ()._NET_WM_STATE;
        payload_below.format         = x::atom_format;
        payload_below.window         = handle<x::handle_type> ();

        payload_below.data.data32[0] = x::atom_state::remove;
        payload_below.data.data32[1] = data ()._NET_WM_STATE_BELOW;

        // ===============================

        payload_above.response_type  = XCB_CLIENT_MESSAGE;
        payload_above.type           = data ()._NET_WM_STATE;
        payload_above.format         = x::atom_format;
        payload_above.window         = handle<x::handle_type> ();

        payload_above.data.data32[0] = x::atom_state::add;
        payload_above.data.data32[1] = data ()._NET_WM_STATE_ABOVE;

        // ===============================

        x::send_event (display (),
                       x::propagate,
                       handle<x::handle_type> (),
                       static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                         XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                       direct_cast<cchar*> (&payload_below));

        // ===============================

        x::send_event (display (),
                       x::propagate,
                       handle<x::handle_type> (),
                       static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                         XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                       direct_cast<cchar*> (&payload_above));

        _M_eFlags += window_flag::TopMost;
    }
    else
    {
        x::client_msg_event_type payload { };

        payload.response_type  = XCB_CLIENT_MESSAGE;
        payload.type           = data ()._NET_WM_STATE;
        payload.format         = x::atom_format;
        payload.window         = handle<x::handle_type> ();

        payload.data.data32[0] = x::atom_state::remove;
        payload.data.data32[1] = data ()._NET_WM_STATE_ABOVE;

        x::send_event (display (),
                       x::propagate,
                       _M_pScreen->root,
                       static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                         XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                       direct_cast<cchar*> (&payload));

        _M_eFlags -= window_flag::TopMost;
    }

    ::xcb_flush (display ());
}

bool xcb_window::is_above () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return prop->format == x::atom_format &&
           states_end   != std::find (states, states_end, data ()._NET_WM_STATE_ABOVE);
}

void xcb_window::keep_below (bool bBelow)
{
    if (bBelow)
    {
        x::client_msg_event_type payload_below { };
        x::client_msg_event_type payload_above { };

        // ===============================

        payload_below.response_type  = XCB_CLIENT_MESSAGE;
        payload_below.type           = data ()._NET_WM_STATE;
        payload_below.format         = x::atom_format;
        payload_below.window         = handle<x::handle_type> ();

        payload_below.data.data32[0] = x::atom_state::add;
        payload_below.data.data32[1] = data ()._NET_WM_STATE_BELOW;

        // ===============================

        payload_above.response_type  = XCB_CLIENT_MESSAGE;
        payload_above.type           = data ()._NET_WM_STATE;
        payload_above.format         = x::atom_format;
        payload_above.window         = handle<x::handle_type> ();

        payload_above.data.data32[0] = x::atom_state::remove;
        payload_above.data.data32[1] = data ()._NET_WM_STATE_ABOVE;

        // ===============================

        x::send_event (display (),
                       x::propagate,
                       _M_pScreen->root,
                       static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                         XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                       direct_cast<cchar*> (&payload_below));

        // ===============================

        x::send_event (display (),
                       x::propagate,
                       _M_pScreen->root,
                       static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                         XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                       direct_cast<cchar*> (&payload_above));

        _M_eFlags += window_flag::BelowOthers;
    }
    else
    {
        x::client_msg_event_type payload { };

        payload.response_type  = XCB_CLIENT_MESSAGE;
        payload.type           = data ()._NET_WM_STATE;
        payload.format         = x::atom_format;
        payload.window         = handle<x::handle_type> ();

        payload.data.data32[0] = x::atom_state::remove;
        payload.data.data32[1] = data ()._NET_WM_STATE_BELOW;

        x::send_event (display (),
                       x::propagate,
                       _M_pScreen->root,
                       static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                         XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                       direct_cast<cchar*> (&payload));

        _M_eFlags -= window_flag::BelowOthers;
    }

    ::xcb_flush (display ());
}

bool xcb_window::is_below () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return prop->format == x::atom_format &&
           states_end   != std::find (states, states_end, data ()._NET_WM_STATE_BELOW);
}

bool xcb_window::is_mapped () const
{
    auto reply = x::get_window_attributes (display (),
                                           handle<x::handle_type> ());

    return reply->map_state;
}

xcb_window::string_type xcb_window::title () const
{
    auto reply = x::get_property (display (),
                                  x::dont_delete,
                                  handle<x::handle_type> (),
                                  XCB_ATOM_WM_NAME,
                                  XCB_ATOM_STRING,
                                  x::offset_begin,
                                  x::string_max_len);


    return string_type (static_cast<char*>     (::xcb_get_property_value        (&(*reply))) ,
                        static_cast<size_type> (::xcb_get_property_value_length (&(*reply))));
}

void xcb_window::set_title (string_type const& gTitle)
{
     x::change_property (display (),
                         XCB_PROP_MODE_REPLACE,
                         handle<x::handle_type> (),
                         XCB_ATOM_WM_NAME,
                         XCB_ATOM_STRING,
                         x::string_format,
                         static_cast<u32> (gTitle.length ()),
                         gTitle.c_str ());
}

void xcb_window::set_shaded (bool bShaded)
{
    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ()._NET_WM_STATE;
    payload.format         = x::atom_format;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = bShaded ? x::atom_state::add : x::atom_state::remove;
    payload.data.data32[1] = data ()._NET_WM_STATE_SHADED;
    payload.data.data32[2] = XCB_ATOM_NONE;
    payload.data.data32[3] = XCB_ATOM_NONE;
    payload.data.data32[4] = XCB_ATOM_NONE;

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    ::xcb_flush (display ());
}

bool xcb_window::is_shaded () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return prop->format == x::atom_format &&
           states_end   != std::find (states, states_end, data ()._NET_WM_STATE_SHADED);
}

void xcb_window::set_modal (bool bModal)
{
    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ()._NET_WM_STATE;
    payload.format         = x::atom_format;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = bModal ? x::atom_state::add : x::atom_state::remove;
    payload.data.data32[1] = data ()._NET_WM_STATE_MODAL;

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    ::xcb_flush (display ());
}

bool xcb_window::is_modal () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return prop->format == x::atom_format &&
           states_end   != std::find (states, states_end, data ()._NET_WM_STATE_MODAL);
}

void xcb_window::set_fullscreen (bool bFullscreen)
{
    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ()._NET_WM_STATE;
    payload.format         = 32;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = bFullscreen ? x::atom_state::add : x::atom_state::remove;
    payload.data.data32[1] = data ()._NET_WM_STATE_FULLSCREEN;

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    ::xcb_flush (display ());
}

bool xcb_window::is_fullscreen () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return prop->format == x::atom_format &&
           states_end   != std::find (states, states_end, data ()._NET_WM_STATE_FULLSCREEN);
}

void xcb_window::set_maximized (bool bMaximized)
{
    if (!_M_eFlags.test (window_flag::Maximize)) return;

    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ()._NET_WM_STATE;
    payload.format         = x::atom_format;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = bMaximized ? x::atom_state::add : x::atom_state::remove;
    payload.data.data32[1] = data ()._NET_WM_STATE_MAXIMIZED_HORZ;
    payload.data.data32[2] = data ()._NET_WM_STATE_MAXIMIZED_VERT;

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_RESIZE_REDIRECT       |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    ::xcb_flush (display ());
}

bool xcb_window::is_maximized () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return prop->format == x::atom_format &&
           (states_end  != std::find (states, states_end, data ()._NET_WM_STATE_MAXIMIZED_HORZ) ||
            states_end  != std::find (states, states_end, data ()._NET_WM_STATE_MAXIMIZED_VERT));
}

void xcb_window::set_minimized (bool bMinimized)
{
    if (!bMinimized)
    {
        x::map_window (display (), handle<x::handle_type> ());
        ::xcb_flush (display ());
        return;
    }

    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ().WM_CHANGE_STATE;
    payload.format         = x::atom_format;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = data ().WM_CHANGE_STATE_ICONIC;

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    ::xcb_flush (display ());
}

bool xcb_window::is_minimized () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return prop->format == x::atom_format &&
           states_end   != std::find (states, states_end, data ()._NET_WM_STATE_HIDDEN);

    //return !xcb_window::is_mapped ();
}

void xcb_window::set_visible_in_taskbar (bool bVisible)
{
    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ()._NET_WM_STATE;
    payload.format         = x::atom_format;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = bVisible ? x::atom_state::remove : x::atom_state::add;
    payload.data.data32[1] = data ()._NET_WM_STATE_SKIP_TASKBAR;

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    if (bVisible) _M_eFlags += window_flag::Taskbar;
    else _M_eFlags -= window_flag::Taskbar;

    ::xcb_flush (display ());
}

bool xcb_window::is_visible_in_taskbar () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return states_end == std::find (states, states_end, data ()._NET_WM_STATE_SKIP_TASKBAR);
}

void xcb_window::set_visible_in_pager (bool bVisible)
{
    x::client_msg_event_type payload { };

    payload.response_type  = XCB_CLIENT_MESSAGE;
    payload.type           = data ()._NET_WM_STATE;
    payload.format         = x::atom_format;
    payload.window         = handle<x::handle_type> ();

    payload.data.data32[0] = bVisible ? x::atom_state::remove : x::atom_state::add;
    payload.data.data32[1] = data ()._NET_WM_STATE_SKIP_PAGER;

    x::send_event (display (),
                   x::propagate,
                   _M_pScreen->root,
                   static_cast<u32> (XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
                                     XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY),
                   direct_cast<cchar*> (&payload));

    if (bVisible) _M_eFlags += window_flag::Pager;
    else _M_eFlags -= window_flag::Pager;

    ::xcb_flush (display ());
}

bool xcb_window::is_visible_in_pager () const
{
    auto prop = x::get_property (display (),
                                 x::dont_delete,
                                 handle<x::handle_type> (),
                                 data ()._NET_WM_STATE,
                                 XCB_ATOM_ATOM,
                                 x::offset_begin,
                                 x::atom_max_len);

    auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
    auto const states_end = states + prop->length;

    return states_end == std::find (states, states_end, data ()._NET_WM_STATE_SKIP_PAGER);
}

void xcb_window::flash (uint)
{
}

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
