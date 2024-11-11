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

#include <cppual/gfx/gl/glsl.h>
#include <cppual/string.h>
#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/gldef.h>

#include <fstream>
#include <algorithm>
#include <iterator>

namespace cppual { namespace gfx { namespace gl {

namespace { //! optimize for internal unit usage

typedef sl_program::string_type string_type;

string_type sync_read_file (string_type const& gFilePath, std::ios_base::openmode eMode = std::ios::in)
{
    typedef string_type::size_type size_type;

    std::ifstream gFile (gFilePath, eMode);
    string_type   gContent;

    if (gFile.is_open ())
    {
        //gFile.read ((string::pointer) &uSize, 2);
        size_type uSize = static_cast<size_type> (gFile.tellg ());
        gContent.reserve (uSize);

        //! copy to string
        {
            std::istream_iterator<string_type::value_type> itCur (gFile);

            std::copy_n (itCur,
                         uSize,
                         std::insert_iterator<string_type> (gContent, gContent.begin ()));
        }

        gFile.close ();
    }

    return gContent;
}

constexpr static shader_type convert_shader_type (int nType) noexcept
{
    switch (nType)
    {
    case gl::FragmentShaderARB:
        return shader_type::fragment;
    case gl::VertexShaderARB:
        return shader_type::vertex;
    case gl::GeometryShaderARB:
        return shader_type::geometry;
    case gl::TessControlShaderARB:
        return shader_type::tess_control;
    case gl::TessEvaluationShaderARB:
        return shader_type::tess_evaluation;
#ifndef OS_ANDROID
    case gl::ComputeShaderARB:
        return shader_type::compute;
#endif
    default:
        return shader_type::fragment;
    }
}

inline static uint format_count () noexcept
{
    int nNumFormats = 0;
    ::glGetIntegerv (gl::NumProgramBinaryFormats, &nNumFormats);
    return static_cast<uint> (nNumFormats);
}

} // anonymous namespace

// ====================================================

uint binary::binary_formats () noexcept
{
    static const auto formats = format_count ();
    return formats;
}

// ====================================================

shader::shader (shader_type eType) noexcept
: object     (eType),
  _M_gSource (),
  _M_gStates (),
  _M_eType   (eType)
{ }

shader::string_type shader::log ()
{
    if (!handle ()) return string_type ();

    string_type gLogString;
    int         nLogLen   ;

    ::glGetShaderiv (handle (), gl::LogLength, &nLogLen);

    if (nLogLen > 1)
    {
        gLogString.resize  (static_cast<string_type::size_type> (nLogLen));
        ::glGetShaderInfoLog (handle (), nLogLen, nullptr,  &gLogString[0]);
    }

    return gLogString;
}

bool shader::load_from_file (string_type const& gFile)
{
    if (!handle ()) return false;
    string_type gContent (sync_read_file (gFile));

    if (gContent.length ())
    {
        _M_gSource = std::move (gContent);

        if (context_interface::current ()->version () < 3)
        {
            string_type::const_pointer pBuffer = _M_gSource.c_str ();
            ::glShaderSource (handle (), 1, &pBuffer, nullptr);
        }
        else
        {
            string_type::const_pointer pBuffer = _M_gSource.c_str ();
            ::glShaderSourceARB (handle (), 1, &pBuffer, nullptr);
        }

        _M_gStates += shader::loaded;
        _M_gStates -= shader::compiled;
        return true;
    }

    return false;
}

bool shader::load_From_binary (string_type const& gFile)
{
    if (!handle ()) return false;
    string_type gContent (sync_read_file (gFile, std::ios::binary));

    if (gContent.length ())
    {
        uint uId = handle ();

        ::glShaderBinary (1, &uId, 0, gContent.c_str (), static_cast<GLsizei> (gContent.length ()));

        _M_gSource.clear ()             ;
        _M_gStates -= shader::loaded  ;
        _M_gStates += shader::compiled;
        return true;
    }

    return false;
}

bool shader::load_from_memory (string_type const& gSource)
{
    if (!handle () or gSource.empty ()) return false;

    string_type::const_pointer pBuffer =
            _M_gSource.assign (std::move (gSource)).c_str ();

    if (context_interface::current ()->version () < 3)
    {
        ::glShaderSource (handle (), 1, &pBuffer, nullptr);
    }
    else
    {
        ::glShaderSourceARB (handle (), 1, &pBuffer, nullptr);
    }

    _M_gStates += shader::loaded  ;
    _M_gStates -= shader::compiled;
    return true;
}

bool shader::compile () noexcept
{
    if      ( _M_gStates.test (shader::compiled)) return  true;
    else if (!_M_gStates.test (shader::loaded  )) return false;

    int nStatus;

    // compile the shader
    context_interface::current ()->version () < 3  ?
                     ::glCompileShader    (handle ()) :
                     ::glCompileShaderARB (handle ()) ;

    // check the compilation status
    ::glGetShaderiv (handle (), gl::CompileStatus, &nStatus);
    nStatus == gl::TRUE ? _M_gStates += shader::compiled :
                          _M_gStates -= shader::compiled ;

    return _M_gStates.test (shader::compiled);
}

sl_program::sl_program () noexcept
: object (resource_type::program),
  _M_gAttribLocList  (),
  _M_gUniformLocList (),
  _M_uShaderCount    (),
  _M_gShaderTypes    (),
  _M_gStates         ()
{ }

sl_program::sl_program (string_type const& gBinaryName) noexcept
: object (resource_type::program),
  _M_gAttribLocList  (),
  _M_gUniformLocList (),
  _M_uShaderCount    (),
  _M_gShaderTypes    (),
  _M_gStates         ()
{ load_from_binary (gBinaryName); }

bool sl_program::link () noexcept
{
    if (handle () and !_M_gStates.test (sl_program::linked) and _M_uShaderCount >= 2)
    {
        int nStatus;

        // link the program
        if (context_interface::current ()->version () < 3)
        {
            ::glLinkProgram  (handle ());
            ::glGetProgramiv (handle (), gl::LinkStatus, &nStatus);
        }
        else
        {
            ::glLinkProgramARB  (handle ());
            ::glGetProgramivARB (handle (), gl::LinkStatus, &nStatus);
        }

        // check linking status
        nStatus == gl::TRUE ? _M_gStates += sl_program::linked:
                              _M_gStates -= sl_program::linked;

        return _M_gStates.test (sl_program::linked);
    }

    return false;
}

bool sl_program::validate () noexcept
{
    if (handle ())
    {
        int nStatus;

        // validate and check status
        if (context_interface::current ()->version () < 3)
        {
            ::glValidateProgram (handle ());
            ::glGetProgramiv    (handle () , gl::ValidateStatus, &nStatus);
        }
        else
        {
            ::glValidateProgramARB (handle ());
            ::glGetProgramivARB    (handle () , gl::ValidateStatus, &nStatus);
        }

        return nStatus;
    }

    return false;
}

int sl_program::add_attribute (string_type const& gName)
{
    _M_gAttribLocList[gName] = context_interface::current ()->version () < 3 ?
                                  ::glGetAttribLocation    (handle (), gName.c_str ()) :
                                  ::glGetAttribLocationARB (handle (), gName.c_str ());
    return _M_gAttribLocList[gName];
}

int sl_program::add_uniform (string_type const& gName)
{
    _M_gUniformLocList[gName] = context_interface::current ()->version () < 3 ?
                                   ::glGetUniformLocation    (handle (), gName.c_str ()) :
                                   ::glGetUniformLocationARB (handle (), gName.c_str ());
    return _M_gUniformLocList[gName];
}

void sl_program::use () noexcept
{
    if (_M_gStates.test (sl_program::linked)) ::glUseProgram (handle ());
}

void sl_program::disable () noexcept
{
    if (_M_gStates.test (sl_program::linked)) ::glUseProgram (0);
}

bool sl_program::load_from_binary (string_type const& gFile)
{
    if (!handle ()) return false;
    string_type gData (sync_read_file (gFile, std::ios::binary));

    if (gData.length ())
    {
        uint uAttached[5];
        int  nStatus = 0, nCount = 0, nType = 0;

        // load binary
        ::glProgramBinary (handle (),
                         0,
                         gData.c_str (),
                         static_cast<GLsizei> (gData.length ()));

        // check linking status
        context_interface::current ()->version () < 3 ?
                    ::glGetProgramiv    (handle (), gl::LinkStatus, &nStatus) :
                    ::glGetProgramivARB (handle (), gl::LinkStatus, &nStatus);

        _M_gShaderTypes = 0;

        if (nStatus == gl::FALSE)
        {
            _M_uShaderCount = 0;
            _M_gStates     -= sl_program::linked;
            return false;
        }

        // get list of attached shaders
        context_interface::current ()->version () < 3 ?
                    glGetAttachedShaders (handle (),
                                          5,
                                          &nCount,
                                          &uAttached[0]) :
            glGetAttachedObjectsARB (handle (),
                                     5,
                                     &nCount,
                                     &uAttached[0]);

        // shader count
        _M_uShaderCount = static_cast<uint> (nCount);

        // set linked shader types
        while (nCount)
        {
            glGetShaderiv (uAttached[--nCount], gl::SpecifiedShaderType, &nType);

            if (!_M_gShaderTypes.test (convert_shader_type (nType)))
                _M_gShaderTypes += convert_shader_type (nType);
        }

        _M_gStates += sl_program::linked;
    }

    return true;
}

void* sl_program::binary () noexcept
{
    void* pBinary = nullptr;

    if (_M_gStates.test (sl_program::linked))
    {
        int nBuffSize;

        context_interface::current ()->version () < 3 ?
                    glGetProgramiv    (handle (), gl::ProgramBinaryLength, &nBuffSize) :
                    glGetProgramivARB (handle (), gl::ProgramBinaryLength, &nBuffSize);

        glGetProgramBinary (handle (), nBuffSize, nullptr, nullptr, pBinary);
    }

    return pBinary;
}

uint sl_program::binary_format () noexcept
{
    uint uFormat = 0;

    if (_M_gStates.test (sl_program::linked))
    {
        if (!_M_gStates.test (sl_program::binary_available))
        {
            context_interface::current ()->version () < 3 ?
                        ::glProgramParameteri    (handle (),
                                                gl::IsProgramBinaryRetrievable,
                                                gl::TRUE) :
                        ::glProgramParameteriARB (handle (),
                                                gl::IsProgramBinaryRetrievable,
                                                gl::TRUE);

            _M_gStates += sl_program::binary_available;
        }

        ::glGetProgramBinary (handle (), 0, nullptr, &uFormat, nullptr);
    }

    return uFormat;
}

bool sl_program::attach (shader const& gShader)
{
    if (!handle () or _M_gShaderTypes.test (gShader.type ()))
        return false;

    context_interface::current ()->version () < 3 ?
                ::glAttachShader    (handle (), gShader.handle ()) :
                ::glAttachObjectARB (handle (), gShader.handle ());

    _M_gShaderTypes += gShader.type ();
    ++_M_uShaderCount;
    return true;
}

bool sl_program::detach (shader const& gShader)
{
    if (!_M_gShaderTypes.test (gShader.type ()) or !is_attached (gShader))
        return false;

    context_interface::current ()->version () < 3 ?
                ::glDetachShader    (handle (), gShader.handle ()) :
                ::glDetachObjectARB (handle (), gShader.handle ());

    _M_gShaderTypes -= gShader.type ();
    --_M_uShaderCount;
    return true;
}

bool sl_program::is_attached (shader const& gShader)
{
    if (!handle () or !_M_uShaderCount or !gShader.valid ()) return false;

    uint uAttached[5];
    uint uCount = _M_uShaderCount;

    // get list of attached shaders
    context_interface::current ()->version () < 3 ?
                ::glGetAttachedShaders    (handle (),
                                           static_cast<GLsizei> (_M_uShaderCount),
                                           nullptr,
                                           uAttached) :
                ::glGetAttachedObjectsARB (handle (),
                                           static_cast<GLsizei> (_M_uShaderCount),
                                           nullptr,
                                           uAttached);

    // check if the shader is attached
    while (uCount) if (uAttached[--uCount] == gShader.handle ()) return true;
    return false;
}

sl_program::string_type sl_program::log ()
{
    if (!handle ()) return string_type ();

    string_type gLogString;
    int         nLogLen   ;

    context_interface::current ()->version () < 3 ?
                ::glGetProgramiv    (handle (), gl::LogLength, &nLogLen) :
                ::glGetProgramivARB (handle (), gl::LogLength, &nLogLen) ;

    if (nLogLen > 1)
    {
        gLogString.reserve (static_cast<string_type::size_type> (nLogLen));
        ::glGetProgramInfoLog (handle (), nLogLen, nullptr, &gLogString[0]);
    }

    return gLogString;
}

} } } // namespace GL
