/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#ifndef CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#define CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
#ifdef __cplusplus

#include <type_traits>
#include <cppual/types.h>
#include <cppual/flags.h>
#include <cppual/compute/device.h>

namespace cppual { namespace Compute {

class IDeviceManager
{
public:
    enum Type
    {
        Graphics    = 1 << 0,
        GPGPU       = 1 << 1,
        Specialized = 1 << 2,
        Universal   = Graphics | GPGPU | Specialized
    };

    typedef BitSet<Type>    types;
    typedef Device::pointer pointer;

    virtual pointer createObject (Resource type) = 0;
};

// =========================================================

class DeviceRepository
{
public:
    typedef std::size_t size_type;

private:

};

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_BACKEND_INTERFACE_H_
