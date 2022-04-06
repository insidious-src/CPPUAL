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

#include <cppual/ui/frame.h>
#include <cppual/ui/composite.h>

#include <functional>

namespace cppual { namespace ui {

frame_view::frame_view () noexcept
: _M_gCloseBtn (this, ""),
  _M_gMaxBtn   (this, ""),
  _M_gMinBtn   (this, ""),
  _M_gHelpBtn  (this, ""),
  _M_gSysMenu  (),
  _M_gTitle    (),
  _M_gIconRect (),
  _M_gFrameBtnsRect (),
  _M_pIcon          (),
  _M_pTarget        (),
  _M_gFrameFlags    (),
  _M_gMouseStates   (),
  _M_gHoldingEdges  (),
  _M_eVisibility    ()
{
}

frame_view::frame_view (frame_view&& gObj) noexcept
: skinnable_view (gObj),
  _M_gCloseBtn (this, ""),
  _M_gMaxBtn   (this, ""),
  _M_gMinBtn   (this, ""),
  _M_gHelpBtn  (this, "")
{
}

frame_view::frame_view (frame_view const& gObj) noexcept
: skinnable_view (gObj),
  _M_gCloseBtn (this, ""),
  _M_gMaxBtn   (this, ""),
  _M_gMinBtn   (this, ""),
  _M_gHelpBtn  (this, "")
{
}

frame_view& frame_view::operator = (frame_view&& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

frame_view& frame_view::operator = (frame_view const& gObj) noexcept
{
    if (this == &gObj) return *this;
    return *this;
}

frame_view::~frame_view () noexcept
{
}

void frame_view::stretch ()
{
    renderable ().lock ().get ()->set_maximized (true);
}

void frame_view::unstretch ()
{
    renderable ().lock ().get ()->set_maximized (false);
}

void frame_view::attach (view* pView)
{
    if (!pView || !pView->valid ()) return;

    if (!is_using_internal_compositor ())
    {
        if (_M_pTarget && _M_pTarget->valid ())
        {
            _M_pTarget->renderable_unsafe ()->set_flags
                    (_M_pTarget->renderable_unsafe ()->flags () -= window_flag::Frame);
        }

        pView->renderable_unsafe ()->set_flags
                (pView->renderable_unsafe ()->flags () += window_flag::Frame);

    }

    _M_pTarget = pView;
}

void frame_view::detach ()
{
    if (!_M_pTarget) return;

    if (!is_using_internal_compositor ())
    {
        _M_pTarget->renderable_unsafe ()->set_flags
                (_M_pTarget->renderable_unsafe ()->flags () -= Frame);
    }
    else
    {
    }

    _M_pTarget = nullptr;
}

void frame_view::set_icon (image_type*)
{
}

void frame_view::set_label (string_type const&)
{
}

void frame_view::set_flags (window_flags)
{
}

frame_view::frame_view (view*         pParent,
                        rect const&   gTargetRect,
                        string_type&& gLabel,
                        image_type*   pIcon,
                        window_flags  gFlags)
: skinnable_view (pParent, calc_frame_size (gTargetRect)),
  _M_gCloseBtn (this, ""),
  _M_gMaxBtn   (this, ""),
  _M_gMinBtn   (this, ""),
  _M_gHelpBtn  (this, ""),
  _M_gSysMenu  (),
  _M_gTitle    (gLabel),
  _M_gIconRect (),
  _M_gFrameBtnsRect (),
  _M_pIcon          (pIcon),
  _M_pTarget        (),
  _M_gFrameFlags    (gFlags),
  _M_gMouseStates   (),
  _M_gHoldingEdges  (),
  _M_eVisibility    ()
{
    if (valid ())
    {
        if (!gFlags.test (Maximize)) _M_gMaxBtn.hide();
        if (!gFlags.test (Help)) _M_gHelpBtn.hide();
        if (pIcon) set_icon (pIcon);
        _M_gTitle = std::forward<string_type> (gLabel);

        if (_M_gSysMenu.create ({ _M_gIconRect.left, _M_gIconRect.top }))
        {
            _M_gSysMenu.add_command (nullptr);
            _M_gSysMenu.add_command (nullptr);
            _M_gSysMenu.add_separator ();
            _M_gSysMenu.add_sub_menu (nullptr, "Compositor");
            _M_gSysMenu.add_separator ();
            _M_gSysMenu.add_command (nullptr);
        }
    }
}

void frame_view::show_event (bool bVisible)
{
    if (!bVisible) _M_gSysMenu.hide ();
}

void frame_view::paint_event (rect const&)
{
}

void frame_view::size_event (point2u)
{
}

void frame_view::on_mouse_left_down (point2i gPos)
{
    _M_gMouseStates += frame_view::mouse_left_down;

    if (_M_gFrameBtnsRect.contains (gPos))
    {
        // frame buttons
    }
    else if (_M_gIconRect.contains (gPos))
    {
        _M_gSysMenu.set_position (point2i { _M_gIconRect.left,
                                          _M_gIconRect.bottom });
        _M_gSysMenu.show ();
    }
    else
    {
        begin_size_move_event (geometry ());
    }
}

void frame_view::on_mouse_left_up (point2i)
{
    _M_gMouseStates -= frame_view::mouse_left_down;
}

void frame_view::on_mouse_right_down (point2i gPos)
{
    _M_gMouseStates += frame_view::mouse_right_down;

    if (geometry ().contains (gPos)  and
        !_M_gIconRect.contains (gPos) and
        !_M_gFrameBtnsRect.contains (gPos))
    {
        _M_gSysMenu.set_position (gPos);
        _M_gSysMenu.show ();
    }
}

void frame_view::on_mouse_right_up (point2i)
{
    _M_gMouseStates -= frame_view::mouse_right_down;
}

void frame_view::on_mouse_move (point2i)
{
    if (!_M_gMouseStates.test (frame_view::mouse_left_down)) return;

    switch (_M_gHoldingEdges)
    {
    case frame_view::left:
        break;
    case frame_view::top:
        break;
    case frame_view::right:
        break;
    case frame_view::bottom:
        break;
    case frame_view::top_left:
        break;
    case frame_view::top_right:
        break;
    case frame_view::bottom_left:
        break;
    case frame_view::bottom_right:
        break;
    }
}

void frame_view::begin_size_move_event (rect const&)
{
}

void frame_view::end_size_move_event (rect const&)
{
}

rect frame_view::calc_frame_size (rect const& gRect) noexcept
{
    return gRect;
}

} } // namespace Ui
