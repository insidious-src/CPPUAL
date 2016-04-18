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

#ifndef CPPUAL_UI_DIALOG_MESSAGEDLG_H_
#define CPPUAL_UI_DIALOG_MESSAGEDLG_H_

#include <cppual/ui/dialog.h>

namespace cppual { namespace Ui {

enum class MessageType : char { Plain, Information, Warning, Error };
typedef    BitSet<Dialog::Result> Results;

Dialog::Result showQuestion (string&& title,
                             string&& question,
                             Results  answers,
                             Window*  owner = nullptr);

void showMessage (string&&    title,
                  string&&    message,
                  MessageType message_type = MessageType::Warning,
                  Window*     owner        = nullptr);

} } // namespace Ui

#endif // CPPUAL_UI_DIALOG_MESSAGEDLG_H_
