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

#ifndef CPPUAL_GFX_SPACE_SHADER_H_
#define CPPUAL_GFX_SPACE_SHADER_H_
#ifdef __cplusplus

#include <string>
#include <map>
#include <cppual/flags.h>
#include <cppual/common.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/draw.h>

using std::map;
using string;

namespace cppual { namespace Graphics {

enum class TessGenType : unsigned char
{
    Quads,
    Triangles,
    Isolines
};

enum class TessGenSpacing : unsigned char
{
    Equal,
    FractionalEven,
    FractionalOdd
};

enum class VertexOrder : bool
{
    OrientCW,
    OrientCCW
};

// ====================================================

class Binary
{
public:
    inline static uint getNumBinaryFormats () noexcept
    { return sm_uNumFormats; }

private:
    static cuint sm_uNumFormats;
};

// ====================================================

class Shader : public Binary
{
public:
    enum Type
    {
        Vertex         = 1 << 0,
        Fragment       = 1 << 1,
        Geometry       = 1 << 2,
        TessControl    = 1 << 3,
        TessEvaluation = 1 << 4,
        Compute        = 1 << 5,
        Atomic         = 1 << 6
    };

    typedef BitSet<Shader::Type> Types;

    Shader () = default;
    Shader (Shader::Type type) noexcept;
    Shader (Shader&&) noexcept;
    Shader& operator = (Shader&&) noexcept;

    string log  ();
    bool   load (string const& str, LoadFrom mode = LoadFrom::File);
    bool   compile () noexcept;

    inline string const& source () const noexcept { return _M_gSource; }
    inline Shader::Type  type   () const noexcept { return _M_eType  ; }

    inline bool isLoaded () const noexcept
    { return _M_gStates.test (Shader::IsLoaded); }

    inline bool isCompiled () const noexcept
    { return _M_gStates.test (Shader::IsCompiled); }

private:
    enum State
    {
        IsLoaded   = 1 << 0,
        IsCompiled = 1 << 1
    };

    typedef BitSet<Shader::State> States;

    string       _M_gSource;
    States       _M_gStates;
    Shader::Type _M_eType;

    bool loadFromFile   (string const& file);
    bool loadFromBinary (string const& file);
    bool loadFromMemory (string const& source);
};

struct FragmentShader final : public Shader
{
    inline FragmentShader () noexcept
    : Shader (Shader::Fragment)
    { }

    inline FragmentShader (string const& gString,
                           LoadFrom      eMode = LoadFrom::File)
    : Shader (Shader::Fragment)
    { load (gString, eMode); }
};

struct VertexShader final : public Shader
{
    inline VertexShader () noexcept
    : Shader (Shader::Vertex)
    { }

    inline VertexShader (string const& gString,
                         LoadFrom      eMode = LoadFrom::File)
    : Shader (Shader::Vertex)
    { load (gString, eMode); }
};

struct GeometryShader final : public Shader
{
    inline GeometryShader () noexcept
    : Shader (Shader::Geometry)
    { }

    inline GeometryShader (string const& gString,
                           LoadFrom      eMode = LoadFrom::File)
    : Shader (Shader::Geometry)
    { load (gString, eMode); }
};

struct ComputeShader final : public Shader
{
    inline ComputeShader () noexcept
    : Shader (Shader::Compute)
    { }

    inline ComputeShader (string const& gString,
                          LoadFrom      eMode = LoadFrom::File)
    : Shader (Shader::Compute)
    { load (gString, eMode); }
};

struct TessControlShader final : public Shader
{
    inline TessControlShader () noexcept
    : Shader (Shader::TessControl)
    { }

    inline TessControlShader (string const& gString,
                              LoadFrom      eMode = LoadFrom::File)
    : Shader (Shader::TessControl)
    { load (gString, eMode); }
};

struct TessEvaluationShader final : public Shader
{
    inline TessEvaluationShader () noexcept
    : Shader (Shader::TessEvaluation)
    { }

    inline TessEvaluationShader (string const& gString,
                                 LoadFrom      eMode = LoadFrom::File)
    : Shader (Shader::TessEvaluation)
    { load (gString, eMode); }
};

struct AtomicShader final : public Shader
{
    inline AtomicShader () noexcept
    : Shader (Shader::Atomic)
    { }

    inline AtomicShader (string const& gString,
                         LoadFrom      eMode = LoadFrom::File)
    : Shader (Shader::Fragment)
    { load (gString, eMode); }
};

// ====================================================

class SLProgram final : public Binary
{
public:
    enum State
    {
        IsLinked        = 1 << 0,
        BinaryAvailable = 1 << 1
    };

    typedef BitSet<SLProgram::State> States;

    SLProgram () noexcept;
    SLProgram (string const& binary_name) noexcept;
    SLProgram (SLProgram&&) noexcept;
    SLProgram& operator = (SLProgram&&) noexcept;

    string log ();
    void*  binary () noexcept;
    uint   binaryFormat () noexcept;
    int    addAttribute (string const& name);
    int    addUniform (string const& name);
    bool   loadFromBinary (string const& file);
    bool   attach (Shader const& shader);
    bool   detach (Shader const& shader);
    bool   isAttached (Shader const& shader);
    bool   link () noexcept;
    bool   validate () noexcept;
    void   use () noexcept;
    void   disable () noexcept;

    inline uint    shaderCount () const noexcept
    { return _M_uShaderCount; }

    inline bool isLinked () const noexcept
    { return _M_gStates.test (SLProgram::IsLinked); }

    inline int attribute (string const& gName)
    { return _M_gAttribLocList[gName]; }

    inline int uniform (string const& gName)
    { return _M_gUniformLocList[gName]; }

    inline bool hasFragmentShader () const noexcept
    { return _M_gShaderTypes.test (Shader::Fragment); }

    inline bool hasVertexShader () const noexcept
    { return _M_gShaderTypes.test (Shader::Vertex); }

    inline bool hasComputeShader () const noexcept
    { return _M_gShaderTypes.test (Shader::Compute); }

    inline bool hasTessControlShader () const noexcept
    { return _M_gShaderTypes.test (Shader::TessControl); }

    inline bool hasTessEvaluationShader () const noexcept
    { return _M_gShaderTypes.test (Shader::TessEvaluation); }

private:
    map<string, int> _M_gAttribLocList, _M_gUniformLocList;
    uint             _M_uShaderCount;
    Shader::Types    _M_gShaderTypes;
    States           _M_gStates;
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_SPACE_SHADER_H_
