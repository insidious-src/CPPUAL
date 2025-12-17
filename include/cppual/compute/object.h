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

#include <cppual/types>
#include <cppual/bitflags>
#include <cppual/resource>
#include <cppual/containers>

// =========================================================

namespace cppual::compute {

// =========================================================

/// application instance info
class host_connection ;
class device          ;
/// device and queue load balancing
class behaviour       ;
/// different types of devices in one queue
class mixed_queue     ;
/// thread queue
class host_queue      ;
class device_queue    ;
class device_surface  ;
class pipeline        ;
class image           ;
/// render passes and states
class render          ;
class command         ;
class command_sequence;
class event           ;

// =========================================================

template <resource_type R>
class SHARED_API object : public resource<void, void*>
{
public:
    typedef object<R>             self_type  ;
    typedef resource<void, void*> base_type  ;
    typedef std::size_t           size_type  ;
    typedef string                string_type;

    constexpr object () noexcept = default;

    consteval static resource_type type () noexcept { return R; }

    constexpr operator handle_type::value_type () const noexcept
    { return handle<handle_type::value_type> (); }

protected:
    constexpr object (handle_type handle) noexcept
    : base_type (handle)
    { }
};

// =========================================================

} // compute

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_OBJECT_H_
