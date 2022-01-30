#ifndef CPPUAL_XKB_HPP
#define CPPUAL_XKB_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>
#include <cppual/string.h>

#include <vector>

#include <xcb/xkb.h>

namespace cppual { namespace xkb {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_xkb_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_xkb_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::xkb::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::xkb::event::dispatcher<Connection>;
    using error_dispatcher = cppual::xkb::error::dispatcher;
};

namespace event {
template<typename Connection>
class new_keyboard_notify
        : public cppual::generic::event<xcb_xkb_new_keyboard_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_new_keyboard_notify_event_t> base;

    template<typename C>
    new_keyboard_notify(C && c,
                        uint8_t first_event,
                        const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~new_keyboard_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_NEW_KEYBOARD_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_NEW_KEYBOARD_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class new_keyboard_notify
} // namespace event


namespace event {
template<typename Connection>
class map_notify
        : public cppual::generic::event<xcb_xkb_map_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_map_notify_event_t> base;

    template<typename C>
    map_notify(C && c,
               uint8_t first_event,
               const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~map_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_MAP_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_MAP_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class map_notify
} // namespace event


namespace event {
template<typename Connection>
class state_notify
        : public cppual::generic::event<xcb_xkb_state_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_state_notify_event_t> base;

    template<typename C>
    state_notify(C && c,
                 uint8_t first_event,
                 const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~state_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_STATE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_STATE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class state_notify
} // namespace event


namespace event {
template<typename Connection>
class controls_notify
        : public cppual::generic::event<xcb_xkb_controls_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_controls_notify_event_t> base;

    template<typename C>
    controls_notify(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~controls_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_CONTROLS_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_CONTROLS_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class controls_notify
} // namespace event


namespace event {
template<typename Connection>
class indicator_state_notify
        : public cppual::generic::event<xcb_xkb_indicator_state_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_indicator_state_notify_event_t> base;

    template<typename C>
    indicator_state_notify(C && c,
                           uint8_t first_event,
                           const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~indicator_state_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_INDICATOR_STATE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_INDICATOR_STATE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class indicator_state_notify
} // namespace event


namespace event {
template<typename Connection>
class indicator_map_notify
        : public cppual::generic::event<xcb_xkb_indicator_map_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_indicator_map_notify_event_t> base;

    template<typename C>
    indicator_map_notify(C && c,
                         uint8_t first_event,
                         const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~indicator_map_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_INDICATOR_MAP_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_INDICATOR_MAP_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class indicator_map_notify
} // namespace event


namespace event {
template<typename Connection>
class names_notify
        : public cppual::generic::event<xcb_xkb_names_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_names_notify_event_t> base;

    template<typename C>
    names_notify(C && c,
                 uint8_t first_event,
                 const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~names_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_NAMES_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_NAMES_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class names_notify
} // namespace event


namespace event {
template<typename Connection>
class compat_map_notify
        : public cppual::generic::event<xcb_xkb_compat_map_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_compat_map_notify_event_t> base;

    template<typename C>
    compat_map_notify(C && c,
                      uint8_t first_event,
                      const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~compat_map_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_COMPAT_MAP_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_COMPAT_MAP_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class compat_map_notify
} // namespace event


namespace event {
template<typename Connection>
class bell_notify
        : public cppual::generic::event<xcb_xkb_bell_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_bell_notify_event_t> base;

    template<typename C>
    bell_notify(C && c,
                uint8_t first_event,
                const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~bell_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_BELL_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_BELL_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    name(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->name),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->name,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    window(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->window),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->window,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class bell_notify



} // namespace event


namespace event {
template<typename Connection>
class action_message
        : public cppual::generic::event<xcb_xkb_action_message_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_action_message_event_t> base;

    template<typename C>
    action_message(C && c,
                   uint8_t first_event,
                   const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~action_message(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_ACTION_MESSAGE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_ACTION_MESSAGE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class action_message
} // namespace event


namespace event {
template<typename Connection>
class access_x_notify
        : public cppual::generic::event<xcb_xkb_access_x_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_access_x_notify_event_t> base;

    template<typename C>
    access_x_notify(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~access_x_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_ACCESS_X_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_ACCESS_X_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class access_x_notify
} // namespace event


namespace event {
template<typename Connection>
class extension_device_notify
        : public cppual::generic::event<xcb_xkb_extension_device_notify_event_t>
{
public:
    typedef cppual::xkb::extension extension;

    typedef cppual::generic::event<xcb_xkb_extension_device_notify_event_t> base;

    template<typename C>
    extension_device_notify(C && c,
                            uint8_t first_event,
                            const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~extension_device_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_EXTENSION_DEVICE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XKB_EXTENSION_DEVICE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class extension_device_notify
} // namespace event



namespace error {
class keyboard
        : public cppual::generic::error<keyboard,
        ::xcb_xkb_keyboard_error_t>
{
public:
    using cppual::generic::error<keyboard, ::xcb_xkb_keyboard_error_t>::error;

    virtual ~keyboard(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XKB_KEYBOARD;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::xkb::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_XKB_KEYBOARD");
    }

protected:
    uint8_t _M_first_error;
}; // class keyboard
} // namespace error


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class use_extension
        : public cppual::generic::reply<use_extension<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_use_extension_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<use_extension<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_use_extension_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    use_extension(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class use_extension

} // namespace detail

namespace checked {
template<typename Connection>
using use_extension = detail::use_extension<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_use_extension)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using use_extension = detail::use_extension<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_use_extension_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::use_extension<Connection>
use_extension(Connection && c, Parameter && ... parameter)
{
    return reply::checked::use_extension<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::use_extension<Connection>
use_extension_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::use_extension<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_events_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_select_events_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_events(Parameter && ... parameter)
{
    ::xcb_xkb_select_events(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
bell_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_bell_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
bell(Parameter && ... parameter)
{
    ::xcb_xkb_bell(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_state
        : public cppual::generic::reply<get_state<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_state_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_state<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_state_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_state(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_state

} // namespace detail

namespace checked {
template<typename Connection>
using get_state = detail::get_state<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_state)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_state = detail::get_state<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_state_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_state<Connection>
get_state(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_state<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_state<Connection>
get_state_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_state<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
latch_lock_state_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_latch_lock_state_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
latch_lock_state(Parameter && ... parameter)
{
    ::xcb_xkb_latch_lock_state(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_controls
        : public cppual::generic::reply<get_controls<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_controls_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_controls<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_controls_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_controls(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_controls

} // namespace detail

namespace checked {
template<typename Connection>
using get_controls = detail::get_controls<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_controls)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_controls = detail::get_controls<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_controls_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_controls<Connection>
get_controls(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_controls<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_controls<Connection>
get_controls_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_controls<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_controls_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_set_controls_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_controls(Parameter && ... parameter)
{
    ::xcb_xkb_set_controls(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_map
        : public cppual::generic::reply<get_map<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_map_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_map<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_map_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_map(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_map

} // namespace detail

namespace checked {
template<typename Connection>
using get_map = detail::get_map<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_map)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_map = detail::get_map<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_map_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_map<Connection>
get_map(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_map<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_map<Connection>
get_map_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_map<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_map_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_set_map_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_map(Parameter && ... parameter)
{
    ::xcb_xkb_set_map(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_compat_map
        : public cppual::generic::reply<get_compat_map<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_compat_map_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_compat_map<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_compat_map_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_compat_map(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xkb_get_compat_map_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_sym_interpret_t,
            SIGNATURE(xcb_xkb_get_compat_map_si_rtrn),
            SIGNATURE(xcb_xkb_get_compat_map_si_rtrn_length)>
            >
            si_rtrn(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_get_compat_map_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_sym_interpret_t,
                SIGNATURE(xcb_xkb_get_compat_map_si_rtrn),
                SIGNATURE(xcb_xkb_get_compat_map_si_rtrn_length)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_get_compat_map_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_mod_def_t,
            SIGNATURE(xcb_xkb_get_compat_map_group_rtrn),
            SIGNATURE(xcb_xkb_get_compat_map_group_rtrn_length)>
            >
            group_rtrn(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_get_compat_map_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_mod_def_t,
                SIGNATURE(xcb_xkb_get_compat_map_group_rtrn),
                SIGNATURE(xcb_xkb_get_compat_map_group_rtrn_length)>
                >(this->_M_c, this->get());
    }
}; // class get_compat_map

} // namespace detail

namespace checked {
template<typename Connection>
using get_compat_map = detail::get_compat_map<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_compat_map)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_compat_map = detail::get_compat_map<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_compat_map_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_compat_map<Connection>
get_compat_map(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_compat_map<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_compat_map<Connection>
get_compat_map_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_compat_map<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_compat_map_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_set_compat_map_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_compat_map(Parameter && ... parameter)
{
    ::xcb_xkb_set_compat_map(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_indicator_state
        : public cppual::generic::reply<get_indicator_state<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_indicator_state_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_indicator_state<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_indicator_state_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_indicator_state(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_indicator_state

} // namespace detail

namespace checked {
template<typename Connection>
using get_indicator_state = detail::get_indicator_state<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_indicator_state)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_indicator_state = detail::get_indicator_state<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_indicator_state_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_indicator_state<Connection>
get_indicator_state(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_indicator_state<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_indicator_state<Connection>
get_indicator_state_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_indicator_state<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_indicator_map
        : public cppual::generic::reply<get_indicator_map<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_indicator_map_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_indicator_map<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_indicator_map_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_indicator_map(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xkb_get_indicator_map_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_indicator_map_t,
            SIGNATURE(xcb_xkb_get_indicator_map_maps),
            SIGNATURE(xcb_xkb_get_indicator_map_maps_length)>
            >
            maps(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_get_indicator_map_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_indicator_map_t,
                SIGNATURE(xcb_xkb_get_indicator_map_maps),
                SIGNATURE(xcb_xkb_get_indicator_map_maps_length)>
                >(this->_M_c, this->get());
    }
}; // class get_indicator_map

} // namespace detail

namespace checked {
template<typename Connection>
using get_indicator_map = detail::get_indicator_map<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_indicator_map)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_indicator_map = detail::get_indicator_map<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_indicator_map_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_indicator_map<Connection>
get_indicator_map(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_indicator_map<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_indicator_map<Connection>
get_indicator_map_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_indicator_map<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_indicator_map_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_set_indicator_map_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_indicator_map(Parameter && ... parameter)
{
    ::xcb_xkb_set_indicator_map(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_named_indicator
        : public cppual::generic::reply<get_named_indicator<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_named_indicator_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_named_indicator<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_named_indicator_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_named_indicator(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
            indicator(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->indicator),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->indicator,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_named_indicator

} // namespace detail

namespace checked {
template<typename Connection>
using get_named_indicator = detail::get_named_indicator<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_named_indicator)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_named_indicator = detail::get_named_indicator<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_named_indicator_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_named_indicator<Connection>
get_named_indicator(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_named_indicator<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_named_indicator<Connection>
get_named_indicator_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_named_indicator<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_named_indicator_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_set_named_indicator_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_named_indicator(Parameter && ... parameter)
{
    ::xcb_xkb_set_named_indicator(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_names
        : public cppual::generic::reply<get_names<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_names_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_names<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_names_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_names(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_names

} // namespace detail

namespace checked {
template<typename Connection>
using get_names = detail::get_names<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_names)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_names = detail::get_names<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_names_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_names<Connection>
get_names(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_names<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_names<Connection>
get_names_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_names<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_names_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_set_names_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_names(Parameter && ... parameter)
{
    ::xcb_xkb_set_names(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class per_client_flags
        : public cppual::generic::reply<per_client_flags<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_per_client_flags_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<per_client_flags<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_per_client_flags_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    per_client_flags(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class per_client_flags

} // namespace detail

namespace checked {
template<typename Connection>
using per_client_flags = detail::per_client_flags<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_per_client_flags)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using per_client_flags = detail::per_client_flags<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_per_client_flags_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::per_client_flags<Connection>
per_client_flags(Connection && c, Parameter && ... parameter)
{
    return reply::checked::per_client_flags<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::per_client_flags<Connection>
per_client_flags_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::per_client_flags<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_components
        : public cppual::generic::reply<list_components<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_list_components_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_components<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_list_components_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_components(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xkb_list_components_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_listing_t,
            SIGNATURE(xcb_xkb_listing_next),
            SIGNATURE(xcb_xkb_listing_sizeof),
            SIGNATURE(xcb_xkb_list_components_keymaps_iterator)>
            >
            keymaps(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_list_components_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_listing_t,
                SIGNATURE(xcb_xkb_listing_next),
                SIGNATURE(xcb_xkb_listing_sizeof),
                SIGNATURE(xcb_xkb_list_components_keymaps_iterator)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_list_components_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_listing_t,
            SIGNATURE(xcb_xkb_listing_next),
            SIGNATURE(xcb_xkb_listing_sizeof),
            SIGNATURE(xcb_xkb_list_components_keycodes_iterator)>
            >
            keycodes(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_list_components_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_listing_t,
                SIGNATURE(xcb_xkb_listing_next),
                SIGNATURE(xcb_xkb_listing_sizeof),
                SIGNATURE(xcb_xkb_list_components_keycodes_iterator)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_list_components_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_listing_t,
            SIGNATURE(xcb_xkb_listing_next),
            SIGNATURE(xcb_xkb_listing_sizeof),
            SIGNATURE(xcb_xkb_list_components_types_iterator)>
            >
            types(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_list_components_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_listing_t,
                SIGNATURE(xcb_xkb_listing_next),
                SIGNATURE(xcb_xkb_listing_sizeof),
                SIGNATURE(xcb_xkb_list_components_types_iterator)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_list_components_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_listing_t,
            SIGNATURE(xcb_xkb_listing_next),
            SIGNATURE(xcb_xkb_listing_sizeof),
            SIGNATURE(xcb_xkb_list_components_compat_maps_iterator)>
            >
            compat_maps(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_list_components_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_listing_t,
                SIGNATURE(xcb_xkb_listing_next),
                SIGNATURE(xcb_xkb_listing_sizeof),
                SIGNATURE(xcb_xkb_list_components_compat_maps_iterator)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_list_components_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_listing_t,
            SIGNATURE(xcb_xkb_listing_next),
            SIGNATURE(xcb_xkb_listing_sizeof),
            SIGNATURE(xcb_xkb_list_components_symbols_iterator)>
            >
            symbols(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_list_components_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_listing_t,
                SIGNATURE(xcb_xkb_listing_next),
                SIGNATURE(xcb_xkb_listing_sizeof),
                SIGNATURE(xcb_xkb_list_components_symbols_iterator)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_list_components_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_listing_t,
            SIGNATURE(xcb_xkb_listing_next),
            SIGNATURE(xcb_xkb_listing_sizeof),
            SIGNATURE(xcb_xkb_list_components_geometries_iterator)>
            >
            geometries(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_list_components_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_listing_t,
                SIGNATURE(xcb_xkb_listing_next),
                SIGNATURE(xcb_xkb_listing_sizeof),
                SIGNATURE(xcb_xkb_list_components_geometries_iterator)>
                >(this->_M_c, this->get());
    }
}; // class list_components

} // namespace detail

namespace checked {
template<typename Connection>
using list_components = detail::list_components<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_list_components)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_components = detail::list_components<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_list_components_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_components<Connection>
list_components(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_components<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_components<Connection>
list_components_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_components<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_kbd_by_name
        : public cppual::generic::reply<get_kbd_by_name<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_kbd_by_name_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_kbd_by_name<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_kbd_by_name_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_kbd_by_name(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_kbd_by_name

} // namespace detail

namespace checked {
template<typename Connection>
using get_kbd_by_name = detail::get_kbd_by_name<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_kbd_by_name)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_kbd_by_name = detail::get_kbd_by_name<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_kbd_by_name_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_kbd_by_name<Connection>
get_kbd_by_name(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_kbd_by_name<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_kbd_by_name<Connection>
get_kbd_by_name_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_kbd_by_name<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_info
        : public cppual::generic::reply<get_device_info<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_get_device_info_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_info<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_get_device_info_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_info(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xkb_get_device_info_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_string8_t,
            SIGNATURE(xcb_xkb_get_device_info_name),
            SIGNATURE(xcb_xkb_get_device_info_name_length)>
            >
            name(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_get_device_info_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_string8_t,
                SIGNATURE(xcb_xkb_get_device_info_name),
                SIGNATURE(xcb_xkb_get_device_info_name_length)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_get_device_info_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_action_t,
            SIGNATURE(xcb_xkb_get_device_info_btn_actions),
            SIGNATURE(xcb_xkb_get_device_info_btn_actions_length)>
            >
            btn_actions(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_get_device_info_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_action_t,
                SIGNATURE(xcb_xkb_get_device_info_btn_actions),
                SIGNATURE(xcb_xkb_get_device_info_btn_actions_length)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xkb_get_device_info_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xkb_device_led_info_t,
            SIGNATURE(xcb_xkb_device_led_info_next),
            SIGNATURE(xcb_xkb_device_led_info_sizeof),
            SIGNATURE(xcb_xkb_get_device_info_leds_iterator)>
            >
            leds(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xkb_get_device_info_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xkb_device_led_info_t,
                SIGNATURE(xcb_xkb_device_led_info_next),
                SIGNATURE(xcb_xkb_device_led_info_sizeof),
                SIGNATURE(xcb_xkb_get_device_info_leds_iterator)>
                >(this->_M_c, this->get());
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
            devtype(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->devtype),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->devtype,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_device_info

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_info = detail::get_device_info<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_get_device_info)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_info = detail::get_device_info<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_get_device_info_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_info<Connection>
get_device_info(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_info<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_info<Connection>
get_device_info_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_info<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_device_info_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xkb::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xkb_set_device_info_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_device_info(Parameter && ... parameter)
{
    ::xcb_xkb_set_device_info(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class set_debugging_flags
        : public cppual::generic::reply<set_debugging_flags<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xkb_set_debugging_flags_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<set_debugging_flags<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xkb_set_debugging_flags_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    set_debugging_flags(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class set_debugging_flags

} // namespace detail

namespace checked {
template<typename Connection>
using set_debugging_flags = detail::set_debugging_flags<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xkb_set_debugging_flags)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_debugging_flags = detail::set_debugging_flags<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xkb_set_debugging_flags_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_debugging_flags<Connection>
set_debugging_flags(Connection && c, Parameter && ... parameter)
{
    return reply::checked::set_debugging_flags<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_debugging_flags<Connection>
set_debugging_flags_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::set_debugging_flags<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}



































template<typename Derived, typename Connection>
class interface
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

public:
    typedef cppual::xkb::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    xkb(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    use_extension(Parameter && ... parameter) const
    -> reply::checked::use_extension<Connection>
    {
        return cppual::xkb::use_extension(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    use_extension_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::use_extension<Connection>
    {
        return cppual::xkb::use_extension_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_events_checked(Parameter && ... parameter) const
    {
        cppual::xkb::select_events_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_events(Parameter && ... parameter) const
    {
        cppual::xkb::select_events(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    bell_checked(Parameter && ... parameter) const
    {
        cppual::xkb::bell_checked(connection(),
                               std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    bell(Parameter && ... parameter) const
    {
        cppual::xkb::bell(connection(),
                       std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_state(Parameter && ... parameter) const
    -> reply::checked::get_state<Connection>
    {
        return cppual::xkb::get_state(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_state_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_state<Connection>
    {
        return cppual::xkb::get_state_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    latch_lock_state_checked(Parameter && ... parameter) const
    {
        cppual::xkb::latch_lock_state_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    latch_lock_state(Parameter && ... parameter) const
    {
        cppual::xkb::latch_lock_state(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_controls(Parameter && ... parameter) const
    -> reply::checked::get_controls<Connection>
    {
        return cppual::xkb::get_controls(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_controls_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_controls<Connection>
    {
        return cppual::xkb::get_controls_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_controls_checked(Parameter && ... parameter) const
    {
        cppual::xkb::set_controls_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_controls(Parameter && ... parameter) const
    {
        cppual::xkb::set_controls(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_map(Parameter && ... parameter) const
    -> reply::checked::get_map<Connection>
    {
        return cppual::xkb::get_map(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_map_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_map<Connection>
    {
        return cppual::xkb::get_map_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_map_checked(Parameter && ... parameter) const
    {
        cppual::xkb::set_map_checked(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_map(Parameter && ... parameter) const
    {
        cppual::xkb::set_map(connection(),
                          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_compat_map(Parameter && ... parameter) const
    -> reply::checked::get_compat_map<Connection>
    {
        return cppual::xkb::get_compat_map(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_compat_map_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_compat_map<Connection>
    {
        return cppual::xkb::get_compat_map_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_compat_map_checked(Parameter && ... parameter) const
    {
        cppual::xkb::set_compat_map_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_compat_map(Parameter && ... parameter) const
    {
        cppual::xkb::set_compat_map(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_indicator_state(Parameter && ... parameter) const
    -> reply::checked::get_indicator_state<Connection>
    {
        return cppual::xkb::get_indicator_state(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_indicator_state_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_indicator_state<Connection>
    {
        return cppual::xkb::get_indicator_state_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_indicator_map(Parameter && ... parameter) const
    -> reply::checked::get_indicator_map<Connection>
    {
        return cppual::xkb::get_indicator_map(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_indicator_map_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_indicator_map<Connection>
    {
        return cppual::xkb::get_indicator_map_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_indicator_map_checked(Parameter && ... parameter) const
    {
        cppual::xkb::set_indicator_map_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_indicator_map(Parameter && ... parameter) const
    {
        cppual::xkb::set_indicator_map(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_named_indicator(Parameter && ... parameter) const
    -> reply::checked::get_named_indicator<Connection>
    {
        return cppual::xkb::get_named_indicator(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_named_indicator_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_named_indicator<Connection>
    {
        return cppual::xkb::get_named_indicator_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_named_indicator_checked(Parameter && ... parameter) const
    {
        cppual::xkb::set_named_indicator_checked(connection(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_named_indicator(Parameter && ... parameter) const
    {
        cppual::xkb::set_named_indicator(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_names(Parameter && ... parameter) const
    -> reply::checked::get_names<Connection>
    {
        return cppual::xkb::get_names(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_names_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_names<Connection>
    {
        return cppual::xkb::get_names_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_names_checked(Parameter && ... parameter) const
    {
        cppual::xkb::set_names_checked(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_names(Parameter && ... parameter) const
    {
        cppual::xkb::set_names(connection(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    per_client_flags(Parameter && ... parameter) const
    -> reply::checked::per_client_flags<Connection>
    {
        return cppual::xkb::per_client_flags(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    per_client_flags_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::per_client_flags<Connection>
    {
        return cppual::xkb::per_client_flags_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_components(Parameter && ... parameter) const
    -> reply::checked::list_components<Connection>
    {
        return cppual::xkb::list_components(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_components_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_components<Connection>
    {
        return cppual::xkb::list_components_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_kbd_by_name(Parameter && ... parameter) const
    -> reply::checked::get_kbd_by_name<Connection>
    {
        return cppual::xkb::get_kbd_by_name(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_kbd_by_name_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_kbd_by_name<Connection>
    {
        return cppual::xkb::get_kbd_by_name_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_info(Parameter && ... parameter) const
    -> reply::checked::get_device_info<Connection>
    {
        return cppual::xkb::get_device_info(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_info<Connection>
    {
        return cppual::xkb::get_device_info_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_device_info_checked(Parameter && ... parameter) const
    {
        cppual::xkb::set_device_info_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_device_info(Parameter && ... parameter) const
    {
        cppual::xkb::set_device_info(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_debugging_flags(Parameter && ... parameter) const
    -> reply::checked::set_debugging_flags<Connection>
    {
        return cppual::xkb::set_debugging_flags(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_debugging_flags_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_debugging_flags<Connection>
    {
        return cppual::xkb::set_debugging_flags_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::xkb::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::xkb::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch (event->pad0) {

        case XCB_XKB_NEW_KEYBOARD_NOTIFY:
            handler(cppual::xkb::event::new_keyboard_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_MAP_NOTIFY:
            handler(cppual::xkb::event::map_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_STATE_NOTIFY:
            handler(cppual::xkb::event::state_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_CONTROLS_NOTIFY:
            handler(cppual::xkb::event::controls_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_INDICATOR_STATE_NOTIFY:
            handler(cppual::xkb::event::indicator_state_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_INDICATOR_MAP_NOTIFY:
            handler(cppual::xkb::event::indicator_map_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_NAMES_NOTIFY:
            handler(cppual::xkb::event::names_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_COMPAT_MAP_NOTIFY:
            handler(cppual::xkb::event::compat_map_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_BELL_NOTIFY:
            handler(cppual::xkb::event::bell_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_ACTION_MESSAGE:
            handler(cppual::xkb::event::action_message<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_ACCESS_X_NOTIFY:
            handler(cppual::xkb::event::access_x_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_XKB_EXTENSION_DEVICE_NOTIFY:
            handler(cppual::xkb::event::extension_device_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        };

        return false;
    }

protected:
    Connection _M_c;
    uint8_t _M_first_event;
}; // class dispatcher

} // namespace event

namespace error {

class dispatcher
{
public:
    typedef cppual::xkb::extension extension;

    dispatcher(uint8_t first_error)
        : _M_first_error(first_error)
    {}

    dispatcher(const cppual::xkb::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - _M_first_error) {

        case XCB_XKB_KEYBOARD: // 0
            throw cppual::xkb::error::keyboard(error);

        };
    }

protected:
    uint8_t _M_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::xkb

#endif // CPPUAL_XKB_HPP
