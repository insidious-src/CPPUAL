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

namespace cppual { namespace compute {

/// application instance info
class host_connection;
class device;
/// device and queue load balancing
class behaviour;
/// difference types of devices in one queue
class mixed_queue;
/// thread queue
class host_queue;
class device_queue;
class device_surface;
class pipeline;
class image;
/// render passes and states
class render;
class command;
class command_sequence;
class event;

// =========================================================

template <resource_type Type>
class object : public resource<void, void*>
{
public:
    typedef std::size_t size_type  ;
    typedef string      string_type;

    constexpr object () noexcept = default;

    object (object&&) noexcept = default;
    object& operator = (object&&) = default;

    constexpr static resource_type type () noexcept { return Type; }

    constexpr operator handle_type::pointer () const noexcept
    { return handle<handle_type::pointer>(); }

protected:
    constexpr object (handle_type handle) noexcept
    : resource (handle)
    { }
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OBJECT_H_
