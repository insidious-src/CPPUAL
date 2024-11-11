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

#include <cppual/gfx/draw.h>
#include <cppual/process/plugin.h>

#include <algorithm>
#include <iostream>

namespace cppual { namespace gfx {

// ====================================================

namespace { namespace internal {

typedef process::plugin_manager<draw_factory> manager_type   ;
typedef context_interface                     context_type   ;
typedef context_interface*                    context_pointer;

// ====================================================

class initializer final
{
private:
    manager_type   mgr    ;
    shared_factory factory;

    inline static cchar* plugin_name () noexcept
    {
    #   if defined OS_GNU_LINUX || defined OS_BSD || defined OS_ANDROID
        //return "libcppual-gfx-plugin-glx";
        return "libcppual-gfx-plugin-egl";
    #   elif defined OS_WINDOWS
        return "libcppual-gfx-plugin-wgl";
    #   endif
    }

public:
    inline initializer ()
    {
        if (mgr.load_plugin (plugin_name ())) factory = mgr.plugin (plugin_name ()).iface ();
    }

    inline operator shared_factory () const
    {
        return factory;
    }
};

// ====================================================

inline static shared_factory plugin_instance ()
{
    static auto inst = initializer ();
    return inst;
}

inline context_interface*& current () noexcept
{
    static thread_local context_interface* current_dc = nullptr;
    return current_dc;
}


} } // anonymous namespace internal

// ====================================================

painter::painter (shared_surface const& surface)
: _M_pPainter (draw_factory::instance ()->create_painter (surface)),
  _M_pSurface (surface)
{
    _M_gDrawables.reserve (10);
}

painter::~painter ()
{
    std::cout << "drawable count: " << _M_gDrawables.size () << std::endl;

    for (auto i = 0U; i < _M_gDrawables.size (); ++i)
        _M_gDrawables[i].first->draw (transform2d (_M_gDrawables[i].second, _M_pSurface));
}

void painter::create_line (rect  const& rct,
                           color const& color_fill,
                           uint         line_size,
                           line_style   style)
{
    _M_gDrawables.push_back ({ _M_pPainter->create_line (color_fill, line_size, style), rct });
}

void painter::create_path (vector<point2i> const& coord,
                           color           const& clr,
                           uint                   line_size,
                           line_style             style)
{
    _M_gDrawables.push_back ({ _M_pPainter->create_path (coord, clr, line_size, style), rect () });
}

void painter::create_ellipse (rect const& rct, color fill, color outline, uint size)
{
    _M_gDrawables.push_back ({ _M_pPainter->create_ellipse (fill, outline, size), rct });
}

void painter::create_rectangle (rect const& rct, color fill, color outline, uint size)
{
    _M_gDrawables.push_back ({ _M_pPainter->create_rectangle (fill, outline, size), rct });
}

void painter::create_polygon (polygon_array const& coord, color fill, color outline, uint size)
{
    i16 X[3] = { coord[0].x, coord[1].x, coord[2].x };
    i16 Y[3] = { coord[0].y, coord[1].y, coord[2].y };

    i16 Xmax = *std::max_element(X, X + 3);
    i16 Xmin = *std::min_element(X, X + 3);

    i16 Ymax = *std::max_element(Y, Y + 3);
    i16 Ymin = *std::min_element(Y, Y + 3);

    _M_gDrawables.push_back ({ _M_pPainter->create_polygon (coord, fill, outline, size),
                               rect (Xmin, Ymin,
                                     static_cast<u16> (Xmax - Xmin), static_cast<u16> (Ymax - Ymin)) });
}

void painter::create_image (string_type  const& path,
                            rect         const& rct,
                            pixel_format const& format,
                            color               mask)
{
    _M_gDrawables.push_back ({ _M_pPainter->create_image (path, format, mask), rct });
}

void painter::create_text (rect const& rct, string_type const& text, font& txt_font)
{
    _M_gDrawables.push_back ({ _M_pPainter->create_text (text, txt_font), rct });
}

// ====================================================

draw_factory::~draw_factory ()
{
}

shared_factory draw_factory::instance ()
{
    return internal::plugin_instance ();
}

bool draw_factory::has_valid_instance () noexcept
{
    return internal::plugin_instance () != nullptr;
}

// ====================================================

context_interface* context_interface::current () noexcept
{
    return internal::current ();
}

void context_interface::acquire (shared_context const& pContext) noexcept
{
    auto current_cntxt = internal::current ();

    if (current_cntxt != pContext.get () && current_cntxt != nullptr) current_cntxt->release ();

    internal::current () = pContext.get ();
    internal::current ()->assign (pContext);
}

// ====================================================

} } // namespace gfx
