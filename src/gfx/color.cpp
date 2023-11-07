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

#include <cppual/gfx/color.h>

namespace cppual { namespace gfx {

color color_from_name (color_name eName) noexcept
{
    switch (eName)
    {
    case color_name::white:
        return { 255, 255, 255 };
    case color_name::grey:
        return { 150, 150, 150 };
    case color_name::red:
        return { 255, 0, 0 };
    case color_name::green:
        return { 0, 255, 0 };
    case color_name::blue:
        return { 0, 0, 255 };
    case color_name::yellow:
        return { 0, 0, 0 };
    case color_name::brown:
        return { 0, 0, 0 };
    case color_name::orange:
        return { 0, 0, 0 };
    default:
        return { 0, 0, 0 };
    }
}

color color_from_hex_value (u8) noexcept
{
    return color ();
}

// =========================================================

color cmyk_to_rgb_color (cmyk_color const& gCMYK) noexcept
{
    return
    {
        u8 (gCMYK.yellow + gCMYK.magenta),
        u8 (gCMYK.cyan   + gCMYK.yellow ),
        u8 (gCMYK.cyan   + gCMYK.magenta)
    };
}

color yuv_to_rgb_color  (const yuv_color& gYUV) noexcept
{
    return
    {
        u8 (gYUV.y +  0       * gYUV.u +  1.13983 * gYUV.v),
        u8 (gYUV.y + -0.39465 * gYUV.u + -0.58060 * gYUV.v),
        u8 (gYUV.y + -0.03211 * gYUV.u +  0       * gYUV.v)
    };
}

color hsl_to_rgb_color (const hsl_color&) noexcept
{
    return { 0, 0, 0 };
}

color ycb_cr_to_rgb_color  (const ycb_cr_color&) noexcept
{
    return { 0, 0, 0 };
}

color ypb_pr_to_rgb_color  (const ypb_pr_color&) noexcept
{
    return { 0, 0, 0 };
}

color ycb_cr_to_rgb_color_hdtv (const ycb_cr_color&) noexcept
{
    return { 0, 0, 0 };
}

color ypb_pr_to_rgb_color_hdtv (const ypb_pr_color&) noexcept
{
    return { 0, 0, 0 };
}

cmyk_color rgb_to_cmyk_color (color const& gRgb) noexcept
{
    return
    {
        u8 (gRgb.blue () + gRgb.green ()),
        u8 (gRgb.red ()  + gRgb.blue () ),
        u8 (gRgb.red ()  + gRgb.green ()),
        0
    };
}

hsl_color rgb_to_hsl_color (color const&) noexcept
{
    return { 0, 0, 0 };
}

yuv_color rgb_to_yuv_color (color const& gRgb) noexcept
{
    return
    {
        double ( 0.299  * gRgb.red () + 0.587  * gRgb.green () + 0.114 * gRgb.blue ()      ),
        double (-0.1687 * gRgb.red () - 0.3313 * gRgb.green () + 0.5   * gRgb.blue () + 128),
        double ( 0.5    * gRgb.red () - 0.4187 * gRgb.green () - 0.813 * gRgb.blue () + 128)
    };
}

ycb_cr_color rgb_to_ycb_cr_color (color const&) noexcept
{
    return { 0, 0, 0 };
}

ypb_pr_color rgb_to_ypb_pr_color (color const&) noexcept
{
    return { 0, 0, 0 };
}

ycb_cr_color rgb_to_ycb_cr_color_hdtv (color const&) noexcept
{
    return { 0, 0, 0 };
}

ypb_pr_color rgb_to_ypb_pr_color_hdtv (color const&) noexcept
{
    return { 0, 0, 0 };
}

} } // namespace Graphics
