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

#include <cppual/gfx/gl/text.h>
#include <cppual/gfx/gl/gldef.h>

//#include <iostream>

namespace cppual { namespace gfx { namespace gl {

label2d::label2d ()
: _M_pFont (), _M_gText ()
{
}

label2d::label2d (string_type const& gName, font& gFont)
: _M_pFont (&gFont),
  _M_gText ( gName)
{
}

label2d::label2d (label2d const& gObj)
: _M_pFont (gObj._M_pFont),
  _M_gText (gObj._M_gText)
{
}

label2d::label2d (label2d&& gObj) noexcept
: _M_pFont (gObj._M_pFont),
  _M_gText (std::move (gObj._M_gText))
{
}

label2d& label2d::operator = (label2d const&)
{
    return *this;
}

label2d& label2d::operator = (label2d&&) noexcept
{
    return *this;
}

void label2d::draw (transform2d const& /*transform*/)
{
    if (_M_gText.empty ()) return;

//    glBindTexture (GL::Texture2D, 0);
//    glBegin (GL::Quads);

//    for (std::size_t i = 0; i < _M_gText.length (); ++i)
//    {
//        //glTexCoord2f (glyph->tex_x1, glyph->tex_y1);
//        glVertex2i (transform.geometry ().left, transform.geometry ().top);
//        //glTexCoord2f (glyph->tex_x1, glyph->tex_y1 + _tex_line_height);
//        //glVertex2i (transform.x (), transform.y () + font ().line_height);
//        //glTexCoord2f (glyph->tex_x2, glyph->tex_y1 + _tex_line_height);
//        //glVertex2i (transform.x () + glyph->advance, transform.y () + _M_gFont.height);
//        //glTexCoord2f (glyph->tex_x2, glyph->tex_y1);
//        //glVertex2i (transform.x () + glyph->advance, transform.y ());
//    }

//    glEnd ();
}

} } } // namespace OpenGL
