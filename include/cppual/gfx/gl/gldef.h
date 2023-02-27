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

#ifndef CPPUAL_GFX_GL_DEFINES_H_
#define CPPUAL_GFX_GL_DEFINES_H_
#ifdef __cplusplus

#include <cppual/decl.h>

//#include <GL/glew.h>
#include "glad.h"

namespace cppual { namespace gfx { namespace gl {

enum
{
    // error
    InvalidEnum             = GL_INVALID_ENUM,
    InvalidValue            = GL_INVALID_VALUE,
    InvalidIndex            = GL_INVALID_INDEX,
    InvalidOperation        = GL_INVALID_OPERATION,
    InvalidFrameBufferOp    = GL_INVALID_FRAMEBUFFER_OPERATION,
    OutOfMemory             = GL_OUT_OF_MEMORY,

    // verticies
    Quads                   = GL_QUADS,

    // modes
    Projection              = GL_PROJECTION,
    ModelView               = GL_MODELVIEW,

    // shader
    FlatShader              = GL_FLAT,
    SmoothShader            = GL_SMOOTH,

    // operations
    IsProgramBinaryRetrievable = GL_PROGRAM_BINARY_RETRIEVABLE_HINT,
    NumProgramBinaryFormats    = GL_NUM_PROGRAM_BINARY_FORMATS,
    ProgramBinaryFormats       = GL_PROGRAM_BINARY_FORMATS,
    ProgramBinaryLength        = GL_PROGRAM_BINARY_LENGTH,
    LogLength                  = GL_INFO_LOG_LENGTH,
    CompileStatus              = GL_COMPILE_STATUS,
    LinkStatus                 = GL_LINK_STATUS,
    ValidateStatus             = GL_VALIDATE_STATUS,
    SpecifiedShaderType        = GL_SHADER_TYPE,
    QueryResult                = GL_QUERY_RESULT,
    IsQueryResultAvailable     = GL_QUERY_RESULT_AVAILABLE,
    AnySmaplesPassed           = GL_ANY_SAMPLES_PASSED,
    AnySamplesPassedConservative = GL_ANY_SAMPLES_PASSED_CONSERVATIVE,
    PrimitivesGenerated        = GL_PRIMITIVES_GENERATED,
    SamplesPassed              = GL_SAMPLES_PASSED,
    TimeElapsed                = GL_TIME_ELAPSED,
    Timestamp                  = GL_TIMESTAMP,
    TransformFeedbackPrimitivesWritten = GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,

    // formats
    FragmentShaderARB          = GL_FRAGMENT_SHADER,
    VertexShaderARB            = GL_VERTEX_SHADER,
    GeometryShaderARB          = GL_GEOMETRY_SHADER,
    TessControlShaderARB       = GL_TESS_CONTROL_SHADER,
    TessEvaluationShaderARB    = GL_TESS_EVALUATION_SHADER,
    ComputeShaderARB           = GL_COMPUTE_SHADER,
    Texture2D                  = GL_TEXTURE_2D,
    Nearest                    = GL_NEAREST,
    NearestMipMapNearest       = GL_NEAREST_MIPMAP_NEAREST,
    NearestMipMapLinear        = GL_NEAREST_MIPMAP_LINEAR,
    Linear                     = GL_LINEAR,
    LinearMipMapNearest        = GL_LINEAR_MIPMAP_NEAREST,
    LinearMipMapLinear         = GL_LINEAR_MIPMAP_LINEAR,
    Tex2DMagFilter             = GL_TEXTURE_MAG_FILTER,
    Tex2DMinFilter             = GL_TEXTURE_MIN_FILTER,
    RGB                        = GL_RGB,
    Luminance                  = GL_LUMINANCE,

    // primary
    InfoRenderer               = GL_RENDERER,
    InfoVendor                 = GL_VENDOR,
    InfoVersion                = GL_VERSION,
    InfoMajorVersion           = GL_MAJOR_VERSION,
    InfoMinorVersion           = GL_MINOR_VERSION,
    InfoSLVersion              = GL_SHADING_LANGUAGE_VERSION,
    InfoExtensions             = GL_EXTENSIONS,
    TRUE                       = GL_TRUE,
    FALSE                      = GL_FALSE
};

} } } // namespace GL

#endif // __cplusplus
#endif // CPPUAL_GFX_GL_DEFINES_H_
