/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

namespace cppual { namespace Compute {

class HostConnection; // application instance info
class Device;
class Object;
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

class Handle
{
public:
    typedef void* pointer;

    inline    Handle () noexcept = default;
    constexpr Handle (pointer handle) noexcept : m_handle (handle) { }
    constexpr Handle (std::nullptr_t) noexcept : m_handle ()       { }

    constexpr operator pointer () const noexcept
    { return m_handle; }

    template <typename T>
    constexpr typename std::remove_pointer<T>::type* get () const noexcept
    { return static_cast<typename std::remove_pointer<T>::type*> (m_handle); }

    friend
    constexpr bool operator == (Handle const&, Handle const&) noexcept;

    friend
    constexpr bool operator == (Handle const&, std::nullptr_t) noexcept;

private:
    pointer m_handle;
};

constexpr bool operator == (Handle const& conn1, Handle const& conn2) noexcept
{ return conn1.m_handle == conn2.m_handle; }

constexpr bool operator == (Handle const& conn1, std::nullptr_t) noexcept
{ return conn1.m_handle == nullptr; }

constexpr bool operator != (Handle const& conn1, Handle const& conn2) noexcept
{ return !(conn1 == conn2); }

constexpr bool operator != (Handle const& conn1, std::nullptr_t) noexcept
{ return !(conn1 == nullptr); }

// =========================================================

class Object
{
public:
    typedef Handle      pointer;
    typedef std::size_t size_type;

    enum Resource
    {
        Buffer,
        CommandSequence,
        Image,
        Pipeline,
        RenderPass,
        Shader,
        DescriptorPool,
        Event,
        State,
        Queue
    };

    Object (Object&&) noexcept = default;
    Object (Object const& rhs) noexcept;
    Object& operator = (Object&&) = default;
    Object& operator = (Object const& rhs) noexcept;
    ~Object () noexcept;

    Object () noexcept : m_object (), m_resType () { }
    Resource resource_type () const noexcept { return m_resType; }
    pointer  handle        () const noexcept { return m_object;  }


private:
    pointer  m_object;
    Resource m_resType;
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OBJECT_H_