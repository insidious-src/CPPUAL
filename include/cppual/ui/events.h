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
#include <cppual/input/keyboard.h>
#include <cppual/input/pointer.h>
#include <cppual/input/joystick.h>
#include <cppual/ui/wm.h>

using std::atomic;
using std::atomic_bool;
using std::shared_ptr;

namespace cppual { namespace Ui {

class View;

} } // namespace Ui

namespace cppual { namespace Input {

struct  IDisplayQueue;
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

template <>
class Event <Ui::View>
{
public:
	typedef Joystick::Axis axis_type;
	typedef std::size_t	   size_type;

	enum Type
	{
		Null          = 0,
		KeyPressed    = 1 <<  0,
		KeyReleased   = 1 <<  1,
		KeyMap     = 1 <<  2,
		ButtonDown    = 1 <<  3,
		ButtonUp      = 1 <<  4,
		PointerMove   = 1 <<  5,
		Scroll        = 1 <<  6,
		TouchPress    = 1 <<  7,
		TouchRelease  = 1 <<  8,
		TouchMove     = 1 <<  9,
		SystemMessage = 1 << 10,
		Paint         = 1 << 11,
		Focus         = 1 << 12,
		Step          = 1 << 13,
		Size          = 1 << 14,
		Move          = 1 << 15,
		Visibility    = 1 << 16,
		Property      = 1 << 17
	};

	enum
	{
		Key		  = KeyPressed  | KeyReleased   | KeyMap,
		Pointer	  = ButtonDown  | ButtonUp      | PointerMove | Scroll,
		Touch	  = TouchPress  | TouchRelease  | TouchMove,
		Window    = Paint | Focus   | Size   | Move   | Visibility | Property,
		AllEvents = Key   | Pointer | Touch  | Window | SystemMessage
	};

	enum KeyMod
	{
		Alt     = 1 << 0,
		Control = 1 << 1,
		Shift   = 1 << 2,
		System  = 1 << 3
	};

	typedef BitSet<KeyMod> KeyMods;

	struct KeyData
	{
		u8      key;
		KeyMods mod;
	};

	struct MouseButtonData
	{
		Graphics::Element element;
		point2i           pos;
		u8                button;
	};

	struct MouseMoveData
	{
		Graphics::Element element;
		point2i           pos;
	};

	struct ScrollData
	{
		Graphics::Element element;
		point2i           pos;
		int32             delta;
	};

	struct TouchData
	{
		int32   pid;
		point2i pos;
	};

	struct SizeData
	{
		Graphics::Element element;
		point2u           size;
	};

	struct MoveData
	{
		Graphics::Element element;
		point2i           pos;
	};

	struct SizeMoveData
	{
		Graphics::Element element;
		Rect              rect;
	};

	struct PaintData
	{
		Graphics::Element element;
		Rect              region;
	};

	struct StateData
	{
		Graphics::Element element;
		bool              in;
	};

	struct VisibilityData
	{
		Graphics::Element element;
		bool              visible;
	};

	struct PropertyData
	{
		Graphics::Element element;
		u32               prop;
		int32             value;
	};

	union Data
	{
		u8              key;
		KeyData         keyCode;
		MouseButtonData mouseButton;
		MouseMoveData   mouseMove;
		ScrollData      scroll;
		TouchData       touch;
		int32           message;
		PaintData       paint;
		SizeData        size;
		MoveData        move;
		VisibilityData  visibility;
		StateData       state;
		PropertyData    property;
	};

	void operator () () noexcept;

	inline    Event () noexcept = default;
	constexpr Data const& data () const noexcept { return m_gData; }
	constexpr Event::Type type () const noexcept { return m_eType; }

	constexpr Event (Type eType) noexcept
	: m_gData (),
	  m_eType (eType)
	{ }

protected:
	Data m_gData;
	Type m_eType;
};

// =========================================================

struct EventSignals final : NonCopyable
{
	typedef Event<Ui::View> event_type;

	Signal<void(u8)>                                 keyPress;
	Signal<void(u8)>                                 keyRelease;
	Signal<void(event_type::KeyData)>                keyNotify;
	Signal<void(event_type::MouseButtonData const&)> mousePress;
	Signal<void(event_type::MouseButtonData const&)> mouseRelease;
	Signal<void(event_type::MouseMoveData const&)>   mouseMove;
	Signal<void(event_type::ScrollData const&)>      scroll;
	Signal<void(event_type::TouchData const&)>       touchPress;
	Signal<void(event_type::TouchData const&)>       touchRelease;
	Signal<void(event_type::TouchData const&)>       touchMove;
	Signal<void(int32)>                              sysMessage;
	Signal<void(event_type::PaintData const&)>       winPaint;
	Signal<void(event_type::MoveData const&)>        winMove;
	Signal<void(event_type::SizeData const&)>        winSize;
	Signal<void(event_type::StateData const&)>       winFocus;
	Signal<void(event_type::StateData const&)>       winStep;
	Signal<void(event_type::PropertyData const&)>    winProperty;
	Signal<void(event_type::VisibilityData const&)>  winVisible;

	EventSignals () noexcept;
};

// =========================================================

struct IDisplayQueue : public NonCopyableVirtual
{
	typedef BitSet<Event<Ui::View>::Type> mask_type;
	typedef Event<Ui::View>               event_type;
	typedef Queue<Ui::View>               Object;

	virtual bool pop_front (event_type&, bool wait) noexcept = 0;
	virtual bool isValid () const noexcept = 0;
	virtual bool setRenderableEvents (Ui::IRenderable&, mask_type) noexcept = 0;

	static IDisplayQueue* instance ();
	static bool           hasValidInstance () noexcept;
};

// =========================================================

template <>
class Queue <Ui::View> : public NonCopyable
{
public:
	typedef IDisplayQueue::event_type event_type;
	typedef IDisplayQueue::mask_type  mask_type;
	typedef CircularQueue<event_type> queue_type;
	typedef std::size_t               size_type;
	typedef atomic<mask_type>         atomic_mask;

	Queue () noexcept;
	Queue (mask_type accept_events) noexcept;
	static EventSignals& events () noexcept;

	bool pop_front (event_type& next_event, bool wait) noexcept;
	int  poll (bool wait = true) noexcept;

	inline mask_type accepted () const noexcept
	{ return m_gAcceptedEvents.load (); }

	inline void push_back (event_type const& gEvent) noexcept
	{ m_gEventQueue.push_back (gEvent); }

	inline void accept (mask_type gEvents) noexcept
	{ m_gAcceptedEvents = gEvents; }

	inline void quit () noexcept
	{ m_bPoll = false; }

	inline bool isPolling () const noexcept
	{ return m_bPoll.load (); }

private:
	queue_type  m_gEventQueue;
	atomic_mask m_gAcceptedEvents;
	atomic_bool m_bPoll;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_EVENT_H
