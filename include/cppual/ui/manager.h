/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_PLATFORM_MANAGER_H_
#define CPPUAL_PLATFORM_MANAGER_H_
#ifdef __cplusplus

#include <cppual/process/module.h>
#include <cppual/ui/display.h>
#include <cppual/ui/queue.h>
#include <cppual/ui/wm.h>

namespace cppual { namespace Ui { namespace Platform {

struct  Factory;
typedef shared_ptr<Factory> shared_manager;

// =========================================================

struct Factory : public NonCopyableVirtual
{
    typedef Process::Module module_type;

    virtual shared_window createWindow (Rect const& rect,
                                        u32         screen  = 0,
                                        IDisplay*   display = IDisplay::instance ()) = 0;

    virtual shared_display connectDisplay (cchar* name = nullptr) = 0;
    virtual shared_queue   createQueueInstance () = 0;

    static module_type& module   ();
    static Factory*     instance ();
    static bool         hasValidInstance () noexcept;
};

} } } // namespace Platform

#endif // __cplusplus
#endif // CPPUAL_PLATFORM_MANAGER_H_
