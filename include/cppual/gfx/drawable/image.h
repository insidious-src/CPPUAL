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

#ifndef CPPUAL_GFX_DRAWABLE_IMAGE_H_
#define CPPUAL_GFX_DRAWABLE_IMAGE_H_
#ifdef __cplusplus

#include <cppual/gfx/drawable/shape.h>
#include <cppual/memory/allocator.h>

//#include <string>

namespace cppual { namespace gfx {

// =========================================================

using memory::memory_resource;

// =========================================================

class image_interface : public drawable_interface, public transformable_interface
{
public:
    typedef string string_type;

    virtual virtual_buffer* map () noexcept = 0;
    virtual bool            is_loaded () const noexcept = 0;
};

// =========================================================

/**
 * @class raster_image
 * @brief Raster image parser for jpg, png, bmp, tga, gif
 */

class raster_image final : public image_interface, private memory::allocator<raster_image>
{
public:
    inline raster_image () noexcept = default;
    ~raster_image () { }

    void draw (transform const& info);

    inline raster_image (string_type  const& gPath,
                         pixel_format const& gFomat = pixel_format (),
                         color               gMask  = color        (),
                         memory_resource&    pAtor  = memory::get_default_resource ())
    : allocator (pAtor),
      _M_gPixBuffer ({ 0, 0 }, gFomat),
      _M_gColorMask (gMask),
      _M_bIsLoaded  (parse_image (gPath))
    { }

    inline color mask      () const noexcept { return _M_gColorMask; }
    inline bool  is_loaded () const noexcept { return _M_bIsLoaded ; }

    inline device_backend type () const noexcept
    { return device_backend::gl; }

    inline void set_mask (color gMask) noexcept
    { _M_gColorMask = gMask; }

    inline virtual_buffer* map () noexcept
    { return &_M_gPixBuffer; }

private:
    virtual_buffer _M_gPixBuffer   ;
    color          _M_gColorMask   ;
    cbool          _M_bIsLoaded { };

    bool parse_image (string_type const&);
};

// =========================================================

/**
 * @class vector_image
 * @brief Vector image parser for svg
 */

class vector_image final : public image_interface, private memory::allocator<vector_image>
{
public:
    vector_image  () = delete;
    ~vector_image ();

    void draw (transform const& info);

    inline device_backend type () const noexcept
    { return device_backend::gl; }

    inline virtual_buffer* map () noexcept
    { return &_M_gPixBuffer; }

    inline bool is_loaded () const noexcept
    { return _M_bIsLoaded; }

    inline vector_image (string_type const&  gPath,
                         memory_resource&    pAtor  = memory::get_default_resource (),
                         pixel_format const& gFomat = pixel_format ())
    : allocator (pAtor),
      _M_gPixBuffer ({ 0, 0 },   gFomat),
      _M_bIsLoaded  (parse_image (gPath))
    { }

private:
    virtual_buffer _M_gPixBuffer;
    cbool          _M_bIsLoaded ;

    bool parse_image (string_type const&);
};

// =========================================================

class icon
{
public:
    typedef string string_type;

    icon () { }
    icon (string_type const&) { }
    bool load (string_type const&) { return false; }
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_IMAGE_H_
