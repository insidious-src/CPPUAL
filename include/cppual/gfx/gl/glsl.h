/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <string>
#include <unordered_map>
#include <cppual/flags.h>
#include <cppual/common.h>
#include <cppual/noncopyable.h>
#include <cppual/gfx/gl/glbase.h>

using std::unordered_map;
using std::string;

namespace cppual { namespace Graphics { namespace GL {

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
	static uint binaryFormats () noexcept
	{ return sm_uNumFormats; }

private:
	static cuint sm_uNumFormats;
};

// ====================================================

class Shader : public Object, public Binary
{
public:
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

	string log ();
	bool   compile () noexcept;
	bool   loadFromFile   (string const& file);
	bool   loadFromBinary (string const& file);
	bool   loadFromMemory (string const& source);

	string const& source () const noexcept { return m_gSource; }
	Shader::Type  type   () const noexcept { return m_eType;   }

	bool isLoaded () const noexcept
	{ return m_gStates.hasBit (Shader::IsLoaded); }

	bool isCompiled () const noexcept
	{ return m_gStates.hasBit (Shader::IsCompiled); }

	bool load (string const& gString, LoadFrom const eMode = LoadFrom::File)
	{
		switch (eMode)
		{
		case LoadFrom::File:
			return loadFromFile (gString);
		case LoadFrom::Memory:
			return loadFromMemory (gString);
		default:
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
	string       m_gSource;
	States       m_gStates;
	Shader::Type m_eType;
};

struct FragmentShader : public Shader
{
	FragmentShader () noexcept
	: Shader (Shader::Fragment)
	{ }

	FragmentShader (string const& gString, LoadFrom eMode = LoadFrom::File)
	: Shader (Shader::Fragment)
	{ load (gString, eMode); }
};

struct VertexShader : public Shader
{
	VertexShader () noexcept
	: Shader (Shader::Vertex)
	{ }

	VertexShader (string const& gString, LoadFrom eMode = LoadFrom::File)
	: Shader (Shader::Vertex)
	{ load (gString, eMode); }
};

struct GeometryShader : public Shader
{
	GeometryShader () noexcept
	: Shader (Shader::Geometry)
	{ }

	GeometryShader (string const& gString, LoadFrom eMode = LoadFrom::File)
	: Shader (Shader::Geometry)
	{ load (gString, eMode); }
};

struct ComputeShader : public Shader
{
	ComputeShader () noexcept
	: Shader (Shader::Compute)
	{ }

	ComputeShader (string const& gString, LoadFrom eMode = LoadFrom::File)
	: Shader (Shader::Compute)
	{ load (gString, eMode); }
};

struct TessControlShader : public Shader
{
	TessControlShader () noexcept
	: Shader (Shader::TessControl)
	{ }

	TessControlShader (string const& gString, LoadFrom eMode = LoadFrom::File)
	: Shader (Shader::TessControl)
	{ load (gString, eMode); }
};

struct TessEvaluationShader : public Shader
{
	TessEvaluationShader () noexcept
	: Shader (Shader::TessEvaluation)
	{ }

	TessEvaluationShader (string const& gString, LoadFrom eMode = LoadFrom::File)
	: Shader (Shader::TessEvaluation)
	{ load (gString, eMode); }
};

// ====================================================

class SLProgram final : public Object, public Binary
{
public:
	enum State
	{
		IsLinked		= 1 << 0,
		BinaryAvailable = 1 << 1
	};

	typedef unordered_map<string, int> map_type;
	typedef BitSet<SLProgram::State>   States;

	SLProgram () noexcept;
	SLProgram (string const& binary_name) noexcept;

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

	uint shaderCount () const noexcept
	{ return m_uShaderCount; }

	bool isLinked () const noexcept
	{ return m_gStates.hasBit (SLProgram::IsLinked); }

	int attribute (string const& gName)
	{ return m_gAttribLocList[gName]; }

	int uniform (string const& gName)
	{ return m_gUniformLocList[gName]; }

	bool hasFragmentShader () const noexcept
	{ return m_gShaderTypes.hasBit (Shader::Fragment); }

	bool hasVertexShader () const noexcept
	{ return m_gShaderTypes.hasBit (Shader::Vertex); }

	bool hasComputeShader () const noexcept
	{ return m_gShaderTypes.hasBit (Shader::Compute); }

	bool hasTessControlShader () const noexcept
	{ return m_gShaderTypes.hasBit (Shader::TessControl); }

	bool hasTessEvaluationShader () const noexcept
	{ return m_gShaderTypes.hasBit (Shader::TessEvaluation); }

private:
	map_type      m_gAttribLocList, m_gUniformLocList;
	uint          m_uShaderCount;
	Shader::Types m_gShaderTypes;
	States        m_gStates;
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_SL_H_
