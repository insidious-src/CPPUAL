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
 *memory
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_COMPUTE_OPENCL_IMAGE_H_
#define CPPUAL_COMPUTE_OPENCL_IMAGE_H_
#ifdef __cplusplus

#include "opencl.h"

namespace cppual::compute::cl {

// =========================================================

class image final : public image_interface
{
public:
    typedef image                 self_type  ;
    typedef image_interface       base_type  ;
    typedef CLObject<memory_type> value_type ;
    typedef value_type*           pointer    ;
    typedef std::size_t           size_type  ;
    typedef resource_handle       handle_type;

    constexpr image (pointer handle) noexcept : base_type (handle) { }

    constexpr pointer handle () const noexcept
    { return base_type::handle<pointer> (); }

private:
    template <typename T>
    T get_info (image_info_type info) const
    {
        return get_object_info< dyn_array<T> > (::clGetImageInfo, handle(), info);
    }
};

// =========================================================

} // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OPENCL_IMAGE_H_
