/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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

#define  API_EXPORT
#include "xwindow.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xcb_window.h"

#include <memory>

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>

namespace cppual { namespace ui {

// =========================================================

XWindow::XWindow (rect const& gRect, u32 nScreen, shared_display pDisplay) noexcept
: platform_wnd_interface (pDisplay, ::xcb_generate_id (pDisplay->native<xcb::display_type> ())),
  _M_eFlags  (WindowHints),
  _M_uScreen (nScreen)
{
    if (valid ())
    {
        auto pScreen = xcb::screenHandle (connection ()->native<xcb::display_type> (), nScreen);

        ::xcb_create_window (connection ()->native<xcb::display_type> (),
                             XCB_COPY_FROM_PARENT,
                             handle ().get<xcb::handle_type> (),
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

        auto destroyReply = xcb::internAtomHelper(connection ()->native<xcb::display_type> (),
                                                "WM_DELETE_WINDOW");

        auto protoReply = xcb::internAtomHelper(connection ()->native<xcb::display_type> (),
                                              "WM_PROTOCOLS",
                                              true);

        ::xcb_change_property(connection ()->native<xcb::display_type> (),
                              XCB_PROP_MODE_REPLACE,
                              handle ().get<xcb::handle_type> (),
                              protoReply->atom,
                              4,
                              xcb::atom_format,
                              1,
                              &(destroyReply->atom));
    }
}

XWindow::~XWindow () noexcept
{
    ::xcb_destroy_window (connection ()->native<xcb::display_type> (), handle ().get<xcb::handle_type> ());
}

rect XWindow::geometry () const
{
    auto reply = xcb::geometryHelper(connection ()->native<xcb::display_type> (),
                                     handle ().get<xcb::handle_type> ());


    return rect (reply->x, reply->y, reply->width, reply->height);
}

void XWindow::setGeometry (rect const& gRect)
{
    cu32 uRect[4] =
    {
        static_cast<u32> (gRect.left),
        static_cast<u32> (gRect.top),
        static_cast<u32> (gRect.width()),
        static_cast<u32> (gRect.height())
    };

    ::xcb_configure_window (connection ()->native<xcb::display_type> (),
                            handle ().get<xcb::handle_type> (),
                            xcb::Resize,
                            uRect);
}

void XWindow::raise ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_ABOVE };

    ::xcb_configure_window (connection ()->native<xcb::display_type> (),
                            handle ().get<xcb::handle_type> (),
                            xcb::Stack,
                            uMode);
}

void XWindow::lower ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_BELOW };

    ::xcb_configure_window (connection ()->native<xcb::display_type> (),
                            handle ().get<xcb::handle_type> (),
                            xcb::Stack,
                            uMode);
}

void XWindow::setOwner (shared_window const& pParent)
{
    static u32 dump;
    rect rect = geometry ();

    ::xcb_reparent_window (connection ()->native<xcb::display_type> (),
                           handle ().get<xcb::handle_type> (),
                           pParent != nullptr ?
                                            pParent->handle ().get<xcb::handle_type> () :
                                            xcb::screenHandle (connection ()->native<xcb::display_type> (),
                                                               dump)->root,
                           rect.left, rect.top);
}

void XWindow::move (point2i gPos)
{
    cu32 uPoint[2] = { static_cast<u32> (gPos.x), static_cast<u32> (gPos.y) };

    ::xcb_configure_window (connection ()->native<xcb::display_type> (),
                            handle ().get<xcb::handle_type> (),
                            xcb::Move,
                            uPoint);
}

void XWindow::map ()
{
    ::xcb_map_window (connection ()->native<xcb::display_type> (),
                      handle ().get<xcb::handle_type> ());

    ::xcb_flush (connection ()->native<xcb::display_type> ());
}

void XWindow::unmap ()
{
    ::xcb_unmap_window (connection ()->native<xcb::display_type> (),
                        handle ().get<xcb::handle_type> ());

    ::xcb_flush (connection ()->native<xcb::display_type> ());
}

void XWindow::setFlags (window_flags) noexcept
{
}

bool XWindow::isMapped () const
{
    auto reply = xcb::attribHelper(connection ()->native<xcb::display_type> (),
                                   handle ().get<xcb::handle_type> ());

    return reply->map_state;
}

// =========================================================

XWindow::string_type XWindow::title () const noexcept
{
    auto cookie = ::xcb_get_property (connection ()->native<xcb::display_type> (),
                                      XCB_GET_PROPERTY_TYPE_ANY,
                                      handle ().get<xcb::handle_type> (),
                                      XCB_ATOM_WM_NAME,
                                      XCB_ATOM_STRING,
                                      0,
                                      255);


    xcb::prop_ptr reply (::xcb_get_property_reply (connection ()->native<xcb::display_type> (),
                                                   cookie,
                                                   nullptr));

    return string_type(static_cast<char*>     (::xcb_get_property_value       (reply.get ())) ,
                       static_cast<size_type> (::xcb_get_property_value_length(reply.get ())));
}

void XWindow::setTitle (string_type const& gTitle) noexcept
{
     ::xcb_change_property (connection ()->native<xcb::display_type> (),
                            XCB_PROP_MODE_REPLACE,
                            handle ().get<xcb::handle_type> (),
                            XCB_ATOM_WM_NAME,
                            XCB_ATOM_STRING,
                            xcb::string_format,
                            static_cast<u32> (gTitle.length ()),
                            gTitle.c_str ());
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

void XWindow::setFullscreen (bool bFullscreen) noexcept
{
    auto atom_wm_state = xcb::internAtomHelper (connection ()->native<xcb::display_type> (),
                                                "_NET_WM_STATE");
    auto atom_wm_fullscreen = xcb::internAtomHelper (connection ()->native<xcb::display_type> (),
                                                     "_NET_WM_STATE_FULLSCREEN");

    if (bFullscreen)
    {
        ::xcb_change_property (connection ()->native<xcb::display_type> (),
                               XCB_PROP_MODE_REPLACE,
                               handle ().get<xcb::handle_type> (),
                               atom_wm_state->atom,
                               XCB_ATOM_ATOM,
                               xcb::atom_format,
                               1,
                               &(atom_wm_fullscreen->atom));
    }
    else
    {
        ::xcb_delete_property (connection ()->native<xcb::display_type> (),
                               handle ().get<xcb::handle_type> (),
                               atom_wm_state->atom);
    }

    unmap();
    map();
}

bool XWindow::isFullscreen () noexcept
{
    auto atom_wm_state = xcb::internAtomHelper(connection ()->native<xcb::display_type> (),
                                               "_NET_WM_STATE");

    auto atom_wm_fullscreen = xcb::internAtomHelper(connection ()->native<xcb::display_type> (),
                                                    "_NET_WM_STATE_FULLSCREEN");

    auto fullscreen_prop = xcb::propAtomHelper(connection ()->native<xcb::display_type> (),
                                               handle ().get<xcb::handle_type> (),
                                               atom_wm_state->atom,
                                               XCB_ATOM_ATOM);

    auto atom_fullscreen = static_cast<xcb::atom_type*>(::xcb_get_property_value(fullscreen_prop.get()));

    return *atom_fullscreen == atom_wm_fullscreen->atom;
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

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
