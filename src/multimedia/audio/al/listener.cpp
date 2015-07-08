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

#include <mutex>
#include <atomic>
#include <cppual/noncopyable.h>
#include <cppual/multimedia/audio/al/listener.h>
#include "aldef.h"

using std::mutex;
using std::atomic_bool;
using std::lock_guard;

namespace cppual { namespace Audio { namespace AL {

namespace { // optimize for internal unit usage

struct Internal final : NonConstructible
{
    static mutex       listenerMutex;
    static atomic_bool listenerIsMute;
    static float       listenerVolume;
};

mutex       Internal::listenerMutex;
atomic_bool Internal::listenerIsMute { false };
float       Internal::listenerVolume = 1.0f;

} // anonymous

// ====================================================

void Listener::reset () noexcept
{
    int nOrient[6] { 0, 0, -1, 0, 1, 0 };

    alListener3i (AL::Position,  0, 0, 0);
    alListener3i (AL::Velocity,  0, 0, 0);
    alListener3i (AL::Direction, 0, 0, 0);
    alListeneriv (AL::Orientation, nOrient);
}

void Listener::setPosition (point3f const& gPos) noexcept
{
    alListenerfv (AL::Position, &gPos.x);
}

point3f Listener::position () noexcept
{
    point3f gValue;
    alGetListenerfv (AL::Position, &gValue.x);
    return gValue;
}

void Listener::setVelocity (point3f const& gVelocity) noexcept
{
    alListenerfv (AL::Velocity, &gVelocity.x);
}

point3f Listener::velocity () noexcept
{
    point3f gValue;
    alGetListenerfv (AL::Velocity, &gValue.x);
    return gValue;
}

void Listener::setOrientation (point3f const& gAt, point3f const& gUp) noexcept
{
    float fOrient[] { gAt.x, gAt.y, gAt.z, gUp.x, gUp.y, gUp.z };
    alListenerfv (AL::Orientation, &fOrient[0]);
}

point3f Listener::orientationAt () noexcept
{
    float fOrient[6];
    alGetListenerfv (AL::Orientation, &fOrient[0]);
    return { fOrient[0], fOrient[1], fOrient[2] };
}

point3f Listener::orientationUp () noexcept
{
    float fOrient[6];
    alGetListenerfv (AL::Orientation, &fOrient[0]);
    return { fOrient[3], fOrient[4], fOrient[5] };
}

void Listener::setVolume (float fValue) noexcept
{
    if (fValue <= .0f)
    {
        alListenerf (AL::Volume, fValue);
        Internal::listenerIsMute.store (true);
    }
    else
    {
        if (!Internal::listenerIsMute.load (std::memory_order_relaxed))
        {
            alListenerf (AL::Volume, fValue);
        }

        Internal::listenerMutex.lock ();
        Internal::listenerVolume = fValue;
        Internal::listenerMutex.unlock ();
    }
}

float Listener::volume () noexcept
{
    lock_guard<mutex> gLock (Internal::listenerMutex);
    return Internal::listenerVolume;
}

bool Listener::isMute () noexcept
{
    return Internal::listenerIsMute.load ();
}

void Listener::mute () noexcept
{
    if (Internal::listenerIsMute.load (std::memory_order_acquire)) return;
    alListenerf (AL::Volume, .0f);
    Internal::listenerIsMute.store (true, std::memory_order_release);
}

void Listener::unmute () noexcept
{
    if (!Internal::listenerIsMute.load (std::memory_order_acquire)) return;
    Internal::listenerMutex.lock ();
    alListenerf (AL::Volume, Internal::listenerVolume);
    Internal::listenerMutex.unlock ();
    Internal::listenerIsMute.store (false, std::memory_order_release);
}

} } } // namespace Audio
