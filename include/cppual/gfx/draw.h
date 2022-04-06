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

#ifndef CPPUAL_GFX_DRAW_H_
#define CPPUAL_GFX_DRAW_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/resource.h>
#include <cppual/containers.h>
#include <cppual/noncopyable.h>
#include <cppual/string.h>
#include <cppual/gfx/color.h>
#include <cppual/gfx/coord.h>

#include <memory>

namespace cppual { namespace gfx {

// =========================================================

struct pixel_flag final
{
    enum type
    {
        /// support surface buffer
        drawable = 1 << 0,
        /// support pixmap buffer
        bitmap   = 1 << 1,
        /// support off-screen buffer
        palette  = 1 << 2
    };
};

enum class surface_type : byte
{
    drawable     ,
    double_buffer,
    back_buffer  ,
    pixmap
};

enum class device_backend : byte
{
    native,
    gl,
    vulkan,
    direct3d,
    direct3d12,
    direct2d,
    metal,
    custom,
};

enum class polygon_face : byte
{
    front,
    back ,
    both_sides
};

// =========================================================

struct  context_interface;
struct  surface_interface;
struct  drawable2d_interface;
struct  drawable3d_interface;
struct  painter_interface;
struct  draw_factory;
typedef bitset<pixel_flag::type>              pixel_flags      ;
typedef std::shared_ptr<painter_interface>    shared_painter   ;
typedef std::shared_ptr<draw_factory>         shared_factory   ;
typedef std::shared_ptr<context_interface>    shared_context   ;
typedef std::shared_ptr<surface_interface>    shared_surface   ;
typedef std::shared_ptr<drawable2d_interface> shared_drawable2d;
typedef std::shared_ptr<drawable3d_interface> shared_drawable3d;
typedef std::weak_ptr  <context_interface>    weak_context     ;
typedef std::weak_ptr  <surface_interface>    weak_surface     ;
typedef std::weak_ptr  <drawable2d_interface> weak_drawable2d  ;
typedef std::weak_ptr  <drawable3d_interface> weak_drawable3d  ;

// ====================================================

struct bad_match       : public std::logic_error { using std::logic_error::logic_error; };
struct bad_display     : public std::logic_error { using std::logic_error::logic_error; };
struct bad_surface     : public std::logic_error { using std::logic_error::logic_error; };
struct bad_parameter   : public std::logic_error { using std::logic_error::logic_error; };
struct bad_window      : public std::logic_error { using std::logic_error::logic_error; };
struct bad_context     : public std::logic_error { using std::logic_error::logic_error; };
struct bad_config      : public std::logic_error { using std::logic_error::logic_error; };
struct bad_attrib      : public std::logic_error { using std::logic_error::logic_error; };
struct bad_access      : public std::logic_error { using std::logic_error::logic_error; };
struct context_lost    : public std::logic_error { using std::logic_error::logic_error; };
struct not_initialized : public std::logic_error { using std::logic_error::logic_error; };

// =========================================================

class pixel_format final
{
public:
    pixel_flags flags;
    byte        red, green, blue, alpha;
    byte        depth, stencil;
    color_type  type;

    constexpr byte bits () const noexcept
    { return byte (red + green + blue + alpha); }

    constexpr static pixel_format default2d () noexcept
    {
        return
        {
            pixel_flag::drawable | pixel_flag::bitmap,
            8,
            8,
            8,
            0,
            0,
            0,
            color_type::true_type
        };
    }

    constexpr static pixel_format default3d () noexcept
    {
        return
        {
            pixel_flag::drawable | pixel_flag::bitmap,
            8,
            8,
            8,
            8,
            24,
            0,
            color_type::direct
        };
    }
};

// =========================================================

constexpr bool operator == (pixel_format const& lh, pixel_format const& rh) noexcept
{
    return lh.flags   == rh.flags   &&
           lh.red     == rh.red     &&
           lh.green   == rh.green   &&
           lh.blue    == rh.blue    &&
           lh.alpha   == rh.alpha   &&
           lh.depth   == rh.depth   &&
           lh.stencil == rh.stencil &&
           lh.type    == rh.type;
}

constexpr bool operator != (pixel_format const& lh, pixel_format const& rh) noexcept
{ return !(lh == rh); }

// =========================================================

/// internal memory buffer
class virtual_buffer
{
public:
    typedef vector<color> vector_type;
    typedef pixel_format  format_type;
    typedef std::size_t   size_type  ;
    typedef color         value_type ;

    inline format_type  format () const noexcept { return _M_gFormat; }
    inline vector_type& data   ()       noexcept { return _M_gPixels; }

    inline virtual_buffer () noexcept
    : _M_gFormat (pixel_format::default2d ()),
      _M_gPixels ()
    { }

    inline virtual_buffer (point2i gSize, pixel_format const& gFormat = pixel_format::default2d ())
    : _M_gFormat (gFormat),
      _M_gPixels (size_type (gSize.x * gSize.y))
    { }

private:
    format_type _M_gFormat;
    vector_type _M_gPixels;
};

// =========================================================

class transform2d
{
public:
    transform2d () noexcept = default;
    constexpr rect geometry () const noexcept { return _M_gRect; }

private:
    rect _M_gRect;
};

// =========================================================

class transform3d
{ };

// =========================================================

struct resource_interface
{
    typedef resource_connection connection_type;
    typedef resource_handle     handle_type    ;
    typedef pixel_format        format_type    ;

    virtual ~resource_interface () { }

    virtual connection_type     connection () const = 0;
    virtual format_type         format     () const = 0;
    virtual handle_type         handle     () const = 0;
    virtual device_backend      device     () const = 0;
    virtual void                flush      ()       = 0;
};

// =========================================================

struct surface_interface : public resource_interface
{
    virtual point2u      size  () const = 0;
    virtual surface_type type  () const = 0;
    virtual void         scale (point2u size) = 0;
};

// =========================================================

struct context_interface : public resource_interface
{
    typedef surface_interface*       pointer      ;
    typedef surface_interface const* const_pointer;
    typedef resource_version         version_type ;

    virtual pointer       drawable () const = 0;
    virtual const_pointer readable () const = 0;
    virtual version_type  version  () const = 0;
    virtual bool          assign   () = 0;
    virtual bool          use      (pointer, const_pointer) = 0;
    virtual void          finish   () = 0;
    virtual void          release  () = 0;

    static context_interface* current () noexcept;
    static void               acquire (context_interface*) noexcept;

    bool active () const noexcept
    { return this == current (); }
};

// =========================================================

struct drawable2d_interface
{
    virtual device_backend type () const noexcept    = 0;
    virtual void           draw (transform2d const&) = 0;

    virtual ~drawable2d_interface () { }
};

// =========================================================

struct drawable3d_interface
{
    virtual device_backend type () const noexcept    = 0;
    virtual void           draw (transform3d const&) = 0;

    virtual ~drawable3d_interface () { }
};

// =========================================================

struct transformable2d_interface
{
    virtual ~transformable2d_interface () { }
};

// =========================================================

struct transformable3d_interface
{
    virtual ~transformable3d_interface () { }
};

// ====================================================

struct draw_factory : public non_copyable_virtual
{
    typedef string                              string_type    ;
    typedef resource_interface::connection_type connection_type;
    typedef resource_interface::handle_type     handle_type    ;

    virtual shared_surface create_surface (connection_type conn,
                                           pixel_format format,
                                           point2u size,
                                           handle_type wnd,
                                           surface_type type = surface_type::double_buffer) = 0;

    virtual shared_context create_context (connection_type conn,
                                           pixel_format format,
                                           shared_context shared = nullptr) = 0;

    virtual device_backend backend () = 0;
    virtual shared_painter create_painter () = 0;

    //virtual shared_drawable2d create_image () = 0;
    //virtual shared_drawable2d create_line () = 0;
    //virtual shared_drawable2d create_elipse () = 0;
    //virtual shared_drawable2d create_polygon () = 0;
    //virtual shared_drawable2d create_rectangle () = 0;
    //virtual shared_drawable2d create_text () = 0;

    static shared_factory instance ();
    static bool           has_valid_instance () noexcept;
};

// ====================================================

} } // namespace gfx

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAW_H_
