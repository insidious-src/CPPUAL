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

#ifndef CPPUAL_COMPUTE_HOST_H_
#define CPPUAL_COMPUTE_HOST_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/compute/device.h>

namespace cppual { namespace Compute {

class HostConnection : public NonCopyable
{
public:
    typedef std::size_t size_type;
    typedef Handle      pointer;

    size_type deviceCount ();
    size_type gpuCount    ();

    pointer handle () const noexcept { return m_pConn; }

private:
    pointer m_pConn;
};

} } // namespace Platform

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_HOST_H_