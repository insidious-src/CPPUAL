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

#include <cppual/gfx/gl/rectangle.h>
#include <cppual/gfx/gl/gldef.h>

//#include <iostream>

namespace cppual { namespace gfx { namespace gl {

void rectangle::draw (transform2d const& transform_info)
{
    ::GLfloat const x = transform_info.geometry ().left;
    ::GLfloat const y = transform_info.geometry ().top;
    ::GLfloat const w = transform_info.geometry ().width ();
    ::GLfloat const h = transform_info.geometry ().height ();

    ::glBegin (GL_QUADS);

    ::glColor3f (static_cast<::GLfloat> (_M_gFillColor.red   ()) / 255.f,
                 static_cast<::GLfloat> (_M_gFillColor.green ()) / 255.f,
                 static_cast<::GLfloat> (_M_gFillColor.blue  ()) / 255.f);

    ::glVertex2f (x, y);
    ::glVertex2f (x + w, y);
    ::glVertex2f (x + w, y + h);
    ::glVertex2f (x, y + h);

    ::glEnd ();

    ::glLineWidth (static_cast<::GLfloat> (_M_uSize));

    ::glBegin (GL_LINE_LOOP);

    ::glColor3f (static_cast<::GLfloat> (_M_gOuterColor.red   ()) / 255.f,
                 static_cast<::GLfloat> (_M_gOuterColor.green ()) / 255.f,
                 static_cast<::GLfloat> (_M_gOuterColor.blue  ()) / 255.f);

    ::glVertex2f(x, y);
    ::glVertex2f(x + w, y);
    ::glVertex2f(x + w, y + h);
    ::glVertex2f(x, y + h);

    ::glEnd ();
}

} } } // namespace OpenGL