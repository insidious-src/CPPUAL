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

#include <cppual/ui/wm.h>
#include <cppual/gfx/draw.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <wayland-client-protocol.h>

using cppual::Graphics::PixelMap;
using cppual::Graphics::PixelFormat;

namespace cppual { namespace Ui {

class WlSurface final : public IWindow
{
public:
	WlSurface () = delete;
	WlSurface (Rect const&, IWindow*, u32, PixelFormat const&) noexcept;
	~WlSurface () noexcept;

	bool isMapped () const noexcept;
	void setParent (const_reference, point2i) noexcept;
	void move (point2i) noexcept;
	void setGeometry (Rect const&) noexcept;
	void raise () noexcept;
	void lower () noexcept;
	void map () noexcept;
	void unmap () noexcept;

	IDisplay*    display () const noexcept { return m_pDisplay; }
	Element   handle () const noexcept { return m_pViewHandle; }
	PixelFormat  format () const noexcept { return m_gFormat; }
	PixelMap*    pixelMap () noexcept { return nullptr; }
	weak_window parent () const noexcept { return m_pParent; }
	Rect         geometry () const noexcept { return m_gRect; }
	u32          screen () const noexcept { return 0; }

	string title () const { return string (); }
	void setTitle (string const&) { }
	void setShaded (bool) { }
	bool isShaded () { return false; }
	void setModal (bool) { }
	bool isModal () { return false; }
	void setFullscreen (bool) { }
	bool isFullscreen () { return false; }
	void setMaximized (bool) { }
	bool isMaximized () { return false; }
	void setMinimized (bool) { }
	bool isMinimized () { return false; }
	void setVisibleInTaskbar (bool) { }
	bool isVisibleInTaskbar () { return false; }
	void setVisibleInPager (bool) { }
	bool isVisibleInPager () { return false; }
	void setWMFrame (bool) { }
	void setMimimumSize (point2u) { }
	void setMaximumSize (point2u) { }
	void flash () { }
	Rect workArea () { return Rect (); }

private:
	PixelFormat       m_gFormat;
	shared_window m_pParent;
	Rect              m_gRect;
	IDisplay*      m_pDisplay;
	wl_surface*       m_pViewHandle;
	u32               m_uScreen;
};

// ====================================================

WlSurface::WlSurface (Rect const&        gRect,
					  IWindow*       pParent,
					  u32                nScreen,
					  PixelFormat const& gFormat) noexcept
: m_gFormat (gFormat),
  m_pParent (pParent),
  m_gRect (gRect),
  m_pDisplay (),
  m_pViewHandle (),
  m_uScreen (nScreen)
{
}

WlSurface::~WlSurface () noexcept
{
}


void WlSurface::setGeometry (Rect const&) noexcept
{
}

void WlSurface::raise () noexcept
{
}

void WlSurface::lower () noexcept
{
}

void WlSurface::map () noexcept
{
}

void WlSurface::unmap () noexcept
{
}

void WlSurface::setParent (const_reference, point2i) noexcept
{
}

void WlSurface::move (point2i) noexcept
{
}

} } // namespace Ui

#endif // OS_GNU_LINUX
