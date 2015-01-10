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
	inline MessageEvent () noexcept = default;

	inline MessageEvent (int nMsg) noexcept
	: Event (Event::SystemMessage)
	{ m_gData.message = nMsg; }
};

// =========================================================

struct VisibilityEvent : public Event
{
	inline VisibilityEvent () noexcept = default;

	inline VisibilityEvent (Graphics::Element elem, bool bVis) noexcept
	: Event (Event::Visibility)
	{
		m_gData.visibility.element = elem;
		m_gData.visibility.visible = bVis;
	}
};

// =========================================================

struct PaintEvent : public Event
{
	inline PaintEvent () noexcept = default;

	inline PaintEvent (Graphics::Element elem, Rect gRect) noexcept
	: Event (Event::Paint)
	{
		m_gData.paint.element = elem;
		m_gData.paint.region  = gRect;
	}
};

// =========================================================

struct SizeEvent : public Event
{
	inline SizeEvent () noexcept = default;

	inline SizeEvent (Graphics::Element elem, point2u size) noexcept
	: Event (Event::Size)
	{
		m_gData.size.element = elem;
		m_gData.size.size    = size;
	}
};

// =========================================================

struct MoveEvent : public Event
{
	inline MoveEvent () noexcept = default;

	inline MoveEvent (Graphics::Element elem, point2i pos) noexcept
	: Event (Event::Move)
	{
		m_gData.move.element = elem;
		m_gData.move.pos     = pos;
	}
};

// =========================================================

struct FocusEvent : public Event
{
	inline FocusEvent () noexcept = default;

	inline FocusEvent (Graphics::Element elem, bool in) noexcept
	: Event (Event::Focus)
	{
		m_gData.state.element = elem;
		m_gData.state.in      = in;
	}
};

// =========================================================

struct StepEvent : public Event
{
	inline StepEvent () noexcept = default;

	inline StepEvent (Graphics::Element elem, bool in) noexcept
	: Event (Event::Step)
	{
		m_gData.state.element = elem;
		m_gData.state.in      = in;
	}
};

// =========================================================

struct PropertyEvent : public Event
{
	inline PropertyEvent () noexcept = default;

	inline PropertyEvent (Graphics::Element elem, u32 prop, int32 value) noexcept
	: Event (Event::Property)
	{
		m_gData.property.element = elem;
		m_gData.property.prop    = prop;
		m_gData.property.value   = value;
	}
};

// =========================================================

struct KeyPressEvent : public Event
{
	inline KeyPressEvent () noexcept = default;

	inline KeyPressEvent (KeyData nKey) noexcept
	: Event (Event::KeyPressed)
	{ m_gData.keyCode = nKey; }
};

// =========================================================

struct KeyReleaseEvent : public Event
{
	inline KeyReleaseEvent () noexcept = default;

	inline KeyReleaseEvent (KeyData nKey) noexcept
	: Event (Event::KeyReleased)
	{ m_gData.keyCode = nKey; }
};

// =========================================================

struct KeysMappedEvent : public Event
{
	inline KeysMappedEvent () noexcept = default;

	inline KeysMappedEvent (KeyData nMap) noexcept
	: Event (Event::KeyMap)
	{
		m_gData.keyCode = nMap;
	}
};

// =========================================================

struct MousePressEvent : public Event
{
	inline MousePressEvent () noexcept = default;

	inline MousePressEvent (Graphics::Element elem,
							u8                nBtn,
							point2i           gPos) noexcept
	: Event (Event::ButtonDown)
	{
		m_gData.mouseButton.element = elem;
		m_gData.mouseButton.button  = nBtn;
		m_gData.mouseButton.pos     = gPos;
	}
};

// =========================================================

struct MouseReleaseEvent : public Event
{
	inline MouseReleaseEvent () noexcept = default;

	inline MouseReleaseEvent (Graphics::Element elem,
							  u8                nBtn,
							  point2i           gPos) noexcept
	: Event (Event::ButtonUp)
	{
		m_gData.mouseButton.element = elem;
		m_gData.mouseButton.button  = nBtn;
		m_gData.mouseButton.pos     = gPos;
	}
};

// =========================================================

struct MouseMoveEvent : public Event
{
	inline MouseMoveEvent () noexcept = default;

	inline MouseMoveEvent (Graphics::Element elem,
						   point2i           gPos) noexcept
	: Event (Event::PointerMove)
	{
		m_gData.mouseButton.element = elem;
		m_gData.mouseButton.pos     = gPos;
	}
};

// =========================================================

struct ScrollEvent : public Event
{
	inline ScrollEvent () noexcept = default;

	inline ScrollEvent (Graphics::Element elem,
						int32             nDelta,
						point2i           gPos) noexcept
	: Event (Event::Scroll)
	{
		m_gData.mouseButton.element = elem;
		m_gData.scroll.delta        = nDelta;
		m_gData.scroll.pos          = gPos;
	}
};

// =========================================================

struct TouchPressEvent : public Event
{
	inline TouchPressEvent () noexcept = default;

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
	inline TouchReleaseEvent () noexcept = default;

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
	inline TouchMovedEvent () noexcept = default;

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
