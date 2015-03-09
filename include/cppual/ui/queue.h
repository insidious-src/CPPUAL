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

#ifndef CPPUAL_UI_QUEUE_H
#define CPPUAL_UI_QUEUE_H
#ifdef __cplusplus

#include <thread>
#include <atomic>
#include <memory>
#include <cppual/flags.h>
#include <cppual/noncopyable.h>
#include <cppual/ui/events.h>
#include <cppual/ui/window.h>

using std::atomic_bool;
using std::shared_ptr;

namespace cppual { namespace Ui {

class   IDisplayQueue;
typedef shared_ptr<IDisplayQueue> shared_queue;

// =========================================================

class IDisplayQueue : public NonCopyableVirtual
{
public:
	typedef BitSet<Event::Type> mask_type;
	typedef IWindow             window_type;
	typedef Event               event_type;

	virtual bool set_window_events (window_type const&, mask_type) = 0;
	virtual bool pop_front         (event_type& receiver, bool wait) = 0;
	virtual int  poll              (window_type const&, atomic_bool& poll) = 0;
	virtual void send              (event_type const&) = 0;
	virtual void post              (event_type const&) = 0;

	static IDisplayQueue* instance ();
	static bool           hasValidInstance () noexcept;

	Connection display () const noexcept { return m_display; }
	bool       valid   () const noexcept { return m_display; }

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
	typedef View                      window_type;

	EventQueue () noexcept
	: queue    (IDisplayQueue::instance ()),
	  polling  ()
	{ }

	int poll (window_type const& window)
	{
		if (std::this_thread::get_id () != window.renderable_unsafe ()->thread_id ())
			throw std::logic_error ("the window was created on a different thread");

		polling = true;
		return queue->poll (*window.renderable_unsafe (), polling);
	}

	void send (event_type const& event) { queue->send (event); }
	void post (event_type const& event) { queue->post (event); }
	void quit () noexcept { polling = false; }

	bool pop_front (event_type& receiver, bool wait)
	{ return queue->pop_front (receiver, wait); }

	bool isPolling () const noexcept
	{ return polling.load (); }

private:
	IDisplayQueue* queue;
	atomic_bool    polling;
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_UI_QUEUE_H
