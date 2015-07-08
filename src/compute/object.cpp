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

#include <cppual/compute/object.h>
#include <Vulkan/vk.h>

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

inline Object::pointer copy_object (Object const& /*gObj*/) noexcept
{
    return nullptr;
}

} // anonymous namespace

Object::Object (Object const& gObj) noexcept
: m_object  (copy_object (gObj)),
  m_resType (gObj.resource_type ())
{
}

Object& Object::operator = (Object const& gObj) noexcept
{
    if (this != &gObj)
    {
        if (m_object) ::vkDestroyObject (m_object.get<VK_OBJECT> ());
        m_object  = copy_object (gObj);
        m_resType = gObj.resource_type ();
    }

    return *this;
}

Object::~Object () noexcept
{
    if (m_object) ::vkDestroyObject (m_object.get<VK_OBJECT> ());
}

} } // namespace Compute
