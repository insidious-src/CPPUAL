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

#include <cppual/decl.h>

#ifdef OS_WINDOWS

#include <cppual/ui/manager.h>
#include "w32input.h"
#include "w32window.h"
#include "w32backend.h"

using namespace cppual::Ui;

namespace cppual { namespace Platform {

struct Win32Platform final : public Factory
{
    shared_queue   createQueueObject ();
    shared_display connectDisplay    (cchar*);
    shared_window  createWindow      (Rect const&, u32, IDisplay*);
};

shared_display Win32Platform::connectDisplay (cchar*)
{
    return shared_display (new Win32Display);
}

shared_queue Win32Platform::createQueueObject ()
{
    return shared_queue (new Win32Queue);
}

shared_window Win32Platform::createWindow (Rect const& gRect,
                                           u32         nScreen,
                                           IDisplay*   pDisplay)
{
    return shared_window (new Win32Window (gRect, nScreen, pDisplay));
}

} } // namespace Platform

// =========================================================

using cppual::Platform::Win32Platform;
using cppual::Platform::shared_manager;

extern "C" int module_main (shared_manager& instance)
{
    instance.reset (new Win32Platform);
    return instance == nullptr;
}

#endif // OS_WINDOWS
