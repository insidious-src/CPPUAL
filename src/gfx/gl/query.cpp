/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cppual/gfx/draw.h>
#include <cppual/gfx/gl/query.h>
#include "gldef.h"

namespace cppual { namespace Graphics { namespace GL {

namespace {

inline uint convertTargetType (Query::Type eType) noexcept
{
    switch (eType)
    {
    case Query::Type::AnySamplesPassed:
        return GL::AnySmaplesPassed;
    case Query::Type::AnySamplesPassedConservative:
        return GL::AnySamplesPassedConservative;
    case Query::Type::PrimitivesGenerated:
        return GL::PrimitivesGenerated;
    case Query::Type::SamplesPassed:
        return GL::SamplesPassed;
    case Query::Type::TimeElapsed:
        return GL::TimeElapsed;
    case Query::Type::Timestamp:
        return GL::Timestamp;
    case Query::Type::TransformFeedbackPrimitivesWritten:
        return GL::TransformFeedbackPrimitivesWritten;
    case Query::Type::Undefined:
        return 0;
    }
}

} // anonymous

// ====================================================

Query::Query  () noexcept
: Object      (ResourceType::Query)   ,
  m_eType     (Query::Type::Undefined),
  m_bHasQuery ()
{ }

Query::~Query () noexcept
{
    if (m_bHasQuery)
    {
        if (IDeviceContext::current ()->version () < 3)
            glEndQuery     (convertTargetType (m_eType));
        else
            glEndQueryARB  (convertTargetType (m_eType));
    }
}

u64 Query::get64 () noexcept
{
    if (m_bHasQuery)
    {
        u64 uResult;
        glGetQueryObjectui64v (id (), GL::QueryResult, &uResult);
        return uResult;
    }

    return 0;
}

uint Query::get () noexcept
{
    if (m_bHasQuery)
    {
        uint uResult;
        glGetQueryObjectuiv (id (), GL::QueryResult, &uResult);
        return uResult;
    }

    return 0;
}

bool Query::isReady () noexcept
{
    if (m_bHasQuery)
    {
        int nReady;
        glGetQueryObjectiv (id (), GL::QueryResult, &nReady);
        return nReady;
    }

    return false;
}

void Query::beginQuery (Query::Type eType) noexcept
{
    if (id () and !m_bHasQuery)
    {
        m_eType = eType;
        glBeginQuery (convertTargetType (eType), id ());
    }
}

void Query::beginQueryIndexed (Query::Type eType, uint uIdx) noexcept
{
    if (id () and !m_bHasQuery)
    {
        m_eType = eType;
        glBeginQueryIndexed (convertTargetType (eType), uIdx, id ());
    }
}

void Query::endQuery () noexcept
{
    if (m_bHasQuery) glEndQuery (convertTargetType (m_eType));
}

void Query::queryCounter (Query::Type eType) noexcept
{
    if (id () and !m_bHasQuery)
    {
        m_eType = eType;
        glQueryCounter (id (), convertTargetType (eType));
    }
}

} } } // namespace GL
