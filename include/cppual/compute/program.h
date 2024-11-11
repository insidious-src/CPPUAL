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

#ifndef CPPUAL_COMPUTE_INSTANCE_H_
#define CPPUAL_COMPUTE_INSTANCE_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/resource.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/devtask.h>
#include <cppual/compute/behaviour.h>

#include <vector>

namespace cppual { namespace compute {

class program : public object<resource_type::program>
{
public:
    typedef std::size_t size_type;

    program ();
    program (behaviour const&, string const& binary_path);
    program (behaviour const&, std::vector<string> const& source);
    program (program&&);
    program (program const&);
    program& operator = (program&&);
    program& operator = (program const&);

    void bind  (device_queue const& queue);
    bool build ();

    static program* current () noexcept;
};

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_INSTANCE_H_
