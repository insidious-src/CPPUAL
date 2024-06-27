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

#include <cppual/memory/allocator.h>
#include <cppual/compute/device.h>

#include <new>
#include <cstdlib>

namespace cppual { namespace memory {

// =========================================================

namespace { // optimize for internal unit usage

// =========================================================

static const std::thread::id _main_thread_id = std::this_thread::get_id ();

// =========================================================

inline static memory_resource::base_pointer_reference internal_default_resource () noexcept
{
    static memory_resource::base_pointer def_ptr = &new_delete_resource ();
    return def_ptr;
}

// =========================================================

inline static memory_resource::base_pointer_reference internal_default_thread_resource () noexcept
{
    static thread_local memory_resource::base_pointer def_ptr = internal_default_resource ();
    return def_ptr;
}

// =========================================================

class null_memory_resource final : public memory_resource
{
public:
    using base_type::base_type;
    using base_type::operator=;

private:
    inline void* do_allocate (size_type /*bytes*/, size_type /*align*/)
    { throw std::bad_alloc (); return nullptr; }

    inline void* do_reallocate (pointer /*p*/, size_type /*old_size*/, size_type /*size*/, size_type /*align*/)
    { throw std::bad_alloc (); return nullptr; }

    inline void do_deallocate (void* /*p*/, size_type /*bytes*/, size_type /*align*/)
    { }

    inline bool do_is_equal (base_const_reference other) const noexcept
    { return this == &other; }
};

// =========================================================

class new_delete_memory_resource final : public memory_resource
{
public:
    using base_type::base_type;
    using base_type::operator=;

    inline bool is_thread_safe () const noexcept { return true; }

private:
    inline void* do_allocate (size_type bytes, size_type align)
    {
        if (bytes > max_size ()) throw std::bad_alloc ();
        return ::operator new (bytes, std::align_val_t (align));
    }

    inline void do_deallocate (void* p, size_type bytes, size_type align)
    {
        ::operator delete (p, bytes, std::align_val_t (align));
        //::operator delete (p);
    }

    inline bool do_is_equal (base_const_reference other) const noexcept
    {
        return this == &other;
    }
};

// =========================================================

class malloc_memory_resource final : public memory_resource
{
public:
    using base_type::base_type;
    using base_type::operator=;

    inline bool is_thread_safe () const noexcept { return true; }

private:
    inline void* do_allocate (size_type bytes, size_type /*align*/)
    {
        return std::malloc (bytes);
    }

    inline void do_deallocate (void* p, size_type /*bytes*/, size_type /*align*/)
    {
        std::free (p);
    }

    inline void* do_reallocate (void* p, size_type /*old_size*/, size_type new_size, size_type /*align*/)
    {
        return p ? std::realloc (p, new_size) : std::malloc (new_size);
    }

    inline bool do_is_equal (base_const_reference other) const noexcept
    {
        return this == &other;
    }
};

} // anonymous namespace

// =========================================================

memory_resource::~memory_resource ()
{
}

memory_resource::device_reference memory_resource::host_device () noexcept
{
    return device_type::host ();
}

// =========================================================

memory_resource& new_delete_resource () noexcept
{
    static new_delete_memory_resource res;
    return res;
}

memory_resource& null_resource () noexcept
{
    static null_memory_resource res;
    return res;
}

memory_resource& malloc_resource () noexcept
{
    static malloc_memory_resource res;
    return res;
}

memory_resource& get_default_resource () noexcept
{
    return *internal_default_resource ();
}

memory_resource& get_default_thread_resource () noexcept
{
    return *internal_default_thread_resource ();
}

void set_default_resource(memory_resource& res) noexcept
{
    internal_default_resource () = &res;
}

void set_default_thread_resource(memory_resource& res) noexcept
{
    internal_default_thread_resource () = &res;
}

std::thread::id main_thread_id () noexcept
{
    return _main_thread_id;
}

// =========================================================

} } // Memory
