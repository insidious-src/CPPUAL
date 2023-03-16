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

#ifndef CPPUAL_UI_VIEW_MANAGER_H_
#define CPPUAL_UI_VIEW_MANAGER_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/resource.h>
#include <cppual/gfx/coord.h>
#include <cppual/ui/display.h>

#include <thread>
#include <chrono>
#include <memory>

namespace cppual { namespace ui {

enum window_flag
{
    Frame       = 1 <<  0,
    ThinFrame   = 1 <<  0 | 1 << 1,
    Resizable   = 1 <<  2,
    Maximize    = 1 <<  5,
    Help        = 1 <<  6,
    Transparent = 1 <<  7,
    TopMost     = 1 <<  8,
    BelowOthers = 1 <<  9,
    Taskbar     = 1 << 10,
    Pager       = 1 << 11,
    WindowHints = Resizable | Maximize | Taskbar | Pager,
    DialogHints = Resizable | Maximize | Taskbar,
    ToolHints   = ThinFrame
};

class   platform_wnd_interface;
typedef std::shared_ptr<platform_wnd_interface> shared_window;
typedef std::weak_ptr  <platform_wnd_interface> weak_window  ;
typedef bitset         <window_flag           > window_flags ;

// ====================================================

class SHARED_API platform_wnd_interface : public resource <shared_display, resource_handle>
{
public:
    using resource<shared_display, resource_handle>::resource;

    typedef shared_window const& const_pointer;
    typedef std::size_t          size_type    ;
    typedef string               string_type  ;

    virtual weak_window  owner () const = 0;
    virtual window_flags flags () const = 0;
    virtual string_type  title () const = 0;
    virtual void         set_title (string_type const&) = 0;
    virtual void         set_shaded (bool) = 0;
    virtual bool         is_shaded () const = 0;
    virtual void         set_modal (bool) = 0;
    virtual bool         is_modal () const = 0;
    virtual void         set_fullscreen (bool) = 0;
    virtual bool         is_fullscreen () const = 0;
    virtual void         set_maximized (bool) = 0;
    virtual bool         is_maximized () const = 0;
    virtual void         set_minimized (bool) = 0;
    virtual bool         is_minimized () const = 0;
    virtual void         set_flags (window_flags) = 0;
    virtual void         set_visible_in_taskbar (bool) = 0;
    virtual bool         is_visible_in_taskbar () const = 0;
    virtual void         set_visible_in_pager (bool) = 0;
    virtual bool         is_visible_in_pager () const = 0;
    virtual void         flash (uint count = 1) = 0;
    virtual rect         geometry () const = 0;
    virtual u32          screen () const = 0;
    virtual bool         is_mapped () const = 0;
    virtual void         set_owner (const_pointer) = 0;
    virtual void         set_geometry (rect const&) = 0;
    virtual void         raise () = 0;
    virtual void         lower () = 0;
    virtual void         move (point2i) = 0;
    virtual void         map () = 0;
    virtual void         map_minimized () = 0;
    virtual void         map_maximized () = 0;
    virtual void         unmap () = 0;
    virtual void         keep_above (bool) = 0;
    virtual bool         is_above () const = 0;
    virtual void         keep_below (bool) = 0;
    virtual bool         is_below () const = 0;

    std::thread::id thread_id () const noexcept
    { return _M_thread; }

private:
    std::thread::id _M_thread { std::this_thread::get_id () };
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIEW_MANAGER_H_
