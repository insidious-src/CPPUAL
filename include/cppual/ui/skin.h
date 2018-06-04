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

#ifndef CPPUAL_UI_SKIN_H_
#define CPPUAL_UI_SKIN_H_
#ifdef __cplusplus

#include <cppual/gfx/drawable/image.h>
#include <cppual/gfx/font.h>
#include <cppual/ui/view.h>

using std::string;
using cppual::Graphics::Color;

namespace cppual { namespace Ui {

class SkinnableView;

enum class SkinElement : char
{
    Toolbar,
    Menu,
    Button,
    Radio,
    CheckBox,
    GroupBox,
    Combo,
    List,
    Tree,
    Progress,
    TabView,
    Slider,
    ScrollBar,
    Tooltip,
    Switch,
    Roller,
    Separator,
    Window
};

enum class IconGroup : unsigned char
{
    Actions,
    Animations,
    Apps,
    Categories,
    Devices,
    Emblems,
    MIMETypes,
    Places,
    States
};

// Static - always stays the same; Dynamic - changes its properties on specific events
enum class SkinType : bool { Static, Dynamic };

// =========================================================

class ISkin : public NonCopyableVirtual
{
public:
    typedef Graphics::Font::Format font_format;
    typedef Graphics::Image        image_type ;
    typedef Graphics::Element      window_type;

    virtual image_type* getIcon (IconGroup     group,
                                 string const& icon_name,
                                 Rect const&   size) = 0;

    virtual image_type* getImage (SkinElement element,
                                  int         image_id) = 0;

    virtual font_format getFont  (SkinElement element, int font_id)  = 0;
    virtual Color       getColor (SkinElement element, int color_id) = 0;
    virtual int         getValue (SkinElement element, int value_id) = 0;
    virtual SkinType    getType  () const = 0;
    virtual bool        reload   () = 0;

    static ISkin* getDefault () noexcept;
    static bool   setDefault (ISkin* skin) noexcept;

};

// =========================================================

class SkinEngine final : public NonCopyable
{
public:
    SkinEngine (SkinnableView*) noexcept;
    inline bool isValid () const noexcept { return m_pView; }

private:
    SkinnableView* m_pView;
};

// =========================================================

class SkinnableView : public View
{
public:
    typedef View::event_type                                        event_type;
    //typedef typename Signal<void(event_type::PaintData)>::slot_type skin_conn;

    SkinnableView ();
    SkinnableView (SkinnableView&&);
    SkinnableView (SkinnableView const&);
    SkinnableView& operator = (SkinnableView&&);
    SkinnableView& operator = (SkinnableView const&);

    SkinnableView (View* pParent, Rect const& gRect, u32 uScreen = 0)
    : View (pParent, gRect, uScreen),
      m_gSkinEngine (this),
      m_pSkin ()
    { }

    void setSkin (ISkin*) noexcept;

    inline ISkin* getSkin () noexcept
    { return m_pSkin; }

protected:
    SkinEngine m_gSkinEngine;
    ISkin*     m_pSkin;

    virtual void onSkinChange () { }
    virtual bool onCreatePost () { return true; }

private:
    void onCreate   ();
    void changeSkin ();
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_SKIN_H_
