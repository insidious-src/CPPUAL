/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#include <freetype2/ft2build.h>
#include <freetype2/freetype/freetype.h>

#include <iostream>

namespace cppual { namespace Graphics {

namespace { namespace FreeType { // optimize for internal unit usage

typedef ::FT_Library   library_pointer ;
typedef ::FT_Face      face_pointer    ;
typedef ::FT_GlyphSlot glyph_pointer   ;
typedef ::FT_List      list_pointer    ;
typedef ::FT_ListNode  listnode_pointer;

inline library_pointer init () noexcept
{
    library_pointer lib;
    return ::FT_Init_FreeType (&lib) == 0 ? lib : nullptr;
}

inline library_pointer instance () noexcept
{
    static const library_pointer lib = init ();
    return lib;
}

} } // anonymous namespace FreeType

// =========================================================

Font::Atlas::Atlas (string_type const& gName)
: m_gName (gName),
  m_pFace ()
{
    FreeType::face_pointer tmp_handle;

    if (::FT_New_Face (FreeType::instance (), gName.c_str (), 0, &tmp_handle))
        std::cerr << "Couldn't load font " << gName << std::endl;
    else
        m_pFace = tmp_handle;
}

Font::Atlas::~Atlas ()
{
    if (m_pFace) ::FT_Done_Face (static_cast<FreeType::face_pointer> (m_pFace));
}

// =========================================================

Font::Font ()
{
}

Font::~Font ()
{
}

} } // namespace Graphics
