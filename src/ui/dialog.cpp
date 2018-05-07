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

#include <cppual/ui/dialog.h>

namespace cppual { namespace Ui {

Dialog& Dialog::operator = (Dialog&& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

Dialog& Dialog::operator = (Dialog const& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

Dialog::Result Dialog::getResult() noexcept
{
    return Dialog::Close;
}

void Dialog::showModal (Ui::Window* pOwner) noexcept
{
    if (valid ())
    {
        if (pOwner and pOwner->valid ())
        {
            m_pOwnerWnd = pOwner;
            pOwner->disable ();
        }

        show ();
        setFocus ();
    }
}

void Dialog::onCreate ()
{
    onInit ();
}

void Dialog::onDestroy ()
{
    onEnd ();

    if (m_pOwnerWnd)
    {
        m_pOwnerWnd->enable ();
        m_pOwnerWnd->setFocus ();
        m_pOwnerWnd = nullptr;
    }
}

} } // namespace Ui
