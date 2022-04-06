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

#include <cppual/ui/dialogs/msgdlg.h>
#include <cppual/ui/controls/label.h>
#include <cppual/ui/controls/button.h>

namespace cppual { namespace ui {

class message_box final : public dialog
{
    //Label _M_gMessage;

    void on_init ();
    void on_end  ();

    using dialog::dialog;
};

dialog::result show_question (string&& gTitle,
                              string&& /*gQuestion*/,
                              results  /*gAnswers*/,
                              window*  pOwner)
{
    message_box gQuestionBox (nullptr, rect (), std::forward<string> (gTitle), nullptr);

    if (gQuestionBox.valid ())
    {
        gQuestionBox.show_in_taskbar (false);
        gQuestionBox.show_modal (pOwner);
        return gQuestionBox.get_result ();
    }

    return dialog::result::close;
}

void show_message (string&&    gTitle,
                   string&&    /*gMessage*/,
                   message_type /*eType*/,
                   window*     pOwner)
{
    message_box gMessageBox (nullptr, rect (), std::forward<string> (gTitle), nullptr);

    if (gMessageBox.valid ())
    {
        gMessageBox.show_in_taskbar (false);
        gMessageBox.show_modal (pOwner);
    }
}

void message_box::on_init ()
{
//    _M_gMessage.create (this,
//                       "",
//                       Rect (100, 100,
//                             static_cast<u16> (geometry ().width  () - 20u),
//                             static_cast<u16> (geometry ().height () - 20u))
//                       );
}

void message_box::on_end ()
{
}

} } // namespace Ui
