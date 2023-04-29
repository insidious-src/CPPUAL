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

#define GL_GLEXT_PROTOTYPES

#include <cppual/gfx/gl/glbase.h>
#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/gldef.h>

namespace cppual { namespace gfx { namespace gl {

namespace { // optimize for internal unit usage

inline ::GLuint generate_object (resource_type eType)
{
    context_interface* pContext = context_interface::current ();

    if (!pContext or pContext->device () != device_backend::gl)
        throw bad_context ("NO GL context bound to thread");

    ::GLuint uId;

    switch (eType)
    {
    case resource_type::buffer:
        pContext->version () < 3 ?
                    glGenBuffers (1, &uId) : glGenBuffersARB (1, &uId);
        break;
    case resource_type::macro:
        glGenVertexArrays (1, &uId);
        break;
    case resource_type::program:
        uId = pContext->version () < 3 ?
                  glCreateProgram () : glCreateProgramObjectARB ();
        break;
    case resource_type::query:
        pContext->version () < 3 ?
                    glGenQueries (1, &uId) : glGenQueriesARB (1, &uId);
        break;
    case resource_type::texture:
        glGenTextures (1, &uId);
        if (uId) glBindTexture (gl::Texture2D, uId);
        break;
    default:
        uId = 0;
        break;
    }

    return uId;
}

inline ::GLuint generate_shader (::GLenum uType)
{
    context_interface* pContext = context_interface::current ();

    if (!pContext or pContext->device () != device_backend::gl)
        throw bad_context ("NO GL context bound to thread");

    return pContext->version () < 3 ?
                glCreateShader (uType) : glCreateShaderObjectARB (uType);
}

} // anonymous

// ====================================================

object::object (resource_type eType)
: resource     (generate_object (eType)),
  _M_eResType  (eType)
{ }

object::object (uint uShaderType)
: resource     (generate_shader (uShaderType)),
  _M_eResType  (resource_type::shader)
{ }

object::~object () noexcept
{
    auto const uId = handle ();

    if (uId)
    {
        if (context_interface::current ()->version () < 3)
        {
            switch (type ())
            {
            case resource_type::buffer:
                glDeleteBuffers (1, &uId);
                break;
            case resource_type::macro:
                glDeleteVertexArrays (1, &uId);
                break;
            case resource_type::program:
                glDeleteProgram (uId);
                break;
            case resource_type::query:
                glDeleteQueries (1, &uId);
                break;
            case resource_type::shader:
                glDeleteShader (uId);
                break;
            case resource_type::texture:
                glDeleteTextures (1, &uId);
                break;
            default:
                break;
            }
        }
        else
        {
            switch (type ())
            {
            case resource_type::buffer:
                glDeleteBuffersARB (1, &uId);
                break;
            case resource_type::program:
                glDeleteProgramsARB (1, &uId);
                break;
            case resource_type::query:
                glDeleteQueriesARB (1, &uId);
                break;
            default:
                glDeleteObjectARB (uId);
                break;
            }
        }
    }
}

} } } // namespace GL
