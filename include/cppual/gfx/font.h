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

#ifndef CPPUAL_GFX_FONT_H_
#define CPPUAL_GFX_FONT_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/string.h>
#include <cppual/containers.h>

#include <string>
#include <vector>

namespace cppual { namespace gfx {

class font
{
public:
    enum style
    {
        undecorated =      0,
        italic      = 1 << 0,
        bold        = 1 << 1,
        underlined  = 1 << 2,
        overlined   = 1 << 3,
        erased      = 1 << 4
    };

    enum class sub_pixel : byte
    {
        none        ,
        rgb         ,
        bgr         ,
        vertical_rgb,
        vertical_bgr
    };

    typedef std::size_t                               size_type     ;
    typedef string                                    string_type   ;
    typedef memory::allocator<uint>                   allocator_type;
    typedef vector<uint>                              vector_type   ;
    typedef bitset<font::style>                       styles        ;
    typedef struct { u8 spacing, size, line_height; } format_type   ;

    class atlas
    {
    public:
        atlas  (string_type const& name);
        ~atlas ();

        atlas () noexcept
        : _M_gName (), _M_pFace ()
        { }

        friend class font;

    private:
        string_type _M_gName;
        void*       _M_pFace;
    };

    font ();
    font (font&&);
    font (font const&);
    font& operator = (font&&);
    font& operator = (font const&);
    ~font ();

    font (atlas const&,
          allocator_type&,
          int     kerning = 0,
          bool anti_alias = true,
          sub_pixel   sub = sub_pixel::none);

    sub_pixel sub_pixel_rendering  ()      ;
    bool      is_anti_aliased      () const;
    int       kerning              () const;
    void      set_kerning       (int)      ;
    void      glyph   (size_type idx) const;

private:
    vector_type _M_gPixels;
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_TEXT_H_
