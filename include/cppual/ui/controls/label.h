/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#ifndef CPPUAL_UI_TEXT_H_
#define CPPUAL_UI_TEXT_H_

#include <cppual/ui/skin.h>

namespace cppual { namespace Ui {

enum class TextFormat : unsigned char
{
    Plain,
    WordWrap,
    Enclosed
};

class Label : public SkinnableView
{
public:
    Label () { }
    ~Label () { }

    void setText (string const&);
    void setFormat (TextFormat);

    bool create (View*       parent,
                 string const& text,
                 Rect const&   rect,
                 TextFormat    format = TextFormat::Plain);

    inline TextFormat getFormat () const noexcept { return m_eFormat; }
    inline string     getText () const noexcept { return m_gText; }

    Signal<void()> signalTextChanged;

private:
    string     m_gText;
    TextFormat m_eFormat;

    void onPaint (Rect const&);
    void onEnable (bool);
    void onSize (point2u);
};

} } // Ui

#endif // CPPUAL_UI_TEXT_H_
