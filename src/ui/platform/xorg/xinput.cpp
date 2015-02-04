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
#include <cppual/ui/events.h>
#include <cppual/input/pointer.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xinput.h"

using std::string;
using namespace cppual::Input;

namespace cppual { namespace Ui {

namespace {

typedef xcb_button_t     xcb_button;
typedef xcb_keycode_t    xcb_keycode;
typedef xcb_keysym_t     xcb_keysym;
typedef xcb_connection_t display_type;
typedef xcb_window_t     handle_type;

// ====================================================

inline u8 button (xcb_button uId) noexcept
{
	switch (uId)
	{
	case 1:
		return Pointer::Left;
	case 2:
		return Pointer::Middle;
	case 3:
		return Pointer::Right;
	case 6:
		return Pointer::XButton1;
	case 7:
		return Pointer::XButton2;
	case 8:
		return Pointer::XButton3;
	case 9:
		return Pointer::XButton4;
	default:
		return uId;
	}
}

//u8 translateKeycode (xcb_keysym) noexcept
//{
//	return 0;
//}

// ====================================================

class XcbEvent final
{
public:
	typedef Event                       event_type;
	typedef Event::window_type          window_type;
	typedef xcb_generic_event_t         value_type;
	typedef xcb_generic_event_t*        pointer;
	typedef xcb_button_press_event_t    btn_press_type;
	typedef xcb_button_release_event_t  btn_release_type;
	typedef xcb_motion_notify_event_t   mouse_move_type;
	typedef xcb_key_press_event_t	    key_press_type;
	typedef xcb_key_release_event_t	    key_release_type;
	typedef xcb_keymap_notify_event_t   keymap_notify_type;
	typedef xcb_expose_event_t          expose_type;
	typedef xcb_focus_in_event_t        focus_in_type;
	typedef xcb_focus_out_event_t       focus_out_type;
	typedef xcb_enter_notify_event_t    enter_type;
	typedef xcb_leave_notify_event_t    leave_type;
	typedef xcb_resize_request_event_t  resize_type;
	typedef xcb_map_notify_event_t      map_type;
	typedef xcb_unmap_notify_event_t    unmap_type;
	typedef xcb_property_notify_event_t property_type;

	enum
	{
		MousePress	     = XCB_BUTTON_PRESS,
		MouseRelease     = XCB_BUTTON_RELEASE,
		MouseMove	     = XCB_MOTION_NOTIFY,
		KeyPress		 = XCB_KEY_PRESS,
		KeyRelease	     = XCB_KEY_RELEASE,
		KeyMapNotify     = XCB_KEYMAP_NOTIFY,
		ChangeKbCtrl     = XCB_CHANGE_KEYBOARD_CONTROL,
		GetKbCtrl        = XCB_GET_KEYBOARD_CONTROL,
		Expose           = XCB_EXPOSE,
		Enter            = XCB_ENTER_NOTIFY,
		Leave            = XCB_LEAVE_NOTIFY,
		FocusIn          = XCB_FOCUS_IN,
		FocusOut         = XCB_FOCUS_OUT,
		Size             = XCB_RESIZE_REQUEST,
		Visiblility      = XCB_VISIBILITY_NOTIFY,
		Create           = XCB_CREATE_NOTIFY,
		NoExposure       = XCB_NO_EXPOSURE,
		GraphicsExposure = XCB_GRAPHICS_EXPOSURE,
		Destroy          = XCB_DESTROY_NOTIFY,
		Unmap            = XCB_UNMAP_NOTIFY,
		Map              = XCB_MAP_NOTIFY,
		MapRequest       = XCB_MAP_REQUEST,
		ChangeParent     = XCB_REPARENT_NOTIFY,
		Configure        = XCB_CONFIGURE_NOTIFY,
		ConfigRequest    = XCB_CONFIGURE_REQUEST,
		Gravity          = XCB_GRAVITY_NOTIFY,
		Circulate        = XCB_CIRCULATE_NOTIFY,
		CirculateRequest = XCB_CIRCULATE_REQUEST,
		Property         = XCB_PROPERTY_NOTIFY,
		SelectionClear   = XCB_SELECTION_CLEAR,
		SelectionRequest = XCB_SELECTION_REQUEST,
		Selection        = XCB_SELECTION_NOTIFY,
		Colormap         = XCB_COLORMAP_NOTIFY,
		ClientMessage    = XCB_CLIENT_MESSAGE,
		Mapping          = XCB_MAPPING_NOTIFY,
		GeGeneric        = XCB_GE_GENERIC,
	};

	enum
	{
		MouseWheelUp   = 4,
		MouseWheelDown = 5
	};

	operator pointer () const noexcept
	{ return m_handle; }

	pointer operator -> () const noexcept
	{ return m_handle; }

	template <typename T>
	T* get () const noexcept
	{ return reinterpret_cast<T*> (m_handle); }

	uint type () const noexcept
	{ return m_handle->response_type & ~0x80; }

	~XcbEvent () noexcept
	{ ::free (m_handle); }

	XcbEvent ()               noexcept = default;
	XcbEvent (pointer pDpy)   noexcept : m_handle (pDpy) { }
	XcbEvent (std::nullptr_t) noexcept : m_handle ()     { }

	window_type window () const noexcept
	{
		switch (type ())
		{
		case XcbEvent::MousePress:
			return get<XcbEvent::btn_press_type> ()->event;
		case XcbEvent::MouseRelease:
			return get<XcbEvent::btn_release_type> ()->event;
		case XcbEvent::MouseMove:
			return get<XcbEvent::mouse_move_type> ()->event;
		case XcbEvent::KeyPress:
			return get<XcbEvent::key_press_type> ()->event;
		case XcbEvent::KeyRelease:
			return get<XcbEvent::key_release_type> ()->event;
		case XcbEvent::Expose:
			return get<XcbEvent::expose_type> ()->window;
		case XcbEvent::Enter:
			return get<XcbEvent::enter_type> ()->event;
		case XcbEvent::Leave:
			return get<XcbEvent::leave_type> ()->event;
		case XcbEvent::FocusIn:
			return get<XcbEvent::focus_in_type> ()->event;
		case XcbEvent::FocusOut:
			return get<XcbEvent::focus_out_type> ()->event;
		case XcbEvent::Size:
			return get<XcbEvent::resize_type> ()->window;
		case XcbEvent::Property:
			return get<XcbEvent::property_type> ()->window;
		case XcbEvent::Map:
			return get<XcbEvent::map_type> ()->window;
		case XcbEvent::Unmap:
			return get<XcbEvent::unmap_type> ()->window;
		default:
			return m_handle->pad;
		}
	}

	static xcb_generic_event_t toNativeEvent (event_type const&)
	{
		return xcb_generic_event_t ();
	}

	event_type toEvent () const noexcept
	{
		switch (type ())
		{
		case XcbEvent::MousePress:
			switch (get<XcbEvent::btn_press_type> ()->detail)
			{
			case XcbEvent::MouseWheelUp:
				return Ui::ScrollEvent (get<XcbEvent::btn_press_type> ()->event,
										1, { get<XcbEvent::btn_press_type> ()->event_x,
											 get<XcbEvent::btn_press_type> ()->event_y });
			case XcbEvent::MouseWheelDown:
				return Ui::ScrollEvent (get<XcbEvent::btn_press_type> ()->event,
										-1, { get<XcbEvent::btn_press_type> ()->event_x,
											  get<XcbEvent::btn_press_type> ()->event_y });
			default:
				return Ui::MousePressEvent (get<XcbEvent::btn_press_type> ()->event,
											button (get<XcbEvent::btn_press_type> ()->detail),
				{ get<XcbEvent::btn_press_type> ()->event_x,
				  get<XcbEvent::btn_press_type> ()->event_y });
			}
			break;
		case XcbEvent::MouseRelease:
			if (get<XcbEvent::btn_release_type> ()->detail == XcbEvent::MouseWheelUp or
				get<XcbEvent::btn_release_type> ()->detail == XcbEvent::MouseWheelDown)

			return Ui::MouseReleaseEvent (get<XcbEvent::btn_release_type> ()->event,
										  button (get<XcbEvent::btn_release_type> ()->detail),
			{ get<XcbEvent::btn_release_type> ()->event_x,
			  get<XcbEvent::btn_release_type> ()->event_y });
		case XcbEvent::MouseMove:
			return Ui::MouseMoveEvent (get<XcbEvent::mouse_move_type> ()->event,
			{ get<XcbEvent::mouse_move_type> ()->event_x,
			  get<XcbEvent::mouse_move_type> ()->event_y });
		case XcbEvent::KeyPress:
			return Ui::KeyPressEvent (get<XcbEvent::key_press_type> ()->event,
			{ get<XcbEvent::key_press_type> ()->detail });
		case XcbEvent::KeyRelease:
			return Ui::KeyReleaseEvent (get<XcbEvent::key_release_type> ()->event,
			{ get<XcbEvent::key_release_type> ()->detail });
		case XcbEvent::Expose:
			return Ui::PaintEvent (get<XcbEvent::expose_type> ()->window,
									 Rect (static_cast<int16> (get<XcbEvent::expose_type> ()->x),
										   static_cast<int16> (get<XcbEvent::expose_type> ()->y),
										   get<XcbEvent::expose_type> ()->width,
										   get<XcbEvent::expose_type> ()->height));
			break;
		case XcbEvent::Enter:
			return Ui::StepEvent (get<XcbEvent::enter_type> ()->event, true);
		case XcbEvent::Leave:
			return Ui::StepEvent (get<XcbEvent::leave_type> ()->event, false);
		case XcbEvent::FocusIn:
			return Ui::FocusEvent (get<XcbEvent::focus_in_type> ()->event, true);
		case XcbEvent::FocusOut:
			return Ui::FocusEvent (get<XcbEvent::focus_out_type> ()->event, false);
		case XcbEvent::Size:
			return Ui::SizeEvent (get<XcbEvent::resize_type> ()->window,
			{ get<XcbEvent::resize_type> ()->width,
			  get<XcbEvent::resize_type> ()->height });
		case XcbEvent::Property:
			return Ui::PropertyEvent (get<XcbEvent::property_type> ()->window,
									  get<XcbEvent::property_type> ()->atom,
									  get<XcbEvent::property_type> ()->state);
		case XcbEvent::Map:
			return Ui::VisibilityEvent (get<XcbEvent::map_type> ()->window, true);
		case XcbEvent::Unmap:
			return Ui::VisibilityEvent (get<XcbEvent::unmap_type> ()->window, false);
		case XcbEvent::Destroy:
			return event_type (event_type::Null);
		case XcbEvent::GraphicsExposure:
			return event_type (event_type::Null);
		case XcbEvent::NoExposure:
			return event_type (event_type::Null);
		case XcbEvent::ChangeParent:
			return event_type (event_type::Null);
		case XcbEvent::Mapping:
			return event_type (event_type::Null);
		case XcbEvent::Configure:
			return event_type (event_type::Null);
		case XcbEvent::GetKbCtrl:
			return event_type (event_type::Null);
		case XcbEvent::ChangeKbCtrl:
			return event_type (event_type::Null);
		default:
			return event_type (event_type::Null);
		}
	}

private:
	pointer m_handle;
};

} // anonymous namespace

// ====================================================

XQueue::XQueue () noexcept
: IDisplayQueue (IDisplay::hasValidInstance () ? IDisplay::instance ()->native () : nullptr)
{ }

bool XQueue::set_window_events (IWindow const& pRenderable, mask_type gFlags) noexcept
{
	if (pRenderable.connection ()->native () != display ())
		return false;

	u32 uMask = XCB_EVENT_MASK_NO_EVENT;

//	XCB_EVENT_MASK_EXPOSURE
//			| XCB_EVENT_MASK_STRUCTURE_NOTIFY
//			| XCB_EVENT_MASK_KEY_PRESS
//			| XCB_EVENT_MASK_KEY_RELEASE
//			| XCB_EVENT_MASK_BUTTON_PRESS
//			| XCB_EVENT_MASK_BUTTON_RELEASE
//			| XCB_EVENT_MASK_BUTTON_MOTION
//			| XCB_EVENT_MASK_ENTER_WINDOW
//			| XCB_EVENT_MASK_LEAVE_WINDOW
//			| XCB_EVENT_MASK_PROPERTY_CHANGE
//			| XCB_EVENT_MASK_FOCUS_CHANGE;

	if (gFlags)
	{
		// the order of all events must be exact
		if (gFlags.test (event_type::Size))        uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY |
															XCB_EVENT_MASK_RESIZE_REDIRECT;
		if (gFlags.test (event_type::Paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
		if (gFlags.test (event_type::KeyPressed))  uMask |= XCB_EVENT_MASK_KEY_PRESS;
		if (gFlags.test (event_type::KeyReleased)) uMask |= XCB_EVENT_MASK_KEY_RELEASE;
		if (gFlags.test (event_type::ButtonDown))  uMask |= XCB_EVENT_MASK_BUTTON_PRESS;
		if (gFlags.test (event_type::ButtonUp))    uMask |= XCB_EVENT_MASK_BUTTON_RELEASE;
		if (gFlags.test (event_type::PointerMove)) uMask |= XCB_EVENT_MASK_POINTER_MOTION;
		if (gFlags.test (event_type::Step))        uMask |= XCB_EVENT_MASK_ENTER_WINDOW |
															  XCB_EVENT_MASK_LEAVE_WINDOW;
//		if (gFlags.test (event_type::PointerMove)) uMask |= XCB_EVENT_MASK_POINTER_MOTION;
//		if (gFlags.test (event_type::KeyMap))      uMask |= XCB_EVENT_MASK_KEYMAP_STATE;
//		if (gFlags.test (event_type::Paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
		if (gFlags.test (event_type::Visibility))  uMask |= XCB_EVENT_MASK_VISIBILITY_CHANGE;
//		if (gFlags.test (event_type::Size))        uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY |
//															XCB_EVENT_MASK_RESIZE_REDIRECT;
//		if (gFlags.test (event_type::Focus))       uMask |= XCB_EVENT_MASK_FOCUS_CHANGE;
		if (gFlags.test (event_type::Property))    uMask |= XCB_EVENT_MASK_PROPERTY_CHANGE;
		if (gFlags.test (event_type::Focus))       uMask |= XCB_EVENT_MASK_FOCUS_CHANGE;
	}

	xcb_change_window_attributes (display ().get<xcb_connection_t> (),
								  pRenderable.id ().get<u32> (),
								  XCB_CW_EVENT_MASK,
								  &uMask);
	return true;
}

bool XQueue::pop_front (event_type& gEvent, bool bWait) noexcept
{
	XcbEvent pEv (bWait ? xcb_wait_for_event (display ().get<display_type> ()) :
						  xcb_poll_for_event (display ().get<display_type> ()));

	if (!pEv) return false;
	gEvent = pEv.toEvent ();
	return true;
}

int XQueue::poll (IWindow const& window, bool wait, atomic_bool& poll)
{
	while (poll.load (std::memory_order_relaxed))
	{
		XcbEvent pEv (wait ? xcb_wait_for_event (display ().get<display_type> ()) :
							 xcb_poll_for_event (display ().get<display_type> ()));

		if (!pEv or pEv.window () != window.id ()) continue;
		pEv.toEvent ()();
	}

	return 0;
}

void XQueue::send (event_type const& event)
{
	XcbEvent::value_type send_event = XcbEvent::toNativeEvent (event);

	xcb_send_event (display ().get<display_type> (),
					false,
					event.window ().get<handle_type> (),
					send_event.response_type,
					reinterpret_cast<cchar*> (&send_event));

	xcb_flush (display ().get<display_type> ());
}

void XQueue::post (event_type const& event)
{
	XcbEvent::value_type post_event = XcbEvent::toNativeEvent (event);

	xcb_send_event (display ().get<display_type> (),
					false,
					event.window ().get<handle_type> (),
					post_event.response_type,
					reinterpret_cast<cchar*> (&post_event));

	xcb_flush (display ().get<display_type> ());
}

} } // namespace Input

#endif // OS_GNU_LINUX or OS_BSD
