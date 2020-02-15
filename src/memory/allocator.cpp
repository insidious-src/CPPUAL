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

#include <cppual/memory/heap.h>
#include <deque>

namespace cppual { namespace Memory {

namespace {

typedef MemoryResource  resource_type;
typedef MemoryResource* pointer_type ;

// =========================================================

struct NewResource final : public MemoryResource
{
public:
    using MemoryResource::MemoryResource;

    bool is_thread_safe () const noexcept { return true; }

protected:
    void* do_allocate(size_t bytes, size_t alignment)
    {
        if (bytes > this->max_size()) throw std::bad_alloc();

        if (alignment > max_align)
        {
            return ::operator new(bytes, std::align_val_t(alignment));
        }

        return ::operator new(bytes);
    }

    void do_deallocate(void* p, size_t /*bytes*/, size_t alignment)
    {
        if (alignment > max_align)
        {
            ::operator delete(p, std::align_val_t(alignment));
            return;
        }

        ::operator delete(p);
    }

    virtual bool do_is_equal(MemoryResource const& other) const noexcept
    {
        return this == &other;
    }
};

// =========================================================

struct NullResource final : public MemoryResource
{
public:
    using MemoryResource::MemoryResource;

protected:
    void* do_allocate(size_t, size_t)
    {
        throw std::bad_alloc();
    }

    void do_deallocate(void*, size_t, size_t)
    {
    }

    virtual bool do_is_equal(MemoryResource const& other) const noexcept
    {
        return this == &other;
    }
};

// =========================================================

inline pointer_type _null_resource() noexcept
{
    static NullResource rc;
    return &rc;
}

inline pointer_type _new_delete_resource() noexcept
{
    static NewResource rc;
    return &rc;
}

inline pointer_type& _default_resource() noexcept
{
    static pointer_type rc = _new_delete_resource();
    return rc;
}

} // anonymous namespace

// =========================================================

MemoryResource::~MemoryResource()
{

}

// =========================================================

MemoryResource* new_delete_resource() noexcept
{
    return _new_delete_resource();
}

MemoryResource* null_memory_resource() noexcept
{
    return _null_resource();
}

MemoryResource* get_default_resource() noexcept
{
    return _default_resource();
}

void set_default_resource(MemoryResource& rc) noexcept
{
    _default_resource() = &rc;
}

} } // Memory
