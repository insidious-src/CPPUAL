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

#include <ft2build.h>
#include <freetype.h>
#include <iostream>
#include <cppual/gfx/drawable/text.h>
#include "../gl/gldef.h"

namespace cppual { namespace Graphics {

Label2D::Label2D ()
: m_pFont (), m_gText ()
{
}

Label2D::Label2D (string const& gName, Font& gFont)
: m_pFont (&gFont),
  m_gText ( gName)
{
}

Label2D::Label2D (Label2D const& gObj)
: m_pFont (gObj.m_pFont),
  m_gText (gObj.m_gText)
{
}

Label2D::Label2D (Label2D&& gObj) noexcept
: m_pFont (gObj.m_pFont),
  m_gText (std::move (gObj.m_gText))
{
}

Label2D& Label2D::operator = (Label2D const&)
{
    return *this;
}

Label2D& Label2D::operator = (Label2D&&) noexcept
{
    return *this;
}

void Label2D::draw (Transform2D const& transform)
{
    if (m_gText.empty ()) return;

    glBindTexture (GL::Texture2D, 0);
    glBegin (GL::Quads);

    for (std::size_t i = 0; i < m_gText.length (); ++i)
    {
        //glTexCoord2f (glyph->tex_x1, glyph->tex_y1);
        glVertex2i (transform.geometry ().left, transform.geometry ().top);
        //glTexCoord2f (glyph->tex_x1, glyph->tex_y1 + _tex_line_height);
        //glVertex2i (transform.x (), transform.y () + font ().lineHeight);
        //glTexCoord2f (glyph->tex_x2, glyph->tex_y1 + _tex_line_height);
        //glVertex2i (transform.x () + glyph->advance, transform.y () + m_gFont.height);
        //glTexCoord2f (glyph->tex_x2, glyph->tex_y1);
        //glVertex2i (transform.x () + glyph->advance, transform.y ());
    }

    glEnd ();
}

} } // namespace Graphics
