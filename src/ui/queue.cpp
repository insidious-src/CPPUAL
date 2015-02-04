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

#include <cppual/ui/manager.h>

using std::string;
using cppual::Platform::Factory;

namespace cppual { namespace Ui {

namespace { namespace Internal { // optimize for internal unit usage

inline shared_queue& queue () noexcept
{
	static shared_queue platform_queue;
	return platform_queue;
}

void attachQueue ()
{
	if (!Factory::hasValidInstance ()) return;
	Internal::queue () = Factory::instance ()->createQueueObject ();
}

} } // anonymous namespace Internal

// =========================================================

IDisplayQueue* IDisplayQueue::instance ()
{
	if (Internal::queue () == nullptr) Internal::attachQueue ();
	return Internal::queue ().get ();
}

bool IDisplayQueue::hasValidInstance () noexcept
{
	return Internal::queue () != nullptr;
}

} } // namespace Input
