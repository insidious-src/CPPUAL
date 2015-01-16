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

#ifndef CPPUAL_DEVICES_DEFAULT_EVENTS_H
#define CPPUAL_DEVICES_DEFAULT_EVENTS_H
#ifdef __cplusplus

#include <cppual/ui/events.h>

namespace cppual { namespace Ui {

struct MessageEvent : public Event
{
	inline MessageEvent (int nMsg) noexcept
	: Event (Event::SystemMessage)
	{
		m_gData.message = nMsg;
	}
};

// =========================================================

struct VisibilityEvent : public Event
{
	inline VisibilityEvent (window_type wnd, bool bVis) noexcept
	: Event (Event::Visibility, wnd)
	{
		m_gData.visibility.visible = bVis;
	}
};

// =========================================================

struct PaintEvent : public Event
{
	inline PaintEvent (window_type wnd, Rect gRect) noexcept
	: Event (Event::Paint, wnd)
	{
		m_gData.paint.region = gRect;
	}
};

// =========================================================

struct SizeEvent : public Event
{
	inline SizeEvent (window_type wnd, point2u size) noexcept
	: Event (Event::Size, wnd)
	{
		m_gData.size.size = size;
	}
};

// =========================================================

struct FocusEvent : public Event
{
	inline FocusEvent (window_type wnd, bool in) noexcept
	: Event (Event::Focus, wnd)
	{
		m_gData.state.in = in;
	}
};

// =========================================================

struct StepEvent : public Event
{
	inline StepEvent (window_type wnd, bool in) noexcept
	: Event (Event::Step, wnd)
	{
		m_gData.state.in = in;
	}
};

// =========================================================

struct PropertyEvent : public Event
{
	inline PropertyEvent (window_type wnd, u32 prop, int32 value) noexcept
	: Event (Event::Property, wnd)
	{
		m_gData.property.prop  = prop;
		m_gData.property.value = value;
	}
};

// =========================================================

struct KeyPressEvent : public Event
{
	inline KeyPressEvent (window_type wnd, KeyData nKey) noexcept
	: Event (Event::KeyPressed, wnd)
	{
		m_gData.keyCode = nKey;
	}
};

// =========================================================

struct KeyReleaseEvent : public Event
{
	inline KeyReleaseEvent (window_type wnd, KeyData nKey) noexcept
	: Event (Event::KeyReleased, wnd)
	{
		m_gData.keyCode = nKey;
	}
};

// =========================================================

struct MousePressEvent : public Event
{
	inline MousePressEvent (window_type wnd, u8 nBtn, point2i gPos) noexcept
	: Event (Event::ButtonDown, wnd)
	{
		m_gData.mouseButton.button = nBtn;
		m_gData.mouseButton.pos    = gPos;
	}
};

// =========================================================

struct MouseReleaseEvent : public Event
{
	inline MouseReleaseEvent (window_type wnd, u8 nBtn, point2i gPos) noexcept
	: Event (Event::ButtonUp, wnd)
	{
		m_gData.mouseButton.button  = nBtn;
		m_gData.mouseButton.pos     = gPos;
	}
};

// =========================================================

struct MouseMoveEvent : public Event
{
	inline MouseMoveEvent (window_type wnd, point2i gPos) noexcept
	: Event (Event::PointerMove, wnd)
	{
		m_gData.mouseButton.pos = gPos;
	}
};

// =========================================================

struct ScrollEvent : public Event
{
	inline ScrollEvent (window_type wnd, int32 nDelta, point2i gPos) noexcept
	: Event (Event::Scroll, wnd)
	{
		m_gData.scroll.delta = nDelta;
		m_gData.scroll.pos   = gPos;
	}
};

// =========================================================

struct TouchPressEvent : public Event
{
	inline TouchPressEvent (int32 pid, point2i gPos) noexcept
	: Event (Event::TouchPress)
	{
		m_gData.touch.pid = pid;
		m_gData.touch.pos = gPos;
	}
};

// =========================================================

struct TouchReleaseEvent : public Event
{
	inline TouchReleaseEvent (int32 pid, point2i gPos) noexcept
	: Event (Event::TouchRelease)
	{
		m_gData.touch.pid = pid;
		m_gData.touch.pos = gPos;
	}
};

// =========================================================

struct TouchMovedEvent : public Event
{
	inline TouchMovedEvent (int32 pid, point2i gPos) noexcept
	: Event (Event::TouchMove)
	{
		m_gData.touch.pid = pid;
		m_gData.touch.pos = gPos;
	}
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_DEFAULT_EVENTS_H
