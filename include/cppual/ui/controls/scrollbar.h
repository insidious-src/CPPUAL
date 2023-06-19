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

#ifndef CPPUAL_UI_SCROLLBAR_H_
#define CPPUAL_UI_SCROLLBAR_H_

#include <cppual/ui/controls/abslider.h>
#include <cppual/common.h>
#include <cppual/signal.h>

namespace cppual { namespace ui {

class scrollbar : public abstract_slider
{
public:
    typedef orientation orientation_type;

    enum class arrows : u8
    {
        none,
        double_sided,
        double_sided_extra
    };

    scrollbar (view*, orientation = orientation::vertical);

    void set_arrow_style (arrows);
    void set_range (int min, int max);
    void set_position (int);
    void set_orientation (orientation_type);

    inline orientation_type orientation () const noexcept
    { return _M_eOrientation; }

    inline arrows arrow_style () const noexcept
    { return _M_eArrows; }

public:
    signal<void(int)> position_changed;

protected:
    virtual void paint_event (rect const&);
    virtual void size_event  (point2u);
    virtual void on_enable   (bool);

private:
    arrows           _M_eArrows     ;
    orientation_type _M_eOrientation;
};

} } // Ui

#endif // CPPUAL_UI_SCROLLBAR_H_
