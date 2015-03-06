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

#include <cppual/multimedia/audio/al/alobject.h>
#include "aldef.h"

using std::atomic_uint;

namespace cppual { namespace Audio { namespace AL {

atomic_uint AudioObject::sm_uALObjCount { 0 };

// =========================================================

namespace { // optimize for internal unit usage

inline uint generateObject (ObjectType eType) noexcept
{
	uint n = 0;

	switch (eType)
	{
	case ObjectType::Buffer:
		alGenBuffers (1, &n);
		break;
	case ObjectType::Source:
		alGenSources (1, &n);
		break;
	}

	return n;
}

} // anonymous

// =========================================================

AudioObject::AudioObject (ObjectType eType) noexcept
: m_uObjId (generateObject (eType)), m_eObjType (eType)
{
	if (m_uObjId) ++sm_uALObjCount;
}

AudioObject::AudioObject (AudioObject const& gObj) noexcept
: m_uObjId (generateObject (gObj.m_eObjType)), m_eObjType (gObj.m_eObjType)
{
	if (m_uObjId) ++sm_uALObjCount;
}

AudioObject::AudioObject (AudioObject&& gObj) noexcept
: m_uObjId (gObj.m_uObjId),
  m_eObjType (gObj.m_eObjType)
{
	gObj.m_uObjId = 0;
}

AudioObject& AudioObject::operator = (AudioObject&& gObj) noexcept
{
	if (this != &gObj)
	{
		reset ();
		m_uObjId   = gObj.m_uObjId;
		m_eObjType = gObj.m_eObjType;
		gObj.m_uObjId = 0;
	}

	return *this;
}

AudioObject& AudioObject::operator = (AudioObject const& gObj) noexcept
{
	if (this != &gObj and m_eObjType != gObj.m_eObjType)
	{
		reset ();
		m_uObjId   = generateObject (gObj.m_eObjType);
		m_eObjType = gObj.m_eObjType;
	}

	return *this;
}

void AudioObject::reset () noexcept
{
	if (!m_uObjId) return;

	switch (m_eObjType)
	{
	case ObjectType::Buffer:
		alDeleteBuffers (1, &m_uObjId);
		break;
	case ObjectType::Source:
		alDeleteSources (1, &m_uObjId);
		break;
	}

	--sm_uALObjCount;
}

} } } // namespace Audio
