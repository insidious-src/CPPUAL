/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#include <cppual/platform/manager.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include "xbackend.h"
#include "xinput.h"
#include "xsurface.h"

using cppual::Input::XQueue;
using namespace cppual::Ui;

namespace cppual { namespace Platform {

struct XPlatform final : public IService
{
	shared_queue      createQueueObject ();
	shared_renderable createRenderable (Rect const&, u32, IDisplay*);
	shared_display    connectDisplay (cchar*);
};

shared_display XPlatform::connectDisplay (cchar* pDeviceName)
{
	return std::move (shared_display (new XDisplay (pDeviceName)));
}

shared_queue XPlatform::createQueueObject ()
{
	return std::move (shared_queue (new XQueue));
}

shared_renderable XPlatform::createRenderable (Rect const&  gRect,
											   u32          nScreen,
											   IDisplay* pDisplay)
{
	return shared_renderable (new XRenderable (gRect, nScreen, pDisplay));
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
