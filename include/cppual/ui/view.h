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

#ifndef CPPUAL_UI_VIEW_H_
#define CPPUAL_UI_VIEW_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>
#include <cppual/circular_queue.h>
#include <cppual/ui/vsurface.h>
#include <cppual/ui/default_events.h>

namespace cppual { namespace Ui {

class View
{
public:
	typedef Event                                event_type;
	typedef EventQueue                           queue_type;
	typedef Memory::GenericPolicy<View*>         allocator_type;
	typedef CircularQueue<View*, allocator_type> container;
	typedef std::size_t	                         size_type;

	typedef typename
	container::iterator iterator;

	typedef typename
	Signal<void(point2i)>
	::slot_type mouse_move_conn;

	typedef typename
	Signal<void(event_type::MouseButtonData const&)>
	::slot_type mouse_btn_conn;

	enum
	{
		DefaultWidth  = 1024,
		DefaultHeight = 768
	};

	View (View&&) noexcept;
	View (View const&) noexcept;
	View& operator = (View&&) noexcept;
	View& operator = (View const&) noexcept;
	~View ();

	View (View*       parent,
		  Rect const& rect,
		  u32         screen    = 0,
		  allocator_type const& = allocator_type ());

	void destroy ();
	void show ();
	void hide ();
	bool setParent (View* parent, point2i pos = point2i ());
	void setGeometry (Rect const&);
	void setMinimumSize (point2u);
	void setMaximumSize (point2u);
	void move (point2i);
	void destroyChildren ();
	void refresh ();
	void enable ();
	void disable ();
	void setFocus ();
	void killFocus ();

	inline weak_renderable renderable () const noexcept { return m_pRenderable; }
	inline IRenderable*    renderable_unsafe () const noexcept { return m_pRenderable.get (); }
	inline point2u         minimumSize () const noexcept { return m_gMinSize; }
	inline point2u         maximumSize () const noexcept { return m_gMaxSize; }

	inline bool isValid () const noexcept
	{ return m_gStateFlags.hasBit (View::Valid); }

	inline bool isEnabled () const noexcept
	{ return m_gStateFlags.hasBit (View::Enabled); }

	inline bool hasFocus () const noexcept
	{ return m_gStateFlags.hasBit (View::HasFocus); }

	inline bool isHidden () const noexcept
	{ return !m_gStateFlags.hasBit (View::Valid) or !m_pRenderable->isMapped (); }

	inline Rect geometry () const noexcept
	{ return m_gStateFlags.hasBit (View::Valid) ? m_pRenderable->geometry () : Rect (); }

	inline View () noexcept
	: View (nullptr, Rect (0, 0, DefaultWidth, DefaultHeight))
	{ }

protected:
	virtual void onDestroy () { }
	virtual void onShow (bool) { }
	virtual void onPaint (Rect const&) { }
	virtual void onEnable (bool) { }
	virtual void onSize (point2u) { }
	virtual void onMove (point2i) { }
	virtual void onBeginSizeMove (Rect const&) { }
	virtual void onEndSizeMove (Rect const&) { }
	virtual void onMinMaxSize (point2u) { }
	virtual void onFocus (bool) { }
	virtual void onParentSize (point2u) { }

private:
	enum StateFlag
	{
		Valid    = 1 << 0,
		HasFocus = 1 << 1,
		Enabled  = 1 << 2
	};

	typedef BitSet<StateFlag> StateFlags;

	void paint (Rect const&);
	void size (point2u);
	void mouseMoved (point2i);
	void mousePressed (event_type::MouseButtonData const&);
	void mouseReleased (event_type::MouseButtonData const&);
	void destroyResources ();
	void invalidate () noexcept;
	static void registerEvents ();

private:
	container         m_gChildrenList;
	point2u           m_gMinSize, m_gMaxSize;
	shared_renderable m_pRenderable;
	iterator          m_gItFromParent;
	View*             m_pParentObj;
	StateFlags        m_gStateFlags;
};

// =========================================================

class Widget
{
public:
	typedef Memory::GenericPolicy<Widget*>         allocator_type;
	typedef CircularQueue<Widget*, allocator_type> container_type;
	typedef std::size_t	                           size_type;

	void destroy ();
	void show ();
	void hide ();
	bool setParent (View* parent, point2i pos = point2i ());
	void setGeometry (Rect const&);
	void setMinimumSize (point2u);
	void setMaximumSize (point2u);
	void move (point2u);
	void refresh ();
	void enable ();
	void disable ();
	void setFocus ();
	void killFocus ();

	Widget (Widget*     parent,
			Rect const& rect,
			allocator_type const& = allocator_type ());

	point2u minimumSize () const noexcept { return m_gMinSize; }
	point2u maximumSize () const noexcept { return m_gMaxSize; }

	bool isEnabled () const noexcept
	{ return m_gStateFlags.hasBit (Widget::Enabled); }

	bool hasFocus () const noexcept
	{ return m_gStateFlags.hasBit (Widget::HasFocus); }

	bool isHidden () const noexcept
	{ return !m_gBuffer.isMapped (); }

	Rect geometry () const noexcept
	{ return m_gBuffer.geometry (); }

protected:
	virtual void onDestroy () { }
	virtual void onShow (bool) { }
	virtual void onPaint (Rect const&) { }
	virtual void onEnable (bool) { }
	virtual void onSize (point2u) { }
	virtual void onMove (point2u) { }
	virtual void onBeginSizeMove (Rect const&) { }
	virtual void onEndSizeMove (Rect const&) { }
	virtual void onMinMaxSize (point2u) { }
	virtual void onFocus (bool) { }
	virtual void onParentSize (point2u) { }

private:
	enum StateFlag
	{
		HasFocus = 1 << 1,
		Enabled  = 1 << 2
	};

	typedef BitSet<StateFlag> StateFlags;

	void paint (Rect const&);

private:
	VirtualBuffer  m_gBuffer;
	container_type m_gChildren;
	Widget*        m_pParent;
	point2u        m_gMinSize, m_gMaxSize;
	StateFlags     m_gStateFlags;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIEW_H_
