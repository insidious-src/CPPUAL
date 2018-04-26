/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#define GL_GLEXT_PROTOTYPES

#include <cppual/gfx/gl/glbase.h>
#include <cppual/gfx/gl/runtime.h>
#include <cppual/gfx/gl/surface.h>
#include "gldef.h"

namespace cppual { namespace Graphics { namespace GL {

namespace { // optimize for internal unit usage

inline uint generateObject (ResourceType eType)
{
    IDeviceContext* pContext = IDeviceContext::current ();

    if (!pContext or pContext->device () != DeviceType::GL)
        throw bad_context ("NO GL context bound to thread");

    uint uId;

    switch (eType)
    {
    case ResourceType::Buffer:
        pContext->version () < 3 ?
                    glGenBuffers (1, &uId) : glGenBuffersARB (1, &uId);
        break;
    case ResourceType::Macro:
        glGenVertexArrays (1, &uId);
        break;
    case ResourceType::Program:
        uId = pContext->version () < 3 ?
                  glCreateProgram () : glCreateProgramObjectARB ();
        break;
    case ResourceType::Query:
        pContext->version () < 3 ?
                    glGenQueries (1, &uId) : glGenQueriesARB (1, &uId);
        break;
    case ResourceType::Texture:
        glGenTextures (1, &uId);
        if (uId) glBindTexture (GL::Texture2D, uId);
        break;
    default:
        uId = 0;
        break;
    }

    return uId;
}

inline uint generateShader (uint uType)
{
    IDeviceContext* pContext = IDeviceContext::current ();

    if (!pContext or pContext->device () != DeviceType::GL)
        throw bad_context ("NO GL context bound to thread");

    return pContext->version () < 3 ?
                glCreateShader (uType) : glCreateShaderObjectARB (uType);
}

} // anonymous

// ====================================================

Object::Object (ResourceType eType)
: Resource   (generateObject (eType)),
  m_eResType (eType)
{ }

Object::Object (uint uShaderType)
: Resource   (generateShader (uShaderType)),
  m_eResType (ResourceType::Shader)
{ }

Object::~Object () noexcept
{
    uint uId = id ();

    if (uId)
    {
        if (IDeviceContext::current ()->version () < 3)
        {
            switch (resType ())
            {
            case ResourceType::Buffer:
                glDeleteBuffers (1, &uId);
                break;
            case ResourceType::Macro:
                glDeleteVertexArrays (1, &uId);
                break;
            case ResourceType::Program:
                glDeleteProgram (uId);
                break;
            case ResourceType::Query:
                glDeleteQueries (1, &uId);
                break;
            case ResourceType::Shader:
                glDeleteShader (uId);
                break;
            case ResourceType::Texture:
                glDeleteTextures (1, &uId);
                break;
            default:
                break;
            }
        }
        else
        {
            switch (resType ())
            {
            case ResourceType::Buffer:
                glDeleteBuffersARB (1, &uId);
                break;
            case ResourceType::Program:
                glDeleteProgramsARB (1, &uId);
                break;
            case ResourceType::Query:
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
