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
#include <cppual/ui/default_events.h>
#include <cppual/input/pointer.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xinput.h"

using std::string;
using namespace cppual::Input;

namespace cppual { namespace Ui {

namespace {

typedef xcb_button_t  xcb_button;
typedef xcb_keycode_t xcb_keycode;
typedef xcb_keysym_t  xcb_keysym;

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

	constexpr operator pointer () const noexcept
	{ return m_handle; }

	constexpr pointer operator -> () const noexcept
	{ return m_handle; }

	template <typename T>
	constexpr T* get () const noexcept
	{ return reinterpret_cast<T*> (m_handle); }

	constexpr uint type () const noexcept
	{ return m_handle->response_type & ~0x80; }

	void release ()
	{ ::free (m_handle); }

	inline    XcbEvent ()               noexcept = default;
	constexpr XcbEvent (pointer pDpy)   noexcept : m_handle (pDpy) { }
	constexpr XcbEvent (std::nullptr_t) noexcept : m_handle ()     { }

private:
	pointer m_handle;
};

} // anonymous namespace

// ====================================================

XQueue::XQueue () noexcept
: IDisplayQueue (IDisplay::hasValidInstance () ? IDisplay::instance ()->native () : nullptr)
{ }

bool XQueue::setRenderableEvents (Ui::IRenderable& pRenderable,
								  mask_type        gFlags) noexcept
{
	if (pRenderable.connection ()->native () != display ())
		return false;

	u32 uMask = XCB_EVENT_MASK_NO_EVENT;

	if (gFlags)
	{
		// the order of all events must be exact
		if (gFlags.hasBit (event_type::KeyPressed))  uMask |= XCB_EVENT_MASK_KEY_PRESS;
		if (gFlags.hasBit (event_type::KeyReleased)) uMask |= XCB_EVENT_MASK_KEY_RELEASE;
		if (gFlags.hasBit (event_type::ButtonDown))  uMask |= XCB_EVENT_MASK_BUTTON_PRESS;
		if (gFlags.hasBit (event_type::ButtonUp))    uMask |= XCB_EVENT_MASK_BUTTON_RELEASE;
		if (gFlags.hasBit (event_type::Step))        uMask |= XCB_EVENT_MASK_ENTER_WINDOW |
															  XCB_EVENT_MASK_LEAVE_WINDOW;
		if (gFlags.hasBit (event_type::PointerMove)) uMask |= XCB_EVENT_MASK_POINTER_MOTION;
		if (gFlags.hasBit (event_type::KeyMap))      uMask |= XCB_EVENT_MASK_KEYMAP_STATE;
		if (gFlags.hasBit (event_type::Paint))       uMask |= XCB_EVENT_MASK_EXPOSURE;
		if (gFlags.hasBit (event_type::Visibility))  uMask |= XCB_EVENT_MASK_VISIBILITY_CHANGE;
		if (gFlags.hasBit (event_type::Size))        uMask |= XCB_EVENT_MASK_STRUCTURE_NOTIFY |
															  XCB_EVENT_MASK_RESIZE_REDIRECT;
		if (gFlags.hasBit (event_type::Focus))       uMask |= XCB_EVENT_MASK_FOCUS_CHANGE;
		if (gFlags.hasBit (event_type::Property) or
				gFlags.hasBit (event_type::Move))    uMask |= XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
															  XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
															  XCB_EVENT_MASK_PROPERTY_CHANGE;
	}

	xcb_change_window_attributes (display ().get<xcb_connection_t> (),
								  pRenderable.id ().get<u32> (),
								  XCB_CW_EVENT_MASK,
								  &uMask);
	return true;
}

bool XQueue::pop_front (event_type& gEvent, bool bWait) noexcept
{
	XcbEvent pEv (bWait ? xcb_wait_for_event (display ().get<xcb_connection_t> ()) :
						  xcb_poll_for_event (display ().get<xcb_connection_t> ()));

	if (!pEv) return false;

	switch (pEv.type ())
	{
	case XcbEvent::MousePress:
		switch (pEv.get<XcbEvent::btn_press_type> ()->detail)
		{
		case XcbEvent::MouseWheelUp:
			gEvent = Ui::ScrollEvent (pEv.get<XcbEvent::btn_press_type> ()->event,
									  1, { pEv.get<XcbEvent::btn_press_type> ()->event_x,
										   pEv.get<XcbEvent::btn_press_type> ()->event_y });
			break;
		case XcbEvent::MouseWheelDown:
			gEvent = Ui::ScrollEvent (pEv.get<XcbEvent::btn_press_type> ()->event,
									  -1, { pEv.get<XcbEvent::btn_press_type> ()->event_x,
											pEv.get<XcbEvent::btn_press_type> ()->event_y });
			break;
		default:
			gEvent = Ui::MousePressEvent (pEv.get<XcbEvent::btn_press_type> ()->event,
										  button (pEv.get<XcbEvent::btn_press_type> ()->detail),
			{ pEv.get<XcbEvent::btn_press_type> ()->event_x,
			  pEv.get<XcbEvent::btn_press_type> ()->event_y });
			break;
		}
		break;
	case XcbEvent::MouseRelease:
		if (pEv.get<XcbEvent::btn_release_type> ()->detail == XcbEvent::MouseWheelUp or
				pEv.get<XcbEvent::btn_release_type> ()->detail == XcbEvent::MouseWheelDown)
			break;

		gEvent = Ui::MouseReleaseEvent (pEv.get<XcbEvent::btn_release_type> ()->event,
										button (pEv.get<XcbEvent::btn_release_type> ()->detail),
		{ pEv.get<XcbEvent::btn_release_type> ()->event_x,
		  pEv.get<XcbEvent::btn_release_type> ()->event_y });
		break;
	case XcbEvent::MouseMove:
		gEvent = Ui::MouseMoveEvent (pEv.get<XcbEvent::mouse_move_type> ()->event,
		{ pEv.get<XcbEvent::mouse_move_type> ()->event_x,
		  pEv.get<XcbEvent::mouse_move_type> ()->event_y });
		break;
	case XcbEvent::KeyPress:
		gEvent = Ui::KeyPressEvent ({ pEv.get<XcbEvent::key_press_type> ()->detail });
		break;
	case XcbEvent::KeyRelease:
		gEvent = Ui::KeyReleaseEvent ({ pEv.get<XcbEvent::key_release_type> ()->detail });
		break;
	case XcbEvent::KeyMapNotify:
		gEvent = Ui::KeysMappedEvent ({ pEv.get<XcbEvent::keymap_notify_type> ()->keys[0]/*,
				reinterpret_cast<xcb_keymap_notify*> (pEv)->keys[1],
				reinterpret_cast<xcb_keymap_notify*> (pEv)->keys[2],
				reinterpret_cast<xcb_keymap_notify*> (pEv)->keys[3],
				reinterpret_cast<xcb_keymap_notify*> (pEv)->keys[4]*/ });
#		ifdef DEBUG_MODE
		std::cout << "keys map event\n";
#		endif
		break;
	case XcbEvent::Expose:
		gEvent = Ui::PaintEvent (pEv.get<XcbEvent::expose_type> ()->window,
								 Rect (static_cast<int16> (pEv.get<XcbEvent::expose_type> ()->x),
									   static_cast<int16> (pEv.get<XcbEvent::expose_type> ()->y),
									   pEv.get<XcbEvent::expose_type> ()->width,
									   pEv.get<XcbEvent::expose_type> ()->height));
#		ifdef DEBUG_MODE
		std::cout << "paint event\n";
#		endif
		break;
	case XcbEvent::Enter:
		gEvent = Ui::StepEvent (pEv.get<XcbEvent::enter_type> ()->event, true);
#		ifdef DEBUG_MODE
		std::cout << "enter event\n";
#		endif
		break;
	case XcbEvent::Leave:
		gEvent = Ui::StepEvent (pEv.get<XcbEvent::leave_type> ()->event, false);
#		ifdef DEBUG_MODE
		std::cout << "leave event\n";
#		endif
		break;
	case XcbEvent::FocusIn:
		gEvent = Ui::FocusEvent (pEv.get<XcbEvent::focus_in_type> ()->event, true);
#		ifdef DEBUG_MODE
		std::cout << "focus event\n";
#		endif
		break;
	case XcbEvent::FocusOut:
		gEvent = Ui::FocusEvent (pEv.get<XcbEvent::focus_out_type> ()->event, false);
#		ifdef DEBUG_MODE
		std::cout << "kill focus event\n";
#		endif
		break;
	case XcbEvent::Size:
		gEvent = Ui::SizeEvent (pEv.get<XcbEvent::resize_type> ()->window,
		{ pEv.get<XcbEvent::resize_type> ()->width,
		  pEv.get<XcbEvent::resize_type> ()->height });
#		ifdef DEBUG_MODE
		std::cout << "resize event\n";
#		endif
		break;
	case XcbEvent::Property:
		gEvent = Ui::PropertyEvent (pEv.get<XcbEvent::property_type> ()->window,
									pEv.get<XcbEvent::property_type> ()->atom,
									pEv.get<XcbEvent::property_type> ()->state);
#		ifdef DEBUG_MODE
		std::cout << "property event\n";
#		endif
		break;
//	case Visiblility:
//		gEvent = Ui::VisibilityEvent (pEv.get<xcb_visibility_notify_event_t> ()->window,
//									  pEv.get<xcb_visibility_notify_event_t> ()->state);
//		std::cout << "visibility event\n";
//		break;
	case XcbEvent::Map:
		gEvent = Ui::VisibilityEvent (pEv.get<XcbEvent::map_type> ()->window, true);
#		ifdef DEBUG_MODE
		std::cout << "show event\n";
#		endif
		break;
	case XcbEvent::Unmap:
		gEvent = Ui::VisibilityEvent (pEv.get<XcbEvent::unmap_type> ()->window, false);
#		ifdef DEBUG_MODE
		std::cout << "hide event\n";
#		endif
		break;
	case XcbEvent::Destroy:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "destroy event\n";
#		endif
		break;
	case XcbEvent::GraphicsExposure:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "expose all event\n";
#		endif
		break;
	case XcbEvent::NoExposure:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "mask event\n";
#		endif
		break;
	case XcbEvent::ChangeParent:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "set parent event\n";
#		endif
		break;
	case XcbEvent::Mapping:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "mapping event\n";
#		endif
		break;
	case XcbEvent::Configure:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "configure event\n";
#		endif
		break;
	case XcbEvent::GetKbCtrl:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "get keyboard control event\n";
#		endif
		break;
	case XcbEvent::ChangeKbCtrl:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "change keyboard control event\n";
#		endif
		break;
	default:
		gEvent = event_type (event_type::Null);
#		ifdef DEBUG_MODE
		std::cout << "unhandled event " << pEv.type () << std::endl;
#		endif
		break;
	}

	pEv.release ();
	return true;
}

} } // namespace Input

#endif // OS_GNU_LINUX or OS_BSD
