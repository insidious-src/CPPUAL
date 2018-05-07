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

#include <iostream>
#include <cppual/memory/static.h>
#include <cppual/process/plugin.h>
#include <cppual/gfx/gl/runtime.h>
#include "gldef.h"

using cppual::Memory::StaticPool     ;
using cppual::Memory::StaticAllocator;
using namespace std::string_literals ;

namespace cppual { namespace Graphics { namespace GL {

namespace {

struct Internal
{
    typedef Process::DynLoader                                 module_type   ;
    typedef StaticPool<sizeof (module_type)>                   allocator_type;
    typedef StaticAllocator<module_type, sizeof (module_type)> policy_type   ;

    inline Internal () : allocator (), instance ()
    { /*Driver::load ("libGL"s);*/ }

    inline ~Internal () noexcept
    { /*Driver::unload ();*/ }

    allocator_type allocator;
    module_type*   instance ;

} internal;

} // anonymous

bool Driver::load (string const& gName)
{
    if (internal.instance) unload ();
    Internal::policy_type gAtor (internal.allocator);

    internal.instance = gAtor.allocate (1);
    if (!internal.instance) return false;
    gAtor.construct (internal.instance, gName.c_str ());

    if (!internal.instance->is_attached ())
    {
        std::cout << "Failed to load OpenGL driver "
                  << internal.instance->path () << std::endl;

        unload ();
        return false;
    }

    //internal.instance->get ("glAttachShader", internal.glAttachShader);
    return false;
}

void Driver::unload ()
{
    if (!internal.instance) return;
    internal.instance->~DynLoader ();
    Internal::policy_type (internal.allocator).deallocate (internal.instance, 1);
    internal.instance = nullptr;
}

bool Driver::hasValidInstance () noexcept
{
    return internal.instance;
}

GFXVersion Driver::version ()
{
    return GFXVersion ();
}

GFXVersion Driver::glVersion ()
{
    return GFXVersion ();
}

GFXVersion Driver::glslVersion ()
{
    return GFXVersion ();
}

string Driver::glLabel (StringQuery)
{
    return string ();
}

bool Driver::isVersionSupported (GFXVersion const&)
{
    return false;
}

bool Driver::isExtensionSupported (string const&)
{
    return false;
}

} } } // namespace GL
