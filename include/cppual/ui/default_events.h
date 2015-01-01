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

struct MessageEvent : public Input::IDisplayQueue::event_type
{
	inline MessageEvent () noexcept = default;

	inline MessageEvent (int nMsg) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::SystemMessage)
	{ m_gData.message = nMsg; }
};

// =========================================================

struct VisibilityEvent : public Input::IDisplayQueue::event_type
{
	inline VisibilityEvent () noexcept = default;

	inline VisibilityEvent (Graphics::Element elem, bool bVis) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Visibility)
	{
		m_gData.visibility.element = elem;
		m_gData.visibility.visible = bVis;
	}
};

// =========================================================

struct PaintEvent : public Input::IDisplayQueue::event_type
{
	inline PaintEvent () noexcept = default;

	inline PaintEvent (Graphics::Element elem, Rect gRect) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Paint)
	{
		m_gData.paint.element = elem;
		m_gData.paint.region  = gRect;
	}
};

// =========================================================

struct SizeEvent : public Input::IDisplayQueue::event_type
{
	inline SizeEvent () noexcept = default;

	inline SizeEvent (Graphics::Element elem, point2u size) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Size)
	{
		m_gData.size.element = elem;
		m_gData.size.size    = size;
	}
};

// =========================================================

struct MoveEvent : public Input::IDisplayQueue::event_type
{
	inline MoveEvent () noexcept = default;

	inline MoveEvent (Graphics::Element elem, point2i pos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Move)
	{
		m_gData.move.element = elem;
		m_gData.move.pos     = pos;
	}
};

// =========================================================

struct FocusEvent : public Input::IDisplayQueue::event_type
{
	inline FocusEvent () noexcept = default;

	inline FocusEvent (Graphics::Element elem, bool in) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Focus)
	{
		m_gData.state.element = elem;
		m_gData.state.in      = in;
	}
};

// =========================================================

struct StepEvent : public Input::IDisplayQueue::event_type
{
	inline StepEvent () noexcept = default;

	inline StepEvent (Graphics::Element elem, bool in) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Step)
	{
		m_gData.state.element = elem;
		m_gData.state.in      = in;
	}
};

// =========================================================

struct PropertyEvent : public Input::IDisplayQueue::event_type
{
	inline PropertyEvent () noexcept = default;

	inline PropertyEvent (Graphics::Element elem, u32 prop, int32 value) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Property)
	{
		m_gData.property.element = elem;
		m_gData.property.prop    = prop;
		m_gData.property.value   = value;
	}
};

// =========================================================

struct KeyPressEvent : public Input::IDisplayQueue::event_type
{
	inline KeyPressEvent () noexcept = default;

	inline KeyPressEvent (u8 nKey) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::KeyPressed)
	{ m_gData.key = nKey; }
};

// =========================================================

struct KeyReleaseEvent : public Input::IDisplayQueue::event_type
{
	inline KeyReleaseEvent () noexcept = default;

	inline KeyReleaseEvent (u8 nKey) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::KeyReleased)
	{ m_gData.key = nKey; }
};

// =========================================================

struct KeysMappedEvent : public Input::IDisplayQueue::event_type
{
	inline KeysMappedEvent () noexcept = default;

	inline KeysMappedEvent (KeyData const& gKeyCode) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::KeyMap)
	{ m_gData.keyCode = gKeyCode; }

	inline KeysMappedEvent (u8 nKey, KeyMods mod = KeyMods (nullptr)) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::KeyMap)
	{
		m_gData.keyCode.key = nKey;
		m_gData.keyCode.mod = mod;
	}
};

// =========================================================

struct MousePressEvent : public Input::IDisplayQueue::event_type
{
	inline MousePressEvent () noexcept = default;

	inline MousePressEvent (Graphics::Element elem,
							u8                nBtn,
							point2i           gPos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::ButtonDown)
	{
		m_gData.mouseButton.element = elem;
		m_gData.mouseButton.button  = nBtn;
		m_gData.mouseButton.pos     = gPos;
	}
};

// =========================================================

struct MouseReleaseEvent : public Input::IDisplayQueue::event_type
{
	inline MouseReleaseEvent () noexcept = default;

	inline MouseReleaseEvent (Graphics::Element elem,
							  u8                nBtn,
							  point2i           gPos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::ButtonUp)
	{
		m_gData.mouseButton.element = elem;
		m_gData.mouseButton.button  = nBtn;
		m_gData.mouseButton.pos     = gPos;
	}
};

// =========================================================

struct MouseMoveEvent : public Input::IDisplayQueue::event_type
{
	inline MouseMoveEvent () noexcept = default;

	inline MouseMoveEvent (Graphics::Element elem,
						   point2i           gPos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::PointerMove)
	{
		m_gData.mouseButton.element = elem;
		m_gData.mouseButton.pos     = gPos;
	}
};

// =========================================================

struct ScrollEvent : public Input::IDisplayQueue::event_type
{
	inline ScrollEvent () noexcept = default;

	inline ScrollEvent (Graphics::Element elem,
						int32             nDelta,
						point2i           gPos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::Scroll)
	{
		m_gData.mouseButton.element = elem;
		m_gData.scroll.delta        = nDelta;
		m_gData.scroll.pos          = gPos;
	}
};

// =========================================================

struct TouchPressEvent : public Input::IDisplayQueue::event_type
{
	inline TouchPressEvent () noexcept = default;

	inline TouchPressEvent (int32 pid, point2i gPos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::TouchPress)
	{
		m_gData.touch.pid = pid;
		m_gData.touch.pos = gPos;
	}
};

// =========================================================

struct TouchReleaseEvent : public Input::IDisplayQueue::event_type
{
	inline TouchReleaseEvent () noexcept = default;

	inline TouchReleaseEvent (int32 pid, point2i gPos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::TouchRelease)
	{
		m_gData.touch.pid = pid;
		m_gData.touch.pos = gPos;
	}
};

// =========================================================

struct TouchMovedEvent : public Input::IDisplayQueue::event_type
{
	inline TouchMovedEvent () noexcept = default;

	inline TouchMovedEvent (int32 pid, point2i gPos) noexcept
	: Input::IDisplayQueue::event_type (Input::IDisplayQueue::event_type::TouchMove)
	{
		m_gData.touch.pid = pid;
		m_gData.touch.pos = gPos;
	}
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_DEFAULT_EVENTS_H
