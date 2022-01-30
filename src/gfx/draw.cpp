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

#include <cppual/gfx/draw.h>
#include <cppual/process/plugin.h>

namespace cppual { namespace gfx {

// ====================================================

namespace { namespace internal {

typedef process::plugin_manager<drawable2d_interface> manager2d_type      ;
typedef process::plugin_manager<drawable3d_interface> manager3d_type      ;
typedef process::plugin_manager<context_interface>    manager_context_type;
typedef context_interface                             context_type        ;
typedef context_interface*                            context_pointer     ;

inline context_pointer& current () noexcept
{
    static thread_local context_pointer current_dc;
    return current_dc;
}

inline manager2d_type& manager2d () noexcept
{
    static manager2d_type drawable_mgr;
    return drawable_mgr;
}

inline manager3d_type& manager3d () noexcept
{
    static manager3d_type drawable_mgr;
    return drawable_mgr;
}

inline manager_context_type& manager_context() noexcept
{
    static manager_context_type context_mgr;
    return context_mgr;
}

} } // anonymous namespace Internal

// ====================================================

context_interface* context_interface::current () noexcept
{
    return internal::current ();
}

void context_interface::acquire (context_interface* pContext) noexcept
{
    if (internal::current () != pContext)
    {
        if (internal::current () && pContext) internal::current ()->release ();

        internal::current () = pContext;
    }
}

// ====================================================

shared_drawable2d drawable_factory::create2d (string_type const& gName)
{
    return internal::manager2d ().plugin (gName).interface ();
}

shared_drawable3d drawable_factory::create3d (string_type const& gName)
{
    return internal::manager3d ().plugin (gName).interface ();
}

shared_context context_factory::create (string_type const& gName)
{
    return internal::manager_context().plugin (gName).interface ();
}

} } // namespace Graphics
