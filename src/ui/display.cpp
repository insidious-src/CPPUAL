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

#include <cppual/ui/manager.h>

namespace cppual { namespace ui {

// ====================================================

namespace { // optimize for internal unit usage

inline static shared_display& backend () noexcept
{
    static shared_display back;
    return back;
}

} // anonymous namespace

// ====================================================

display_interface::pointer display_interface::primary ()
{
    if (backend () == nullptr) primary ("");
    return backend ();
}

bool display_interface::has_valid_instance () noexcept
{
    return backend () != nullptr;
}

bool display_interface::primary (string_type const& dsp_name)
{
    static bool bConnected = false;

    if (platform::factory::has_valid_instance ())
        backend () = platform::factory::instance ()->connectDisplay (dsp_name);

    if (!bConnected and backend () != nullptr) return (bConnected = true);
    return backend () != nullptr;
}

display_interface::pointer display_interface::connect (string_type const& dsp_name)
{
    if (backend () != nullptr and backend ()->name () == dsp_name)
        return backend ();

    return platform::factory::has_valid_instance () ?
                platform::factory::instance ()->connectDisplay (dsp_name) :
                shared_display ();
}

} } // namespace Ui
