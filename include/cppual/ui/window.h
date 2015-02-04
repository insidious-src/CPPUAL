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

#ifndef CPPUAL_UI_WINDOW_H_
#define CPPUAL_UI_WINDOW_H_
#ifdef __cplusplus

#include <chrono>
#include <cppual/gfx/drawable/image.h>
#include <cppual/ui/frame.h>

namespace cppual { namespace Ui {

class Window : public View
{
public:
	typedef Graphics::Image image_type;

	Window ();
	Window (Window&&);
	Window (Window const&);
	Window& operator = (Window&&);
	Window& operator = (Window const&);
	~Window ();

	bool setIcon (image_type*);
	void setTitle (string const&);
	void goFullscreen ();
	void exitFullscreen ();
	void flash (ushort);
	void showInTaskbar (bool);
	void restore ();
	void minimize ();
	void maximize ();
	void close ();

    Window (View*         parent,
            Rect   const& rect,
            string const& title,
            image_type*   icon   = nullptr,
            u32           screen = 0);

    inline FrameView*  frame () noexcept { return m_gFrame; }
	inline image_type* icon () const noexcept { return m_pIcon; }
	inline bool        isFullscreen () const noexcept { return m_bIsFullScreen; }

	inline bool isMinimized () const noexcept
    { return m_gFrame->attached () == this and m_gFrame->isHidden (); }

	inline bool isMaximized () const noexcept
    { return m_gFrame->attached () == this and m_gFrame->isStretched (); }

protected:
	virtual bool onClose () { return true; }
	virtual void onMinimize () { }
	virtual void onMaximize () { }
	virtual void onHelp () { }

private:
    FrameView*  m_gFrame;
	image_type* m_pIcon;
	bool        m_bIsFullScreen;
};

// =========================================================

class WindowAdapter : public NonCopyableVirtual
{
public:
	typedef Event                                  event_type;
	typedef EventQueue                             queue_type;
	typedef Memory::GenericPolicy<Widget*>         allocator_type;
	typedef CircularQueue<Widget*, allocator_type> container;
	typedef std::size_t	                           size_type;

	typedef typename
	container::iterator iterator;

	typedef typename
	Signal<void(event_type::window_type, event_type::MouseMoveData)>
	::slot_type
	move_slot;

	typedef typename
	Signal<void(event_type::window_type, event_type::MouseButtonData)>
	::slot_type
	btn_slot;


	enum
	{
		DefaultWidth  = 1024,
		DefaultHeight = 768
	};

	WindowAdapter (WindowAdapter&&);
	WindowAdapter& operator = (WindowAdapter&&);

	WindowAdapter (Widget&     main_widget,
				   WindowFlags flags  = WindowHints,
				   Icon const& icon   = Icon (),
				   u32         screen = 0);

	bool setIcon (Icon const&);
	void restore ();

	void close ()
	{ if (m_pPlatformWnd) m_pPlatformWnd.reset (); }

	void flash (std::chrono::seconds sec)
	{ if (m_pPlatformWnd) m_pPlatformWnd->flash (sec); }

	void goFullscreen ()
	{ if (m_pPlatformWnd) m_pPlatformWnd->setFullscreen (true); }

	void exitFullscreen ()
	{ if (m_pPlatformWnd) m_pPlatformWnd->setFullscreen (false); }

	void showInTaskbar (bool state)
	{ if (m_pPlatformWnd) m_pPlatformWnd->setVisibleInTaskbar (state); }

	void showInPager (bool state)
	{ if (m_pPlatformWnd) m_pPlatformWnd->setVisibleInPager (state); }

	void minimize ()
	{ if (m_pPlatformWnd) m_pPlatformWnd->setMaximized (true); }

	void maximize ()
	{ if (m_pPlatformWnd) m_pPlatformWnd->setMaximized (true); }


private:
	virtual bool onClose        () { return true; }
	virtual void onMinimize     () { }
	virtual void onMaximize     () { }
	virtual void onHelp         () { }
	virtual void onPointerMove  (event_type::MouseMoveData) { }
	virtual void onMousePress   (event_type::MouseButtonData) { }
	virtual void onMouseRelease (event_type::MouseButtonData) { }
	virtual void onKeyPress     (event_type::KeyData) { }
	virtual void onKeyRelease   (event_type::KeyData) { }

	virtual void onSize         (event_type::SizeData);
	virtual void onPaint        (event_type::PaintData) { }
	virtual void onFocus        (bool) { }
	virtual void onShow         (bool) { }

	static void registerEvents ();

private:
	shared_window m_pPlatformWnd;
	Widget*       m_pMainWidget;
	Icon          m_pIcon;
};

class Wnd : public WindowAdapter
{
public:
private:
};

} } // namespace Ui
#endif // __cplusplus
#endif // CPPUAL_UI_WINDOW_H_
