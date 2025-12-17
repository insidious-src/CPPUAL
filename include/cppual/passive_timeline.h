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

#ifndef CPPUAL_UTILITY_TIMELINE_H_
#define CPPUAL_UTILITY_TIMELINE_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/system/clock.h>

#include <atomic>

namespace cppual {

using namespace std::chrono_literals;

class pasive_timeline
{
public:
    typedef std::chrono::steady_clock         clock_type;
    typedef clock::pausable_timer<clock_type> timer_type;
    typedef std::chrono::milliseconds::rep    rep       ;
    typedef std::chrono::milliseconds         duration  ;
    typedef u16                               count_type;
    typedef float                             ratio_type;

    constexpr static cfloat instant        = 10.0f;
    constexpr static cfloat very_fast      =  5.0f;
    constexpr static cfloat fast           =  2.0f;
    constexpr static cfloat normal         =  1.0f;
    constexpr static cfloat slow           =  0.8f;
    constexpr static cfloat very_slow      =  0.5f;
    constexpr static cfloat extremely_slow =  0.3f;
    constexpr static cfloat slowest        =  0.1f;

    enum
    {
        infinite = 0,
    };

    typedef enum state_type
    {
        inactive,
        active  ,
        paused
    }
    const const_state;

    typedef std::atomic<ratio_type> atomic_ratio;
    typedef std::atomic<state_type> atomic_state;

    pasive_timeline (duration length, ratio_type speed = normal) noexcept
    : _M_length     (length),
      _M_speed      (speed)
    { }

    duration length () const noexcept
    { return _M_length; }

    void set_length (duration len) noexcept
    { if (!is_active ()) _M_length = len; }

    rep frame () const noexcept
    { return time ().count () + (length ().count () / 60); }

    bool is_active () const noexcept
    { return state () == pasive_timeline::active; }

    void reverse () noexcept
    { scale (-(_M_speed.load (std::memory_order_relaxed))); }

    void set_forward () noexcept
    { if (_M_speed < ratio_type ()) scale (-(_M_speed.load (std::memory_order_relaxed))); }

    void set_backward () noexcept
    { if (_M_speed > ratio_type ()) scale (-(_M_speed.load (std::memory_order_relaxed))); }

    void stop () noexcept
    { _M_state = pasive_timeline::inactive; }

    ratio_type speed () const noexcept
    {
        return _M_speed.load (std::memory_order_relaxed);
    }

    void scale (ratio_type speed) noexcept
    {
        _M_speed = speed;
    }

    void start (count_type loop_count = 1) noexcept
    {
        stop ();
        _M_count = loop_count;
        _M_timer.reset ();
        _M_accumulated_time = duration ();
        _M_last_speed_change_time = clock_type::now ();
        _M_state = pasive_timeline::active;
    }

    state_type state () const noexcept
    {
        auto const curstate = _M_state.load (std::memory_order_relaxed);

        switch (curstate)
        {
        case pasive_timeline::active:
            if (!_M_count or (normalize_elapsed () / _M_length.count ()) < _M_count)
                return pasive_timeline::active;

            _M_state = pasive_timeline::inactive;
            return pasive_timeline::inactive;
        default:
            return curstate;
        }
    }

    void resume () noexcept
    {
        if (_M_state.load (std::memory_order_relaxed) != pasive_timeline::paused) return;
        _M_timer.resume ();
        _M_state = pasive_timeline::active;
    }

    void pause () noexcept
    {
        if (_M_state.load (std::memory_order_relaxed) != pasive_timeline::active) return;
        _M_timer.pause ();

        rep elapsed = normalize_elapsed ();
        elapsed     = elapsed - ((elapsed / _M_length.count ()) * _M_length.count ());

        if (elapsed > _M_length.count ()) stop ();
        else
        {
            _M_saved = duration (elapsed);
            _M_state = pasive_timeline::paused;
        }
    }

    // duration time () const noexcept
    // {
    //     switch (_M_state.load (std::memory_order_relaxed))
    //     {
    //     case pasive_timeline::inactive:
    //         return duration ();
    //     case pasive_timeline::paused:
    //         return _M_saved;
    //     default:
    //         rep elapsed    = normalize_elapsed ();
    //         rep play_count = elapsed / _M_length.count ();

    //         // get current position
    //         return !_M_count or play_count < _M_count ?
    //                    duration (elapsed - (play_count * _M_length.count ())) :
    //                    duration ();
    //     }
    // }

    duration time () const noexcept
    {
        switch (_M_state.load (std::memory_order_relaxed))
        {
        case pasive_timeline::inactive:
            return duration ();
        case pasive_timeline::paused:
            return _M_saved;
        default:
            auto now            = std::chrono::steady_clock::now ();
            auto current_speed  = _M_speed.load (std::memory_order_relaxed);
            auto recent_elapsed = std::chrono::duration_cast<duration> (now - _M_last_speed_change_time);
            auto total_elapsed  = _M_accumulated_time + duration(rep(ratio_type(recent_elapsed.count()) *
                                                                                current_speed));
            rep play_count      = total_elapsed.count () / _M_length.count ();
            return (!_M_count || play_count < _M_count) ?
                       duration (total_elapsed.count () - (play_count * _M_length.count ())) :
                       duration ();
        }
    }

private:
    rep normalize_elapsed () const noexcept
    {
        auto const current_speed = _M_speed.load (std::memory_order_relaxed);

        return rep(ratio_type (_M_timer.elapsed<duration> ().count ()) * current_speed);
    }

private:
    timer_type             _M_timer                  { };
    duration               _M_length { }, _M_saved   { };
    duration               _M_accumulated_time       { };
    clock_type::time_point _M_last_speed_change_time { };
    atomic_ratio           _M_speed                  { };
    count_type             _M_count                  { };
    mutable atomic_state   _M_state                  { };
};

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_UTILITY_TIMELINE_H_
