/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#include <vector>
#include <cppual/ui/view.h>

using std::vector;

namespace cppual { namespace Ui {

struct LayoutMetrics
{ uint row, column, rowspan, colspan; };

struct Align
{
	enum Side
	{
		Left   = 1 << 0,
		Top    = 1 << 1,
		Right  = 1 << 2,
		Bottom = 1 << 3
	};
};

// =========================================================

class Layout
{
public:
	Layout ();
	Layout (Layout&&);
	Layout (Layout const&);
	Layout& operator = (Layout&&);
	Layout& operator = (Layout const&);

	bool addView (View*, LayoutMetrics const&);
	bool removeView (View*);
	void setViewMetrics (View*, LayoutMetrics const&);
	void setPadding (Rect const&);

private:
	vector<View*> m_gViewList;
	View*         m_pOwner;
	uint          m_uNumRows, m_uNumColumns;

	void onSize (Rect const&);
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_LAYOUT_H_
