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

#ifndef CPPUAL_GFX_VERTEX_H_
#define CPPUAL_GFX_VERTEX_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/common.h>
#include <cppual/string.h>
#include <cppual/gfx/coord.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/gl/buffer.h>

#include <vector>

namespace cppual { namespace gfx { namespace gl {

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

typedef bitset<GLMapFlag::Type> GLMapFlags;

// ====================================================

class vertex_buffer final : public buffer_object
{
public:
    typedef byte              value_type;
    typedef std::vector<byte> vector_type;
    typedef byte*             pointer;
    typedef byte const*       const_pointer;

    vertex_buffer () noexcept;
    vertex_buffer (size_type size) noexcept;
    void* map_buffer_to_memory (access_mode access) noexcept;
    void* map_subbuffer_to_memory (difference_type offset, difference_type length, GLMapFlags flags) noexcept;
    void  unmap_buffer () noexcept;
    void  bind (BufferType type) noexcept;
    void  upload (UsageType usage_hint);
    void  add_data (pointer data, size_type data_size);

    inline const_pointer getDataPointer () const noexcept
    { return _M_bIsUploaded ? &_M_gData[0] : nullptr; }

private:
    vector_type _M_gData;
    BufferType  _M_eType;
    bool        _M_bIsUploaded;
};

// ====================================================

class vertex_array final : public object
{
public:
    vertex_array () noexcept;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_VERTEX_H_
