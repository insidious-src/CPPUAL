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

#ifndef CPPUAL_COMPUTE_PARALLEL_TASK_H_
#define CPPUAL_COMPUTE_PARALLEL_TASK_H_
#ifdef __cplusplus

#include <cppual/bitset.h>
#include <cppual/compute/device.h>

// =========================================================

namespace cppual::compute {

// =========================================================

class device_queue : public object<resource_type::queue>
{
public:
    typedef enum class engine_type : u8
    {
        none      =      0,
        compute   = 1 << 0,
        graphics  = 1 << 1,
        dma       = 1 << 2, // direct memory access
        universal = 1 << 3,
        timer     = 1 << 4,
        custom    = 1 << 7
    }
    const const_engine;

    typedef bitset<engine_type> engine_types;

    device_queue ();
    device_queue (device& dev, engine_type exec_engine_type);
    device_queue (device_queue&& dev);
    device_queue& operator = (device_queue&&);

    static engine_types supported_types (device const& dev) noexcept;

    engine_type type () const noexcept { return _M_eQueueType; }

private:
    device*     _M_pDevice   ;
    engine_type _M_eQueueType;
};

// =========================================================

template <typename T>
class device_task : private device_queue
{
};

// =========================================================

} //! namespace compute

#endif // __cplusplus
#endif // CPPUAL_COMPUTE_PARALLEL_TASK_H_
