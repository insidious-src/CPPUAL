/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_XCB_EVENT_H_
#define CPPUAL_PLATFORM_XCB_EVENT_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/ui/queue.h>
#include <cppual/input/pointer.h>
#include <xcb/xcb_icccm.h>
#include <iostream>
#include "xcb_def.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

namespace cppual { namespace Ui { namespace Xcb {

typedef ::xcb_button_t     xcb_button;
typedef ::xcb_keysym_t     xcb_keysym;
typedef ::xcb_keycode_t    xcb_keycode;
typedef ::xcb_connection_t display_type;

// ====================================================

inline u8 button (xcb_button uId) noexcept
{
    switch (uId)
    {
    case 1:
        return Input::Mouse::Left;
    case 2:
        return Input::Mouse::Middle;
    case 3:
        return Input::Mouse::Right;
    case 6:
        return Input::Mouse::XButton1;
    case 7:
        return Input::Mouse::XButton2;
    case 8:
        return Input::Mouse::XButton3;
    case 9:
        return Input::Mouse::XButton4;
    default:
        return uId;
    }
}

//u8 translateKeycode (::xcb_keysym) noexcept
//{
//    return 0;
//}

// ====================================================

struct Event final : ::xcb_generic_event_t
{
    typedef ::xcb_generic_event_t         base_type;
    typedef Input::Event                  event_type;
    typedef Event                         self_type;
    typedef Event*                        pointer;
    typedef ::xcb_button_press_event_t    btn_press_type;
    typedef ::xcb_button_release_event_t  btn_release_type;
    typedef ::xcb_motion_notify_event_t   mouse_move_type;
    typedef ::xcb_key_press_event_t       key_press_type;
    typedef ::xcb_key_release_event_t     key_release_type;
    typedef ::xcb_keymap_notify_event_t   keymap_notify_type;
    typedef ::xcb_expose_event_t          expose_type;
    typedef ::xcb_focus_in_event_t        focus_in_type;
    typedef ::xcb_focus_out_event_t       focus_out_type;
    typedef ::xcb_enter_notify_event_t    enter_type;
    typedef ::xcb_leave_notify_event_t    leave_type;
    typedef ::xcb_resize_request_event_t  resize_type;
    typedef ::xcb_map_notify_event_t      map_type;
    typedef ::xcb_unmap_notify_event_t    unmap_type;
    typedef ::xcb_property_notify_event_t property_type;
    typedef ::xcb_destroy_notify_event_t  destroy_type;

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

    constexpr Event () noexcept : base_type ()
    { }

    constexpr Event (base_type const& base) noexcept : base_type (base)
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
        case MousePress:
            return cast<btn_press_type> ().event;
        case MouseRelease:
            return cast<btn_release_type> ().event;
        case MouseMove:
            return cast<mouse_move_type> ().event;
        case KeyPress:
            return cast<key_press_type> ().event;
        case KeyRelease:
            return cast<key_release_type> ().event;
        case Expose:
            return cast<expose_type> ().window;
        case Size:
            return cast<resize_type> ().window;
        case Property:
            return cast<property_type> ().window;
        case Map:
            return cast<map_type> ().window;
        case Unmap:
            return cast<unmap_type> ().window;
        case Enter:
            return cast<enter_type> ().event;
        case Leave:
            return cast<leave_type> ().event;
        case FocusIn:
            return cast<focus_in_type> ().event;
        case FocusOut:
            return cast<focus_out_type> ().event;
        case Destroy:
            return cast<destroy_type> ().window;
        //case ChangeParent:
        //case Create:
        //case MapRequest:
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
            return Input::KeyReleaseEvent (cast<key_release_type> ().detail);
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
                EventQueue::emit ().scroll (window (),
                {{ cast<btn_press_type> ().event_x, cast<btn_press_type> ().event_y }, 1 });
                break;
            case MouseWheelDown:
                EventQueue::emit ().scroll (window (),
                {{ cast<btn_press_type> ().event_x, cast<btn_press_type> ().event_y }, -1 });
                break;
            default:
                EventQueue::emit ().mousePress (window (),
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
                EventQueue::emit ().mouseRelease (window (),
                {{ cast<btn_release_type> ().event_x, cast<btn_release_type> ().event_y },
                 button (cast<btn_release_type> ().detail) });
                break;
            }
            break;
        case MouseMove:
            EventQueue::emit ().mouseMove (window (),
            { cast<mouse_move_type> ().event_x, cast<mouse_move_type> ().event_y });
            break;
        case KeyPress:
            EventQueue::emit ().keyPress (window (),
            { cast<key_press_type> ().detail });
            break;
        case KeyRelease:
            EventQueue::emit ().keyPress (window (),
            { cast<key_release_type> ().detail });
            break;
        case Expose:
            EventQueue::emit ().winPaint (window (),
            { Rect (static_cast<int16> (cast<expose_type> ().x),
              static_cast<int16> (cast<expose_type> ().y),
              cast<expose_type> ().width,
              cast<expose_type> ().height) });
            break;
        case Enter:
            EventQueue::emit ().winStep (window (), true);
            break;
        case Leave:
            EventQueue::emit ().winStep (window (), false);
            break;
        case FocusIn:
            EventQueue::emit ().winFocus (window (), true);
            break;
        case FocusOut:
            EventQueue::emit ().winFocus (window (), false);
            break;
        case Size:
            EventQueue::emit ().winSize (window (),
            { cast<resize_type> ().width, cast<resize_type> ().height });
            break;
        case Property:
            EventQueue::emit ().winProperty (window (),
            { cast<property_type> ().atom, cast<property_type> ().state });
            break;
        case Map:
            EventQueue::emit ().winVisible (window (), true);
            break;
        case Unmap:
            EventQueue::emit ().winVisible (window (), false);
            break;
        case Destroy:
            EventQueue::emit ().winDestroy (window ());
#           ifdef DEBUG_MODE
            std::cout << "Destroy event: " << type () << std::endl;
#           endif
            break;
        case GraphicsExposure:
            break;
        case NoExposure:
            break;
        case ChangeParent:
            break;
        case Mapping:
            break;
        case MapRequest:
            break;
        case Configure:
            break;
        case GetKbCtrl:
            break;
        case ChangeKbCtrl:
            break;
        default:
#           ifdef DEBUG_MODE
            std::cout << "unhandled event: " << type () << std::endl;
#           endif
            break;
        }
    }
};

// ====================================================

class EventPtr final
{
public:
    typedef Event*            pointer;
    typedef Event::base_type* base_pointer;

    operator base_pointer () const noexcept
    { return m_handle; }

    void    operator () () const { m_handle->operator ()(); }
    pointer operator -> () const { return m_handle        ; }

    ~EventPtr ()
    { ::free (m_handle); }

    constexpr EventPtr (base_pointer pEvent) noexcept :
    m_handle (static_cast<pointer> (pEvent))
    { }

private:
    pointer m_handle { };
};

} } } // namespace Xcb

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_EVENT_H_
