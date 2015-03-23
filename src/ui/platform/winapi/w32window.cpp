/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

Win32Window::Win32Window (Rect const& gRect, u32, IDisplay* pDisplay)
: IWindow (pDisplay,
           ::CreateWindowExA (0, WindowClass::registered ().name (), nullptr, WS_OVERLAPPEDWINDOW,
                              gRect.left, gRect.top, gRect.width (), gRect.height (),
                              0, 0, WindowClass::registered ().instance (), 0),
           ResourceType::Window)
{
}

Win32Window::~Win32Window ()
{
    if (id ()) ::DestroyWindow (id ().get<handle_type> ());
}

string Win32Window::title () const
{
    string gTitle (::GetWindowTextLengthA (id ().get<handle_type> ()),
                   string::value_type ());

    ::GetWindowTextA (id ().get<handle_type> (), &gTitle[0], static_cast<int> (gTitle.length ()));
    return std::move (gTitle);
}

void Win32Window::setTitle (string const& gText) noexcept
{
    ::SetWindowTextA (id ().get<handle_type> (), &gText[0]);
}

void Win32Window::setShaded (bool) noexcept
{

}

bool Win32Window::isShaded () noexcept
{
    return false;
}

void Win32Window::setModal (bool bModal) noexcept
{
    if (!owner ().expired ())
        ::EnableWindow (owner ().lock ()->id().get<handle_type> (), bModal ? false : true);
}

bool Win32Window::isModal () noexcept
{
    return !owner ().expired () and !::IsWindowEnabled (owner ().lock ()->id().get<handle_type> ());
}

void Win32Window::setFullscreen (bool) noexcept
{

}

bool Win32Window::isFullscreen () noexcept
{
    return false;
}

void Win32Window::setMaximized (bool bMaximized) noexcept
{
    ::ShowWindowAsync (id ().get<handle_type> (), bMaximized ? SW_MAXIMIZE : SW_SHOWNORMAL);
}

bool Win32Window::isMaximized () noexcept
{
    return ::IsZoomed (id ().get<handle_type> ());
}

void Win32Window::setMinimized (bool bMinimized) noexcept
{
    if (bMinimized) ::CloseWindow (id ().get<handle_type> ());
    else ::OpenIcon (id ().get<handle_type> ());
}

bool Win32Window::isMinimized () noexcept
{
    return ::IsIconic (id ().get<handle_type> ());
}

void Win32Window::setFlags (WindowFlags flags) noexcept
{
    ::SetWindowLongA (id ().get<handle_type> (),
                      GWL_EXSTYLE,
                      toExStyles (flags));
}

void Win32Window::setVisibleInTaskbar (bool bVis) noexcept
{
    long_t uStyles = ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE);

    ::SetWindowLongA (id ().get<handle_type> (),
                      GWL_EXSTYLE,
                      bVis ? uStyles | WS_EX_APPWINDOW : uStyles | ~WS_EX_APPWINDOW);
}

bool Win32Window::isVisibleInTaskbar () noexcept
{
    return ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE) & WS_EX_APPWINDOW;
}

void Win32Window::setVisibleInPager (bool bVis) noexcept
{
    long_t uStyles = ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE);

    ::SetWindowLongA (id ().get<handle_type> (),
                      GWL_EXSTYLE,
                      bVis ? uStyles | WS_EX_TOOLWINDOW : uStyles | ~WS_EX_TOOLWINDOW);
}

bool Win32Window::isVisibleInPager () noexcept
{
    return ::GetWindowLong (id ().get<handle_type> (), GWL_EXSTYLE) & WS_EX_TOOLWINDOW;
}

void Win32Window::flash (uint count) noexcept
{
    FLASHWINFO info;

    info.hwnd      = id ().get<handle_type> ();
    info.cbSize    = sizeof (FLASHWINFO);
    info.uCount    = count;
    info.dwFlags   = count ? FLASHW_ALL : FLASHW_ALL | FLASHW_TIMERNOFG;
    info.dwTimeout = 0;

    ::FlashWindowEx (&info);
}

Rect Win32Window::geometry () const
{
    Win32Rect rect;

    ::GetClientRect (id ().get<handle_type> (), &rect);
    return Rect (rect.left_unsafe  (), rect.top_unsafe    (),
                 rect.width_unsafe (), rect.height_unsafe ());
}

bool Win32Window::isMapped () const
{
    return ::IsWindowVisible (id ().get<handle_type> ());
}

void Win32Window::setOwner (const_pointer pWnd)
{
    ::SetParent (id ().get<handle_type> (), pWnd == nullptr ? handle_type () : pWnd->id ().get<handle_type> ());
}

void Win32Window::setGeometry (Rect const& rect)
{
    ::SetWindowPos (id ().get<handle_type> (), handle_type (), rect.left, rect.top, rect.right, rect.bottom, 0);
}

void Win32Window::raise ()
{
    ::BringWindowToTop (id ().get<handle_type> ());
}

void Win32Window::lower ()
{
    ::SetForegroundWindow (::GetWindow (id ().get<handle_type> (), GW_HWNDPREV));
}

void Win32Window::move (point2i gPos)
{
    Win32Rect rect;

    ::GetClientRect (id ().get<handle_type> (), &rect);
    ::MoveWindow    (id ().get<handle_type> (), gPos.x, gPos.y, rect.width (), rect.height (), false);
}

void Win32Window::map ()
{
    ::ShowWindowAsync (id ().get<handle_type> (), SW_SHOW);
}

void Win32Window::unmap ()
{
    ::ShowWindowAsync (id ().get<handle_type> (), SW_HIDE);
}

} } // Ui

#endif // OS_WINDOWS
