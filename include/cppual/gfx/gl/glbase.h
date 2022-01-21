/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#ifndef CPPUAL_GFX_GL_BASE_H_
#define CPPUAL_GFX_GL_BASE_H_
#ifdef __cplusplus

#include <cppual/resource.h>

#include <cstddef>

namespace cppual { namespace Graphics { namespace GL {

class BufferObject;
class Shader;
class SLProgram;
class Query;
class Texture;
class FrameBuffer;
class VertexBuffer;
class VertexArray;

// ====================================================

class Object : public Resource <void, uint>
{
public:
    typedef std::ptrdiff_t ptrdiff;
    typedef std::size_t    size_type;

    Object  () noexcept = default;
    Object  (ResourceType type);
    Object  (uint  shader_type);
    ~Object () noexcept;

    ResourceType resType () const noexcept
    { return m_eResType; }

private:
    ResourceType m_eResType;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_BASE_H_
