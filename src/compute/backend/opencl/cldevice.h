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

#ifndef CPPUAL_COMPUTE_OPENCL_DEVICE_H_
#define CPPUAL_COMPUTE_OPENCL_DEVICE_H_
#ifdef __cplusplus

#include "opencl.h"

namespace cppual { namespace compute { namespace cl {

// =========================================================

class device final : public device_object
{
public:
    typedef vector<pointer>        device_ids_vector;
    typedef factory::device_vector device_vector    ;

    string_type     name                 () const;
    string_type     vendor               () const;
    profile_type    profile              () const;
    backend_type    backend              () const;
    device_ils      supported_ils        () const;
    device_category dev_type             () const;
    version_type    version              () const;
    size_type       cache_size           () const;
    size_type       cache_line_size      () const;
    size_type       local_memory_size    () const;
    size_type       const_memory_size    () const;
    size_type       global_memory_size   () const;
    size_type       max_memory_alloc_size() const;
    u32             compute_units        () const;

    device (pointer handle) noexcept : interface (handle) { }

private:
    vector<string_type> extensions() const;
    bool supports_extension(string_type const& name) const;

    static device_ids_vector get_device_ids();
    static device_ids_vector get_device_ids(device_vector const& devs);
    static device_vector     get_devices(memory::memory_resource& rc);
    static size_type         count();

    template <typename T>
    T get_info (device_info_type info) const
    {
        return get_object_info<T> (::clGetDeviceInfo, handle(), info);
    }

    friend class platform  ;
    friend class context   ;
    friend class cl_factory;
};

// =========================================================

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OPENCL_DEVICE_H_
