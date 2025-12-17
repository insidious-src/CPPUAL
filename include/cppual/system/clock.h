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

#ifndef CPPUAL_SYSTEM_CLOCK_H_
#define CPPUAL_SYSTEM_CLOCK_H_
#ifdef __cplusplus

#include <cppual/meta_type>

#include <chrono>

namespace cppual::clock {

template <typename Clock = std::chrono::steady_clock,
          typename       = base_from_derived<Clock,
                                             std::chrono::steady_clock,
                                             std::chrono::system_clock,
                                             std::chrono::high_resolution_clock
                                             >
          >
class timer
{
public:
    typedef timer<Clock>               self_type ;
    typedef typename Clock::time_point time_point;
    typedef Clock                      clock_type;

    timer      () noexcept : _M_epoch  (clock_type::now ()) { }
    void reset () noexcept { _M_epoch = clock_type::now (); }

    template <typename T = std::chrono::milliseconds>
    T elapsed () const noexcept
    { return std::chrono::duration_cast<T> (clock_type::now () - _M_epoch); }

private:
    time_point _M_epoch;
};

// =========================================================

template <typename Clock = std::chrono::steady_clock,
          typename       = base_from_derived<Clock,
                                             std::chrono::steady_clock,
                                             std::chrono::system_clock,
                                             std::chrono::high_resolution_clock
                                             >
          >
class pausable_timer
{
public:
    typedef pausable_timer<Clock>      self_type ;
    typedef typename Clock::time_point time_point;
    typedef Clock                      clock_type;

    pausable_timer () noexcept
    : _M_epoch (clock_type::now ()), _M_pause ()
    { }

    void reset () noexcept
    {
        _M_epoch = clock_type::now ();
        _M_pause = time_point ();
    }

    template <typename T = std::chrono::milliseconds>
    T elapsed () const noexcept
    {
        return std::chrono::duration_cast<T>
                (_M_pause.time_since_epoch ().count () ? _M_pause.time_since_epoch () :
                                                        clock_type::now () - _M_epoch);
    }

    bool is_paused () const noexcept
    {
        return _M_pause.time_since_epoch ().count ();
    }

    void pause () noexcept
    {
        if (_M_pause.time_since_epoch ().count ()) return;
        _M_pause = time_point (clock_type::now () - _M_epoch);
    }

    void resume () noexcept
    {
        if (!_M_pause.time_since_epoch ().count ()) return;
        _M_epoch += clock_type::now () - (_M_epoch + _M_pause.time_since_epoch ());
        _M_pause  = time_point ();
    }

private:
    time_point _M_epoch, _M_pause;
};

} //! namespace Clock

#endif // __cplusplus
#endif // CPPUAL_SYSTEM_CLOCK_H_
