/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include "xinput.h"
#include "xsurface.h"
#include "xbackend.h"

using namespace cppual::Ui;

namespace cppual { namespace Platform {

struct XPlatform final : public Factory
{
	shared_queue      createQueueObject ();
	shared_window createWindow  (Rect const&, u32, IDisplay*);
	shared_display    connectDisplay    (cchar*);
};

shared_display XPlatform::connectDisplay (cchar* pDeviceName)
{
	return shared_display (new XDisplay (pDeviceName? pDeviceName : getenv ("DISPLAY")));
}

shared_queue XPlatform::createQueueObject ()
{
	return shared_queue (new XQueue);
}

shared_window XPlatform::createWindow (Rect const& gRect,
											   u32         nScreen,
											   IDisplay*   pDisplay)
{
	return shared_window (new XWindow (gRect, nScreen, pDisplay));
}

} } // namespace Platform

// =========================================================

using cppual::Platform::XPlatform;
using cppual::Platform::shared_manager;

extern "C" int module_main (shared_manager& instance)
{
	instance.reset (new XPlatform);
	return instance == nullptr;
}

#endif // OS_GNU_LINUX or OS_BSD
