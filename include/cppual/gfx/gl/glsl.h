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

#ifndef CPPUAL_GFX_GL_SL_H_
#define CPPUAL_GFX_GL_SL_H_
#ifdef __cplusplus

#include <cppual/flags.h>
#include <cppual/common.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/gl/glbase.h>
#include <cppual/string.h>

#include <unordered_map>

namespace cppual { namespace Graphics { namespace GL {

enum class TessGenType : unsigned char
{
    Quads    ,
    Triangles,
    Isolines
};

enum class TessGenSpacing : unsigned char
{
    Equal         ,
    FractionalEven,
    FractionalOdd
};

enum class VertexOrder : bool
{
    OrientCW,
    OrientCCW
};

// ====================================================

struct Binary
{
    static uint binaryFormats () noexcept;
};

// ====================================================

class Shader : public Object, public Binary
{
public:
    typedef string string_type;

    enum Type
    {
        Vertex         = 1 << 0,
        Fragment       = 1 << 1,
        Geometry       = 1 << 2,
        TessControl    = 1 << 3,
        TessEvaluation = 1 << 4,
        Compute        = 1 << 5
    };

    typedef BitSet<Shader::Type> Types;

    Shader () = default;
    Shader (Shader::Type type) noexcept;
    Shader (Shader&&) noexcept;
    Shader& operator = (Shader&&) noexcept;

    string_type log ();
    bool        compile () noexcept;
    bool        loadFromFile   (string_type const& file  );
    bool        loadFromBinary (string_type const& file  );
    bool        loadFromMemory (string_type const& source);

    string_type const& source () const noexcept { return m_gSource; }
    Shader::Type       type   () const noexcept { return m_eType  ; }

    bool isLoaded () const noexcept
    { return m_gStates.test (Shader::IsLoaded); }

    bool isCompiled () const noexcept
    { return m_gStates.test (Shader::IsCompiled); }

    bool load (string_type const& gString, LoadFrom const eMode = LoadFrom::File)
    {
        switch (eMode)
        {
        case LoadFrom::File:
            return loadFromFile   (gString);
        case LoadFrom::Memory:
            return loadFromMemory (gString);
        case LoadFrom::Binary:
            return loadFromBinary (gString);
        };
    }

private:
    enum State
    {
        IsLoaded   = 1 << 0,
        IsCompiled = 1 << 1
    };

    typedef BitSet<Shader::State> States;

private:
    string_type  m_gSource;
    States       m_gStates;
    Shader::Type m_eType  ;
};

struct FragmentShader : public Shader
{
    typedef string string_type;

    FragmentShader () noexcept
    : Shader (Shader::Fragment)
    { }

    FragmentShader (string_type const& gString, LoadFrom eMode = LoadFrom::File)
    : Shader (Shader::Fragment)
    { load (gString, eMode); }
};

struct VertexShader : public Shader
{
    typedef string string_type;

    VertexShader () noexcept
    : Shader (Shader::Vertex)
    { }

    VertexShader (string_type const& gString, LoadFrom eMode = LoadFrom::File)
    : Shader (Shader::Vertex)
    { load (gString, eMode); }
};

struct GeometryShader : public Shader
{
    typedef string string_type;

    GeometryShader () noexcept
    : Shader (Shader::Geometry)
    { }

    GeometryShader (string_type const& gString, LoadFrom eMode = LoadFrom::File)
    : Shader (Shader::Geometry)
    { load (gString, eMode); }
};

struct ComputeShader : public Shader
{
    typedef string string_type;

    ComputeShader () noexcept
    : Shader (Shader::Compute)
    { }

    ComputeShader (string_type const& gString, LoadFrom eMode = LoadFrom::File)
    : Shader (Shader::Compute)
    { load (gString, eMode); }
};

struct TessControlShader : public Shader
{
    typedef string string_type;

    TessControlShader () noexcept
    : Shader (Shader::TessControl)
    { }

    TessControlShader (string_type const& gString, LoadFrom eMode = LoadFrom::File)
    : Shader (Shader::TessControl)
    { load (gString, eMode); }
};

struct TessEvaluationShader : public Shader
{
    typedef string string_type;

    TessEvaluationShader () noexcept
    : Shader (Shader::TessEvaluation)
    { }

    TessEvaluationShader (string_type const& gString, LoadFrom eMode = LoadFrom::File)
    : Shader (Shader::TessEvaluation)
    { load (gString, eMode); }
};

// ====================================================

class SLProgram final : public Object, public Binary
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
                               Memory::Allocator<std::pair<const string_type, int>>> map_type;

    typedef BitSet<SLProgram::State> States;

    SLProgram () noexcept;
    SLProgram (string_type const& binary_name) noexcept;

    string_type log ();
    void*       binary () noexcept;
    uint        binaryFormat () noexcept;
    int         addAttribute (string_type const& name);
    int         addUniform (string_type const& name);
    bool        loadFromBinary (string_type const& file);
    bool        attach (Shader const& shader);
    bool        detach (Shader const& shader);
    bool        isAttached (Shader const& shader);
    bool        link () noexcept;
    bool        validate () noexcept;
    void        use () noexcept;
    void        disable () noexcept;

    uint shaderCount () const noexcept
    { return m_uShaderCount; }

    bool isLinked () const noexcept
    { return m_gStates.test (SLProgram::IsLinked); }

    int attribute (string_type const& gName)
    { return m_gAttribLocList[gName]; }

    int uniform (string_type const& gName)
    { return m_gUniformLocList[gName]; }

    bool hasFragmentShader () const noexcept
    { return m_gShaderTypes.test (Shader::Fragment); }

    bool hasVertexShader () const noexcept
    { return m_gShaderTypes.test (Shader::Vertex); }

    bool hasComputeShader () const noexcept
    { return m_gShaderTypes.test (Shader::Compute); }

    bool hasTessControlShader () const noexcept
    { return m_gShaderTypes.test (Shader::TessControl); }

    bool hasTessEvaluationShader () const noexcept
    { return m_gShaderTypes.test (Shader::TessEvaluation); }

private:
    map_type      m_gAttribLocList, m_gUniformLocList;
    uint          m_uShaderCount;
    Shader::Types m_gShaderTypes;
    States        m_gStates;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_SL_H_
