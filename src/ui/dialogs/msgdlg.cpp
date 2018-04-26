/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
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

#include <cppual/ui/dialogs/msgdlg.h>
#include <cppual/ui/controls/label.h>
#include <cppual/ui/controls/button.h>

using namespace cppual::Graphics;

namespace cppual { namespace Ui {

class MessageBox final : public Dialog
{
    //Label m_gMessage;

    void onInit ();
    void onEnd  ();

    using Dialog::Dialog;
};

Dialog::Result showQuestion (string&& gTitle,
                             string&& /*gQuestion*/,
                             Results  /*gAnswers*/,
                             Window*  pOwner)
{
    MessageBox gQuestionBox (nullptr, Rect (), std::forward<string> (gTitle), nullptr);

    if (gQuestionBox.valid ())
    {
        gQuestionBox.showInTaskbar (false);
        gQuestionBox.showModal (pOwner);
        return gQuestionBox.getResult ();
    }

    return Dialog::Close;
}

void showMessage (string&&    gTitle,
                  string&&    /*gMessage*/,
                  MessageType /*eType*/,
                  Window*     pOwner)
{
    MessageBox gMessageBox (nullptr, Rect (), std::forward<string> (gTitle), nullptr);

    if (gMessageBox.valid ())
    {
        gMessageBox.showInTaskbar (false);
        gMessageBox.showModal (pOwner);
    }
}

void MessageBox::onInit ()
{
//    m_gMessage.create (this,
//                       "",
//                       Rect (100, 100,
//                             static_cast<u16> (geometry ().width  () - 20u),
//                             static_cast<u16> (geometry ().height () - 20u))
//                       );
}

void MessageBox::onEnd ()
{
}

} } // namespace Ui
