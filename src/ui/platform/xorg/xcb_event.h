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
#include <cppual/input/keyboard.h>
#include <cppual/input/pointer.h>

#include <iostream>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xcb_window.h"

#include <xcb/xcb_icccm.h>
#include <cppual/ui/platforms/xcb/xcb.hpp>

namespace cppual { namespace Ui { namespace Xcb {

typedef ::xcb_button_t     button_type ;
typedef ::xcb_keysym_t     keysym_type ;
typedef ::xcb_keycode_t    keycode_type;
typedef ::xcb_connection_t display_type;

typedef ::xcb_generic_event_t           base_event_type;
typedef ::xcb_button_press_event_t      btn_event_type;
typedef ::xcb_motion_notify_event_t     mouse_move_event_type;
typedef ::xcb_key_press_event_t         key_event_type;
typedef ::xcb_keymap_notify_event_t     keymap_notify_event_type;
typedef ::xcb_expose_event_t            expose_event_type;
typedef ::xcb_focus_in_event_t          focus_event_type;
typedef ::xcb_enter_notify_event_t      step_event_type;
typedef ::xcb_resize_request_event_t    resize_event_type;
typedef ::xcb_map_notify_event_t        map_event_type;
typedef ::xcb_unmap_notify_event_t      unmap_event_type;
typedef ::xcb_property_notify_event_t   property_event_type;
typedef ::xcb_destroy_notify_event_t    destroy_event_type;
typedef ::xcb_visibility_notify_event_t visibility_event_type;
typedef ::xcb_ge_generic_event_t        ge_generic_event_type;
typedef ::xcb_client_message_event_t    client_msg_event_type;

enum Key
{
    UndefinedKey = 0,
    One = 10,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Zero,
    Num0 = 90,
    Num1 = 87,
    Num2 = 88,
    Num3 = 89,
    Num4 = 83,
    Num5 = 84,
    Num6 = 85,
    Num7 = 79,
    Num8 = 80,
    Num9 = 81,
    NumPlus = 86,
    NumMinus = 82,
    NumMultiply = 63,
    NumDivide = 106,
    NumEnter = 104,
    NumDot = 91,
    NumEqual = UndefinedKey,
    A = 38,
    B = 56,
    C = 54,
    D = 40,
    E = 26,
    F = 41,
    G = 42,
    H = 43,
    I = 31,
    J = 44,
    K = 45,
    L = 46,
    M = 58,
    N = 57,
    O = 32,
    P = 33,
    Q = 24,
    R = 27,
    S = 39,
    T = 28,
    U = 30,
    V = 55,
    W = 25,
    X = 53,
    Y = 29,
    Z = 52,
    Escape = 9,
    LControl = 37,
    LShift = 50,
    LAlt = 64,
    LSystem = 133,
    RControl = 105,
    RShift = 62,
    RAlt = 108,
    RSystem = 133,
    Menu = 135,
    LBracket = 34,
    RBracket = 35,
    LParen = UndefinedKey,
    RParen = UndefinedKey,
    SemiColon = 47,
    Colon = UndefinedKey,
    Comma = 59,
    Dot = 60,
    Quote = 48,
    DoubleQuote = UndefinedKey,
    Slash = 61,
    BackSlash = 51,
    Tilde = UndefinedKey,
    Separator = UndefinedKey,
    Equal = 21,
    Greater = UndefinedKey,
    Less = UndefinedKey,
    Dash = 20,
    Space = 65,
    Return = 36,
    BackSpace = 22,
    Tab = 23,
    PageUp = 112,
    PageDown = 117,
    End = 115,
    Home = 110,
    Insert = 118,
    Delete = 119,
    Plus = UndefinedKey,
    Asterisk = UndefinedKey,
    At = UndefinedKey,
    Exclamation = UndefinedKey,
    Question = UndefinedKey,
    Underscore = UndefinedKey,
    Caret = UndefinedKey,
    BackQuote = 49,
    Dollar = UndefinedKey,
    Hash = UndefinedKey,
    Ampersand = UndefinedKey,
    Percent = UndefinedKey,
    Left = 113,
    Right = 114,
    Up = 111,
    Down = 116,
    F1 = 67,
    F2 = 68,
    F3 = 69,
    F4 = 70,
    F5 = 71,
    F6 = 72,
    F7 = 73,
    F8 = 74,
    F9 = 75,
    F10 = 76,
    F11 = 95,
    F12 = 96,
    Pause = UndefinedKey,
    PrintScr = 107,
    CapsLock = 66,
    NumLock = 77,
    ScrollLock = 78
};

// ====================================================

inline u8 button (button_type uId) noexcept
{
    switch (uId)
    {
    case 1:
        return Input::Mouse::Left;
    case 2:
        return Input::Mouse::Middle;
    case 3:
        return Input::Mouse::Right;
    case 8:
        return Input::Mouse::XButton1;
    case 9:
        return Input::Mouse::XButton2;
    case 10:
        return Input::Mouse::XButton3;
    case 11:
        return Input::Mouse::XButton4;
    default:
        return uId;
    }
}

Input::Keyboard::Key keyCode (keycode_type keycode) noexcept
{
    switch (keycode)
    {
    case Zero: return Input::Keyboard::Zero;
    case One: return Input::Keyboard::One;
    case Two: return Input::Keyboard::Two;
    case Three: return Input::Keyboard::Three;
    case Four: return Input::Keyboard::Four;
    case Five: return Input::Keyboard::Five;
    case Six: return Input::Keyboard::Six;
    case Seven: return Input::Keyboard::Seven;
    case Eight: return Input::Keyboard::Eight;
    case Nine: return Input::Keyboard::Nine;
    case Num0: return Input::Keyboard::Numpad0;
    case Num1: return Input::Keyboard::Numpad1;
    case Num2: return Input::Keyboard::Numpad2;
    case Num3: return Input::Keyboard::Numpad3;
    case Num4: return Input::Keyboard::Numpad4;
    case Num5: return Input::Keyboard::Numpad5;
    case Num6: return Input::Keyboard::Numpad6;
    case Num7: return Input::Keyboard::Numpad7;
    case Num8: return Input::Keyboard::Numpad8;
    case Num9: return Input::Keyboard::Numpad9;
    case NumPlus: return Input::Keyboard::NumpadPlus;
    case NumMinus: return Input::Keyboard::NumpadMinus;
    case NumMultiply: return Input::Keyboard::NumpadMultiply;
    case NumDivide: return Input::Keyboard::NumpadDivide;
    case NumEnter: return Input::Keyboard::NumpadEnter;
    case NumDot: return Input::Keyboard::NumpadDot;
    case NumEqual: return Input::Keyboard::UndefinedKey;
    case A: return Input::Keyboard::A;
    case B: return Input::Keyboard::B;
    case C: return Input::Keyboard::C;
    case D: return Input::Keyboard::D;
    case E: return Input::Keyboard::E;
    case F: return Input::Keyboard::F;
    case G: return Input::Keyboard::G;
    case H: return Input::Keyboard::H;
    case I: return Input::Keyboard::I;
    case J: return Input::Keyboard::J;
    case K: return Input::Keyboard::K;
    case L: return Input::Keyboard::L;
    case M: return Input::Keyboard::M;
    case N: return Input::Keyboard::N;
    case O: return Input::Keyboard::O;
    case P: return Input::Keyboard::P;
    case Q: return Input::Keyboard::Q;
    case R: return Input::Keyboard::R;
    case S: return Input::Keyboard::S;
    case T: return Input::Keyboard::T;
    case U: return Input::Keyboard::U;
    case V: return Input::Keyboard::V;
    case W: return Input::Keyboard::W;
    case X: return Input::Keyboard::X;
    case Y: return Input::Keyboard::Y;
    case Z: return Input::Keyboard::Z;
    case Escape: return Input::Keyboard::Escape;
    case LControl: return Input::Keyboard::LControl;
    case LShift: return Input::Keyboard::LShift;
    case LAlt: return Input::Keyboard::LAlt;
    case LSystem: return Input::Keyboard::LSystem;
    case RControl: return Input::Keyboard::RControl;
    case RShift: return Input::Keyboard::RShift;
    case RAlt: return Input::Keyboard::RAlt;
    case Menu: return Input::Keyboard::Menu;
    case LBracket: return Input::Keyboard::LBracket;
    case RBracket: return Input::Keyboard::RBracket;
    case SemiColon: return Input::Keyboard::SemiColon;
    case Comma: return Input::Keyboard::Comma;
    case Dot: return Input::Keyboard::Dot;
    case Quote: return Input::Keyboard::Quote;
    case Slash: return Input::Keyboard::Slash;
    case BackSlash: return Input::Keyboard::BackSlash;
    case Equal: return Input::Keyboard::Equal;
    case Dash: return Input::Keyboard::Dash;
    case Space: return Input::Keyboard::Space;
    case Return: return Input::Keyboard::Return;
    case BackSpace: return Input::Keyboard::BackSpace;
    case Tab: return Input::Keyboard::Tab;
    case PageUp: return Input::Keyboard::PageUp;
    case PageDown: return Input::Keyboard::PageDown;
    case End: return Input::Keyboard::End;
    case Home: return Input::Keyboard::Home;
    case Insert: return Input::Keyboard::Insert;
    case Delete: return Input::Keyboard::Delete;
    case BackQuote: return Input::Keyboard::BackQuote;
    case Left: return Input::Keyboard::Left;
    case Right: return Input::Keyboard::Right;
    case Up: return Input::Keyboard::Up;
    case Down: return Input::Keyboard::Down;
    case F1: return Input::Keyboard::F1;
    case F2: return Input::Keyboard::F2;
    case F3: return Input::Keyboard::F3;
    case F4: return Input::Keyboard::F4;
    case F5: return Input::Keyboard::F5;
    case F6: return Input::Keyboard::F6;
    case F7: return Input::Keyboard::F7;
    case F8: return Input::Keyboard::F8;
    case F9: return Input::Keyboard::F9;
    case F10: return Input::Keyboard::F10;
    case F11: return Input::Keyboard::F11;
    case F12: return Input::Keyboard::F12;
    case PrintScr: return Input::Keyboard::PrintScr;
    case CapsLock: return Input::Keyboard::CapsLock;
    case NumLock: return Input::Keyboard::NumLock;
    case ScrollLock: return Input::Keyboard::ScrollLock;
    default: return Input::Keyboard::UndefinedKey;
    }
}

// ====================================================

class EventPtr final
{
public:
    typedef ::xcb_generic_event_t           base_type;
    typedef ::xcb_button_press_event_t      btn_type;
    typedef ::xcb_motion_notify_event_t     mouse_move_type;
    typedef ::xcb_key_press_event_t         key_type;
    typedef ::xcb_keymap_notify_event_t     keymap_notify_type;
    typedef ::xcb_expose_event_t            expose_type;
    typedef ::xcb_focus_in_event_t          focus_type;
    typedef ::xcb_enter_notify_event_t      step_type;
    typedef ::xcb_resize_request_event_t    resize_type;
    typedef ::xcb_map_notify_event_t        map_type;
    typedef ::xcb_unmap_notify_event_t      unmap_type;
    typedef ::xcb_property_notify_event_t   property_type;
    typedef ::xcb_destroy_notify_event_t    destroy_type;
    typedef ::xcb_visibility_notify_event_t visibility_type;
    typedef ::xcb_ge_generic_event_t        ge_generic_type;
    typedef ::xcb_client_message_event_t    client_msg_type;

    union Data
    {
        base_type          generic;
        btn_type           mouseButton;
        mouse_move_type    mouseMove;
        key_type           key;
        keymap_notify_type keymap;
        expose_type        paint;
        focus_type         focus;
        step_type          step;
        resize_type        size;
        map_type           map;
        unmap_type         unmap;
        property_type      property;
        destroy_type       destroy;
        visibility_type    visibility;
        ge_generic_type    geGeneric;
        client_msg_type    clientMessage;
    };

    typedef Data* pointer;

    constexpr operator pointer () const noexcept
    { return m_handle; }

    constexpr pointer get () const noexcept
    { return m_handle; }

    constexpr pointer operator -> () const { return m_handle; }

    ~EventPtr ()
    { ::free (m_handle); }

    inline EventPtr (base_type* pEvent) noexcept
    : m_handle (reinterpret_cast<pointer> (pEvent))
    { }

private:
    pointer m_handle { };
};

// ====================================================

union EventData
{
    base_event_type*          generic;
    btn_event_type*           mouseButton;
    mouse_move_event_type*    mouseMove;
    key_event_type*           key;
    keymap_notify_event_type* keymap;
    expose_event_type*        paint;
    focus_event_type*         focus;
    step_event_type*          step;
    resize_event_type*        size;
    map_event_type*           map;
    unmap_event_type*         unmap;
    property_event_type*      property;
    destroy_event_type*       destroy;
    visibility_event_type*    visibility;
    ge_generic_event_type*    geGeneric;
    client_msg_event_type*    clientMessage;

    constexpr u32 type () const noexcept
    { return generic->response_type & ~0x80; }

    constexpr EventData (base_event_type* event) noexcept
    : generic (event)
    { }
};

class EventNew final
{
public:
    EventNew (EventData event) noexcept
    : m_data (event)
    { }

private:
    EventData m_data;
};

// ====================================================

class Event final
{
public:
    typedef ::xcb_generic_event_t base_type;
    typedef Input::Event          event_type;
    typedef Event                 self_type;
    typedef Event*                pointer;

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
        Visibility       = XCB_VISIBILITY_NOTIFY,
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

    inline Event (base_type* base) noexcept
    : m_pEvent (base)
    { }

    constexpr auto get () const noexcept
    { return m_pEvent.get(); }

    inline u32 type () const noexcept
    { return m_pEvent->generic.response_type & ~0x80; }

    handle_type window () const noexcept
    {
        switch (type ())
        {
        case MousePress:
            return get ()->mouseButton.event;
        case MouseRelease:
            return get ()->mouseButton.event;
        case MouseMove:
            return get ()->mouseMove.event;
        case KeyPress:
            return get ()->key.event;
        case KeyRelease:
            return get ()->key.event;
        case Expose:
            return get ()->paint.window;
        case Size:
            return get ()->size.window;
        case Property:
            return get ()->property.window;
        case Map:
            return get ()->map.window;
        case Unmap:
            return get ()->unmap.window;
        case Enter:
            return get ()->step.event;
        case Leave:
            return get ()->step.event;
        case FocusIn:
            return get ()->focus.event;
        case FocusOut:
            return get ()->focus.event;
        case Visibility:
            return get ()->visibility.window;
        case Destroy:
            return get ()->destroy.window;
        case ClientMessage:
            return get ()->clientMessage.window;
        //case ChangeParent:
        //case Create:
        //case MapRequest:
        default:
            return get ()->generic.pad[2];
        }
    }

    static base_type toXcbEvent (event_type const&) noexcept
    {
        return base_type ();
    }

    inline event_type toEvent (IDisplay::handle_type connection) const noexcept
    {
        switch (type ())
        {
        case MousePress:
            switch (get ()->mouseButton.detail)
            {
            case MouseWheelUp:
                return Input::WheelEvent ( 1, { get ()->mouseButton.event_x,
                                                get ()->mouseButton.event_y });
            case MouseWheelDown:
                return Input::WheelEvent (-1, { get ()->mouseButton.event_x,
                                                get ()->mouseButton.event_y });
            default:
                return Input::MousePressEvent (button (get ()->mouseButton.detail),
                                              { get ()->mouseButton.event_x,
                                                get ()->mouseButton.event_y });
            }
        case MouseRelease:
            if (get ()->mouseButton.detail == MouseWheelUp or
                get ()->mouseButton.detail == MouseWheelDown)
            return Input::MouseReleaseEvent (button (get ()->mouseButton.detail),
                                            { get ()->mouseButton.event_x,
                                              get ()->mouseButton.event_y });
            break;
        case MouseMove:
            return Input::MouseMoveEvent ({ get ()->mouseMove.event_x,
              get ()->mouseMove.event_y });
        case KeyPress:
            return Input::KeyPressEvent ({ keyCode(get ()->key.detail), 0 });
        case KeyRelease:
            return Input::KeyReleaseEvent ({ keyCode(get ()->key.detail), 0 });
        case Expose:
            return Input::PaintEvent (Rect (static_cast<int16> (get ()->paint.x),
                                            static_cast<int16> (get ()->paint.y),
                                            get ()->paint.width,
                                            get ()->paint.height));
        case Enter:
            return Input::StepEvent (true);
        case Leave:
            return Input::StepEvent (false);
        case FocusIn:
            return Input::FocusEvent (true);
        case FocusOut:
            return Input::FocusEvent (false);
        case Size:
            return Input::SizeEvent ({ get ()->size.width,
                                       get ()->size.height });
        case Property:
            return Input::PropertyEvent (get ()->property.atom,
                                         get ()->property.state);
        case Map:
            return Input::VisibilityEvent (true);
        case Unmap:
            return Input::VisibilityEvent (false);
        case Destroy:
            return event_type (event_type::Destroy);
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
        case ClientMessage:
            {
                Xcb::intern_ptr destroyReply (Xcb::internAtomHelper(connection.get<display_type>(),
                                                                    "WM_DELETE_WINDOW"));

                if (get ()->clientMessage.data.data32[0] == destroyReply->atom)
                {
                    return event_type (event_type::Destroy);
                }

                return event_type (event_type::Null);
            }
        default:
            #ifdef DEBUG_MODE
            std::cout << __PRETTY_FUNCTION__ << " :: unhandled event: " << type () << std::endl;
            #endif
            return event_type (event_type::Null);
        }

        return event_type (event_type::Null);
    }

    inline void operator ()(IDisplay::handle_type connection) const
    {
        switch (type ())
        {
        case MousePress:
            switch (get ()->mouseButton.detail)
            {
            case MouseWheelUp:
                EventQueue::events ().scroll (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y }, 1 });
                break;
            case MouseWheelDown:
                EventQueue::events ().scroll (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y }, -1 });
                break;
            default:
                EventQueue::events ().mousePress (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y },
                 button (get ()->mouseButton.detail) });
                break;
            }
            break;
        case MouseRelease:
            switch (get ()->mouseButton.detail)
            {
            case MouseWheelUp:
            case MouseWheelDown:
                break;
            default:
                EventQueue::events ().mouseRelease (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y },
                 button (get ()->mouseButton.detail) });
                break;
            }
            break;
        case MouseMove:
            EventQueue::events ().mouseMove (window (),
            { get ()->mouseMove.event_x, get ()->mouseMove.event_y });
            break;
        case KeyPress:
            EventQueue::events ().keyPress (window (),
            { keyCode(get ()->key.detail), 0 });
            break;
        case KeyRelease:
            EventQueue::events ().keyPress (window (),
            { keyCode(get ()->key.detail), 0 });
            break;
        case Expose:
            EventQueue::events ().winPaint (window (),
            { Rect (static_cast<int16> (get ()->paint.x),
              static_cast<int16> (get ()->paint.y),
              get ()->paint.width,
              get ()->paint.height) });
            break;
        case Enter:
            EventQueue::events ().winStep (window (), true);
            break;
        case Leave:
            EventQueue::events ().winStep (window (), false);
            break;
        case FocusIn:
            EventQueue::events ().winFocus (window (), true);
            break;
        case FocusOut:
            EventQueue::events ().winFocus (window (), false);
            break;
        case Size:
            EventQueue::events ().winSize (window (),
            { get ()->size.width, get ()->size.height });
            break;
        case Property:
            EventQueue::events ().winProperty (window (),
            { get ()->property.atom, get ()->property.state });
            break;
        case Map:
            EventQueue::events ().winVisible (window (), true);
            break;
        case Unmap:
            EventQueue::events ().winVisible (window (), false);
            break;
        case Destroy:
            EventQueue::events ().winDestroy (window ());
            break;
        case ClientMessage:
            {
                Xcb::intern_ptr destroyReply (Xcb::internAtomHelper(connection.get<display_type>(),
                                                                    "WM_DELETE_WINDOW"));

                if (get ()->clientMessage.data.data32[0] == destroyReply->atom)
                {
                    EventQueue::events ().winDestroy (window ());
                }
            }
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
        case Visibility:
            break;
        default:
#           ifdef DEBUG_MODE
            std::cout << __PRETTY_FUNCTION__ << " :: unhandled event: " << type () << std::endl;
#           endif
            break;
        }
    }

private:
    EventPtr m_pEvent;
};

} } } // namespace Xcb

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_EVENT_H_
