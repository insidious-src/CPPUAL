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

#include <cppual/ui/queue.h>
#include <cppual/ui/manager.h>

namespace cppual::ui {

namespace { namespace internal { /// optimize for internal unit usage

constexpr static const shared_queue& queue ()
{
    static const shared_queue instance (platform::factory::instance ()->createQueueInstance ());
    return instance;
}

} } // anonymous namespace Internal

// =========================================================

shared_queue display_queue_interface::primary () noexcept
{
    return internal::queue ();
}

bool display_queue_interface::has_valid_instance () noexcept
{
    return internal::queue () != nullptr;
}

} // namespace Input
