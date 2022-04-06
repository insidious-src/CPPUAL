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

#include <cppual/gfx/gl/vbo.h>
#include <cppual/gfx/draw.h>

#include "gldef.h"

namespace cppual { namespace gfx { namespace gl {

constexpr uint get_access_mode (access_mode eMode) noexcept
{
    switch (eMode)
    {
    case access_mode::read:
        return GL_READ_ONLY;
    case access_mode::write:
        return GL_WRITE_ONLY;
    case access_mode::read_write:
        return GL_READ_WRITE;
    default:
        return 0;
    }
}

constexpr uint get_buffer_type (BufferType eType) noexcept
{
    switch (eType)
    {
    case BufferType::Array:
        return GL_ARRAY_BUFFER;
    case BufferType::AtomicCounter:
        return GL_ATOMIC_COUNTER_BUFFER;
    case BufferType::CopyRead:
        return GL_COPY_READ_BUFFER;
    case BufferType::CopyWrite:
        return GL_COPY_WRITE_BUFFER;
#ifndef OS_ANDROID
    case BufferType::DispatchIndirect:
        return GL_DISPATCH_INDIRECT_BUFFER;
#endif
    case BufferType::DrawIndirect:
        return GL_DRAW_INDIRECT_BUFFER;
    case BufferType::ElementArray:
        return GL_ELEMENT_ARRAY_BUFFER;
    case BufferType::PixelPack:
        return GL_PIXEL_PACK_BUFFER;
    case BufferType::PixelUnpack:
        return GL_PIXEL_UNPACK_BUFFER;
#ifndef OS_ANDROID
    case BufferType::Query:
        return GL_QUERY_BUFFER;
    case BufferType::ShaderStorage:
        return GL_SHADER_STORAGE_BUFFER;
#endif
    case BufferType::Texture:
        return GL_TEXTURE_BUFFER;
    case BufferType::TransformFeedback:
        return GL_TRANSFORM_FEEDBACK_BUFFER;
    case BufferType::Uniform:
        return GL_UNIFORM_BUFFER;
    default:
        return 0;
    }
}

constexpr uint get_usage_type (UsageType eType) noexcept
{
    switch (eType)
    {
    case UsageType::StreamRead:
        return GL_STREAM_READ;
    case UsageType::StreamCopy:
        return GL_STREAM_COPY;
    case UsageType::StreamDraw:
        return GL_STREAM_DRAW;
    case UsageType::StaticRead:
        return GL_STATIC_READ;
    case UsageType::StaticCopy:
        return GL_STATIC_COPY;
    case UsageType::StaticDraw:
        return GL_STATIC_DRAW;
    case UsageType::DynamicRead:
        return GL_DYNAMIC_READ;
    case UsageType::DynamicCopy:
        return GL_DYNAMIC_COPY;
    case UsageType::DynamicDraw:
        return GL_DYNAMIC_DRAW;
    default:
        return 0;
    }
}

// ====================================================

vertex_buffer::vertex_buffer (size_type uSize) noexcept
: _M_gData (),
  _M_eType (),
  _M_bIsUploaded ()
{
    _M_gData.reserve (uSize);
}

void* vertex_buffer::map_buffer_to_memory (access_mode eMode) noexcept
{
    if (!_M_bIsUploaded) return nullptr;
    return context_interface::current ()->version () < 3 ?
                glMapBuffer    (get_buffer_type (_M_eType), get_access_mode (eMode)) :
                glMapBufferARB (get_buffer_type (_M_eType), get_access_mode (eMode)) ;
}

void* vertex_buffer::map_subbuffer_to_memory (difference_type    uOffset,
                                          difference_type    uLen,
                                          GLMapFlags gAccesFlags) noexcept
{
    if (!_M_bIsUploaded) return nullptr;
    return glMapBufferRange (get_buffer_type (_M_eType),
                             uOffset,
                             uLen,
                             gAccesFlags);
}

void vertex_buffer::unmap_buffer () noexcept
{
    if (handle ()) glUnmapBuffer (get_buffer_type (_M_eType));
}

void vertex_buffer::bind (BufferType eType) noexcept
{
    if (handle ())
    {
        _M_eType = eType;
        glBindBuffer (get_buffer_type (eType), handle ());
    }
}

void vertex_buffer::upload (UsageType eUsage)
{
    if (handle ())
    {
        glBufferData (get_buffer_type (_M_eType),
                      static_cast<difference_type> (_M_gData.size ()),
                      &_M_gData[0],
                      get_usage_type (eUsage));

        _M_bIsUploaded = true;
        _M_gData.clear ();
    }
}

void vertex_buffer::add_data (pointer pData, size_type uDataSize)
{
    if (handle ())
    {
        _M_gData.insert (_M_gData.end (), pData, pData + uDataSize);
        _M_bIsUploaded = false;
    }
}

// ====================================================

vertex_array::vertex_array () noexcept
: object (resource_type::macro)
{ }

} } } // namespace GL
