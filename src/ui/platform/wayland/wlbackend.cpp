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
#include <cppual/ui/display.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <wayland-client-protocol.h>

namespace cppual { namespace ui {

class SHARED_API WlDisplay final : public display_interface
{
public:
    WlDisplay () = delete;

    ~WlDisplay () noexcept
    { if (native ()) ::wl_display_disconnect (native<::wl_display> ()); }


    string_type name        () const noexcept { return _M_gName;        }
    uint        screen_count () const noexcept { return _M_nScreenCount; }

    void flush () noexcept
    { ::wl_display_flush (native<::wl_display> ()); }

    WlDisplay (string_type const& strName) noexcept
    : display_interface       (::wl_display_connect (strName.c_str()), nullptr),
      _M_nScreenCount (),
      _M_gName        (strName)
    { }

private:
    uint        _M_nScreenCount;
    string_type _M_gName;
};

} } // namespace Graphics

#endif // OS_GNU_LINUX or OS_BSD
