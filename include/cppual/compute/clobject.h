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

#ifndef CPPUAL_CL_OBJECT_H_
#define CPPUAL_CL_OBJECT_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/resource.h>

namespace cppual { namespace Compute {

class Instance;
class Device;
class Behaviour;
class DeviceQueue;
class DeviceSurface;
class MemoryChunk;
class Program;
class Kernel;
class Event;

// =========================================================

template <class T>
class Object
{
public:
    typedef Handle pointer;
    typedef u32    uint_type;
    typedef int32  int_type;

    Object (Object&&) noexcept = default;
    Object (Object const& rhs) noexcept;
    Object& operator = (Object&&) = default;
    Object& operator = (Object const& rhs) noexcept;
    ~Object () noexcept;

    constexpr Object () noexcept : m_object () { }
    constexpr pointer handle () const noexcept { return m_object; }

    constexpr Object (pointer obj) noexcept : m_object (obj)
    { }

private:
    pointer m_object;
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_CL_OBJECT_H_
