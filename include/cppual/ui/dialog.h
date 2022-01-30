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

#ifndef CPPUAL_UI_DLG_H_
#define CPPUAL_UI_DLG_H_

#include <cppual/ui/window.h>

namespace cppual { namespace ui {

class dialog : public window
{
public:
    dialog () noexcept;
    dialog (dialog&&) noexcept;
    dialog (dialog const&) noexcept;
    dialog& operator = (dialog&&) noexcept;
    dialog& operator = (dialog const&) noexcept;

    dialog (view*              parent,
            rect const&        rect,
            string_type const& title,
            image_type*        icon   = nullptr,
            u32                screen = 0)
    : window (parent, rect, title, icon, screen),
      _M_pOwnerWnd ()
    { }

    enum class result : u16
    {
        close      =       0,
        ok         = 1 <<  0,
        cancel     = 1 <<  1,
        yes        = 1 <<  2,
        yes_to_all = 1 <<  3,
        no         = 1 <<  4,
        no_to_all  = 1 <<  5,
        discard    = 1 <<  6,
        accept     = 1 <<  7,
        decline    = 1 <<  8,
        save       = 1 <<  9,
        save_all   = 1 << 10,
        ignore     = 1 << 11
    };

    result get_result () noexcept;
    void   show_modal (window*) noexcept;

protected:
    virtual void on_init () { }
    virtual void on_end  () { }

private:
    void on_create ();
    void destroy_event ();

private:
    window* _M_pOwnerWnd;
};

} } // namespace Ui

#endif // CPPUAL_UI_DLG_H_
