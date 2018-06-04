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
#include <cppual/ui/display.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <wayland-client-protocol.h>

namespace cppual { namespace Ui {

class SHARED_API WlBackend final : public IDisplay
{
public:
    WlBackend  () = delete;
    ~WlBackend () noexcept;

    string_type name        () const noexcept { return m_gName;        }
    uint        screenCount () const noexcept { return m_nScreenCount; }

    void flush () noexcept
    { ::wl_display_flush (native<wl_display> ()); }

    WlBackend (cchar* pName) noexcept
    : IDisplay       (::wl_display_connect (pName), nullptr),
      m_nScreenCount (),
      m_gName        (pName)
    { }

private:
    uint        m_nScreenCount;
    string_type m_gName;
};

WlBackend::~WlBackend () noexcept
{ if (native ()) ::wl_display_disconnect (native<wl_display> ()); }

} } // namespace Graphics

#endif // OS_GNU_LINUX or OS_BSD
