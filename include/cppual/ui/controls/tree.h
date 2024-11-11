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

#ifndef CPPUAL_UI_TREE_H_
#define CPPUAL_UI_TREE_H_

#include <vector>
#include <cppual/ui/skin.h>

namespace cppual { namespace ui {

// =========================================================

class tree_view : public skinnable_view
{
public:
    string_type& node_name   (uint, uint);
    int          find        (string_type const&);
    bool         insert_node (string_type const&, uint);
    bool         remove      (uint);

    tree_view () : _M_nTreeType (), _M_gNodeList ()
    { }

    void set_type (byte nTreeType)
    {
        _M_nTreeType = nTreeType;
        refresh ();
    }

protected:
    struct node_list
    {
        string_type       name    ;
        vector<node_list> children;
    };

    virtual void paint_event (rect const&);
    virtual void on_node_click ();

private:
    byte              _M_nTreeType;
    vector<node_list> _M_gNodeList;
};

// =========================================================

} } // Ui

#endif // CPPUAL_UI_TREE_H_
