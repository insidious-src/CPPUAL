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
#include "w32input.h"

#ifdef   OS_WINDOWS
#define  WIN32_MEAN_AND_LEAN 1
#include <windows.h>

namespace cppual { namespace Ui {

namespace {

typedef ::MSG  message_type;
typedef ::HWND handle_type;
typedef ::UINT message_value;

struct Win32Event final : public message_type
{
    typedef IDisplayQueue::event_type::window_type window_type;
    typedef IDisplayQueue::event_type              event_type;

    enum
    {
        KeyPress      = WM_KEYUP,
        KeyRelease    = WM_KEYDOWN,
        ButtonPress   = WM_MBUTTONDOWN,
        ButtonRelease = WM_MBUTTONUP,
        PointerMove   = WM_MOUSEMOVE,
        Scroll        = WM_MOUSEWHEEL,
        Destroy       = WM_DESTROY,
        BeginSizeMove = WM_ENTERSIZEMOVE,
        EndSizeMove   = WM_EXITSIZEMOVE,
        Show          = WM_SHOWWINDOW,
        SetFocus      = WM_SETFOCUS,
        KillFocus     = WM_KILLFOCUS,
        Paint         = WM_PAINT,
        Size          = WM_SIZE,
        Close         = WM_CLOSE,
        Quit          = WM_QUIT
    };

    Win32Event () = default;

    constexpr Win32Event (std::nullptr_t) noexcept
    : message_type ()
    { }

    constexpr handle_type window () const noexcept
    { return hwnd; }

    constexpr message_value type () const noexcept
    { return message; }

    template <typename T>
    T secondary () const noexcept
    { return reinterpret_cast<T> (lParam); }

    template <typename T>
    constexpr T primary () const noexcept
    { return static_cast<T> (wParam); }

    constexpr point2u position () const noexcept
    { return { static_cast<u16> (pt.x), static_cast<u16> (pt.y) }; }

    inline void operator ()() const
    {
        switch (type ())
        {
        case Win32Event::KeyPress:
            event_type::registers ().keyPress (window (), { primary<u8> () });
            break;
        case Win32Event::KeyRelease:
            event_type::registers ().keyRelease (window (), { primary<u8> () });
            break;
        case Win32Event::ButtonPress:
            event_type::registers ().mousePress (window (), { position (), primary<u8> () });
            break;
        case Win32Event::ButtonRelease:
            event_type::registers ().mouseRelease (window (), { position (), primary<u8> () });
            break;
        case Win32Event::Scroll:
            event_type::registers ().scroll (window (), { position (), primary<int32> () });
            break;
        case Win32Event::PointerMove:
            event_type::registers ().mouseMove (window (), position ());
            break;
        case Win32Event::Show:
            break;
        case Win32Event::Destroy:
            break;
        case Win32Event::Paint:
            break;
        case Win32Event::Size:
            break;
        case Win32Event::SetFocus:
            break;
        case Win32Event::KillFocus:
            break;
        default:
            break;
        }
    }

    inline event_type toEvent () const noexcept
    {
        switch (type ())
        {
        case Win32Event::KeyPress:
            return KeyPressEvent (window (), primary<u8> ());
        case Win32Event::KeyRelease:
            return KeyReleaseEvent (window (), primary<u8> ());
        case Win32Event::ButtonPress:
            return MousePressEvent (window (), primary<u8> (), position ());
        case Win32Event::ButtonRelease:
            return MouseReleaseEvent (window (), primary<u8> (), position ());
        case Win32Event::Scroll:
            return ScrollEvent (window (), primary<int32> (), position ());
        case Win32Event::PointerMove:
            return PointerMoveEvent (window (), position ());
        case Win32Event::Show:
            return event_type ();
        case Win32Event::Destroy:
            return event_type ();
        case Win32Event::Paint:
            return event_type ();
        case Win32Event::Size:
            return event_type ();
        case Win32Event::SetFocus:
            return event_type ();
        case Win32Event::KillFocus:
            return event_type ();
        default:
            return event_type ();
        }
    }
};

} // anonymous namespace

// ====================================================

Win32Queue::Win32Queue () noexcept
: IDisplayQueue (::GetDC (handle_type ()))
{
}

bool Win32Queue::set_window_events (IWindow const&, mask_type)
{
    return false;
}

void Win32Queue::send (event_type const& event)
{
    ::SendMessageA (event.window ().get<handle_type> (), event.type (), 0, 0);
}

void Win32Queue::post (event_type const& event)
{
    ::PostMessageA (event.window ().get<handle_type> (), event.type (), 0, 0);
}

bool Win32Queue::pop_front (event_type& event, bool wait)
{
    static Win32Event internal_event;

    if (wait ? ::GetMessageA  (&internal_event, nullptr, 0, 0) :
               ::PeekMessageA (&internal_event, nullptr, 0, 0, PM_NOREMOVE) <= 0)
        return false;

    ::TranslateMessage (&internal_event);
    event = internal_event.toEvent ();
    return true;
}

int Win32Queue::poll (IWindow const& window, atomic_bool& polling)
{
    Win32Event event;

    while (polling.load (std::memory_order_relaxed))
    {
        switch (::GetMessageA  (&event, window.id ().get<handle_type> (), 0, 0))
        {
        case  0:
            polling = false;
            return 0;
        case -1:
            return 1;
        }

        ::TranslateMessage (&event);
        event ();

        if (event.type () == Win32Event::Quit) polling = false;
    }

    return 0;
}

} } // Ui

#endif // OS_WINDOWS
