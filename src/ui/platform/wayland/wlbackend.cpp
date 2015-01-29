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

#define API_EXPORT
#include <cppual/ui/display.h>
#include <EGL/egl.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <wayland-client-protocol.h>

namespace cppual { namespace Ui {

class WlBackend final : public IDisplay
{
public:
	WlBackend () = delete;
	WlBackend (cchar*) noexcept;
	~WlBackend () noexcept;
	void flush () noexcept;

	Connection native () const noexcept { return m_pDisplay; }
	string     name () const { return string (); }
	Connection gl () const noexcept { return m_pEGLDisplay; }
	int        screenCount () const noexcept { return m_nScreenCount; }

private:
	wl_display* m_pDisplay;
	void*       m_pEGLDisplay;
	int         m_nScreenCount;
};

// ====================================================

WlBackend::WlBackend (cchar* pName) noexcept
: m_pDisplay (wl_display_connect (pName)),
  m_pEGLDisplay (),
  m_nScreenCount ()
{ }

WlBackend::~WlBackend () noexcept
{
	if (m_pDisplay) wl_display_disconnect (m_pDisplay);
}

void WlBackend::flush () noexcept
{
	if (m_pDisplay) wl_display_flush (m_pDisplay);
}

} } // namespace Graphics

#endif // OS_GNU_LINUX or OS_BSD
