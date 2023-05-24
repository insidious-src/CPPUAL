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

#ifndef CPPUAL_GFX_GL_BASE_H_
#define CPPUAL_GFX_GL_BASE_H_
#ifdef __cplusplus

#include <cppual/resource.h>

#include <cstddef>

namespace cppual { namespace gfx { namespace gl {

// ====================================================

class buffer_object         ;
class shader                ;
class fragment_shader       ;
class vertex_shader         ;
class geometry_shader       ;
class compute_shader        ;
class tess_control_shader   ;
class tess_evaluation_shader;
class sl_program            ;
class query                 ;
class texture               ;
class frame_buffer          ;
class vertex_buffer         ;
class vertex_array          ;

// ====================================================

enum class shader_type : byte
{
    compute         = 1 << 0,
    vertex          = 1 << 1,
    tess_control    = 1 << 2,
    tess_evaluation = 1 << 3,
    geometry        = 1 << 4,
    fragment        = 1 << 5
};

typedef bitset<shader_type> shader_types;

// ====================================================

class object : public resource<void, uint>
{
public:
    typedef std::ptrdiff_t difference_type;
    typedef std::size_t    size_type      ;

    object  () noexcept = default;
    object  (resource_type type);
    object  (shader_type   shader_type);
    ~object () noexcept;

    resource_type type () const noexcept
    { return _M_eResType; }

private:
    resource_type _M_eResType { };
};

// ====================================================

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_BASE_H_
