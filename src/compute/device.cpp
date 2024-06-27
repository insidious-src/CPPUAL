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

#include <cppual/compute/device.h>

// =========================================================

namespace cppual { namespace compute {

// =========================================================

namespace { // optimize for internal unit usage



} // anonymous namespace

// =========================================================

device::size_type device::count () noexcept
{
    auto const mgr       = factories::instances ();
    size_type  dev_count = 0;

    for (auto inst : mgr)
    {
         dev_count += inst->device_count (device_category::any);
    }

    return dev_count;
}

bool device::available (string_type const& /*feature*/)
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return false;
}

string device::info (info_type nfo)
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();

    switch (nfo)
    {
    case info_type::name:
        return _M_pDev->name ();
    case info_type::vendor:
        return _M_pDev->vendor ();
    default:
        return string_type ();
    }
}

device::size_type device::cache_size () const
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->cache_size ();
}

device::size_type device::cache_line_size () const
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->cache_line_size ();
}

device::size_type device::local_memory_size () const
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->local_memory_size ();
}

device::size_type device::const_memory_size () const
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->const_memory_size ();
}

device::size_type device::global_memory_size () const
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->global_memory_size ();
}

device::size_type device::max_alloc_size () const
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->max_memory_alloc_size ();
}

u32 device::compute_units () const
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->compute_units ();
}

bool device::is_host () const noexcept
{
    return this == &host ();
}

device_category device::type () const noexcept
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->dev_type ();
}

bool device::valid () const noexcept
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->valid ();
}

backend_type device::backend () const noexcept
{
    if (_M_pDev == nullptr) assign_dev_from_cat ();
    return _M_pDev->backend ();
}

void device::assign_dev_from_cat () const
{
    auto const mgr = factories::instances ();

    for (auto inst : mgr)
    {
        auto const devs = inst->get_devices (_M_eCategory);

        if (!devs.empty ())
        {
            /// if device_category == cpu -> by default it tries to get the host device.
            /// every compute backend creates host as cpu at index 0
            _M_pDev = std::move (devs[0]);
            return;
        }
    }
}

// =========================================================

} } // Compute
