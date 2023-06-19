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

#ifndef CPPUAL_UI_FRAME_H_
#define CPPUAL_UI_FRAME_H_
#ifdef __cplusplus

#include <cppual/ui/controls/popup.h>
#include <cppual/ui/composite.h>
#include <cppual/string.h>

namespace cppual { namespace ui {

enum class vis_type : u8
{
    hidden,
    normal,
    minimized,
    maximized
};

class SHARED_API frame_view : public skinnable_view
{
public:
    typedef gfx::image_interface image_type;

    enum edge
    {
        left         = 1 << 0,
        top          = 1 << 1,
        right        = 1 << 2,
        bottom       = 1 << 3,
        top_left     = left  | top,
        top_right    = right | top,
        bottom_left  = left  | bottom,
        bottom_right = right | bottom
    };

    enum mouse_state
    {
        mouse_left_down  = 1 << 0,
        mouse_right_down = 1 << 1,
    };

    typedef bitset<frame_view::mouse_state> mouse_states;
    typedef bitset<frame_view::edge>        edges       ;

    frame_view () noexcept;
    frame_view (frame_view&&) noexcept;
    frame_view (frame_view const&) noexcept;
    frame_view& operator = (frame_view&&) noexcept;
    frame_view& operator = (frame_view const&) noexcept;
    ~frame_view () noexcept;

    void stretch ();
    void unstretch ();
    void attach (view*);
    void detach ();
    void set_icon (image_type*);
    void set_label (string_type const&);
    void set_flags (window_flags);

    frame_view (view*         parent,
                rect const&   target_rect,
                string_type&& title,
                image_type*   icon,
                window_flags  flags = WindowHints);

    inline image_type*  icon () const noexcept { return _M_pIcon; }
    inline view*        attached () const noexcept { return _M_pTarget; }
    inline popup_menu*  menu () noexcept { return &_M_gSysMenu; }
    inline window_flags flags () const noexcept { return _M_gFrameFlags; }
    inline vis_type     visibility () const noexcept { return _M_eVisibility; }

    inline bool is_stretched () const noexcept
    { return _M_eVisibility == vis_type::maximized; }

protected:
    void begin_size_move_event (rect const&);
    void end_size_move_event (rect const&);

private:
    rect calc_frame_size (rect const& client_rect) noexcept;
    void show_event (bool);
    void paint_event (rect const&);
    void size_event (point2u);
    void on_mouse_left_down (point2i);
    void on_mouse_left_up (point2i);
    void on_mouse_right_down (point2i);
    void on_mouse_right_up (point2i);
    void on_mouse_move (point2i);

private:
    push_button  _M_gCloseBtn;
    push_button  _M_gMaxBtn;
    push_button  _M_gMinBtn;
    push_button  _M_gHelpBtn;
    popup_menu   _M_gSysMenu;
    string_type  _M_gTitle;
    rect         _M_gIconRect;
    rect         _M_gFrameBtnsRect;
    image_type*  _M_pIcon;
    view*        _M_pTarget;
    window_flags _M_gFrameFlags;
    mouse_states _M_gMouseStates;
    edges        _M_gHoldingEdges;
    vis_type     _M_eVisibility;
};

// =========================================================

// control window frame eighter using internal compositor
// or the window manager
class SHARED_API Frame
{
public:
    typedef frame_view::image_type image_type;

    void stretch ();
    void unstretch ();
    void attach (view*);
    void detach ();
    void setIcon (image_type*);
    void setLabel (string const&);

    image_type* icon () const noexcept;
    view*       attached () const noexcept;
    popup_menu*  menu () noexcept;
    vis_type  visibility () const noexcept;
    bool        isStretched () const noexcept;

private:
    frame_view* _M_pInternalFrame;
    bool       _M_bIsUsingInternalCompositor;
};

} } // namespace Ui

#endif // __cplusplus
#endif // CPPUAL_UI_FRAME_H_
