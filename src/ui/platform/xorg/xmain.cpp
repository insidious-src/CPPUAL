/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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
    shared_display connectDisplay      (cchar*);
    shared_window  createWindow        (Rect const&, u32, IDisplay*);
};

shared_display XFactory::connectDisplay (cchar* pDeviceName)
{
    return shared_display (new XDisplay (pDeviceName));
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

using cppual::Ui::Platform::XFactory;
using cppual::Ui::Platform::shared_manager;

extern "C" int plugin_main (shared_manager& instance)
{
    instance.reset (new XFactory);
    return instance == nullptr;
}

#endif // OS_GNU_LINUX or OS_BSD
