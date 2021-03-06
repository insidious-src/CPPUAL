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

#ifndef CPPUAL_COMPUTE_INSTANCE_H_
#define CPPUAL_COMPUTE_INSTANCE_H_
#ifdef __cplusplus

#include <vector>
#include <cppual/types.h>
#include <cppual/resource.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/devtask.h>

using std::vector;

namespace cppual { namespace Compute {

class Program : public Object <Resource::CommandSequence>
{
public:
    Program ();
    Program (Behaviour const&, string const& binary_path);
    Program (Behaviour const&, vector<string> const& source);
    Program (Program&&);
    Program (Program const&);
    Program& operator = (Program&&);
    Program& operator = (Program const&);

    void bind  (DeviceQueue const& queue);
    bool build ();

    static Program* current () noexcept;
};

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_INSTANCE_H_
