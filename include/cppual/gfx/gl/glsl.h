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

#ifndef CPPUAL_GFX_GL_SL_H_
#define CPPUAL_GFX_GL_SL_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/common.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/gl/glbase.h>
#include <cppual/string.h>

#include <unordered_map>

namespace cppual { namespace gfx { namespace gl {

enum class TessGenType : byte
{
    Quads    ,
    Triangles,
    Isolines
};

enum class TessGenSpacing : byte
{
    Equal         ,
    FractionalEven,
    FractionalOdd
};

enum class VertexOrder : byte
{
    OrientCW,
    OrientCCW
};

// ====================================================

struct binary
{
    static uint binary_formats () noexcept;
};

// ====================================================

class shader : public object, public binary
{
public:
    typedef string string_type;

    shader () = default;
    shader (shader_type type) noexcept;
    shader (shader&&) noexcept;
    shader& operator = (shader&&) noexcept;

    string_type log ();
    bool        compile () noexcept;
    bool        load_from_file   (string_type const& file  );
    bool        load_From_binary (string_type const& file  );
    bool        load_from_memory (string_type const& source);

    string_type const& source () const noexcept { return _M_gSource; }
    shader_type        type   () const noexcept { return _M_eType  ; }

    bool is_loaded () const noexcept
    { return _M_gStates.test (shader::IsLoaded); }

    bool is_compiled () const noexcept
    { return _M_gStates.test (shader::IsCompiled); }

    bool load (string_type const& gString, load_from const eMode = load_from::file)
    {
        switch (eMode)
        {
        case load_from::file:
            return load_from_file   (gString);
        case load_from::memory:
            return load_from_memory (gString);
        case load_from::binary:
            return load_From_binary (gString);
        };
    }

private:
    enum State
    {
        IsLoaded   = 1 << 0,
        IsCompiled = 1 << 1
    };

    typedef bitset<shader::State> states;

private:
    string_type _M_gSource;
    states      _M_gStates;
    shader_type _M_eType  ;
};

// ====================================================

class fragment_shader : public shader
{
public:
    typedef string string_type;

    fragment_shader () noexcept
    : shader (shader_type::fragment)
    { }

    fragment_shader (string_type const& gString, load_from eMode = load_from::file)
    : shader (shader_type::fragment)
    { load (gString, eMode); }
};

// ====================================================

class vertex_shader : public shader
{
public:
    typedef string string_type;

    vertex_shader () noexcept
    : shader (shader_type::vertex)
    { }

    vertex_shader (string_type const& gString, load_from eMode = load_from::file)
    : shader (shader_type::vertex)
    { load (gString, eMode); }
};

// ====================================================

class geometry_shader : public shader
{
public:
    typedef string string_type;

    geometry_shader () noexcept
    : shader (shader_type::geometry)
    { }

    geometry_shader (string_type const& gString, load_from eMode = load_from::file)
    : shader (shader_type::geometry)
    { load (gString, eMode); }
};

// ====================================================

class compute_shader : public shader
{
public:
    typedef string string_type;

    compute_shader () noexcept
    : shader (shader_type::compute)
    { }

    compute_shader (string_type const& gString, load_from eMode = load_from::file)
    : shader (shader_type::compute)
    { load (gString, eMode); }
};

// ====================================================

class tess_control_shader : public shader
{
public:
    typedef string string_type;

    tess_control_shader () noexcept
    : shader (shader_type::tess_control)
    { }

    tess_control_shader (string_type const& gString, load_from eMode = load_from::file)
    : shader (shader_type::tess_control)
    { load (gString, eMode); }
};

// ====================================================

class tess_evaluation_shader : public shader
{
public:
    typedef string string_type;

    tess_evaluation_shader () noexcept
    : shader (shader_type::tess_evaluation)
    { }

    tess_evaluation_shader (string_type const& gString, load_from eMode = load_from::file)
    : shader (shader_type::tess_evaluation)
    { load (gString, eMode); }
};

// ====================================================

class sl_program final : public object, public binary
{
public:
    typedef string string_type;

    enum State
    {
        IsLinked        = 1 << 0,
        BinaryAvailable = 1 << 1
    };

    typedef std::unordered_map<string_type, int,
                               std::hash<string_type>, std::equal_to<string_type>,
                               memory::allocator<std::pair<const string_type, int>>> map_type;

    typedef bitset<sl_program::State> States;

    sl_program () noexcept;
    sl_program (string_type const& binary_name) noexcept;

    string_type log ();
    void*       binary () noexcept;
    uint        binary_format () noexcept;
    int         add_attribute (string_type const& name);
    int         add_uniform (string_type const& name);
    bool        load_from_binary (string_type const& file);
    bool        attach (shader const& shader);
    bool        detach (shader const& shader);
    bool        is_attached (shader const& shader);
    bool        link () noexcept;
    bool        validate () noexcept;
    void        use () noexcept;
    void        disable () noexcept;

    uint shader_count () const noexcept
    { return _M_uShaderCount; }

    bool is_linked () const noexcept
    { return _M_gStates.test (sl_program::IsLinked); }

    int attribute (string_type const& gName)
    { return _M_gAttribLocList[gName]; }

    int uniform (string_type const& gName)
    { return _M_gUniformLocList[gName]; }

    bool has_fragment_shader () const noexcept
    { return _M_gShaderTypes.test (shader_type::fragment); }

    bool has_vertex_shader () const noexcept
    { return _M_gShaderTypes.test (shader_type::vertex); }

    bool has_compute_shader () const noexcept
    { return _M_gShaderTypes.test (shader_type::compute); }

    bool has_tess_control_shader () const noexcept
    { return _M_gShaderTypes.test (shader_type::tess_control); }

    bool has_tess_evaluation_shader () const noexcept
    { return _M_gShaderTypes.test (shader_type::tess_evaluation); }

private:
    map_type     _M_gAttribLocList, _M_gUniformLocList;
    uint         _M_uShaderCount;
    shader_types _M_gShaderTypes;
    States       _M_gStates;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_SL_H_
