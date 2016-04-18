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

#ifndef CPPUAL_UI_PROGRESS_H_
#define CPPUAL_UI_PROGRESS_H_

#include <cppual/ui/skin.h>

namespace cppual { namespace Ui {

class Progress : public SkinnableView
{
public:
    void setValue (int);
    void setStep (int);
    void setRange (int min, int max);
    void setMarquee (bool);
    void showValue (bool);
    void increment ();
    void decrement ();

    bool create (View*     parent,
                 Rect const& rect,
                 int         value =   0,
                 int         min   =   0,
                 int         max   = 100,
                 int         step  =   1);

    enum ProgressFlag
    {
        Marquee   = 1 << 0,
        ShowValue = 1 << 1
    };

    typedef FlagSet<ProgressFlag, u8> ProgressFlags;

    inline int getValue () const noexcept { return m_nValue; }
    inline int getMinValue () const noexcept { return m_nMin; }
    inline int getMaxValue () const noexcept { return m_nMax; }
    inline int getStep () const noexcept { return m_nStep; }

    inline bool isMarquee () const noexcept
    { return m_eProgressFlags.hasFlag (Progress::Marquee); }

    inline bool isValueVisible () const noexcept
    { return m_eProgressFlags.hasFlag (Progress::ShowValue); }

    Signal<void(int)> signalValueChanged;

private:
    int           m_nValue, m_nMin, m_nMax, m_nStep;
    ProgressFlags m_eProgressFlags;

    void onPaint ();
    void onSize (Rect const&);
};

} } // Ui

#endif // CPPUAL_UI_PROGRESS_H_
