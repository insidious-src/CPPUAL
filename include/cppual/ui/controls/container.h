/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#ifndef CPPUAL_UI_CONTAINER_H_
#define CPPUAL_UI_CONTAINER_H_

#include <cppual/ui/skin.h>
#include <cppual/ui/layout.h>

namespace cppual { namespace ui {

// =========================================================

class container_view : public skinnable_view
{
protected:
    uint _M_uBorderSize;

public:
    void setBorderSize (uint);
    void setPadding (uint);
    container_view (view*, const rect&, uint = 0U);

    uint borderSize () const { return _M_uBorderSize; }
    uint padding () const { return 0U; }
};

// =========================================================

class adv_table : public container_view
{
public:
    adv_table (view*, rect const&, uint, uint, uint = 0U);

    void set_num_columns (uint);
    void set_num_rows (uint);
    void set_item_rect (rect const&);
    bool add_view (view*, uint, uint, uint = 1U, uint = 1U);
    bool remove_view (view*);

    uint num_columns () const noexcept { return _M_uNumColumns; }
    uint num_rows    () const noexcept { return _M_uNumRows   ; }

protected:
    uint   _M_uNumColumns, _M_uNumRows;
    layout _M_gTableLayout;
};

// =========================================================

} } // Ui

#endif // CPPUAL_UI_CONTAINER_H_
