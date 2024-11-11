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

#ifndef CPPUAL_GFX_DRAWABLE_LINE_H_
#define CPPUAL_GFX_DRAWABLE_LINE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>

namespace cppual { namespace gfx {

/**
 * @class gl_line
 * @brief A drawable that creates a line using
 * rect coordinates
 */

class gl_line : public drawable2d_interface
{
public:
    void draw (transform2d const& transform_info);

    color      get_color ()              const noexcept { return _M_gColor;   }
    uint       size      ()              const noexcept { return _M_uSize;    }
    line_style style     ()              const noexcept { return _M_eStyle;   }
    void       set_color (color const& gColor) noexcept { _M_gColor = gColor; }
    void       set_size  (uint uSize)          noexcept { _M_uSize = uSize;   }
    void       set_style (line_style eStyle)   noexcept { _M_eStyle = eStyle; }

    constexpr gl_line () noexcept = default;

    gl_line (color const& gFillColor,
             uint         uLineSize = 1U,
             line_style   eStyle    = line_style::solid) noexcept
    : _M_gColor (gFillColor), _M_uSize (uLineSize), _M_eStyle (eStyle)
    { }

    gl_line (gl_line const& gObj) noexcept
    : _M_gColor (gObj._M_gColor), _M_uSize (gObj._M_uSize), _M_eStyle (gObj._M_eStyle)
    { }

    device_backend type () const noexcept
    {
        return device_backend::gl;
    }

    gl_line& operator = (gl_line const& gObj) noexcept
    {
        if (this == &gObj) return *this;

        _M_gColor = gObj._M_gColor;
        _M_uSize  = gObj._M_uSize ;
        _M_eStyle = gObj._M_eStyle;
        return *this;
    }

private:
    color      _M_gColor { };
    uint       _M_uSize  { };
    line_style _M_eStyle { };
};

/**
 * @class gl_path
 * @brief A drawable that creates a line path using
 * rect coordinates
 */

class gl_path : public drawable2d_interface
{
public:
    void draw (transform2d const& transform_info);

    color      get_color ()              const noexcept { return _M_gColor;   }
    uint       size      ()              const noexcept { return _M_uSize;    }
    line_style style     ()              const noexcept { return _M_eStyle;   }
    void       set_color (color const& gColor) noexcept { _M_gColor = gColor; }
    void       set_size  (uint uSize)          noexcept { _M_uSize  = uSize ; }
    void       set_style (line_style eStyle)   noexcept { _M_eStyle = eStyle; }

    inline gl_path () noexcept = default;

    gl_path (vector<point2i> const& coord,
             color           const& gFillColor,
             uint                   uLineSize = 1U,
             line_style             eStyle    = line_style::solid) noexcept
    : _M_coord (coord),
      _M_gColor(gFillColor),
      _M_uSize (uLineSize),
      _M_eStyle(eStyle)
    { }

    gl_path (gl_path const& gObj) noexcept
    : _M_gColor (gObj._M_gColor), _M_uSize (gObj._M_uSize), _M_eStyle (gObj._M_eStyle)
    { }

    device_backend type () const noexcept
    {
        return device_backend::gl;
    }

    gl_path& operator = (gl_path const& gObj) noexcept
    {
        if (this == &gObj) return *this;

        _M_gColor = gObj._M_gColor;
        _M_uSize  = gObj._M_uSize ;
        _M_eStyle = gObj._M_eStyle;

        return *this;
    }

private:
    vector<point2i> _M_coord     ;
    color           _M_gColor { };
    uint            _M_uSize  { };
    line_style      _M_eStyle { };
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_LINE_H_
