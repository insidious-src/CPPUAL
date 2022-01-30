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

#include <cppual/ui/wm.h>
#include <cppual/gfx/draw.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <wayland-client-protocol.h>

namespace cppual { namespace ui {

class WlSurface final : public platform_wnd_interface
{
public:
    WlSurface  () = delete;
    ~WlSurface () noexcept { }

    WlSurface (rect const& gRect, u32 nScreen, shared_display pDisplay) noexcept
    : platform_wnd_interface(pDisplay, nullptr),
      _M_gRect        (gRect),
      _M_uScreen      (nScreen)
    { }

    bool isMapped    () const noexcept { return false; }
    void setOwner    (const_pointer) noexcept { }
    void move        (point2i) noexcept { }
    void setGeometry (rect const&) noexcept { }
    void raise () noexcept { }
    void lower () noexcept { }
    void map   () noexcept { }
    void unmap () noexcept { }

    weak_window owner    () const noexcept { return weak_window(); }
    rect        geometry () const noexcept { return _M_gRect      ; }
    u32         screen   () const noexcept { return _M_uScreen    ; }

    window_flags flags () const { return window_flags (); }
    void setFlags (window_flags) { }

    string_type title () const { return string_type (); }
    void setTitle (string_type const&) { }
    void setShaded (bool) { }
    bool isShaded () { return false; }
    void setModal (bool) { }
    bool isModal () { return false; }
    void setFullscreen (bool) { }
    bool isFullscreen () { return false; }
    void setMaximized (bool) { }
    bool isMaximized () { return false; }
    void setMinimized (bool) { }
    bool isMinimized () { return false; }
    void setVisibleInTaskbar (bool) { }
    bool isVisibleInTaskbar () { return false; }
    void setVisibleInPager (bool) { }
    bool isVisibleInPager () { return false; }
    void flash (uint) { }

private:
    rect _M_gRect;
    u32  _M_uScreen;
};

} } // namespace Ui

#endif // OS_GNU_LINUX
