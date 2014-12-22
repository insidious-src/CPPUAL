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

#include <memory>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include "xsurface.h"

namespace cppual { namespace Ui {

enum
{
	Move   = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
	Resize = Move | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
	Stack  = XCB_CONFIG_WINDOW_STACK_MODE
};

xcb_screen_t* screenHandle (Connection pDsp, u32& nScreen) noexcept
{
	static xcb_setup_t const* pSetup = xcb_get_setup (pDsp.get<xcb_connection_t> ());
	static u32 nNum = static_cast<u32> (xcb_setup_roots_length (pSetup));
	xcb_screen_iterator_t it = xcb_setup_roots_iterator (pSetup);

	if (nNum < nScreen) nScreen = nNum - 1;
	else nNum = nScreen;

	for (u8 n = 1; n < nScreen; ++n) xcb_screen_next (&it);
	return it.data;
}

// =========================================================

XRenderable::XRenderable (Rect const& gRect, u32 nScreen, IDisplay* pDisplay) noexcept
: IRenderable (pDisplay,
			   xcb_generate_id (pDisplay->native ().get<xcb_connection_t> ()),
			   ResourceType::Surface),
  m_uScreen ()
{
	if (isValid ())
	{
		xcb_screen_t* pScreen = screenHandle (connection ()->native ().get<xcb_connection_t> (),
											  nScreen);

		xcb_create_window (connection ()->native ().get<xcb_connection_t> (),
						   XCB_COPY_FROM_PARENT,
						   id ().get<u32> (),
						   pScreen->root,
						   gRect.left,
						   gRect.top,
						   gRect.width (),
						   gRect.height (),
						   1,
						   XCB_WINDOW_CLASS_INPUT_OUTPUT,
						   pScreen->root_visual,
						   0,
						   nullptr);

		m_uScreen = nScreen;
	}
}

Rect XRenderable::geometry () const
{
	typedef std::auto_ptr<xcb_get_geometry_reply_t> geo_ptr;

	geo_ptr pReply (xcb_get_geometry_reply (connection ()->native ().get<xcb_connection_t> (),
											xcb_get_geometry (connection ()->native ().
															  get<xcb_connection_t> (),
															  id ().get<u32> ()),
											nullptr));

	return Rect (pReply->x, pReply->y, pReply->width, pReply->height);
}

void XRenderable::setGeometry (Rect const& gRect)
{
	cu32 uRect[4] =
	{
		static_cast<u32> (gRect.left),
		static_cast<u32> (gRect.top),
		static_cast<u32> (gRect.right  - gRect.left),
		static_cast<u32> (gRect.bottom - gRect.top)
	};

	xcb_configure_window (connection ()->native ().get<xcb_connection_t> (),
						  id ().get<u32> (),
						  Resize, uRect);
}

void XRenderable::raise ()
{
	static constexpr cu32 uMode[] = { XCB_STACK_MODE_ABOVE };

	xcb_configure_window (connection ()->native ().get<xcb_connection_t> (),
						  id ().get<u32> (),
						  Stack,
						  uMode);
}

void XRenderable::lower ()
{
	static constexpr cu32 uMode[] = { XCB_STACK_MODE_BELOW };

	xcb_configure_window (connection ()->native ().get<xcb_connection_t> (),
						  id ().get<u32> (),
						  Stack, uMode);
}

//void XRenderable::setParent (shared_renderable const& pParent, point2i gPos)
//{
//	static u32 dump;

//	xcb_reparent_window (connection ()->native ().get<xcb_connection_t> (),
//						 id ().get<u32> (),
//						 pParent != nullptr ?
//										pParent->id ().get<u32> () :
//										screenHandle (connection ()->native ().
//													  get<xcb_connection_t> (),
//													  dump)->root,
//						 gPos.x, gPos.y);
//}

void XRenderable::move (point2i gPos)
{
	cu32 uPoint[2] = { static_cast<u32> (gPos.x), static_cast<u32> (gPos.y) };

	xcb_configure_window (connection ()->native ().get<xcb_connection_t> (),
						  id ().get<u32> (),
						  Move, uPoint);
}

void XRenderable::map ()
{
	xcb_map_window (connection ()->native ().get<xcb_connection_t> (),
					id ().get<u32> ());
}

void XRenderable::unmap ()
{
	xcb_unmap_window (connection ()->native ().get<xcb_connection_t> (),
					  id ().get<u32> ());
}

bool XRenderable::isMapped () const
{
	typedef std::auto_ptr<xcb_get_window_attributes_reply_t> attrib_ptr;

	attrib_ptr pReply (xcb_get_window_attributes_reply (
						   connection ()->native ().get<xcb_connection_t> (),
						   xcb_get_window_attributes (connection ()->native ().
													  get<xcb_connection_t> (),
													  id ().get<u32> ()),
						   nullptr));
	return pReply->map_state;
}

// =========================================================

string XRenderable::title () const noexcept
{
//	xcb_get_property_reply_t* pReply =
//			xcb_get_property_reply (connection ()->id ().x_handle,
//									xcb_get_property (
//										connection ()->id ().x_handle,
//										XCB_GET_PROPERTY_TYPE_ANY,
//										id ().x_handle,
//										XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 0),
//									nullptr);

	return string ();
}

void XRenderable::setTitle (string const& gTitle) noexcept
{
	// set the title of the windowqdw
	 xcb_change_property (connection ()->native ().get<xcb_connection_t> (),
						  XCB_PROP_MODE_REPLACE,
						  id ().get<u32> (),
						  XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
						  static_cast<u32> (gTitle.length ()), gTitle.c_str ());
}

void XRenderable::setShaded (bool) noexcept
{
}

bool XRenderable::isShaded () noexcept
{
	return false;
}

void XRenderable::setModal (bool) noexcept
{
}

bool XRenderable::isModal () noexcept
{
	return false;
}

void XRenderable::setFullscreen (bool) noexcept
{
}

bool XRenderable::isFullscreen () noexcept
{
	return false;
}

void XRenderable::setMaximized (bool) noexcept
{
}

bool XRenderable::isMaximized () noexcept
{
	return false;
}

void XRenderable::setMinimized (bool) noexcept
{
}

bool XRenderable::isMinimized () noexcept
{
	return false;
}

void XRenderable::setVisibleInTaskbar (bool) noexcept
{
}

bool XRenderable::isVisibleInTaskbar () noexcept
{
	return true;
}

void XRenderable::setVisibleInPager (bool) noexcept
{
}

bool XRenderable::isVisibleInPager () noexcept
{
	return true;
}

void XRenderable::setWMFrame (bool) noexcept
{
}

void XRenderable::setMimimumSize (point2u /*gSize*/) noexcept
{
	xcb_get_property_cookie_t cookie =
			xcb_icccm_get_wm_hints_unchecked (connection ()->native ().get<xcb_connection_t> (),
											  id ().get<u32> ());

	xcb_icccm_wm_hints_t hints;
	xcb_icccm_get_wm_hints_reply (connection ()->native ().get<xcb_connection_t> (),
								  cookie, &hints, nullptr);

	//xcb_icccm_size_hints_set_min_size (&hints, gSize.x, gSize.y);
}

void XRenderable::setMaximumSize (point2u /*gSize*/) noexcept
{
	xcb_get_property_cookie_t cookie =
			xcb_icccm_get_wm_hints_unchecked (connection ()->native ().get<xcb_connection_t> (),
											  id ().get<u32> ());

	xcb_icccm_wm_hints_t hints;
	xcb_icccm_get_wm_hints_reply (connection ()->native ().get<xcb_connection_t> (),
								  cookie, &hints, nullptr);

	//xcb_icccm_size_hints_set_max_size (&hints, gSize.x, gSize.y);
}

void XRenderable::flash () noexcept
{
}

Rect XRenderable::workArea () noexcept
{
	return Rect ();
}

} } //Ui
