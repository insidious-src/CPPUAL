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

#include <cppual/memory/allocator.h>

#include <cppual/compute/device.h>

#include <cstdlib>
#include <new>

// =========================================================

namespace cppual::memory {

// =========================================================

namespace { //! optimize for internal unit usage

// =========================================================

constexpr static const std::thread::id init_main_thread () noexcept
{
    get_default_resource ();
    return std::this_thread::get_id ();
}

// =========================================================

constexpr static memory_resource::base_pointer internal_set_def_resource () noexcept
{
    memory_resource::base_pointer new_del_res_ptr = &new_delete_resource ();
    std::pmr::set_default_resource (new_del_res_ptr);
    return new_del_res_ptr;
}

// =========================================================

constexpr static memory_resource::base_pointer_reference internal_default_resource () noexcept
{
    static memory_resource::base_pointer def_ptr = internal_set_def_resource ();
    return def_ptr;
}

constexpr static memory_resource::base_pointer_reference internal_default_thread_resource () noexcept
{
    static thread_local memory_resource::base_pointer def_thread_ptr = internal_default_resource ();
    return def_thread_ptr;
}

// =========================================================

inline static const std::thread::id _main_thread_id = init_main_thread ();

// =========================================================

class null_memory_resource final : public memory_resource
{
public:
    using base_type::base_type;
    using base_type::operator=;

    constexpr bool is_thread_safe () const noexcept override { return true; }

private:
    constexpr void* do_allocate (size_type, align_type) override
    { throw std::bad_alloc (); }

    constexpr void* do_reallocate (pointer, size_type, size_type, align_type) override
    { throw std::bad_alloc (); }

    constexpr void do_deallocate (pointer, size_type, align_type) override
    { }

    constexpr bool do_is_equal (abs_base_const_reference other) const noexcept override
    { return this == &other; }
};

// =========================================================

class new_delete_memory_resource final : public memory_resource
{
public:
    using base_type::base_type;
    using base_type::operator=;

    constexpr bool is_thread_safe () const noexcept { return true ; }
    constexpr bool is_lock_free   () const noexcept { return false; }

private:
    constexpr void* do_allocate (size_type bytes, size_type align)
    {
        if (bytes > max_size ()) throw std::bad_alloc ();
        return ::operator new (bytes, std::align_val_t (align));
    }

    constexpr void do_deallocate (pointer p, size_type bytes, size_type align)
    {
        ::operator delete (p, bytes, std::align_val_t (align));
    }

    constexpr bool do_is_equal (abs_base_const_reference other) const noexcept
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

    constexpr bool is_thread_safe () const noexcept { return true ; }
    constexpr bool is_lock_free   () const noexcept { return false; }

private:
    constexpr void* do_allocate (size_type bytes, size_type)
    {
        return std::malloc (bytes);
    }

    constexpr void do_deallocate (pointer p, size_type, size_type)
    {
        std::free (p);
    }

    constexpr void* do_reallocate (pointer p, size_type, size_type new_size, size_type)
    {
        return p ? std::realloc (p, new_size) : std::malloc (new_size);
    }

    constexpr bool do_is_equal (abs_base_const_reference other) const noexcept
    {
        return this == &other;
    }
};

} // anonymous namespace

// =========================================================

memory_resource::device_reference memory_resource::host_device () noexcept
{
    return device_type::host ();
}

// =========================================================

memory_resource::base_reference new_delete_resource () noexcept
{
    static new_delete_memory_resource res;
    return res;
}

memory_resource::base_reference null_resource () noexcept
{
    static null_memory_resource res;
    return res;
}

memory_resource::base_reference malloc_resource () noexcept
{
    static malloc_memory_resource res;
    return res;
}

memory_resource::base_reference get_default_resource () noexcept
{
    return *internal_default_resource ();
}

memory_resource::base_reference get_default_thread_resource () noexcept
{
    return *internal_default_thread_resource ();
}

void set_default_resource (memory_resource& res) noexcept
{
    internal_default_resource () =  &res ;
    std::pmr::set_default_resource (&res);
}

void set_default_thread_resource (memory_resource& res) noexcept
{
    internal_default_thread_resource () = &res;
    std::pmr::set_default_resource (&res);
}

std::thread::id main_thread_id () noexcept
{
    return _main_thread_id;
}

// =========================================================

} // Memory
