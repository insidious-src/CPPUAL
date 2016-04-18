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

#ifndef CPPUAL_UI_CONTAINER_H_
#define CPPUAL_UI_CONTAINER_H_

#include <cppual/ui/layout.h>

namespace cppual { namespace Ui {

class CContainer : public View
{
protected:
    uint m_uBorderSize;

public:
    void SetBorderSize (uint);
    void SetPadding (uint);
    bool Create (View*, const Rect&, uint = 0U);

    uint GetBorderSize () const { return m_uBorderSize; }
    uint GetPadding () const { return 0U; }
};

class CAdvTable : public CContainer
{
protected:
    uint m_uNumColumns, m_uNumRows;
    Layout m_cTableLayout;

public:
    void SetNumColumns (uint);
    void SetNumRows (uint);
    void SetItemRect (const Rect&);
    bool AddView (View*, uint, uint, uint = 1U, uint = 1U);
    bool RemoveView (View*);
    bool Create (View*, const Rect&, uint, uint, uint = 0U);

    uint GetNumColumns () const { return m_uNumColumns; }
    uint GetNumRows () const { return m_uNumRows; }
};

} } // Ui

#endif // CPPUAL_UI_CONTAINER_H_
