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

#include <cppual/compute/backend_iface.h>
#include <cppual/process/plugin.h>

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

class Initializer final
{
private:
    Factory::manager_type mgr    ;
    shared_factory        factory;

    constexpr cchar* platform_name () noexcept
    {
        return "libcppual-compute-opencl";
    }

    inline static Initializer& platform () noexcept
    {
        static Initializer init;
        return init;
    }

    inline Initializer ()
    {
        if (mgr.load_plugin(platform_name())) factory = mgr.plugin(platform_name()).interface();
    }

public:
    inline static shared_factory instance () noexcept
    {
        return platform ().factory;
    }
};

} // anonymous namespace

shared_factory Factory::instance ()
{
    return Initializer::instance ();
}

bool Factory::hasValidInstance () noexcept
{
    return Initializer::instance () != nullptr;
}

} } // namespace Compute
