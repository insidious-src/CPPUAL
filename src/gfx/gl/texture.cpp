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

#include <cppual/gfx/gl/texture.h>
#include <cppual/gfx/gl/ext.h>
#include <cppual/type_meta.h>

#define GL_GLEXT_PROTOTYPES
#include <cppual/gfx/gl/gldef.h>

#include <fstream>

namespace cppual { namespace gfx { namespace gl {

namespace { //! optimize for internal unit usage

struct GLStates final
{
    inline constinit static bool hasDirectAccess = false;
};

constexpr int convert_mag_filter (texture::MagFilter eMag) noexcept
{
    return eMag == texture::MagFilter::Bilinear ? gl::Linear : gl::Nearest;
}

constexpr int convert_min_filter (texture::MinFilter eMin) noexcept
{
    switch (eMin)
    {
    case texture::MinFilter::Bilinear:
        return gl::Linear;
    case texture::MinFilter::NearestMipMap:
        return gl::NearestMipMapNearest;
    case texture::MinFilter::BilinearMipMap:
        return gl::LinearMipMapNearest;
    case texture::MinFilter::TrilinearMipMap:
        return gl::LinearMipMapLinear;
    default:
        return gl::Nearest;
    }
}

} // anonymous namespace

// ====================================================

texture::texture () noexcept
: object (resource_type::texture),
  _M_gFormat (),
  _M_gSize (),
  _M_uSampleId (),
  _M_eMin (MinFilter::NearestMipMap),
  _M_eMag (MagFilter::Nearest),
  _M_gStates ()
{ }

texture::texture (string const&       gFile,
                  pixel_format const& gFormat,
                  bool                bGenMipMaps)
: object (resource_type::texture),
  _M_gFormat (),
  _M_gSize (),
  _M_uSampleId (),
  _M_eMin (MinFilter::NearestMipMap),
  _M_eMag (MagFilter::Nearest),
  _M_gStates ()
{
    if (load_texture_2d (gFile, gFormat, bGenMipMaps))
    {
        if (bGenMipMaps) _M_gStates += texture::has_mipmaps;
        _M_gStates += texture::loaded;
    }
}

texture::~texture () noexcept
{
    if (handle () and _M_uSampleId) ::glDeleteSamplers (1, &_M_uSampleId);
}

void texture::set_state (State eState, bool bSwitch) noexcept
{
    switch (eState)
    {
    case State::DirectAccess:
        GLStates::hasDirectAccess = bSwitch;
        break;
    }
}

bool texture::load_texture_2d (string const&       gFilePath,
                               pixel_format const& gFormat,
                               bool                bGenMipMaps)
{
    if (!handle () or _M_gStates.test (texture::loaded)) return false;
    std::ifstream gFile (gFilePath.c_str(), std::ios_base::binary);
    uint          uFormat;

    if (gFile.is_open ())
    {
        if (_M_gSize.x and !is_power_of_two (_M_gSize.x)) --_M_gSize.x;
        if (_M_gSize.y and !is_power_of_two (_M_gSize.y)) --_M_gSize.y;
    }

    if (_M_gSize.x == 0 or _M_gSize.y == 0) return false;

    uFormat    = gFormat.depth == 24 ? GL_RGB : (gFormat.depth == 8 ? GL_LUMINANCE : 0);

    ::glTexImage2D (gl::Texture2D,
                    0, // level
                    gFormat.depth == 24 ? GL_RGB : GL_DEPTH_COMPONENT,
                    _M_gSize.x, _M_gSize.y,
                    0, // border
                    uFormat,
                    GL_UNSIGNED_BYTE,
                    gFile.rdbuf ());

    gFile.close ();

    if (bGenMipMaps) ::glGenerateMipmap (gl::Texture2D);
    ::glGenSamplers (1, &_M_uSampleId);

    _M_gFormat  = gFormat;
    _M_gStates += texture::loaded;
    if (bGenMipMaps) _M_gStates += texture::has_mipmaps;
    return true;
}

bool texture::load_texture_2d (cvoid*             pPixels,
                               point2i     const& gSize,
                               pixel_format const& gFormat,
                               bool               bGenMipMaps)
{
    if (!handle () or _M_gStates.test (texture::loaded) or
            !pPixels or !gSize.x or !gSize.y)
        return false;

    uint uFormat = gFormat.depth == 24 ?
                       gl::RGB : (gFormat.depth == 8 ?
                                      static_cast<uint> (gl::Luminance) : 0);

    ::glTexImage2D (gl::Texture2D,
                  0, // level
                  gFormat.depth == 24 ? static_cast<int> (gl::RGB) : GL_DEPTH_COMPONENT,
                  _M_gSize.x, _M_gSize.y,
                  0, // border
                  uFormat,
                  GL_UNSIGNED_BYTE,
                  pPixels);

    if (bGenMipMaps) ::glGenerateMipmap (gl::Texture2D);
    ::glGenSamplers (1, &_M_uSampleId);

    _M_gFormat  = gFormat;
    _M_gStates += texture::loaded;
    if (bGenMipMaps) _M_gStates += texture::has_mipmaps;
    return true;
}

void texture::set_parameter (uint uName, int nParam) noexcept
{
    if (!handle<::GLuint> ()) return;

    if (!GLStates::hasDirectAccess)
    {
        uint uBoundTex = 0;

        ::glGetIntegerv (GL_TEXTURE_BINDING_2D, reinterpret_cast<int*> (&uBoundTex));
        ::glBindTexture (gl::Texture2D, handle<::GLuint> ());
        ::glTexParameteri (gl::Texture2D, uName, nParam);
        ::glBindTexture (gl::Texture2D, uBoundTex);
    }
    else ::glTextureParameteriEXT (handle<::GLuint> (), gl::Texture2D, uName, nParam);
}

void texture::bind (uint uTexId) noexcept
{
    if (_M_gStates.test (texture::loaded))
    {
        ::glActiveTexture (GL_TEXTURE0 + uTexId);
        ::glBindTexture   (gl::Texture2D, handle<::GLuint> ());
        ::glBindSampler   (uTexId, _M_uSampleId);
    }
}

void texture::set_filtering (MagFilter eMag, MinFilter eMin) noexcept
{
    if (_M_uSampleId)
    {
        ::glSamplerParameteri (_M_uSampleId, gl::Tex2DMinFilter, convert_min_filter (eMin));
        ::glSamplerParameteri (_M_uSampleId, gl::Tex2DMagFilter, convert_mag_filter (eMag));

        _M_eMin = eMin;
        _M_eMag = eMag;
    }
}

void texture::release () noexcept
{
    if (_M_gStates.test (texture::loaded))
    {
        if (_M_uSampleId) ::glDeleteSamplers (1, &_M_uSampleId);
        _M_uSampleId = 0;
        _M_gStates  -= texture::loaded;
    }
}

} } } // namespace GL
