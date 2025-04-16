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

#include <cppual/gfx/gl/line.h>
#include <cppual/gfx/gl/gldef.h>

//#include <iostream>

namespace cppual { namespace gfx { namespace gl {

void line::draw (transform const& transform_info)
{
    ::GLfloat x = transform_info.geometry ().left  ;
    ::GLfloat y = transform_info.geometry ().top   ;
    ::GLfloat r = transform_info.geometry ().right ;
    ::GLfloat b = transform_info.geometry ().bottom;

    if (_M_eStyle != line_style::solid)
    {
        ::glEnable (GL_LINE_STIPPLE);

        ::GLint    factor  = _M_eStyle == line_style::dot || _M_eStyle == line_style::dash_dot_dot ? 2 : 10;
        ::GLushort pattern = _M_eStyle == line_style::dash_dot_dot ? 0x050f : 0x5555;

        ::glLineStipple (factor, pattern);
    }

    ::glLineWidth (static_cast<::GLfloat> (_M_uSize));

    ::glBegin (GL_LINES);

    ::glColor3f (static_cast<::GLfloat> (_M_gColor.red   ()) / 255.f,
                 static_cast<::GLfloat> (_M_gColor.green ()) / 255.f,
                 static_cast<::GLfloat> (_M_gColor.blue  ()) / 255.f);

    ::glVertex2f(x, y);
    ::glVertex2f(r, b);

    ::glEnd();

    if (_M_eStyle != line_style::solid) ::glDisable (GL_LINE_STIPPLE);
}

void path::draw (transform const& /*transform_info*/)
{
    if (_M_eStyle != line_style::solid)
    {
        ::glEnable (GL_LINE_STIPPLE);

        ::GLint    factor  = _M_eStyle == line_style::dot || _M_eStyle == line_style::dash_dot_dot ? 2 : 10;
        ::GLushort pattern = _M_eStyle == line_style::dash_dot_dot ? 0x050f : 0x5555;

        ::glLineStipple (factor, pattern);
    }

    ::glLineWidth (static_cast<::GLfloat> (_M_uSize));

    ::glBegin (GL_LINE_STRIP);

    ::glColor3f (static_cast<::GLfloat> (_M_gColor.red   ()) / 255.f,
                 static_cast<::GLfloat> (_M_gColor.green ()) / 255.f,
                 static_cast<::GLfloat> (_M_gColor.blue  ()) / 255.f);

    for (auto i = 0U; i < _M_coord.size (); ++i)
    {
        ::glVertex2i (_M_coord[i].x, _M_coord[i].y);
    }

    ::glEnd ();

    if (_M_eStyle != line_style::solid) ::glDisable (GL_LINE_STIPPLE);
}

} } } // namespace OpenGL