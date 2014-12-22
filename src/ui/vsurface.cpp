/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#include <cppual/ui/vsurface.h>

namespace cppual { namespace Ui {

ProxyRenderable::ProxyRenderable (shared_renderable const& pParent, Rect const& gRect) noexcept
: IRenderable (pParent != nullptr ? pParent->connection () : nullptr,
			pParent != nullptr ? pParent->id () : Element (nullptr),
			ResourceType::Surface),
  m_pParent (pParent),
  m_gRect (gRect),
  m_bIsVisible ()
{
}

ProxyRenderable::ProxyRenderable (IRenderable*) noexcept
{
}

ProxyRenderable::ProxyRenderable (ProxyRenderable const&) noexcept
{
}

ProxyRenderable& ProxyRenderable::operator = (IRenderable*) noexcept
{
	return *this;
}

ProxyRenderable& ProxyRenderable::operator = (ProxyRenderable const& gObj) noexcept
{
	if (this == &gObj) return *this;
	return *this;
}

void ProxyRenderable::setGeometry (Rect const&) noexcept
{
}

void ProxyRenderable::setParent (shared_renderable const&, point2i) noexcept
{
}

void ProxyRenderable::map () noexcept
{
	if (m_pParent == nullptr) return;
}

void ProxyRenderable::unmap () noexcept
{
	if (m_pParent == nullptr) return;
}

void ProxyRenderable::move (point2i gPos) noexcept
{
	if (m_pParent != nullptr)
	{
		m_gRect = Rect (gPos.x, gPos.y, m_gRect.width (), m_gRect.height ());
	}
}

} } // namespace Ui
