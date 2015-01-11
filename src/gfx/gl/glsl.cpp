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

#include <fstream>
#include <algorithm>
#include <iterator>
#include <cppual/mstring.h>
#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/glsl.h>
#include "gldef.h"

using std::ios_base;
using std::ifstream;
using std::string;

namespace cppual { namespace Graphics { namespace GL {

namespace { // optimize for internal unit usage

string syncReadFile (string const& gFilePath, ios_base::openmode eMode = ios_base::in)
{
	typedef string::size_type size_type;

	ifstream gFile (gFilePath, eMode);
	string   gContent;

	if (gFile.is_open ())
	{
		//gFile.read ((string::pointer) &uSize, 2);
		size_type uSize = static_cast<size_type> (gFile.tellg ());
		gContent.reserve (uSize);

		// copy to string
		{
			std::istream_iterator<string::value_type> itCur (gFile);

			std::copy_n (itCur,
						 uSize,
						 std::insert_iterator<string> (gContent, gContent.begin ()));
		}

		gFile.close ();
	}

	return std::move (gContent);
}

inline static uint getShaderType (Shader::Type eType) noexcept
{
	switch (eType)
	{
	case Shader::Vertex:
		return GL::VertexShaderARB;
	case Shader::Geometry:
		return GL::GeometryShaderARB;
	case Shader::TessControl:
		return GL::TessControlShaderARB;
	case Shader::TessEvaluation:
		return GL::TessEvaluationShaderARB;
	case Shader::Compute:
		return GL::ComputeShaderARB;
	default:
		return GL::FragmentShaderARB;
	}
}

inline static Shader::Type getShaderType (int nType) noexcept
{
	switch (nType)
	{
	case GL::FragmentShaderARB:
		return Shader::Fragment;
	case GL::VertexShaderARB:
		return Shader::Vertex;
	case GL::GeometryShaderARB:
		return Shader::Geometry;
	case GL::TessControlShaderARB:
		return Shader::TessControl;
	case GL::TessEvaluationShaderARB:
		return Shader::TessEvaluation;
	case GL::ComputeShaderARB:
		return Shader::Compute;
	default:
		return Shader::Fragment;
	}
}

inline static uint formatCount () noexcept
{
	int nNumFormats = 0;
	glGetIntegerv (GL::NumProgramBinaryFormats, &nNumFormats);
	return static_cast<uint> (nNumFormats);
}

} // anonymous namespace

cuint Binary::sm_uNumFormats = formatCount ();

// ====================================================

Shader::Shader (Shader::Type eType) noexcept
: Object (getShaderType (eType)),
  m_gSource (),
  m_gStates (),
  m_eType (eType)
{ }

string Shader::log ()
{
	if (!id ()) return std::move (string ());

	string gLogString;
	int	   nLogLen;

	glGetShaderiv (id (), GL::LogLength, &nLogLen);

	if (nLogLen > 1)
	{
		gLogString.resize (static_cast<string::size_type> (nLogLen));
		glGetShaderInfoLog (id (), nLogLen, nullptr, &gLogString[0]);
	}

	return std::move (gLogString);
}

bool Shader::loadFromFile (string const& gFile)
{
	if (!id ()) return false;
	string gContent (std::move (syncReadFile (gFile)));

	if (gContent.length ())
	{
		m_gSource = std::move (gContent);

		if (IDeviceContext::current ()->version () < 3)
		{
			string::const_pointer pBuffer = m_gSource.c_str ();
			glShaderSource (id (), 1, &pBuffer, nullptr);
		}
		else
		{
			string::const_pointer pBuffer = m_gSource.c_str ();
			glShaderSourceARB (id (), 1, &pBuffer, nullptr);
		}

		m_gStates += Shader::IsLoaded;
		m_gStates -= Shader::IsCompiled;
		return true;
	}

	return false;
}

bool Shader::loadFromBinary (string const& gFile)
{
	if (!id ()) return false;
	string gContent (std::move (syncReadFile (gFile, ios_base::binary)));

	if (gContent.length ())
	{
		uint uId = id ();

		glShaderBinary (1, &uId, 0, gContent.c_str (),
				static_cast<GLsizei> (gContent.length ()));

		m_gSource.clear ();
		m_gStates -= Shader::IsLoaded;
		m_gStates += Shader::IsCompiled;
		return true;
	}

	return false;
}

bool Shader::loadFromMemory (string const& gSource)
{
	if (!id () or gSource.empty ()) return false;

	if (IDeviceContext::current ()->version () < 3)
	{
		string::const_pointer pBuffer =
				m_gSource.assign (std::move (gSource)).c_str ();
		glShaderSource (id (), 1, &pBuffer, nullptr);
	}
	else
	{
		string::const_pointer pBuffer =
				m_gSource.assign (std::move (gSource)).c_str ();
		glShaderSourceARB (id (), 1, &pBuffer, nullptr);
	}

	m_gStates += Shader::IsLoaded;
	m_gStates -= Shader::IsCompiled;
	return true;
}

bool Shader::compile () noexcept
{
	if (m_gStates.hasBit (Shader::IsCompiled)) return true;
	else if (!m_gStates.hasBit (Shader::IsLoaded)) return false;

	int	nStatus;

	// compile the shader
	IDeviceContext::current ()->version () < 3 ?
				glCompileShader    (id ()) :
				glCompileShaderARB (id ());

	// check the compilation status
	glGetShaderiv (id (), GL::CompileStatus, &nStatus);
	nStatus == GL::TRUE ? m_gStates += Shader::IsCompiled :
										 m_gStates -= Shader::IsCompiled;

	return m_gStates.hasBit (Shader::IsCompiled);
}

SLProgram::SLProgram () noexcept
: Object (ResourceType::Program),
  m_gAttribLocList (),
  m_gUniformLocList (),
  m_uShaderCount (),
  m_gShaderTypes (),
  m_gStates ()
{ }

SLProgram::SLProgram (string const& gBinaryName) noexcept
: Object (ResourceType::Program),
  m_gAttribLocList (),
  m_gUniformLocList (),
  m_uShaderCount (),
  m_gShaderTypes (),
  m_gStates ()
{ loadFromBinary (gBinaryName); }

bool SLProgram::link () noexcept
{
	if (id () and !m_gStates.hasBit (SLProgram::IsLinked) and m_uShaderCount >= 2)
	{
		int nStatus;

		// link the program
		if (IDeviceContext::current ()->version () < 3)
		{
			glLinkProgram  (id ());
			glGetProgramiv (id (), GL::LinkStatus, &nStatus);
		}
		else
		{
			glLinkProgramARB  (id ());
			glGetProgramivARB (id (), GL::LinkStatus, &nStatus);
		}

		// check linking status
		nStatus == GL::TRUE ? m_gStates += SLProgram::IsLinked :
								m_gStates -= SLProgram::IsLinked;

		return m_gStates.hasBit (SLProgram::IsLinked);
	}

	return false;
}

bool SLProgram::validate () noexcept
{
	if (id ())
	{
		int nStatus;

		// validate and check status
		if (IDeviceContext::current ()->version () < 3)
		{
			glValidateProgram (id ());
			glGetProgramiv    (id (), GL::ValidateStatus, &nStatus);
		}
		else
		{
			glValidateProgramARB (id ());
			glGetProgramivARB    (id (), GL::ValidateStatus, &nStatus);
		}

		return nStatus;
	}

	return false;
}

int SLProgram::addAttribute (string const& gName)
{
	m_gAttribLocList[gName] = IDeviceContext::current ()->version () < 3 ?
								  glGetAttribLocation    (id (), gName.c_str ()) :
								  glGetAttribLocationARB (id (), gName.c_str ());
	return m_gAttribLocList[gName];
}

int SLProgram::addUniform (string const& gName)
{
	m_gUniformLocList[gName] = IDeviceContext::current ()->version () < 3 ?
								   glGetUniformLocation    (id (), gName.c_str ()) :
								   glGetUniformLocationARB (id (), gName.c_str ());
	return m_gUniformLocList[gName];
}

void SLProgram::use () noexcept
{
	if (m_gStates.hasBit (SLProgram::IsLinked)) glUseProgram (id ());
}

void SLProgram::disable () noexcept
{
	if (m_gStates.hasBit (SLProgram::IsLinked)) glUseProgram (0);
}

bool SLProgram::loadFromBinary (string const& gFile)
{
	if (!id ()) return false;
	string gData (std::move (syncReadFile (gFile, ios_base::binary)));

	if (gData.length ())
	{
		uint uAttached[5];
		int  nStatus = 0, nCount = 0, nType = 0;

		// load binary
		glProgramBinary (id (),
						 0,
						 gData.c_str (),
						 static_cast<GLsizei> (gData.length ()));

		// check linking status
		IDeviceContext::current ()->version () < 3 ?
					glGetProgramiv    (id (), GL::LinkStatus, &nStatus) :
					glGetProgramivARB (id (), GL::LinkStatus, &nStatus);

		m_gShaderTypes = 0;

		if (nStatus == GL::FALSE)
		{
			m_uShaderCount = 0;
			m_gStates	  -= SLProgram::IsLinked;
			return false;
		}

		// get list of attached shaders
		IDeviceContext::current ()->version () < 3 ?
					glGetAttachedShaders (id (),
										  5,
										  &nCount,
										  &uAttached[0]) :
			glGetAttachedObjectsARB (id (),
									 5,
									 &nCount,
									 &uAttached[0]);

		// shader count
		m_uShaderCount = (uint) nCount;

		// set linked shader types
		while (nCount)
		{
			glGetShaderiv (uAttached[--nCount], GL::SpecifiedShaderType, &nType);

			if (!m_gShaderTypes.hasBit (getShaderType (nType)))
				m_gShaderTypes += getShaderType (nType);
		}

		m_gStates += SLProgram::IsLinked;
	}

	return true;
}

void* SLProgram::binary () noexcept
{
	void* pBinary = nullptr;

	if (m_gStates.hasBit (SLProgram::IsLinked))
	{
		int nBuffSize;

		IDeviceContext::current ()->version () < 3 ?
					glGetProgramiv    (id (), GL::ProgramBinaryLength, &nBuffSize) :
					glGetProgramivARB (id (), GL::ProgramBinaryLength, &nBuffSize);

		glGetProgramBinary (id (), nBuffSize, nullptr, nullptr, pBinary);
	}

	return pBinary;
}

uint SLProgram::binaryFormat () noexcept
{
	uint uFormat = 0;

	if (m_gStates.hasBit (SLProgram::IsLinked))
	{
		if (!m_gStates.hasBit (SLProgram::BinaryAvailable))
		{
			IDeviceContext::current ()->version () < 3 ?
						glProgramParameteri    (id (),
												GL::IsProgramBinaryRetrievable,
												GL::TRUE) :
						glProgramParameteriARB (id (),
												GL::IsProgramBinaryRetrievable,
												GL::TRUE);

			m_gStates += SLProgram::BinaryAvailable;
		}

		glGetProgramBinary (id (), 0, nullptr, &uFormat, nullptr);
	}

	return uFormat;
}

bool SLProgram::attach (Shader const& gShader)
{
	if (!id () or m_gShaderTypes.hasBit (gShader.type ()))
		return false;

	IDeviceContext::current ()->version () < 3 ?
				glAttachShader    (id (), gShader.id ()) :
				glAttachObjectARB (id (), gShader.id ());

	m_gShaderTypes += gShader.type ();
	++m_uShaderCount;
	return true;
}

bool SLProgram::detach (Shader const& gShader)
{
	if (!m_gShaderTypes.hasBit (gShader.type ()) or !isAttached (gShader))
		return false;

	IDeviceContext::current ()->version () < 3 ?
				glDetachShader    (id (), gShader.id ()) :
				glDetachObjectARB (id (), gShader.id ());

	m_gShaderTypes -= gShader.type ();
	--m_uShaderCount;
	return true;
}

bool SLProgram::isAttached (Shader const& gShader)
{
	if (!id () or !m_uShaderCount or !gShader.isValid ()) return false;

	uint uAttached[5];
	uint uCount = m_uShaderCount;

	// get list of attached shaders
	IDeviceContext::current ()->version () < 3 ?
				glGetAttachedShaders    (id (),
										 static_cast<GLsizei> (m_uShaderCount),
										 nullptr,
										 uAttached) :
				glGetAttachedObjectsARB (id (),
										 static_cast<GLsizei> (m_uShaderCount),
										 nullptr,
										 uAttached);

	// check if the shader is attached
	while (uCount) if (uAttached[--uCount] == gShader.id ()) return true;
	return false;
}

string SLProgram::log ()
{
	if (!id ()) return std::move (string ());

	string gLogString;
	int	   nLogLen;

	IDeviceContext::current ()->version () < 3 ?
				glGetProgramiv    (id (), GL::LogLength, &nLogLen) :
				glGetProgramivARB (id (), GL::LogLength, &nLogLen);

	if (nLogLen > 1)
	{
		gLogString.reserve (static_cast<string::size_type> (nLogLen));
		glGetProgramInfoLog (id (), nLogLen, nullptr, &gLogString[0]);
	}

	return std::move (gLogString);
}

} } } // namespace GL
