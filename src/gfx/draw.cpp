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

typedef process::plugin_manager<draw_factory> manager_type   ;
typedef context_interface                     context_type   ;
typedef context_interface*                    context_pointer;

// ====================================================

class initializer final
{
private:
    manager_type   mgr    ;
    shared_factory factory;

    inline static cchar* plugin_name () noexcept
    {
    #   if defined OS_GNU_LINUX || defined OS_BSD || defined OS_ANDROID
            return "libcppual-gfx-module-glx";
    #   elif defined OS_WINDOWS
            return "libcppual-gfx-module-wgl";
    #   endif
    }

public:
    inline initializer ()
    {
        if (mgr.load_plugin (plugin_name ())) factory = mgr.plugin (plugin_name ()).iface ();
    }

    inline operator shared_factory () const
    {
        return factory;
    }
};

// ====================================================

inline static shared_factory plugin_instance ()
{
    static auto inst = initializer ();
    return inst;
}

inline context_pointer& current () noexcept
{
    static thread_local context_pointer current_dc;
    return current_dc;
}


} } // anonymous namespace internal

// ====================================================

resource_interface::~resource_interface ()
{
}

// ====================================================

surface_interface::~surface_interface ()
{
}

// ====================================================

draw_factory::~draw_factory ()
{
}

shared_factory draw_factory::instance ()
{
    return internal::plugin_instance ();
}

bool draw_factory::has_valid_instance () noexcept
{
    return internal::plugin_instance () != nullptr;
}

// ====================================================

context_interface::~context_interface ()
{
}

context_interface* context_interface::current () noexcept
{
    return internal::current ();
}

void context_interface::acquire (context_interface* pContext) noexcept
{
    if (internal::current () != pContext)
    {
        if (internal::current ()) internal::current ()->release ();

        internal::current () = pContext;
    }
}

// ====================================================

} } // namespace gfx
