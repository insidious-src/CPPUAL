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

#ifndef CPPUAL_GFX_COLOR_H_
#define CPPUAL_GFX_COLOR_H_
#ifdef __cplusplus

#include <cppual/types.h>
#include <type_traits>

namespace cppual { namespace Graphics {

union   RGBColor  ;
struct  CMYKColor ;
struct  HSLColor  ;
struct  YUVColor  ;
struct  YCbCrColor;
struct  YPbPrColor;

// =========================================================

typedef RGBColor Color;

// =========================================================

enum class ColorName : uchar
{
    White ,
    Black ,
    Grey  ,
    Red   ,
    Green ,
    Blue  ,
    Yellow,
    Brown ,
    Orange
};

enum class ColorType : uchar
{
    TrueType,
    Direct  ,
    Pseudo  ,
    Static  ,
    Grey    ,
    StaticGrey
};

// =========================================================

union RGBColor
{
    typedef std::size_t size_type;

    enum
    {
        Red,
        Green,
        Blue,
        Count
    };

    byte idx[Count];
    byte red, green, blue;

    RGBColor () noexcept = default;

    // right to left in binary mode
    constexpr RGBColor binary () const noexcept
    { return { idx[Blue], idx[Green], idx[Red] }; }

    constexpr byte const& operator [] (size_type i) const noexcept
    { return idx[i]; }

    constexpr RGBColor (byte r, byte g, byte b) noexcept : idx { r, g, b } { }
    byte& operator [] (size_type i) noexcept { return idx[i]; }
};

constexpr bool operator == (RGBColor const& gObj1, RGBColor const& gObj2) noexcept
{
    return (gObj1.red   == gObj2.red   and
            gObj1.green == gObj2.green and
            gObj1.blue  == gObj2.blue);
}

constexpr bool operator != (RGBColor const& gObj1, RGBColor const& gObj2) noexcept
{
    return (gObj1.red != gObj2.red or
                         gObj1.green != gObj2.green or
                                        gObj1.blue != gObj2.blue);
}

static_assert (sizeof (RGBColor) == 3, "RGBColor is not 24-bit!");
static_assert (std::is_pod<RGBColor>::value, "RGBColor is not POD!");

// =========================================================

struct CMYKColor
{ u8 cyan, magenta, yellow, black; };

// =========================================================

struct HSLColor
{ u8 hue, saturation, luminance; };

// =========================================================

struct YUVColor
{ double y, u, v; };

// =========================================================

struct YCbCrColor
{ u8 luminance, Cb, Cr; };

// =========================================================

struct YPbPrColor
{ u8 luminance, Pb, Pr; };

// =========================================================

struct Gradient
{ RGBColor color; u8 position; };

inline bool operator == (Gradient const& gObj1, Gradient const& gObj2) noexcept
{ return (gObj1.position == gObj2.position and gObj1.color == gObj2.color); }

inline bool operator != (Gradient const& gObj1, Gradient const& gObj2) noexcept
{ return (gObj1.position != gObj2.position or gObj1.color != gObj2.color); }

// =========================================================

RGBColor   colorFromName       (ColorName)         noexcept;
RGBColor   colorFromHexValue   (u8 value)          noexcept; // ex. 0xff0000
RGBColor   CMYKtoRGBColor      (CMYKColor  const&) noexcept;
RGBColor   HSLtoRGBColor       (HSLColor   const&) noexcept;
RGBColor   YUVtoRGBColor       (YUVColor   const&) noexcept;
RGBColor   YCbCrtoRGBColor     (YCbCrColor const&) noexcept;
RGBColor   YPbPrtoRGBColor     (YPbPrColor const&) noexcept;
RGBColor   YCbCrtoRGBColorHDTV (YCbCrColor const&) noexcept;
RGBColor   YPbPrtoRGBColorHDTV (YPbPrColor const&) noexcept;

CMYKColor  RGBtoCMYKColor      (RGBColor   const&) noexcept;
HSLColor   RGBtoHSLColor       (RGBColor   const&) noexcept;
YUVColor   RGBtoYUVColor       (RGBColor   const&) noexcept;
YCbCrColor RGBtoYCbCrColor     (RGBColor   const&) noexcept;
YPbPrColor RGBtoYPbPrColor     (RGBColor   const&) noexcept;
YCbCrColor RGBtoYCbCrColorHDTV (RGBColor   const&) noexcept;
YPbPrColor RGBtoYPbPrColorHDTV (RGBColor   const&) noexcept;

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_COLOR_H_
