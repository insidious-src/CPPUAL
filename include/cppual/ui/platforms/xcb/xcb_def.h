/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#ifndef CPPUAL_PLATFORM_XCB_DEF_H_
#define CPPUAL_PLATFORM_XCB_DEF_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/types.h>
#include <cppual/containers.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <cppual/ui/platforms/xcb/xcb.hpp>
#include <cppual/ui/platforms/xcb/xcb_iterator.h>

#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xcb/xcb_ewmh.h>
#include <X11/Xlib-xcb.h>

namespace cppual { namespace x {

// =========================================================

enum
{
    Move   = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y,
    Resize = Move | XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT,
    Stack  = XCB_CONFIG_WINDOW_STACK_MODE
};

// =========================================================

enum atom_state
{
    remove,
    add   ,
    toggle
};

// =========================================================

typedef string                          string_type             ;
typedef ::xcb_connection_t              display_type            ;
typedef remove_ptr_t<::Display>*        legacy_type             ;
typedef ::xcb_screen_t                  screen_type             ;
typedef ::xcb_window_t                  handle_type             ;
typedef ::xcb_atom_t                    atom_type               ;
typedef ::xcb_ewmh_connection_t         ewmh_conn_type          ;
typedef ::xcb_button_t                  button_type             ;
typedef ::xcb_keysym_t                  keysym_type             ;
typedef ::xcb_keycode_t                 keycode_type            ;
typedef ::xcb_generic_event_t           base_event_type         ;
typedef ::xcb_button_press_event_t      btn_event_type          ;
typedef ::xcb_motion_notify_event_t     mouse_move_event_type   ;
typedef ::xcb_key_press_event_t         key_event_type          ;
typedef ::xcb_keymap_notify_event_t     keymap_notify_event_type;
typedef ::xcb_expose_event_t            expose_event_type       ;
typedef ::xcb_focus_in_event_t          focus_event_type        ;
typedef ::xcb_enter_notify_event_t      step_event_type         ;
typedef ::xcb_resize_request_event_t    resize_event_type       ;
typedef ::xcb_map_notify_event_t        map_event_type          ;
typedef ::xcb_unmap_notify_event_t      unmap_event_type        ;
typedef ::xcb_property_notify_event_t   property_event_type     ;
typedef ::xcb_destroy_notify_event_t    destroy_event_type      ;
typedef ::xcb_visibility_notify_event_t visibility_event_type   ;
typedef ::xcb_ge_generic_event_t        ge_generic_event_type   ;
typedef ::xcb_client_message_event_t    client_msg_event_type   ;

constexpr cu8  atom_format    = sizeof (atom_type) * 8;
constexpr cu8  string_format  = sizeof (char     ) * 8;
constexpr cu8  propagate      =    1;
constexpr cu8  dont_propagate =    0;
constexpr cu8  dont_delete    =    0;
constexpr cu32 offset_begin   =    0;
constexpr cu32 atom_max_len   = 1024;
constexpr cu32 string_max_len =  255;

// =========================================================

class screen_setup
{
public:
    typedef ::xcb_setup_t const* handle_type   ;
    typedef screen_iterator      iterator      ;
    typedef screen_iterator      const_iterator;
    typedef u32                  size_type     ;

    screen_setup () = delete;

    inline
    screen_setup (display_type* conn) noexcept
    : _M_setup (::xcb_get_setup (conn)),
      _M_count (static_cast<size_type> (::xcb_setup_roots_length (_M_setup)))
    { }

    inline
    const_iterator begin () const noexcept
    { return iterator (*_M_setup); }

    inline
    const_iterator cbegin () const noexcept
    { return iterator (*_M_setup); }

    inline
    iterator begin () noexcept
    { return iterator (*_M_setup); }

    constexpr
    const_iterator end () const noexcept
    { return iterator ({ nullptr, 0, static_cast<int> (_M_count) }); }

    constexpr
    const_iterator cend () const noexcept
    { return iterator ({ nullptr, 0, static_cast<int> (_M_count) }); }

    constexpr
    iterator end () noexcept
    { return iterator ({ nullptr, 0, static_cast<int> (_M_count) }); }

    constexpr
    size_type count () const noexcept
    { return _M_count; }

private:
    handle_type _M_setup;
    size_type   _M_count;
};

// =========================================================

constexpr screen_type* screen_handle (display_type* conn, u32& screen) noexcept
{
    auto setup = screen_setup (conn);
    auto it    = setup.begin ();

    if (setup.count () < screen) screen = setup.count () - 1;

    for (u32 n = 0; n < screen; ++n) ++it;
    return it->data;
}

// =========================================================

struct display_data
{
    x::atom_type _NET_SUPPORTED;
    x::atom_type _NET_CLIENT_LIST;
    x::atom_type _NET_CLIENT_LIST_STACKING;
    x::atom_type _NET_NUMBER_OF_DESKTOPS;
    x::atom_type _NET_DESKTOP_GEOMETRY;
    x::atom_type _NET_DESKTOP_VIEWPORT;
    x::atom_type _NET_CURRENT_DESKTOP;
    x::atom_type _NET_DESKTOP_NAMES;
    x::atom_type _NET_ACTIVE_WINDOW;
    x::atom_type _NET_WORKAREA;
    x::atom_type _NET_SUPPORTING_WM_CHECK;
    x::atom_type _NET_VIRTUAL_ROOTS;
    x::atom_type _NET_DESKTOP_LAYOUT;
    x::atom_type _NET_SHOWING_DESKTOP;
    x::atom_type _NET_CLOSE_WINDOW;
    x::atom_type _NET_MOVERESIZE_WINDOW;
    x::atom_type _NET_WM_MOVERESIZE;
    x::atom_type _NET_RESTACK_WINDOW;
    x::atom_type _NET_REQUEST_FRAME_EXTENTS;
    x::atom_type _NET_WM_NAME;
    x::atom_type _NET_WM_VISIBLE_NAME;
    x::atom_type _NET_WM_ICON_NAME;
    x::atom_type _NET_WM_VISIBLE_ICON_NAME;
    x::atom_type _NET_WM_DESKTOP;
    x::atom_type _NET_WM_WINDOW_TYPE;
    x::atom_type _NET_WM_STATE;
    x::atom_type _NET_WM_ALLOWED_ACTIONS;
    x::atom_type _NET_WM_STRUT;
    x::atom_type _NET_WM_STRUT_PARTIAL;
    x::atom_type _NET_WM_ICON_GEOMETRY;
    x::atom_type _NET_WM_ICON;
    x::atom_type _NET_WM_PID;
    x::atom_type _NET_WM_HANDLED_ICONS;
    x::atom_type _NET_WM_USER_TIME;
    x::atom_type _NET_WM_USER_TIME_WINDOW;
    x::atom_type _NET_FRAME_EXTENTS;
    x::atom_type _NET_WM_PING;
    x::atom_type _NET_WM_SYNC_REQUEST;
    x::atom_type _NET_WM_SYNC_REQUEST_COUNTER;
    x::atom_type _NET_WM_FULLSCREEN_MONITORS;
    x::atom_type _NET_WM_FULL_PLACEMENT;
    x::atom_type UTF8_STRING;
    x::atom_type WM_PROTOCOLS;
    x::atom_type MANAGER;
    x::atom_type _NET_WM_WINDOW_TYPE_DESKTOP;
    x::atom_type _NET_WM_WINDOW_TYPE_DOCK;
    x::atom_type _NET_WM_WINDOW_TYPE_TOOLBAR;
    x::atom_type _NET_WM_WINDOW_TYPE_MENU;
    x::atom_type _NET_WM_WINDOW_TYPE_UTILITY;
    x::atom_type _NET_WM_WINDOW_TYPE_SPLASH;
    x::atom_type _NET_WM_WINDOW_TYPE_DIALOG;
    x::atom_type _NET_WM_WINDOW_TYPE_DROPDOWN_MENU;
    x::atom_type _NET_WM_WINDOW_TYPE_POPUP_MENU;
    x::atom_type _NET_WM_WINDOW_TYPE_TOOLTIP;
    x::atom_type _NET_WM_WINDOW_TYPE_NOTIFICATION;
    x::atom_type _NET_WM_WINDOW_TYPE_COMBO;
    x::atom_type _NET_WM_WINDOW_TYPE_DND;
    x::atom_type _NET_WM_WINDOW_TYPE_NORMAL;
    x::atom_type _NET_WM_STATE_MODAL;
    x::atom_type _NET_WM_STATE_STICKY;
    x::atom_type _NET_WM_STATE_MAXIMIZED_VERT;
    x::atom_type _NET_WM_STATE_MAXIMIZED_HORZ;
    x::atom_type _NET_WM_STATE_SHADED;
    x::atom_type _NET_WM_STATE_SKIP_TASKBAR;
    x::atom_type _NET_WM_STATE_SKIP_PAGER;
    x::atom_type _NET_WM_STATE_HIDDEN;
    x::atom_type _NET_WM_STATE_FULLSCREEN;
    x::atom_type _NET_WM_STATE_ABOVE;
    x::atom_type _NET_WM_STATE_BELOW;
    x::atom_type _NET_WM_STATE_DEMANDS_ATTENTION;
    x::atom_type _NET_WM_ACTION_MOVE;
    x::atom_type _NET_WM_ACTION_RESIZE;
    x::atom_type _NET_WM_ACTION_MINIMIZE;
    x::atom_type _NET_WM_ACTION_SHADE;
    x::atom_type _NET_WM_ACTION_STICK;
    x::atom_type _NET_WM_ACTION_MAXIMIZE_HORZ;
    x::atom_type _NET_WM_ACTION_MAXIMIZE_VERT;
    x::atom_type _NET_WM_ACTION_FULLSCREEN;
    x::atom_type _NET_WM_ACTION_CHANGE_DESKTOP;
    x::atom_type _NET_WM_ACTION_CLOSE;
    x::atom_type _NET_WM_ACTION_ABOVE;
    x::atom_type _NET_WM_ACTION_BELOW;
    x::atom_type WM_DELETE_WINDOW;
    x::atom_type WM_CHANGE_STATE;
    x::atom_type WM_STATE;
    x::atom_type WM_CHANGE_STATE_ICONIC;
    x::atom_type WM_CHANGE_STATE_NORMAL;

    display_data () = delete;

    inline
    display_data (x::display_type* conn) noexcept
    : _NET_SUPPORTED (x::intern_atom (conn, true, "_NET_SUPPORTED")->atom),
      _NET_CLIENT_LIST (x::intern_atom (conn, true, "_NET_CLIENT_LIST")->atom),
      _NET_CLIENT_LIST_STACKING (x::intern_atom (conn, true, "_NET_CLIENT_LIST_STACKING")->atom),
      _NET_NUMBER_OF_DESKTOPS (x::intern_atom (conn, true, "_NET_NUMBER_OF_DESKTOPS")->atom),
      _NET_DESKTOP_GEOMETRY (x::intern_atom (conn, true, "_NET_DESKTOP_GEOMETRY")->atom),
      _NET_DESKTOP_VIEWPORT (x::intern_atom (conn, true, "_NET_DESKTOP_VIEWPORT")->atom),
      _NET_CURRENT_DESKTOP (x::intern_atom (conn, true, "_NET_CURRENT_DESKTOP")->atom),
      _NET_DESKTOP_NAMES (x::intern_atom (conn, true, "_NET_DESKTOP_NAMES")->atom),
      _NET_ACTIVE_WINDOW (x::intern_atom (conn, true, "_NET_ACTIVE_WINDOW")->atom),
      _NET_WORKAREA (x::intern_atom (conn, true, "_NET_WORKAREA")->atom),
      _NET_SUPPORTING_WM_CHECK (x::intern_atom (conn, true, "_NET_SUPPORTING_WM_CHECK")->atom),
      _NET_VIRTUAL_ROOTS (x::intern_atom (conn, true, "_NET_VIRTUAL_ROOTS")->atom),
      _NET_DESKTOP_LAYOUT (x::intern_atom (conn, true, "_NET_DESKTOP_LAYOUT")->atom),
      _NET_SHOWING_DESKTOP (x::intern_atom (conn, true, "_NET_SHOWING_DESKTOP")->atom),
      _NET_CLOSE_WINDOW (x::intern_atom (conn, true, "_NET_CLOSE_WINDOW")->atom),
      _NET_MOVERESIZE_WINDOW (x::intern_atom (conn, true, "_NET_MOVERESIZE_WINDOW")->atom),
      _NET_WM_MOVERESIZE (x::intern_atom (conn, true, "_NET_WM_MOVERESIZE")->atom),
      _NET_RESTACK_WINDOW (x::intern_atom (conn, true, "_NET_RESTACK_WINDOW")->atom),
      _NET_REQUEST_FRAME_EXTENTS (x::intern_atom (conn, true, "_NET_REQUEST_FRAME_EXTENTS")->atom),
      _NET_WM_NAME (x::intern_atom (conn, true, "_NET_WM_NAME")->atom),
      _NET_WM_VISIBLE_NAME (x::intern_atom (conn, true, "_NET_WM_VISIBLE_NAME")->atom),
      _NET_WM_ICON_NAME (x::intern_atom (conn, true, "_NET_WM_ICON_NAME")->atom),
      _NET_WM_VISIBLE_ICON_NAME (x::intern_atom (conn, true, "_NET_WM_VISIBLE_ICON_NAME")->atom),
      _NET_WM_DESKTOP (x::intern_atom (conn, true, "_NET_WM_DESKTOP")->atom),
      _NET_WM_WINDOW_TYPE (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE")->atom),
      _NET_WM_STATE (x::intern_atom (conn, true, "_NET_WM_STATE")->atom),
      _NET_WM_ALLOWED_ACTIONS (x::intern_atom (conn, true, "_NET_WM_ALLOWED_ACTIONS")->atom),
      _NET_WM_STRUT (x::intern_atom (conn, true, "_NET_WM_STRUT")->atom),
      _NET_WM_STRUT_PARTIAL (x::intern_atom (conn, true, "_NET_WM_STRUT_PARTIAL")->atom),
      _NET_WM_ICON_GEOMETRY (x::intern_atom (conn, true, "_NET_WM_ICON_GEOMETRY")->atom),
      _NET_WM_ICON (x::intern_atom (conn, true, "_NET_WM_ICON")->atom),
      _NET_WM_PID (x::intern_atom (conn, true, "_NET_WM_PID")->atom),
      _NET_WM_HANDLED_ICONS (x::intern_atom (conn, true, "_NET_WM_HANDLED_ICONS")->atom),
      _NET_WM_USER_TIME (x::intern_atom (conn, true, "_NET_WM_USER_TIME")->atom),
      _NET_WM_USER_TIME_WINDOW (x::intern_atom (conn, true, "_NET_WM_USER_TIME_WINDOW")->atom),
      _NET_FRAME_EXTENTS (x::intern_atom (conn, true, "_NET_FRAME_EXTENTS")->atom),
      _NET_WM_PING (x::intern_atom (conn, true, "_NET_WM_PING")->atom),
      _NET_WM_SYNC_REQUEST (x::intern_atom (conn, true, "_NET_WM_SYNC_REQUEST")->atom),
      _NET_WM_SYNC_REQUEST_COUNTER (x::intern_atom (conn, true, "_NET_WM_SYNC_REQUEST_COUNTER")->atom),
      _NET_WM_FULLSCREEN_MONITORS (x::intern_atom (conn, true, "_NET_WM_FULLSCREEN_MONITORS")->atom),
      _NET_WM_FULL_PLACEMENT (x::intern_atom (conn, true, "_NET_WM_FULL_PLACEMENT")->atom),
      UTF8_STRING (x::intern_atom (conn, true, "UTF8_STRING")->atom),
      WM_PROTOCOLS (x::intern_atom (conn, true, "WM_PROTOCOLS")->atom),
      MANAGER (x::intern_atom (conn, true, "MANAGER")->atom),
      _NET_WM_WINDOW_TYPE_DESKTOP (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_DESKTOP")->atom),
      _NET_WM_WINDOW_TYPE_DOCK (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_DOCK")->atom),
      _NET_WM_WINDOW_TYPE_TOOLBAR (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_TOOLBAR")->atom),
      _NET_WM_WINDOW_TYPE_MENU (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_MENU")->atom),
      _NET_WM_WINDOW_TYPE_UTILITY (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_UTILITY")->atom),
      _NET_WM_WINDOW_TYPE_SPLASH (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_SPLASH")->atom),
      _NET_WM_WINDOW_TYPE_DIALOG (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_DIALOG")->atom),
      _NET_WM_WINDOW_TYPE_DROPDOWN_MENU (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_DROPDOWN_MENU")->atom),
      _NET_WM_WINDOW_TYPE_POPUP_MENU (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_POPUP_MENU")->atom),
      _NET_WM_WINDOW_TYPE_TOOLTIP (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_TOOLTIP")->atom),
      _NET_WM_WINDOW_TYPE_NOTIFICATION (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_NOTIFICATION")->atom),
      _NET_WM_WINDOW_TYPE_COMBO (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_COMBO")->atom),
      _NET_WM_WINDOW_TYPE_DND (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_DND")->atom),
      _NET_WM_WINDOW_TYPE_NORMAL (x::intern_atom (conn, true, "_NET_WM_WINDOW_TYPE_NORMAL")->atom),
      _NET_WM_STATE_MODAL (x::intern_atom (conn, true, "_NET_WM_STATE_MODAL")->atom),
      _NET_WM_STATE_STICKY (x::intern_atom (conn, true, "_NET_WM_STATE_STICKY")->atom),
      _NET_WM_STATE_MAXIMIZED_VERT (x::intern_atom (conn, true, "_NET_WM_STATE_MAXIMIZED_VERT")->atom),
      _NET_WM_STATE_MAXIMIZED_HORZ (x::intern_atom (conn, true, "_NET_WM_STATE_MAXIMIZED_HORZ")->atom),
      _NET_WM_STATE_SHADED (x::intern_atom (conn, true, "_NET_WM_STATE_SHADED")->atom),
      _NET_WM_STATE_SKIP_TASKBAR (x::intern_atom (conn, true, "_NET_WM_STATE_SKIP_TASKBAR")->atom),
      _NET_WM_STATE_SKIP_PAGER (x::intern_atom (conn, true, "_NET_WM_STATE_SKIP_PAGER")->atom),
      _NET_WM_STATE_HIDDEN (x::intern_atom (conn, true, "_NET_WM_STATE_HIDDEN")->atom),
      _NET_WM_STATE_FULLSCREEN (x::intern_atom (conn, true, "_NET_WM_STATE_FULLSCREEN")->atom),
      _NET_WM_STATE_ABOVE (x::intern_atom (conn, true, "_NET_WM_STATE_ABOVE")->atom),
      _NET_WM_STATE_BELOW (x::intern_atom (conn, true, "_NET_WM_STATE_BELOW")->atom),
      _NET_WM_STATE_DEMANDS_ATTENTION (x::intern_atom (conn, true, "_NET_WM_STATE_DEMANDS_ATTENTION")->atom),
      _NET_WM_ACTION_MOVE (x::intern_atom (conn, true, "_NET_WM_ACTION_MOVE")->atom),
      _NET_WM_ACTION_RESIZE (x::intern_atom (conn, true, "_NET_WM_ACTION_RESIZE")->atom),
      _NET_WM_ACTION_MINIMIZE (x::intern_atom (conn, true, "_NET_WM_ACTION_MINIMIZE")->atom),
      _NET_WM_ACTION_SHADE (x::intern_atom (conn, true, "_NET_WM_ACTION_SHADE")->atom),
      _NET_WM_ACTION_STICK (x::intern_atom (conn, true, "_NET_WM_ACTION_STICK")->atom),
      _NET_WM_ACTION_MAXIMIZE_HORZ (x::intern_atom (conn, true, "_NET_WM_ACTION_MAXIMIZE_HORZ")->atom),
      _NET_WM_ACTION_MAXIMIZE_VERT (x::intern_atom (conn, true, "_NET_WM_ACTION_MAXIMIZE_VERT")->atom),
      _NET_WM_ACTION_FULLSCREEN (x::intern_atom (conn, true, "_NET_WM_ACTION_FULLSCREEN")->atom),
      _NET_WM_ACTION_CHANGE_DESKTOP (x::intern_atom (conn, true, "_NET_WM_ACTION_CHANGE_DESKTOP")->atom),
      _NET_WM_ACTION_CLOSE (x::intern_atom (conn, true, "_NET_WM_ACTION_CLOSE")->atom),
      _NET_WM_ACTION_ABOVE (x::intern_atom (conn, true, "_NET_WM_ACTION_ABOVE")->atom),
      _NET_WM_ACTION_BELOW (x::intern_atom (conn, true, "_NET_WM_ACTION_BELOW")->atom),
      WM_DELETE_WINDOW (x::intern_atom (conn, true, "WM_DELETE_WINDOW")->atom),
      WM_CHANGE_STATE (x::intern_atom (conn, true, "WM_CHANGE_STATE")->atom),
      WM_STATE (x::intern_atom (conn, true, "WM_STATE")->atom),
      WM_CHANGE_STATE_ICONIC (XCB_ICCCM_WM_STATE_ICONIC),
      WM_CHANGE_STATE_NORMAL (XCB_ICCCM_WM_STATE_NORMAL)
    { }
};

// =========================================================

} } // namespace x

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_XCB_DEF_H_
