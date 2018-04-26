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

#ifndef CPPUAL_COMPUTE_OBJECT_H_
#define CPPUAL_COMPUTE_OBJECT_H_
#ifdef __cplusplus

#include <type_traits>
#include <cppual/types.h>
#include <cppual/resource.h>

namespace cppual { namespace Compute {

class HostConnection; // application instance info
class Device;
class Behaviour;      // device and queue load balancing
class MixedQueue;     // host and multiple devices
class HostQueue;      // thread queue
class DeviceQueue;
class DeviceSurface;
class Pipeline;
class Image;
class Render;         // render passes and states
class Command;
class CommandSequence;
class Event;

// =========================================================

enum class Resource : unsigned short
{
    Buffer          = 1 << 0,
    CommandSequence = 1 << 1,
    Image           = 1 << 2,
    Pipeline        = 1 << 3,
    RenderPass      = 1 << 4,
    Shader          = 1 << 5,
    DescriptorPool  = 1 << 6,
    Event           = 1 << 7,
    State           = 1 << 8,
    Queue           = 1 << 9
};

// =========================================================

template <Resource Type>
class Object
{
public:
    typedef Handle      pointer;
    typedef std::size_t size_type;

    Object (Object&&) noexcept = default;
    Object (Object const& rhs) noexcept;
    Object& operator = (Object&&) = default;
    Object& operator = (Object const& rhs) noexcept;
    ~Object () noexcept;

    Object (Device& owner) noexcept : m_object (), m_owner (&owner) { }
    Device*   owner         () const noexcept { return m_owner; }
    constexpr Resource type () const noexcept { return Type;    }

    template <typename T = pointer::pointer>
    T handle () const noexcept { return m_object.get<T> ();  }

private:
    pointer m_object;
    Device* m_owner ;
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OBJECT_H_
