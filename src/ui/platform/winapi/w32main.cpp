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

#include <cppual/decl.h>

#ifdef OS_WINDOWS

#include <cppual/ui/manager.h>

#include "w32input.h"
#include "w32window.h"
#include "w32backend.h"

namespace cppual { namespace Ui { namespace Platform {

struct Win32Factory final : Factory
{
    shared_queue   createQueueInstance ();
    shared_display connectDisplay      (string_type const&);
    shared_window  createWindow        (Rect const&, u32, IDisplay*);
};

shared_display Win32Factory::connectDisplay (string_type const&)
{
    return shared_display (new Win32Display);
}

shared_queue Win32Factory::createQueueInstance ()
{
    return shared_queue (new Win32Queue);
}

shared_window Win32Factory::createWindow (Rect const& gRect, u32 nScreen, IDisplay* pDisplay)
{
    return shared_window (new Win32Window (gRect, nScreen, pDisplay));
}

} } } // namespace Platform

// =========================================================

using cppual::Ui::Platform::Win32Factory;
using cppual::Process::Plugin           ;
using cppual::Memory::MemoryResource    ;
using cppual::Memory::Allocator         ;

extern "C" Plugin* plugin_main (MemoryResource* rc)
{
    static Plugin plugin;

    plugin.name     = "Win32Factory"    ;
    plugin.desc     = "Win32 UI Factory";
    plugin.provides = "Ui::Factory"     ;
    plugin.verMajor = 1                 ;
    plugin.verMinor = 0                 ;

    plugin.iface    = std::static_pointer_cast<void>
            (std::allocate_shared<Win32Factory>(Allocator<Win32Factory>(*rc)));

    return &plugin;
}

#endif // OS_WINDOWS
