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

#include <memory>
#include <xcb/xcb.h>
#include <xcb/xcb_ewmh.h>
#include <xcb/xcb_icccm.h>
#include "xcb_window.h"
#include "xcb_iterator.h"


namespace cppual { namespace Ui {

enum
{
    XcbMove   = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
    XcbResize = XcbMove | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
    XcbStack  = XCB_CONFIG_WINDOW_STACK_MODE
};

xcb_screen_t* screenHandle (Graphics::Connection pDsp, u32& nScreen) noexcept
{
    static xcb_setup_t const* pSetup = xcb_get_setup (pDsp.get<Xcb::display_type> ());
    static u32 nNum = static_cast<u32> (xcb_setup_roots_length (pSetup));
    Xcb::ScreenForwardIterator it (*pSetup);

    if (nNum < nScreen) nScreen = nNum - 1;
    else nNum = nScreen;

    for (u8 n = 1; n < nScreen; ++n) ++it;
    return it->data;
}

// =========================================================

XWindow::XWindow (Rect const& gRect, u32 nScreen, IDisplay* pDisplay) noexcept
: IPlatformWindow (pDisplay, ::xcb_generate_id (pDisplay->native<Xcb::display_type> ())),
  m_eFlags  (WindowHints),
  m_uScreen (nScreen)
{
    if (valid ())
    {
        auto pScreen = screenHandle (connection ()->native<Xcb::display_type> (),
                                     nScreen);

        ::xcb_create_window (connection ()->native<Xcb::display_type> (),
                             XCB_COPY_FROM_PARENT,
                             id ().get<Xcb::handle_type> (),
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

        Xcb::intern_ptr destroyReply (Xcb::internAtomHelper(connection ()->native<Xcb::display_type> (),
                                                            "WM_DELETE_WINDOW"));

        Xcb::intern_ptr reply (Xcb::internAtomHelper(connection ()->native<Xcb::display_type> (),
                                                     "WM_PROTOCOLS", true));

        ::xcb_change_property(connection ()->native<Xcb::display_type> (),
                              XCB_PROP_MODE_REPLACE,
                              id ().get<Xcb::handle_type> (),
                              reply->atom, 4, 32, 1,
                              &destroyReply->atom);
    }
}

XWindow::~XWindow () noexcept
{
    ::xcb_destroy_window (connection ()->native<Xcb::display_type> (), id ().get<Xcb::handle_type> ());
    //if (m_pDestroy) delete m_pDestroy.get<::xcb_intern_atom_reply_t> ();
}

Rect XWindow::geometry () const
{
    typedef Xcb::handle_ptr<::xcb_get_geometry_reply_t> geo_ptr;

    geo_ptr pReply (xcb_get_geometry_reply (connection ()->native<Xcb::display_type> (),
                                            xcb_get_geometry (connection ()->native<Xcb::display_type> (),
                                                              id ().get<Xcb::handle_type> ()),
                                            nullptr));

    return Rect (pReply->x, pReply->y, pReply->width, pReply->height);
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

    xcb_configure_window (connection ()->native<Xcb::display_type> (),
                          id ().get<Xcb::handle_type> (),
                          XcbResize, uRect);
}

void XWindow::raise ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_ABOVE };

    xcb_configure_window (connection ()->native<Xcb::display_type> (),
                          id ().get<Xcb::handle_type> (),
                          XcbStack,
                          uMode);
}

void XWindow::lower ()
{
    static constexpr cu32 uMode[] = { XCB_STACK_MODE_BELOW };

    xcb_configure_window (connection ()->native<Xcb::display_type> (),
                          id ().get<Xcb::handle_type> (),
                          XcbStack, uMode);
}

void XWindow::setOwner (shared_window const& pParent)
{
    static u32 dump;
    Rect       rect = geometry ();

    ::xcb_reparent_window (connection ()->native<Xcb::display_type> (),
                           id ().get<Xcb::handle_type> (),
                           pParent != nullptr ?
                                            pParent->id ().get<Xcb::handle_type> () :
                                            screenHandle (connection ()->native<Xcb::display_type> (),
                                                          dump)->root,
                           rect.left, rect.top);
}

void XWindow::move (point2i gPos)
{
    cu32 uPoint[2] = { static_cast<u32> (gPos.x), static_cast<u32> (gPos.y) };

    ::xcb_configure_window (connection ()->native<Xcb::display_type> (),
                            id ().get<Xcb::handle_type> (),
                            XcbMove, uPoint);
}

void XWindow::map ()
{
    ::xcb_map_window (connection ()->native<Xcb::display_type> (),
                      id ().get<Xcb::handle_type> ());

    ::xcb_flush (connection ()->native<Xcb::display_type> ());
}

void XWindow::unmap ()
{
    ::xcb_unmap_window (connection ()->native<Xcb::display_type> (),
                        id ().get<Xcb::handle_type> ());

    ::xcb_flush (connection ()->native<Xcb::display_type> ());
}

void XWindow::setFlags (WindowFlags) noexcept
{
}

bool XWindow::isMapped () const
{
    typedef Xcb::handle_ptr<xcb_get_window_attributes_reply_t> attrib_ptr;

    attrib_ptr pReply (::xcb_get_window_attributes_reply (
                            connection ()->native<Xcb::display_type> (),
                            ::xcb_get_window_attributes (connection ()->native<Xcb::display_type> (),
                                                         id ().get<Xcb::handle_type> ()),
                            nullptr));
    return pReply->map_state;
}

// =========================================================

XWindow::string_type XWindow::title () const noexcept
{
    typedef Xcb::handle_ptr<xcb_get_property_reply_t> prop_ptr;

    prop_ptr pReply (::xcb_get_property_reply (connection ()->native<Xcb::display_type> (),
                                               xcb_get_property (
                                                    connection ()->native<Xcb::display_type> (),
                                                    XCB_GET_PROPERTY_TYPE_ANY,
                                                    id ().get<Xcb::handle_type> (),
                                                    XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 0, 255),
                                               nullptr));

    return string_type(static_cast<char*>     (::xcb_get_property_value       (pReply.get ())) ,
                       static_cast<size_type> (::xcb_get_property_value_length(pReply.get ())));
}

void XWindow::setTitle (string const& gTitle) noexcept
{
     ::xcb_change_property (connection ()->native<Xcb::display_type> (),
                            XCB_PROP_MODE_REPLACE,
                            id ().get<Xcb::handle_type> (),
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

void XWindow::setFullscreen (bool bFullscreen) noexcept
{

    Xcb::intern_ptr atom_wm_state (Xcb::internAtomHelper (connection ()->native<Xcb::display_type> (),
                                                          "_NET_WM_STATE"));
    Xcb::intern_ptr atom_wm_fullscreen (Xcb::internAtomHelper (connection ()->native<Xcb::display_type> (),
                                                               "_NET_WM_STATE_FULLSCREEN"));


    ::xcb_change_property (connection ()->native<Xcb::display_type> (),
                           XCB_PROP_MODE_REPLACE,
                           id ().get<Xcb::handle_type> (), atom_wm_state->atom,
                           XCB_ATOM_ATOM, 32, bFullscreen ? 1 : 0,
                           &(atom_wm_fullscreen->atom));
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

#endif // OS_GNU_LINUX or OS_BSD
