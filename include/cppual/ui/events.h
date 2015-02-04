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

#ifndef CPPUAL_UI_EVENTS_H
#define CPPUAL_UI_EVENTS_H
#ifdef __cplusplus

#include <iostream>
#include <cppual/flags.h>
#include <cppual/signal.h>
#include <cppual/gfx/coord.h>
#include <cppual/gfx/dsp_details.h>

using std::atomic;
using std::atomic_bool;
using std::shared_ptr;

namespace cppual { namespace Ui {

struct SystemMessage final
{
	enum
	{
		Quit,
		Child,
		Terminate
	};
};

// =========================================================

class Event
{
public:

    typedef Graphics::Element window_type;
    typedef std::size_t       size_type;

	enum Type
	{
		Null          = 0,
		KeyPressed    = 1 <<  0,
		KeyReleased   = 1 <<  1,
		ButtonDown    = 1 <<  2,
		ButtonUp      = 1 <<  3,
		PointerMove   = 1 <<  4,
		Scroll        = 1 <<  5,
		TouchPress    = 1 <<  6,
		TouchRelease  = 1 <<  7,
		TouchMove     = 1 <<  8,
		SystemMessage = 1 <<  9,
		Paint         = 1 << 10,
		Focus         = 1 << 11,
		Step          = 1 << 12,
		Size          = 1 << 13,
		Visibility    = 1 << 14,
		Property      = 1 << 15
	};

	enum
	{
		Key       = KeyPressed | KeyReleased,
		Pointer	  = ButtonDown | ButtonUp     | PointerMove | Scroll,
		Touch	  = TouchPress | TouchRelease | TouchMove,
		Window    = Paint | Focus   | Size    | Visibility | Property,
		AllEvents = Key   | Pointer | Touch   | Window     | SystemMessage
	};

	struct KeyData
	{
		u8 key;
	};

	struct MouseButtonData
	{
		point2i pos;
		u8      button;
	};

	struct MouseMoveData
	{
		point2i pos;
	};

	struct ScrollData
	{
		point2i pos;
		int32   delta;
	};

	struct TouchData
	{
		int32   pid;
		point2i pos;
	};

	struct SizeData
	{
		point2u size;
	};

	struct PaintData
	{
		Rect region;
    };

	struct PropertyData
	{
		u32   prop;
		int32 value;
	};

	union Data
	{
		KeyData         keyCode;
		MouseButtonData mouseButton;
		MouseMoveData   mouseMove;
		ScrollData      scroll;
		TouchData       touch;
		int32           message;
		PaintData       paint;
        SizeData        size;
        bool            state;
		PropertyData    property;
	};

	struct Signals final : NonCopyable
	{
		Signal<void(window_type, KeyData)>         keyPress;
		Signal<void(window_type, KeyData)>         keyRelease;
		Signal<void(window_type, MouseButtonData)> mousePress;
		Signal<void(window_type, MouseButtonData)> mouseRelease;
		Signal<void(window_type, MouseMoveData)>   mouseMove;
		Signal<void(window_type, ScrollData)>      scroll;
		Signal<void(window_type, TouchData)>       touchPress;
		Signal<void(window_type, TouchData)>       touchRelease;
		Signal<void(window_type, TouchData)>       touchMove;
		Signal<void(window_type, int32)>           sysMessage;
		Signal<void(window_type, PaintData)>       winPaint;
		Signal<void(window_type, SizeData)>        winSize;
        Signal<void(window_type, bool)>            winFocus;
        Signal<void(window_type, bool)>            winStep;
		Signal<void(window_type, PropertyData)>    winProperty;
        Signal<void(window_type, bool)>            winVisible;
	};

	Event () noexcept = default;
	constexpr Data const& data   () const noexcept { return m_gData; }
	constexpr Event::Type type   () const noexcept { return m_eType; }
    constexpr window_type window () const noexcept { return m_window; }

    static Signals& registers ()
	{
		static Signals event_signals;
		return event_signals;
	}

    constexpr Event (Type type, window_type window = window_type ()) noexcept
	: m_window (window),
	  m_gData  (),
	  m_eType  (type)
	{ }

    inline void operator () () noexcept
    {
        switch (type ())
        {
        case KeyPressed:
            registers ().keyPress (window (), data ().keyCode);
            break;
        case KeyReleased:
            registers ().keyRelease (window (), data ().keyCode);
            break;
        case ButtonDown:
            registers ().mousePress (window (), data ().mouseButton);
            break;
        case ButtonUp:
            registers ().mouseRelease (window (), data ().mouseButton);
            break;
        case PointerMove:
            registers ().mouseMove (window (), data ().mouseMove);
            break;
        case Scroll:
            registers ().scroll (window (), data ().scroll);
            break;
        case TouchPress:
            registers ().touchPress (window (), data ().touch);
            break;
        case TouchRelease:
            registers ().touchRelease (window (), data ().touch);
            break;
        case TouchMove:
            registers ().touchMove (window (), data ().touch);
            break;
        case SystemMessage:
            registers ().sysMessage (window (), data ().message);
            break;
        case Paint:
            registers ().winPaint (window (), data ().paint);
            break;
        case Size:
            registers ().winSize (window (), data ().size);
            break;
        case Focus:
            registers ().winFocus (window (), data ().state);
            break;
        case Step:
            registers ().winStep (window (), data ().state);
            break;
        case Visibility:
            registers ().winVisible (window (), data ().state);
            break;
        case Property:
            registers ().winProperty (window (), data ().property);
            break;
        default:
            std::cerr << "unknown event\n";
        }
    }

protected:
	window_type m_window;
	Data        m_gData;
	Type        m_eType;
};

// =========================================================

struct MessageEvent : public Event
{
    MessageEvent (int nMsg) noexcept
    : Event (Event::SystemMessage)
    {
        m_gData.message = nMsg;
    }
};

// =========================================================

struct VisibilityEvent : public Event
{
    VisibilityEvent (window_type wnd, bool bVis) noexcept
    : Event (Event::Visibility, wnd)
    {
        m_gData.state = bVis;
    }
};

// =========================================================

struct PaintEvent : public Event
{
    PaintEvent (window_type wnd, Rect gRect) noexcept
    : Event (Event::Paint, wnd)
    {
        m_gData.paint.region = gRect;
    }
};

// =========================================================

struct SizeEvent : public Event
{
    SizeEvent (window_type wnd, point2u size) noexcept
    : Event (Event::Size, wnd)
    {
        m_gData.size.size = size;
    }
};

// =========================================================

struct FocusEvent : public Event
{
    FocusEvent (window_type wnd, bool in) noexcept
    : Event (Event::Focus, wnd)
    {
        m_gData.state = in;
    }
};

// =========================================================

struct StepEvent : public Event
{
    StepEvent (window_type wnd, bool in) noexcept
    : Event (Event::Step, wnd)
    {
        m_gData.state = in;
    }
};

// =========================================================

struct PropertyEvent : public Event
{
    PropertyEvent (window_type wnd, u32 prop, int32 value) noexcept
    : Event (Event::Property, wnd)
    {
        m_gData.property.prop  = prop;
        m_gData.property.value = value;
    }
};

// =========================================================

struct KeyPressEvent : public Event
{
    KeyPressEvent (window_type wnd, u8 nKey) noexcept
    : Event (Event::KeyPressed, wnd)
    {
        m_gData.keyCode.key = nKey;
    }
};

// =========================================================

struct KeyReleaseEvent : public Event
{
    KeyReleaseEvent (window_type wnd, u8 nKey) noexcept
    : Event (Event::KeyReleased, wnd)
    {
        m_gData.keyCode.key = nKey;
    }
};

// =========================================================

struct MousePressEvent : public Event
{
    MousePressEvent (window_type wnd, u8 nBtn, point2i gPos) noexcept
    : Event (Event::ButtonDown, wnd)
    {
        m_gData.mouseButton.button = nBtn;
        m_gData.mouseButton.pos    = gPos;
    }
};

// =========================================================

struct MouseReleaseEvent : public Event
{
    MouseReleaseEvent (window_type wnd, u8 nBtn, point2i gPos) noexcept
    : Event (Event::ButtonUp, wnd)
    {
        m_gData.mouseButton.button  = nBtn;
        m_gData.mouseButton.pos     = gPos;
    }
};

// =========================================================

struct MouseMoveEvent : public Event
{
    MouseMoveEvent (window_type wnd, point2i gPos) noexcept
    : Event (Event::PointerMove, wnd)
    {
        m_gData.mouseButton.pos = gPos;
    }
};

// =========================================================

struct ScrollEvent : public Event
{
    ScrollEvent (window_type wnd, int32 nDelta, point2i gPos) noexcept
    : Event (Event::Scroll, wnd)
    {
        m_gData.scroll.delta = nDelta;
        m_gData.scroll.pos   = gPos;
    }
};

// =========================================================

struct TouchPressEvent : public Event
{
    TouchPressEvent (int32 pid, point2i gPos) noexcept
    : Event (Event::TouchPress)
    {
        m_gData.touch.pid = pid;
        m_gData.touch.pos = gPos;
    }
};

// =========================================================

struct TouchReleaseEvent : public Event
{
    TouchReleaseEvent (int32 pid, point2i gPos) noexcept
    : Event (Event::TouchRelease)
    {
        m_gData.touch.pid = pid;
        m_gData.touch.pos = gPos;
    }
};

// =========================================================

struct TouchMovedEvent : public Event
{
    TouchMovedEvent (int32 pid, point2i gPos) noexcept
    : Event (Event::TouchMove)
    {
        m_gData.touch.pid = pid;
        m_gData.touch.pos = gPos;
    }
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_UI_EVENTS_H
