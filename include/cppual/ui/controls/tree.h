/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
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

#ifndef CPPUAL_UI_TREE_H_
#define CPPUAL_UI_TREE_H_

#include <cppual/vector.h>
#include <cppual/string/dstring.h>
#include <cppual/ui/widget.h>

namespace cppual { namespace Ui {

class CTreeView : public View
{
protected:
    typedef struct SNodeList { string strNodeName; CVector<SNodeList> vChildNodeList; } NodeList;
    byte m_nTreeType;
    CVector<NodeList> m_vNodeList;

    virtual void OnPaint ();
    virtual void OnNodeClick ();

public:
    string& GetNodeName (uint, uint);
    int Find (const string&);
    bool InsertNode (const string&, uint);
    bool Remove (uint);

    CTreeView () : m_nTreeType (), m_vNodeList () { }

    void SetType (byte nTreeType)
    {
        m_nTreeType = nTreeType;
        Update ();
    }
};

} } // Ui

#endif // CPPUAL_UI_TREE_H_
