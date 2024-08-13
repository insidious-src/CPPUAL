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

#include <cppual/compute/devtask.h>

namespace cppual { namespace compute {

device_queue::device_queue ()
{

}

device_queue::device_queue (device& dev, engine_type exec_engine_type)
: _M_pDevice    (&dev),
  _M_eQueueType (exec_engine_type)
{

}

device_queue::device_queue (device_queue&& obj)
: _M_pDevice    (obj._M_pDevice   ),
  _M_eQueueType (obj._M_eQueueType)
{
    obj._M_pDevice    = nullptr       ;
    obj._M_eQueueType = engine_type ();
}

device_queue& device_queue::operator = (device_queue&& obj)
{
    if (this != &obj)
    {
        std::swap (_M_pDevice   , obj._M_pDevice   );
        std::swap (_M_eQueueType, obj._M_eQueueType);
    }

    return *this;
}

device_queue::engine_types device_queue::supported_types (device const& /*dev*/) noexcept
{
    return engine_types ();
}

} } // namespace Compute
