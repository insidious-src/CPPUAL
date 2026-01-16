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

#ifndef CPPUAL_COMPUTE_MEMORY_H_
#define CPPUAL_COMPUTE_MEMORY_H_
#ifdef __cplusplus

#include <cppual/noncopyable.h>

//#include <new>

// =========================================================

namespace cppual::compute {

// =========================================================

class device;

// =========================================================

class device_memory : public non_copyable
{
public:
    typedef device      device_type     ;
    typedef device*     device_pointer  ;
    typedef device&     device_reference;
    typedef std::size_t size_type       ;

    device_memory (device_reference dev, size_type size);

    constexpr device_reference device () const noexcept
    { return *_M_pDevice; }

private:
    device_pointer _M_pDevice;
};

} // namespace Compute

// =========================================================

using cppual::compute::device       ;
using cppual::compute::device_memory;

// =========================================================

void* operator new    (std::size_t size, device& obj);
void  operator delete (void* ptr, device& obj);

constexpr void* operator new[] (std::size_t size, device& obj)
{ return ::operator new (size, obj); }

constexpr void  operator delete[] (void* ptr, device& obj)
{ ::operator delete (ptr, obj); }

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_MEMORY_H_
