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
#include <cppual/ui/manager.h>

namespace cppual { namespace Ui { namespace Platform {

namespace { // optimize for internal unit usage

class Initializer final
{
private:
    Factory::manager_type mgr    ;
    shared_manager        factory;

    inline cchar* display_server () noexcept
    {
    #   if defined OS_GNU_LINUX or defined OS_BSD
            return std::getenv ("WAYLAND_DISPLAY") ? "libcppual-ui-wayland" :
                                                     "libcppual-ui-xorg"    ;
    #   elif defined OS_WINDOWS
            return "libcppual-ui-win";
    #   endif
    }

    inline static Initializer& platform () noexcept
    {
        static Initializer init;
        return init;
    }

public:
    inline Initializer ()
    {
        if (mgr.load_plugin(display_server())) factory = mgr.construct(display_server());
    }

    inline static shared_manager& instance () noexcept
    {
        return platform ().factory;
    }
};

} // anonymous namespace

// ====================================================

Factory* Factory::instance ()
{
    return Initializer::instance ().get ();
}

bool Factory::hasValidInstance () noexcept
{
    return Initializer::instance () != nullptr;
}

} } } // namespace Platform
