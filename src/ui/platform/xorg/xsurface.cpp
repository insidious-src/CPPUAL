/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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
#include "xsurface.h"

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xcb_window.h"

#include <memory>

#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>

namespace cppual { namespace Ui {

// =========================================================

XWindow::XWindow (Rect const& gRect, u32 nScreen, IDisplay* pDisplay) noexcept
: IPlatformWindow (pDisplay, ::xcb_generate_id (pDisplay->native<x::display_type> ())),
  m_eFlags  (WindowHints),
  m_uScreen (nScreen)
{
    if (valid ())
    {
        auto pScreen = x::screenHandle (connection ()->native<x::display_type> (), nScreen);

        ::xcb_create_window (connection ()->native<x::display_type> (),
                             XCB_COPY_FROM_PARENT,
                             id ().get<x::handle_type> (),
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

        auto destroyReply = x::internAtomHelper(connection ()->native<x::display_type> (),
                                                  "WM_DELETE_WINDOW");

        auto protoReply = x::internAtomHelper(connection ()->native<x::display_type> (),
                                                "WM_PROTOCOLS",
                                                true);

        ::xcb_change_property(connection ()->native<x::display_type> (),
                              XCB_PROP_MODE_REPLACE,
                              id ().get<x::handle_type> (),
                              protoReply->atom,
                              4,
                              x::atom_format,
                              1,
                              &(destroyReply->atom));
    }
}

XWindow::~XWindow () noexcept
{
    ::xcb_destroy_window (connection ()->native<x::display_type> (), id ().get<x::handle_type> ());
}

Rect XWindow::geometry () const
{
    auto reply = x::geometryHelper(connection ()->native<x::display_type> (),
                                     id ().get<x::handle_type> ());


    return Rect (reply->x, reply->y, reply->width, reply->height);
}

void XWindow::setGeometry (Rect const& gRect)
{
    cu32 uRect[4] =
    {
        static_cast<u32> (gRect.left),
        static_cast<u32> (gRect.top),
        static_cast<u32> (gRect.width()),
        static_cast<u32> (gRect.height())
    };

    ::xcb_configure_window (connection ()->native<x::display_type> (),
                            id ().get<x::handle_type> (),
                            x::Resize,
                            uRect);
}

void XWindow::raise ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_ABOVE };

    ::xcb_configure_window (connection ()->native<x::display_type> (),
                            id ().get<x::handle_type> (),
                            x::Stack,
                            uMode);
}

void XWindow::lower ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_BELOW };

    ::xcb_configure_window (connection ()->native<x::display_type> (),
                            id ().get<x::handle_type> (),
                            x::Stack,
                            uMode);
}

void XWindow::setOwner (shared_window const& pParent)
{
    static u32 dump;
    Rect rect = geometry ();

    ::xcb_reparent_window (connection ()->native<x::display_type> (),
                           id ().get<x::handle_type> (),
                           pParent != nullptr ?
                                            pParent->id ().get<x::handle_type> () :
                                            x::screenHandle (connection ()->native<x::display_type> (),
                                                               dump)->root,
                           rect.left, rect.top);
}

void XWindow::move (point2i gPos)
{
    cu32 uPoint[2] = { static_cast<u32> (gPos.x), static_cast<u32> (gPos.y) };

    ::xcb_configure_window (connection ()->native<x::display_type> (),
                            id ().get<x::handle_type> (),
                            x::Move,
                            uPoint);
}

void XWindow::map ()
{
    ::xcb_map_window (connection ()->native<x::display_type> (),
                      id ().get<x::handle_type> ());

    ::xcb_flush (connection ()->native<x::display_type> ());
}

void XWindow::unmap ()
{
    ::xcb_unmap_window (connection ()->native<x::display_type> (),
                        id ().get<x::handle_type> ());

    ::xcb_flush (connection ()->native<x::display_type> ());
}

void XWindow::setFlags (WindowFlags) noexcept
{
}

bool XWindow::isMapped () const
{
    auto reply = x::attribHelper(connection ()->native<x::display_type> (),
                                   id ().get<x::handle_type> ());

    return reply->map_state;
}

// =========================================================

XWindow::string_type XWindow::title () const noexcept
{
    auto cookie = ::xcb_get_property (connection ()->native<x::display_type> (),
                                      XCB_GET_PROPERTY_TYPE_ANY,
                                      id ().get<x::handle_type> (),
                                      XCB_ATOM_WM_NAME,
                                      XCB_ATOM_STRING,
                                      0,
                                      255);


    x::prop_ptr reply (::xcb_get_property_reply (connection ()->native<x::display_type> (),
                                                   cookie,
                                                   nullptr));

    return string_type(static_cast<char*>     (::xcb_get_property_value       (reply.get ())) ,
                       static_cast<size_type> (::xcb_get_property_value_length(reply.get ())));
}

void XWindow::setTitle (string const& gTitle) noexcept
{
     ::xcb_change_property (connection ()->native<x::display_type> (),
                            XCB_PROP_MODE_REPLACE,
                            id ().get<x::handle_type> (),
                            XCB_ATOM_WM_NAME,
                            XCB_ATOM_STRING,
                            x::string_format,
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
    auto atom_wm_state = x::internAtomHelper (connection ()->native<x::display_type> (),
                                                "_NET_WM_STATE");
    auto atom_wm_fullscreen = x::internAtomHelper (connection ()->native<x::display_type> (),
                                                     "_NET_WM_STATE_FULLSCREEN");

    if (bFullscreen)
    {
        ::xcb_change_property (connection ()->native<x::display_type> (),
                               XCB_PROP_MODE_REPLACE,
                               id ().get<x::handle_type> (),
                               atom_wm_state->atom,
                               XCB_ATOM_ATOM,
                               x::atom_format,
                               1,
                               &(atom_wm_fullscreen->atom));
    }
    else
    {
        ::xcb_delete_property (connection ()->native<x::display_type> (),
                               id ().get<x::handle_type> (),
                               atom_wm_state->atom);
    }

    unmap();
    map();
}

bool XWindow::isFullscreen () noexcept
{
    auto atom_wm_state = x::internAtomHelper(connection ()->native<x::display_type> (),
                                               "_NET_WM_STATE");

    auto atom_wm_fullscreen = x::internAtomHelper(connection ()->native<x::display_type> (),
                                                    "_NET_WM_STATE_FULLSCREEN");

    auto fullscreen_prop = x::propAtomHelper(connection ()->native<x::display_type> (),
                                               id ().get<x::handle_type> (),
                                               atom_wm_state->atom,
                                               XCB_ATOM_ATOM);

    auto atom_fullscreen = static_cast<x::atom_type*>(::xcb_get_property_value(fullscreen_prop.get()));

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
