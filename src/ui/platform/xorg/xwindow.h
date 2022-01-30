/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_X_SURFACE_H_
#define CPPUAL_PLATFORM_X_SURFACE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>
#include <cppual/ui/wm.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

namespace cppual { namespace ui {

class SHARED_API XWindow final : public platform_wnd_interface
{
public:
    XWindow () = delete;
    XWindow (rect const& rect, u32 screen, shared_display display) noexcept;
    ~XWindow() noexcept;

    string_type title () const noexcept;
    void        setTitle (string_type const&) noexcept;
    void        setShaded (bool) noexcept;
    bool        isShaded () noexcept;
    void        setModal (bool) noexcept;
    bool        isModal () noexcept;
    void        setFullscreen (bool) noexcept;
    bool        isFullscreen () noexcept;
    void        setMaximized (bool) noexcept;
    bool        isMaximized () noexcept;
    void        setMinimized (bool) noexcept;
    bool        isMinimized () noexcept;
    void        setVisibleInTaskbar (bool) noexcept;
    bool        isVisibleInTaskbar () noexcept;
    void        setVisibleInPager (bool) noexcept;
    bool        isVisibleInPager () noexcept;
    void        flash (uint) noexcept;
    rect        geometry () const;
    bool        isMapped () const;
    void        setOwner (const_pointer);
    void        setGeometry (rect const&);
    void        raise ();
    void        lower ();
    void        move (point2i);
    void        map ();
    void        unmap ();
    void        setFlags (window_flags) noexcept;

    window_flags flags  () const noexcept { return _M_eFlags     ; }
    weak_window owner  () const noexcept { return  weak_window(); }
    u32         screen () const noexcept { return _M_uScreen    ; }

private:
    window_flags _M_eFlags ;
    u32         _M_uScreen;
};

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_X_SURFACE_H_
