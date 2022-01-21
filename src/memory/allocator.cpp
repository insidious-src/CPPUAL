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

#include <cppual/memory/allocator.h>

#include <cstdlib>

namespace cppual { namespace Memory {

// =========================================================

namespace { // optimize for internal unit usage

MemoryResource::base_double_pointer default_resource_pointer() noexcept
{
    static MemoryResource::base_pointer def_ptr = nullptr;
    return &def_ptr;
}

// =========================================================

class NewDeleteResource final : public MemoryResource
{
public:
    using base_type::base_type;
    using base_type::operator=;

    bool is_thread_safe () const noexcept
    {
        return true;
    }

private:
    void* do_allocate(size_type bytes, size_type alignment)
    {
        if (bytes > this->max_size()) throw std::bad_alloc();

        if (alignment > max_align) return ::operator new(bytes, std::align_val_t(alignment));
        return ::operator new(bytes);
    }

    void do_deallocate(void* p, size_type /*bytes*/, size_type alignment)
    {
        if (alignment > max_align) ::operator delete(p, std::align_val_t(alignment));
        else ::operator delete(p);
    }

    bool do_is_equal(base_type const& other) const noexcept
    {
        return this == &other;
    }
};

// =========================================================

class MallocResource final : public MemoryResource
{
public:
    using base_type::base_type;
    using base_type::operator=;

    bool is_thread_safe () const noexcept
    {
        return true;
    }

private:
    void* do_allocate(size_type bytes, size_type /*alignment*/)
    {
        return std::malloc(bytes);
    }

    void do_deallocate(void* p, size_type /*bytes*/, size_type /*alignment*/)
    {
        std::free(p);
    }

    void* do_reallocate(void* p, size_type /*old_size*/, size_type new_size, size_type /*alignment*/)
    {
        return p ? std::realloc(p, new_size) : std::malloc(new_size);
    }

    bool do_is_equal(base_type const& other) const noexcept
    {
        return this == &other;
    }
};

} // anonymous namespace

// =========================================================

MemoryResource* new_delete_resource() noexcept
{
    static NewDeleteResource new_delete_resource;
    return &new_delete_resource;
}

MemoryResource* malloc_resource() noexcept
{
    static MallocResource malloc_resource;
    return &malloc_resource;
}

MemoryResource* get_default_resource() noexcept
{
    static MemoryResource::base_double_pointer def_resource_ptr =
            &(*default_resource_pointer() = new_delete_resource());
    return *def_resource_ptr;
}

void set_default_resource(MemoryResource& res) noexcept
{
    if (*default_resource_pointer() == nullptr) get_default_resource();

    *default_resource_pointer() = &res;
}

// =========================================================

} } // Memory
