/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2016 insidious
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

#ifndef CPPUAL_GFX_DRAWABLE_TEXT_H_
#define CPPUAL_GFX_DRAWABLE_TEXT_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>
#include <cppual/gfx/font.h>

namespace cppual { namespace Graphics {

class Label2D final : public IDrawable2D, public ITransformable2D
{
public:
    using Styles = Font::Styles;

    Label2D ();
    Label2D (string  const&, Font&);
    Label2D (Label2D const&);
    Label2D (Label2D&&) noexcept;
    Label2D& operator = (Label2D const&);
    Label2D& operator = (Label2D&&) noexcept;
    void draw (Transform2D const&);

    DeviceType type     ()              const noexcept { return DeviceType::GL; }
    string     text     ()              const noexcept { return m_gText;        }
    Styles     style    ()              const noexcept { return m_gStyle;       }
    void       setStyle (Styles gStyle)       noexcept { m_gStyle = gStyle;     }
    void       setText  (string const& gText) noexcept { m_gText  = gText;      }

private:
    Font*  m_pFont;
    string m_gText;
    Styles m_gStyle;
};

// =========================================================

class Label3D final : public IDrawable3D, public ITransformable3D
{
public:
    using Styles = Font::Styles;

    Label3D ();
    Label3D (string const&, Font const& = Font ());
    Label3D (string&&, Font const& = Font ()) noexcept;
    Label3D (Label3D const&);
    Label3D (Label3D&&) noexcept;
    Label3D& operator = (Label3D const&);
    Label3D& operator = (Label3D&&) noexcept;
    void draw (Transform3D const&);

    DeviceType type     ()              const noexcept { return DeviceType::GL; }
    string     text     ()              const noexcept { return m_gText;        }
    Styles     style    ()              const noexcept { return m_gStyle;       }
    void       setStyle (Styles gStyle)       noexcept { m_gStyle = gStyle;     }
    void       setText  (string const& gText) noexcept { m_gText  = gText;      }

private:
    Font*  m_pFont;
    string m_gText;
    Styles m_gStyle;
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_TEXT_H_
