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

#include <cppual/gfx/drawable/polygon.h>
#include <cppual/gfx/gl/gldef.h>

#include <iostream>

namespace cppual { namespace gfx {

void gl_polygon::draw (transform2d const& /*transform_info*/)
{
    ::glBegin (GL_TRIANGLES);

    ::glColor3f (static_cast<::GLfloat> (_M_gFillColor.red   ()) / 255.f,
                 static_cast<::GLfloat> (_M_gFillColor.green ()) / 255.f,
                 static_cast<::GLfloat> (_M_gFillColor.blue  ()) / 255.f);

    ::glVertex2f (_M_coord[0].x, _M_coord[0].y);
    ::glVertex2f (_M_coord[1].x, _M_coord[1].y);
    ::glVertex2f (_M_coord[2].x, _M_coord[2].y);

    ::glEnd ();

    ::glLineWidth (static_cast<::GLfloat> (_M_uSize));

    ::glBegin (GL_LINE_LOOP);

    ::glColor3f (static_cast<::GLfloat> (_M_gOuterColor.red   ()) / 255.f,
                 static_cast<::GLfloat> (_M_gOuterColor.green ()) / 255.f,
                 static_cast<::GLfloat> (_M_gOuterColor.blue  ()) / 255.f);

    ::glVertex2f (_M_coord[0].x, _M_coord[0].y);
    ::glVertex2f (_M_coord[1].x, _M_coord[1].y);
    ::glVertex2f (_M_coord[2].x, _M_coord[2].y);

    ::glEnd ();
}

} } // Graphics