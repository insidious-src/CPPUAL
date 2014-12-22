/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#include <cppual/ui/window.h>

namespace cppual { namespace Ui {

Window::Window ()
{
}

Window::Window (Window&& gObj)
: View (gObj)
{
}

Window::Window (Window const& gObj)
: View (gObj)
{
}

Window& Window::operator = (Window&& gObj)
{
	if (this == &gObj) return *this;
	return *this;
}

Window& Window::operator = (Window const& gObj)
{
	if (this == &gObj) return *this;
	return *this;
}

Window::~Window ()
{
	if (isValid ())
	{
		onClose ();
		m_gFrame.destroy ();
	}
}

Window::Window (View*       pParent,
				Rect const& gRect,
				string&&    gTitle,
				image_type* pIcon,
				u32         nScreen)
: View (&m_gFrame, gRect, nScreen),
  m_gFrame (pParent, gRect, std::forward<string> (gTitle), pIcon)
{
	if (isValid () and m_gFrame.isValid ())
	{
		m_gFrame.attach (this);
		m_pIcon = m_gFrame.icon ();
	}
}

bool Window::setIcon (image_type* pIcon)
{
	if (isValid ())
	{
		m_pIcon = pIcon;
		m_gFrame.setIcon (pIcon);
		refresh ();
	}

	return false;
}

void Window::setTitle (string const& gTitle)
{
	if (isValid ())
	{
		if (m_gFrame.isValid ()) m_gFrame.setLabel (gTitle);
		renderable ().lock ().get ()->setTitle (gTitle);
	}
}

void Window::restore ()
{
	if (m_gFrame.attached () == this)
	{
		if (m_gFrame.isHidden ()) m_gFrame.show ();
		else if (m_gFrame.isStretched ()) m_gFrame.unstretch ();
	}
	else if (isHidden ()) show ();
}

void Window::minimize ()
{
	if (!isMinimized ())
	{
		m_gFrame.hide ();
		onMinimize ();
	}
}

void Window::maximize ()
{
	if (!isMaximized ())
	{
		m_gFrame.stretch ();
		onMaximize ();
	}
}

void Window::close ()
{
	if (isValid () and onClose ())
	{
		m_gFrame.destroy ();
		destroy ();
	}
}

void Window::goFullscreen ()
{
	if (isValid () and !m_bIsFullScreen)
	{
		m_gFrame.detach ();
		renderable ().lock ().get ()->setFullscreen (true);
		m_bIsFullScreen = true;
	}
}

void Window::exitFullscreen ()
{
	if (m_bIsFullScreen)
	{
		m_gFrame.attach (this);
		renderable ().lock ().get ()->setFullscreen (false);
		m_bIsFullScreen = false;
	}
}

void Window::flash (ushort)
{
	if (isValid ()) renderable ().lock ().get ()->flash ();
}

void Window::showInTaskbar (bool bShow)
{
	if (isValid ()) renderable ().lock ().get ()->setVisibleInTaskbar (bShow);
}

} } // namespace Ui
