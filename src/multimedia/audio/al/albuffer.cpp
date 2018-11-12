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

#include <cppual/multimedia/audio/al/alsource.h>
#include <cppual/multimedia/audio/format.h>
#include <cppual/error.h>
#include "aldef.h"

using std::string;
using std::ifstream;
using std::ios_base;

namespace cppual { namespace Audio { namespace AL {

//inline int convertQuality (OutputFormat eFormat,
//                           SoundQuality eQuality) noexcept
//{
//    switch (eFormat)
//    {
//    case OutputFormat::Mono:
//        switch (eQuality)
//        {
//        case SoundQuality::Low:
//            return AL::Mono8;
//        case SoundQuality::Medium:
//            return AL::Mono16;
//        case SoundQuality::High:
//            return AL::Mono32;
//        }
//        break;
//    case OutputFormat::Stereo:
//        switch (eQuality)
//        {
//        case SoundQuality::Low:
//            return AL::Stereo8;
//        case SoundQuality::Medium:
//            return AL::Stereo16;
//        case SoundQuality::High:
//            return AL::Stereo32;
//        }
//        break;
//    case OutputFormat::Rear:
//        switch (eQuality)
//        {
//        case SoundQuality::Low:
//            return AL::Rear8;
//        case SoundQuality::Medium:
//            return AL::Rear16;
//        case SoundQuality::High:
//            return AL::Rear32;
//        }
//        break;
//    case OutputFormat::Quad:
//        switch (eQuality)
//        {
//        case SoundQuality::Low:
//            return AL::Quad8;
//        case SoundQuality::Medium:
//            return AL::Quad16;
//        case SoundQuality::High:
//            return AL::Quad32;
//        }
//        break;
//    case OutputFormat::CH51:
//        switch (eQuality)
//        {
//        case SoundQuality::Low:
//            return AL::CH51_8;
//        case SoundQuality::Medium:
//            return AL::CH51_16;
//        case SoundQuality::High:
//            return AL::CH51_32;
//        }
//        break;
//    case OutputFormat::CH61:
//        switch (eQuality)
//        {
//        case SoundQuality::Low:
//            return AL::CH61_8;
//        case SoundQuality::Medium:
//            return AL::CH61_16;
//        case SoundQuality::High:
//            return AL::CH61_32;
//        }
//        break;
//    case OutputFormat::CH71:
//        switch (eQuality)
//        {
//        case SoundQuality::Low:
//            return AL::CH71_8;
//        case SoundQuality::Medium:
//            return AL::CH71_16;
//        case SoundQuality::High:
//            return AL::CH71_32;
//        }
//        break;
//    }

//    return AL::Mono8;
//}

// ====================================================

SoundBuffer::SoundBuffer () noexcept
: Object (ObjectType::Buffer),
  m_gSources (),
  m_pContext (Instance::current ())
{ }

SoundBuffer::SoundBuffer (SoundBuffer&& gObj) noexcept
: Object (gObj),
  m_gSources (std::move (gObj.m_gSources)),
  m_pContext (Instance::current ())
{
    for (size_type i = 0; i < m_gSources.size (); ++i)
        if (m_gSources[i]) m_gSources[i]->m_pBuffer = this;
}

SoundBuffer::SoundBuffer (SoundBuffer const& gObj) noexcept
: Object (gObj),
  Sound (),
  m_gSources (),
  m_pContext (Instance::current ())
{ }

SoundBuffer& SoundBuffer::operator = (SoundBuffer&& gObj) noexcept
{
    for (size_type i = 0; i < m_gSources.size (); ++i)
        if (m_gSources[i]) m_gSources[i]->m_pBuffer = this;

    m_gSources = std::move (gObj.m_gSources);
    return *this;
}

SoundBuffer& SoundBuffer::operator = (SoundBuffer const& gObj) noexcept
{
    if (id () and gObj.id ())
    {
        for (size_type i = 0; i < m_gSources.size (); ++i)
            if (m_gSources[i]) m_gSources[i]->onDetach ();

        m_gSources.clear ();
    }

    return *this;
}

SoundBuffer::~SoundBuffer () noexcept
{
    if (id ())
    {
        for (size_type i = 0; i < m_gSources.size (); ++i)
            if (m_gSources[i]) m_gSources[i]->onDetach ();
    }
}

int SoundBuffer::getFrequency () const noexcept
{
    int nFreq = 0;
    if (id ()) alGetBufferi (id (), AL::Frequency, &nFreq);
    return nFreq;
}

int SoundBuffer::getSize () const noexcept
{
    int nSize = 0;
    if (id ()) alGetBufferi (id (), AL::Size, &nSize);
    return nSize;
}

int SoundBuffer::getBits () const noexcept
{
    int nBits = 0;
    if (id ()) alGetBufferi (id (), AL::Bits, &nBits);
    return nBits;
}

int SoundBuffer::getDuration () const noexcept
{
    return 0;
}

int SoundBuffer::onOpen () noexcept
{
    return Error::Failed;
}

} } } // namespace Audio
