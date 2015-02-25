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

namespace cppual { namespace Ui { namespace Platform {

namespace { namespace Internal { // optimize for internal unit usage

inline cchar* server () noexcept
{
#	if defined OS_GNU_LINUX or defined OS_BSD
	return getenv ("WAYLAND_DISPLAY") ? "libcppual-ui-wayland" : "libcppual-ui-xorg";
#	elif defined OS_WINDOWS
	return "libcppual-win";
#	endif
}

struct Initializer
{
    Module         mod;
    shared_manager mgr;

	inline Initializer () noexcept
    : mod (server (), true, Module::ResolvePolicy::Lazy),
	  mgr ()
	{ }
};

inline Initializer& platform () noexcept
{
	static Initializer init;
	return init;
}

inline shared_manager& instance () noexcept
{
	return platform ().mgr;
}

inline Module& module () noexcept
{
	return platform ().mod;
}

} } // anonymous namespace Internal

// ====================================================

Factory* Factory::instance ()
{
	if (Internal::instance () == nullptr)
	{
        if (!Internal::module ().contains ("module_main") or
                Internal::module   ().call<int> ("module_main", Internal::instance ()) or
				Internal::instance () == nullptr)
			throw std::bad_alloc ();
	}

	return Internal::instance ().get ();
}

Module& Factory::module ()
{
	return Internal::module ();
}

bool Factory::hasValidInstance () noexcept
{
	return Internal::instance () != nullptr;
}

} } } // namespace Platform
