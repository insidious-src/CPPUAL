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

#ifndef CPPUAL_UI_TREE_H_
#define CPPUAL_UI_TREE_H_

#include <vector>
#include <cppual/ui/skin.h>

namespace cppual { namespace Ui {

class TreeView : public SkinnableView
{
protected:
    typedef struct NodeList_T
    { string strNodeName; std::vector<NodeList_T> gChildNodeList; }
    NodeList;

    byte                  m_nTreeType;
    std::vector<NodeList> m_gNodeList;

    virtual void onPaint ();
    virtual void onNodeClick ();

public:
    string& nodeName   (uint, uint);
    int     find       (const string&);
    bool    insertNode (const string&, uint);
    bool    remove     (uint);

    TreeView () : m_nTreeType (), m_gNodeList ()
    { }

    void setType (byte nTreeType)
    {
        m_nTreeType = nTreeType;
        refresh ();
    }
};

} } // Ui

#endif // CPPUAL_UI_TREE_H_
