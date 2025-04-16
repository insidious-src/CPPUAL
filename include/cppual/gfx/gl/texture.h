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

#ifndef CPPUAL_GFX_GL_TEXTURE_H_
#define CPPUAL_GFX_GL_TEXTURE_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/glbase.h>
#include <cppual/string.h>

namespace cppual { namespace gfx { namespace gl {

class texture : public object, transformable_interface
{
public:
    enum class MinFilter : u8
    {
        Nearest,
        Bilinear,
        NearestMipMap,
        BilinearMipMap,
        TrilinearMipMap
    };

    enum class MagFilter : u8
    {
        Nearest,
        Bilinear
    };

    enum class State : u8
    {
        DirectAccess
    };

    texture  () noexcept;
    ~texture () noexcept;
    static void set_state (State type, bool state) noexcept;
    //bool loadTexture2D (RasterImage const& image, bool gen_mipmap = false);
    void set_parameter (uint name, int param) noexcept;
    void bind (uint texture_id) noexcept;
    void set_filtering (MagFilter mag, MinFilter min) noexcept;
    void release () noexcept;

    texture (string       const& file,
             pixel_format const& format     = pixel_format (),
             bool                gen_mipmap = false);

    bool load_texture_2d (cvoid*             pixels,
                          point2i      const& size,
                          pixel_format const& format     = pixel_format (),
                          bool                gen_mipmap = false);

    bool load_texture_2d (string       const& path,
                          pixel_format const& format     = pixel_format (),
                          bool                gen_mipmap = false);

    MinFilter    min_filter () const noexcept { return _M_eMin     ; }
    MagFilter    mag_filter () const noexcept { return _M_eMag     ; }
    pixel_format format     () const noexcept { return _M_gFormat  ; }
    uint         sample     () const noexcept { return _M_uSampleId; }

    bool is_loaded () const noexcept
    { return _M_gStates.test (texture::loaded); }

    bool has_mip_maps () const noexcept
    { return _M_gStates.test (texture::has_mipmaps); }

private:
    enum    flag { loaded = 1 << 0, has_mipmaps = 1 << 1 };
    typedef bitset<texture::flag> flags;

    pixel_format _M_gFormat;
    point2i     _M_gSize;
    uint        _M_uSampleId;
    MinFilter   _M_eMin;
    MagFilter   _M_eMag;
    flags       _M_gStates;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_TEXTURE_H_
