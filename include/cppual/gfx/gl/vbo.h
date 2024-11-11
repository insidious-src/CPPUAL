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

#ifndef CPPUAL_GFX_VERTEX_H_
#define CPPUAL_GFX_VERTEX_H_
#ifdef __cplusplus

#include <cppual/bitset.h>
#include <cppual/common.h>
#include <cppual/string.h>
#include <cppual/gfx/coord.h>
#include <cppual/containers.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/gl/buffer.h>

namespace cppual { namespace gfx { namespace gl {

enum class usage_type : uchar
{
    stream_draw ,
    stream_read ,
    stream_copy ,
    static_draw ,
    static_read ,
    static_copy ,
    dynamic_draw,
    dynamic_read,
    dynamic_copy
};

struct gl_map_flag
{
    enum type
    {
        read              = 1 << 0,
        write             = 1 << 1,
        invalidate_range  = 1 << 2,
        invalidate_buffer = 1 << 3,
        explicit_flush    = 1 << 4,
        unsynchronized    = 1 << 5,
        persistent        = 1 << 6,
        coherent          = 1 << 7
    };
};

// ====================================================

typedef bitset<gl_map_flag::type> gl_map_flags;

// ====================================================

class vertex_buffer : public buffer
{
public:
    typedef vertex_buffer      self_type    ;
    typedef buffer             base_type    ;
    typedef u8                 value_type   ;
    typedef value_type*        pointer      ;
    typedef value_type const*  const_pointer;
    typedef vector<value_type> vector_type  ;

    vertex_buffer () noexcept;
    vertex_buffer (size_type size) noexcept;

    void* map_buffer_to_memory (access_mode access) noexcept;
    void* map_subbuffer_to_memory (difference_type offset, difference_type length, gl_map_flags flags) noexcept;
    void  unmap_buffer () noexcept;
    void  bind (buffer_type type) noexcept;
    void  upload (usage_type usage_hint);
    void  add_data (pointer data, size_type data_size);

    constexpr const_pointer get_data_pointer () const noexcept
    { return _M_bIsUploaded ? _M_gData.data () : nullptr; }

private:
    vector_type _M_gData      ;
    buffer_type _M_eType      ;
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
