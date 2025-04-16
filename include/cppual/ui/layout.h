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

#ifndef CPPUAL_UI_LAYOUT_H_
#define CPPUAL_UI_LAYOUT_H_
#ifdef __cplusplus

#include <cppual/ui/view.h>

namespace cppual { namespace ui {

struct layout_metrics
{ uint row, column, rowspan, colspan; };

struct align
{
    enum side
    {
        left   = 1 << 0,
        top    = 1 << 1,
        right  = 1 << 2,
        bottom = 1 << 3
    };
};

// =========================================================

class SHARED_API layout
{
public:
    typedef dyn_array<view*> vector_type;

    layout ();
    layout (layout&&);
    layout (layout const&);
    layout& operator = (layout&&);
    layout& operator = (layout const&);

    bool add_view (view*, layout_metrics const&);
    bool remove_view (view*);
    void set_view_metrics (view*, layout_metrics const&);
    void set_padding (rect const&);

private:
    void on_size (rect const&);

private:
    vector_type _M_gViewList;
    view*       _M_pOwner;
    uint        _M_uNumRows, _M_uNumColumns;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_LAYOUT_H_
