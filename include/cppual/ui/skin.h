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

#ifndef CPPUAL_UI_SKIN_H_
#define CPPUAL_UI_SKIN_H_
#ifdef __cplusplus

#include <cppual/gfx/drawable/image.h>
#include <cppual/gfx/font.h>
#include <cppual/ui/view.h>

namespace cppual { namespace ui {

// =========================================================

class skinnable_view;

// =========================================================

enum class skin_element : u8
{
    toolbar,
    menu,
    button,
    radio,
    check_box,
    group_box,
    combo,
    list,
    tree,
    progress,
    tab_view,
    slider,
    scrollbar,
    tooltip,
    switcher,
    roller,
    separator,
    window
};

// =========================================================

enum class icon_group : u8
{
    action,
    alert,
    av,
    common,
    communication,
    content,
    animation,
    apps,
    categories,
    device,
    emblem,
    editor,
    file,
    hardware,
    image,
    maps,
    mime_types,
    navigation,
    notification,
    places,
    social,
    states,
    toggle
};

// =========================================================

/// statically - always stays the same; dynamically - changes its properties on specific events
enum class skin_loading : u8 { statically, dynamically };

// =========================================================

class SHARED_API skin_interface : public non_copyable_virtual
{
public:
    typedef gfx::font::format_type font_format;
    typedef gfx::image_interface   image_type ;
    typedef gfx::color             color_type ;
    typedef resource_handle        window_type;
    typedef string                 string_type;

    virtual image_type* icon (icon_group         group,
                              string_type const& icon_name,
                              rect const&        size) = 0;

    virtual image_type*  image (skin_element element, int image_id) = 0;
    virtual font_format  font  (skin_element element, int font_id ) = 0;
    virtual color_type   color (skin_element element, int color_id) = 0;
    virtual int          value (skin_element element, int value_id) = 0;
    virtual skin_loading loading_type () const = 0;
    virtual bool         reload () = 0;

    static skin_interface* get_default () noexcept;
    static bool            set_default (skin_interface* skin) noexcept;
};

// =========================================================

class SHARED_API skinnable_view : public view
{
public:
    typedef view::event_type event_type;

    skinnable_view ();
    skinnable_view (skinnable_view&&);
    skinnable_view (skinnable_view const&);
    skinnable_view& operator = (skinnable_view&&);
    skinnable_view& operator = (skinnable_view const&);

    skinnable_view (view* pParent, rect const& gRect, u32 uScreen = 0)
    : view (pParent, gRect, uScreen),
      _M_pSkin ()
    { }

    void set_skin (skin_interface*) noexcept;

    inline skin_interface* skin () noexcept
    { return _M_pSkin; }

protected:
    skin_interface* _M_pSkin;

    virtual void on_skin_change () { }

private:
    void change_skin ();
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_SKIN_H_
