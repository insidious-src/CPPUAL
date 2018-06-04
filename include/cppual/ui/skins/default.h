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

#ifndef CPPUAL_UI_SKINS_DEFAULT_H_
#define CPPUAL_UI_SKINS_DEFAULT_H_
#ifdef __cplusplus

#include <cppual/ui/skin.h>

using std::string;
using cppual::Graphics::Font;

namespace cppual { namespace Ui {

class DefaultSkin final : public ISkin
{
public:
    SkinType getType () const noexcept { return sm_eType; }

    bool reload ()
    {
        return false;
    }

    int getValue (const SkinElement, int)
    {
        return 0;
    }

    RGBColor getColor (const SkinElement, int)
    {
        return RGBColor ();
    }

    image_type* getImage (const SkinElement, int)
    {
        return nullptr;
    }

    image_type* getIcon (IconGroup, string const&, Rect const&)
    {
        return nullptr;
    }

    font_format getFont (const SkinElement, int)
    {
        return font_format ();
    }

private:
    static const SkinType sm_eType = SkinType::Dynamic;
};

} } // Ui

#endif // __cplusplus
#endif // CPPUAL_UI_SKINS_DEFAULT_H_
