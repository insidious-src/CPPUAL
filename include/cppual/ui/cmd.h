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

#ifndef CPPUAL_UI_ACTION_H_
#define CPPUAL_UI_ACTION_H_
#ifdef __cplusplus

#include <cppual/signal.h>
#include <cppual/gfx/drawable/image.h>
#include <cppual/string.h>

namespace cppual { namespace ui {

// ====================================================

class popup_menu;

// ====================================================

class SHARED_API command
{
public:
    typedef gfx::icon icon_type;

    inline icon_type* icon () const noexcept { return const_cast<icon_type*> (&_M_gIcon); }
    inline uint       id () const noexcept { return _M_uId; }
    inline uint       hotkey () const noexcept { return _M_uHotkey; }
    inline bool       has_menu () const noexcept { return _M_pMenu; }
    inline bool       has_side_menu () const noexcept { return _M_bSideMenu; }

    inline bool set_icon (string const& gPath) noexcept
    { return _M_gIcon.load (gPath); }

    inline void set_text (string const& gText) noexcept
    { _M_gText = gText; }

    inline void set_menu (popup_menu* pMenu) noexcept
    { if (pMenu) _M_pMenu = pMenu; }

    inline void set_menu_side_mode (bool bSide) noexcept
    { if (_M_pMenu) _M_bSideMenu = bSide; }

    inline string const& text () const noexcept
    { return _M_gText; }

    signal<void(bool)> triggered;

private:
    icon_type   _M_gIcon;
    string      _M_gText;
    uint        _M_uId, _M_uHotkey;
    popup_menu* _M_pMenu;
    bool        _M_bSideMenu;

};

// ====================================================

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_ACTION_H_
