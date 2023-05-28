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

#include <cppual/ui/manager.h>
#include <cppual/memory/stacked.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#define  API_EXPORT
#include "wlinput.h"
#include "wlsurface.cpp"
#include "wlbackend.cpp"

namespace cppual { namespace ui { namespace platform {

struct wl_factory final : factory
{
    shared_queue   createQueueInstance ();
    shared_display connectDisplay      (string_type const&);
    shared_window  createWindow        (rect const&, u32, shared_display);
};

shared_display wl_factory::connectDisplay (string_type const& name)
{
    return shared_display (new wl_display (name));
}

shared_queue wl_factory::createQueueInstance ()
{
    return shared_queue ();
}

shared_window wl_factory::createWindow (rect const& gRect, u32 nScreen, shared_display pDisplay)
{
    return shared_window (new wl_surface (gRect, nScreen, pDisplay));
}

} } } // namespace Platform

// =========================================================

using cppual::ui::platform::wl_factory ;
using cppual::process::plugin_vars    ;
using cppual::memory::memory_resource ;
using cppual::memory::stacked_resource;
using cppual::memory::allocator       ;
using cppual::memory::allocate_shared ;

extern "C" plugin_vars* plugin_main (memory_resource* /*rc*/)
{
    static char buffer[sizeof (wl_factory) + memory_resource::max_align * 2];
    static stacked_resource static_resource (buffer, sizeof (buffer));
    static plugin_vars plugin;

    plugin.name     = "wl_factory"         ;
    plugin.desc     = "Wayland UI Factory";
    plugin.provides = "Ui::Factory"       ;
    plugin.verMajor = 1                   ;
    plugin.verMinor = 0                   ;

    plugin.iface    = allocate_shared<wl_factory, void> (&static_resource);

    return &plugin;
}

#endif // OS_GNU_LINUX or OS_BSD
