/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#include <string>
#include <cppual/ui/manager.h>

using std::string;
using std::shared_ptr;

namespace cppual { namespace Ui {

namespace {  namespace Internal { // optimize for internal unit usage

inline shared_display& backend () noexcept
{
    static shared_display back;
    return back;
}

} } // anonymous namespace Internal

// ====================================================

IDisplay* IDisplay::instance ()
{
    if (Internal::backend () == nullptr) set (nullptr);
    return Internal::backend ().get ();
}

bool IDisplay::hasValidInstance () noexcept
{
    return Internal::backend () != nullptr;
}

bool IDisplay::set (cchar* pDevName)
{
    static bool bConnected = false;

    if (Platform::Factory::hasValidInstance ())
    {
        Internal::backend () = Platform::Factory::instance ()->connectDisplay (pDevName);

        if (Internal::backend () != nullptr and !Internal::backend ()->native ())
            Internal::backend ().reset ();
    }

    if (!bConnected and Internal::backend () != nullptr) return (bConnected = true);
    return Internal::backend () != nullptr;
}

IDisplay::pointer IDisplay::connect (cchar* pDevName)
{
    if (Internal::backend () != nullptr and
            Internal::backend ()->name () == pDevName)
        return Internal::backend ();

    return Platform::Factory::hasValidInstance () ?
                std::move (Platform::Factory::instance ()->connectDisplay (pDevName)) :
                IDisplay::pointer ();
}

} } // namespace Ui
