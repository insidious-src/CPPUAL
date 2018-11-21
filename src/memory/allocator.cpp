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
#include <new>

namespace cppual { namespace Memory {

namespace {

class NewDeleteResource final : public MemoryResource
{
    void* do_allocate (size_type bytes, size_type alignment)
    {
        return ::operator new (bytes, static_cast<std::align_val_t>(alignment));
    }

    void do_deallocate (void* p, size_type bytes, size_type alignment)
    {
        ::operator delete (p, bytes, static_cast<std::align_val_t>(alignment));
    }

    bool do_is_equal (memory_resource const& rc) const noexcept
    {
        return this == &rc;
    }
};

inline MemoryResource*& default_resource() noexcept
{
    static NewDeleteResource new_del_rc;
    static MemoryResource* rc = &new_del_rc;
    return rc;
}

} // anonymous namespace

MemoryResource* get_default_resource()
{
    return default_resource();
}

void set_default_resource(MemoryResource& rc)
{
    default_resource() = &rc;
}



} } // Memory

