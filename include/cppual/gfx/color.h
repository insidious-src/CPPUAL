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

#ifndef CPPUAL_GFX_COLOR_H_
#define CPPUAL_GFX_COLOR_H_
#ifdef __cplusplus

#include <cppual/types.h>

#include <type_traits>

namespace cppual { namespace gfx {

class  rgb_color   ;
struct cmyk_color  ;
struct hsl_color   ;
struct yuv_color   ;
struct ycb_cr_color;
struct ypb_pr_color;

// =========================================================

typedef rgb_color color;

// =========================================================

enum class color_name : u8
{
    white ,
    black ,
    grey  ,
    red   ,
    green ,
    blue  ,
    yellow,
    brown ,
    orange
};

enum class color_type : u8
{
    none       ,
    true_type  ,
    direct     ,
    pseudo     ,
    static_type,
    grey       ,
    static_grey
};

// =========================================================

class rgb_color
{
public:
    typedef std::size_t size_type;

    enum
    {
        red_idx  ,
        green_idx,
        blue_idx
    };

    rgb_color () noexcept = default;

    //! right to left in binary mode
    constexpr rgb_color binary () const noexcept
    { return { idx[blue_idx], idx[green_idx], idx[red_idx] }; }

    constexpr u8 operator [] (size_type i) const noexcept
    { return idx[i]; }

    constexpr rgb_color (u8 r, u8 g, u8 b) noexcept : idx { r, g, b } { }
    constexpr rgb_color (u8 rgb[3]) noexcept : idx { rgb[0], rgb[1], rgb[2] } { }
    constexpr u8    red () const noexcept { return idx[red_idx]; }
    constexpr u8  green () const noexcept { return idx[green_idx]; }
    constexpr u8   blue () const noexcept { return idx[blue_idx]; }

    u8& operator [] (size_type i) noexcept { return idx[i]; }

private:
    u8 idx[3];
};

constexpr bool operator == (rgb_color const& gObj1, rgb_color const& gObj2) noexcept
{
    return (gObj1.red ()   == gObj2.red ()   and
            gObj1.green () == gObj2.green () and
            gObj1.blue ()  == gObj2.blue ());
}

constexpr bool operator != (rgb_color const& gObj1, rgb_color const& gObj2) noexcept
{
    return (gObj1.red () != gObj2.red () or
                            gObj1.green () != gObj2.green () or
                                              gObj1.blue () != gObj2.blue ());
}

static_assert (sizeof (rgb_color) == sizeof (u8) * 3, "RGBColor is not 24-bit!");
static_assert (std::is_pod<rgb_color>::value, "RGBColor is not POD!");

// =========================================================

struct cmyk_color
{ u8 cyan, magenta, yellow, black; };

// =========================================================

struct hsl_color
{ u8 hue, saturation, luminance; };

// =========================================================

struct yuv_color
{ double y, u, v; };

// =========================================================

struct ycb_cr_color
{ u8 luminance, Cb, Cr; };

// =========================================================

struct ypb_pr_color
{ u8 luminance, Pb, Pr; };

// =========================================================

struct Gradient
{ rgb_color color; byte position; };

inline bool operator == (Gradient const& gObj1, Gradient const& gObj2) noexcept
{ return (gObj1.position == gObj2.position and gObj1.color == gObj2.color); }

inline bool operator != (Gradient const& gObj1, Gradient const& gObj2) noexcept
{ return (gObj1.position != gObj2.position or gObj1.color != gObj2.color); }

// =========================================================

rgb_color    color_from_name          (color_name)          noexcept;
rgb_color    color_from_hex_value     (u8   value)          noexcept; // ex. 0xff0000
rgb_color    cmyk_to_rgb_color        (cmyk_color   const&) noexcept;
rgb_color    hsl_to_rgb_color         (hsl_color    const&) noexcept;
rgb_color    yuv_to_rgb_color         (yuv_color    const&) noexcept;
rgb_color    ycb_cr_to_rgb_color      (ycb_cr_color const&) noexcept;
rgb_color    ypb_pr_to_rgb_color      (ypb_pr_color const&) noexcept;
rgb_color    ycb_cr_to_rgb_color_hdtv (ycb_cr_color const&) noexcept;
rgb_color    ypb_pr_to_rgb_color_hdtv (ypb_pr_color const&) noexcept;

cmyk_color   rgb_to_cmyk_color        (rgb_color   const&) noexcept;
hsl_color    rgb_to_hsl_color         (rgb_color   const&) noexcept;
yuv_color    rgb_to_yuv_color         (rgb_color   const&) noexcept;
ycb_cr_color rgb_to_ycb_cr_color      (rgb_color   const&) noexcept;
ypb_pr_color rgb_to_ypb_pr_color      (rgb_color   const&) noexcept;
ycb_cr_color rgb_to_ycb_cr_color_hdtv (rgb_color   const&) noexcept;
ypb_pr_color rgb_to_ypb_pr_color_hdtv (rgb_color   const&) noexcept;

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_COLOR_H_
