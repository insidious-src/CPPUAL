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

#include <cppual/multimedia/audio/al/alsource.h>
#include <cppual/multimedia/audio/format.h>
#include <cppual/error.h>

#include "aldef.h"

namespace cppual { namespace audio { namespace al {

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

sound_buffer::sound_buffer () noexcept
: object (object_type::buffer),
  _M_gSources (),
  _M_pContext (instance::current ())
{ }

sound_buffer::sound_buffer (sound_buffer&& gObj) noexcept
: object (std::move (gObj)),
  _M_gSources (std::move (gObj._M_gSources)),
  _M_pContext (instance::current ())
{
    for (size_type i = 0; i < _M_gSources.size (); ++i)
        if (_M_gSources[i]) _M_gSources[i]->_M_pBuffer = this;
}

sound_buffer& sound_buffer::operator = (sound_buffer&& gObj) noexcept
{
    for (size_type i = 0; i < _M_gSources.size (); ++i)
        if (_M_gSources[i]) _M_gSources[i]->_M_pBuffer = this;

    _M_gSources = std::move (gObj._M_gSources);
    return *this;
}

sound_buffer::~sound_buffer () noexcept
{
    if (valid ())
    {
        for (size_type i = 0; i < _M_gSources.size (); ++i)
            if (_M_gSources[i]) _M_gSources[i]->on_detach ();
    }
}

int sound_buffer::frequency () const noexcept
{
    int nFreq = 0;
    if (valid ()) ::alGetBufferi (handle (), al::frequency, &nFreq);
    return nFreq;
}

int sound_buffer::size () const noexcept
{
    int nSize = 0;
    if (valid ()) ::alGetBufferi (handle (), al::size, &nSize);
    return nSize;
}

int sound_buffer::bits () const noexcept
{
    int nBits = 0;
    if (valid ()) ::alGetBufferi (handle (), al::bits, &nBits);
    return nBits;
}

int sound_buffer::duration () const noexcept
{
    return 0;
}

int sound_buffer::on_open () noexcept
{
    return error::failed;
}

} } } // namespace Audio
