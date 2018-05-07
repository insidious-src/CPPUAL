/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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
#include <cppual/input/pointer.h>

using std::string;
using namespace cppual::Input;

namespace cppual { namespace Ui {

namespace {

typedef xcb_button_t     xcb_button;
typedef xcb_keysym_t     xcb_keysym;
typedef xcb_window_t     handle_type;
typedef xcb_keycode_t    xcb_keycode;
typedef xcb_connection_t display_type;

// ====================================================

inline u8 button (xcb_button uId) noexcept
{
    switch (uId)
    {
    case 1:
        return Mouse::Left;
    case 2:
        return Mouse::Middle;
    case 3:
        return Mouse::Right;
    case 6:
        return Mouse::XButton1;
    case 7:
        return Mouse::XButton2;
    case 8:
        return Mouse::XButton3;
    case 9:
        return Mouse::XButton4;
    default:
        return uId;
    }
}

//u8 translateKeycode (xcb_keysym) noexcept
//{
//    return 0;
//}

// ====================================================

struct XcbEvent final : xcb_generic_event_t
{
    typedef xcb_generic_event_t         base_type;
    typedef Input::Event                event_type;
    typedef XcbEvent                    self_type;
    typedef XcbEvent*                   pointer;
    typedef xcb_button_press_event_t    btn_press_type;
    typedef xcb_button_release_event_t  btn_release_type;
    typedef xcb_motion_notify_event_t   mouse_move_type;
    typedef xcb_key_press_event_t       key_press_type;
    typedef xcb_key_release_event_t     key_release_type;
    typedef xcb_keymap_notify_event_t   keymap_notify_type;
    typedef xcb_expose_event_t          expose_type;
    typedef xcb_focus_in_event_t        focus_in_type;
    typedef xcb_focus_out_event_t       focus_out_type;
    typedef xcb_enter_notify_event_t    enter_type;
    typedef xcb_leave_notify_event_t    leave_type;
    typedef xcb_resize_request_event_t  resize_type;
    typedef xcb_map_notify_event_t      map_type;
    typedef xcb_unmap_notify_event_t    unmap_type;
    typedef xcb_property_notify_event_t property_type;

    enum Type
    {
        MousePress       = XCB_BUTTON_PRESS,
        MouseRelease     = XCB_BUTTON_RELEASE,
        MouseMove        = XCB_MOTION_NOTIFY,
        KeyPress         = XCB_KEY_PRESS,
        KeyRelease       = XCB_KEY_RELEASE,
        KeyMapNotify     = XCB_KEYMAP_NOTIFY,
        ChangeKbCtrl     = XCB_CHANGE_KEYBOARD_CONTROL,
        GetKbCtrl        = XCB_GET_KEYBOARD_CONTROL,
        Expose           = XCB_EXPOSE,
        Enter            = XCB_ENTER_NOTIFY,
        Leave            = XCB_LEAVE_NOTIFY,
        FocusIn          = XCB_FOCUS_IN,
        FocusOut         = XCB_FOCUS_OUT,
        Size             = XCB_RESIZE_REQUEST,
        Visiblility      = XCB_VISIBILITY_NOTIFY,
        Create           = XCB_CREATE_NOTIFY,
        NoExposure       = XCB_NO_EXPOSURE,
        GraphicsExposure = XCB_GRAPHICS_EXPOSURE,
        Destroy          = XCB_DESTROY_NOTIFY,
        Unmap            = XCB_UNMAP_NOTIFY,
        Map              = XCB_MAP_NOTIFY,
        MapRequest       = XCB_MAP_REQUEST,
        ChangeParent     = XCB_REPARENT_NOTIFY,
        Configure        = XCB_CONFIGURE_NOTIFY,
        ConfigRequest    = XCB_CONFIGURE_REQUEST,
        Gravity          = XCB_GRAVITY_NOTIFY,
        Circulate        = XCB_CIRCULATE_NOTIFY,
        CirculateRequest = XCB_CIRCULATE_REQUEST,
        Property         = XCB_PROPERTY_NOTIFY,
        SelectionClear   = XCB_SELECTION_CLEAR,
        SelectionRequest = XCB_SELECTION_REQUEST,
        Selection        = XCB_SELECTION_NOTIFY,
        Colormap         = XCB_COLORMAP_NOTIFY,
        ClientMessage    = XCB_CLIENT_MESSAGE,
        Mapping          = XCB_MAPPING_NOTIFY,
        GeGeneric        = XCB_GE_GENERIC
    };

    enum
    {
        MouseWheelUp   = 4,
        MouseWheelDown = 5
    };

    constexpr XcbEvent () noexcept : base_type ()
    { }

    constexpr XcbEvent (base_type const& base) noexcept : base_type (base)
    { }

    constexpr u32 type () const noexcept
    { return response_type & ~0x80; }

    template <typename T>
    T const& cast () const noexcept
    { return reinterpret_cast<T const&> (*this); }

    handle_type window () const noexcept
    {
        switch (type ())
        {
        case Expose:
        case Size:
        case Property:
        case Map:
        case Unmap:
        case GraphicsExposure:
        case NoExposure:
        case Destroy:
        case ChangeParent:
            return pad[0];
        case Create:
        case MapRequest:
            return pad[1];
        default:
            return pad[2];
        }
    }

    static self_type toXcbEvent (event_type const&)
    {
        return self_type ();
    }

    inline event_type toEvent () const noexcept
    {
        switch (type ())
        {
        case MousePress:
            switch (cast<btn_press_type> ().detail)
            {
            case MouseWheelUp:
                return Input::ScrollEvent ( 1, { cast<btn_press_type> ().event_x,
                                                 cast<btn_press_type> ().event_y });
            case MouseWheelDown:
                return Input::ScrollEvent (-1, { cast<btn_press_type> ().event_x,
                                                 cast<btn_press_type> ().event_y });
            default:
                return Input::MousePressEvent (button (cast<btn_press_type> ().detail),
                                              { cast<btn_press_type> ().event_x,
                                                cast<btn_press_type> ().event_y });
            }
        case MouseRelease:
            if (cast<btn_release_type> ().detail == MouseWheelUp or
                cast<btn_release_type> ().detail == MouseWheelDown)
            return Input::MouseReleaseEvent (button (cast<btn_release_type> ().detail),
                                            { cast<btn_release_type> ().event_x,
                                              cast<btn_release_type> ().event_y });
            break;
        case MouseMove:
            return Input::PointerMoveEvent ({ cast<mouse_move_type> ().event_x,
              cast<mouse_move_type> ().event_y });
        case KeyPress:
            return Input::KeyPressEvent (cast<key_press_type> ().detail);
        case KeyRelease:
            return Input::KeyReleaseEvent (cast<key_release_type> ().detail );
        case Expose:
            return Input::PaintEvent (Rect (static_cast<int16> (cast<expose_type> ().x),
                                            static_cast<int16> (cast<expose_type> ().y),
                                            cast<expose_type> ().width,
                                            cast<expose_type> ().height));
        case Enter:
            return Input::StepEvent (true);
        case Leave:
            return Input::StepEvent (false);
        case FocusIn:
            return Input::FocusEvent (true);
        case FocusOut:
            return Input::FocusEvent (false);
        case Size:
            return Input::SizeEvent ({ cast<resize_type> ().width,
                                       cast<resize_type> ().height });
        case Property:
            return Input::PropertyEvent (cast<property_type> ().atom,
                                         cast<property_type> ().state);
        case Map:
            return Input::VisibilityEvent (true);
        case Unmap:
            return Input::VisibilityEvent (false);
        case Destroy:
            return event_type (event_type::Null);
        case GraphicsExposure:
            return event_type (event_type::Null);
        case NoExposure:
            return event_type (event_type::Null);
        case ChangeParent:
            return event_type (event_type::Null);
        case Mapping:
            return event_type (event_type::Null);
        case Configure:
            return event_type (event_type::Null);
        case GetKbCtrl:
            return event_type (event_type::Null);
        case ChangeKbCtrl:
            return event_type (event_type::Null);
        default:
            return event_type (event_type::Null);
        }

        return event_type (event_type::Null);
    }

    inline void operator ()() const
    {
        switch (type ())
        {
        case MousePress:
            switch (cast<btn_press_type> ().detail)
            {
            case MouseWheelUp:
                EventQueue::emit ().scroll (cast<btn_press_type> ().event,
                {{ cast<btn_press_type> ().event_x, cast<btn_press_type> ().event_y }, 1 });
                break;
            case MouseWheelDown:
                EventQueue::emit ().scroll (cast<btn_press_type> ().event,
                {{ cast<btn_press_type> ().event_x, cast<btn_press_type> ().event_y }, -1 });
                break;
            default:
                EventQueue::emit ().mousePress (cast<btn_press_type> ().event,
                {{ cast<btn_press_type> ().event_x, cast<btn_press_type> ().event_y },
                 button (cast<btn_press_type> ().detail) });
                break;
            }
            break;
        case MouseRelease:
            switch (cast<btn_release_type> ().detail)
            {
            case MouseWheelUp:
            case MouseWheelDown:
                break;
            default:
                EventQueue::emit ().mouseRelease (cast<btn_release_type> ().event,
                {{ cast<btn_release_type> ().event_x, cast<btn_release_type> ().event_y },
                 button (cast<btn_release_type> ().detail) });
                break;
            }
            break;
        case MouseMove:
            EventQueue::emit ().mouseMove (cast<mouse_move_type> ().event,
            { cast<mouse_move_type> ().event_x, cast<mouse_move_type> ().event_y });
            break;
        case KeyPress:
            EventQueue::emit ().keyPress (cast<key_press_type> ().event,
            { cast<key_press_type> ().detail });
            break;
        case KeyRelease:
            EventQueue::emit ().keyPress (cast<key_release_type> ().event,
            { cast<key_release_type> ().detail });
            break;
        case Expose:
            EventQueue::emit ().winPaint (cast<expose_type> ().window,
            { Rect (static_cast<int16> (cast<expose_type> ().x),
              static_cast<int16> (cast<expose_type> ().y),
              cast<expose_type> ().width,
              cast<expose_type> ().height) });
            break;
        case Enter:
            EventQueue::emit ().winStep (cast<enter_type> ().event, true);
            break;
        case Leave:
            EventQueue::emit ().winStep (cast<leave_type> ().event, false);
            break;
        case FocusIn:
            EventQueue::emit ().winFocus (cast<focus_in_type> ().event, true);
            break;
        case FocusOut:
            EventQueue::emit ().winFocus (cast<focus_out_type> ().event, false);
            break;
        case Size:
            EventQueue::emit ().winSize (cast<resize_type> ().window,
            { cast<resize_type> ().width, cast<resize_type> ().height });
            break;
        case Property:
            EventQueue::emit ().winProperty (cast<property_type> ().window,
            { cast<property_type> ().atom, cast<property_type> ().state });
            break;
        case Map:
            EventQueue::emit ().winVisible (cast<map_type> ().window, true);
            break;
        case Unmap:
            EventQueue::emit ().winVisible (cast<unmap_type> ().window, false);
            break;
        case Destroy:
            break;
        case GraphicsExposure:
            break;
        case NoExposure:
            break;
        case ChangeParent:
            break;
        case Mapping:
            break;
        case Configure:
            break;
        case GetKbCtrl:
            break;
        case ChangeKbCtrl:
            break;
        default:
            break;
        }
    }
};

// ====================================================

class XcbEventPtr final
{
public:
    typedef XcbEvent*            pointer;
    typedef XcbEvent::base_type* base_pointer;

    operator base_pointer () const noexcept
    { return m_handle; }

    void    operator () () const { m_handle->operator ()(); }
    pointer operator -> () const { return m_handle        ; }

    ~XcbEventPtr ()
    { ::free (m_handle); }

    constexpr XcbEventPtr (base_pointer pXcbEvent) noexcept :
    m_handle (static_cast<pointer> (pXcbEvent))
    { }

private:
    pointer m_handle { };
};

} // anonymous namespace

// ====================================================

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
        if (gFlags.test (event_type::PointerMove)) uMask |= XCB_EVENT_MASK_POINTER_MOTION;
        if (gFlags.test (event_type::Step))        uMask |= XCB_EVENT_MASK_ENTER_WINDOW |
                                                              XCB_EVENT_MASK_LEAVE_WINDOW;
//        if (gFlags.test (event_type::PointerMove)) uMask |= XCB_EVENT_MASK_POINTER_MOTION;
//        if (gFlags.test (event_type::KeyMap))      uMask |= XCB_EVENT_MASK_KEYMAP_STATE;
//        if (gFlags.test (event_type::Paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
        if (gFlags.test (event_type::Visibility))  uMask |= XCB_EVENT_MASK_VISIBILITY_CHANGE;
//        if (gFlags.test (event_type::Size))        uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY |
//                                                            XCB_EVENT_MASK_RESIZE_REDIRECT;
//        if (gFlags.test (event_type::Focus))       uMask |= XCB_EVENT_MASK_FOCUS_CHANGE;
        if (gFlags.test (event_type::Property))    uMask |= XCB_EVENT_MASK_PROPERTY_CHANGE;
        if (gFlags.test (event_type::Focus))       uMask |= XCB_EVENT_MASK_FOCUS_CHANGE;
    }

    xcb_change_window_attributes (display   ().get<display_type> (),
                                  window.id ().get<handle_type>  (),
                                  XCB_CW_EVENT_MASK,
                                  &uMask);

    xcb_flush (display ().get<display_type> ());
    return true;
}

bool XQueue::pop_front (event_type& gEvent, bool bWait) noexcept
{
    XcbEventPtr pEv (bWait ? xcb_wait_for_event (display ().get<display_type> ()) :
                             xcb_poll_for_event (display ().get<display_type> ()));

    if (!pEv) return false;
    gEvent = pEv->toEvent ();
    return true;
}

int XQueue::poll (window_type const& window, atomic_bool& poll)
{
    while (poll.load (std::memory_order_relaxed))
    {
        XcbEventPtr event (xcb_wait_for_event (display ().get<display_type> ()));

        if (!event or event->window () != window.id ().get<handle_type> ())
            continue;

        event ();
    }

    return 0;
}

void XQueue::send (window_type const& window, event_type const& event)
{
    XcbEvent send_event = XcbEvent::toXcbEvent (event);

    xcb_send_event (display ().get<display_type> (),
                    false,
                    window.id ().get<handle_type> (),
                    send_event.type (),
                    reinterpret_cast<cchar*> (&send_event));

    xcb_flush (display ().get<display_type> ());
}

void XQueue::post (window_type const& window, event_type const& event)
{
    XcbEvent post_event = XcbEvent::toXcbEvent (event);

    xcb_send_event (display ().get<display_type> (),
                    false,
                    window.id ().get<handle_type> (),
                    post_event.type (),
                    reinterpret_cast<cchar*> (&post_event));

    xcb_flush (display ().get<display_type> ());
}

} } // namespace Input

#endif // OS_GNU_LINUX or OS_BSD
