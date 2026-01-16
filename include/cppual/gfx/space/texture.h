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

#ifndef CPPUAL_GFX_SPACE_TEXTURE_H_
#define CPPUAL_GFX_SPACE_TEXTURE_H_
#ifdef __cplusplus

#include <cppual/gfx/drawable/image.h>
#include <cppual/gfx/draw.h>

using string;

namespace cppual { namespace Graphics {

enum class TexMinFilter : unsigned char
{
    Nearest,
    Bilinear,
    NearestMipMap,
    BilinearMipMap,
    TrilinearMipMap
};

enum class TexMagFilter : bool
{
    Nearest,
    Bilinear
};

enum class TexState : unsigned char
{
    DirectAccess
};

// ====================================================

class Texture final : public ITransformable2D
{
public:
    Texture () noexcept;
    ~Texture () noexcept;
    static void setState (TexState type, bool state) noexcept;
    bool loadTexture2D (RasterImage const& image, bool gen_mipmap = false);
    void setParameter (uint name, int param) noexcept;
    void bind (uint texture_id) noexcept;
    void setFiltering (TexMagFilter mag, TexMinFilter min) noexcept;
    void release () noexcept;

    Texture (string const&      file,
             PixelFormat const& format     = PixelFormat (),
             bool               gen_mipmap = false);

    bool loadTexture2D (cvoid*             pixels,
                        point2i const&     size,
                        PixelFormat const& format     = PixelFormat (),
                        bool               gen_mipmap = false);

    bool loadTexture2D (string const&      path,
                        PixelFormat const& format     = PixelFormat (),
                        bool               gen_mipmap = false);

    constexpr TexMinFilter getMinFilter () const noexcept { return _M_eMin; }
    constexpr TexMagFilter getMagFilter () const noexcept { return _M_eMag; }
    constexpr PixelFormat  format () const noexcept { return _M_gFormat; }
    constexpr uint            sampleId () const noexcept { return _M_uSampleId; }

    constexpr bool    isLoaded () const noexcept
    { return _M_gStates.hasFlag (Texture::IsLoaded); }

    constexpr bool    hasMipMaps () const noexcept
    { return _M_gStates.hasFlag (Texture::HasMipMaps); }

private:
    enum    State { IsLoaded = 1 << 0, HasMipMaps = 1 << 1 };
    typedef FlagSet<Texture::State, u8> States;

    PixelFormat  _M_gFormat;
    point2i      _M_gSize;
    uint         _M_uSampleId;
    TexMinFilter _M_eMin;
    TexMagFilter _M_eMag;
    States       _M_gStates;
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_SPACE_TEXTURE_H_
