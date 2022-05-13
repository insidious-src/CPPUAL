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

#include <cppual/ui/platforms/xcb/xcb_def.h>

#include "xbackend.h"

#include <X11/Xlibint.h>

namespace cppual { namespace x {

enum class key : u8
{
    undefined_key = 0,
    Escape = 9,
    One = 10,
    Exclamation = 10,
    Two = 11,
    At = 11,
    Three = 12,
    Hash = 12,
    Four = 13,
    Dollar = 13,
    Five = 14,
    Percent = 14,
    Six = 15,
    Caret = 15,
    Seven = 16,
    Ampersand = 16,
    Eight = 17,
    Asterisk = 17,
    Nine = 18,
    LParen = 18,
    Zero = 19,
    RParen = 19,
    Dash = 20,
    Underscore = 20,
    Plus = 21,
    Equal = 21,
    BackSpace = 22,
    Tab = 23,
    Num0 = 90,
    Num8 = 80,
    Num9 = 81,
    NumMinus = 82,
    Num4 = 83,
    Num5 = 84,
    Num6 = 85,
    NumPlus = 86,
    Num1 = 87,
    Num2 = 88,
    Num3 = 89,
    Num7 = 79,
    NumDivide = 106,
    NumEnter = 104,
    NumDot = 91,
    NumEqual = undefined_key,
    LControl = 37,
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
    LSystem = 133,
    RControl = 105,
    RAlt = 108,
    RSystem = 133,
    Menu = 135,
    LBracket = 34,
    RBracket = 35,
    SemiColon = 47,
    Colon = 47,
    Quote = 48,
    DoubleQuote = 48,
    BackQuote = 49,
    Tilde = 49,
    LShift = 50,
    BackSlash = 51,
    Separator = 51,
    Comma = 59,
    Less = 59,
    Dot = 60,
    Greater = 60,
    Slash = 61,
    Question = 61,
    RShift = 62,
    NumMultiply = 63,
    LAlt = 64,
    Space = 65,
    CapsLock = 66,
    Return = 36,
    PageUp = 112,
    PageDown = 117,
    End = 115,
    Home = 110,
    Insert = 118,
    Delete = 119,
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
    Pause = undefined_key,
    PrintScr = 107,
    NumLock = 77,
    ScrollLock = 78
};

enum class key_state : u16
{
    none      = 16,
    shift     = 17,
    caps_lock = 18,
    alt       = 24,
    ctrl      = 20
};

// ====================================================

constexpr u8 button (button_type uId) noexcept
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

input::keyboard::key get_key (keycode_type keycode) noexcept
{
    switch (keycode)
    {
    case u8 (key::Zero): return input::keyboard::key::Zero;
    case u8 (key::One): return input::keyboard::key::One;
    case u8 (key::Two): return input::keyboard::key::Two;
    case u8 (key::Three): return input::keyboard::key::Three;
    case u8 (key::Four): return input::keyboard::key::Four;
    case u8 (key::Five): return input::keyboard::key::Five;
    case u8 (key::Six): return input::keyboard::key::Six;
    case u8 (key::Seven): return input::keyboard::key::Seven;
    case u8 (key::Eight): return input::keyboard::key::Eight;
    case u8 (key::Nine): return input::keyboard::key::Nine;
    case u8 (key::Num0): return input::keyboard::key::Numpad0;
    case u8 (key::Num1): return input::keyboard::key::Numpad1;
    case u8 (key::Num2): return input::keyboard::key::Numpad2;
    case u8 (key::Num3): return input::keyboard::key::Numpad3;
    case u8 (key::Num4): return input::keyboard::key::Numpad4;
    case u8 (key::Num5): return input::keyboard::key::Numpad5;
    case u8 (key::Num6): return input::keyboard::key::Numpad6;
    case u8 (key::Num7): return input::keyboard::key::Numpad7;
    case u8 (key::Num8): return input::keyboard::key::Numpad8;
    case u8 (key::Num9): return input::keyboard::key::Numpad9;
    case u8 (key::NumPlus): return input::keyboard::key::NumpadPlus;
    case u8 (key::NumMinus): return input::keyboard::key::NumpadMinus;
    case u8 (key::NumMultiply): return input::keyboard::key::NumpadMultiply;
    case u8 (key::NumDivide): return input::keyboard::key::NumpadDivide;
    case u8 (key::NumEnter): return input::keyboard::key::NumpadEnter;
    case u8 (key::NumDot): return input::keyboard::key::NumpadDot;
    case u8 (key::NumEqual): return input::keyboard::key::undefined_key;
    case u8 (key::A): return input::keyboard::key::A;
    case u8 (key::B): return input::keyboard::key::B;
    case u8 (key::C): return input::keyboard::key::C;
    case u8 (key::D): return input::keyboard::key::D;
    case u8 (key::E): return input::keyboard::key::E;
    case u8 (key::F): return input::keyboard::key::F;
    case u8 (key::G): return input::keyboard::key::G;
    case u8 (key::H): return input::keyboard::key::H;
    case u8 (key::I): return input::keyboard::key::I;
    case u8 (key::J): return input::keyboard::key::J;
    case u8 (key::K): return input::keyboard::key::K;
    case u8 (key::L): return input::keyboard::key::L;
    case u8 (key::M): return input::keyboard::key::M;
    case u8 (key::N): return input::keyboard::key::N;
    case u8 (key::O): return input::keyboard::key::O;
    case u8 (key::P): return input::keyboard::key::P;
    case u8 (key::Q): return input::keyboard::key::Q;
    case u8 (key::R): return input::keyboard::key::R;
    case u8 (key::S): return input::keyboard::key::S;
    case u8 (key::T): return input::keyboard::key::T;
    case u8 (key::U): return input::keyboard::key::U;
    case u8 (key::V): return input::keyboard::key::V;
    case u8 (key::W): return input::keyboard::key::W;
    case u8 (key::X): return input::keyboard::key::X;
    case u8 (key::Y): return input::keyboard::key::Y;
    case u8 (key::Z): return input::keyboard::key::Z;
    case u8 (key::Escape): return input::keyboard::key::Escape;
    case u8 (key::LControl): return input::keyboard::key::LControl;
    case u8 (key::LShift): return input::keyboard::key::LShift;
    case u8 (key::LAlt): return input::keyboard::key::LAlt;
    case u8 (key::LSystem): return input::keyboard::key::LSystem;
    case u8 (key::RControl): return input::keyboard::key::RControl;
    case u8 (key::RShift): return input::keyboard::key::RShift;
    case u8 (key::RAlt): return input::keyboard::key::RAlt;
    case u8 (key::Menu): return input::keyboard::key::Menu;
    case u8 (key::LBracket): return input::keyboard::key::LBracket;
    case u8 (key::RBracket): return input::keyboard::key::RBracket;
    case u8 (key::SemiColon): return input::keyboard::key::SemiColon;
    case u8 (key::Comma): return input::keyboard::key::Comma;
    case u8 (key::Dot): return input::keyboard::key::Dot;
    case u8 (key::Quote): return input::keyboard::key::Quote;
    case u8 (key::Slash): return input::keyboard::key::Slash;
    case u8 (key::BackSlash): return input::keyboard::key::BackSlash;
    case u8 (key::Equal): return input::keyboard::key::Equal;
    case u8 (key::Dash): return input::keyboard::key::Dash;
    case u8 (key::Space): return input::keyboard::key::Space;
    case u8 (key::Return): return input::keyboard::key::Return;
    case u8 (key::BackSpace): return input::keyboard::key::BackSpace;
    case u8 (key::Tab): return input::keyboard::key::Tab;
    case u8 (key::PageUp): return input::keyboard::key::PageUp;
    case u8 (key::PageDown): return input::keyboard::key::PageDown;
    case u8 (key::End): return input::keyboard::key::End;
    case u8 (key::Home): return input::keyboard::key::Home;
    case u8 (key::Insert): return input::keyboard::key::Insert;
    case u8 (key::Delete): return input::keyboard::key::Delete;
    case u8 (key::BackQuote): return input::keyboard::key::BackQuote;
    case u8 (key::Left): return input::keyboard::key::Left;
    case u8 (key::Right): return input::keyboard::key::Right;
    case u8 (key::Up): return input::keyboard::key::Up;
    case u8 (key::Down): return input::keyboard::key::Down;
    case u8 (key::F1): return input::keyboard::key::F1;
    case u8 (key::F2): return input::keyboard::key::F2;
    case u8 (key::F3): return input::keyboard::key::F3;
    case u8 (key::F4): return input::keyboard::key::F4;
    case u8 (key::F5): return input::keyboard::key::F5;
    case u8 (key::F6): return input::keyboard::key::F6;
    case u8 (key::F7): return input::keyboard::key::F7;
    case u8 (key::F8): return input::keyboard::key::F8;
    case u8 (key::F9): return input::keyboard::key::F9;
    case u8 (key::F10): return input::keyboard::key::F10;
    case u8 (key::F11): return input::keyboard::key::F11;
    case u8 (key::F12): return input::keyboard::key::F12;
    case u8 (key::PrintScr): return input::keyboard::key::PrintScr;
    case u8 (key::CapsLock): return input::keyboard::key::CapsLock;
    case u8 (key::NumLock): return input::keyboard::key::NumLock;
    case u8 (key::ScrollLock): return input::keyboard::key::ScrollLock;
    default: return input::keyboard::key::undefined_key;
    }
}

// ====================================================

constexpr input::keyboard::modifier get_modifier (u16 mod)
{
    switch (mod)
    {
    case int (key_state::shift):
        return input::keyboard::modifier::shift;
    case int (key_state::caps_lock):
        return input::keyboard::modifier::caps_lock;
    case int (key_state::alt):
        return input::keyboard::modifier::alt;
    case int (key_state::ctrl):
        return input::keyboard::modifier::ctrl;
    default:
        return input::keyboard::modifier::none;
    }
}

// ====================================================

union event_data
{
    base_event_type          generic;
    btn_event_type           mouseButton;
    mouse_move_event_type    mouseMove;
    key_event_type           key;
    keymap_notify_event_type keymap;
    expose_event_type        paint;
    focus_event_type         focus;
    step_event_type          step;
    resize_event_type        size;
    map_event_type           map;
    unmap_event_type         unmap;
    property_event_type      property;
    destroy_event_type       destroy;
    visibility_event_type    visibility;
    ge_generic_event_type    geGeneric;
    client_msg_event_type    clientMessage;
};

// ====================================================

class event_ptr final
{
public:
    typedef event_data* pointer;

    constexpr operator pointer () const noexcept
    { return _M_handle; }

    constexpr pointer get () const noexcept
    { return _M_handle; }

    constexpr pointer operator -> () const { return _M_handle; }

    ~event_ptr ()
    { if (_M_handle) std::free (_M_handle); }

    constexpr event_ptr (base_event_type* pEvent) noexcept
    : _M_handle (direct_cast<pointer> (pEvent))
    { }

private:
    pointer _M_handle { };
};

// ====================================================

class event_handle final
{
public:
    typedef base_event_type        base_type              ;
    typedef input::event           event_type             ;
    typedef event_handle           self_type              ;
    typedef event_handle*          pointer                ;
    typedef ui::shared_display     display_pointer        ;
    typedef display_pointer&       display_reference      ;
    typedef display_pointer const& display_const_reference;

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

    constexpr event_handle (base_type* base) noexcept
    : _M_pEvent (base)
    { }

    constexpr event_ptr::pointer get () const noexcept
    { return _M_pEvent.get (); }

    constexpr event_ptr::pointer get () noexcept
    { return _M_pEvent.get (); }

    constexpr u32 type () const noexcept
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

    static base_type to_xcb_event (event_type const&) noexcept
    {
        return base_type ();
    }

    constexpr
    static
    ui::xcb_display::data_const_reference data (display_const_reference conn) noexcept
    {
        return static_cast<ui::xcb_display&> (*conn).data ();
    }

    constexpr
    static
    ui::xcb_display::atoms_refrence prev_atoms (display_const_reference conn) noexcept
    {
        return static_cast<ui::xcb_display&> (*conn).prev_atoms ();
    }

    void handle_secret_xlib_event (x::legacy_type* dpy, base_type* ev)
    {
        auto const response_type = ev->response_type & ~0x80;
        Bool (* proc)(x::legacy_type*, ::XEvent*, ::xEvent*);

        ::XLockDisplay (dpy);

        proc = ::XESetWireToEvent (dpy, response_type, nullptr);

        if (proc)
        {
            ::XESetWireToEvent (dpy, response_type, proc);
            ::XEvent xev;

            ev->sequence = static_cast<u16> (LastKnownRequestProcessed (dpy));
            proc (dpy, &xev, (::xEvent*)(ev));
        }

        ::XUnlockDisplay (dpy);

        //if (proc) return 1;

        //return 0;
    }

    static
    circular_queue<std::pair<x::atom_type, bool>> handle_wm_state (display_const_reference     conn,
                                                                   vector<x::atom_type> const& atoms)
    {
        circular_queue<std::pair<x::atom_type, bool>> ret_atoms;
        auto&                                         prev_atom_vector = prev_atoms (conn);

        ret_atoms.reserve (prev_atom_vector.size () <= atoms.size () ?
                           atoms.size () : prev_atom_vector.size  ());

        if (prev_atom_vector.empty ())
        {
            for (auto i = 0U; i < atoms.size (); ++i)
            {
                prev_atom_vector.push_back (atoms[i]);
                ret_atoms.push_back (std::make_pair (atoms[i], true));
            }

            return ret_atoms;
        }

        if (atoms.empty ())
        {
            for (auto n = 0U; n < prev_atom_vector.size (); ++n)
            {
                ret_atoms.push_back (std::make_pair (prev_atom_vector[n], false));
            }

            return ret_atoms;
        }

        for (auto i = 0U; i < atoms.size (); ++i)
        {
            for (auto n = 0U; n < prev_atom_vector.size (); ++n)
            {
                if (atoms[i] == prev_atom_vector[n]) continue;

                auto state_it = std::find (atoms.cbegin (), atoms.cend (),
                                           prev_atom_vector[n]);

                auto atom_it = std::find (prev_atom_vector.begin (), prev_atom_vector.end (),
                                          atoms[i]);

                if (state_it == atoms.cend ())
                {
                    typedef circular_queue<x::atom_type>::const_iterator const_iterator;

                    auto const prev_atom = prev_atom_vector[n];
                    auto const pair = std::pair<x::atom_type, bool> (prev_atom, false);

                    prev_atom_vector.erase (const_iterator (prev_atom_vector, n));

                    if (ret_atoms.empty ()) ret_atoms.push_back (pair);
                    else if (ret_atoms.back () != pair) ret_atoms.push_back (pair);
                }

                if (atom_it == prev_atom_vector.end ())
                {
                    auto const pair = std::pair<x::atom_type, bool> (atoms[i], true);

                    prev_atom_vector.push_back (atoms[i]);

                    if (ret_atoms.empty ()) ret_atoms.push_back (pair);
                    else if (ret_atoms.back () != pair) ret_atoms.push_back (pair);
                }
            }
        }

        return ret_atoms;
    }

    inline event_type to_event (display_const_reference conn) const noexcept
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
            return input::key_press_event ({ get_key (get ()->key.detail), get_modifier (get ()->key.state) });
        case TKeyRelease:
            return input::key_release_event ({ get_key(get ()->key.detail), get_modifier (get ()->key.state) });
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
            if (get ()->clientMessage.data.data32[0] == data(conn).WM_DELETE_WINDOW)
            {
                return event_type (event_type::destroy);
            }

            return event_type (event_type::null);
        default:
            #ifdef DEBUG_MODE
            std::cout << __PRETTY_FUNCTION__ << " :: unhandled event: " << type () << std::endl;
            #endif
            return event_type (event_type::null);
        }

        return event_type (event_type::null);
    }

    inline void operator () (display_const_reference conn)
    {
        switch (type ())
        {
        case TMousePress:
            switch (get ()->mouseButton.detail)
            {
            case MouseWheelUp:
                ui::event_queue::events ().scroll (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y }, 1 });
                break;
            case MouseWheelDown:
                ui::event_queue::events ().scroll (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y }, -1 });
                break;
            default:
                ui::event_queue::events ().mousePress (window (),
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
                ui::event_queue::events ().mouseRelease (window (),
                {{ get ()->mouseButton.event_x, get ()->mouseButton.event_y },
                 button (get ()->mouseButton.detail) });
                break;
            }
            break;
        case TMouseMove:
            ui::event_queue::events ().mouseMove (window (),
            { get ()->mouseMove.event_x, get ()->mouseMove.event_y });
            break;
        case TKeyPress:
            ui::event_queue::events ().keyPress (window (),
            { get_key (get ()->key.detail), get_modifier (get ()->key.state) });
            break;
        case TKeyRelease:
            ui::event_queue::events ().keyPress (window (),
            { get_key (get ()->key.detail), get_modifier (get ()->key.state) });
            break;
        case TExpose:
            ui::event_queue::events ().winPaint (window (),
            { rect (static_cast<i16> (get ()->paint.x),
              static_cast<i16> (get ()->paint.y),
              get ()->paint.width,
              get ()->paint.height) });
            break;
        case TEnter:
            ui::event_queue::events ().winStep (window (), true);
            break;
        case TLeave:
            ui::event_queue::events ().winStep (window (), false);
            break;
        case TFocusIn:
            ui::event_queue::events ().winFocus (window (), true);
            break;
        case TFocusOut:
            ui::event_queue::events ().winFocus (window (), false);
            break;
        case TSize:
            handle_secret_xlib_event (conn->legacy<x::legacy_type> (), &get ()->generic);

            ui::event_queue::events ().winSize (window (),
            { get ()->size.width, get ()->size.height });
            break;
        case TProperty:
            if (get ()->property.atom == data (conn)._NET_WM_STATE)
            {
                auto prop = x::get_property (conn->native<x::display_type> (),
                                             x::dont_delete,
                                             get ()->property.window,
                                             data (conn)._NET_WM_STATE,
                                             XCB_ATOM_ATOM,
                                             x::offset_begin,
                                             x::atom_max_len);

                if (prop->format != x::atom_format) break;

                //std::cout << "_NET_WM_STATE property changed" << std::endl;

                auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
                auto const states_end = states + prop->length;

                vector<x::atom_type> const atoms (states, states_end);

                std::cout << "prev_atoms";

                for (auto atom : prev_atoms (conn))
                {
                    if (atom == x::atom_type ()) continue;

                    std::cout << " :: [" << atom << "] "
                              << x::get_atom_name (conn->native<x::display_type> (), atom).name ();
                }

                std::cout << "\n_NET_WM_STATE";

                for (auto atom : atoms)
                {
                    std::cout << " :: [" << atom << "] "
                              << x::get_atom_name (conn->native<x::display_type> (), atom).name ();
                }

                std::cout << std::endl;

                auto const changed_vector = handle_wm_state (conn, atoms);

                for (auto& state : changed_vector)
                {
                    if (state.first == data (conn)._NET_WM_STATE_MAXIMIZED_HORZ ||
                        state.first == data (conn)._NET_WM_STATE_MAXIMIZED_VERT)
                    {
                        ui::event_queue::events ().winMaximize (window (), state.second);
                        //break;
                    }
                    else if (state.first == data (conn)._NET_WM_STATE_FULLSCREEN)
                    {
                        ui::event_queue::events ().winFullscreen (window (), state.second);
                        //break;
                    }
                    else if (state.first == data (conn)._NET_WM_STATE_HIDDEN)
                    {
                        ui::event_queue::events ().winMinimize (window (), state.second);
                        //break;
                    }
                }
            }
            /*else if (get ()->property.atom == data (conn).WM_STATE)
            {
                auto prop = x::get_property (conn->native<x::display_type> (),
                                             x::dont_delete,
                                             window (),
                                             data (conn)._NET_WM_STATE,
                                             XCB_ATOM_ATOM,
                                             x::offset_begin,
                                             x::atom_max_len);

                if (prop->format != x::atom_format) break;

                //std::cout << "WM_STATE property changed" << std::endl;

                auto const states     = static_cast<x::atom_type*> (::xcb_get_property_value (&(*prop)));
                auto const states_end = states + prop->length;

                vector<x::atom_type> const atoms (states, states_end);

                //std::cout << "prev_atoms";

                //for (auto atom : prev_atoms (conn))
                //{
                      if (atom == x::atom_type ()) continue;
                //
                //    std::cout << " :: [" << atom << "] "
                //              << x::get_atom_name (conn->native<x::display_type> (), atom).name ();
                //}

                //std::cout << std::endl;

                //std::cout << "WM_STATE";

                //for (auto atom : atoms)
                //{
                //    std::cout << " :: [" << atom << "] "
                //              << x::get_atom_name (conn->native<x::display_type> (), atom).name ();
                //}

                //std::cout << std::endl;

                auto const changed_vector = handle_wm_state (conn, atoms);

                for (auto& state : changed_vector)
                {
                    if (state.first == data (conn)._NET_WM_STATE_HIDDEN)
                    {
                        ui::event_queue::events ().winMinimize (window (), state.second);
                        //break;
                    }
                }
            }*/
            else
            {
                ui::event_queue::events ().winProperty (window (),
                { get ()->property.atom, get ()->property.state });
            }
            break;
        case TMap:
            ui::event_queue::events ().winVisible (window (), true);
            break;
        case TUnmap:
            ui::event_queue::events ().winVisible (window (), false);
            break;
        case TDestroy:
            ui::event_queue::events ().winClose (window ());
            break;
        case TClientMessage:
            {
                if (get ()->clientMessage.data.data32[0] == data(conn).WM_DELETE_WINDOW)
                {
                    ui::event_queue::events ().winClose (window ());
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
    event_ptr _M_pEvent;
};

} } // namespace x

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_EVENT_H_
