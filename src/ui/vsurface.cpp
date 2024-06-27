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

#include <cppual/ui/vsurface.h>

namespace cppual { namespace ui {

// ====================================================

proxy_renderable::proxy_renderable (shared_window const& pParent, rect const& gRect) noexcept
: platform_wnd_interface (pParent != nullptr ? pParent->connection () : nullptr,
                          pParent != nullptr ? pParent->handle     () : nullptr),
  _M_pParent (pParent),
  _M_gRect (gRect),
  _M_bIsVisible ()
{
}

proxy_renderable::proxy_renderable (platform_wnd_interface*) noexcept
{
}

proxy_renderable::proxy_renderable (proxy_renderable const&) noexcept
{
}

proxy_renderable& proxy_renderable::operator = (platform_wnd_interface*) noexcept
{
    return *this;
}

proxy_renderable& proxy_renderable::operator = (proxy_renderable const& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

void proxy_renderable::set_geometry (rect const&) noexcept
{
}

void proxy_renderable::set_owner (const_pointer) noexcept
{
}

void proxy_renderable::map () noexcept
{
    if (_M_pParent == nullptr) return;
}

void proxy_renderable::unmap () noexcept
{
    if (_M_pParent == nullptr) return;
}

void proxy_renderable::move (point2i gPos) noexcept
{
    if (_M_pParent != nullptr)
    {
        _M_gRect = rect (gPos.x, gPos.y, _M_gRect.width (), _M_gRect.height ());
    }
}

// ====================================================

} } // namespace Ui
