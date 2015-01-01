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

#include <iostream>
#include <cppual/ui/manager.h>

using std::string;
using cppual::Platform::IService;

namespace cppual { namespace Input {

namespace { namespace Internal { // optimize for internal unit usage

inline shared_queue& queue () noexcept
{
	static shared_queue platform_queue (nullptr);
	return platform_queue;
}

void attachQueue ()
{
	if (!IService::hasValidInstance ()) return;
	Internal::queue () = IService::instance ()->createQueueObject ();
}

} } // anonymous namespace Internal

// =========================================================

void IDisplayQueue::event_type::operator () () noexcept
{
	switch (type ())
	{
	case KeyPressed:
		IDisplayQueue::Object::events ().keyPress (data ().key);
		break;
	case KeyReleased:
		IDisplayQueue::Object::events ().keyRelease (data ().key);
		break;
	case KeyMap:
		IDisplayQueue::Object::events ().keyNotify (data ().keyCode);
		break;
	case ButtonDown:
		IDisplayQueue::Object::events ().mousePress (data ().mouseButton);
		break;
	case ButtonUp:
		IDisplayQueue::Object::events ().mouseRelease (data ().mouseButton);
		break;
	case PointerMove:
		IDisplayQueue::Object::events ().mouseMove (data ().mouseMove);
		break;
	case Scroll:
		IDisplayQueue::Object::events ().scroll (data ().scroll);
		break;
	case TouchPress:
		IDisplayQueue::Object::events ().touchPress (data ().touch);
		break;
	case TouchRelease:
		IDisplayQueue::Object::events ().touchRelease (data ().touch);
		break;
	case TouchMove:
		IDisplayQueue::Object::events ().touchMove (data ().touch);
		break;
	case SystemMessage:
		IDisplayQueue::Object::events ().sysMessage (data ().message);
		break;
	case Paint:
		IDisplayQueue::Object::events ().winPaint (data ().paint);
		break;
	case Size:
		IDisplayQueue::Object::events ().winSize (data ().size);
		break;
	case Move:
		IDisplayQueue::Object::events ().winMove (data ().move);
		break;
	case Focus:
		IDisplayQueue::Object::events ().winFocus (data ().state);
		break;
	case Step:
		IDisplayQueue::Object::events ().winStep (data ().state);
		break;
	case Visibility:
		IDisplayQueue::Object::events ().winVisible (data ().visibility);
		break;
	case Property:
		IDisplayQueue::Object::events ().winProperty (data ().property);
		break;
	case Null:
		std::cout << "null event\n";
		break;
	default:
		throw std::logic_error ("invalid event type");
	}
}

// =========================================================

IDisplayQueue* IDisplayQueue::instance ()
{
	if (Internal::queue () == nullptr) Internal::attachQueue ();
	return Internal::queue ().get ();
}

bool IDisplayQueue::hasValidInstance () noexcept
{
	return Internal::queue () != nullptr;
}

// =========================================================

EventSignals::EventSignals () noexcept
: keyPress (),
  keyRelease (),
  keyNotify (),
  mousePress (),
  mouseRelease (),
  mouseMove (),
  scroll (),
  touchPress (),
  touchRelease (),
  touchMove (),
  sysMessage (),
  winPaint (),
  winMove (),
  winSize (),
  winFocus (),
  winStep (),
  winProperty (),
  winVisible ()
{ }

// =========================================================

IDisplayQueue::Object::Queue () noexcept
: m_gEventQueue (25),
  m_gAcceptedEvents (event_type::AllEvents),
  m_bPoll ()
{ }

IDisplayQueue::Object::Queue (mask_type nEvents) noexcept
: m_gEventQueue (25),
  m_gAcceptedEvents (nEvents),
  m_bPoll ()
{ }

bool IDisplayQueue::Object::pop_front (event_type& gEvent, bool bWait) noexcept
{
	if (!m_gEventQueue.empty ())
	{
		gEvent = m_gEventQueue.front ();
		m_gEventQueue.pop_front ();
	}
	else if (!IDisplayQueue::instance () or !Internal::queue ()->pop_front (gEvent, bWait))
		return false;

	return gEvent.type () != event_type::Null;
}

int IDisplayQueue::Object::poll (bool bWait) noexcept
{
	event_type gEvent (event_type::Null);
	m_bPoll.store (bWait);

	do // event polling
	{
		if (!pop_front (gEvent, bWait) or
				!m_gAcceptedEvents.load (std::memory_order_relaxed).hasBit (gEvent.type ()))
			continue;

		gEvent ();

		if (gEvent.type () == event_type::SystemMessage and
				gEvent.data ().message == SystemMessage::Quit)
			return 0;
	}
	while (m_bPoll.load (std::memory_order_relaxed));

	return 0;
}

EventSignals& IDisplayQueue::Object::events () noexcept
{
	static EventSignals event_signals;
	return event_signals;
}

} } // namespace Input
