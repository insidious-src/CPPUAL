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

#ifndef CPPUAL_COMPUTE_DEVICE_H_
#define CPPUAL_COMPUTE_DEVICE_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/string.h>
#include <cppual/noncopyable.h>
#include <cppual/compute/backend_iface.h>

#include <memory>

namespace cppual { namespace Compute {

// =========================================================

class device_exception    : public std::bad_exception { };
class bad_device          : public device_exception   { };
class not_available       : public device_exception   { };
class already_created     : public device_exception   { };
class bad_partition_count : public device_exception   { };

// =========================================================

class Device
{
public:
    typedef std::shared_ptr<IDevice> shared_device;
    typedef string                   string_type  ;
    typedef std::size_t              size_type    ;

    enum class Info : byte
    {
        Name,
        Board,
        Vendor,
        Profile,
        Version
    };

    static size_type count () noexcept;
    static Device&   host  () noexcept;

    bool      available    (string_type const& feature);
    string    info         (Info);
    size_type cache        () const;
    size_type cacheLine    () const;
    size_type localMemory  () const;
    size_type globalMemory () const;

    DeviceType type     () const noexcept;
    bool       valid    () const noexcept;
    Backend    backend  () const noexcept;

private:
    shared_device _M_devPtr;
};

} } // Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_DEVICE_H_
