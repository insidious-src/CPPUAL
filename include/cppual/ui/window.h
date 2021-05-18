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

#ifndef CPPUAL_UI_WINDOW_H_
#define CPPUAL_UI_WINDOW_H_
#ifdef __cplusplus

#include <chrono>
#include <cppual/gfx/drawable/image.h>
#include <cppual/ui/frame.h>
#include <cppual/ui/view.h>
#include <cppual/input/event.h>

namespace cppual { namespace Ui {

class Window : public View
{
public:
    typedef Graphics::Image image_type ;
    typedef string     string_type;

    Window ();
    Window (Window const&);
    Window& operator = (Window const&);
    ~Window ();

    bool setIcon (image_type*);
    void setTitle (string_type const&);
    void setFullscreen (bool);
    void flash (ushort);
    void showInTaskbar (bool);
    void restore ();
    void minimize ();
    void maximize ();
    void close ();

    Window (View*              parent,
            Rect   const&      rect,
            string_type const& title,
            image_type*        icon   = nullptr,
            u32                screen = 0);

    string_type title () const;

    inline FrameView*  frame () noexcept { return m_gFrame; }
    inline image_type* icon  () const noexcept { return m_pIcon; }

    inline bool isFullscreen () const noexcept
    { return renderable_unsafe()->isFullscreen(); }

    inline bool isMinimized () const noexcept
    { return m_gFrame->attached () == this and m_gFrame->isHidden (); }

    inline bool isMaximized () const noexcept
    { return m_gFrame->attached () == this and m_gFrame->isStretched (); }

protected:
    virtual bool onClose () { return true; }
    virtual void onMinimize () { }
    virtual void onMaximize () { }
    virtual void onHelp () { }

private:
    FrameView*  m_gFrame;
    image_type* m_pIcon;
};

} } // namespace Ui
#endif // __cplusplus
#endif // CPPUAL_UI_WINDOW_H_
