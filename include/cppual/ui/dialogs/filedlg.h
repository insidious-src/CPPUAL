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

#ifndef CPPUAL_UI_DIALOG_FILEDLG_H_
#define CPPUAL_UI_DIALOG_FILEDLG_H_

#include <cppual/ui/dialog.h>

namespace cppual { namespace ui {

class browser_dialog final : public dialog
{
public:
    enum flag
    {
        directory    = 1 << 0,
        multi_select = 1 << 1,
    };

    void set_directory_dialog (bool);
    void set_multi_select     (bool);
};

} } // namespace Ui

#endif // CPPUAL_UI_DIALOG_FILEDLG_H_
