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
#include "w32input.h"

#ifdef   OS_WINDOWS
#define  WIN32_MEAN_AND_LEAN 1
#include <windows.h>

namespace cppual { namespace ui {

namespace {

typedef ::MSG  message_type;
typedef ::HWND handle_type;
typedef ::UINT message_value;

struct Win32Event final : public message_type
{
    typedef display_queue_interface::window_type window_type;
    typedef display_queue_interface::event_type  event_type ;

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
            event_queue::events ().keyPress (window (), { primary<input::keyboard::key> (),
                                                          primary<input::keyboard::modifier> () });
            break;
        case Win32Event::KeyRelease:
            event_queue::events ().keyRelease (window (), { primary<input::keyboard::key> (),
                                                            primary<input::keyboard::modifier> () });
            break;
        case Win32Event::ButtonPress:
            event_queue::events ().mousePress (window (), { position (), primary<u8> () });
            break;
        case Win32Event::ButtonRelease:
            event_queue::events ().mouseRelease (window (), { position (), primary<u8> () });
            break;
        case Win32Event::Scroll:
            event_queue::events ().scroll (window (), { position (), primary<i32> () });
            break;
        case Win32Event::PointerMove:
            event_queue::events ().mouseMove (window (), position ());
            break;
        case Win32Event::Show:
            event_queue::events ().winVisible (window (), primary<bool> ());
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
            return input::key_press_event (primary<input::keyboard::key> ());
        case Win32Event::KeyRelease:
            return input::key_release_event (primary<input::keyboard::key> ());
        case Win32Event::ButtonPress:
            return input::mouse_press_event (primary<u8> (), position ());
        case Win32Event::ButtonRelease:
            return input::mouse_release_event (primary<u8> (), position ());
        case Win32Event::Scroll:
            return input::wheel_event (primary<i32> (), position ());
        case Win32Event::PointerMove:
            return input::mouse_move_event (position ());
        case Win32Event::Show:
            return input::visibility_event (true);
        case Win32Event::Destroy:
            return input::visibility_event (false);
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

win32_queue::win32_queue () noexcept
: display_queue_interface (display_interface::primary ())
{
}

bool win32_queue::set_window_events (platform_wnd_interface const&, mask_type)
{
    return false;
}

void win32_queue::send (window_type const& wnd, event_type const& event)
{
    ::SendMessageA (wnd.handle<handle_type> (), event.type (), 0, 0);
}

void win32_queue::post (window_type const& wnd, event_type const& event)
{
    ::PostMessageA (wnd.handle<handle_type> (), event.type (), 0, 0);
}

bool win32_queue::pop_front (bool wait)
{
    static Win32Event internal_event;

    if (wait ? ::GetMessageA  (&internal_event, nullptr, 0, 0) :
               ::PeekMessageA (&internal_event, nullptr, 0, 0, PM_NOREMOVE) <= 0)
        return false;

    ::TranslateMessage (&internal_event);
    //event = internal_event.toEvent ();
    return true;
}

int win32_queue::poll (bool_type& polling)
{
    Win32Event event;

    while (polling.load (std::memory_order_relaxed))
    {
        switch (::GetMessageA  (&event, nullptr, 0, 0))
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
