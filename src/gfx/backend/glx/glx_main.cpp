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

#include <cppual/gfx/draw.h>
#include <cppual/process/plugin.h>
#include <cppual/memory/stacked.h>
#include <cppual/gfx/gl/painter.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD) or defined (OS_ANDROID)

#include "glx_surface.h"

namespace cppual { namespace gfx {

// ====================================================

struct glx_factory final : public draw_factory
{
    shared_surface create_surface (connection_type native,
                                   connection_type legacy,
                                   pixel_format format,
                                   point2u size,
                                   handle_type wnd,
                                   surface_type type);

    shared_context create_context (connection_type native,
                                   connection_type legacy,
                                   pixel_format format,
                                   shared_context shared);

    device_backend backend ();
    shared_painter create_painter (shared_surface const& surface);
};

shared_surface glx_factory::create_surface (connection_type native,
                                            connection_type legacy,
                                            pixel_format    format,
                                            point2u         size,
                                            handle_type     wnd,
                                            surface_type    type)
{
    auto const config = gl::config (native, legacy, format);

    config.print ();

    return shared_surface (new gl::surface (config,
                                            size,
                                            wnd,
                                            type));
}

shared_context glx_factory::create_context (connection_type native,
                                            connection_type legacy,
                                            pixel_format    format,
                                            shared_context  /*shared*/)
{
    return shared_context (new gl::context (gl::config(native, legacy, format),
                                            gl::context::default_version ()/*,
                                            shared*/));
}

device_backend glx_factory::backend ()
{
    return device_backend::gl;
}

shared_painter glx_factory::create_painter (shared_surface const& surface)
{
    return shared_painter (new gl::painter (surface));
}

} } // namespace gfx

// =========================================================

using cppual::gfx::glx_factory        ;
using cppual::process::plugin_vars    ;
using cppual::memory::memory_resource ;
using cppual::memory::stacked_resource;
using cppual::memory::allocate_shared ;

extern "C" plugin_vars* plugin_main (memory_resource* /*rc*/)
{
    static char buffer[sizeof (glx_factory) + memory_resource::max_adjust];
    static stacked_resource static_resource (buffer, sizeof (buffer));
    static plugin_vars plugin;

    plugin.name     = "glx_factory"        ;
    plugin.desc     = "XCB_GLX GFX Factory";
    plugin.provides = "gfx::draw_factory"  ;
    plugin.verMajor = 1                    ;
    plugin.verMinor = 0                    ;

    plugin.iface    = allocate_shared<void, glx_factory> (&static_resource);

    return &plugin;
}

#endif // OS_GNU_LINUX or OS_BSD or OS_ANDROID
