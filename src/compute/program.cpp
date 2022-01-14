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

#include <cppual/compute/program.h>
#include <cppual/compute/backend_iface.h>

#include "backend/opencl.h"

namespace cppual { namespace Compute {

namespace {

//inline Program::handle_type createProgramFromSource (Behaviour const& context,
//                                                     std::vector<string> const& source)
//{
//    if (!source.empty ())
//    {
//        std::vector<Program::size_type> sizes;

//        sizes.reserve(source.size());

//        for (auto i = 0U; i < source.size (); ++i) sizes.push_back(source[i].size ());

//        auto sources = source.front ().c_str ();

//         return ::clCreateProgramWithSource (context.handle ().get<CL::context_type> (),
//                                           static_cast<uint> (source.size ()),
//                                           &sources,
//                                           sizes,
//                                           nullptr);
//    }

//    return nullptr;
//}

} // anonymous namespace

// =========================================================

//Program::Program (Behaviour const&, string const&)
//: Object ()
//{
//}

//Program::Program (Behaviour const& context, vector<string> const& source)
//: Object (createProgramFromSource (context, source))
//{
//}

} } // namespace Compute
