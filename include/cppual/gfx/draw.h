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

#ifndef CPPUAL_GFX_DRAW_H_
#define CPPUAL_GFX_DRAW_H_
#ifdef __cplusplus

#include <cppual/bitset.h>
#include <cppual/string.h>
#include <cppual/resource.h>
#include <cppual/gfx/coord.h>
#include <cppual/gfx/color.h>
#include <cppual/containers.h>
#include <cppual/noncopyable.h>
#include <cppual/memory/allocator.h>

#include <memory>
#include <mutex>

// =========================================================

namespace cppual::gfx {

// =========================================================

class font;

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

enum class surface_type : u8
{
    drawable     ,
    double_buffer,
    back_buffer  ,
    pixmap
};

enum class device_backend : u8
{
    native,
    vulkan,
    gl,
    direct3d,
    direct3d12,
    direct2d,
    metal,
    custom,
};

enum class polygon_face : u8
{
    front,
    back ,
    both_sides
};

// =========================================================

struct  context_interface   ;
struct  surface_interface   ;
struct  drawable_interface;
struct  drawable3d_interface;
struct  painter_interface   ;
struct  draw_factory        ;

typedef bitset<pixel_flag::type>              pixel_flags      ;
typedef std::shared_ptr<painter_interface>    shared_painter   ;
typedef std::shared_ptr<draw_factory>         shared_factory   ;
typedef std::shared_ptr<context_interface>    shared_context   ;
typedef std::shared_ptr<surface_interface>    shared_surface   ;
typedef std::shared_ptr<drawable_interface> shared_drawable2d;
typedef std::shared_ptr<drawable3d_interface> shared_drawable3d;
typedef std::weak_ptr  <context_interface>    weak_context     ;
typedef std::weak_ptr  <surface_interface>    weak_surface     ;
typedef std::weak_ptr  <drawable_interface> weak_drawable2d  ;
typedef std::weak_ptr  <drawable3d_interface> weak_drawable3d  ;

// ====================================================

struct bad_alloc       : public std::logic_error { using std::logic_error::logic_error; };
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

class pixel_format
{
public:
    typedef pixel_format self_type;

    pixel_flags flags { pixel_flag::drawable /*| pixel_flag::bitmap*/ };
    u8          red   { 8 }, green { 8 }, blue { 8 }, alpha { };
    u8          depth {   }, stencil { };
    color_type  type  { color_type::true_type };

    constexpr u8 bits () const noexcept
    { return u8 (red + green + blue + alpha); }

    //! default values for 2D
    constexpr static self_type default2d () noexcept
    {
        return self_type ();
    }

    //! default values for 3D
    constexpr static self_type default3d () noexcept
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
    typedef pixel_format                  format_type   ;
    typedef std::size_t                   size_type     ;
    typedef size_type const               const_size    ;
    typedef color                         value_type    ;
    typedef dyn_array<value_type>         vector_type   ;
    typedef memory::allocator<value_type> allocator_type;

    constexpr format_type  format () const noexcept { return _M_gFormat; }
    constexpr vector_type& data   ()       noexcept { return _M_gPixels; }

    constexpr virtual_buffer (pixel_format const& gFormat = pixel_format::default2d ()) noexcept
    : _M_gFormat (gFormat),
      _M_gPixels ()
    { }

    constexpr virtual_buffer (point2i gSize,
                              pixel_format   const& gFormat = pixel_format::default2d (),
                              allocator_type const& ator    = allocator_type ())
    : _M_gFormat (gFormat),
      _M_gPixels (size_type (gSize.x * gSize.y), ator)
    { }

private:
    format_type _M_gFormat;
    vector_type _M_gPixels;
};

// =========================================================

struct SHARED_API resource_interface : public resource<resource_handle, resource_handle>
{
    typedef resource_interface self_type  ;
    typedef pixel_format       format_type;

    virtual connection_type connection () const = 0;
    virtual format_type     format     () const = 0;
    virtual handle_type     handle     () const = 0;
    virtual device_backend  device     () const = 0;
    virtual void            flush      ()       = 0;
};

// =========================================================

struct SHARED_API surface_interface : public resource_interface
{
    typedef surface_interface self_type;

    virtual void           set_parent (shared_surface parent) = 0;
    virtual shared_surface parent   () const = 0;
    virtual point2i        position () const = 0;
    virtual point2u        size     () const = 0;
    virtual surface_type   type     () const = 0;
    virtual void           scale (point2u size) = 0;
    virtual void           paint_background (color clr) = 0;
    virtual shared_context context  () const = 0;
};

// =========================================================

struct SHARED_API context_interface : public resource_interface
{
    typedef context_interface self_type   ;
    typedef resource_version  version_type;

    virtual shared_surface drawable () const = 0;
    virtual shared_surface readable () const = 0;
    virtual version_type   version  () const = 0;
    virtual bool           assign   (shared_context cntxt) = 0;
    virtual bool           use      (shared_surface, shared_surface) = 0;
    virtual void           finish   () = 0;
    virtual void           release  () = 0;

    static context_interface* current () noexcept;
    static void               acquire (shared_context cntxt) noexcept;

    constexpr bool active () const noexcept
    { return this == current (); }
};

// =========================================================

struct SHARED_API proxy_surface : public surface_interface
{
public:
    typedef proxy_surface self_type;

    void           set_parent (shared_surface parent) noexcept;
    shared_surface parent   () const noexcept;
    point2i        position () const noexcept;
    point2u        size     () const noexcept;
    surface_type   type     () const noexcept;
    void           scale (point2u size);
    void           paint_background (color clr);
    shared_context context  () const noexcept;

private:
    shared_surface _M_parent;
    rect           _M_rect  ;
};

// =========================================================

class SHARED_API transform
{
public:
    typedef transform self_type;

    constexpr transform (rect const& gRect,
                         float z,
                         shared_surface const& surface,
                         float rotate = .0f) noexcept
    : _M_rect    (gRect  ),
      _M_surface (surface),
      _M_z_depth (z      ),
      _M_rotate  (rotate )
    { }

    constexpr transform ()                             noexcept = default;
    inline    transform (self_type &&)                 noexcept = default;
    inline    transform (self_type const&)             noexcept = default;
    inline    self_type& operator = (self_type &&)     noexcept = default;
    inline    self_type& operator = (self_type const&) noexcept = default;

    constexpr rect           geometry () const noexcept { return _M_rect              ; }
    constexpr float          z_depth  () const noexcept { return _M_z_depth           ; }
    constexpr shared_surface surface  () const noexcept { return _M_surface           ; }
    constexpr shared_context context  () const noexcept { return _M_surface->context(); }
    constexpr float          rotation () const noexcept { return _M_rotate            ; }

private:
    rect           _M_rect    { };
    shared_surface _M_surface { };
    float          _M_z_depth { };
    float          _M_rotate  { };
};

// =========================================================

struct SHARED_API drawable_interface : public non_copyable_virtual
{
    typedef drawable_interface self_type;

    typedef enum class line_style : u8
    {
        solid = 1,
        dash     ,
        dot      ,
        dash_dot_dot
    }
    const const_line_style;

    constexpr drawable_interface () noexcept = default;

    virtual device_backend type () const noexcept  = 0;
    virtual void           draw (transform const&) = 0;
};

// =========================================================

struct SHARED_API transformable_interface : public non_copyable_virtual
{
    typedef transformable_interface self_type;

    constexpr transformable_interface () noexcept = default;
};

// =========================================================

struct SHARED_API painter_interface : public non_copyable_virtual
{
    typedef painter_interface                self_type    ;
    typedef string                           string_type  ;
    typedef drawable_interface::line_style line_style   ;
    typedef resource_version                 version_type ;
    typedef std::array<point2i, 3>           polygon_array;

    virtual shared_drawable2d create_shape (u8 shape_type) = 0;

    virtual shared_drawable2d create_line (color const& clr,
                                           uint         line_width = 1U,
                                           line_style   style      = line_style::solid) = 0;

    virtual shared_drawable2d create_path (dyn_array<point2i> const& coord,
                                           color              const& clr,
                                           uint                      line_width = 1U,
                                           line_style                style      = line_style::solid) = 0;

    virtual shared_drawable2d create_ellipse (color fill, color outline, uint outline_width = 1U) = 0;
    virtual shared_drawable2d create_rectangle (color fill, color outline, uint outline_width = 1U) = 0;

    virtual shared_drawable2d create_polygon (polygon_array const& coord,
                                              color fill, color outline, uint outline_width = 1U) = 0;

    virtual shared_drawable2d create_image (string_type  const& path,
                                            pixel_format const& format = pixel_format (),
                                            color               mask   = color        ()) = 0;

    virtual shared_drawable2d create_text (string_type const& text, font& txt_font) = 0;
};

// ====================================================

class SHARED_API painter : public non_copyable
{
public:
    typedef painter                                       self_type         ;
    typedef string                                        string_type       ;
    typedef dyn_array<std::pair<shared_drawable2d, rect>> drawable_container;
    typedef drawable_interface::line_style              line_style        ;
    typedef std::array<point2i, 3>                        polygon_array     ;

    painter  (shared_surface const& surface);
    ~painter ();

    void create_line (rect  const& rct,
                      color const& clr,
                      uint         line_size = 1U,
                      line_style   style     = line_style::solid);

    void create_path (dyn_array<point2i> const& coord,
                      color              const& clr,
                      uint                      line_size = 1U,
                      line_style                style     = line_style::solid);

    void create_ellipse (rect const& rct, color fill, color outline, uint outline_size = 1U);
    void create_rectangle (rect const& rct, color fill, color outline, uint outline_size = 1U);
    void create_polygon (polygon_array const& coord, color fill, color outline, uint outline_size = 1U);

    void create_image (string_type  const& path,
                       rect         const& rct,
                       pixel_format const& format = pixel_format (),
                       color               mask   = color        ());

    void create_text (rect const& rct, string_type const& text, font& txt_font);

private:
    shared_painter     _M_pPainter  ;
    drawable_container _M_gDrawables;
    shared_surface     _M_pSurface  ;
};

// ====================================================

class SHARED_API context_mutex
{
public:
    typedef std::recursive_mutex           mutex_type        ;
    typedef mutex_type::native_handle_type native_handle_type;

    constexpr context_mutex (shared_context const& context) noexcept
    : _M_context (context)
    { }

    constexpr ~context_mutex () noexcept
    {
        if (context_interface::current () == _M_context.get ()) unlock ();
    }

    constexpr native_handle_type native_handle () const noexcept
    {
        return _M_mutex.native_handle ();
    }

    constexpr void lock ()
    {
        _M_mutex.lock ();
        context_interface::acquire (_M_context);
    }

    constexpr bool try_lock ()
    {
        if (!_M_mutex.try_lock ()) return false;

        context_interface::acquire (_M_context);
        return true;
    }

    constexpr void unlock ()
    {
        context_interface::acquire (nullptr);
        _M_mutex.unlock ();
    }

private:
    mutex_type mutable _M_mutex  ;
    shared_context     _M_context;
};

// ====================================================

struct SHARED_API draw_factory : public non_copyable_virtual
{
    typedef draw_factory                        self_type      ;
    typedef string                              string_type    ;
    typedef resource_interface::connection_type connection_type;
    typedef resource_interface::handle_type     handle_type    ;

    virtual ~draw_factory ();

    virtual shared_surface create_surface (connection_type native,
                                           connection_type legacy,
                                           pixel_format    format,
                                           point2u         size  ,
                                           handle_type     wnd   ,
                                           surface_type    type = surface_type::double_buffer) = 0;

    virtual shared_context create_context (connection_type native,
                                           connection_type legacy,
                                           pixel_format    format,
                                           shared_context  shared = nullptr) = 0;

    virtual device_backend backend () = 0;
    virtual shared_painter create_painter (shared_surface const& surface) = 0;

    static shared_factory instance ();
    static bool           has_valid_instance () noexcept;
};

// ====================================================

} // namespace gfx

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAW_H_
