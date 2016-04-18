/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_GFX_GL_BUFFER_H_
#define CPPUAL_GFX_GL_BUFFER_H_
#ifdef __cplusplus

#include <cppual/gfx/gl/glbase.h>

namespace cppual { namespace Graphics { namespace GL {

enum class BufferType : unsigned char
{
    Array,
    AtomicCounter,
    CopyRead,
    CopyWrite,
    DrawIndirect,
    DispatchIndirect,
    ElementArray,
    PixelPack,
    PixelUnpack,
    Query,
    ShaderStorage,
    Texture,
    TransformFeedback,
    Uniform
};

class BufferObject : public Object
{
public:
    BufferObject () noexcept;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_BUFFER_H_
