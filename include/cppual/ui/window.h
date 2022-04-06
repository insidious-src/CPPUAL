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

#ifndef CPPUAL_UI_WINDOW_H_
#define CPPUAL_UI_WINDOW_H_
#ifdef __cplusplus

#include <cppual/ui/frame.h>
#include <cppual/ui/view.h>
#include <cppual/gfx/drawable/image.h>
#include <cppual/input/event.h>

namespace cppual { namespace ui {

class window : public view
{
public:
    typedef gfx::image_interface image_type;

    window (view*                 parent,
            rect        const&    rect,
            string_type const&    title,
            image_type*           icon   = nullptr,
            u32                   screen = 0,
            allocator_type const& ator   = allocator_type ());

    window ();
    window (window const&);
    window& operator = (window const&);
    ~window ();

    void show_minimized ();
    void show_maximized ();
    bool set_icon (image_type*);
    void set_title (string_type const&);
    void set_fullscreen (bool);
    void flash (uint count);
    void show_in_taskbar (bool);
    bool is_visible_in_taskbar () const;
    void show_in_pager (bool);
    bool is_visible_in_pager () const;
    void set_shaded (bool);
    bool is_shaded () const;
    void keep_above (bool);
    bool is_above () const;
    void keep_below (bool);
    bool is_below () const;
    void restore ();
    void minimize ();
    void maximize ();
    void close ();

    string_type title () const;

    inline frame_view* frame () const noexcept { return _M_gFrame; }
    inline image_type* icon  () const noexcept { return _M_pIcon ; }

    inline bool is_fullscreen () const noexcept
    { return renderable_unsafe()->is_fullscreen (); }

    bool is_minimized () const noexcept;

    bool is_maximized () const noexcept;

protected:
    virtual bool on_close    () { return true; }
    virtual void on_minimize () { }
    virtual void on_maximize () { }
    virtual void on_help     () { }

private:
    frame_view* _M_gFrame;
    image_type* _M_pIcon ;
};

} } // namespace Ui
#endif // __cplusplus
#endif // CPPUAL_UI_WINDOW_H_
