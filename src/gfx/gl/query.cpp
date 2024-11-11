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

#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/query.h>
#include <cppual/gfx/gl/gldef.h>

namespace cppual { namespace gfx { namespace gl {

namespace {

constexpr uint convert_target_type (query::Type eType) noexcept
{
    switch (eType)
    {
    case query::Type::AnySamplesPassed:
        return gl::AnySmaplesPassed;
    case query::Type::AnySamplesPassedConservative:
        return gl::AnySamplesPassedConservative;
    case query::Type::PrimitivesGenerated:
        return gl::PrimitivesGenerated;
    case query::Type::SamplesPassed:
        return gl::SamplesPassed;
    case query::Type::TimeElapsed:
        return gl::TimeElapsed;
    case query::Type::Timestamp:
        return gl::Timestamp;
    case query::Type::TransformFeedbackPrimitivesWritten:
        return gl::TransformFeedbackPrimitivesWritten;
    default:
        return 0;
    }
}

} // anonymous

// ====================================================

query::query   () noexcept
: object       (resource_type::query)   ,
  _M_eType     (query::Type::Undefined),
  _M_bHasQuery ()
{ }

query::~query () noexcept
{
    if (_M_bHasQuery)
    {
        if (context_interface::current ()->version () < 3)
            glEndQuery     (convert_target_type (_M_eType));
        else
            glEndQueryARB  (convert_target_type (_M_eType));
    }
}

u64 query::get64 () noexcept
{
    if (_M_bHasQuery)
    {
        u64 uResult;
        glGetQueryObjectui64v (handle (), gl::QueryResult, &uResult);
        return uResult;
    }

    return 0;
}

uint query::get () noexcept
{
    if (_M_bHasQuery)
    {
        uint uResult;
        glGetQueryObjectuiv (handle (), gl::QueryResult, &uResult);
        return uResult;
    }

    return 0;
}

bool query::is_ready () noexcept
{
    if (_M_bHasQuery)
    {
        int nReady;
        glGetQueryObjectiv (handle (), gl::QueryResult, &nReady);
        return nReady;
    }

    return false;
}

void query::begin_query (query::Type eType) noexcept
{
    if (handle () and !_M_bHasQuery)
    {
        _M_eType = eType;
        glBeginQuery (convert_target_type (eType), handle ());
    }
}

void query::begin_query_indexed (query::Type eType, uint uIdx) noexcept
{
    if (handle () and !_M_bHasQuery)
    {
        _M_eType = eType;
        glBeginQueryIndexed (convert_target_type (eType), uIdx, handle ());
    }
}

void query::end_query () noexcept
{
    if (_M_bHasQuery) glEndQuery (convert_target_type (_M_eType));
}

void query::query_counter (query::Type eType) noexcept
{
    if (handle () and !_M_bHasQuery)
    {
        _M_eType = eType;
        glQueryCounter (handle (), convert_target_type (eType));
    }
}

} } } // namespace GL
