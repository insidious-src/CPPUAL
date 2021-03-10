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

#ifndef CPPUAL_UI_VIEW_MANAGER_H_
#define CPPUAL_UI_VIEW_MANAGER_H_
#ifdef __cplusplus

#include <thread>
#include <chrono>
#include <memory>
#include <cppual/flags.h>
#include <cppual/resource.h>
#include <cppual/gfx/coord.h>
#include <cppual/gfx/color.h>
#include <cppual/gfx/dsp_details.h>
#include <cppual/ui/display.h>

namespace cppual { namespace Ui {

enum WindowFlag
{
    Frame       = 1 <<  0,
    ThinFrame   = 1 <<  0 | 1 << 1,
    Resizable   = 1 <<  2,
    Maximize    = 1 <<  5,
    Help        = 1 <<  6,
    Transparent = 1 <<  7,
    TopMost     = 1 <<  8,
    Taskbar     = 1 <<  9,
    Pager       = 1 << 10,
    WindowHints = Resizable | Maximize | Taskbar | Pager,
    DialogHints = Resizable | Maximize | Taskbar,
    ToolHints   = ThinFrame
};

class   IPlatformWindow;
typedef std::shared_ptr<IPlatformWindow> shared_window;
typedef std::weak_ptr  <IPlatformWindow> weak_window  ;
typedef BitSet         <WindowFlag     > WindowFlags  ;

// ====================================================

class IPlatformWindow : public Resource <IDisplay*, Graphics::Element>
{
public:
    using Resource<IDisplay*, Graphics::Element>::Resource;

    typedef shared_window const& const_pointer;
    typedef std::size_t          size_type    ;
    typedef std::string          string_type  ;

    virtual weak_window owner () const = 0;
    virtual WindowFlags flags () const = 0;
    virtual string_type title () const = 0;
    virtual void        setTitle (string_type const&) = 0;
    virtual void        setShaded (bool) = 0;
    virtual bool        isShaded () = 0;
    virtual void        setModal (bool) = 0;
    virtual bool        isModal () = 0;
    virtual void        setFullscreen (bool) = 0;
    virtual bool        isFullscreen () = 0;
    virtual void        setMaximized (bool) = 0;
    virtual bool        isMaximized () = 0;
    virtual void        setMinimized (bool) = 0;
    virtual bool        isMinimized () = 0;
    virtual void        setFlags (WindowFlags) = 0;
    virtual void        flash (uint count = 1) = 0;
    virtual Rect        geometry () const = 0;
    virtual u32         screen () const = 0;
    virtual bool        isMapped () const = 0;
    virtual void        setOwner (const_pointer) = 0;
    virtual void        setGeometry (Rect const&) = 0;
    virtual void        raise () = 0;
    virtual void        lower () = 0;
    virtual void        move (point2i) = 0;
    virtual void        map () = 0;
    virtual void        unmap () = 0;

    std::thread::id thread_id () const noexcept
    { return m_thread; }

private:
    std::thread::id m_thread { std::this_thread::get_id () };
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_VIEW_MANAGER_H_
