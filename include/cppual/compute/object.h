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

template <ResourceType Type>
class Object : public Resource<void, void*>
{
public:
    typedef std::size_t size_type  ;
    typedef string      string_type;

    constexpr Object () noexcept = default;

    Object (Object&&) noexcept = default;
    Object& operator = (Object&&) = default;

    constexpr static ResourceType type () noexcept { return Type; }

    constexpr operator handle_type::pointer () const noexcept
    { return handle<handle_type::pointer>(); }

protected:
    constexpr Object (handle_type handle) noexcept
    : Resource (handle)
    { }
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OBJECT_H_
