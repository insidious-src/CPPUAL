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

#include <cppual/gfx/gl/painter.h>
#include <cppual/gfx/drawable/text.h>
#include <cppual/gfx/drawable/line.h>
#include <cppual/gfx/drawable/image.h>
#include <cppual/gfx/drawable/elipse.h>
#include <cppual/gfx/drawable/polygon.h>
#include <cppual/gfx/drawable/rectangle.h>
#include <cppual/gfx/gl/gldef.h>

namespace cppual { namespace gfx {

// =========================================================

gl_painter::gl_painter (shared_surface const& surface) noexcept
: _M_pSurface (surface)
{
    ::glPushMatrix ();

    ::glOrtho (0.f, surface->size ().x, surface->size ().y, 0.f, 0.f, 1.f);
}

gl_painter::~gl_painter () noexcept
{
    ::glPopMatrix ();
}

shared_drawable2d gl_painter::create_shape (u8 shape_type)
{
    switch (shape_type)
    {
    case static_cast<u8> (shape::image):
        return shared_drawable2d (new raster_image ());
    case static_cast<u8> (shape::text):
        return shared_drawable2d (new label2d ());
    case static_cast<u8> (shape::line):
        return shared_drawable2d (new gl_line ());
    case static_cast<u8> (shape::path):
        return shared_drawable2d (new gl_path ());
    case static_cast<u8> (shape::elipse):
        return shared_drawable2d (new gl_elipse ());
    case static_cast<u8> (shape::polygon):
        return shared_drawable2d (new gl_polygon ());
    case static_cast<u8> (shape::rectangle):
        return shared_drawable2d (new gl_rectangle ());
    default:
        return shared_drawable2d ();
    }
}

shared_drawable2d gl_painter::create_line (color const& color_fill,
                                           uint         line_size,
                                           line_style   style)
{
    return shared_drawable2d (new gl_line (color_fill, line_size, style));
}

shared_drawable2d gl_painter::create_path (vector<point2i> const& coord,
                                           color           const& clr,
                                           uint                   line_size,
                                           line_style             style)
{
    return shared_drawable2d (new gl_path (coord, clr, line_size, style));
}

shared_drawable2d gl_painter::create_elipse (color fill, color outline, uint outline_size)
{
    return shared_drawable2d (new gl_elipse (fill, outline, outline_size));
}

shared_drawable2d gl_painter::create_rectangle (color fill, color outline, uint outline_size)
{
    return shared_drawable2d (new gl_rectangle (fill, outline, outline_size));
}

shared_drawable2d gl_painter::create_polygon (polygon_array const& coord,
                                              color fill,
                                              color outline,
                                              uint outline_size)
{
    return shared_drawable2d (new gl_polygon (coord, fill, outline, outline_size));
}

shared_drawable2d gl_painter::create_image (string_type  const& path,
                                            pixel_format const& format,
                                            color               mask)
{
    return shared_drawable2d (new raster_image (path, format, mask));
}

shared_drawable2d gl_painter::create_text (string_type const& text, font& txt_font)
{
    return shared_drawable2d (new label2d (text, txt_font));
}

} } // namespace Graphics
