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

#include <cppual/gfx/font.h>
#include <iostream>
#include <ft2build.h>
#include <freetype.h>

namespace cppual { namespace Graphics {

namespace { namespace FreeType { // optimize for internal unit usage

typedef FT_Library   font_library;
typedef FT_Face      font_face;
typedef FT_GlyphSlot font_glyph;
typedef FT_List      font_list;
typedef FT_ListNode  font_listnode;

inline font_library init () noexcept
{
	font_library lib;
	return FT_Init_FreeType (&lib) == 0 ? lib : nullptr;
}

inline font_library instance () noexcept
{
	static font_library lib = init ();
	return lib;
}

} } // anonymous namespace FreeType

// =========================================================

Font::Atlas::Atlas (string const& gName)
: m_gName (gName),
  m_pFace ()
{
	FreeType::font_face tmp_handle;

	if (FT_New_Face (FreeType::instance (), gName.c_str (), 0, &tmp_handle))
		std::cout << "Couldn't load font " << gName << std::endl;
	else
		m_pFace = tmp_handle;
}

Font::Atlas::~Atlas ()
{
	if (m_pFace) FT_Done_Face (static_cast<FreeType::font_face> (m_pFace));
}

// =========================================================

Font::Font ()
{
}

Font::~Font ()
{
}

} } // namespace Graphics
