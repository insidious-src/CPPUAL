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

#include <cppual/ui/manager.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#define  API_EXPORT
#include "xinput.h"
#include "xsurface.h"
#include "xbackend.h"

namespace cppual { namespace Ui { namespace Platform {

struct XFactory final : Factory
{
    shared_queue   createQueueInstance ();
    shared_display connectDisplay      (string_type const&);
    shared_window  createWindow        (Rect const&, u32, IDisplay*);
};

shared_display XFactory::connectDisplay (string_type const& name)
{
    return shared_display (new XDisplay (name));
}

shared_queue XFactory::createQueueInstance ()
{
    return shared_queue (new XQueue);
}

shared_window XFactory::createWindow (Rect const& gRect, u32 nScreen, IDisplay* pDisplay)
{
    return shared_window (new XWindow (gRect, nScreen, pDisplay));
}

} } } // namespace Platform

// =========================================================

using cppual::Process::PluginManager;
using cppual::Ui::Platform::Factory ;
using cppual::Ui::Platform::XFactory;
using cppual::Process::Plugin       ;

extern "C" Plugin plugin_main ()
{
    Plugin plugin;

    static const auto name     = "XFactory"       ;
    static const auto desc     = "Xorg UI Factory";
    static const auto provides = "Ui::Factory"    ;
    static const auto iface    = "create_xfactory";

    plugin.name     = name    ;
    plugin.desc     = desc    ;
    plugin.provides = provides;
    plugin.verMajor = 1       ;
    plugin.verMinor = 0       ;
    plugin.iface    = iface   ;

    return plugin;
}

extern "C" Factory* create_xfactory ()
{
    return new XFactory;
}

#endif // OS_GNU_LINUX or OS_BSD
