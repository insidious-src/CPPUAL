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

#include <cppual/multimedia/audio/al/alobject.h>

#include "aldef.h"

namespace cppual { namespace audio { namespace al {

std::atomic_uint object::sm_uALObjCount { 0 };

// =========================================================

namespace { // optimize for internal unit usage

inline uint generate_object (object_type eType) noexcept
{
    uint n = 0;

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
: _M_uObjId (generate_object (eType)), _M_eObjType (eType)
{
    if (_M_uObjId) ++sm_uALObjCount;
}

object::object (object const& gObj) noexcept
: _M_uObjId (generate_object (gObj._M_eObjType)), _M_eObjType (gObj._M_eObjType)
{
    if (_M_uObjId) ++sm_uALObjCount;
}

object::object (object&& gObj) noexcept
: _M_uObjId   (gObj._M_uObjId),
  _M_eObjType (gObj._M_eObjType)
{
    gObj._M_uObjId = 0;
}

object& object::operator = (object&& gObj) noexcept
{
    if (this != &gObj)
    {
        reset ();

        _M_uObjId   = gObj._M_uObjId;
        _M_eObjType = gObj._M_eObjType;

        gObj._M_uObjId = 0;
    }

    return *this;
}

object& object::operator = (object const& gObj) noexcept
{
    if (this != &gObj and _M_eObjType != gObj._M_eObjType)
    {
        reset ();

        _M_uObjId   = generate_object (gObj._M_eObjType);
        _M_eObjType = gObj._M_eObjType;
    }

    return *this;
}

void object::reset () noexcept
{
    if (!_M_uObjId) return;

    switch (_M_eObjType)
    {
    case object_type::buffer:
        ::alDeleteBuffers (1, &_M_uObjId);
        break;
    case object_type::source:
        ::alDeleteSources (1, &_M_uObjId);
        break;
    }

    --sm_uALObjCount;
}

} } } // namespace Audio
