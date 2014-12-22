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

#include <cppual/platform/manager.h>

namespace cppual { namespace Graphics {

namespace {

struct Internal { static thread_local IDeviceContext* current; };
thread_local IDeviceContext* Internal::current = nullptr;

} // anonymous namespace

IDeviceContext* IDeviceContext::current () noexcept
{
	return Internal::current;
}

void IDeviceContext::acquire (IDeviceContext* pContext) noexcept
{
	if (Internal::current and pContext and Internal::current->type () != pContext->type ())
		Internal::current->release ();

	Internal::current = pContext;
}

} } // namespace Graphics
