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

#include <cppual/string>
#include <cppual/resource>
#include <cppual/noncopyable>
#include <cppual/interface>

#include <memory>

namespace cppual::ui {

class   display_interface;
typedef std::shared_ptr<display_interface> shared_display;
typedef std::weak_ptr  <display_interface> weak_display  ;

// ====================================================

class SHARED_API display_interface : public dyn_unbound_interface
{
public:
    typedef display_interface self_type  ;
    typedef shared_display    pointer    ;
    typedef resource_handle   handle_type;
    typedef fstring           string_type;
    typedef fstring_view      string_view;

    virtual string_type name         () const = 0;
    virtual uint        screen_count () const = 0;
    virtual void        flush        ()       = 0;

    static  pointer primary            ();
    static  bool    has_valid_instance () noexcept;
    static  bool    primary            (string_view const& name);
    static  pointer connect            (string_view const& name);

    handle_type native () const noexcept { return _M_native; }
    handle_type legacy () const noexcept { return _M_legacy; }

    template <ptr U>
    constexpr U native () const noexcept
    { return _M_native.get<U> (); }

    template <ptr U>
    constexpr U legacy () const noexcept
    { return _M_legacy.get<U> (); }

protected:
    template <pair_like... Ps>
    constexpr display_interface (handle_type native, handle_type legacy, Ps... pairs) noexcept
    : base_type (std::make_pair ("name", make_fn (*this, &self_type::name)),
                 std::make_pair ("screen_count", make_fn (*this, &self_type::screen_count)),
                 std::make_pair ("flush", make_fn (*this, &self_type::flush)),
                 pairs...)
    , _M_native (native  )
    , _M_legacy (legacy  )
    { }

private:
    display_interface () = delete;

private:
    handle_type _M_native { }, _M_legacy { };
};

} // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_DISPLAY_H_
