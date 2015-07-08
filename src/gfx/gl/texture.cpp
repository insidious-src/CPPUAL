/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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
#include <cppual/func.h>
#include <fstream>

#define GL_GLEXT_PROTOTYPES
#include "gldef.h"

using std::string;
using std::ios_base;
using std::ifstream;

namespace cppual { namespace Graphics { namespace GL {

namespace { // optimize for internal unit usage

struct GLStates final
{
    static bool hasDirectAccess;
};

bool GLStates::hasDirectAccess = false;

constexpr int convertMagFilter (Texture::MagFilter eMag) noexcept
{
    return eMag == Texture::MagFilter::Bilinear ? GL::Linear : GL::Nearest;
}

inline int convertMinFilter (Texture::MinFilter eMin) noexcept
{
    switch (eMin)
    {
    case Texture::MinFilter::Bilinear:
        return GL::Linear;
    case Texture::MinFilter::NearestMipMap:
        return GL::NearestMipMapNearest;
    case Texture::MinFilter::BilinearMipMap:
        return GL::LinearMipMapNearest;
    case Texture::MinFilter::TrilinearMipMap:
        return GL::LinearMipMapLinear;
    default:
        return GL::Nearest;
    }
}

} // anonymous namespace

// ====================================================

Texture::Texture () noexcept
: Object (ResourceType::Texture),
  m_gFormat (),
  m_gSize (),
  m_uSampleId (),
  m_eMin (MinFilter::NearestMipMap),
  m_eMag (MagFilter::Nearest),
  m_gStates ()
{ }

Texture::Texture (string const&      gFile,
                  PixelFormat const& gFormat,
                  bool               bGenMipMaps)
: Object (ResourceType::Texture),
  m_gFormat (),
  m_gSize (),
  m_uSampleId (),
  m_eMin (MinFilter::NearestMipMap),
  m_eMag (MagFilter::Nearest),
  m_gStates ()
{
    if (loadTexture2D (gFile, gFormat, bGenMipMaps))
    {
        if (bGenMipMaps) m_gStates += Texture::HasMipMaps;
        m_gStates += Texture::IsLoaded;
    }
}

Texture::~Texture () noexcept
{
    if (id () and m_uSampleId) glDeleteSamplers (1, &m_uSampleId);
}

void Texture::setState (State eState, bool bSwitch) noexcept
{
    switch (eState)
    {
    case State::DirectAccess:
        GLStates::hasDirectAccess = bSwitch;
        break;
    }
}

bool Texture::loadTexture2D (string const&      gFilePath,
                             PixelFormat const& gFormat,
                             bool               bGenMipMaps)
{
    if (!id () or m_gStates.test (Texture::IsLoaded)) return false;
    ifstream gFile (gFilePath, ios_base::binary);
    uint     uFormat;

    if (gFile.is_open ())
    {
        if (m_gSize.x and !isPowerOfTwo (m_gSize.x)) --m_gSize.x;
        if (m_gSize.y and !isPowerOfTwo (m_gSize.y)) --m_gSize.y;
    }

    if (m_gSize.x == 0 or m_gSize.y == 0) return false;

    uFormat    = gFormat.depth == 24 ?
                  GL_RGB : (gFormat.depth == 8 ? GL_LUMINANCE : 0);

    glTexImage2D (GL::Texture2D,
                  0, // level
                  gFormat.depth == 24 ? GL_RGB : GL_DEPTH_COMPONENT,
                  m_gSize.x, m_gSize.y,
                  0, // border
                  uFormat,
                  GL_UNSIGNED_BYTE,
                  gFile.rdbuf ());

    gFile.close ();

    if (bGenMipMaps) glGenerateMipmap (GL::Texture2D);
    glGenSamplers (1, &m_uSampleId);

    m_gFormat  = gFormat;
    m_gStates += Texture::IsLoaded;
    if (bGenMipMaps) m_gStates += Texture::HasMipMaps;
    return true;
}

bool Texture::loadTexture2D (cvoid*             pPixels,
                             point2i     const& gSize,
                             PixelFormat const& gFormat,
                             bool               bGenMipMaps)
{
    if (!id () or m_gStates.test (Texture::IsLoaded) or
            !pPixels or !gSize.x or !gSize.y)
        return false;

    uint uFormat = gFormat.depth == 24 ?
                       GL::RGB : (gFormat.depth == 8 ?
                                      static_cast<uint> (GL::Luminance) : 0);

    glTexImage2D (GL::Texture2D,
                  0, // level
                  gFormat.depth == 24 ? static_cast<int> (GL::RGB) : GL_DEPTH_COMPONENT,
                  m_gSize.x, m_gSize.y,
                  0, // border
                  uFormat,
                  GL_UNSIGNED_BYTE,
                  pPixels);

    if (bGenMipMaps) glGenerateMipmap (GL::Texture2D);
    glGenSamplers (1, &m_uSampleId);

    m_gFormat  = gFormat;
    m_gStates += Texture::IsLoaded;
    if (bGenMipMaps) m_gStates += Texture::HasMipMaps;
    return true;
}

void Texture::setParameter (uint uName, int nParam) noexcept
{
    if (!id ()) return;

    if (!GLStates::hasDirectAccess)
    {
        uint uBoundTex = 0;

        glGetIntegerv (GL_TEXTURE_BINDING_2D, reinterpret_cast<int*> (&uBoundTex));
        glBindTexture (GL::Texture2D, id ());
        glTexParameteri (GL::Texture2D, uName, nParam);
        glBindTexture (GL::Texture2D, uBoundTex);
    }
    else glTextureParameteriEXT (id (), GL::Texture2D, uName, nParam);
}

void Texture::bind (uint uTexId) noexcept
{
    if (m_gStates.test (Texture::IsLoaded))
    {
        glActiveTexture (GL_TEXTURE0 + uTexId);
        glBindTexture (GL::Texture2D, id ());
        glBindSampler (uTexId, m_uSampleId);
    }
}

void Texture::setFiltering (MagFilter eMag, MinFilter eMin) noexcept
{
    if (m_uSampleId)
    {
        glSamplerParameteri (m_uSampleId, GL::Tex2DMinFilter, convertMinFilter (eMin));
        glSamplerParameteri (m_uSampleId, GL::Tex2DMagFilter, convertMagFilter (eMag));

        m_eMin = eMin;
        m_eMag = eMag;
    }
}

void Texture::release () noexcept
{
    if (m_gStates.test (Texture::IsLoaded))
    {
        if (m_uSampleId) glDeleteSamplers (1, &m_uSampleId);
        m_uSampleId = 0;
        m_gStates  -= Texture::IsLoaded;
    }
}

} } } // namespace GL
