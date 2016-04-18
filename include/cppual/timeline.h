/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_UTILITY_TIMELINE_H_
#define CPPUAL_UTILITY_TIMELINE_H_
#ifdef __cplusplus

#include <atomic>
#include <cppual/types.h>
#include <cppual/system/clock.h>

using namespace std::chrono_literals;

namespace cppual {

class Timeline
{
public:
    typedef Clock::PausableTimer<std::chrono::steady_clock> timer_type;
    typedef std::chrono::milliseconds::rep                  rep;
    typedef std::chrono::milliseconds                       duration;
    typedef std::atomic<float>                              atomic_ratio;
    typedef std::atomic_uint_fast8_t                        atomic_state;
    typedef ushort                                          count_type;
    typedef float                                           ratio_type;
    typedef u8                                              state_type;

    constexpr static cfloat Instant       = 10.0f;
    constexpr static cfloat VeryFast      =  5.0f;
    constexpr static cfloat Fast          =  2.0f;
    constexpr static cfloat Normal        =  1.0f;
    constexpr static cfloat Slow          =  0.8f;
    constexpr static cfloat VerySlow      =  0.5f;
    constexpr static cfloat ExtremelySlow =  0.3f;
    constexpr static cfloat Slowest       =  0.1f;

    enum
    {
        Infinite = 0,
    };

    enum
    {
        Inactive,
        Active,
        Paused
    };

    Timeline (duration length, ratio_type speed = Timeline::Normal) noexcept
    : m_timer  (),
      m_length (length),
      m_state  (),
      m_speed  (speed)
    { }

    duration length () const noexcept
    { return m_length; }

    void length (duration len) noexcept
    { if (!active ()) m_length = len; }

    rep frame () const noexcept
    { return time ().count () + (length ().count () / 60); }

    bool active () const noexcept
    { return state () == Timeline::Active; }

    void reverse () noexcept
    { m_speed = -(m_speed.load (std::memory_order_relaxed)); }

    void forward () noexcept
    { if (m_speed < 0) m_speed = -(m_speed.load (std::memory_order_relaxed)); }

    void backward () noexcept
    { if (m_speed > 0) m_speed = -(m_speed.load (std::memory_order_relaxed)); }

    void stop () noexcept
    { m_state = Timeline::Inactive; }

    ratio_type speed () const noexcept
    {
        auto   curspeed = m_speed.load (std::memory_order_relaxed);
        return curspeed < 0 ? -(curspeed) : curspeed;
    }

    void scale (ratio_type speed) noexcept
    {
        if (speed == 0) stop ();
        m_speed = speed;
    }

    void start (count_type loop_count = 1) noexcept
    {
        stop ();
        m_count = loop_count;
        m_timer.reset ();
        m_state = Timeline::Active;
    }

    state_type state () const noexcept
    {
        auto curstate = m_state.load (std::memory_order_relaxed);

        switch (curstate)
        {
        case Timeline::Active:
            if (!m_count or (normalize_elapsed () / m_length.count ()) < m_count)
                return Timeline::Active;

            m_state = Timeline::Inactive;
            return Timeline::Inactive;
        default:
            return curstate;
        }
    }

    void resume () noexcept
    {
        if (m_state.load (std::memory_order_relaxed) != Timeline::Paused) return;
        m_timer.resume ();
        m_state = Timeline::Active;
    }

    void pause () noexcept
    {
        if (m_state.load (std::memory_order_relaxed) != Timeline::Active) return;
        m_timer.pause ();

        rep elapsed = normalize_elapsed ();
        elapsed     = elapsed - ((elapsed / m_length.count ()) * m_length.count ());

        if (elapsed > m_length.count ()) stop ();
        else
        {
            m_saved = duration (elapsed);
            m_state = Timeline::Paused;
        }
    }

    duration time () const noexcept
    {
        switch (m_state.load (std::memory_order_relaxed))
        {
        case Timeline::Inactive:
            return duration ();
        case Timeline::Paused:
            return m_saved;
        default:
            rep elapsed    = normalize_elapsed ();
            rep play_count = elapsed / m_length.count ();

            // get current position
            return !m_count or play_count < m_count ?
                        duration (elapsed - (play_count * m_length.count ())):
                        duration ();
        }
    }

private:
    rep normalize_elapsed () const noexcept
    {
        return rep (ratio_type (m_timer.elapsed<duration> ().count ()) *
                    m_speed.load (std::memory_order_relaxed));
    }

private:
    timer_type           m_timer;
    duration             m_length, m_saved;
    mutable atomic_state m_state;
    count_type           m_count;
    atomic_ratio         m_speed;
};

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_UTILITY_TIMELINE_H_
