/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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


namespace cppual { namespace ui { namespace xcb {

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
        return input::mouse::left;
    case 2:
        return input::mouse::middle;
    case 3:
        return input::mouse::right;
    case 8:
        return input::mouse::xbutton1;
    case 9:
        return input::mouse::xbutton2;
    case 10:
        return input::mouse::xbutton3;
    case 11:
        return input::mouse::xbutton4;
    default:
        return uId;
    }
}

input::keyboard::key keyCode (keycode_type keycode) noexcept
{
    switch (keycode)
    {
    case Zero: return input::keyboard::Zero;
    case One: return input::keyboard::One;
    case Two: return input::keyboard::Two;
    case Three: return input::keyboard::Three;
    case Four: return input::keyboard::Four;
    case Five: return input::keyboard::Five;
    case Six: return input::keyboard::Six;
    case Seven: return input::keyboard::Seven;
    case Eight: return input::keyboard::Eight;
    case Nine: return input::keyboard::Nine;
    case Num0: return input::keyboard::Numpad0;
    case Num1: return input::keyboard::Numpad1;
    case Num2: return input::keyboard::Numpad2;
    case Num3: return input::keyboard::Numpad3;
    case Num4: return input::keyboard::Numpad4;
    case Num5: return input::keyboard::Numpad5;
    case Num6: return input::keyboard::Numpad6;
    case Num7: return input::keyboard::Numpad7;
    case Num8: return input::keyboard::Numpad8;
    case Num9: return input::keyboard::Numpad9;
    case NumPlus: return input::keyboard::NumpadPlus;
    case NumMinus: return input::keyboard::NumpadMinus;
    case NumMultiply: return input::keyboard::NumpadMultiply;
    case NumDivide: return input::keyboard::NumpadDivide;
    case NumEnter: return input::keyboard::NumpadEnter;
    case NumDot: return input::keyboard::NumpadDot;
    case NumEqual: return input::keyboard::undefined_key;
    case A: return input::keyboard::A;
    case B: return input::keyboard::B;
    case C: return input::keyboard::C;
    case D: return input::keyboard::D;
    case E: return input::keyboard::E;
    case F: return input::keyboard::F;
    case G: return input::keyboard::G;
    case H: return input::keyboard::H;
    case I: return input::keyboard::I;
    case J: return input::keyboard::J;
    case K: return input::keyboard::K;
    case L: return input::keyboard::L;
    case M: return input::keyboard::M;
    case N: return input::keyboard::N;
    case O: return input::keyboard::O;
    case P: return input::keyboard::P;
    case Q: return input::keyboard::Q;
    case R: return input::keyboard::R;
    case S: return input::keyboard::S;
    case T: return input::keyboard::T;
    case U: return input::keyboard::U;
    case V: return input::keyboard::V;
    case W: return input::keyboard::W;
    case X: return input::keyboard::X;
    case Y: return input::keyboard::Y;
    case Z: return input::keyboard::Z;
    case Escape: return input::keyboard::Escape;
    case LControl: return input::keyboard::LControl;
    case LShift: return input::keyboard::LShift;
    case LAlt: return input::keyboard::LAlt;
    case LSystem: return input::keyboard::LSystem;
    case RControl: return input::keyboard::RControl;
    case RShift: return input::keyboard::RShift;
    case RAlt: return input::keyboard::RAlt;
    case Menu: return input::keyboard::Menu;
    case LBracket: return input::keyboard::LBracket;
    case RBracket: return input::keyboard::RBracket;
    case SemiColon: return input::keyboard::SemiColon;
    case Comma: return input::keyboard::Comma;
    case Dot: return input::keyboard::Dot;
    case Quote: return input::keyboard::Quote;
    case Slash: return input::keyboard::Slash;
    case BackSlash: return input::keyboard::BackSlash;
    case Equal: return input::keyboard::Equal;
    case Dash: return input::keyboard::Dash;
    case Space: return input::keyboard::Space;
    case Return: return input::keyboard::Return;
    case BackSpace: return input::keyboard::BackSpace;
    case Tab: return input::keyboard::Tab;
    case PageUp: return input::keyboard::PageUp;
    case PageDown: return input::keyboard::PageDown;
    case End: return input::keyboard::End;
    case Home: return input::keyboard::Home;
    case Insert: return input::keyboard::Insert;
    case Delete: return input::keyboard::Delete;
    case BackQuote: return input::keyboard::BackQuote;
    case Left: return input::keyboard::Left;
    case Right: return input::keyboard::Right;
    case Up: return input::keyboard::Up;
    case Down: return input::keyboard::Down;
    case F1: return input::keyboard::F1;
    case F2: return input::keyboard::F2;
    case F3: return input::keyboard::F3;
    case F4: return input::keyboard::F4;
    case F5: return input::keyboard::F5;
    case F6: return input::keyboard::F6;
    case F7: return input::keyboard::F7;
    case F8: return input::keyboard::F8;
    case F9: return input::keyboard::F9;
    case F10: return input::keyboard::F10;
    case F11: return input::keyboard::F11;
    case F12: return input::keyboard::F12;
    case PrintScr: return input::keyboard::PrintScr;
    case CapsLock: return input::keyboard::CapsLock;
    case NumLock: return input::keyboard::NumLock;
    case ScrollLock: return input::keyboard::ScrollLock;
    default: return input::keyboard::undefined_key;
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
    { return _M_handle; }

    constexpr pointer get () const noexcept
    { return _M_handle; }

    constexpr pointer operator -> () const { return _M_handle; }

    ~EventPtr ()
    { std::free (_M_handle); }

    inline EventPtr (base_type* pEvent) noexcept
    : _M_handle (reinterpret_cast<pointer> (pEvent))
    { }

private:
    pointer _M_handle { };
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
    : _M_data (event)
    { }

private:
    EventData _M_data;
};

// ====================================================

class Event final
{
public:
    typedef ::xcb_generic_event_t base_type;
    typedef input::event          event_type;
    typedef Event                 self_type;
    typedef Event*                pointer;

    enum Type
    {
        TMousePress       = XCB_BUTTON_PRESS,
        TMouseRelease     = XCB_BUTTON_RELEASE,
        TMouseMove        = XCB_MOTION_NOTIFY,
        TKeyPress         = XCB_KEY_PRESS,
        TKeyRelease       = XCB_KEY_RELEASE,
        TKeyMapNotify     = XCB_KEYMAP_NOTIFY,
        TChangeKbCtrl     = XCB_CHANGE_KEYBOARD_CONTROL,
        TGetKbCtrl        = XCB_GET_KEYBOARD_CONTROL,
        TExpose           = XCB_EXPOSE,
        TEnter            = XCB_ENTER_NOTIFY,
        TLeave            = XCB_LEAVE_NOTIFY,
        TFocusIn          = XCB_FOCUS_IN,
        TFocusOut         = XCB_FOCUS_OUT,
        TSize             = XCB_RESIZE_REQUEST,
        TVisibility       = XCB_VISIBILITY_NOTIFY,
        TCreate           = XCB_CREATE_NOTIFY,
        TNoExposure       = XCB_NO_EXPOSURE,
        TGraphicsExposure = XCB_GRAPHICS_EXPOSURE,
        TDestroy          = XCB_DESTROY_NOTIFY,
        TUnmap            = XCB_UNMAP_NOTIFY,
        TMap              = XCB_MAP_NOTIFY,
        TMapRequest       = XCB_MAP_REQUEST,
        TChangeParent     = XCB_REPARENT_NOTIFY,
        TConfigure        = XCB_CONFIGURE_NOTIFY,
        TConfigRequest    = XCB_CONFIGURE_REQUEST,
        TGravity          = XCB_GRAVITY_NOTIFY,
        TCirculate        = XCB_CIRCULATE_NOTIFY,
        TCirculateRequest = XCB_CIRCULATE_REQUEST,
        TProperty         = XCB_PROPERTY_NOTIFY,
        TSelectionClear   = XCB_SELECTION_CLEAR,
        TSelectionRequest = XCB_SELECTION_REQUEST,
        TSelection        = XCB_SELECTION_NOTIFY,
        TColormap         = XCB_COLORMAP_NOTIFY,
        TClientMessage    = XCB_CLIENT_MESSAGE,
        TMapping          = XCB_MAPPING_NOTIFY,
        TGeGeneric        = XCB_GE_GENERIC
    };

    enum
    {
        MouseWheelUp   = 4,
        MouseWheelDown = 5
    };

    inline Event (base_type* base) noexcept
    : _M_pEvent (base)
    { }

    constexpr auto get () const noexcept
    { return _M_pEvent.get(); }

    inline u32 type () const noexcept
    { return _M_pEvent->generic.response_type & ~0x80; }

    handle_type window () const noexcept
    {
        switch (type ())
        {
        case TMousePress:
            return get ()->mouseButton.event;
        case TMouseRelease:
            return get ()->mouseButton.event;
        case TMouseMove:
            return get ()->mouseMove.event;
        case TKeyPress:
            return get ()->key.event;
        case TKeyRelease:
            return get ()->key.event;
        case TExpose:
            return get ()->paint.window;
        case TSize:
            return get ()->size.window;
        case TProperty:
            return get ()->property.window;
        case TMap:
            return get ()->map.window;
        case TUnmap:
            return get ()->unmap.window;
        case TEnter:
            return get ()->step.event;
        case TLeave:
            return get ()->step.event;
        case TFocusIn:
            return get ()->focus.event;
        case TFocusOut:
            return get ()->focus.event;
        case TVisibility:
            return get ()->visibility.window;
        case TDestroy:
            return get ()->destroy.window;
        case TClientMessage:
            return get ()->clientMessage.window;
        //case TChangeParent:
        //case TCreate:
        //case TMapRequest:
        default:
            return get ()->generic.pad[2];
        }
    }

    static base_type toXcbEvent (event_type const&) noexcept
    {
        return base_type ();
    }

    inline event_type toEvent (display_interface::handle_type connection) const noexcept
    {
        switch (type ())
        {
        case TMousePress:
            switch (get ()->mouseButton.detail)
            {
            case MouseWheelUp:
                return input::wheel_event ( 1, { get ()->mouseButton.event_x,
                                                get ()->mouseButton.event_y });
            case MouseWheelDown:
                return input::wheel_event (-1, { get ()->mouseButton.event_x,
                                                get ()->mouseButton.event_y });
            default:
                return input::mouse_press_event (button (get ()->mouseButton.detail),
                                              { get ()->mouseButton.event_x,
                                                get ()->mouseButton.event_y });
            }
        case TMouseRelease:
            if (get ()->mouseButton.detail == MouseWheelUp or
                get ()->mouseButton.detail == MouseWheelDown)
            return input::mouse_release_event (button (get ()->mouseButton.detail),
                                            { get ()->mouseButton.event_x,
                                              get ()->mouseButton.event_y });
            break;
        case TMouseMove:
            return input::mouse_move_event ({ get ()->mouseMove.event_x,
              get ()->mouseMove.event_y });
        case TKeyPress:
            return input::key_press_event ({ keyCode(get ()->key.detail), 0 });
        case TKeyRelease:
            return input::key_release_event ({ keyCode(get ()->key.detail), 0 });
        case TExpose:
            return input::paint_event (rect (static_cast<i16> (get ()->paint.x),
                                            static_cast<i16> (get ()->paint.y),
                                            get ()->paint.width,
                                            get ()->paint.height));
        case TEnter:
            return input::step_event (true);
        case TLeave:
            return input::step_event (false);
        case TFocusIn:
            return input::focus_event (true);
        case TFocusOut:
            return input::focus_event (false);
        case TSize:
            return input::size_event ({ get ()->size.width,
                                       get ()->size.height });
        case TProperty:
            return input::property_event (get ()->property.atom,
                                         get ()->property.state);
        case TMap:
            return input::visibility_event (true);
        case TUnmap:
            return input::visibility_event (false);
        case TDestroy:
            return event_type (event_type::destroy);
        case TGraphicsExposure:
            return event_type (event_type::null);
        case TNoExposure:
            return event_type (event_type::null);
        case TChangeParent:
            return event_type (event_type::null);
        case TMapping:
            return event_type (event_type::null);
        case TConfigure:
            return event_type (event_type::null);
        case TGetKbCtrl:
            return event_type (event_type::null);
        case TChangeKbCtrl:
            return event_type (event_type::null);
        case TClientMessage:
            {
                auto destroyReply = xcb::internAtomHelper(connection.get<display_type>(),
                                                          "WM_DELETE_WINDOW");

                if (get ()->clientMessage.data.data32[0] == destroyReply->atom)
                {
                    return event_type (event_type::destroy);
                }

                return event_type (event_type::null);
            }
        default:
            #ifdef DEBUG_MODE
            std::cout << __PRETTY_FUNCTION__ << " :: unhandled event: " << type () << std::endl;
            #endif
            return event_type (event_type::null);
        }

        return event_type (event_type::null);
    }

    inline void operator ()(display_interface::handle_type connection) const
    {
        switch (type ())
        {
        case TMousePress:
            switch (get ()->mouseButton.detail)
            {
            case MouseWheelUp:
                event_queue::events ().scroll (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y }, 1 });
                break;
            case MouseWheelDown:
                event_queue::events ().scroll (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y }, -1 });
                break;
            default:
                event_queue::events ().mousePress (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y },
                 button (get ()->mouseButton.detail) });
                break;
            }
            break;
        case TMouseRelease:
            switch (get ()->mouseButton.detail)
            {
            case MouseWheelUp:
            case MouseWheelDown:
                break;
            default:
                event_queue::events ().mouseRelease (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y },
                 button (get ()->mouseButton.detail) });
                break;
            }
            break;
        case TMouseMove:
            event_queue::events ().mouseMove (window (),
            { get ()->mouseMove.event_x, get ()->mouseMove.event_y });
            break;
        case TKeyPress:
            event_queue::events ().keyPress (window (),
            { keyCode(get ()->key.detail), 0 });
            break;
        case TKeyRelease:
            event_queue::events ().keyPress (window (),
            { keyCode(get ()->key.detail), 0 });
            break;
        case Expose:
            event_queue::events ().winPaint (window (),
            { rect (static_cast<i16> (get ()->paint.x),
              static_cast<i16> (get ()->paint.y),
              get ()->paint.width,
              get ()->paint.height) });
            break;
        case TEnter:
            event_queue::events ().winStep (window (), true);
            break;
        case TLeave:
            event_queue::events ().winStep (window (), false);
            break;
        case TFocusIn:
            event_queue::events ().winFocus (window (), true);
            break;
        case TFocusOut:
            event_queue::events ().winFocus (window (), false);
            break;
        case TSize:
            event_queue::events ().winSize (window (),
            { get ()->size.width, get ()->size.height });
            break;
        case TProperty:
            event_queue::events ().winProperty (window (),
            { get ()->property.atom, get ()->property.state });
            break;
        case TMap:
            event_queue::events ().winVisible (window (), true);
            break;
        case TUnmap:
            event_queue::events ().winVisible (window (), false);
            break;
        case TDestroy:
            event_queue::events ().winDestroy (window ());
            break;
        case TClientMessage:
            {
                xcb::intern_ptr destroyReply (xcb::internAtomHelper(connection.get<display_type>(),
                                                                    "WM_DELETE_WINDOW"));

                if (get ()->clientMessage.data.data32[0] == destroyReply->atom)
                {
                    event_queue::events ().winDestroy (window ());
                }
            }
            break;
        case TGraphicsExposure:
            break;
        case TNoExposure:
            break;
        case TChangeParent:
            break;
        case TMapping:
            break;
        case TMapRequest:
            break;
        case TConfigure:
            break;
        case TGetKbCtrl:
            break;
        case TChangeKbCtrl:
            break;
        case TVisibility:
            break;
        default:
#           ifdef DEBUG_MODE
            std::cout << __PRETTY_FUNCTION__ << " :: unhandled event: " << type () << std::endl;
#           endif
            break;
        }
    }

private:
    EventPtr _M_pEvent;
};

} } } // namespace x

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_EVENT_H_
