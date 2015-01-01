/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_X_SURFACE_H_
#define CPPUAL_PLATFORM_X_SURFACE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>
#include <cppual/ui/wm.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

namespace cppual { namespace Ui {

class XRenderable final : public IRenderable
{
public:
	XRenderable () = delete;
	XRenderable (Rect const& rect, u32 screen, IDisplay* display) noexcept;

	string title () const noexcept;
	void   setTitle (string const&) noexcept;
	void   setShaded (bool) noexcept;
	bool   isShaded () noexcept;
	void   setModal (bool) noexcept;
	bool   isModal () noexcept;
	void   setFullscreen (bool) noexcept;
	bool   isFullscreen () noexcept;
	void   setMaximized (bool) noexcept;
	bool   isMaximized () noexcept;
	void   setMinimized (bool) noexcept;
	bool   isMinimized () noexcept;
	void   setVisibleInTaskbar (bool) noexcept;
	bool   isVisibleInTaskbar () noexcept;
	void   setVisibleInPager (bool) noexcept;
	bool   isVisibleInPager () noexcept;
	void   setWMFrame (bool) noexcept;
	void   setMimimumSize (point2u) noexcept;
	void   setMaximumSize (point2u) noexcept;
	void   flash () noexcept;
	Rect   workArea () noexcept;
	Rect   geometry () const;
	bool   isMapped () const;
	//void   setParent (const_reference, point2i);
	void   setGeometry (Rect const&);
	void   setMinSize ();
	void   setMaxSize ();
	void   raise ();
	void   lower ();
	void   move (point2i);
	void   map ();
	void   unmap ();

	weak_renderable parent () const noexcept { return shared_renderable (); }
	u32             screen () const noexcept { return m_uScreen; }
	void            setParent (const_reference, point2i) { }

private:
	u32 m_uScreen;
};

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_X_SURFACE_H_
