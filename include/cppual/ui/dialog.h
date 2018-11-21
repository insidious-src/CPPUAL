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

#ifndef CPPUAL_UI_DLG_H_
#define CPPUAL_UI_DLG_H_

#include <cppual/ui/window.h>

namespace cppual { namespace Ui {

class Dialog : public Window
{
public:
    Dialog () noexcept;
    Dialog (Dialog&&) noexcept;
    Dialog (Dialog const&) noexcept;
    Dialog& operator = (Dialog&&) noexcept;
    Dialog& operator = (Dialog const&) noexcept;

    Dialog (View*         parent,
            Rect const&   rect,
            string const& title,
            image_type*   icon   = nullptr,
            u32           screen = 0)
    : Window (parent, rect, title, icon, screen),
      m_pOwnerWnd ()
    { }

    enum Result
    {
        Close    = 0,
        OK       = 1 <<  0,
        Cancel   = 1 <<  1,
        Yes      = 1 <<  2,
        YesToAll = 1 <<  3,
        No       = 1 <<  4,
        NoToAll  = 1 <<  5,
        Discard  = 1 <<  6,
        Accept   = 1 <<  7,
        Decline  = 1 <<  8,
        Save     = 1 <<  9,
        SaveAll  = 1 << 10,
        Ignore   = 1 << 11
    };

    Result getResult () noexcept;
    void   showModal (Ui::Window*) noexcept;

protected:
    virtual void onInit () { }
    virtual void onEnd  () { }

private:
    Ui::Window* m_pOwnerWnd;

    void onCreate  ();
    void destroyEvent ();
};

} } // namespace Ui

#endif // CPPUAL_UI_DLG_H_
