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

namespace cppual { namespace Ui {

namespace {

typedef ::WNDCLASSEXA window_class_type;
typedef ::HINSTANCE   instance_type;
typedef ::HWND        handle_type;
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

    constexpr int16 left_unsafe () const noexcept
    { return static_cast<int16> (left); }

    constexpr int16 top_unsafe () const noexcept
    { return static_cast<int16> (top); }

    constexpr u16 width_unsafe () const noexcept
    { return static_cast<u16> (right - left); }

    constexpr uint height () const noexcept
    { return bottom - top; }

    constexpr u16 height_unsafe () const noexcept
    { return static_cast<u16> (bottom - top); }
};

inline long_t toWindowStyles (WindowFlags flags) noexcept
{
    long_t styles;

    return styles;
}

inline long_t toExStyles (WindowFlags flags) noexcept
{
    long_t styles;

    return styles;
}

} // anonymous namespace

// ====================================================

win32_window::win32_window (Rect const& gRect, u32, IDisplay* pDisplay)
: platform_wnd_interface (pDisplay,
           ::CreateWindowExA (0, WindowClass::registered ().name (), nullptr, WS_OVERLAPPEDWINDOW,
                              gRect.left, gRect.top, gRect.width (), gRect.height (),
                              0, 0, WindowClass::registered ().instance (), 0),
           ResourceType::Window)
{
}

win32_window::~win32_window ()
{
    if (id ()) ::DestroyWindow (id ().get<handle_type> ());
}

string win32_window::title () const
{
    string gTitle (::GetWindowTextLengthA (id ().get<handle_type> ()),
                   string::value_type ());

    ::GetWindowTextA (id ().get<handle_type> (), &gTitle[0], static_cast<int> (gTitle.length ()));
    return std::move (gTitle);
}

void win32_window::set_title (string const& gText) noexcept
{
    ::SetWindowTextA (id ().get<handle_type> (), &gText[0]);
}

void win32_window::set_shaded (bool) noexcept
{

}

bool win32_window::is_shaded () noexcept
{
    return false;
}

void win32_window::set_modal (bool bModal) noexcept
{
    if (!owner ().expired ())
        ::EnableWindow (owner ().lock ()->id().get<handle_type> (), bModal ? false : true);
}

bool win32_window::is_modal () noexcept
{
    return !owner ().expired () and !::IsWindowEnabled (owner ().lock ()->id().get<handle_type> ());
}

void win32_window::set_fullscreen (bool) noexcept
{

}

bool win32_window::is_fullscreen () noexcept
{
    return false;
}

void win32_window::set_maximized (bool bMaximized) noexcept
{
    ::ShowWindowAsync (id ().get<handle_type> (), bMaximized ? SW_MAXIMIZE : SW_SHOWNORMAL);
}

bool win32_window::is_maximized () noexcept
{
    return ::IsZoomed (id ().get<handle_type> ());
}

void win32_window::set_minimized (bool bMinimized) noexcept
{
    if (bMinimized) ::CloseWindow (id ().get<handle_type> ());
    else ::OpenIcon (id ().get<handle_type> ());
}

bool win32_window::is_minimized () noexcept
{
    return ::IsIconic (id ().get<handle_type> ());
}

void win32_window::set_flags (WindowFlags flags) noexcept
{
    ::SetWindowLongA (id ().get<handle_type> (),
                      GWL_EXSTYLE,
                      toExStyles (flags));
}

void win32_window::set_visible_in_taskbar (bool bVis) noexcept
{
    long_t uStyles = ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE);

    ::SetWindowLongA (id ().get<handle_type> (),
                      GWL_EXSTYLE,
                      bVis ? uStyles | WS_EX_APPWINDOW : uStyles | ~WS_EX_APPWINDOW);
}

bool win32_window::is_visible_in_taskbar () noexcept
{
    return ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE) & WS_EX_APPWINDOW;
}

void win32_window::set_visible_in_pager (bool bVis) noexcept
{
    long_t uStyles = ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE);

    ::SetWindowLongA (id ().get<handle_type> (),
                      GWL_EXSTYLE,
                      bVis ? uStyles | WS_EX_TOOLWINDOW : uStyles | ~WS_EX_TOOLWINDOW);
}

bool win32_window::is_visible_in_pager () noexcept
{
    return ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE) & WS_EX_TOOLWINDOW;
}

void win32_window::flash (uint count) noexcept
{
    FLASHWINFO info;

    info.hwnd      = id ().get<handle_type> ();
    info.cbSize    = sizeof (FLASHWINFO);
    info.uCount    = count;
    info.dwFlags   = count ? FLASHW_ALL : FLASHW_ALL | FLASHW_TIMERNOFG;
    info.dwTimeout = 0;

    ::FlashWindowEx (&info);
}

Rect win32_window::geometry () const
{
    Win32Rect rect;

    ::GetClientRect (id ().get<handle_type> (), &rect);
    return Rect (rect.left_unsafe  (), rect.top_unsafe    (),
                 rect.width_unsafe (), rect.height_unsafe ());
}

bool win32_window::is_mapped () const
{
    return ::IsWindowVisible (id ().get<handle_type> ());
}

void win32_window::set_owner (const_pointer pWnd)
{
    ::SetParent (id ().get<handle_type> (), pWnd == nullptr ? handle_type () : pWnd->id ().get<handle_type> ());
}

void win32_window::set_geometry (Rect const& rect)
{
    ::SetWindowPos (id ().get<handle_type> (), handle_type (), rect.left, rect.top, rect.right, rect.bottom, 0);
}

void win32_window::raise ()
{
    ::BringWindowToTop (id ().get<handle_type> ());
}

void win32_window::lower ()
{
    ::SetForegroundWindow (::GetWindow (id ().get<handle_type> (), GW_HWNDPREV));
}

void win32_window::move (point2i gPos)
{
    Win32Rect rect;

    ::GetClientRect (id ().get<handle_type> (), &rect);
    ::MoveWindow    (id ().get<handle_type> (), gPos.x, gPos.y, rect.width (), rect.height (), false);
}

void win32_window::map ()
{
    ::ShowWindowAsync (id ().get<handle_type> (), SW_SHOW);
}

void win32_window::unmap ()
{
    ::ShowWindowAsync (id ().get<handle_type> (), SW_HIDE);
}

} } // Ui

#endif // OS_WINDOWS
