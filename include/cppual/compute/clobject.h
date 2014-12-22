/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

class Device;
class Context;
class SerialQueue;
class ParallelQueue;
class MemoryRegion;
class Sampler;
class Program;
class Kernel;
class Event;

template <class T>
class Object
{
public:
	typedef Disposable<T>  value_type;
	typedef Disposable<T>* pointer;
	typedef u32            uint_type;
	typedef int32          int_type;

	Object (Object&&) noexcept = default;
	Object& operator = (Object&&) = default;

	constexpr Object () noexcept : m_object () { }
	inline   ~Object () noexcept { if (m_object != nullptr) delete m_object; }
	inline    pointer handle () const noexcept { return m_object; }

	inline Object (Object<T> const& rhs) noexcept
	: m_object (rhs.m_object)
	{ if (m_object != nullptr) reference (m_object); }

	inline Object& operator = (Object<T> const& rhs) noexcept
	{
		if (this == &rhs) return *this;
		if (m_object != nullptr) delete m_object;
		m_object = rhs.m_object;
		if (m_object != nullptr) reference (m_object);
		return *this;
	}

	inline pointer  operator ()() const noexcept { return m_object; }
	inline pointer& operator ()() noexcept { return m_object; }

protected:
	constexpr Object (pointer obj) noexcept : m_object (obj)
	{ }

private:
	pointer m_object;
};

} // Compute

int32 reference (Disposable<Compute::ParallelQueue>* pObj) noexcept;
int32 reference (Disposable<Compute::Sampler>* pObj) noexcept;
int32 reference (Disposable<Compute::Program>* pObj) noexcept;
int32 reference (Disposable<Compute::Kernel>* pObj) noexcept;
int32 reference (Disposable<Compute::Event>* pObj) noexcept;
int32 reference (Disposable<Compute::Context>* pObj) noexcept;

template <>
struct Disposable <Compute::Context>
{
	static void* operator new (std::size_t) noexcept;
	static void  operator delete (void*) noexcept;
};

template <>
struct Disposable <Compute::ParallelQueue>
{
	static void* operator new (std::size_t) noexcept;
	static void  operator delete (void*) noexcept;
};

template <>
struct Disposable <Compute::Sampler>
{
	static void* operator new (std::size_t) noexcept;
	static void  operator delete (void*) noexcept;
};

template <>
struct Disposable <Compute::Program>
{
	static void* operator new (std::size_t) noexcept;
	static void  operator delete (void*) noexcept;
};

template <>
struct Disposable <Compute::Kernel>
{
	static void* operator new (std::size_t) noexcept;
	static void  operator delete (void*) noexcept;
};

template <>
struct Disposable <Compute::Event>
{
	static void* operator new (std::size_t) noexcept;
	static void  operator delete (void*) noexcept;
};

} // cppual

#endif // __cplusplus
#endif // CPPUAL_CL_OBJECT_H_
