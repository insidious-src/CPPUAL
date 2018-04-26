/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cppual/compute/device.h>

namespace cppual { namespace Compute {

namespace { // optimize for internal unit usage

} // anonymous namespace

template <>
Object<Resource::Buffer>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::Buffer>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::CommandSequence>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::CommandSequence>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::Image>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::Image>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::Pipeline>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::Pipeline>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::RenderPass>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::RenderPass>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::Shader>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::Shader>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::DescriptorPool>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::DescriptorPool>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::Event>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::Event>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::State>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::State>::~Object () noexcept
{
    if (!m_owner) return;
}

// =========================================================

template <>
Object<Resource::Queue>::Object (Object const& gObj) noexcept
: m_object (),
  m_owner  (gObj.m_owner)
{
}

template <>
Object<Resource::Queue>::~Object () noexcept
{
    if (!m_owner) return;
}

} } // namespace Compute
