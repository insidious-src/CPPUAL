/*
 * Product: C++ Unified Abstraction Library
 * Author: fymfifa
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

#ifndef CPPUAL_GFX_DISPLAY_H_
#define CPPUAL_GFX_DISPLAY_H_
#ifdef __cplusplus

#include <memory>
#include <cppual/noncopyable.h>
#include <cppual/gfx/dsp_details.h>
#include <string>

namespace cppual { namespace Ui {

class   IDisplay;
typedef std::shared_ptr<IDisplay> shared_display;
typedef std::weak_ptr  <IDisplay> weak_display  ;

// ====================================================

class IDisplay : public NonCopyableVirtual
{
public:
    typedef Graphics::Connection handle_type;
    typedef std::string          string_type;
    typedef shared_display       pointer    ;

    constexpr IDisplay () noexcept = default;
    ~IDisplay ();

    virtual string_type name             () const = 0;
    virtual uint        screenCount      () const = 0;
    virtual void        flush            ()       = 0;

    static  IDisplay*   primary          ();
    static  bool        hasValidInstance () noexcept;
    static  bool        primary          (string_type const&);
    static  pointer     connect          (string_type const&);

    handle_type native () const noexcept { return m_native; }
    handle_type legacy () const noexcept { return m_legacy; }

    template <typename U>
    constexpr typename std::remove_pointer<U>::type* native () const noexcept
    { return m_native.get<U> (); }

    template <typename U>
    constexpr typename std::remove_pointer<U>::type* legacy () const noexcept
    { return m_legacy.get<U> (); }

protected:
    constexpr IDisplay (handle_type native, handle_type legacy) noexcept
    : m_native (native), m_legacy (legacy)
    { }

private:
    handle_type m_native { }, m_legacy { };
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_GFX_DISPLAY_H_
