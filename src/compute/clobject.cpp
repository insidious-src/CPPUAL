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

#include <cppual/compute/platform.h>
#include "backend.h"

namespace cppual { namespace Compute {

//template <>
//Object<Device>::~Object () noexcept
//{
//    if (_M_object != nullptr) ::clReleaseDevice (_M_object.get<CL::device_type> ());
//}

//template <>
//Object<Behaviour>::~Object () noexcept
//{
//    if (_M_object != nullptr) ::clReleaseContext (_M_object.get<CL::context_type> ());
//}

//template <>
//Object<Kernel>::~Object () noexcept
//{
//    if (_M_object != nullptr) ::clReleaseKernel (_M_object.get<CL::kernel_type> ());
//}

//template <>
//Object<Program>::~Object () noexcept
//{
//    if (_M_object != nullptr) ::clReleaseProgram (_M_object.get<CL::program_type> ());
//}

//template <>
//Object<DeviceQueue>::~Object () noexcept
//{
//    if (_M_object != nullptr) ::clReleaseCommandQueue (_M_object.get<CL::queue_type> ());
//}

//template <>
//Object<GlobalMemory>::~Object () noexcept
//{
//    if (_M_object != nullptr) ::clReleaseMemObject (_M_object.get<CL::memory_type> ());
//}

//template <>
//Object<Event>::~Object () noexcept
//{
//    if (_M_object != nullptr) ::clReleaseEvent (_M_object.get<cl_event> ());
//}

//template <>
//Object<Device>::Object (Object const& rhs) noexcept
//: _M_object (rhs._M_object)
//{
//    if (_M_object != nullptr) ::clRetainDevice (_M_object.get<CL::device_type> ());
//}

//template <>
//Object<Behaviour>::Object (Object const& rhs) noexcept
//: _M_object (rhs._M_object)
//{
//    if (_M_object != nullptr) ::clRetainContext (_M_object.get<CL::context_type> ());
//}

//template <>
//Object<Kernel>::Object (Object const& rhs) noexcept
//: _M_object (rhs._M_object)
//{
//    if (_M_object != nullptr) ::clRetainKernel (_M_object.get<CL::kernel_type> ());
//}

//template <>
//Object<Program>::Object (Object const& rhs) noexcept
//: _M_object (rhs._M_object)
//{
//    if (_M_object != nullptr) ::clRetainProgram (_M_object.get<CL::program_type> ());
//}

//template <>
//Object<DeviceQueue>::Object (Object const& rhs) noexcept
//: _M_object (rhs._M_object)
//{
//    if (_M_object != nullptr) ::clRetainCommandQueue (_M_object.get<CL::queue_type> ());
//}

//template <>
//Object<Event>::Object (Object const& rhs) noexcept
//: _M_object (rhs._M_object)
//{
//    if (_M_object != nullptr) ::clRetainEvent (_M_object.get<CL::event_type> ());
//}

//template <>
//Object<GlobalMemory>::Object (Object const& rhs) noexcept
//: _M_object (rhs._M_object)
//{
//    if (_M_object != nullptr) ::clRetainMemObject (_M_object.get<CL::memory_type> ());
//}

//template <>
//Object<Device>& Object<Device>::operator = (Object const& rhs) noexcept
//{
//    if (this == &rhs) return *this;
//    if (_M_object != nullptr) ::clReleaseDevice (_M_object.get<CL::device_type> ());
//    _M_object = rhs._M_object;
//    if (_M_object != nullptr) ::clRetainDevice  (_M_object.get<CL::device_type> ());
//    return *this;
//}

//template <>
//Object<Behaviour>& Object<Behaviour>::operator = (Object const& rhs) noexcept
//{
//    if (this == &rhs) return *this;
//    if (_M_object != nullptr) ::clReleaseContext (_M_object.get<CL::context_type> ());
//    _M_object = rhs._M_object;
//    if (_M_object != nullptr) ::clRetainContext  (_M_object.get<CL::context_type> ());
//    return *this;
//}

//template <>
//Object<Kernel>& Object<Kernel>::operator = (Object const& rhs) noexcept
//{
//    if (this == &rhs) return *this;
//    if (_M_object != nullptr) ::clReleaseKernel (_M_object.get<CL::kernel_type> ());
//    _M_object = rhs._M_object;
//    if (_M_object != nullptr) ::clRetainKernel  (_M_object.get<CL::kernel_type> ());
//    return *this;
//}

//template <>
//Object<Program>& Object<Program>::operator = (Object const& rhs) noexcept
//{
//    if (this == &rhs) return *this;
//    if (_M_object != nullptr) ::clReleaseProgram (_M_object.get<CL::program_type> ());
//    _M_object = rhs._M_object;
//    if (_M_object != nullptr) ::clRetainProgram  (_M_object.get<CL::program_type> ());
//    return *this;
//}

//template <>
//Object<DeviceQueue>& Object<DeviceQueue>::operator = (Object const& rhs) noexcept
//{
//    if (this == &rhs) return *this;
//    if (_M_object != nullptr) ::clReleaseCommandQueue (_M_object.get<CL::queue_type> ());
//    _M_object = rhs._M_object;
//    if (_M_object != nullptr) ::clRetainCommandQueue  (_M_object.get<CL::queue_type> ());
//    return *this;
//}

//template <>
//Object<Event>& Object<Event>::operator = (Object const& rhs) noexcept
//{
//    if (this == &rhs) return *this;
//    if (_M_object != nullptr) ::clReleaseEvent (_M_object.get<CL::event_type> ());
//    _M_object = rhs._M_object;
//    if (_M_object != nullptr) ::clRetainEvent  (_M_object.get<CL::event_type> ());
//    return *this;
//}

//template <>
//Object<GlobalMemory>& Object<GlobalMemory>::operator = (Object const& rhs) noexcept
//{
//    if (this == &rhs) return *this;
//    if (_M_object != nullptr) ::clReleaseMemObject (_M_object.get<CL::memory_type> ());
//    _M_object = rhs._M_object;
//    if (_M_object != nullptr) ::clRetainMemObject  (_M_object.get<CL::memory_type> ());
//    return *this;
//}

} } // namespace Compute
