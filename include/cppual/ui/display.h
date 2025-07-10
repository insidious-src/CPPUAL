/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
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

#ifndef CPPUAL_GFX_DISPLAY_H_
#define CPPUAL_GFX_DISPLAY_H_
#ifdef __cplusplus

#include <cppual/string.h>
#include <cppual/resource.h>
#include <cppual/noncopyable.h>
#include <cppual/unbound_interface.h>

#include <memory>

namespace cppual::ui {

class   display_interface;
typedef std::shared_ptr<display_interface> shared_display;
typedef std::weak_ptr  <display_interface> weak_display  ;

// ====================================================

class SHARED_API display_interface : public dyn_unbound_interface
{
public:
    typedef resource_connection handle_type;
    typedef string              string_type;
    typedef shared_display      pointer    ;

    constexpr display_interface () noexcept = default;

    virtual string_type name             () const = 0;
    virtual uint        screen_count     () const = 0;
    virtual void        flush            ()       = 0;

    static  pointer primary            ();
    static  bool    has_valid_instance () noexcept;
    static  bool    primary            (string_type const& name);
    static  pointer connect            (string_type const& name);

    handle_type native () const noexcept { return _M_native; }
    handle_type legacy () const noexcept { return _M_legacy; }

    template <typename U>
    constexpr typename std::remove_pointer<U>::type* native () const noexcept
    { return _M_native.get<U> (); }

    template <typename U>
    constexpr typename std::remove_pointer<U>::type* legacy () const noexcept
    { return _M_legacy.get<U> (); }

protected:
    constexpr display_interface (handle_type native, handle_type legacy) noexcept
    : base_type ()
    , _M_native (native)
    , _M_legacy (legacy)
    { }

private:
    handle_type _M_native { }, _M_legacy { };
};

} // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_DISPLAY_H_
