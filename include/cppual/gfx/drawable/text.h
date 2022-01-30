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

#ifndef CPPUAL_GFX_DRAWABLE_TEXT_H_
#define CPPUAL_GFX_DRAWABLE_TEXT_H_
#ifdef __cplusplus

#include <cppual/gfx/draw.h>
#include <cppual/gfx/font.h>

namespace cppual { namespace gfx {

class label2d final : public drawable2d_interface, public transformable2d_interface
{
public:
    typedef font::styles styles     ;
    typedef string       string_type;

    label2d ();
    label2d (string_type const&, font&);
    label2d (label2d const&);
    label2d (label2d&&) noexcept;
    label2d& operator = (label2d const&);
    label2d& operator = (label2d&&) noexcept;
    void draw (transform2d const&);

    device_backend type     ()                    const noexcept { return device_backend::gl; }
    string_type    text     ()                    const noexcept { return _M_gText;           }
    styles         style    ()                    const noexcept { return _M_gStyle;          }
    void           set_style (styles gStyle)            noexcept { _M_gStyle = gStyle;        }
    void           set_text  (string_type const& gText) noexcept { _M_gText  = gText;         }

private:
    font*       _M_pFont ;
    string_type _M_gText ;
    styles      _M_gStyle;
};

// =========================================================

class label3d final : public drawable3d_interface, public transformable3d_interface
{
public:
    typedef font::styles styles     ;
    typedef string       string_type;

    label3d ();
    label3d (string_type const& text, font const& = font ());
    label3d (string_type&& text, font const& = font ()) noexcept;
    label3d (label3d const&);
    label3d (label3d&&) noexcept;
    label3d& operator = (label3d const&);
    label3d& operator = (label3d&&) noexcept;
    void draw (transform3d const&);

    device_backend type      ()                   const noexcept { return device_backend::gl; }
    string_type    text      ()                   const noexcept { return _M_gText;           }
    styles         style     ()                   const noexcept { return _M_gStyle;          }
    void           set_style (styles gStyle)            noexcept { _M_gStyle = gStyle;        }
    void           set_text  (string_type const& gText) noexcept { _M_gText  = gText;         }

private:
    font*       _M_pFont ;
    string_type _M_gText ;
    styles      _M_gStyle;
};

} } // Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_DRAWABLE_TEXT_H_
