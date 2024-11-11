/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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
#include <cppual/memory/stacked.h>

#include <array>

namespace cppual { namespace compute {

namespace { // optimize for internal unit usage

class initializer final
{
private:
    typedef factories::value_type                 value_type                ;
    typedef factories::vector_type                vector_type               ;
    typedef factories::vector_reference           vector_reference          ;
    typedef factories::vector_const_reference     vector_const_reference    ;
    typedef factories::lib_vector                 lib_vector                ;
    typedef factories::lib_vector_reference       lib_vector_reference      ;
    typedef factories::lib_vector_const_reference lib_vector_const_reference;

    factory::manager_type manager        ;
    vector_type           local_factories;

    inline static lib_vector_reference platform_names () noexcept
    {
        static std::array<cchar*, 2> ret_vec
        {
            "libcppual-compute-opencl",
            "libcppual-compute-vulkan"
        };

        return ret_vec;
    }

    inline static initializer& platforms () noexcept
    {
        static initializer init;
        return init;
    }

    inline initializer ()
    {
        static lib_vector_reference vec = platform_names ();

        for (auto i = 0U; i < vec.size (); ++i)
        {
            if (manager.load_plugin (vec[i]))
            {
                local_factories.push_back (manager.plugin (vec[i]).iface ());
            }
        }
    }

public:
    inline static vector_reference instances () noexcept
    {
        return platforms ().local_factories;
    }
};

} // anonymous namespace

factories::vector_reference factories::instances ()
{
    return initializer::instances ();
}

bool factories::has_valid_instance () noexcept
{
    return !initializer::instances ().empty ();
}

} } // namespace Compute
