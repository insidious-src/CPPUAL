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

#include <unordered_map>
#include <cppual/ui/window.h>

namespace cppual { namespace Ui {

namespace { namespace Internal {

typedef std::unordered_map<uptr, WindowAdapter*> map_type;

inline map_type& map ()
{
	static map_type views_map (5);
	return views_map;
}

} } // anonymous namespace Internal

// =========================================================

void WindowAdapter::registerEvents ()
{
	connect (event_type::registers ().mouseMove,
			 [](event_type::window_type wnd, event_type::MouseMoveData data)
	{
		Internal::map ()[wnd]->onPointerMove (data);
	});

	connect (event_type::registers ().mousePress,
			 [](event_type::window_type wnd, event_type::MouseButtonData data)
	{
		Internal::map ()[wnd]->onMousePress (data);
	});

	connect (event_type::registers ().mouseRelease,
			 [](event_type::window_type wnd, event_type::MouseButtonData data)
	{
		Internal::map ()[wnd]->onMouseRelease (data);
	});

	connect (event_type::registers ().winPaint,
			 [](event_type::window_type wnd, event_type::PaintData data)
	{
		Internal::map ()[wnd]->onPaint (data);
	});

	connect (event_type::registers ().winFocus,
			 [](event_type::window_type wnd, event_type::StateData data)
	{
		Internal::map ()[wnd]->onFocus (data.in);
	});

	connect (event_type::registers ().winSize,
			 [](event_type::window_type wnd, event_type::SizeData data)
	{
		Internal::map ()[wnd]->onSize (data);
	});

	connect (event_type::registers ().winVisible,
			 [](event_type::window_type wnd, event_type::VisibilityData data)
	{
		Internal::map ()[wnd]->onShow (data.visible);
	});
}

// =========================================================

WindowAdapter::WindowAdapter (Widget&     widget,
							  WindowFlags flags,
							  Icon const& icon,
							  u32         screen)
: m_pPlatformWnd (Platform::Factory::instance ()->createWindow (widget.geometry (), screen)),
  m_pMainWidget  (&widget),
  m_pIcon        (icon)
{
	if (!m_pPlatformWnd) throw std::bad_alloc ();

	uptr key = m_pPlatformWnd->id ();

	if (!Internal::map ().emplace (std::make_pair (key, this)).second)
	{
		m_pPlatformWnd.reset ();
		throw std::logic_error ("failed to register WindowAdapter events");
	}

	IDisplayQueue::instance ()->
			setWindowEvents (*m_pPlatformWnd,
							 event_type::Key     |
							 event_type::Pointer |
							 event_type::Window);

	m_pPlatformWnd->setFlags (flags);
	m_pPlatformWnd->setTitle (widget.name ());
}

bool WindowAdapter::setIcon (Icon const& gIcon)
{
	m_pIcon = gIcon;
	return false;
}

void WindowAdapter::restore ()
{
	if (m_pPlatformWnd)
	{
		if (m_pPlatformWnd->isMaximized ()) m_pPlatformWnd->setMaximized (false);
		if (m_pPlatformWnd->isMinimized ()) m_pPlatformWnd->setMinimized (false);
	}
}

void WindowAdapter::onSize (event_type::SizeData gData)
{
	m_pMainWidget->setSize (gData.size);
}

WindowAdapter::WindowAdapter (WindowAdapter&& gObj)
: m_pPlatformWnd (gObj.m_pPlatformWnd),
  m_pMainWidget  (gObj.m_pMainWidget),
  m_pIcon        (gObj.m_pIcon)
{
	gObj.m_pPlatformWnd = nullptr;
	gObj.m_pMainWidget  = nullptr;
	gObj.m_pIcon        = Icon ();
}

WindowAdapter& WindowAdapter::operator = (WindowAdapter&& gObj)
{
	if (this != &gObj)
	{
		m_pPlatformWnd = gObj.m_pPlatformWnd;
		m_pMainWidget  = gObj.m_pMainWidget;
		m_pIcon        = gObj.m_pIcon;

		gObj.m_pPlatformWnd = nullptr;
		gObj.m_pMainWidget  = nullptr;
		gObj.m_pIcon        = Icon ();
	}

	return *this;
}

} } // namespace Ui
