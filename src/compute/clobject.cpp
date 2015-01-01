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

#include <cppual/compute/platform.h>
#include "cldef.h"

namespace cppual {

int32 reference (Disposable<Compute::ParallelQueue>* pObj) noexcept
{ return ::clRetainCommandQueue (reinterpret_cast<cl_command_queue> (pObj)); }

int32 reference (Disposable<Compute::Sampler>* pObj) noexcept
{ return ::clRetainSampler (reinterpret_cast<cl_sampler> (pObj)); }

int32 reference (Disposable<Compute::Program>* pObj) noexcept
{ return ::clRetainProgram (reinterpret_cast<cl_program> (pObj)); }

int32 reference (Disposable<Compute::Kernel>* pObj) noexcept
{ return ::clRetainKernel (reinterpret_cast<cl_kernel> (pObj)); }

int32 reference (Disposable<Compute::Event>* pObj) noexcept
{ return ::clRetainEvent (reinterpret_cast<cl_event> (pObj)); }

int32 reference (Disposable<Compute::Context>* pObj) noexcept
{ return ::clRetainContext (reinterpret_cast<cl_context> (pObj)); }

void* Disposable<Compute::Context>::operator new (std::size_t) noexcept
{ return ::clCreateContext (nullptr, 0, nullptr, nullptr, nullptr, nullptr); }

void Disposable<Compute::Context>::operator delete (void* pObj) noexcept
{ ::clReleaseContext (static_cast<cl_context> (pObj)); }

void* Disposable<Compute::ParallelQueue>::operator new (std::size_t) noexcept
{ return ::clCreateCommandQueue (nullptr, nullptr, 0, nullptr); }

void Disposable<Compute::ParallelQueue>::operator delete (void* queue) noexcept
{  ::clReleaseCommandQueue (reinterpret_cast<cl_command_queue> (queue)); }

void* Disposable<Compute::Sampler>::operator new (std::size_t) noexcept
{ return ::clCreateSampler (nullptr, 0, 0, 0, nullptr); }

void Disposable<Compute::Sampler>::operator delete (void* sampler) noexcept
{ ::clReleaseSampler (reinterpret_cast<cl_sampler> (sampler)); }

void* Disposable<Compute::Program>::operator new (std::size_t) noexcept
{ return ::clCreateProgramWithSource (nullptr, 0, nullptr, nullptr, nullptr); }

void Disposable<Compute::Program>::operator delete (void* program) noexcept
{ ::clReleaseProgram (reinterpret_cast<cl_program> (program)); }

void* Disposable<Compute::Kernel>::operator new (std::size_t) noexcept
{ return ::clCreateKernel (nullptr, nullptr, nullptr); }

void Disposable<Compute::Kernel>::operator delete (void* kernel) noexcept
{ ::clReleaseKernel (reinterpret_cast<cl_kernel> (kernel)); }

void* Disposable<Compute::Event>::operator new (std::size_t) noexcept
{ return ::clCreateUserEvent (nullptr, nullptr); }

void Disposable<Compute::Event>::operator delete (void* event) noexcept
{ ::clReleaseEvent (reinterpret_cast<cl_event> (event)); }

} // cppual
