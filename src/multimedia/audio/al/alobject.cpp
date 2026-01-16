/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#include <cppual/multimedia/audio/al/alobject.h>

#include "aldef.h"

namespace cppual { namespace audio { namespace al {

std::atomic_uint object::sm_uALObjCount { 0 };

// =========================================================

namespace { // optimize for internal unit usage

constexpr uint generate_object (object_type eType) noexcept
{
    ::ALuint n = 0;

    switch (eType)
    {
    case object_type::buffer:
        ::alGenBuffers (1, &n);
        break;
    case object_type::source:
        ::alGenSources (1, &n);
        break;
    }

    return n;
}

} // anonymous namespace

// =========================================================

object::object (object_type eType) noexcept
: resource (generate_object (eType)), _M_eObjType (eType)
{
    if (valid ()) ++sm_uALObjCount;
}

object::object (object&& gObj) noexcept
: resource     (std::move (gObj)),
  _M_eObjType  (gObj._M_eObjType)
{
}

object& object::operator = (object&& gObj) noexcept
{
    if (this != &gObj)
    {
        reset ();

        resource::operator = (std::move (gObj));

        _M_eObjType = gObj._M_eObjType;
    }

    return *this;
}

void object::reset () noexcept
{
    if (!valid ()) return;

    auto id = handle<::ALuint> ();

    switch (_M_eObjType)
    {
    case object_type::buffer:
        ::alDeleteBuffers (1, &id);
        break;
    case object_type::source:
        ::alDeleteSources (1, &id);
        break;
    }

    --sm_uALObjCount;
}

} } } // namespace Audio
