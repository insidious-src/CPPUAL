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

#include <cppual/compute/device.h>
#include <vulkan/vulkan.h>

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

} // anonymous namespace

Object<Resource::Buffer>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

//Object& Object::operator = (Object const& gObj) noexcept
//{
//    if (this != &gObj)
//    {
//        if (m_object) ::vkDestroyObject (m_object.get<VK_OBJECT> ());
//        m_object  = copy_object (gObj);
//    }

//    return *this;
//}

Object<Resource::Buffer>::~Object () noexcept
{
    if (!m_owner) return;

    switch (m_owner->backend ())
    {
    case Device::Backend::Vulkan:
        ::vkDestroyBuffer (m_owner->handle<VkDevice> (), handle<VkBuffer> (), nullptr);
        break;
    default:
        break;
    }
}

} } // namespace Compute
