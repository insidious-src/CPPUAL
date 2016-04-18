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

#ifndef CPPUAL_UI_SCROLLBAR_H_
#define CPPUAL_UI_SCROLLBAR_H_

#include <cppual/ui/controls/abslider.h>
#include <cppual/common.h>

namespace cppual { namespace Ui {

class ScrollBar : public AbstractSlider
{
public:
    enum class Arrows : unsigned char
    {
        NoArrows,
        DoubleSided,
        DoubleSidedExtra
    };

    bool create (View*, Orientation = Orientation::Vertical);
    void setArrowStyle (Arrows);
    void setRange (int min, int max);
    void setPosition (int);
    void setOrientation (Orientation);

    inline Orientation getOrientation () const noexcept
    { return m_eOrientation; }

    inline Arrows getArrowStyle () const noexcept
    { return m_eArrows; }

    Signal<void(int)> signalHorizChanged;
    Signal<void(int)> signalVertChanged;

private:
    Arrows      m_eArrows;
    Orientation m_eOrientation;

    void onPaint ();
    void onEnable (bool);
    void onSize (Rect const&);
};

} } // Ui

#endif // CPPUAL_UI_SCROLLBAR_H_
