/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#include <string>
#include <vector>
#include <cppual/flags.h>
#include <cppual/memory/allocator.h>

using std::string;

namespace cppual { namespace Graphics {

class Font
{
public:
    enum Style
    {
        Undecorated = 0,
        Italic        = 1 << 0,
        Bold        = 1 << 1,
        Underlined    = 1 << 2,
        Overlined    = 1 << 3,
        Erased        = 1 << 4
    };

    enum class SubPixel : unsigned char
    {
        None = 0,
        RGB,
        BGR,
        VerticalRGB,
        VerticalBGR
    };

    typedef std::size_t                              size_type;
    typedef Memory::GenericPolicy<uint>              allocator_type;
    typedef std::vector<uint, allocator_type>        vector_type;
    typedef BitSet<Font::Style>                      Styles;
    typedef struct { u8 spacing, size, lineHeight; } Format;

    class Atlas
    {
    public:
        Atlas (string const& name);
        ~Atlas ();

        Atlas () noexcept
        : m_gName (), m_pFace ()
        { }

        friend class Font;

    private:
        string m_gName;
        void*  m_pFace;
    };

    Font ();
    Font (Font&&);
    Font (Font const&);
    Font& operator = (Font&&);
    Font& operator = (Font const&);
    ~Font ();

    Font (Atlas const&,
          allocator_type&,
          int  kerning    = 0,
          bool anti_alias = true,
          SubPixel        = SubPixel::None);

    SubPixel subPixelRendering ();
    bool     isAntiAliased () const;
    int      kerning () const;
    void     glyph (size_type idx) const;
    void     setKerning (int);

private:
    vector_type m_gPixels;
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_TEXT_H_
