/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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
#include <cppual/gfx/gl/gldef.h>

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

constexpr uint get_buffer_type (buffer_type eType) noexcept
{
    switch (eType)
    {
    case buffer_type::array:
        return GL_ARRAY_BUFFER;
    case buffer_type::atomic_counter:
        return GL_ATOMIC_COUNTER_BUFFER;
    case buffer_type::copy_read:
        return GL_COPY_READ_BUFFER;
    case buffer_type::copy_write:
        return GL_COPY_WRITE_BUFFER;
#   ifndef OS_ANDROID
    case buffer_type::dispatch_indirect:
        return GL_DISPATCH_INDIRECT_BUFFER;
#   endif
    case buffer_type::draw_indirect:
        return GL_DRAW_INDIRECT_BUFFER;
    case buffer_type::element_array:
        return GL_ELEMENT_ARRAY_BUFFER;
    case buffer_type::pixel_pack:
        return GL_PIXEL_PACK_BUFFER;
    case buffer_type::pixel_unpack:
        return GL_PIXEL_UNPACK_BUFFER;
#   ifndef OS_ANDROID
    case buffer_type::query:
        return GL_QUERY_BUFFER;
    case buffer_type::shader_storage:
        return GL_SHADER_STORAGE_BUFFER;
#   endif
    case buffer_type::texture:
        return GL_TEXTURE_BUFFER;
    case buffer_type::transform_feedback:
        return GL_TRANSFORM_FEEDBACK_BUFFER;
    case buffer_type::uniform:
        return GL_UNIFORM_BUFFER;
    default:
        return 0;
    }
}

constexpr uint get_usage_type (usage_type eType) noexcept
{
    switch (eType)
    {
    case usage_type::stream_read:
        return GL_STREAM_READ;
    case usage_type::stream_copy:
        return GL_STREAM_COPY;
    case usage_type::stream_draw:
        return GL_STREAM_DRAW;
    case usage_type::static_read:
        return GL_STATIC_READ;
    case usage_type::static_copy:
        return GL_STATIC_COPY;
    case usage_type::static_draw:
        return GL_STATIC_DRAW;
    case usage_type::dynamic_read:
        return GL_DYNAMIC_READ;
    case usage_type::dynamic_copy:
        return GL_DYNAMIC_COPY;
    case usage_type::dynamic_draw:
        return GL_DYNAMIC_DRAW;
    default:
        return uint ();
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
               ::glMapBuffer    (get_buffer_type (_M_eType), get_access_mode (eMode)) :
               ::glMapBufferARB (get_buffer_type (_M_eType), get_access_mode (eMode)) ;
}

void* vertex_buffer::map_subbuffer_to_memory (difference_type uOffset,
                                              difference_type uLen   ,
                                              gl_map_flags    gAccesFlags) noexcept
{
    if (!_M_bIsUploaded) return nullptr;

    return ::glMapBufferRange (get_buffer_type (_M_eType),
                               uOffset,
                               uLen,
                               gAccesFlags);
}

void vertex_buffer::unmap_buffer () noexcept
{
    if (handle ()) ::glUnmapBuffer (get_buffer_type (_M_eType));
}

void vertex_buffer::bind (buffer_type eType) noexcept
{
    if (handle ())
    {
        _M_eType = eType;
        glBindBuffer (get_buffer_type (eType), handle ());
    }
}

void vertex_buffer::upload (usage_type eUsage)
{
    if (handle ())
    {
        ::glBufferData (get_buffer_type (_M_eType),
                        static_cast<difference_type> (_M_gData.size ()),
                        _M_gData.data (),
                        get_usage_type (eUsage));

        _M_bIsUploaded = true;
        _M_gData.clear ();
    }
}

void vertex_buffer::add_data (pointer pData, size_type uDataSize)
{
    if (handle ())
    {
        _M_gData.insert (_M_gData.cend (), pData, pData + uDataSize);
        _M_bIsUploaded = false;
    }
}

// ====================================================

vertex_array::vertex_array () noexcept
: object (resource_type::macro)
{ }

} } } // namespace GL
