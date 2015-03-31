/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#ifndef CPPUAL_UI_ACTION_H_
#define CPPUAL_UI_ACTION_H_
#ifdef __cplusplus

#include <string>
#include <cppual/signal.h>
#include <cppual/gfx/drawable/image.h>

using std::string;
using cppual::Graphics::Icon;

namespace cppual { namespace Ui {

class PopupMenu;

class Command
{
public:
    inline Icon* icon () const noexcept { return const_cast<Icon*> (&m_gIcon); }
    inline uint  id () const noexcept { return m_uId; }
    inline uint  hotkey () const noexcept { return m_uHotkey; }
    inline bool  hasMenu () const noexcept { return m_pMenu; }
    inline bool  hasSideMenu () const noexcept { return m_bSideMenu; }

    inline bool setIcon (string const& gPath) noexcept
    { return m_gIcon.load (gPath); }

    inline void setText (string const& gText) noexcept
    { m_gText = gText; }

    inline void setMenu (PopupMenu* pMenu) noexcept
    { if (pMenu) m_pMenu = pMenu; }

    inline void setMenuSideMode (bool bSide) noexcept
    { if (m_pMenu) m_bSideMenu = bSide; }

    inline string const& text () const noexcept
    { return m_gText; }

    Signal<void(bool)> triggered;

private:
    Icon       m_gIcon;
    string     m_gText;
    uint       m_uId, m_uHotkey;
    PopupMenu* m_pMenu;
    bool       m_bSideMenu;

};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_ACTION_H_
