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

#ifndef CPPUAL_GFX_VERTEX_H_
#define CPPUAL_GFX_VERTEX_H_
#ifdef __cplusplus

#include <string>
#include <vector>
#include <cppual/flags.h>
#include <cppual/common.h>
#include <cppual/gfx/coord.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/gl/buffer.h>

using std::string;
using std::vector;

namespace cppual { namespace Graphics { namespace GL {

enum class UsageType : unsigned char
{
    StreamDraw,
    StreamRead,
    StreamCopy,
    StaticDraw,
    StaticRead,
    StaticCopy,
    DynamicDraw,
    DynamicRead,
    DynamicCopy
};

struct GLMapFlag final
{
    enum Type
    {
        Read             = 1 << 0,
        Write             = 1 << 1,
        InvalidateRange  = 1 << 2,
        InvalidateBuffer = 1 << 3,
        ExplicitFlush     = 1 << 4,
        Unsynchronized     = 1 << 5,
        Persistent         = 1 << 6,
        Coherent         = 1 << 7
    };
};

// ====================================================

typedef BitSet<GLMapFlag::Type> GLMapFlags;

// ====================================================

class VertexBuffer final : public BufferObject
{
public:
    typedef byte         value_type;
    typedef vector<byte> vector_type;
    typedef byte*        pointer;
    typedef byte const*  const_pointer;

    VertexBuffer () noexcept;
    VertexBuffer (size_type size) noexcept;
    void* mapBufferToMemory (AccessMode access) noexcept;
    void* mapSubBufferToMemory (ptrdiff offset, ptrdiff length, GLMapFlags flags) noexcept;
    void  unmapBuffer () noexcept;
    void  bind (BufferType type) noexcept;
    void  upload (UsageType usage_hint);
    void  addData (pointer data, size_type data_size);

    inline const_pointer getDataPointer () const noexcept
    { return m_bIsUploaded ? &m_gData[0] : nullptr; }

private:
    vector_type m_gData;
    BufferType  m_eType;
    bool        m_bIsUploaded;
};

// ====================================================

class VertexArray final : public Object
{
public:
    VertexArray () noexcept;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_VERTEX_H_
