/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

namespace cppual { namespace Graphics {

enum class LineStyle : char { Solid, Dash, Dot, DashDotDot };

/**
 * @class Line
 * @brief A drawable that creates non-transformable line using
 * CRect coordinates
 */

class Line : public IDrawable2D
{
public:
    void draw (Transform2D const& transform_info);

    Color     getColor ()              const noexcept { return m_gColor;   }
    uint      getSize  ()              const noexcept { return m_uSize;    }
    LineStyle getStyle ()              const noexcept { return m_eStyle;   }
    void      setColor (Color const& gColor) noexcept { m_gColor = gColor; }
    void      setSize  (uint uSize)          noexcept { m_uSize = uSize;   }
    void      setStyle (LineStyle eStyle)    noexcept { m_eStyle = eStyle; }

    Line (const Color& gFillColor,
          uint         uLineSize = 1U,
          LineStyle    eStyle    = LineStyle::Solid) noexcept
    : m_gColor (gFillColor), m_uSize (uLineSize), m_eStyle (eStyle)
    { }

    Line (Line const& gObj) noexcept
    : m_gColor (gObj.m_gColor), m_uSize (gObj.m_uSize), m_eStyle (gObj.m_eStyle)
    { }

    Line& operator = (Line const& gObj) noexcept
    {
        m_gColor = gObj.m_gColor;
        m_uSize  = gObj.m_uSize ;
        m_eStyle = gObj.m_eStyle;
        return *this;
    }

private:
    Color     m_gColor;
    uint      m_uSize ;
    LineStyle m_eStyle;
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_LINE_H_
