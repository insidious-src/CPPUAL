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

#ifndef CPPUAL_UI_VIEW_MANAGER_H_
#define CPPUAL_UI_VIEW_MANAGER_H_
#ifdef __cplusplus

#include <thread>
#include <chrono>
#include <memory>
#include <cppual/flags.h>
#include <cppual/resource.h>
#include <cppual/gfx/coord.h>
#include <cppual/gfx/dsp_details.h>
#include <cppual/ui/display.h>

using std::thread;
using std::string;
using std::shared_ptr;
using std::weak_ptr;
using cppual::Graphics::Element;
using cppual::Graphics::Connection;

namespace cppual { namespace Ui {

enum WindowFlag
{
	Frame       = 1 << 0,
	ThinFrame   = 1 << 0 | 1 << 1,
	Resizable   = 1 << 2,
	Close       = 1 << 4,
	Maximize    = 1 << 5,
	Help        = 1 << 6,
	Transparent = 1 << 7,
	WindowHints = Resizable | Maximize | Close,
	ToolHints   = ThinFrame | Close
};

class   IWindow;
typedef shared_ptr<IWindow> shared_window;
typedef weak_ptr<IWindow>   weak_window;
typedef BitSet<WindowFlag>  WindowFlags;

// ====================================================

class IWindow : public Resource < IDisplay*, Element >
{
public:
	using Resource<IDisplay*, Element>::Resource;

	typedef shared_window const& const_reference;
	typedef std::size_t          size_type;

	virtual weak_window parent () const = 0;
	virtual WindowFlags flags () const = 0;
	virtual string      title () const = 0;
	virtual void        setTitle (string const&) = 0;
	virtual void        setShaded (bool) = 0;
	virtual bool        isShaded () = 0;
	virtual void        setModal (bool) = 0;
	virtual bool        isModal () = 0;
	virtual void        setFullscreen (bool) = 0;
	virtual bool        isFullscreen () = 0;
	virtual void        setMaximized (bool) = 0;
	virtual bool        isMaximized () = 0;
	virtual void        setMinimized (bool) = 0;
	virtual bool        isMinimized () = 0;
	virtual void        setVisibleInTaskbar (bool) = 0;
	virtual bool        isVisibleInTaskbar () = 0;
	virtual void        setVisibleInPager (bool) = 0;
	virtual bool        isVisibleInPager () = 0;
	virtual void        setFlags (WindowFlags) = 0;
    virtual void        flash (uint count = 1) = 0;
	virtual Rect        geometry () const = 0;
	virtual u32         screen () const = 0;
	virtual bool        isMapped () const = 0;
	virtual void		setParent (const_reference, point2i pos) = 0;
	virtual void		setGeometry (Rect const&) = 0;
	virtual void		raise () = 0;
	virtual void        lower () = 0;
	virtual void        move (point2i) = 0;
	virtual void		map () = 0;
	virtual void		unmap () = 0;

	thread::id thread_id () const noexcept
	{ return m_thread; }

private:
	thread::id m_thread { std::this_thread::get_id () };
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIEW_MANAGER_H_
