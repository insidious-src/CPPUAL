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

#ifndef CPPUAL_SYSTEM_CLOCK_H_
#define CPPUAL_SYSTEM_CLOCK_H_
#ifdef __cplusplus

#include <chrono>

namespace cppual { namespace Clock {

template <class Clock = std::chrono::steady_clock>
class Timer
{
public:
    typedef typename Clock::time_point time_point;
    typedef Clock                      clock_type;

    Timer      () noexcept : m_epoch  (clock_type::now ()) { }
    void reset () noexcept { m_epoch = clock_type::now (); }

    template <typename T = std::chrono::milliseconds>
    T elapsed () const noexcept
    { return std::chrono::duration_cast<T> (clock_type::now () - m_epoch); }

private:
    time_point m_epoch;
};

// =========================================================

template <class Clock = std::chrono::steady_clock>
class PausableTimer
{
public:
    typedef typename Clock::time_point time_point;
    typedef Clock                      clock_type;

    PausableTimer () noexcept
    : m_epoch  (clock_type::now ()), m_pause ()
    { }

    void reset () noexcept
    {
        m_pause = time_point ();
        m_epoch = clock_type::now ();
    }

    template <typename T = std::chrono::milliseconds>
    T elapsed () const noexcept
    {
        return std::chrono::duration_cast<T>
                (m_pause.time_since_epoch ().count () ? m_pause.time_since_epoch () :
                                                        clock_type::now () - m_epoch);
    }

    void pause () noexcept
    {
        if (m_pause.time_since_epoch ().count ()) return;
        m_pause = time_point (clock_type::now () - m_epoch);
    }

    void resume () noexcept
    {
        if (!m_pause.time_since_epoch ().count ()) return;
        m_epoch += clock_type::now () - (m_epoch + m_pause.time_since_epoch ());
        m_pause  = time_point ();
    }

private:
    time_point m_epoch, m_pause;
};

} } // namespace Clock

#endif // __cplusplus
#endif // CPPUAL_SYSTEM_CLOCK_H_
