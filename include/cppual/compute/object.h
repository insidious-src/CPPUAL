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

#ifndef CPPUAL_COMPUTE_OBJECT_H_
#define CPPUAL_COMPUTE_OBJECT_H_
#ifdef __cplusplus

#include <cppual/meta.h>
#include <cppual/types.h>
#include <cppual/flags.h>
#include <cppual/string.h>
#include <cppual/resource.h>

namespace cppual { namespace Compute {

struct Factory;
class  HostConnection; // application instance info
class  Device;
class  Behaviour;      // device and queue load balancing
class  MixedQueue;     // host and multiple devices
class  HostQueue;      // thread queue
class  DeviceQueue;
class  DeviceSurface;
class  Pipeline;
class  Image;
class  Render;         // render passes and states
class  Command;
class  CommandSequence;
class  Event;

// =========================================================

enum class ObjectType : ushort
{
    Base            =       0,
    Device          = 1 <<  0,
    Buffer          = 1 <<  1,
    CommandSequence = 1 <<  2,
    Image           = 1 <<  3,
    Pipeline        = 1 <<  4,
    RenderPass      = 1 <<  5,
    Shader          = 1 <<  6,
    DescriptorPool  = 1 <<  7,
    Event           = 1 <<  8,
    State           = 1 <<  9,
    Queue           = 1 << 10
};

// =========================================================

template <ObjectType Type>
class Object : public Resource<void, void*>
{
public:
    static_assert (Type != ObjectType::Base, "Not a valid Resource type!");

    typedef std::size_t size_type  ;
    typedef string      string_type;

    constexpr Object () noexcept = default;

    Object (Object&&) noexcept = default;
    Object& operator = (Object&&) = default;

    constexpr static ObjectType type () noexcept { return Type; }

    constexpr operator handle_type::pointer () const noexcept
    { return _M_handle; }

protected:
    constexpr Object (handle_type handle) noexcept
    : _M_handle (handle)
    { }

private:
    handle_type _M_handle;
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OBJECT_H_
