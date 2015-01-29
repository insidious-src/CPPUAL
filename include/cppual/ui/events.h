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

#ifndef CPPUAL_DEVICES_EVENT_H
#define CPPUAL_DEVICES_EVENT_H
#ifdef __cplusplus

#include <atomic>
#include <memory>
#include <cppual/flags.h>
#include <cppual/circular_queue.h>
#include <cppual/signal.h>
#include <cppual/ui/wm.h>

using std::atomic;
using std::atomic_bool;
using std::shared_ptr;

namespace cppual { namespace Ui {

class   View;
class   IDisplayQueue;
typedef shared_ptr<IDisplayQueue> shared_queue;

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
	typedef std::size_t size_type;
	typedef Element     window_type;

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

	struct StateData
	{
		bool in;
	};

	struct VisibilityData
	{
		bool visible;
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
		VisibilityData  visibility;
		StateData       state;
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
		Signal<void(window_type, StateData)>       winFocus;
		Signal<void(window_type, StateData)>       winStep;
		Signal<void(window_type, PropertyData)>    winProperty;
		Signal<void(window_type, VisibilityData)>  winVisible;
	};

	void operator () () noexcept;

	Event () noexcept = default;
	constexpr Data const& data   () const noexcept { return m_gData; }
	constexpr Event::Type type   () const noexcept { return m_eType; }
	constexpr Element     window () const noexcept { return m_window; }

	inline static Signals& registers ()
	{
		static Signals event_signals;
		return event_signals;
	}

	constexpr Event (Type type, Element window = Element ()) noexcept
	: m_window (window),
	  m_gData  (),
	  m_eType  (type)
	{ }

protected:
	window_type m_window;
	Data        m_gData;
	Type        m_eType;
};

// =========================================================

class IDisplayQueue : public NonCopyableVirtual
{
public:
	typedef BitSet<Event::Type> mask_type;
	typedef Event               event_type;

	virtual bool pop_front (event_type&, bool wait) noexcept = 0;
	virtual bool setWindowEvents (IWindow&, mask_type) noexcept = 0;

	static IDisplayQueue* instance ();
	static bool           hasValidInstance () noexcept;

	Connection display () const noexcept { return m_display; }
	bool       isValid () const noexcept { return m_display; }

	constexpr IDisplayQueue (Connection display) noexcept
	: m_display (display)
	{ }

private:
	Connection m_display;
};

// =========================================================

class EventQueue : public NonCopyable
{
public:
	typedef IDisplayQueue::event_type event_type;
	typedef IDisplayQueue::mask_type  mask_type;
	typedef CircularQueue<event_type> queue_type;
	typedef std::size_t               size_type;
	typedef atomic<mask_type>         atomic_mask;

	EventQueue () noexcept;
	EventQueue (mask_type accept_events) noexcept;

	bool pop_front (event_type& next_event, bool wait) noexcept;
	int  poll      (bool wait = true) noexcept;

	mask_type accepted () const noexcept
	{ return m_gAcceptedEvents.load (); }

	void push_back (event_type const& event) noexcept
	{ m_gEventQueue.push_back (event); }

	void accept (mask_type events) noexcept
	{ m_gAcceptedEvents = events; }

	void quit () noexcept
	{ m_bPoll = false; }

	bool isPolling () const noexcept
	{ return m_bPoll.load (); }

private:
	queue_type  m_gEventQueue;
	atomic_mask m_gAcceptedEvents;
	atomic_bool m_bPoll;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_EVENT_H
