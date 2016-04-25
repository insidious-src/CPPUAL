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

#include <memory>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include "xsurface.h"

namespace cppual { namespace Ui {

typedef xcb_connection_t display_type;
typedef xcb_window_t     handle_type;

namespace { namespace Xcb { // optimize for internal unit usage

struct Window
{
    typedef Element    value_type;
    typedef Connection pointer;

    inline static Rect geometry (pointer display, value_type id)
    {
        typedef std::auto_ptr<xcb_get_geometry_reply_t> geo_ptr;

        geo_ptr pReply (xcb_get_geometry_reply (display.get<display_type> (),
                                                xcb_get_geometry (display.
                                                                  get<display_type> (),
                                                                  id.get<handle_type> ()),
                                                nullptr));

        return Rect (pReply->x, pReply->y, pReply->width, pReply->height);
    }
};

} } // anonymous namespace Xcb

enum
{
    XcbMove   = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
    XcbResize = XcbMove | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
    XcbStack  = XCB_CONFIG_WINDOW_STACK_MODE
};

xcb_screen_t* screenHandle (Connection pDsp, u32& nScreen) noexcept
{
    static xcb_setup_t const* pSetup = xcb_get_setup (pDsp.get<display_type> ());
    static u32 nNum = static_cast<u32> (xcb_setup_roots_length (pSetup));
    xcb_screen_iterator_t it = xcb_setup_roots_iterator (pSetup);

    if (nNum < nScreen) nScreen = nNum - 1;
    else nNum = nScreen;

    for (u8 n = 1; n < nScreen; ++n) xcb_screen_next (&it);
    return it.data;
}

// =========================================================

XWindow::XWindow (Rect const& gRect, u32 nScreen, IDisplay* pDisplay) noexcept
: IWindow (pDisplay,
           xcb_generate_id (pDisplay->native ().get<display_type> ()),
           ResourceType::Surface),
  m_eFlags (WindowHints),
  m_pOwner ()
{
    if (valid ())
    {
        xcb_screen_t* pScreen = screenHandle (connection ()->native ().get<display_type> (),
                                              nScreen);

        xcb_create_window (connection ()->native ().get<display_type> (),
                           XCB_COPY_FROM_PARENT,
                           id ().get<handle_type> (),
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
    }
}

Rect XWindow::geometry () const
{
    return Xcb::Window::geometry (connection ()->native (), id ());
}

void XWindow::setGeometry (Rect const& gRect)
{
    cu32 uRect[4] =
    {
        static_cast<u32> (gRect.left),
        static_cast<u32> (gRect.top),
        static_cast<u32> (gRect.right  - gRect.left),
        static_cast<u32> (gRect.bottom - gRect.top)
    };

    xcb_configure_window (connection ()->native ().get<display_type> (),
                          id ().get<handle_type> (),
                          XcbResize, uRect);
}

void XWindow::raise ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_ABOVE };

    xcb_configure_window (connection ()->native ().get<display_type> (),
                          id ().get<handle_type> (),
                          XcbStack,
                          uMode);
}

void XWindow::lower ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_BELOW };

    xcb_configure_window (connection ()->native ().get<display_type> (),
                          id ().get<handle_type> (),
                          XcbStack, uMode);
}

void XWindow::setOwner (shared_window const& pParent)
{
    static u32 dump;
    Rect       rect = geometry ();

    xcb_reparent_window (connection ()->native ().get<display_type> (),
                         id ().get<handle_type> (),
                         pParent != nullptr ?
                                        pParent->id ().get<handle_type> () :
                                        screenHandle (connection ()->native ().
                                                      get<display_type> (),
                                                      dump)->root,
                         rect.left, rect.top);
}

void XWindow::move (point2i gPos)
{
    cu32 uPoint[2] = { static_cast<u32> (gPos.x), static_cast<u32> (gPos.y) };

    xcb_configure_window (connection ()->native ().get<display_type> (),
                          id ().get<handle_type> (),
                          XcbMove, uPoint);
}

void XWindow::map ()
{
    xcb_map_window (connection ()->native ().get<display_type> (),
                    id ().get<handle_type> ());

    xcb_flush (connection ()->native ().get<display_type> ());
}

void XWindow::unmap ()
{
    xcb_unmap_window (connection ()->native ().get<display_type> (),
                      id ().get<handle_type> ());

    xcb_flush (connection ()->native ().get<display_type> ());
}

void XWindow::setFlags (WindowFlags) noexcept
{
}

bool XWindow::isMapped () const
{
    typedef std::auto_ptr<xcb_get_window_attributes_reply_t> attrib_ptr;

    attrib_ptr pReply (xcb_get_window_attributes_reply (
                           connection ()->native ().get<display_type> (),
                           xcb_get_window_attributes (connection ()->native ().
                                                      get<display_type> (),
                                                      id ().get<handle_type> ()),
                           nullptr));
    return pReply->map_state;
}

// =========================================================

string XWindow::title () const noexcept
{
//    xcb_get_property_reply_t* pReply =
//            xcb_get_property_reply (connection ()->id ().x_handle,
//                                    xcb_get_property (
//                                        connection ()->id ().x_handle,
//                                        XCB_GET_PROPERTY_TYPE_ANY,
//                                        id ().x_handle,
//                                        XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 0),
//                                    nullptr);

    return string ();
}

void XWindow::setTitle (string const& gTitle) noexcept
{
     xcb_change_property (connection ()->native ().get<display_type> (),
                          XCB_PROP_MODE_REPLACE,
                          id ().get<handle_type> (),
                          XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                          static_cast<u32> (gTitle.length ()), gTitle.c_str ());
}

void XWindow::setShaded (bool) noexcept
{
}

bool XWindow::isShaded () noexcept
{
    return false;
}

void XWindow::setModal (bool) noexcept
{
}

bool XWindow::isModal () noexcept
{
    return false;
}

void XWindow::setFullscreen (bool) noexcept
{
}

bool XWindow::isFullscreen () noexcept
{
    return false;
}

void XWindow::setMaximized (bool) noexcept
{
}

bool XWindow::isMaximized () noexcept
{
    return false;
}

void XWindow::setMinimized (bool) noexcept
{
}

bool XWindow::isMinimized () noexcept
{
    return false;
}

void XWindow::setVisibleInTaskbar (bool) noexcept
{
}

bool XWindow::isVisibleInTaskbar () noexcept
{
    return true;
}

void XWindow::setVisibleInPager (bool) noexcept
{
}

bool XWindow::isVisibleInPager () noexcept
{
    return true;
}

void XWindow::flash (uint) noexcept
{
}

} } //Ui
