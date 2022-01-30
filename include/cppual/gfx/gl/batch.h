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

#ifndef CPPUAL_GL_BATCH_H_
#define CPPUAL_GL_BATCH_H_
#ifdef __cplusplus

#include <vector>
#include <cppual/noncopyable.h>
#include <cppual/gfx/matrix.h>
#include <cppual/gfx/gl/glbase.h>

namespace cppual { namespace gfx { namespace gl {

struct batch_config
{
    uint    uRenderType;
    int     iPriority;
    uint    uTextureId;
    matrix4 transformMatrix; //initialized as identity matrix

    batch_config (unsigned uRenderTypeIn, int iPriorityIn, unsigned uTextureIdIn)
    : uRenderType (uRenderTypeIn),
      iPriority   (iPriorityIn),
      uTextureId  (uTextureIdIn)
    { }

    bool operator == (batch_config const& other) const
    {
        return uRenderType      == other.uRenderType and
                iPriority       == other.iPriority   and
                uTextureId      == other.uTextureId/*  and
                transformMatrix == other.transformMatrix*/;
    }

    bool operator != (batch_config const& other) const
    { return !(*this == other); }
};

struct gui_vertex
{
//    glm::vec2 position;
//    glm::vec4 color;
//    glm::vec2 texture;

//    GuiVertex (glm::vec2 positionIn, glm::vec4 colorIn, glm::vec2 textureIn = glm::vec2 ())
//    : position (positionIn),
//      color (colorIn),
//      texture (textureIn)
//    { }
};

class batch final : non_copyable
{
public:
    batch  (uint uMaxNumVertices);
    ~batch ();

    batch* fullest (batch*);
    int    priority () const;
    bool   empty () const;
    bool   is_batch_config (batch_config const&) const;
    bool   is_enough_room (uint uNumVertices) const;

    void add (vector<gui_vertex> const& vertices, batch_config const&);
    void add (vector<gui_vertex> const& vertices);
    void render ();

private:
    void clean ();

private:
    uint         _M_uMaxNumVertices;
    uint         _M_uNumUsedVertices;
    uint         _M_vao; //only used in OpenGL v3.x +
    uint         _M_vbo;
    batch_config _M_config;
    gui_vertex   _M_lastVertex;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GL_BATCH_H_
