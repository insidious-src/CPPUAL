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

#ifndef CPPUAL_GFX_DRAWABLE_IMAGE_H_
#define CPPUAL_GFX_DRAWABLE_IMAGE_H_
#ifdef __cplusplus

#include <string>
#include <cppual/gfx/draw.h>
#include <cppual/memory/allocator.h>

using std::string;
using cppual::Memory::Allocator;
using cppual::Memory::Repository;


namespace cppual { namespace Graphics {

class Image : public IDrawable2D, public ITransformable2D
{
public:
    virtual VirtualBuffer* map () noexcept = 0;
    virtual bool           isLoaded () const noexcept = 0;
};

// =========================================================

/**
 * @class CRasterImage
 * @brief Raster image parser for jpg, png, bmp, tga, gif
 */

class RasterImage final : public Image, private Memory::Allocator<RasterImage, Repository>
{
public:
    RasterImage  () = delete;
    ~RasterImage ();
    void draw (Transform2D const& info);

    inline RasterImage (string const&      gPath,
                        Repository&        pAtor,
                        PixelFormat const& gFomat = PixelFormat (),
                        Color              gMask  = Color       ())
    : Allocator (pAtor),
      m_gPixBuffer ({ 0, 0 }, gFomat),
      m_gColorMask (gMask),
      m_bIsLoaded  (parseImage (gPath))
    { }

    inline Color mask     () const noexcept { return m_gColorMask; }
    inline bool  isLoaded () const noexcept { return m_bIsLoaded ; }

    inline DeviceType type () const noexcept
    { return DeviceType::GL; }

    inline void setMask (Color gMask) noexcept
    { m_gColorMask = gMask; }

    inline VirtualBuffer* map () noexcept
    { return &m_gPixBuffer; }

private:
    VirtualBuffer m_gPixBuffer;
    Color         m_gColorMask;
    cbool         m_bIsLoaded ;

    bool parseImage (string const&);
};

// =========================================================

/**
 * @class CVectorImage
 * @brief Vector image parser for svg
 */

class VectorImage final : public Image, private Memory::Allocator<VectorImage, Repository>
{
public:
    VectorImage  () = delete;
    ~VectorImage ();
    void draw (Transform2D const& info);

    inline DeviceType type () const noexcept
    { return DeviceType::GL; }

    inline VirtualBuffer* map () noexcept
    { return &m_gPixBuffer; }

    inline bool isLoaded () const noexcept
    { return m_bIsLoaded; }

    inline VectorImage (string const&      gPath,
                        Repository&        pAtor,
                        PixelFormat const& gFomat = PixelFormat ())
    : Allocator (pAtor),
      m_gPixBuffer ({ 0, 0 },   gFomat),
      m_bIsLoaded  (parseImage (gPath))
    { }

private:
    VirtualBuffer m_gPixBuffer;
    cbool         m_bIsLoaded ;

    bool parseImage (string const&);
};

// =========================================================

class Icon final
{
public:
    Icon () { }
    Icon (string const&) { }
    bool load (string const&) { return false; }
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_IMAGE_H_
