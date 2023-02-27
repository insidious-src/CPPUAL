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

#include <cppual/multimedia/audio/al/listener.h>
#include <cppual/noncopyable.h>

#include <mutex>
#include <atomic>

#include "aldef.h"

namespace cppual { namespace audio { namespace al {

namespace { // optimize for internal unit usage

class Initializer final
{
public:
    std::mutex       listenerMutex ;
    std::atomic_bool listenerIsMute;
    float            listenerVolume;

private:
    Initializer () noexcept
    : listenerMutex (), listenerIsMute (), listenerVolume (1.0f)
    { }

    friend
    Initializer& get () noexcept;
};

inline Initializer& get () noexcept
{
    static Initializer internal;
    return internal;
}

} // anonymous namespace

// ====================================================

void listener::reset () noexcept
{
    int nOrient[6] { 0, 0, -1, 0, 1, 0 };

    ::alListener3i (al::position,  0, 0, 0);
    ::alListener3i (al::velocity,  0, 0, 0);
    ::alListener3i (al::direction, 0, 0, 0);
    ::alListeneriv (al::orientation, nOrient);
}

void listener::set_position (point3f const& gPos) noexcept
{
    ::alListenerfv (al::position, &gPos.x);
}

point3f listener::position () noexcept
{
    point3f gValue;
    ::alGetListenerfv (al::position, &gValue.x);
    return gValue;
}

void listener::set_velocity (point3f const& gVelocity) noexcept
{
    ::alListenerfv (al::velocity, &gVelocity.x);
}

point3f listener::velocity () noexcept
{
    point3f gValue;
    ::alGetListenerfv (al::velocity, &gValue.x);
    return gValue;
}

void listener::set_orientation (point3f const& gAt, point3f const& gUp) noexcept
{
    float fOrient[] { gAt.x, gAt.y, gAt.z, gUp.x, gUp.y, gUp.z };
    ::alListenerfv (al::orientation, &fOrient[0]);
}

point3f listener::orientation_at () noexcept
{
    float fOrient[6];
    ::alGetListenerfv (al::orientation, &fOrient[0]);
    return { fOrient[0], fOrient[1], fOrient[2] };
}

point3f listener::orientation_up () noexcept
{
    float fOrient[6];
    ::alGetListenerfv (al::orientation, &fOrient[0]);
    return { fOrient[3], fOrient[4], fOrient[5] };
}

void listener::set_volume (float fValue) noexcept
{
    if (fValue <= .0f)
    {
        ::alListenerf (al::volume, fValue);
        get ().listenerIsMute.store (true);
    }
    else
    {
        if (!get ().listenerIsMute.load (std::memory_order_relaxed))
        {
            ::alListenerf (al::volume, fValue);
        }

        get ().listenerMutex.lock ();
        get ().listenerVolume = fValue;
        get ().listenerMutex.unlock ();
    }
}

float listener::volume () noexcept
{
    std::lock_guard<std::mutex> gLock (get ().listenerMutex);
    return get ().listenerVolume;
}

bool listener::is_mute () noexcept
{
    return get ().listenerIsMute.load ();
}

void listener::mute () noexcept
{
    if (get ().listenerIsMute.load (std::memory_order_acquire)) return;
    ::alListenerf (al::volume, .0f);
    get ().listenerIsMute.store (true, std::memory_order_release);
}

void listener::unmute () noexcept
{
    if (!get ().listenerIsMute.load (std::memory_order_acquire)) return;
    get ().listenerMutex.lock ();
    ::alListenerf (al::volume, get ().listenerVolume);
    get ().listenerMutex.unlock ();
    get ().listenerIsMute.store (false, std::memory_order_release);
}

} } } // namespace Audio
