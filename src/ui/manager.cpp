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

#include <cppual/ui/manager.h>

#include <iostream>

namespace cppual { namespace ui { namespace platform {

// ====================================================

namespace { // optimize for internal unit usage

class initializer final
{
private:
    factory::manager_type mgr          ;
    shared_manager        local_factory;

    inline static cchar* platform_name () noexcept
    {
    #   if defined OS_GNU_LINUX or defined OS_BSD
            return std::getenv ("WAYLAND_DISPLAY") ? "libcppual-ui-wayland" :
                                                     "libcppual-ui-xorg"    ;
    #   elif defined OS_WINDOWS
            return "libcppual-ui-win";
    #   elif defined OS_ANDROID
            return "libcppual-ui-android";
    #   endif
    }

public:
    inline initializer ()
    {
        if (mgr.load_plugin (platform_name ())) local_factory = mgr.plugin (platform_name ()).interface ();
    }

    inline operator shared_manager () const
    {
        return local_factory;
    }
};

// ====================================================

inline static shared_manager platform_instance ()
{
    static auto inst = initializer();
    return inst;
}

} // anonymous namespace

// ====================================================

shared_manager factory::instance ()
{
    return platform_instance ();
}

bool factory::has_valid_instance () noexcept
{
    return platform_instance () != nullptr;
}

} } } // namespace Platform
