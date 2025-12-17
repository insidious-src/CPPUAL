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

#ifndef CPPUAL_GFX_DRAWABLE_ELIPSE_H_
#define CPPUAL_GFX_DRAWABLE_ELIPSE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>

namespace cppual { namespace gfx { namespace gl {

/**
 * @class ellipse
 * @brief A drawable that creates a OpenGL elipse using
 * rect coordinates
 */

class ellipse : public drawable_interface
{
public:
    void draw (transform const& transform_info);

    color      get_fill_color  ()              const noexcept { return _M_gFillColor;    }
    color      get_outer_color ()              const noexcept { return _M_gOuterColor;   }
    uint       size            ()              const noexcept { return _M_uSize;         }
    void       set_fill_color  (color const& gColor) noexcept { _M_gFillColor  = gColor; }
    void       set_outer_color (color const& gColor) noexcept { _M_gOuterColor = gColor; }
    void       set_size        (uint uSize)          noexcept { _M_uSize = uSize;        }

    constexpr ellipse () noexcept = default;

    ellipse (color const& gFillColor,
               color const& gOutColor ,
               uint         uLineSize = 1U) noexcept
    : _M_gFillColor (gFillColor), _M_gOuterColor (gOutColor), _M_uSize (uLineSize)
    { }

    ellipse (ellipse const& gObj) noexcept
    : _M_gFillColor (gObj._M_gFillColor), _M_gOuterColor (gObj._M_gOuterColor), _M_uSize (gObj._M_uSize)
    { }

    device_backend type () const noexcept
    {
        return device_backend::gl;
    }

    ellipse& operator = (ellipse const& gObj) noexcept
    {
        if (this == &gObj) return *this;

        _M_gFillColor  = gObj._M_gFillColor ;
        _M_gOuterColor = gObj._M_gOuterColor;
        _M_uSize       = gObj._M_uSize      ;

        return *this;
    }

private:
    color _M_gFillColor  { };
    color _M_gOuterColor { };
    uint  _M_uSize       { };
};

} } } // namespace OpenGL

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_ELIPSE_H_
