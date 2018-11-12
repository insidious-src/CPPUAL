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

#ifndef CPPUAL_DEVICES_EVENT_H
#define CPPUAL_DEVICES_EVENT_H
#ifdef __cplusplus

#include <cppual/types.h>
#include <cppual/gfx/coord.h>
#include <cppual/input/keyboard.h>
#include <cppual/noncopyable.h>

namespace cppual { namespace Input {

struct SystemMessage final
{
    enum
    {
        Quit,
        Child,
        Terminate
    };
};

// =========================================================

class Event
{
public:
    typedef std::size_t size_type;

    enum Type
    {
        Null             = 0,
        KeyPressed       = 1 <<  0,
        KeyReleased      = 1 <<  1,
        MButtonDown      = 1 <<  2,
        MButtonUp        = 1 <<  3,
        MWheelStep       = 1 <<  4,
        MouseMove        = 1 <<  5,
        TouchPress       = 1 <<  6,
        TouchRelease     = 1 <<  7,
        TouchMove        = 1 <<  8,
        JoyConnect       = 1 <<  9,
        JoyDisconnect    = 1 << 10,
        JoyButtonPress   = 1 << 11,
        JoyButtonRelease = 1 << 12,
        JoyMove          = 1 << 13,
        JoyTrigger       = 1 << 14,
        JoyTrackMove     = 1 << 15,
        SystemMessage    = 1 << 16,
        Paint            = 1 << 17,
        Focus            = 1 << 18,
        Step             = 1 << 19,
        Size             = 1 << 20,
        Visibility       = 1 << 21,
        Property         = 1 << 22,
        Destroy          = 1 << 23,
        Key       = KeyPressed    | KeyReleased,
        Mouse     = MButtonDown   | MButtonUp     | MouseMove | MWheelStep,
        Touch     = TouchPress    | TouchRelease  | TouchMove,
        Joystick  = JoyConnect    | JoyDisconnect | JoyButtonPress | JoyButtonRelease   |
                    JoyMove       | JoyTrigger    | JoyTrackMove,
        Window    = Paint | Focus | Size          | Visibility     | Property | Destroy | Step,
        All       = Key   | Mouse | Touch         | Window         | SystemMessage
    };

    struct KeyData
    {
        Keyboard::Key key ;
        u16           mask;

        KeyData () noexcept = default;

        constexpr KeyData (Keyboard::Key nKey, u16 uMask) noexcept
        : key (nKey), mask (uMask)
        { }
    };

    struct MButtonData
    {
        point2u pos   ;
        u8      button;

        MButtonData () noexcept = default;

        constexpr MButtonData (point2u gPos, u8 uBtn) noexcept
        : pos (gPos), button (uBtn)
        { }
    };

    struct MWheelData
    {
        point2u pos;
        int32   delta;
    };

    struct TouchData
    {
        int32   pid;
        point2u pos;
    };

    struct JoyButtonData
    {
        size_type id;
        int32     button;
    };

    struct JoyAxisData
    {
        size_type id;
        int16     value;
        u8        axis;
    };

    struct JoyTriggerData
    {
        size_type id;
        int16     threshold;
        u8        trigger;

    };

    struct JoyTrackData
    {
        size_type id;
        point2i   pos;
        u8        track;
    };

    struct JoyPlugData
    {
        size_type id;
    };

    struct PaintData
    {
        Rect region;
    };

    struct PropertyData
    {
        u32   prop;
        int32 value;
    };

    union Data
    {
        KeyData        keyCode;
        MButtonData    mouseButton;
        point2u        position;
        MWheelData     wheel;
        TouchData      touch;
        JoyButtonData  joyButton;
        JoyAxisData    joyAxis;
        JoyPlugData    joyPlug;
        JoyTriggerData joyTrigger;
        JoyTrackData   joyTrack;
        int32          message;
        PaintData      paint;
        point2u        size;
        bool           state;
        PropertyData   property;

        Data () noexcept = default;

        constexpr Data (KeyData const& gKeyData) noexcept
        : keyCode (gKeyData)
        { }

        constexpr Data (MButtonData const& gKeyData) noexcept
        : mouseButton (gKeyData)
        { }
    };

    Event () noexcept = default;
    constexpr Data const& data () const noexcept { return m_data; }
    constexpr Type        type () const noexcept { return m_type; }

    constexpr Event (Type type, Data const& data = Data()) noexcept
    : m_data (data),
      m_type (type)
    { }

protected:
    Data m_data;
    Type m_type;
};

// =========================================================

struct MessageEvent : public Event
{
    MessageEvent (int nMsg) noexcept
    : Event      (Event::SystemMessage)
    {
        m_data.message = nMsg;
    }
};

// =========================================================

struct VisibilityEvent : public Event
{
    VisibilityEvent (bool bVis) noexcept
    : Event         (Event::Visibility)
    {
        m_data.state = bVis;
    }
};

// =========================================================

struct PaintEvent : public Event
{
    PaintEvent (Rect gRect) noexcept
    : Event    (Event::Paint)
    {
        m_data.paint.region = gRect;
    }
};

// =========================================================

struct SizeEvent : public Event
{
    SizeEvent (point2u size) noexcept
    : Event   (Event::Size)
    {
        m_data.size = size;
    }
};

// =========================================================

struct FocusEvent : public Event
{
    FocusEvent (bool in) noexcept
    : Event    (Event::Focus)
    {
        m_data.state = in;
    }
};

// =========================================================

struct StepEvent : public Event
{
    StepEvent (bool in) noexcept
    : Event   (Event::Step)
    {
        m_data.state = in;
    }
};

// =========================================================

struct PropertyEvent : public Event
{
    PropertyEvent (u32 prop, int32 value) noexcept
    : Event       (Event::Property)
    {
        m_data.property.prop  = prop;
        m_data.property.value = value;
    }
};

// =========================================================

struct KeyEvent : public Event
{
    enum class Action : byte
    {
        Press,
        Release
    };

    constexpr KeyEvent (Keyboard::Key nKey, u16 uMask = 0, Action action = Action::Press) noexcept
    : Event (action == Action::Press ? Event::KeyPressed : Event::KeyReleased, { KeyData (nKey, uMask) })
    { }
};

// =========================================================

struct KeyPressEvent : public KeyEvent
{
    constexpr KeyPressEvent (Keyboard::Key nKey, u16 uMask = 0) noexcept
    : KeyEvent (nKey, uMask, Action::Press)
    { }
};

// =========================================================

struct KeyReleaseEvent : public KeyEvent
{
    constexpr KeyReleaseEvent (Keyboard::Key nKey, u16 uMask = 0) noexcept
    : KeyEvent (nKey, uMask, Action::Release)
    { }
};

// =========================================================

struct MouseEvent : public Event
{
    constexpr MouseEvent (u8 nBtn, point2u gPos, bool press_or_release = true) noexcept
    : Event (press_or_release ? Event::MButtonDown : Event::MButtonUp, { MButtonData (gPos, nBtn) })
    { }
};

// =========================================================

struct MousePressEvent : public MouseEvent
{
    constexpr MousePressEvent (u8 nBtn, point2u gPos) noexcept
    : MouseEvent (nBtn, gPos, true)
    { }
};

// =========================================================

struct MouseReleaseEvent : public MouseEvent
{
    constexpr MouseReleaseEvent (u8 nBtn, point2u gPos) noexcept
    : MouseEvent (nBtn, gPos, false)
    { }
};

// =========================================================

struct MouseMoveEvent : public Event
{
    MouseMoveEvent (point2u gPos) noexcept
    : Event        (Event::MouseMove)
    {
        m_data.position = gPos;
    }
};

// =========================================================

struct WheelEvent : public Event
{
    WheelEvent (int32 nDelta, point2u gPos) noexcept
    : Event    (Event::MWheelStep)
    {
        m_data.wheel.delta = nDelta;
        m_data.wheel.pos   = gPos;
    }
};

// =========================================================

struct TouchPressEvent : public Event
{
    TouchPressEvent (int32 pid, point2u gPos) noexcept
    : Event         (Event::TouchPress)
    {
        m_data.touch.pid = pid;
        m_data.touch.pos = gPos;
    }
};

// =========================================================

struct TouchReleaseEvent : public Event
{
    TouchReleaseEvent (int32 pid, point2u gPos) noexcept
    : Event           (Event::TouchRelease)
    {
        m_data.touch.pid = pid;
        m_data.touch.pos = gPos;
    }
};

// =========================================================

struct TouchMovedEvent : public Event
{
    TouchMovedEvent (int32 pid, point2u gPos) noexcept
    : Event         (Event::TouchMove)
    {
        m_data.touch.pid = pid;
        m_data.touch.pos = gPos;
    }
};

} } // namespace Input

#endif // __cplusplus
#endif // CPPUAL_DEVICES_EVENT_H
