/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
 *
 * This program is free software: you can redistribute it &&/or modify
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

#include <cppual/multimedia/audio/al/aldevice.h>
#include <AL/alc.h>
#include "aldef.h"

namespace cppual { namespace audio { namespace al {

namespace { // optimize for internal unit usage

inline static instance*& current_context() noexcept
{
    static thread_local instance* inst = nullptr;
    return inst;
}

} // anonymous

// ====================================================

inline int convert_quality (output_format eFormat, sound_quality eQuality) noexcept
{
    switch (eFormat)
    {
    case output_format::mono:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::mono8;
        case sound_quality::medium:
            return al::mono16;
        case sound_quality::high:
            return al::mono32;
        }
        break;
    case output_format::stereo:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::stereo8;
        case sound_quality::medium:
            return al::stereo16;
        case sound_quality::high:
            return al::stereo32;
        }
        break;
    case output_format::rear:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::rear8;
        case sound_quality::medium:
            return al::rear16;
        case sound_quality::high:
            return al::rear32;
        }
        break;
    case output_format::quad:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::quad8;
        case sound_quality::medium:
            return al::quad16;
        case sound_quality::high:
            return al::quad32;
        }
        break;
    case output_format::ch51:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::ch51_8;
        case sound_quality::medium:
            return al::ch51_16;
        case sound_quality::high:
            return al::ch51_32;
        }
        break;
    case output_format::ch61:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::ch61_8;
        case sound_quality::medium:
            return al::ch61_16;
        case sound_quality::high:
            return al::ch61_32;
        }
        break;
    case output_format::ch71:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::ch71_8;
        case sound_quality::medium:
            return al::ch71_16;
        case sound_quality::high:
            return al::ch71_32;
        }
        break;
    default:
        switch (eQuality)
        {
        case sound_quality::low:
            return al::stereo8;
        case sound_quality::medium:
            return al::stereo16;
        case sound_quality::high:
            return al::stereo32;
        }
        break;
    }

    return 0;
}

inline distance_type convert_distance_model (int nModel) noexcept
{
    switch (nModel)
    {
    case al::inverse_distance:
        return distance_type::inverse_distance;
    case al::inverse_distance_clamped:
        return distance_type::inverse_distance_clamped;
    case al::linear_distance:
        return distance_type::linear_distance;
    case al::linear_distance_clamped:
        return distance_type::linear_distance_clamped;
    case al::exponent_distance:
        return distance_type::exponent_distance;
    case al::exponent_distance_clamped:
        return distance_type::exponent_distance_clamped;
    default:
        return distance_type::plain;
    }
}

inline int convert_distance_model (distance_type eModel) noexcept
{
    switch (eModel)
    {
    case distance_type::plain:
        return 0;
    case distance_type::inverse_distance:
        return al::inverse_distance;
    case distance_type::inverse_distance_clamped:
        return al::inverse_distance_clamped;
    case distance_type::linear_distance:
        return al::linear_distance;
    case distance_type::linear_distance_clamped:
        return al::linear_distance_clamped;
    case distance_type::exponent_distance:
        return al::exponent_distance;
    default:
        return al::exponent_distance_clamped;
    }
}

// ====================================================

bool audio_device::is_extension_supported (string_type const& gExt) noexcept
{
    return ::alIsExtensionPresent (gExt.c_str ());
}

bool audio_device::is_extension_present (string_type const& gExt) noexcept
{
    if (valid ())
        return ::alcIsExtensionPresent (handle<ALCdevice*> (), gExt.c_str ());
    return false;
}

// ====================================================

playback_device::playback_device () noexcept
: audio_device (::alcGetString (nullptr, ALC_DEFAULT_DEVICE_SPECIFIER),
                ::alcOpenDevice (name ().c_str ()),
                device_type::playback)
{ }

playback_device::playback_device (string_type const& gName) noexcept
: audio_device (std::move (gName),
                ::alcOpenDevice (gName.c_str ()),
                device_type::playback)
{ }

playback_device::~playback_device () noexcept
{
    if (valid ()) ::alcCloseDevice (handle<ALCdevice*> ());
}

// ====================================================

capture_device::capture_device () noexcept
: audio_device (::alcGetString (nullptr, ALC_DEFAULT_DEVICE_SPECIFIER),
                ::alcCaptureOpenDevice (name ().c_str (),
                                        22050,
                                        convert_quality (output_format::stereo,
                                                         sound_quality::low),
                                        0),
                device_type::capture),
  _M_eQuality (sound_quality::low),
  _M_eFormat (output_format::stereo)
{
}

capture_device::capture_device (string_type const& gName,
                                uint               uFreq,
                                output_format      eFormat,
                                sound_quality      eQuality) noexcept
: audio_device (gName,
                ::alcCaptureOpenDevice (gName.c_str (),
                                        uFreq,
                                        convert_quality (eFormat, eQuality),
                                        0),
                device_type::capture),
  _M_eQuality (eQuality),
  _M_eFormat (eFormat)
{ }

capture_device::~capture_device () noexcept
{
    if (valid ()) ::alcCaptureCloseDevice (handle<ALCdevice*> ());
}

// ====================================================

instance::instance (playback_device& gDevice, bool bMakeCurrent) noexcept
: _M_gDevice (&gDevice),
  _M_pDevContext (gDevice.valid () ?
                     ::alcCreateContext
                     (static_cast<ALCdevice*> (gDevice.handle ()), nullptr) :
                     nullptr)
{
    if (_M_pDevContext && bMakeCurrent &&
            ::alcMakeContextCurrent (static_cast<ALCcontext*> (_M_pDevContext)) == 1)
        current_context () = this;
}

instance::~instance () noexcept
{
    if (_M_pDevContext)
    {
        invalidate ();
        ::alcDestroyContext (static_cast<ALCcontext*> (_M_pDevContext));
    }
}

bool instance::use () noexcept
{
    if (current_context () != this && ::alcMakeContextCurrent (static_cast<ALCcontext*> (_M_pDevContext)) == 1)
    {
        current_context () = this;
        return true;
    }

    return false;
}

void instance::invalidate () noexcept
{
    if (current_context () == this && ::alcMakeContextCurrent (nullptr) == 1)
        current_context ()  = nullptr;
}

bool instance::set_capability (int nCap, bool bEnable) noexcept
{
    instance* pPrevContext = current_context ();

    if (_M_pDevContext && use ())
    {
        bEnable ? ::alEnable (nCap) : ::alDisable (nCap);

        if (pPrevContext && pPrevContext != this)
        {
            if (::alcMakeContextCurrent (static_cast<ALCcontext*> (pPrevContext->_M_pDevContext)) == 1)
                current_context () = pPrevContext;
        }

        return true;
    }

    return false;
}

bool instance::has_capability (int nCap) noexcept
{
    instance* pPrevContext = current_context ();

    if (_M_pDevContext && use ())
    {
        bool bHasCap = ::alIsEnabled (nCap) == 1;

        if (pPrevContext && pPrevContext != this)
        {
            if (::alcMakeContextCurrent
                    (static_cast<ALCcontext*> (pPrevContext->_M_pDevContext)) == 1)
                current_context () = pPrevContext;
        }

        return bHasCap;
    }

    return false;
}

void instance::process () noexcept
{
    if (_M_pDevContext) ::alcProcessContext (static_cast<ALCcontext*> (_M_pDevContext));
}

void instance::suspend () noexcept
{
    if (_M_pDevContext) ::alcSuspendContext (static_cast<ALCcontext*> (_M_pDevContext));
}

distance_type instance::distance_model () noexcept
{
    int nModel = 0;
    ::alGetIntegerv (al::current_distance_model, &nModel);
    return convert_distance_model (nModel);
}

void instance::set_distance_model (distance_type eModel) noexcept
{
    ::alDistanceModel (convert_distance_model (eModel));
}

instance* instance::current () noexcept
{
    return current_context ();
}

string instance::label (string_query) noexcept
{
    return { };
}

instance::ext_list instance::extensions () noexcept
{
    return { };
}

void instance::set_doppler_factor (float) noexcept
{
}

float instance::doppler_factor () noexcept
{
    return .0f;
}

void instance::set_speed_of_sound (float) noexcept
{
}

float instance::speed_of_sound () noexcept
{
    return .0f;
}

} } } // namespace al
