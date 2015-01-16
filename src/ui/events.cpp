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
using cppual::Platform::Factory;

namespace cppual { namespace Ui {

namespace { namespace Internal { // optimize for internal unit usage

inline shared_queue& queue () noexcept
{
	static shared_queue platform_queue;
	return platform_queue;
}

void attachQueue ()
{
	if (!Factory::hasValidInstance ()) return;
	Internal::queue () = Factory::instance ()->createQueueObject ();
}

} } // anonymous namespace Internal

// =========================================================

void Event::operator () () noexcept
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
		registers ().winVisible (window (), data ().visibility);
		break;
	case Property:
		registers ().winProperty (window (), data ().property);
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

EventQueue::EventQueue () noexcept
: m_gEventQueue (25),
  m_gAcceptedEvents (event_type::AllEvents),
  m_bPoll ()
{ }

EventQueue::EventQueue (mask_type nEvents) noexcept
: m_gEventQueue (25),
  m_gAcceptedEvents (nEvents),
  m_bPoll ()
{ }

bool EventQueue::pop_front (event_type& gEvent, bool bWait) noexcept
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

int EventQueue::poll (bool bWait) noexcept
{
	event_type gEvent (event_type::Null);

	m_bPoll = bWait;

	do // event polling
	{
		if (!pop_front (gEvent, bWait) or
				!m_gAcceptedEvents.load (std::memory_order_relaxed).test (gEvent.type ()))
			continue;

		// signal event
		gEvent ();
	}
	while (m_bPoll.load (std::memory_order_relaxed));

	return 0;
}

} } // namespace Input
