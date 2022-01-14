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

#include <cppual/noncopyable.h>
#include <cppual/multimedia/audio/al/listener.h>

#include <mutex>
#include <atomic>

#include "aldef.h"

namespace cppual { namespace Audio { namespace AL {

namespace { // optimize for internal unit usage

class Initializer final
{
public:
    std::mutex       listenerMutex;
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

void Listener::reset () noexcept
{
    int nOrient[6] { 0, 0, -1, 0, 1, 0 };

    ::alListener3i (AL::Position,  0, 0, 0);
    ::alListener3i (AL::Velocity,  0, 0, 0);
    ::alListener3i (AL::Direction, 0, 0, 0);
    ::alListeneriv (AL::Orientation, nOrient);
}

void Listener::setPosition (point3f const& gPos) noexcept
{
    ::alListenerfv (AL::Position, &gPos.x);
}

point3f Listener::position () noexcept
{
    point3f gValue;
    ::alGetListenerfv (AL::Position, &gValue.x);
    return gValue;
}

void Listener::setVelocity (point3f const& gVelocity) noexcept
{
    ::alListenerfv (AL::Velocity, &gVelocity.x);
}

point3f Listener::velocity () noexcept
{
    point3f gValue;
    ::alGetListenerfv (AL::Velocity, &gValue.x);
    return gValue;
}

void Listener::setOrientation (point3f const& gAt, point3f const& gUp) noexcept
{
    float fOrient[] { gAt.x, gAt.y, gAt.z, gUp.x, gUp.y, gUp.z };
    ::alListenerfv (AL::Orientation, &fOrient[0]);
}

point3f Listener::orientationAt () noexcept
{
    float fOrient[6];
    ::alGetListenerfv (AL::Orientation, &fOrient[0]);
    return { fOrient[0], fOrient[1], fOrient[2] };
}

point3f Listener::orientationUp () noexcept
{
    float fOrient[6];
    ::alGetListenerfv (AL::Orientation, &fOrient[0]);
    return { fOrient[3], fOrient[4], fOrient[5] };
}

void Listener::setVolume (float fValue) noexcept
{
    if (fValue <= .0f)
    {
        ::alListenerf (AL::Volume, fValue);
        get ().listenerIsMute.store (true);
    }
    else
    {
        if (!get ().listenerIsMute.load (std::memory_order_relaxed))
        {
            ::alListenerf (AL::Volume, fValue);
        }

        get ().listenerMutex.lock ();
        get ().listenerVolume = fValue;
        get ().listenerMutex.unlock ();
    }
}

float Listener::volume () noexcept
{
    std::lock_guard<std::mutex> gLock (get ().listenerMutex);
    return get ().listenerVolume;
}

bool Listener::isMute () noexcept
{
    return get ().listenerIsMute.load ();
}

void Listener::mute () noexcept
{
    if (get ().listenerIsMute.load (std::memory_order_acquire)) return;
    ::alListenerf (AL::Volume, .0f);
    get ().listenerIsMute.store (true, std::memory_order_release);
}

void Listener::unmute () noexcept
{
    if (!get ().listenerIsMute.load (std::memory_order_acquire)) return;
    get ().listenerMutex.lock ();
    ::alListenerf (AL::Volume, get ().listenerVolume);
    get ().listenerMutex.unlock ();
    get ().listenerIsMute.store (false, std::memory_order_release);
}

} } } // namespace Audio
