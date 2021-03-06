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

#ifndef CPPUAL_UI_CTL_BUTTON_H_
#define CPPUAL_UI_CTL_BUTTON_H_
#ifdef __cplusplus

#include <cppual/ui/skin.h>
#include <cppual/string.h>

namespace cppual { namespace Ui {

class AbstractButton : public SkinnableView
{
public:
    void setLabel (string const&);
    ~AbstractButton () noexcept;

    inline string getLabel () const noexcept
    { return m_gLabel; }

protected:
    Graphics::Font m_gFont;
    string         m_gLabel;
};

// =========================================================

class PushButton : public AbstractButton
{
public:
    PushButton (View* parent, string const& text);
    void press ();
    void release ();

    inline bool isPushed () const noexcept
    { return m_bIsPushed; }

private:
    bool m_bIsPushed;

    void paintEvent (Rect const&);
    void sizeEvent (point2u);
    void focusEvent (bool);
};

// =========================================================

class RadioBox : public AbstractButton
{
public:
    RadioBox (View* parent, string const& text, bool selected = false);
    void select ();
    void clear ();

    inline bool isSelected () const noexcept
    { return m_bIsSelected; }

private:
    bool m_bIsSelected;

    void paintEvent (Rect const&);
    void sizeEvent (point2u);
    void focusEvent (bool);
};

// =========================================================

enum class CheckState : unsigned char
{
    Unchecked,
    Checked,
    Intermediate
};

class CheckBox : public AbstractButton
{
public:
    CheckBox (View*         parent,
              string const& text  ,
              CheckState    state = CheckState::Unchecked);

    void setCheckState (CheckState);

    inline CheckState getCheckState () const noexcept
    { return m_eCheckState; }

private:
    CheckState m_eCheckState;

    void paintEvent (Rect const&);
    void sizeEvent  (point2u);
    void focusEvent (bool);
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_CTL_BUTTON_H_
