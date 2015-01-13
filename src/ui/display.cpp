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

#include <iostream>
#include <cppual/ui/manager.h>

using std::string;
using std::shared_ptr;
using cppual::Platform::Factory;

namespace cppual { namespace Ui {

namespace {  namespace Internal { // optimize for internal unit usage

struct Initializer
{
	shared_display instance;
};

inline Initializer& backend () noexcept
{
	static Initializer back;
	return back;
}

} } // anonymous namespace Internal

// ====================================================

IDisplay* IDisplay::instance ()
{
	if (Internal::backend ().instance == nullptr) set (nullptr);
	return Internal::backend ().instance.get ();
}

bool IDisplay::hasValidInstance () noexcept
{
	return Internal::backend ().instance != nullptr;
}

bool IDisplay::set (cchar* pDevName)
{
	static bool bConnected = false;

	if (Factory::hasValidInstance ())
	{
		Internal::backend ().instance = Factory::instance ()->connectDisplay (pDevName);

		if (Internal::backend ().instance != nullptr and !Internal::backend ().instance->native ())
			Internal::backend ().instance.reset ();
	}

	if (!bConnected and Internal::backend ().instance != nullptr) return (bConnected = true);
	return Internal::backend ().instance != nullptr;
}

IDisplay::pointer IDisplay::connect (cchar* pDevName)
{
	if (Internal::backend ().instance != nullptr and
			Internal::backend ().instance->name () == pDevName)
		return Internal::backend ().instance;

	return Factory::hasValidInstance () ?
				std::move (Factory::instance ()->connectDisplay (pDevName)) :
				IDisplay::pointer ();
}

} } // namespace Ui
