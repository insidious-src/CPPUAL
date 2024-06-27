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

#include <cppual/gfx/gl/elipse.h>
#include <cppual/gfx/gl/gldef.h>

#include <iostream>
#include <cmath>
#include <algorithm>

namespace cppual { namespace gfx { namespace gl {

void ellipse::draw (transform2d const& transform_info)
{
    static constexpr ::GLfloat PI = 3.1415926f;

    float const theta = PI * 2 / 100.f;
    float const tangetial_factor = std::tanf(theta); //calculate the tangential factor

    float const radial_factor = std::cosf(theta); //calculate the radial factor

    u16   const coordArray[2] = { transform_info.geometry ().width (), transform_info.geometry ().height () };
    float const min           = static_cast<float> (*std::min_element (coordArray, coordArray + 2));

    float x = min / 2; //we start at angle = 0

    float y = 0;

    float const left = transform_info.geometry ().left + transform_info.geometry ().width  () / 2;
    float const top  = transform_info.geometry ().top  + transform_info.geometry ().height () / 2;

    ::glLineWidth (static_cast<::GLfloat> (_M_uSize));

    ::glBegin (GL_LINE_LOOP);

    ::glColor3f (static_cast<::GLfloat> (_M_gOuterColor.red   ()) / 255.f,
                 static_cast<::GLfloat> (_M_gOuterColor.green ()) / 255.f,
                 static_cast<::GLfloat> (_M_gOuterColor.blue  ()) / 255.f);

    for (int ii = 0; ii < 100; ++ii)
    {
        ::glVertex2f (x + left, y + top); //output vertex

        //calculate the tangential vector
        //remember, the radial vector is (x, y)
        //to get the tangential vector we flip those coordinates and negate one of them

        float tx = -y;
        float ty = x;

        //add the tangential vector

        x += tx * tangetial_factor;
        y += ty * tangetial_factor;

        //correct using the radial factor

        x *= radial_factor;
        y *= radial_factor;
    }

    glEnd ();

    std::cout << "draw an ellipse" << std::endl;
}

} } } // namespace OpenGL