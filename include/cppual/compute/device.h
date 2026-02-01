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

#ifndef CPPUAL_COMPUTE_DEVICE_H_
#define CPPUAL_COMPUTE_DEVICE_H_
#ifdef __cplusplus

#include <cppual/bitflags>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/compute/object.h>
#include <cppual/compute/backend_iface.h>

// =========================================================

namespace cppual::compute {

// =========================================================

class device_exception    : public std::bad_exception { };
class bad_device          : public device_exception   { };
class not_available       : public device_exception   { };
class already_created     : public device_exception   { };
class bad_partition_count : public device_exception   { };
class out_of_memory       : public device_exception   { };

// =========================================================

class device : public object<resource_type::device> /*, public device_interface */
{
public:
    typedef std::size_t size_type;

    typedef enum class info_type : u8
    {
        name,
        board,
        vendor,
        profile,
        version
    }
    const const_info;

    static size_type count () noexcept;

    bool      available          (string_type const& feature);
    string    info               (info_type nfo);
    size_type cache_size         () const;
    size_type cache_line_size    () const;
    size_type local_memory_size  () const;
    size_type const_memory_size  () const;
    size_type global_memory_size () const;
    size_type max_alloc_size     () const;
    u32       compute_units      () const;
    bool      is_host            () const noexcept;

    device_type  type    () const noexcept;
    bool         valid   () const noexcept;
    backend_type backend () const noexcept;

    constexpr static device& host () noexcept
    { return get_host_device (); }

private:
    void assign_dev_from_cat () const;

    constexpr static device& get_host_device ()
    {
        static device host (device_type::cpu);
        return host;
    }

    constexpr device (device_type dev_cat) noexcept
    : _M_pDev        (),
      _M_eCategory   (dev_cat)
    { }

    friend class factory;

private:
    shared_device mutable _M_pDev     ;
    device_type           _M_eCategory;
};

// =========================================================

} //! compute

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_DEVICE_H_
