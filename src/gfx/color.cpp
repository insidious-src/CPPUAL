/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#include <cppual/gfx/color.h>

namespace cppual { namespace Graphics {

Color colorFromName (ColorName eName) noexcept
{
    switch (eName)
    {
    case ColorName::White:
        return { 255, 255, 255 };
    case ColorName::Grey:
        return { 150, 150, 150 };
    case ColorName::Red:
        return { 255, 0, 0 };
    case ColorName::Green:
        return { 0, 255, 0 };
    case ColorName::Blue:
        return { 0, 0, 255 };
    case ColorName::Yellow:
        return { 0, 0, 0 };
    case ColorName::Brown:
        return { 0, 0, 0 };
    case ColorName::Orange:
        return { 0, 0, 0 };
    default:
        return { 0, 0, 0 };
    }
}

Color colorFromHexValue (u8) noexcept
{
    return Color ();
}

// =========================================================

Color CMYKtoRGBColor (CMYKColor const& gCMYK) noexcept
{
    return
    {
        u8 (gCMYK.yellow + gCMYK.magenta),
        u8 (gCMYK.cyan   + gCMYK.yellow ),
        u8 (gCMYK.cyan   + gCMYK.magenta)
    };
}

Color YUVtoRGBColor (const YUVColor& gYUV) noexcept
{
    return
    {
        u8 (gYUV.y +  0       * gYUV.u +  1.13983 * gYUV.v),
        u8 (gYUV.y + -0.39465 * gYUV.u + -0.58060 * gYUV.v),
        u8 (gYUV.y + -0.03211 * gYUV.u +  0       * gYUV.v)
    };
}

Color HSLtoRGBColor (const HSLColor&) noexcept
{
    return { 0, 0, 0 };
}

Color YCbCrtoRGBColor (const YCbCrColor&) noexcept
{
    return { 0, 0, 0 };
}

Color YPbPrtoRGBColor (const YPbPrColor&) noexcept
{
    return { 0, 0, 0 };
}

Color YCbCrtoRGBColorHDTV (const YCbCrColor&) noexcept
{
    return { 0, 0, 0 };
}

Color YPbPrtoRGBColorHDTV (const YPbPrColor&) noexcept
{
    return { 0, 0, 0 };
}

CMYKColor RGBtoCMYKColor (Color const& gRgb) noexcept
{
    return
    {
        u8 (gRgb.blue + gRgb.green),
        u8 (gRgb.red  + gRgb.blue ),
        u8 (gRgb.red  + gRgb.green),
        0
    };
}

HSLColor RGBtoHSLColor (Color const&) noexcept
{
    return { 0, 0, 0 };
}

YUVColor RGBtoYUVColor (Color const& gRgb) noexcept
{
    return
    {
        double ( 0.299  * gRgb.red + 0.587  * gRgb.green + 0.114 * gRgb.blue      ),
        double (-0.1687 * gRgb.red - 0.3313 * gRgb.green + 0.5   * gRgb.blue + 128),
        double ( 0.5    * gRgb.red - 0.4187 * gRgb.green - 0.813 * gRgb.blue + 128)
    };
}

YCbCrColor RGBtoYCbCrColor (Color const&) noexcept
{
    return { 0, 0, 0 };
}

YPbPrColor RGBtoYPbPrColor (Color const&) noexcept
{
    return { 0, 0, 0 };
}

YCbCrColor RGBtoYCbCrColorHDTV (Color const&) noexcept
{
    return { 0, 0, 0 };
}

YPbPrColor RGBtoYPbPrColorHDTV (Color const&) noexcept
{
    return { 0, 0, 0 };
}

} } // namespace Graphics
