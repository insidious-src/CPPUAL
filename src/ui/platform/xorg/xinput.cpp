/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cppual/ui/events.h>
#include <cppual/input/pointer.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xinput.h"

using std::string;
using namespace cppual::Input;

namespace cppual { namespace Ui {

namespace {

typedef xcb_button_t     xcb_button;
typedef xcb_keycode_t    xcb_keycode;
typedef xcb_keysym_t     xcb_keysym;
typedef xcb_connection_t display_type;
typedef xcb_window_t     handle_type;

// ====================================================

inline u8 button (xcb_button uId) noexcept
{
    switch (uId)
    {
    case 1:
        return Pointer::Left;
    case 2:
        return Pointer::Middle;
    case 3:
        return Pointer::Right;
    case 6:
        return Pointer::XButton1;
    case 7:
        return Pointer::XButton2;
    case 8:
        return Pointer::XButton3;
    case 9:
        return Pointer::XButton4;
    default:
        return uId;
    }
}

//u8 translateKeycode (xcb_keysym) noexcept
//{
//    return 0;
//}

// ====================================================

struct XcbEvent final : public xcb_generic_event_t
{
    constexpr XcbEvent () noexcept
    : xcb_generic_event_t ()
    { }

    constexpr uint type () const noexcept
    { return response_type & ~0x80; }
};

// ====================================================

class EventPtr final
{
public:
    typedef Event                       event_type;
    typedef Event::window_type          window_type;
    typedef xcb_generic_event_t         value_type;
    typedef xcb_generic_event_t*        pointer;
    typedef xcb_button_press_event_t    btn_press_type;
    typedef xcb_button_release_event_t  btn_release_type;
    typedef xcb_motion_notify_event_t   mouse_move_type;
    typedef xcb_key_press_event_t        key_press_type;
    typedef xcb_key_release_event_t        key_release_type;
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

    enum
    {
        MousePress         = XCB_BUTTON_PRESS,
        MouseRelease     = XCB_BUTTON_RELEASE,
        MouseMove         = XCB_MOTION_NOTIFY,
        KeyPress         = XCB_KEY_PRESS,
        KeyRelease         = XCB_KEY_RELEASE,
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
        GeGeneric        = XCB_GE_GENERIC,
    };

    enum
    {
        MouseWheelUp   = 4,
        MouseWheelDown = 5
    };

    operator pointer () const noexcept
    { return m_handle; }

    pointer operator -> () const noexcept
    { return m_handle; }

    template <typename T>
    T* get () const noexcept
    { return reinterpret_cast<T*> (m_handle); }

    uint type () const noexcept
    { return m_handle->response_type & ~0x80; }

    ~EventPtr () noexcept
    { ::free (m_handle); }

    EventPtr ()               noexcept = default;
    EventPtr (pointer pDpy)   noexcept : m_handle (pDpy) { }
    EventPtr (std::nullptr_t) noexcept : m_handle ()     { }

    inline static XcbEvent toXcbEvent (event_type const&)
    {
        return XcbEvent ();
    }

    inline handle_type window () const noexcept
    {
        switch (type ())
        {
        case MousePress:
            return get<btn_press_type> ()->event;
        case MouseRelease:
            return get<btn_release_type> ()->event;
        case MouseMove:
            return get<mouse_move_type> ()->event;
        case KeyPress:
            return get<key_press_type> ()->event;
        case KeyRelease:
            return get<key_release_type> ()->event;
        case Expose:
            return get<expose_type> ()->window;
        case Enter:
            return get<enter_type> ()->event;
        case Leave:
            return get<leave_type> ()->event;
        case FocusIn:
            return get<focus_in_type> ()->event;
        case FocusOut:
            return get<focus_out_type> ()->event;
        case Size:
            return get<resize_type> ()->window;
        case Property:
            return get<property_type> ()->window;
        case Map:
            return get<map_type> ()->window;
        case Unmap:
            return get<unmap_type> ()->window;
        default:
            return 0;
        }
    }

    inline void operator ()() const
    {
        switch (type ())
        {
        case MousePress:
            switch (get<btn_press_type> ()->detail)
            {
            case MouseWheelUp:
                event_type::registers ().scroll (get<btn_press_type> ()->event,
                {{ get<btn_press_type> ()->event_x, get<btn_press_type> ()->event_y }, 1 });
                break;
            case MouseWheelDown:
                event_type::registers ().scroll (get<btn_press_type> ()->event,
                {{ get<btn_press_type> ()->event_x, get<btn_press_type> ()->event_y }, -1 });
                break;
            default:
                event_type::registers ().mousePress (get<btn_press_type> ()->event,
                {{ get<btn_press_type> ()->event_x, get<btn_press_type> ()->event_y },
                 button (get<btn_press_type> ()->detail) });
                break;
            }
            break;
        case MouseRelease:
            switch (get<btn_release_type> ()->detail)
            {
            case MouseWheelUp:
            case MouseWheelDown:
                break;
            default:
                event_type::registers ().mouseRelease (get<btn_release_type> ()->event,
                {{ get<btn_release_type> ()->event_x, get<btn_release_type> ()->event_y },
                 button (get<btn_release_type> ()->detail) });
                break;
            }
            break;
        case MouseMove:
            event_type::registers ().mouseMove (get<mouse_move_type> ()->event,
            { get<mouse_move_type> ()->event_x, get<mouse_move_type> ()->event_y });
            break;
        case KeyPress:
            event_type::registers ().keyPress (get<key_press_type> ()->event,
            { get<key_press_type> ()->detail });
            break;
        case KeyRelease:
            event_type::registers ().keyPress (get<key_release_type> ()->event,
            { get<key_release_type> ()->detail });
            break;
        case Expose:
            event_type::registers ().winPaint (get<expose_type> ()->window,
            { Rect (static_cast<int16> (get<expose_type> ()->x),
              static_cast<int16> (get<expose_type> ()->y),
              get<expose_type> ()->width,
              get<expose_type> ()->height) });
            break;
        case Enter:
            event_type::registers ().winStep (get<enter_type> ()->event, true);
            break;
        case Leave:
            event_type::registers ().winStep (get<leave_type> ()->event, false);
            break;
        case FocusIn:
            event_type::registers ().winFocus (get<focus_in_type> ()->event, true);
            break;
        case FocusOut:
            event_type::registers ().winFocus (get<focus_out_type> ()->event, false);
            break;
        case Size:
            event_type::registers ().winSize (get<resize_type> ()->window,
            { get<resize_type> ()->width, get<resize_type> ()->height });
            break;
        case Property:
            event_type::registers ().winProperty (get<property_type> ()->window,
            { get<property_type> ()->atom, get<property_type> ()->state });
            break;
        case Map:
            event_type::registers ().winVisible (get<map_type> ()->window, true);

        case Unmap:
            event_type::registers ().winVisible (get<unmap_type> ()->window, false);
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

    inline event_type toEvent () const noexcept
    {
        switch (type ())
        {
        case MousePress:
            switch (get<btn_press_type> ()->detail)
            {
            case MouseWheelUp:
                return Ui::ScrollEvent (get<btn_press_type> ()->event,
                                        1, { get<btn_press_type> ()->event_x,
                                             get<btn_press_type> ()->event_y });
            case MouseWheelDown:
                return Ui::ScrollEvent (get<btn_press_type> ()->event,
                                        -1, { get<btn_press_type> ()->event_x,
                                              get<btn_press_type> ()->event_y });
            default:
                return Ui::MousePressEvent (get<btn_press_type> ()->event,
                                            button (get<btn_press_type> ()->detail),
                { get<btn_press_type> ()->event_x,
                  get<btn_press_type> ()->event_y });
            }
            break;
        case MouseRelease:
            if (get<btn_release_type> ()->detail == MouseWheelUp or
                get<btn_release_type> ()->detail == MouseWheelDown)

            return Ui::MouseReleaseEvent (get<btn_release_type> ()->event,
                                          button (get<btn_release_type> ()->detail),
            { get<btn_release_type> ()->event_x,
              get<btn_release_type> ()->event_y });
        case MouseMove:
            return Ui::PointerMoveEvent (get<mouse_move_type> ()->event,
            { get<mouse_move_type> ()->event_x,
              get<mouse_move_type> ()->event_y });
        case KeyPress:
            return Ui::KeyPressEvent (get<key_press_type> ()->event,
            { get<key_press_type> ()->detail });
        case KeyRelease:
            return Ui::KeyReleaseEvent (get<key_release_type> ()->event,
            { get<key_release_type> ()->detail });
        case Expose:
            return Ui::PaintEvent (get<expose_type> ()->window,
                                     Rect (static_cast<int16> (get<expose_type> ()->x),
                                           static_cast<int16> (get<expose_type> ()->y),
                                           get<expose_type> ()->width,
                                           get<expose_type> ()->height));
            break;
        case Enter:
            return Ui::StepEvent (get<enter_type> ()->event, true);
        case Leave:
            return Ui::StepEvent (get<leave_type> ()->event, false);
        case FocusIn:
            return Ui::FocusEvent (get<focus_in_type> ()->event, true);
        case FocusOut:
            return Ui::FocusEvent (get<focus_out_type> ()->event, false);
        case Size:
            return Ui::SizeEvent (get<resize_type> ()->window,
            { get<resize_type> ()->width,
              get<resize_type> ()->height });
        case Property:
            return Ui::PropertyEvent (get<property_type> ()->window,
                                      get<property_type> ()->atom,
                                      get<property_type> ()->state);
        case Map:
            return Ui::VisibilityEvent (get<map_type> ()->window, true);
        case Unmap:
            return Ui::VisibilityEvent (get<unmap_type> ()->window, false);
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
    }

private:
    pointer m_handle;
};

} // anonymous namespace

// ====================================================

XQueue::XQueue ()
: IDisplayQueue (IDisplay::instance ()->native ())
{ }

bool XQueue::set_window_events (IWindow const& window, mask_type gFlags) noexcept
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
        if (gFlags.test (event_type::ButtonDown))  uMask |= XCB_EVENT_MASK_BUTTON_PRESS;
        if (gFlags.test (event_type::ButtonUp))    uMask |= XCB_EVENT_MASK_BUTTON_RELEASE;
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
    EventPtr pEv (bWait ? xcb_wait_for_event (display ().get<display_type> ()) :
                          xcb_poll_for_event (display ().get<display_type> ()));

    if (!pEv) return false;
    gEvent = pEv.toEvent ();
    return true;
}

int XQueue::poll (window_type const& window, atomic_bool& poll)
{
    while (poll.load (std::memory_order_relaxed))
    {
        EventPtr event (xcb_wait_for_event (display ().get<display_type> ()));

        if (!event or event.window () != window.id ().get<handle_type> ())
            continue;

        event ();
    }

    return 0;
}

void XQueue::send (event_type const& event)
{
    XcbEvent send_event = EventPtr::toXcbEvent (event);

    xcb_send_event (display ().get<display_type> (),
                    false,
                    event.window ().get<handle_type> (),
                    send_event.type (),
                    reinterpret_cast<cchar*> (&send_event));

    xcb_flush (display ().get<display_type> ());
}

void XQueue::post (event_type const& event)
{
    XcbEvent post_event = EventPtr::toXcbEvent (event);

    xcb_send_event (display ().get<display_type> (),
                    false,
                    event.window ().get<handle_type> (),
                    post_event.type (),
                    reinterpret_cast<cchar*> (&post_event));

    xcb_flush (display ().get<display_type> ());
}

} } // namespace Input

#endif // OS_GNU_LINUX or OS_BSD
