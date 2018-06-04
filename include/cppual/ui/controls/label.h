/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

#include <cppual/signal.h>
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
    typedef std::string string_type;

    Label  () { }
    ~Label () { }

    void setText   (string_type const&);
    void setFormat (TextFormat);

    Label (View*            parent,
           string_type const& text,
           Rect        const& rect,
           TextFormat       format = TextFormat::Plain);

    inline TextFormat getFormat () const noexcept { return m_eFormat; }
    inline string_type getText   () const noexcept { return m_gText  ; }

    Signal<void()> textChanged;

private:
    string_type m_gText;
    TextFormat  m_eFormat;

    void onPaint (Rect const&);
    void onEnable (bool);
    void onSize (point2u);
};

} } // Ui

#endif // CPPUAL_UI_TEXT_H_
