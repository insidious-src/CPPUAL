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

#ifndef CPPUAL_GFX_DRAWABLE_PAINTER_H_
#define CPPUAL_GFX_DRAWABLE_PAINTER_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>

namespace cppual { namespace gfx { namespace gl {

struct SHARED_API painter final : public painter_interface
{
public:
    painter  (shared_surface const& surface) noexcept;
    ~painter () noexcept;

    shared_drawable2d create_shape (u8 shape_type);

    shared_drawable2d create_line (color const& color_fill,
                                   uint         line_width,
                                   line_style   style);

    shared_drawable2d create_path (vector<point2i> const& coord,
                                   color           const& clr,
                                   uint                   line_width,
                                   line_style             style);

    shared_drawable2d create_ellipse (color fill, color outline, uint outline_size);
    shared_drawable2d create_rectangle (color fill, color outline, uint outline_width);
    shared_drawable2d create_polygon (polygon_array const& coord, color fill, color outline, uint outline_width);

    shared_drawable2d create_image (string_type  const& path,
                                    pixel_format const& format,
                                    color               mask);

    shared_drawable2d create_text (string_type const& text, font& txt_font);

private:
    shared_surface _M_pSurface;
};

} } } // OpenGL

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_PAINTER_H_
