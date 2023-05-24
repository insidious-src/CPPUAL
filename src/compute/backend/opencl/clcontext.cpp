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
 *memory
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "clcontext.h"

#include <iostream>

namespace cppual { namespace compute { namespace cl {

// =========================================================

namespace {



}

// =========================================================

context::context (device_vector const& devs, context_props_type* props)
: base_type (::clCreateContext (props,
                                static_cast<::cl_uint> (devs.size ()),
                                &device::get_device_ids (devs)[0],
                                nullptr,
                                nullptr,
                                &_M_error))
{
    if (_M_error != success)
    {
        auto const error = opencl_error (_M_error);

        std::cerr << error.error_string () << std::endl;

        throw error;
    }
}

// =========================================================

} } } // namespace CL