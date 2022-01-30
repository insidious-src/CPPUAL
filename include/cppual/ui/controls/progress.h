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

#ifndef CPPUAL_UI_PROGRESS_H_
#define CPPUAL_UI_PROGRESS_H_

#include <cppual/signal.h>
#include <cppual/ui/skin.h>

namespace cppual { namespace ui {

// =========================================================

class progress : public skinnable_view
{
public:
    enum class progress_flag : byte
    {
        marquee    = 1 << 0,
        show_value = 1 << 1
    };

    typedef bitset<progress_flag> progress_flags;

    progress (view*     parent,
              rect const& rect,
              int         value =   0,
              int         min   =   0,
              int         max   = 100,
              int         step  =   1);

    void set_value (int);
    void set_step (int);
    void set_range (int min, int max);
    void set_marquee (bool);
    void show_value (bool);
    void increment ();
    void decrement ();

    inline int value     () const noexcept { return _M_nValue; }
    inline int min_value () const noexcept { return _M_nMin  ; }
    inline int max_value () const noexcept { return _M_nMax  ; }
    inline int step      () const noexcept { return _M_nStep ; }

    inline bool is_marquee () const noexcept
    { return _M_eProgressFlags.test (progress_flag::marquee); }

    inline bool is_value_visible () const noexcept
    { return _M_eProgressFlags.test (progress_flag::show_value); }

public:
    signal<void(int)> value_changed;

protected:
    virtual void paint_event (rect const&);
    virtual void size_event  (point2u);

private:
    int            _M_nValue, _M_nMin, _M_nMax, _M_nStep;
    progress_flags _M_eProgressFlags;
};

// =========================================================

} } // Ui

#endif // CPPUAL_UI_PROGRESS_H_
