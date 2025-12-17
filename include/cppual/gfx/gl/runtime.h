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

#ifndef CPPUAL_GFX_GL_RUNTIME_H_
#define CPPUAL_GFX_GL_RUNTIME_H_
#ifdef __cplusplus

#include <cppual/resource>
#include <cppual/string>

// ====================================================

namespace cppual::gfx::gl {

// ====================================================

typedef enum class string_query : u8
{
    renderer,
    vendor  ,
    version ,
    sl_version
}
const const_string_query;

// ====================================================

struct platform
{
    typedef resource_version version_type;
    typedef string           string_type ;

    static version_type version ();
    static version_type sl_version ();
    static string_type  label (string_query);
    static bool         is_extension_supported (string_type const& name);
    static void         draw_test_triagle (float axis);
};

// ====================================================

} // namespace GL

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_RUNTIME_H_
