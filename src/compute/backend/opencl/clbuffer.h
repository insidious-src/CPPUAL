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
 *memory
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_COMPUTE_OPENCL_BUFFER_H_
#define CPPUAL_COMPUTE_OPENCL_BUFFER_H_
#ifdef __cplusplus

#include "opencl.h"

namespace cppual { namespace Compute { namespace CL {

// =========================================================

class Buffer final : public Interface<ResourceType::Buffer>
{
public:
    typedef std::vector<Buffer::pointer> device_ids_vector;
    typedef Factory::device_vector       device_vector    ;

    Buffer (pointer handle) noexcept : Interface (handle) { }

private:
    template<typename T>
    T get_info (memory_info_type info) const
    {
        return get_object_info<T> (::clGetMemObjectInfo, handle(), info);
    }
};

// =========================================================

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OPENCL_BUFFER_H_
