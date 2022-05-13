/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_PLATFORM_X_DISPLAY_H_
#define CPPUAL_PLATFORM_X_DISPLAY_H_
#ifdef __cplusplus

#include <cppual/ui/display.h>

#if defined (OS_GNU_LINUX) or defined (OS_BSD)

#include <cppual/ui/platforms/xcb/xcb_def.h>
#include <cppual/circular_queue.h>

namespace cppual { namespace ui {

class SHARED_API xcb_display final : public display_interface
{
public:
    typedef x::display_data&             data_reference      ;
    typedef x::display_data const&       data_const_reference;
    typedef circular_queue<x::atom_type> atoms_container     ;
    typedef atoms_container&             atoms_refrence      ;
    typedef atoms_container const&       atoms_const_refrence;

    xcb_display () = delete;
    xcb_display (string_type const& name);
    ~xcb_display ();

    uint screen_count () const noexcept;
    void flush        () noexcept;

    string_type name () const { return _M_gName; }

    constexpr data_const_reference data () const noexcept
    { return _M_data; }

    constexpr atoms_refrence prev_atoms () noexcept
    { return _M_prevAtoms; }

    constexpr atoms_const_refrence prev_atoms () const noexcept
    { return _M_prevAtoms; }

private:
    string_type     _M_gName    ;
    x::display_data _M_data     ;
    atoms_container _M_prevAtoms;
};

} } // namespace Ui

#endif // OS_GNU_LINUX or OS_BSD
#endif // __cplusplus
#endif // CPPUAL_PLATFORM_X_DISPLAY_H_
