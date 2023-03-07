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
#include "w32window.h"

#ifdef   OS_WINDOWS
#define  WIN32_MEAN_AND_LEAN 1
#include <windows.h>

namespace cppual { namespace ui {

namespace {

typedef ::WNDCLASSEXA window_class_type;
typedef ::HINSTANCE   instance_type;
typedef ::HWND        id_type;
typedef ::HBRUSH      brush_type;
typedef ::RECT        rect_type;

struct WindowClass final : public window_class_type
{
    WindowClass ()
    {
        cbSize         = sizeof (WindowClass);
        style          = CS_HREDRAW | CS_VREDRAW;
        lpfnWndProc    = ::DefWindowProcA;
        cbClsExtra     = 0;
        cbWndExtra     = 0;
        hInstance      = ::GetModuleHandleA (nullptr);
        hIcon          = nullptr;
        hCursor        = ::LoadCursorA (NULL, IDC_ARROW);
        hbrBackground  = reinterpret_cast<brush_type> (COLOR_WINDOW + 1);
        lpszMenuName   = nullptr;
        lpszClassName  = "CPPUALWINDOW";
        hIconSm        = nullptr;

        if (::RegisterClassExA (this)) return;
        lpszClassName = nullptr;
        throw std::logic_error ("failed to register window class");
    }

    ~WindowClass () noexcept
    {
        if (lpszClassName) ::UnregisterClassA (lpszClassName, hInstance);
    }

    instance_type instance () const noexcept
    {
        return hInstance;
    }

    cchar* name () const noexcept
    {
        return lpszClassName;
    }

    static WindowClass& registered ()
    {
        static WindowClass window_class;
        return window_class;
    }
};

struct Win32Rect final : public rect_type
{
    Win32Rect () noexcept = default;

    constexpr Win32Rect (std::nullptr_t) noexcept
    : rect_type ()
    { }

    constexpr uint width () const noexcept
    { return right - left; }

    constexpr i16 left_unsafe () const noexcept
    { return static_cast<i16> (left); }

    constexpr i16 top_unsafe () const noexcept
    { return static_cast<i16> (top); }

    constexpr u16 width_unsafe () const noexcept
    { return static_cast<u16> (right - left); }

    constexpr uint height () const noexcept
    { return bottom - top; }

    constexpr u16 height_unsafe () const noexcept
    { return static_cast<u16> (bottom - top); }
};

inline long_t toWindowStyles (window_flags /*flags*/) noexcept
{
    long_t styles;

    return styles;
}

inline long_t toExStyles (window_flags /*flags*/) noexcept
{
    long_t styles;

    return styles;
}

} // anonymous namespace

// ====================================================

win32_window::win32_window (rect const& gRect, u32, shared_display pDisplay)
: platform_wnd_interface (pDisplay,
           ::CreateWindowExA (0, WindowClass::registered ().name (), nullptr, WS_OVERLAPPEDWINDOW,
                              gRect.left, gRect.top, gRect.width (), gRect.height (),
                              0, 0, WindowClass::registered ().instance (), 0))
{
}

win32_window::~win32_window ()
{
    if (handle ()) ::DestroyWindow (handle<id_type> ());
}

string win32_window::title () const
{
    string gTitle (::GetWindowTextLengthA (handle<id_type> ()),
                   string::value_type ());

    ::GetWindowTextA (handle<id_type> (), &gTitle[0], static_cast<int> (gTitle.length ()));
    return gTitle;
}

void win32_window::set_title (string const& gText) noexcept
{
    ::SetWindowTextA (handle<id_type> (), &gText[0]);
}

void win32_window::set_shaded (bool) noexcept
{

}

bool win32_window::is_shaded () const noexcept
{
    return false;
}

void win32_window::set_modal (bool bModal) noexcept
{
    if (!owner ().expired ())
        ::EnableWindow (owner ().lock ()->handle<id_type> (), bModal ? false : true);
}

bool win32_window::is_modal () const noexcept
{
    return !owner ().expired () and !::IsWindowEnabled (owner ().lock ()->handle<id_type> ());
}

void win32_window::set_fullscreen (bool) noexcept
{

}

bool win32_window::is_fullscreen () const noexcept
{
    return false;
}

void win32_window::set_maximized (bool bMaximized) noexcept
{
    ::ShowWindowAsync (handle<id_type> (), bMaximized ? SW_MAXIMIZE : SW_SHOWNORMAL);
}

bool win32_window::is_maximized () const noexcept
{
    return ::IsZoomed (handle<id_type> ());
}

void win32_window::set_minimized (bool bMinimized) noexcept
{
    if (bMinimized) ::CloseWindow (handle<id_type> ());
    else ::OpenIcon (handle<id_type> ());
}

bool win32_window::is_minimized () const noexcept
{
    return ::IsIconic (handle<id_type> ());
}

void win32_window::set_flags (window_flags flags) noexcept
{
    ::SetWindowLongA (handle<id_type> (),
                      GWL_EXSTYLE,
                      toExStyles (flags));
}

void win32_window::set_visible_in_taskbar (bool bVis) noexcept
{
    long_t uStyles = ::GetWindowLong (handle<id_type> (), GWL_EXSTYLE);

    ::SetWindowLongA (handle<id_type> (),
                      GWL_EXSTYLE,
                      bVis ? uStyles | WS_EX_APPWINDOW : uStyles | ~WS_EX_APPWINDOW);
}

bool win32_window::is_visible_in_taskbar () const noexcept
{
    return ::GetWindowLong (handle<id_type> (), GWL_EXSTYLE) & WS_EX_APPWINDOW;
}

void win32_window::set_visible_in_pager (bool bVis) noexcept
{
    long_t uStyles = ::GetWindowLong (handle<id_type> (), GWL_EXSTYLE);

    ::SetWindowLongA (handle<id_type> (),
                      GWL_EXSTYLE,
                      bVis ? uStyles | WS_EX_TOOLWINDOW : uStyles | ~WS_EX_TOOLWINDOW);
}

bool win32_window::is_visible_in_pager () const noexcept
{
    return ::GetWindowLong (handle<id_type> (), GWL_EXSTYLE) & WS_EX_TOOLWINDOW;
}

void win32_window::flash (uint count) noexcept
{
    FLASHWINFO info;

    info.hwnd      = handle<id_type> ();
    info.cbSize    = sizeof (FLASHWINFO);
    info.uCount    = count;
    info.dwFlags   = count ? FLASHW_ALL : FLASHW_ALL | FLASHW_TIMERNOFG;
    info.dwTimeout = 0;

    ::FlashWindowEx (&info);
}

rect win32_window::geometry () const
{
    Win32Rect rct;

    ::GetClientRect (handle<id_type> (), &rct);
    return rect (rct.left_unsafe  (), rct.top_unsafe    (),
                 rct.width_unsafe (), rct.height_unsafe ());
}

bool win32_window::is_mapped () const
{
    return ::IsWindowVisible (handle<id_type> ());
}

void win32_window::set_owner (const_pointer pWnd)
{
    ::SetParent (handle<id_type> (), pWnd == nullptr ? ::HWND () : pWnd->handle<id_type> ());
}

void win32_window::set_geometry (rect const& rect)
{
    ::SetWindowPos (handle<id_type> (), ::HWND (), rect.left, rect.top, rect.right, rect.bottom, 0);
}

void win32_window::raise ()
{
    ::BringWindowToTop (handle<id_type> ());
}

void win32_window::lower ()
{
    ::SetForegroundWindow (::GetWindow (handle<id_type> (), GW_HWNDPREV));
}

void win32_window::move (point2i gPos)
{
    Win32Rect rect;

    ::GetClientRect (handle<id_type> (), &rect);
    ::MoveWindow    (handle<id_type> (), gPos.x, gPos.y, rect.width (), rect.height (), false);
}

void win32_window::map ()
{
    ::ShowWindowAsync (handle<id_type> (), SW_SHOW);
}

void win32_window::unmap ()
{
    ::ShowWindowAsync (handle<id_type> (), SW_HIDE);
}

void win32_window::map_minimized()
{

}

void win32_window::map_maximized()
{

}

void win32_window::keep_above(bool)
{

}

bool win32_window::is_above() const
{
    return false;
}

void win32_window::keep_below(bool)
{

}

bool win32_window::is_below() const
{
    return false;
}

} } // Ui

#endif // OS_WINDOWS
