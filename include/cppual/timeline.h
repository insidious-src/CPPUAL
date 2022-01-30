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

#ifndef CPPUAL_UTILITY_TIMELINE_H_
#define CPPUAL_UTILITY_TIMELINE_H_
#ifdef __cplusplus

#include <atomic>
#include <cppual/types.h>
#include <cppual/system/clock.h>

namespace cppual {

using namespace std::chrono_literals;

class pasive_timeline
{
public:
    typedef clock::pausable_timer<std::chrono::steady_clock> timer_type  ;
    typedef std::chrono::milliseconds::rep                   rep         ;
    typedef std::chrono::milliseconds                        duration    ;
    typedef std::atomic<float>                               atomic_ratio;
    typedef std::atomic_uint_fast8_t                         atomic_state;
    typedef ushort                                           count_type  ;
    typedef float                                            ratio_type  ;
    typedef u8                                               state_type  ;

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

    pasive_timeline (duration length, ratio_type speed = Normal) noexcept
    : _M_timer      (),
      _M_length     (length),
      _M_state      (),
      _M_speed      (speed),
      _M_prev_speed (speed)
    { }

    duration length () const noexcept
    { return _M_length; }

    void length (duration len) noexcept
    { if (!active ()) _M_length = len; }

    rep frame () const noexcept
    { return time ().count () + (length ().count () / 60); }

    bool active () const noexcept
    { return state () == pasive_timeline::Active; }

    void reverse () noexcept
    { _M_speed = -(_M_speed.load (std::memory_order_relaxed)); }

    void forward () noexcept
    { if (_M_speed < ratio_type()) _M_speed = -(_M_speed.load (std::memory_order_relaxed)); }

    void backward () noexcept
    { if (_M_speed > ratio_type()) _M_speed = -(_M_speed.load (std::memory_order_relaxed)); }

    void stop () noexcept
    { _M_state = pasive_timeline::Inactive; }

    ratio_type speed () const noexcept
    {
        return _M_speed.load (std::memory_order_relaxed);
    }

    void scale (ratio_type speed) noexcept
    {
        _M_prev_speed.store(_M_speed.load(std::memory_order_relaxed));
        _M_speed = speed;
    }

    void start (count_type loop_count = 1) noexcept
    {
        stop ();
        _M_count = loop_count;
        _M_timer.reset ();
        _M_state = pasive_timeline::Active;
    }

    state_type state () const noexcept
    {
        auto curstate = _M_state.load (std::memory_order_relaxed);

        switch (curstate)
        {
        case pasive_timeline::Active:
            if (!_M_count or (normalize_elapsed () / _M_length.count ()) < _M_count)
                return pasive_timeline::Active;

            _M_state = pasive_timeline::Inactive;
            return pasive_timeline::Inactive;
        default:
            return curstate;
        }
    }

    void resume () noexcept
    {
        if (_M_state.load (std::memory_order_relaxed) != pasive_timeline::Paused) return;
        _M_timer.resume ();
        _M_state = pasive_timeline::Active;
    }

    void pause () noexcept
    {
        if (_M_state.load (std::memory_order_relaxed) != pasive_timeline::Active) return;
        _M_timer.pause ();

        rep elapsed = normalize_elapsed ();
        elapsed     = elapsed - ((elapsed / _M_length.count ()) * _M_length.count ());

        if (elapsed > _M_length.count ()) stop ();
        else
        {
            _M_saved = duration (elapsed);
            _M_state = pasive_timeline::Paused;
        }
    }

    duration time () const noexcept
    {
        switch (_M_state.load (std::memory_order_relaxed))
        {
        case pasive_timeline::Inactive:
            return duration ();
        case pasive_timeline::Paused:
            return _M_saved;
        default:
            rep elapsed    = normalize_elapsed ();
            rep play_count = elapsed / _M_length.count ();

            // get current position
            return !_M_count or play_count < _M_count ?
                        duration (elapsed - (play_count * _M_length.count ())) :
                        duration ();
        }
    }

private:
    rep normalize_elapsed () const noexcept
    {
        auto       current_speed = _M_prev_speed.load (std::memory_order_relaxed);
        auto const max_speed     = _M_speed.load (std::memory_order_relaxed);

        if      (current_speed < max_speed) current_speed += Slowest;
        else if (current_speed > max_speed) current_speed -= Slowest;
        else     current_speed = max_speed;

        return rep(ratio_type (_M_timer.elapsed<duration> ().count ()) * current_speed);
    }

private:
    timer_type           _M_timer;
    duration             _M_length, _M_saved;
    mutable atomic_state _M_state;
    count_type           _M_count;
    atomic_ratio         _M_speed;
    atomic_ratio         _M_prev_speed;
};

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_UTILITY_TIMELINE_H_
