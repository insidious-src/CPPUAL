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

#ifndef CPPUAL_COMPUTE_OPENCL_DEVICE_H_
#define CPPUAL_COMPUTE_OPENCL_DEVICE_H_
#ifdef __cplusplus

#include "opencl.h"

namespace cppual { namespace Compute { namespace CL {

// =========================================================

class Device final : public Interface<ResourceType::Device>
{
public:
    typedef vector<Device::pointer> device_ids_vector;
    typedef Factory::device_vector  device_vector    ;

    string_type name              () const;
    string_type vendor            () const;
    Profile     profile           () const;
    Backend     backend           () const;
    device_ils  supportedILs      () const;
    DeviceType  devType           () const;
    Version     version           () const;
    size_type   cacheSize         () const;
    size_type   cacheLineSize     () const;
    size_type   localMemorySize   () const;
    size_type   constMemorySize   () const;
    size_type   globalMemorySize  () const;
    u32         computeUnits      () const;
    u64         maxMemoryAllocSize() const;

    Device (pointer handle) noexcept : Interface (handle) { }

private:
    vector<string_type> extensions() const;
    bool supports_extension(string_type const& name) const;
    static device_ids_vector get_device_ids();
    static device_vector     get_devices(Memory::MemoryResource& rc);
    static size_type         count();

    template<typename T>
    T get_info (device_info_type info) const
    {
        return get_object_info<T> (::clGetDeviceInfo, handle(), info);
    }

    friend class Platform ;
    friend class CLFactory;
};

// =========================================================

} } } // namespace CL

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OPENCL_DEVICE_H_
