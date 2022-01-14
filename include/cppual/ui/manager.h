/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_MANAGER_H_
#define CPPUAL_PLATFORM_MANAGER_H_
#ifdef __cplusplus

#include <cppual/process/plugin.h>
#include <cppual/ui/display.h>
#include <cppual/ui/queue.h>
#include <cppual/ui/wm.h>
#include <cppual/string.h>

namespace cppual { namespace Ui { namespace Platform {

struct  Factory;
typedef std::shared_ptr<Factory> shared_manager;

// =========================================================

struct Factory : public NonCopyableVirtual
{
    typedef Process::PluginManager<Factory> manager_type;
    typedef string                          string_type ;

    virtual shared_window createWindow (Rect const& rect,
                                        u32         screen  = 0,
                                        IDisplay*   display = IDisplay::primary ()) = 0;

    virtual shared_display connectDisplay (string_type const& name = string_type()) = 0;
    virtual shared_queue   createQueueInstance () = 0;

    static shared_manager instance ();
    static bool           hasValidInstance () noexcept;
};

} } } // namespace Platform

#endif // __cplusplus
#endif // CPPUAL_PLATFORM_MANAGER_H_
