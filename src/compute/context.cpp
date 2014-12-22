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

#include <cppual/compute/context.h>

namespace cppual { namespace Compute {

class ContextImpl
{
public:
	virtual bool acquire (Device&) = 0;
	virtual void release (Device&) = 0;
	virtual bool available (Device&) = 0;
};

Context::Context (DeviceGroup& gDevice)
: Object (new Disposable<Compute::Context>),
  m_gDevice (&gDevice)
{
}

} } // namespace CL
