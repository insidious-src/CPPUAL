/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

namespace cppual { namespace Graphics { namespace GL {

struct BatchConfig
{
    uint    uRenderType;
    int     iPriority;
    uint    uTextureId;
    Matrix4 transformMatrix; //initialized as identity matrix

    BatchConfig (unsigned uRenderTypeIn, int iPriorityIn, unsigned uTextureIdIn)
    : uRenderType (uRenderTypeIn),
      iPriority (iPriorityIn),
      uTextureId (uTextureIdIn)
    { }

    bool operator == (BatchConfig const& other) const
    {
        return uRenderType      == other.uRenderType and
                iPriority       == other.iPriority   and
                uTextureId      == other.uTextureId/*  and
                transformMatrix == other.transformMatrix*/;
    }

    bool operator != (BatchConfig const& other) const
    { return !(*this == other); }
};

struct GuiVertex
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

class Batch final : NonCopyable
{
public:
    Batch (uint uMaxNumVertices);
    ~Batch ();

    Batch* fullest (Batch*);
    int    priority () const;
    bool   empty () const;
    bool   isBatchConfig (BatchConfig const&) const;
    bool   isEnoughRoom (uint uNumVertices) const;

    void add (std::vector<GuiVertex> const& vVertices, BatchConfig const&);
    void add (std::vector<GuiVertex> const& vVertices);
    void render ();

private:
    void clean ();

private:
    uint        m_uMaxNumVertices;
    uint        m_uNumUsedVertices;
    uint        m_vao; //only used in OpenGL v3.x +
    uint        m_vbo;
    BatchConfig m_config;
    GuiVertex   m_lastVertex;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GL_BATCH_H_
