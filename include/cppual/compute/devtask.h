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

#ifndef CPPUAL_COMPUTE_PARALLEL_TASK_H_
#define CPPUAL_COMPUTE_PARALLEL_TASK_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/compute/device.h>
#include <cppual/compute/platform.h>

namespace cppual { namespace Compute {

class DeviceQueue : public Object<Resource::Queue>
{
public:
    enum EngineType
    {
        Compute   = 1 << 0,
        Graphics  = 1 << 1,
        DMA       = 1 << 2, // direct memory access
        Universal = 1 << 3,
        Timer     = 1 << 4,
        Custom    = 1 << 5
    };

    typedef BitSet<EngineType> EngineTypes;

    DeviceQueue ();
    DeviceQueue (Device const&, EngineType exec_engine_type);
    DeviceQueue (DeviceQueue&&);
    DeviceQueue& operator = (DeviceQueue&&);

    static EngineTypes supportedTypes (Device const&) noexcept;

    EngineType type () const noexcept { return m_eQueueType; }

private:
    EngineType m_eQueueType;
};

template <typename T>
class DeviceTask : private DeviceQueue
{
};

} } // namespace Compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_PARALLEL_TASK_H_
