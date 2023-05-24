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

#ifndef CPPUAL_COMPUTE_OPENCL_EVENT_H_
#define CPPUAL_COMPUTE_OPENCL_EVENT_H_
#ifdef __cplusplus

#include "opencl.h"

namespace cppual { namespace compute { namespace cl {

// =========================================================

class event final : public event_object
{
public:
    typedef event_object                base_type        ;
    typedef std::vector<event::pointer> device_ids_vector;
    typedef factory::device_vector      device_vector    ;

    event (pointer handle) noexcept : base_type (handle) { }

private:
    template<typename T>
    T get_info (event_info_type info) const
    {
        return get_object_info< vector<T> > (::clGetEventInfo, handle(), info);
    }
};

// =========================================================

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OPENCL_EVENT_H_
