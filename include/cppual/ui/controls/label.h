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

#ifndef CPPUAL_UI_TEXT_H_
#define CPPUAL_UI_TEXT_H_

#include <cppual/signal.h>
#include <cppual/ui/skin.h>

namespace cppual { namespace ui {

// =========================================================

enum class text_format : u8
{
    plain,
    word_wrap,
    enclosed
};

// =========================================================

class label : public skinnable_view
{
public:
    label  () { }
    ~label () { }

    void set_text   (string_type const&);
    void set_format (text_format);

    label (view*            parent,
           string_type const& text,
           rect        const& rect,
           text_format       format = text_format::plain);

    inline text_format format () const noexcept { return _M_eFormat; }
    inline string_type text   () const noexcept { return _M_gText  ; }

public:
    signal<void()> text_changed;

protected:
    virtual void paint_event (rect const&);
    virtual void size_event (point2u);

    virtual void on_enable (bool);

private:
    string_type _M_gText  ;
    text_format _M_eFormat;
};

// =========================================================

} } // Ui

#endif // CPPUAL_UI_TEXT_H_
