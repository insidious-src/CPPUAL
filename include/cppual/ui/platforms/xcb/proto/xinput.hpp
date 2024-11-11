#ifndef CPPUAL_INPUT_HPP
#define CPPUAL_INPUT_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>
#include <cppual/string.h>

#include <vector>

#include <xcb/xinput.h>

namespace cppual { namespace input {

template<typename Derived, typename Connection>
class interface;

namespace events { template<typename Connection> class dispatcher; }
namespace error  { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_input_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_input_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::input::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = events::dispatcher<Connection>;
    using error_dispatcher = cppual::input::error::dispatcher;
};

namespace events {
template<typename Connection>
class device_valuator
        : public cppual::generic::event<xcb_input_device_valuator_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_valuator_event_t> base;

    template<typename C>
    device_valuator(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_valuator(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_VALUATOR;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_VALUATOR");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_valuator
} // namespace event


namespace events {
template<typename Connection>
class device_key_press
        : public cppual::generic::event<xcb_input_device_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_press_event_t> base;

    template<typename C>
    device_key_press(C && c,
                     uint8_t first_event,
                     const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_key_press(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_KEY_PRESS;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_KEY_PRESS");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_key_press



} // namespace event


namespace events {
template<typename Connection>
class device_key_release
        : public cppual::generic::event<xcb_input_device_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_press_event_t> base;

    template<typename C>
    device_key_release(C && c,
                       uint8_t first_event,
                       const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_key_release(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_KEY_RELEASE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_KEY_RELEASE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_key_release



} // namespace event


namespace events {
template<typename Connection>
class device_button_press
        : public cppual::generic::event<xcb_input_device_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_press_event_t> base;

    template<typename C>
    device_button_press(C && c,
                        uint8_t first_event,
                        const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_button_press(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_BUTTON_PRESS;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_BUTTON_PRESS");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_button_press



} // namespace event


namespace events {
template<typename Connection>
class device_button_release
        : public cppual::generic::event<xcb_input_device_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_press_event_t> base;

    template<typename C>
    device_button_release(C && c,
                          uint8_t first_event,
                          const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_button_release(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_BUTTON_RELEASE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_BUTTON_RELEASE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_button_release



} // namespace event


namespace events {
template<typename Connection>
class device_motion_notify
        : public cppual::generic::event<xcb_input_device_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_press_event_t> base;

    template<typename C>
    device_motion_notify(C && c,
                         uint8_t first_event,
                         const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_motion_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_MOTION_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_MOTION_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_motion_notify



} // namespace event


namespace events {
template<typename Connection>
class device_focus_in
        : public cppual::generic::event<xcb_input_device_focus_in_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_focus_in_event_t> base;

    template<typename C>
    device_focus_in(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_focus_in(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_FOCUS_IN;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_FOCUS_IN");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
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
}; // class device_focus_in



} // namespace event


namespace events {
template<typename Connection>
class device_focus_out
        : public cppual::generic::event<xcb_input_device_focus_in_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_focus_in_event_t> base;

    template<typename C>
    device_focus_out(C && c,
                     uint8_t first_event,
                     const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_focus_out(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_FOCUS_OUT;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_FOCUS_OUT");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
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
}; // class device_focus_out



} // namespace event


namespace events {
template<typename Connection>
class proximity_in
        : public cppual::generic::event<xcb_input_device_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_press_event_t> base;

    template<typename C>
    proximity_in(C && c,
                 uint8_t first_event,
                 const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~proximity_in(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_PROXIMITY_IN;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_PROXIMITY_IN");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class proximity_in



} // namespace event


namespace events {
template<typename Connection>
class proximity_out
        : public cppual::generic::event<xcb_input_device_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_press_event_t> base;

    template<typename C>
    proximity_out(C && c,
                  uint8_t first_event,
                  const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~proximity_out(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_PROXIMITY_OUT;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_PROXIMITY_OUT");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class proximity_out



} // namespace event


namespace events {
template<typename Connection>
class device_state_notify
        : public cppual::generic::event<xcb_input_device_state_notify_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_state_notify_event_t> base;

    template<typename C>
    device_state_notify(C && c,
                        uint8_t first_event,
                        const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_state_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_STATE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_STATE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_state_notify
} // namespace event


namespace events {
template<typename Connection>
class device_mapping_notify
        : public cppual::generic::event<xcb_input_device_mapping_notify_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_mapping_notify_event_t> base;

    template<typename C>
    device_mapping_notify(C && c,
                          uint8_t first_event,
                          const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_mapping_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_MAPPING_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_MAPPING_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_mapping_notify
} // namespace event


namespace events {
template<typename Connection>
class change_device_notify
        : public cppual::generic::event<xcb_input_change_device_notify_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_change_device_notify_event_t> base;

    template<typename C>
    change_device_notify(C && c,
                         uint8_t first_event,
                         const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~change_device_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_CHANGE_DEVICE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_CHANGE_DEVICE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class change_device_notify
} // namespace event


namespace events {
template<typename Connection>
class device_key_state_notify
        : public cppual::generic::event<xcb_input_device_key_state_notify_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_key_state_notify_event_t> base;

    template<typename C>
    device_key_state_notify(C && c,
                            uint8_t first_event,
                            const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_key_state_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_KEY_STATE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_KEY_STATE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_key_state_notify
} // namespace event


namespace events {
template<typename Connection>
class device_button_state_notify
        : public cppual::generic::event<xcb_input_device_button_state_notify_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_button_state_notify_event_t> base;

    template<typename C>
    device_button_state_notify(C && c,
                               uint8_t first_event,
                               const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_button_state_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_BUTTON_STATE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_BUTTON_STATE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_button_state_notify
} // namespace event


namespace events {
template<typename Connection>
class device_presence_notify
        : public cppual::generic::event<xcb_input_device_presence_notify_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_presence_notify_event_t> base;

    template<typename C>
    device_presence_notify(C && c,
                           uint8_t first_event,
                           const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_presence_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_PRESENCE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_PRESENCE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_presence_notify
} // namespace event


namespace events {
template<typename Connection>
class device_property_notify
        : public cppual::generic::event<xcb_input_device_property_notify_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_property_notify_event_t> base;

    template<typename C>
    device_property_notify(C && c,
                           uint8_t first_event,
                           const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_property_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_PROPERTY_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_PROPERTY_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    property(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->property),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->property,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_property_notify



} // namespace event


namespace events {
template<typename Connection>
class device_changed
        : public cppual::generic::event<xcb_input_device_changed_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_device_changed_event_t> base;

    template<typename C>
    device_changed(C && c,
                   uint8_t first_event,
                   const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~device_changed(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_CHANGED;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_CHANGED");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class device_changed
} // namespace event


namespace events {
template<typename Connection>
class key_press
        : public cppual::generic::event<xcb_input_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_key_press_event_t> base;

    template<typename C>
    key_press(C && c,
              uint8_t first_event,
              const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~key_press(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_KEY_PRESS;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_KEY_PRESS");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class key_press



} // namespace event


namespace events {
template<typename Connection>
class key_release
        : public cppual::generic::event<xcb_input_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_key_press_event_t> base;

    template<typename C>
    key_release(C && c,
                uint8_t first_event,
                const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~key_release(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_KEY_RELEASE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_KEY_RELEASE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class key_release



} // namespace event


namespace events {
template<typename Connection>
class button_press
        : public cppual::generic::event<xcb_input_button_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_button_press_event_t> base;

    template<typename C>
    button_press(C && c,
                 uint8_t first_event,
                 const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~button_press(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_BUTTON_PRESS;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_BUTTON_PRESS");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class button_press



} // namespace event


namespace events {
template<typename Connection>
class button_release
        : public cppual::generic::event<xcb_input_button_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_button_press_event_t> base;

    template<typename C>
    button_release(C && c,
                   uint8_t first_event,
                   const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~button_release(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_BUTTON_RELEASE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_BUTTON_RELEASE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class button_release



} // namespace event


namespace events {
template<typename Connection>
class motion
        : public cppual::generic::event<xcb_input_button_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_button_press_event_t> base;

    template<typename C>
    motion(C && c,
           uint8_t first_event,
           const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~motion(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_MOTION;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_MOTION");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class motion



} // namespace event


namespace events {
template<typename Connection>
class enter
        : public cppual::generic::event<xcb_input_enter_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_enter_event_t> base;

    template<typename C>
    enter(C && c,
          uint8_t first_event,
          const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~enter(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_ENTER;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_ENTER");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class enter



} // namespace event


namespace events {
template<typename Connection>
class leave
        : public cppual::generic::event<xcb_input_enter_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_enter_event_t> base;

    template<typename C>
    leave(C && c,
          uint8_t first_event,
          const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~leave(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_LEAVE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_LEAVE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class leave



} // namespace event


namespace events {
template<typename Connection>
class focus_in
        : public cppual::generic::event<xcb_input_enter_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_enter_event_t> base;

    template<typename C>
    focus_in(C && c,
             uint8_t first_event,
             const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~focus_in(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_FOCUS_IN;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_FOCUS_IN");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class focus_in



} // namespace event


namespace events {
template<typename Connection>
class focus_out
        : public cppual::generic::event<xcb_input_enter_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_enter_event_t> base;

    template<typename C>
    focus_out(C && c,
              uint8_t first_event,
              const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~focus_out(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_FOCUS_OUT;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_FOCUS_OUT");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class focus_out



} // namespace event


namespace events {
template<typename Connection>
class hierarchy
        : public cppual::generic::event<xcb_input_hierarchy_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_hierarchy_event_t> base;

    template<typename C>
    hierarchy(C && c,
              uint8_t first_event,
              const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~hierarchy(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_HIERARCHY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_HIERARCHY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class hierarchy
} // namespace event


namespace events {
template<typename Connection>
class property
        : public cppual::generic::event<xcb_input_property_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_property_event_t> base;

    template<typename C>
    property(C && c,
             uint8_t first_event,
             const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~property(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_PROPERTY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_PROPERTY");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    property_(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->property),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->property,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class property



} // namespace event


namespace events {
template<typename Connection>
class raw_key_press
        : public cppual::generic::event<xcb_input_raw_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_key_press_event_t> base;

    template<typename C>
    raw_key_press(C && c,
                  uint8_t first_event,
                  const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_key_press(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_KEY_PRESS;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_KEY_PRESS");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_key_press
} // namespace event


namespace events {
template<typename Connection>
class raw_key_release
        : public cppual::generic::event<xcb_input_raw_key_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_key_press_event_t> base;

    template<typename C>
    raw_key_release(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_key_release(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_KEY_RELEASE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_KEY_RELEASE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_key_release
} // namespace event


namespace events {
template<typename Connection>
class raw_button_press
        : public cppual::generic::event<xcb_input_raw_button_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_button_press_event_t> base;

    template<typename C>
    raw_button_press(C && c,
                     uint8_t first_event,
                     const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_button_press(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_BUTTON_PRESS;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_BUTTON_PRESS");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_button_press
} // namespace event


namespace events {
template<typename Connection>
class raw_button_release
        : public cppual::generic::event<xcb_input_raw_button_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_button_press_event_t> base;

    template<typename C>
    raw_button_release(C && c,
                       uint8_t first_event,
                       const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_button_release(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_BUTTON_RELEASE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_BUTTON_RELEASE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_button_release
} // namespace event


namespace events {
template<typename Connection>
class raw_motion
        : public cppual::generic::event<xcb_input_raw_button_press_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_button_press_event_t> base;

    template<typename C>
    raw_motion(C && c,
               uint8_t first_event,
               const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_motion(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_MOTION;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_MOTION");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_motion
} // namespace event


namespace events {
template<typename Connection>
class touch_begin
        : public cppual::generic::event<xcb_input_touch_begin_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_touch_begin_event_t> base;

    template<typename C>
    touch_begin(C && c,
                uint8_t first_event,
                const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~touch_begin(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_TOUCH_BEGIN;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_TOUCH_BEGIN");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class touch_begin



} // namespace event


namespace events {
template<typename Connection>
class touch_update
        : public cppual::generic::event<xcb_input_touch_begin_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_touch_begin_event_t> base;

    template<typename C>
    touch_update(C && c,
                 uint8_t first_event,
                 const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~touch_update(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_TOUCH_UPDATE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_TOUCH_UPDATE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class touch_update



} // namespace event


namespace events {
template<typename Connection>
class touch_end
        : public cppual::generic::event<xcb_input_touch_begin_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_touch_begin_event_t> base;

    template<typename C>
    touch_end(C && c,
              uint8_t first_event,
              const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~touch_end(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_TOUCH_END;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_TOUCH_END");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class touch_end



} // namespace event


namespace events {
template<typename Connection>
class touch_ownership
        : public cppual::generic::event<xcb_input_touch_ownership_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_touch_ownership_event_t> base;

    template<typename C>
    touch_ownership(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~touch_ownership(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_TOUCH_OWNERSHIP;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_TOUCH_OWNERSHIP");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    child(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->child,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class touch_ownership



} // namespace event


namespace events {
template<typename Connection>
class raw_touch_begin
        : public cppual::generic::event<xcb_input_raw_touch_begin_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_touch_begin_event_t> base;

    template<typename C>
    raw_touch_begin(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_touch_begin(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_TOUCH_BEGIN;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_TOUCH_BEGIN");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_touch_begin
} // namespace event


namespace events {
template<typename Connection>
class raw_touch_update
        : public cppual::generic::event<xcb_input_raw_touch_begin_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_touch_begin_event_t> base;

    template<typename C>
    raw_touch_update(C && c,
                     uint8_t first_event,
                     const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_touch_update(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_TOUCH_UPDATE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_TOUCH_UPDATE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_touch_update
} // namespace event


namespace events {
template<typename Connection>
class raw_touch_end
        : public cppual::generic::event<xcb_input_raw_touch_begin_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_raw_touch_begin_event_t> base;

    template<typename C>
    raw_touch_end(C && c,
                  uint8_t first_event,
                  const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~raw_touch_end(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_RAW_TOUCH_END;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_RAW_TOUCH_END");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }
protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class raw_touch_end
} // namespace event


namespace events {
template<typename Connection>
class barrier_hit
        : public cppual::generic::event<xcb_input_barrier_hit_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_barrier_hit_event_t> base;

    template<typename C>
    barrier_hit(C && c,
                uint8_t first_event,
                const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~barrier_hit(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_BARRIER_HIT;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_BARRIER_HIT");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_xfixes_barrier_t, typename ... Parameter>
    ReturnType
    barrier(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->barrier),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->barrier,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class barrier_hit



} // namespace event


namespace events {
template<typename Connection>
class barrier_leave
        : public cppual::generic::event<xcb_input_barrier_hit_event_t>
{
public:
    typedef cppual::input::extension extension;

    typedef cppual::generic::event<xcb_input_barrier_hit_event_t> base;

    template<typename C>
    barrier_leave(C && c,
                  uint8_t first_event,
                  const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~barrier_leave(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_BARRIER_LEAVE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_INPUT_BARRIER_LEAVE");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    event(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->event),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->event,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_xfixes_barrier_t, typename ... Parameter>
    ReturnType
    barrier(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->barrier),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->barrier,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class barrier_leave



} // namespace event



namespace error {
class device
        : public cppual::generic::error<device,
        ::xcb_input_device_error_t>
{
public:
    using cppual::generic::error<device, ::xcb_input_device_error_t>::error;

    virtual ~device(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE");
    }

protected:
    uint8_t _M_first_error;
}; // class device
} // namespace error


namespace error {
class event
        : public cppual::generic::error<event,
        ::xcb_input_event_error_t>
{
public:
    using cppual::generic::error<event, ::xcb_input_event_error_t>::error;

    virtual ~event(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_EVENT;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_INPUT_EVENT");
    }

protected:
    uint8_t _M_first_error;
}; // class event
} // namespace error


namespace error {
class mode
        : public cppual::generic::error<mode,
        ::xcb_input_mode_error_t>
{
public:
    using cppual::generic::error<mode, ::xcb_input_mode_error_t>::error;

    virtual ~mode(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_MODE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_INPUT_MODE");
    }

protected:
    uint8_t _M_first_error;
}; // class mode
} // namespace error


namespace error {
class device_busy
        : public cppual::generic::error<device_busy,
        ::xcb_input_device_busy_error_t>
{
public:
    using cppual::generic::error<device_busy, ::xcb_input_device_busy_error_t>::error;

    virtual ~device_busy(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_DEVICE_BUSY;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_INPUT_DEVICE_BUSY");
    }

protected:
    uint8_t _M_first_error;
}; // class device_busy
} // namespace error


namespace error {
class _class
        : public cppual::generic::error<_class,
        ::xcb_input_class_error_t>
{
public:
    using cppual::generic::error<_class, ::xcb_input_class_error_t>::error;

    virtual ~_class(void) {}

    static uint8_t opcode(void)
    {
        return XCB_INPUT_CLASS;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::input::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_INPUT_CLASS");
    }

protected:
    uint8_t _M_first_error;
}; // class _class
} // namespace error


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_extension_version
        : public cppual::generic::reply<get_extension_version<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_extension_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_extension_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_extension_version_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_extension_version(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
            ::xcb_input_get_extension_version_cookie_t
            cookie(xcb_connection_t * const c, uint16_t name_len, const char * name)
    {
        return base::cookie(c, name_len, name);
    }

    static
            ::xcb_input_get_extension_version_cookie_t
            cookie(xcb_connection_t * const c, const string & name)
    {
        return base::cookie(c, static_cast<uint16_t>(name.length()), name.c_str());
    }
}; // class get_extension_version

} // namespace detail

namespace checked {
template<typename Connection>
using get_extension_version = detail::get_extension_version<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_extension_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_extension_version = detail::get_extension_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_extension_version_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_extension_version<Connection>
get_extension_version(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_extension_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_extension_version<Connection>
get_extension_version_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_extension_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_input_devices
        : public cppual::generic::reply<list_input_devices<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_list_input_devices_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_input_devices<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_list_input_devices_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_input_devices(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_list_input_devices_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_device_info_t,
            SIGNATURE(xcb_input_list_input_devices_devices),
            SIGNATURE(xcb_input_list_input_devices_devices_length)>
            >
            devices(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_list_input_devices_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_device_info_t,
                SIGNATURE(xcb_input_list_input_devices_devices),
                SIGNATURE(xcb_input_list_input_devices_devices_length)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_input_list_input_devices_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_input_info_t,
            SIGNATURE(xcb_input_input_info_next),
            SIGNATURE(xcb_input_input_info_sizeof),
            SIGNATURE(xcb_input_list_input_devices_infos_iterator)>
            >
            infos(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_list_input_devices_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_input_info_t,
                SIGNATURE(xcb_input_input_info_next),
                SIGNATURE(xcb_input_input_info_sizeof),
                SIGNATURE(xcb_input_list_input_devices_infos_iterator)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_input_list_input_devices_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_str_t,
            SIGNATURE(xcb_str_next),
            SIGNATURE(xcb_str_sizeof),
            SIGNATURE(xcb_input_list_input_devices_names_iterator)>
            >
            names(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_list_input_devices_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_str_t,
                SIGNATURE(xcb_str_next),
                SIGNATURE(xcb_str_sizeof),
                SIGNATURE(xcb_input_list_input_devices_names_iterator)>
                >(this->_M_c, this->get());
    }
}; // class list_input_devices

} // namespace detail

namespace checked {
template<typename Connection>
using list_input_devices = detail::list_input_devices<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_list_input_devices)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_input_devices = detail::list_input_devices<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_list_input_devices_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_input_devices<Connection>
list_input_devices(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_input_devices<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_input_devices<Connection>
list_input_devices_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_input_devices<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class open_device
        : public cppual::generic::reply<open_device<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_open_device_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<open_device<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_open_device_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    open_device(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_open_device_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_input_class_info_t,
            SIGNATURE(xcb_input_open_device_class_info),
            SIGNATURE(xcb_input_open_device_class_info_length)>
            >
            class_info(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_open_device_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_input_class_info_t,
                SIGNATURE(xcb_input_open_device_class_info),
                SIGNATURE(xcb_input_open_device_class_info_length)>
                >(this->_M_c, this->get());
    }
}; // class open_device

} // namespace detail

namespace checked {
template<typename Connection>
using open_device = detail::open_device<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_open_device)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using open_device = detail::open_device<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_open_device_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::open_device<Connection>
open_device(Connection && c, Parameter && ... parameter)
{
    return reply::checked::open_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::open_device<Connection>
open_device_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::open_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
close_device_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_close_device_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
close_device(Parameter && ... parameter)
{
    ::xcb_input_close_device(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class set_device_mode
        : public cppual::generic::reply<set_device_mode<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_set_device_mode_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<set_device_mode<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_set_device_mode_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    set_device_mode(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class set_device_mode

} // namespace detail

namespace checked {
template<typename Connection>
using set_device_mode = detail::set_device_mode<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_set_device_mode)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_device_mode = detail::set_device_mode<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_set_device_mode_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_device_mode<Connection>
set_device_mode(Connection && c, Parameter && ... parameter)
{
    return reply::checked::set_device_mode<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_device_mode<Connection>
set_device_mode_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::set_device_mode<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_extension_event_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_select_extension_event_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_extension_event(Parameter && ... parameter)
{
    ::xcb_input_select_extension_event(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_selected_extension_events
        : public cppual::generic::reply<get_selected_extension_events<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_selected_extension_events_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_selected_extension_events<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_selected_extension_events_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_selected_extension_events(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_get_selected_extension_events_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_event_class_t,
            SIGNATURE(xcb_input_get_selected_extension_events_this_classes),
            SIGNATURE(xcb_input_get_selected_extension_events_this_classes_length)>
            >
            this_classes(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_selected_extension_events_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_event_class_t,
                SIGNATURE(xcb_input_get_selected_extension_events_this_classes),
                SIGNATURE(xcb_input_get_selected_extension_events_this_classes_length)>
                >(this->_M_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_input_get_selected_extension_events_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_event_class_t,
            SIGNATURE(xcb_input_get_selected_extension_events_all_classes),
            SIGNATURE(xcb_input_get_selected_extension_events_all_classes_length)>
            >
            all_classes(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_selected_extension_events_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_event_class_t,
                SIGNATURE(xcb_input_get_selected_extension_events_all_classes),
                SIGNATURE(xcb_input_get_selected_extension_events_all_classes_length)>
                >(this->_M_c, this->get());
    }
}; // class get_selected_extension_events

} // namespace detail

namespace checked {
template<typename Connection>
using get_selected_extension_events = detail::get_selected_extension_events<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_selected_extension_events)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_selected_extension_events = detail::get_selected_extension_events<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_selected_extension_events_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_selected_extension_events<Connection>
get_selected_extension_events(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_selected_extension_events<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_selected_extension_events<Connection>
get_selected_extension_events_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_selected_extension_events<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_device_dont_propagate_list_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_change_device_dont_propagate_list_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_device_dont_propagate_list(Parameter && ... parameter)
{
    ::xcb_input_change_device_dont_propagate_list(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_dont_propagate_list
        : public cppual::generic::reply<get_device_dont_propagate_list<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_dont_propagate_list_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_dont_propagate_list<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_dont_propagate_list_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_dont_propagate_list(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_get_device_dont_propagate_list_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_event_class_t,
            SIGNATURE(xcb_input_get_device_dont_propagate_list_classes),
            SIGNATURE(xcb_input_get_device_dont_propagate_list_classes_length)>
            >
            classes(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_device_dont_propagate_list_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_event_class_t,
                SIGNATURE(xcb_input_get_device_dont_propagate_list_classes),
                SIGNATURE(xcb_input_get_device_dont_propagate_list_classes_length)>
                >(this->_M_c, this->get());
    }
}; // class get_device_dont_propagate_list

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_dont_propagate_list = detail::get_device_dont_propagate_list<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_dont_propagate_list)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_dont_propagate_list = detail::get_device_dont_propagate_list<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_dont_propagate_list_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_dont_propagate_list<Connection>
get_device_dont_propagate_list(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_dont_propagate_list<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_dont_propagate_list<Connection>
get_device_dont_propagate_list_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_dont_propagate_list<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_motion_events
        : public cppual::generic::reply<get_device_motion_events<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_motion_events_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_motion_events<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_motion_events_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_motion_events(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
            ::xcb_input_get_device_motion_events_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_timestamp_t start, ::xcb_timestamp_t stop, uint8_t device_id)
    {
        return base::cookie(c, start, stop, device_id);
    }

    static
            ::xcb_input_get_device_motion_events_cookie_t
            cookie(xcb_connection_t * const c, uint8_t device_id, ::xcb_timestamp_t start = XCB_TIME_CURRENT_TIME, ::xcb_timestamp_t stop = XCB_TIME_CURRENT_TIME)
    {
        return base::cookie(c, start, stop, device_id);
    }

    cppual::generic::list<Connection,
            ::xcb_input_get_device_motion_events_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_device_time_coord_t,
            SIGNATURE(xcb_input_device_time_coord_next),
            SIGNATURE(xcb_input_device_time_coord_sizeof),
            SIGNATURE(xcb_input_get_device_motion_events_events_iterator)>
            >
            events(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_device_motion_events_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_device_time_coord_t,
                SIGNATURE(xcb_input_device_time_coord_next),
                SIGNATURE(xcb_input_device_time_coord_sizeof),
                SIGNATURE(xcb_input_get_device_motion_events_events_iterator)>
                >(this->_M_c, this->get());
    }
}; // class get_device_motion_events

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_motion_events = detail::get_device_motion_events<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_motion_events)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_motion_events = detail::get_device_motion_events<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_motion_events_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_motion_events<Connection>
get_device_motion_events(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_motion_events<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_motion_events<Connection>
get_device_motion_events_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_motion_events<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class change_keyboard_device
        : public cppual::generic::reply<change_keyboard_device<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_change_keyboard_device_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<change_keyboard_device<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_change_keyboard_device_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    change_keyboard_device(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class change_keyboard_device

} // namespace detail

namespace checked {
template<typename Connection>
using change_keyboard_device = detail::change_keyboard_device<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_change_keyboard_device)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using change_keyboard_device = detail::change_keyboard_device<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_change_keyboard_device_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::change_keyboard_device<Connection>
change_keyboard_device(Connection && c, Parameter && ... parameter)
{
    return reply::checked::change_keyboard_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::change_keyboard_device<Connection>
change_keyboard_device_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::change_keyboard_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class change_pointer_device
        : public cppual::generic::reply<change_pointer_device<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_change_pointer_device_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<change_pointer_device<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_change_pointer_device_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    change_pointer_device(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class change_pointer_device

} // namespace detail

namespace checked {
template<typename Connection>
using change_pointer_device = detail::change_pointer_device<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_change_pointer_device)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using change_pointer_device = detail::change_pointer_device<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_change_pointer_device_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::change_pointer_device<Connection>
change_pointer_device(Connection && c, Parameter && ... parameter)
{
    return reply::checked::change_pointer_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::change_pointer_device<Connection>
change_pointer_device_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::change_pointer_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class grab_device
        : public cppual::generic::reply<grab_device<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_grab_device_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<grab_device<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_grab_device_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    grab_device(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
            ::xcb_input_grab_device_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_window_t grab_window, ::xcb_timestamp_t time, uint16_t num_classes, uint8_t this_device_mode, uint8_t other_device_mode, uint8_t owner_events, uint8_t device_id, const ::xcb_input_event_class_t * classes)
    {
        return base::cookie(c, grab_window, time, num_classes, this_device_mode, other_device_mode, owner_events, device_id, classes);
    }

    static
            ::xcb_input_grab_device_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_window_t grab_window, uint16_t num_classes, uint8_t this_device_mode, uint8_t other_device_mode, uint8_t owner_events, uint8_t device_id, const ::xcb_input_event_class_t * classes, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
    {
        return base::cookie(c, grab_window, time, num_classes, this_device_mode, other_device_mode, owner_events, device_id, classes);
    }
}; // class grab_device

} // namespace detail

namespace checked {
template<typename Connection>
using grab_device = detail::grab_device<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_grab_device)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using grab_device = detail::grab_device<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_grab_device_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::grab_device<Connection>
grab_device(Connection && c, Parameter && ... parameter)
{
    return reply::checked::grab_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::grab_device<Connection>
grab_device_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::grab_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
ungrab_device_checked(Connection && c, ::xcb_timestamp_t time, uint8_t device_id)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_ungrab_device_checked(std::forward<Connection>(c), time, device_id));
}

template<typename Connection>
void
ungrab_device(Connection && c, ::xcb_timestamp_t time, uint8_t device_id)
{
    ::xcb_input_ungrab_device(std::forward<Connection>(c), time, device_id);
}

template<typename Connection>
void
ungrab_device_checked(Connection && c, uint8_t device_id, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_ungrab_device_checked(std::forward<Connection>(c), time, device_id));
}

template<typename Connection>
void
ungrab_device(Connection && c, uint8_t device_id, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    ::xcb_input_ungrab_device(std::forward<Connection>(c), time, device_id);
}

template<typename Connection, typename ... Parameter>
void
grab_device_key_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_grab_device_key_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
grab_device_key(Parameter && ... parameter)
{
    ::xcb_input_grab_device_key(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
ungrab_device_key_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_ungrab_device_key_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
ungrab_device_key(Parameter && ... parameter)
{
    ::xcb_input_ungrab_device_key(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
grab_device_button_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_grab_device_button_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
grab_device_button(Parameter && ... parameter)
{
    ::xcb_input_grab_device_button(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
ungrab_device_button_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_ungrab_device_button_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
ungrab_device_button(Parameter && ... parameter)
{
    ::xcb_input_ungrab_device_button(std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
allow_device_events_checked(Connection && c, ::xcb_timestamp_t time, uint8_t mode, uint8_t device_id)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_allow_device_events_checked(std::forward<Connection>(c), time, mode, device_id));
}

template<typename Connection>
void
allow_device_events(Connection && c, ::xcb_timestamp_t time, uint8_t mode, uint8_t device_id)
{
    ::xcb_input_allow_device_events(std::forward<Connection>(c), time, mode, device_id);
}

template<typename Connection>
void
allow_device_events_checked(Connection && c, uint8_t mode, uint8_t device_id, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_allow_device_events_checked(std::forward<Connection>(c), time, mode, device_id));
}

template<typename Connection>
void
allow_device_events(Connection && c, uint8_t mode, uint8_t device_id, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    ::xcb_input_allow_device_events(std::forward<Connection>(c), time, mode, device_id);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_focus
        : public cppual::generic::reply<get_device_focus<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_focus_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_focus<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_focus_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_focus(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            focus(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->focus),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->focus,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_device_focus

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_focus = detail::get_device_focus<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_focus)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_focus = detail::get_device_focus<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_focus_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_focus<Connection>
get_device_focus(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_focus<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_focus<Connection>
get_device_focus_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_focus<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_device_focus_checked(Connection && c, ::xcb_window_t focus, ::xcb_timestamp_t time, uint8_t revert_to, uint8_t device_id)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_set_device_focus_checked(std::forward<Connection>(c), focus, time, revert_to, device_id));
}

template<typename Connection>
void
set_device_focus(Connection && c, ::xcb_window_t focus, ::xcb_timestamp_t time, uint8_t revert_to, uint8_t device_id)
{
    ::xcb_input_set_device_focus(std::forward<Connection>(c), focus, time, revert_to, device_id);
}

template<typename Connection>
void
set_device_focus_checked(Connection && c, ::xcb_window_t focus, uint8_t revert_to, uint8_t device_id, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_set_device_focus_checked(std::forward<Connection>(c), focus, time, revert_to, device_id));
}

template<typename Connection>
void
set_device_focus(Connection && c, ::xcb_window_t focus, uint8_t revert_to, uint8_t device_id, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    ::xcb_input_set_device_focus(std::forward<Connection>(c), focus, time, revert_to, device_id);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_feedback_control
        : public cppual::generic::reply<get_feedback_control<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_feedback_control_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_feedback_control<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_feedback_control_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_feedback_control(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_get_feedback_control_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_feedback_state_t,
            SIGNATURE(xcb_input_feedback_state_next),
            SIGNATURE(xcb_input_feedback_state_sizeof),
            SIGNATURE(xcb_input_get_feedback_control_feedbacks_iterator)>
            >
            feedbacks(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_feedback_control_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_feedback_state_t,
                SIGNATURE(xcb_input_feedback_state_next),
                SIGNATURE(xcb_input_feedback_state_sizeof),
                SIGNATURE(xcb_input_get_feedback_control_feedbacks_iterator)>
                >(this->_M_c, this->get());
    }
}; // class get_feedback_control

} // namespace detail

namespace checked {
template<typename Connection>
using get_feedback_control = detail::get_feedback_control<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_feedback_control)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_feedback_control = detail::get_feedback_control<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_feedback_control_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_feedback_control<Connection>
get_feedback_control(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_feedback_control<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_feedback_control<Connection>
get_feedback_control_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_feedback_control<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_feedback_control_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_change_feedback_control_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_feedback_control(Parameter && ... parameter)
{
    ::xcb_input_change_feedback_control(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_key_mapping
        : public cppual::generic::reply<get_device_key_mapping<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_key_mapping_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_key_mapping<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_key_mapping_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_key_mapping(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_get_device_key_mapping_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_keysym_t,
            SIGNATURE(xcb_input_get_device_key_mapping_keysyms),
            SIGNATURE(xcb_input_get_device_key_mapping_keysyms_length)>
            >
            keysyms(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_device_key_mapping_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_keysym_t,
                SIGNATURE(xcb_input_get_device_key_mapping_keysyms),
                SIGNATURE(xcb_input_get_device_key_mapping_keysyms_length)>
                >(this->_M_c, this->get());
    }
}; // class get_device_key_mapping

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_key_mapping = detail::get_device_key_mapping<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_key_mapping)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_key_mapping = detail::get_device_key_mapping<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_key_mapping_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_key_mapping<Connection>
get_device_key_mapping(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_key_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_key_mapping<Connection>
get_device_key_mapping_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_key_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_device_key_mapping_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_change_device_key_mapping_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_device_key_mapping(Parameter && ... parameter)
{
    ::xcb_input_change_device_key_mapping(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_modifier_mapping
        : public cppual::generic::reply<get_device_modifier_mapping<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_modifier_mapping_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_modifier_mapping<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_modifier_mapping_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_modifier_mapping(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_get_device_modifier_mapping_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_input_get_device_modifier_mapping_keymaps),
            SIGNATURE(xcb_input_get_device_modifier_mapping_keymaps_length)>
            >
            keymaps(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_device_modifier_mapping_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_input_get_device_modifier_mapping_keymaps),
                SIGNATURE(xcb_input_get_device_modifier_mapping_keymaps_length)>
                >(this->_M_c, this->get());
    }
}; // class get_device_modifier_mapping

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_modifier_mapping = detail::get_device_modifier_mapping<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_modifier_mapping)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_modifier_mapping = detail::get_device_modifier_mapping<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_modifier_mapping_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_modifier_mapping<Connection>
get_device_modifier_mapping(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_modifier_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_modifier_mapping<Connection>
get_device_modifier_mapping_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_modifier_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class set_device_modifier_mapping
        : public cppual::generic::reply<set_device_modifier_mapping<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_set_device_modifier_mapping_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<set_device_modifier_mapping<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_set_device_modifier_mapping_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    set_device_modifier_mapping(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class set_device_modifier_mapping

} // namespace detail

namespace checked {
template<typename Connection>
using set_device_modifier_mapping = detail::set_device_modifier_mapping<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_set_device_modifier_mapping)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_device_modifier_mapping = detail::set_device_modifier_mapping<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_set_device_modifier_mapping_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_device_modifier_mapping<Connection>
set_device_modifier_mapping(Connection && c, Parameter && ... parameter)
{
    return reply::checked::set_device_modifier_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_device_modifier_mapping<Connection>
set_device_modifier_mapping_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::set_device_modifier_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_button_mapping
        : public cppual::generic::reply<get_device_button_mapping<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_button_mapping_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_button_mapping<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_button_mapping_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_button_mapping(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_get_device_button_mapping_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_input_get_device_button_mapping_map),
            SIGNATURE(xcb_input_get_device_button_mapping_map_length)>
            >
            map(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_get_device_button_mapping_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_input_get_device_button_mapping_map),
                SIGNATURE(xcb_input_get_device_button_mapping_map_length)>
                >(this->_M_c, this->get());
    }
}; // class get_device_button_mapping

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_button_mapping = detail::get_device_button_mapping<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_button_mapping)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_button_mapping = detail::get_device_button_mapping<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_button_mapping_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_button_mapping<Connection>
get_device_button_mapping(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_button_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_button_mapping<Connection>
get_device_button_mapping_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_button_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class set_device_button_mapping
        : public cppual::generic::reply<set_device_button_mapping<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_set_device_button_mapping_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<set_device_button_mapping<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_set_device_button_mapping_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    set_device_button_mapping(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class set_device_button_mapping

} // namespace detail

namespace checked {
template<typename Connection>
using set_device_button_mapping = detail::set_device_button_mapping<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_set_device_button_mapping)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_device_button_mapping = detail::set_device_button_mapping<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_set_device_button_mapping_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_device_button_mapping<Connection>
set_device_button_mapping(Connection && c, Parameter && ... parameter)
{
    return reply::checked::set_device_button_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_device_button_mapping<Connection>
set_device_button_mapping_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::set_device_button_mapping<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_device_state
        : public cppual::generic::reply<query_device_state<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_query_device_state_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_device_state<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_query_device_state_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_device_state(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_query_device_state_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_input_state_t,
            SIGNATURE(xcb_input_input_state_next),
            SIGNATURE(xcb_input_input_state_sizeof),
            SIGNATURE(xcb_input_query_device_state_classes_iterator)>
            >
            classes(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_query_device_state_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_input_state_t,
                SIGNATURE(xcb_input_input_state_next),
                SIGNATURE(xcb_input_input_state_sizeof),
                SIGNATURE(xcb_input_query_device_state_classes_iterator)>
                >(this->_M_c, this->get());
    }
}; // class query_device_state

} // namespace detail

namespace checked {
template<typename Connection>
using query_device_state = detail::query_device_state<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_query_device_state)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_device_state = detail::query_device_state<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_query_device_state_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_device_state<Connection>
query_device_state(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_device_state<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_device_state<Connection>
query_device_state_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_device_state<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
device_bell_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_device_bell_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
device_bell(Parameter && ... parameter)
{
    ::xcb_input_device_bell(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class set_device_valuators
        : public cppual::generic::reply<set_device_valuators<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_set_device_valuators_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<set_device_valuators<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_set_device_valuators_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    set_device_valuators(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class set_device_valuators

} // namespace detail

namespace checked {
template<typename Connection>
using set_device_valuators = detail::set_device_valuators<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_set_device_valuators)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_device_valuators = detail::set_device_valuators<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_set_device_valuators_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_device_valuators<Connection>
set_device_valuators(Connection && c, Parameter && ... parameter)
{
    return reply::checked::set_device_valuators<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_device_valuators<Connection>
set_device_valuators_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::set_device_valuators<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_control
        : public cppual::generic::reply<get_device_control<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_control_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_control<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_control_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_control(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_device_control

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_control = detail::get_device_control<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_control)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_control = detail::get_device_control<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_control_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_control<Connection>
get_device_control(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_control<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_control<Connection>
get_device_control_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_control<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class change_device_control
        : public cppual::generic::reply<change_device_control<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_change_device_control_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<change_device_control<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_change_device_control_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    change_device_control(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class change_device_control

} // namespace detail

namespace checked {
template<typename Connection>
using change_device_control = detail::change_device_control<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_change_device_control)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using change_device_control = detail::change_device_control<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_change_device_control_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::change_device_control<Connection>
change_device_control(Connection && c, Parameter && ... parameter)
{
    return reply::checked::change_device_control<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::change_device_control<Connection>
change_device_control_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::change_device_control<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_device_properties
        : public cppual::generic::reply<list_device_properties<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_list_device_properties_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_device_properties<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_list_device_properties_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_device_properties(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename Atoms = ::xcb_atom_t>
    cppual::generic::list<Connection,
            ::xcb_input_list_device_properties_reply_t,
            cppual::generic::iterator<Connection,
            Atoms,
            SIGNATURE(xcb_input_list_device_properties_atoms),
            SIGNATURE(xcb_input_list_device_properties_atoms_length)>
            >
            atoms(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_list_device_properties_reply_t,
                cppual::generic::iterator<Connection,
                Atoms,
                SIGNATURE(xcb_input_list_device_properties_atoms),
                SIGNATURE(xcb_input_list_device_properties_atoms_length)>
                >(this->_M_c, this->get());
    }
}; // class list_device_properties

} // namespace detail

namespace checked {
template<typename Connection>
using list_device_properties = detail::list_device_properties<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_list_device_properties)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_device_properties = detail::list_device_properties<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_list_device_properties_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_device_properties<Connection>
list_device_properties(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_device_properties<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_device_properties<Connection>
list_device_properties_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_device_properties<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_device_property_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_change_device_property_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_device_property(Parameter && ... parameter)
{
    ::xcb_input_change_device_property(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_device_property_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_delete_device_property_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_device_property(Parameter && ... parameter)
{
    ::xcb_input_delete_device_property(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_property
        : public cppual::generic::reply<get_device_property<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_get_device_property_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_property<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_get_device_property_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_property(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
            type(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->type),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->type,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_device_property

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_property = detail::get_device_property<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_get_device_property)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_property = detail::get_device_property<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_get_device_property_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_property<Connection>
get_device_property(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_property<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_property<Connection>
get_device_property_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_property<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_query_pointer
        : public cppual::generic::reply<xi_query_pointer<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_query_pointer_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_query_pointer<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_query_pointer_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_query_pointer(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_xi_query_pointer_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_input_xi_query_pointer_buttons),
            SIGNATURE(xcb_input_xi_query_pointer_buttons_length)>
            >
            buttons(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_xi_query_pointer_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_input_xi_query_pointer_buttons),
                SIGNATURE(xcb_input_xi_query_pointer_buttons_length)>
                >(this->_M_c, this->get());
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            root(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->root),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->root,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            child(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->child),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->child,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class xi_query_pointer

} // namespace detail

namespace checked {
template<typename Connection>
using xi_query_pointer = detail::xi_query_pointer<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_query_pointer)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_query_pointer = detail::xi_query_pointer<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_query_pointer_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_query_pointer<Connection>
xi_query_pointer(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_query_pointer<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_query_pointer<Connection>
xi_query_pointer_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_query_pointer<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_warp_pointer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_warp_pointer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_warp_pointer(Parameter && ... parameter)
{
    ::xcb_input_xi_warp_pointer(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_change_cursor_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_change_cursor_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_change_cursor(Parameter && ... parameter)
{
    ::xcb_input_xi_change_cursor(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_change_hierarchy_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_change_hierarchy_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_change_hierarchy(Parameter && ... parameter)
{
    ::xcb_input_xi_change_hierarchy(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_set_client_pointer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_set_client_pointer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_set_client_pointer(Parameter && ... parameter)
{
    ::xcb_input_xi_set_client_pointer(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_get_client_pointer
        : public cppual::generic::reply<xi_get_client_pointer<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_get_client_pointer_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_get_client_pointer<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_get_client_pointer_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_get_client_pointer(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class xi_get_client_pointer

} // namespace detail

namespace checked {
template<typename Connection>
using xi_get_client_pointer = detail::xi_get_client_pointer<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_get_client_pointer)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_get_client_pointer = detail::xi_get_client_pointer<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_get_client_pointer_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_get_client_pointer<Connection>
xi_get_client_pointer(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_get_client_pointer<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_get_client_pointer<Connection>
xi_get_client_pointer_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_get_client_pointer<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_select_events_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_select_events_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_select_events(Parameter && ... parameter)
{
    ::xcb_input_xi_select_events(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_query_version
        : public cppual::generic::reply<xi_query_version<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_query_version_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_query_version(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class xi_query_version

} // namespace detail

namespace checked {
template<typename Connection>
using xi_query_version = detail::xi_query_version<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_query_version = detail::xi_query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_query_version_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_query_version<Connection>
xi_query_version(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_query_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_query_version<Connection>
xi_query_version_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_query_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_query_device
        : public cppual::generic::reply<xi_query_device<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_query_device_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_query_device<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_query_device_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_query_device(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_xi_query_device_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_xi_device_info_t,
            SIGNATURE(xcb_input_xi_device_info_next),
            SIGNATURE(xcb_input_xi_device_info_sizeof),
            SIGNATURE(xcb_input_xi_query_device_infos_iterator)>
            >
            infos(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_xi_query_device_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_xi_device_info_t,
                SIGNATURE(xcb_input_xi_device_info_next),
                SIGNATURE(xcb_input_xi_device_info_sizeof),
                SIGNATURE(xcb_input_xi_query_device_infos_iterator)>
                >(this->_M_c, this->get());
    }
}; // class xi_query_device

} // namespace detail

namespace checked {
template<typename Connection>
using xi_query_device = detail::xi_query_device<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_query_device)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_query_device = detail::xi_query_device<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_query_device_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_query_device<Connection>
xi_query_device(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_query_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_query_device<Connection>
xi_query_device_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_query_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
xi_set_focus_checked(Connection && c, ::xcb_window_t window, ::xcb_timestamp_t time, ::xcb_input_device_id_t deviceid)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_set_focus_checked(std::forward<Connection>(c), window, time, deviceid));
}

template<typename Connection>
void
xi_set_focus(Connection && c, ::xcb_window_t window, ::xcb_timestamp_t time, ::xcb_input_device_id_t deviceid)
{
    ::xcb_input_xi_set_focus(std::forward<Connection>(c), window, time, deviceid);
}

template<typename Connection>
void
xi_set_focus_checked(Connection && c, ::xcb_window_t window, ::xcb_input_device_id_t deviceid, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_set_focus_checked(std::forward<Connection>(c), window, time, deviceid));
}

template<typename Connection>
void
xi_set_focus(Connection && c, ::xcb_window_t window, ::xcb_input_device_id_t deviceid, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    ::xcb_input_xi_set_focus(std::forward<Connection>(c), window, time, deviceid);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_get_focus
        : public cppual::generic::reply<xi_get_focus<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_get_focus_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_get_focus<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_get_focus_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_get_focus(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            focus(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->focus),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->focus,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class xi_get_focus

} // namespace detail

namespace checked {
template<typename Connection>
using xi_get_focus = detail::xi_get_focus<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_get_focus)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_get_focus = detail::xi_get_focus<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_get_focus_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_get_focus<Connection>
xi_get_focus(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_get_focus<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_get_focus<Connection>
xi_get_focus_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_get_focus<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_grab_device
        : public cppual::generic::reply<xi_grab_device<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_grab_device_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_grab_device<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_grab_device_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_grab_device(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
            ::xcb_input_xi_grab_device_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_window_t window, ::xcb_timestamp_t time, ::xcb_cursor_t cursor, ::xcb_input_device_id_t deviceid, uint8_t mode, uint8_t paired_device_mode, uint8_t owner_events, uint16_t mask_len, const uint32_t * mask)
    {
        return base::cookie(c, window, time, cursor, deviceid, mode, paired_device_mode, owner_events, mask_len, mask);
    }

    static
            ::xcb_input_xi_grab_device_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_window_t window, ::xcb_cursor_t cursor, ::xcb_input_device_id_t deviceid, uint8_t mode, uint8_t paired_device_mode, uint8_t owner_events, uint16_t mask_len, const uint32_t * mask, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
    {
        return base::cookie(c, window, time, cursor, deviceid, mode, paired_device_mode, owner_events, mask_len, mask);
    }

    template<typename Mask_Iterator>
    static
            ::xcb_input_xi_grab_device_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_window_t window, ::xcb_cursor_t cursor, ::xcb_input_device_id_t deviceid, uint8_t mode, uint8_t paired_device_mode, uint8_t owner_events, Mask_Iterator mask_begin, Mask_Iterator mask_end, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
    {
        typedef typename value_type<Mask_Iterator, ! std::is_pointer<Mask_Iterator>::value>::type
                vector_type;
        std::vector<vector_type> mask =
        { value_iterator<Mask_Iterator>(mask_begin), value_iterator<Mask_Iterator>(mask_end) };

        return base::cookie(c, window, time, cursor, deviceid, mode, paired_device_mode, owner_events, static_cast<uint16_t>(mask.size()), const_cast<const vector_type *>(mask.data()));
    }
}; // class xi_grab_device

} // namespace detail

namespace checked {
template<typename Connection>
using xi_grab_device = detail::xi_grab_device<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_grab_device)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_grab_device = detail::xi_grab_device<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_grab_device_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_grab_device<Connection>
xi_grab_device(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_grab_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_grab_device<Connection>
xi_grab_device_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_grab_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
xi_ungrab_device_checked(Connection && c, ::xcb_timestamp_t time, ::xcb_input_device_id_t deviceid)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_ungrab_device_checked(std::forward<Connection>(c), time, deviceid));
}

template<typename Connection>
void
xi_ungrab_device(Connection && c, ::xcb_timestamp_t time, ::xcb_input_device_id_t deviceid)
{
    ::xcb_input_xi_ungrab_device(std::forward<Connection>(c), time, deviceid);
}

template<typename Connection>
void
xi_ungrab_device_checked(Connection && c, ::xcb_input_device_id_t deviceid, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_ungrab_device_checked(std::forward<Connection>(c), time, deviceid));
}

template<typename Connection>
void
xi_ungrab_device(Connection && c, ::xcb_input_device_id_t deviceid, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    ::xcb_input_xi_ungrab_device(std::forward<Connection>(c), time, deviceid);
}

template<typename Connection>
void
xi_allow_events_checked(Connection && c, ::xcb_timestamp_t time, ::xcb_input_device_id_t deviceid, uint8_t event_mode, uint32_t touchid, ::xcb_window_t grab_window)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_allow_events_checked(std::forward<Connection>(c), time, deviceid, event_mode, touchid, grab_window));
}

template<typename Connection>
void
xi_allow_events(Connection && c, ::xcb_timestamp_t time, ::xcb_input_device_id_t deviceid, uint8_t event_mode, uint32_t touchid, ::xcb_window_t grab_window)
{
    ::xcb_input_xi_allow_events(std::forward<Connection>(c), time, deviceid, event_mode, touchid, grab_window);
}

template<typename Connection>
void
xi_allow_events_checked(Connection && c, ::xcb_input_device_id_t deviceid, uint8_t event_mode, uint32_t touchid, ::xcb_window_t grab_window, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_allow_events_checked(std::forward<Connection>(c), time, deviceid, event_mode, touchid, grab_window));
}

template<typename Connection>
void
xi_allow_events(Connection && c, ::xcb_input_device_id_t deviceid, uint8_t event_mode, uint32_t touchid, ::xcb_window_t grab_window, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    ::xcb_input_xi_allow_events(std::forward<Connection>(c), time, deviceid, event_mode, touchid, grab_window);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_passive_grab_device
        : public cppual::generic::reply<xi_passive_grab_device<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_passive_grab_device_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_passive_grab_device<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_passive_grab_device_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_passive_grab_device(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
            ::xcb_input_xi_passive_grab_device_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_timestamp_t time, ::xcb_window_t grab_window, ::xcb_cursor_t cursor, uint32_t detail, ::xcb_input_device_id_t deviceid, uint16_t num_modifiers, uint16_t mask_len, uint8_t grab_type, uint8_t grab_mode, uint8_t paired_device_mode, uint8_t owner_events, const uint32_t * mask, const uint32_t * modifiers)
    {
        return base::cookie(c, time, grab_window, cursor, detail, deviceid, num_modifiers, mask_len, grab_type, grab_mode, paired_device_mode, owner_events, mask, modifiers);
    }

    static
            ::xcb_input_xi_passive_grab_device_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_window_t grab_window, ::xcb_cursor_t cursor, uint32_t detail, ::xcb_input_device_id_t deviceid, uint16_t num_modifiers, uint16_t mask_len, uint8_t grab_type, uint8_t grab_mode, uint8_t paired_device_mode, uint8_t owner_events, const uint32_t * mask, const uint32_t * modifiers, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
    {
        return base::cookie(c, time, grab_window, cursor, detail, deviceid, num_modifiers, mask_len, grab_type, grab_mode, paired_device_mode, owner_events, mask, modifiers);
    }

    cppual::generic::list<Connection,
            ::xcb_input_xi_passive_grab_device_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_grab_modifier_info_t,
            SIGNATURE(xcb_input_xi_passive_grab_device_modifiers),
            SIGNATURE(xcb_input_xi_passive_grab_device_modifiers_length)>
            >
            modifiers(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_xi_passive_grab_device_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_grab_modifier_info_t,
                SIGNATURE(xcb_input_xi_passive_grab_device_modifiers),
                SIGNATURE(xcb_input_xi_passive_grab_device_modifiers_length)>
                >(this->_M_c, this->get());
    }
}; // class xi_passive_grab_device

} // namespace detail

namespace checked {
template<typename Connection>
using xi_passive_grab_device = detail::xi_passive_grab_device<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_passive_grab_device)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_passive_grab_device = detail::xi_passive_grab_device<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_passive_grab_device_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_passive_grab_device<Connection>
xi_passive_grab_device(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_passive_grab_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_passive_grab_device<Connection>
xi_passive_grab_device_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_passive_grab_device<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_passive_ungrab_device_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_passive_ungrab_device_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_passive_ungrab_device(Parameter && ... parameter)
{
    ::xcb_input_xi_passive_ungrab_device(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_list_properties
        : public cppual::generic::reply<xi_list_properties<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_list_properties_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_list_properties<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_list_properties_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_list_properties(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename Properties = ::xcb_atom_t>
    cppual::generic::list<Connection,
            ::xcb_input_xi_list_properties_reply_t,
            cppual::generic::iterator<Connection,
            Properties,
            SIGNATURE(xcb_input_xi_list_properties_properties),
            SIGNATURE(xcb_input_xi_list_properties_properties_length)>
            >
            properties(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_xi_list_properties_reply_t,
                cppual::generic::iterator<Connection,
                Properties,
                SIGNATURE(xcb_input_xi_list_properties_properties),
                SIGNATURE(xcb_input_xi_list_properties_properties_length)>
                >(this->_M_c, this->get());
    }
}; // class xi_list_properties

} // namespace detail

namespace checked {
template<typename Connection>
using xi_list_properties = detail::xi_list_properties<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_list_properties)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_list_properties = detail::xi_list_properties<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_list_properties_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_list_properties<Connection>
xi_list_properties(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_list_properties<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_list_properties<Connection>
xi_list_properties_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_list_properties<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_change_property_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_change_property_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_change_property(Parameter && ... parameter)
{
    ::xcb_input_xi_change_property(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_delete_property_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_delete_property_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_delete_property(Parameter && ... parameter)
{
    ::xcb_input_xi_delete_property(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_get_property
        : public cppual::generic::reply<xi_get_property<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_get_property_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_get_property<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_get_property_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_get_property(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
            type(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->type),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->type,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class xi_get_property

} // namespace detail

namespace checked {
template<typename Connection>
using xi_get_property = detail::xi_get_property<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_get_property)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_get_property = detail::xi_get_property<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_get_property_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_get_property<Connection>
xi_get_property(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_get_property<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_get_property<Connection>
xi_get_property_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_get_property<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class xi_get_selected_events
        : public cppual::generic::reply<xi_get_selected_events<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_input_xi_get_selected_events_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<xi_get_selected_events<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_input_xi_get_selected_events_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    xi_get_selected_events(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_input_xi_get_selected_events_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_input_event_mask_t,
            SIGNATURE(xcb_input_event_mask_next),
            SIGNATURE(xcb_input_event_mask_sizeof),
            SIGNATURE(xcb_input_xi_get_selected_events_masks_iterator)>
            >
            masks(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_input_xi_get_selected_events_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_input_event_mask_t,
                SIGNATURE(xcb_input_event_mask_next),
                SIGNATURE(xcb_input_event_mask_sizeof),
                SIGNATURE(xcb_input_xi_get_selected_events_masks_iterator)>
                >(this->_M_c, this->get());
    }
}; // class xi_get_selected_events

} // namespace detail

namespace checked {
template<typename Connection>
using xi_get_selected_events = detail::xi_get_selected_events<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_input_xi_get_selected_events)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using xi_get_selected_events = detail::xi_get_selected_events<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_input_xi_get_selected_events_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::xi_get_selected_events<Connection>
xi_get_selected_events(Connection && c, Parameter && ... parameter)
{
    return reply::checked::xi_get_selected_events<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::xi_get_selected_events<Connection>
xi_get_selected_events_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::xi_get_selected_events<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
xi_barrier_release_pointer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_xi_barrier_release_pointer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
xi_barrier_release_pointer(Parameter && ... parameter)
{
    ::xcb_input_xi_barrier_release_pointer(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
send_extension_event_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::input::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_input_send_extension_event_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
send_extension_event(Parameter && ... parameter)
{
    ::xcb_input_send_extension_event(std::forward<Parameter>(parameter) ...);
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
    typedef cppual::input::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    input(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    get_extension_version(Parameter && ... parameter) const
    -> reply::checked::get_extension_version<Connection>
    {
        return cppual::input::get_extension_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_extension_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_extension_version<Connection>
    {
        return cppual::input::get_extension_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_input_devices(Parameter && ... parameter) const
    -> reply::checked::list_input_devices<Connection>
    {
        return cppual::input::list_input_devices(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_input_devices_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_input_devices<Connection>
    {
        return cppual::input::list_input_devices_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    open_device(Parameter && ... parameter) const
    -> reply::checked::open_device<Connection>
    {
        return cppual::input::open_device(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    open_device_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::open_device<Connection>
    {
        return cppual::input::open_device_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    close_device_checked(Parameter && ... parameter) const
    {
        cppual::input::close_device_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    close_device(Parameter && ... parameter) const
    {
        cppual::input::close_device(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_device_mode(Parameter && ... parameter) const
    -> reply::checked::set_device_mode<Connection>
    {
        return cppual::input::set_device_mode(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_device_mode_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_device_mode<Connection>
    {
        return cppual::input::set_device_mode_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_extension_event_checked(Parameter && ... parameter) const
    {
        cppual::input::select_extension_event_checked(connection(),
                                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_extension_event(Parameter && ... parameter) const
    {
        cppual::input::select_extension_event(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_selected_extension_events(Parameter && ... parameter) const
    -> reply::checked::get_selected_extension_events<Connection>
    {
        return cppual::input::get_selected_extension_events(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_selected_extension_events_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_selected_extension_events<Connection>
    {
        return cppual::input::get_selected_extension_events_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_device_dont_propagate_list_checked(Parameter && ... parameter) const
    {
        cppual::input::change_device_dont_propagate_list_checked(connection(),
                                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_device_dont_propagate_list(Parameter && ... parameter) const
    {
        cppual::input::change_device_dont_propagate_list(connection(),
                                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_dont_propagate_list(Parameter && ... parameter) const
    -> reply::checked::get_device_dont_propagate_list<Connection>
    {
        return cppual::input::get_device_dont_propagate_list(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_dont_propagate_list_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_dont_propagate_list<Connection>
    {
        return cppual::input::get_device_dont_propagate_list_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_motion_events(Parameter && ... parameter) const
    -> reply::checked::get_device_motion_events<Connection>
    {
        return cppual::input::get_device_motion_events(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_motion_events_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_motion_events<Connection>
    {
        return cppual::input::get_device_motion_events_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    change_keyboard_device(Parameter && ... parameter) const
    -> reply::checked::change_keyboard_device<Connection>
    {
        return cppual::input::change_keyboard_device(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    change_keyboard_device_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::change_keyboard_device<Connection>
    {
        return cppual::input::change_keyboard_device_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    change_pointer_device(Parameter && ... parameter) const
    -> reply::checked::change_pointer_device<Connection>
    {
        return cppual::input::change_pointer_device(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    change_pointer_device_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::change_pointer_device<Connection>
    {
        return cppual::input::change_pointer_device_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    grab_device(Parameter && ... parameter) const
    -> reply::checked::grab_device<Connection>
    {
        return cppual::input::grab_device(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    grab_device_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::grab_device<Connection>
    {
        return cppual::input::grab_device_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    ungrab_device_checked(Parameter && ... parameter) const
    {
        cppual::input::ungrab_device_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    ungrab_device(Parameter && ... parameter) const
    {
        cppual::input::ungrab_device(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    grab_device_key_checked(Parameter && ... parameter) const
    {
        cppual::input::grab_device_key_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    grab_device_key(Parameter && ... parameter) const
    {
        cppual::input::grab_device_key(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    ungrab_device_key_checked(Parameter && ... parameter) const
    {
        cppual::input::ungrab_device_key_checked(connection(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    ungrab_device_key(Parameter && ... parameter) const
    {
        cppual::input::ungrab_device_key(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    grab_device_button_checked(Parameter && ... parameter) const
    {
        cppual::input::grab_device_button_checked(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    grab_device_button(Parameter && ... parameter) const
    {
        cppual::input::grab_device_button(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    ungrab_device_button_checked(Parameter && ... parameter) const
    {
        cppual::input::ungrab_device_button_checked(connection(),
                                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    ungrab_device_button(Parameter && ... parameter) const
    {
        cppual::input::ungrab_device_button(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    allow_device_events_checked(Parameter && ... parameter) const
    {
        cppual::input::allow_device_events_checked(connection(),
                                                std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    allow_device_events(Parameter && ... parameter) const
    {
        cppual::input::allow_device_events(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_focus(Parameter && ... parameter) const
    -> reply::checked::get_device_focus<Connection>
    {
        return cppual::input::get_device_focus(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_focus_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_focus<Connection>
    {
        return cppual::input::get_device_focus_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_device_focus_checked(Parameter && ... parameter) const
    {
        cppual::input::set_device_focus_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_device_focus(Parameter && ... parameter) const
    {
        cppual::input::set_device_focus(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_feedback_control(Parameter && ... parameter) const
    -> reply::checked::get_feedback_control<Connection>
    {
        return cppual::input::get_feedback_control(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_feedback_control_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_feedback_control<Connection>
    {
        return cppual::input::get_feedback_control_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_feedback_control_checked(Parameter && ... parameter) const
    {
        cppual::input::change_feedback_control_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_feedback_control(Parameter && ... parameter) const
    {
        cppual::input::change_feedback_control(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_key_mapping(Parameter && ... parameter) const
    -> reply::checked::get_device_key_mapping<Connection>
    {
        return cppual::input::get_device_key_mapping(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_key_mapping_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_key_mapping<Connection>
    {
        return cppual::input::get_device_key_mapping_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_device_key_mapping_checked(Parameter && ... parameter) const
    {
        cppual::input::change_device_key_mapping_checked(connection(),
                                                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_device_key_mapping(Parameter && ... parameter) const
    {
        cppual::input::change_device_key_mapping(connection(),
                                              std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_modifier_mapping(Parameter && ... parameter) const
    -> reply::checked::get_device_modifier_mapping<Connection>
    {
        return cppual::input::get_device_modifier_mapping(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_modifier_mapping_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_modifier_mapping<Connection>
    {
        return cppual::input::get_device_modifier_mapping_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_device_modifier_mapping(Parameter && ... parameter) const
    -> reply::checked::set_device_modifier_mapping<Connection>
    {
        return cppual::input::set_device_modifier_mapping(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_device_modifier_mapping_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_device_modifier_mapping<Connection>
    {
        return cppual::input::set_device_modifier_mapping_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_button_mapping(Parameter && ... parameter) const
    -> reply::checked::get_device_button_mapping<Connection>
    {
        return cppual::input::get_device_button_mapping(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_button_mapping_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_button_mapping<Connection>
    {
        return cppual::input::get_device_button_mapping_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_device_button_mapping(Parameter && ... parameter) const
    -> reply::checked::set_device_button_mapping<Connection>
    {
        return cppual::input::set_device_button_mapping(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_device_button_mapping_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_device_button_mapping<Connection>
    {
        return cppual::input::set_device_button_mapping_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_device_state(Parameter && ... parameter) const
    -> reply::checked::query_device_state<Connection>
    {
        return cppual::input::query_device_state(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_device_state_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_device_state<Connection>
    {
        return cppual::input::query_device_state_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    device_bell_checked(Parameter && ... parameter) const
    {
        cppual::input::device_bell_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    device_bell(Parameter && ... parameter) const
    {
        cppual::input::device_bell(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_device_valuators(Parameter && ... parameter) const
    -> reply::checked::set_device_valuators<Connection>
    {
        return cppual::input::set_device_valuators(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_device_valuators_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_device_valuators<Connection>
    {
        return cppual::input::set_device_valuators_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_control(Parameter && ... parameter) const
    -> reply::checked::get_device_control<Connection>
    {
        return cppual::input::get_device_control(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_control_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_control<Connection>
    {
        return cppual::input::get_device_control_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    change_device_control(Parameter && ... parameter) const
    -> reply::checked::change_device_control<Connection>
    {
        return cppual::input::change_device_control(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    change_device_control_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::change_device_control<Connection>
    {
        return cppual::input::change_device_control_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_device_properties(Parameter && ... parameter) const
    -> reply::checked::list_device_properties<Connection>
    {
        return cppual::input::list_device_properties(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_device_properties_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_device_properties<Connection>
    {
        return cppual::input::list_device_properties_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_device_property_checked(Parameter && ... parameter) const
    {
        cppual::input::change_device_property_checked(connection(),
                                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_device_property(Parameter && ... parameter) const
    {
        cppual::input::change_device_property(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_device_property_checked(Parameter && ... parameter) const
    {
        cppual::input::delete_device_property_checked(connection(),
                                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_device_property(Parameter && ... parameter) const
    {
        cppual::input::delete_device_property(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_property(Parameter && ... parameter) const
    -> reply::checked::get_device_property<Connection>
    {
        return cppual::input::get_device_property(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_property<Connection>
    {
        return cppual::input::get_device_property_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_query_pointer(Parameter && ... parameter) const
    -> reply::checked::xi_query_pointer<Connection>
    {
        return cppual::input::xi_query_pointer(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_query_pointer_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_query_pointer<Connection>
    {
        return cppual::input::xi_query_pointer_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_warp_pointer_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_warp_pointer_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_warp_pointer(Parameter && ... parameter) const
    {
        cppual::input::xi_warp_pointer(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_change_cursor_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_change_cursor_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_change_cursor(Parameter && ... parameter) const
    {
        cppual::input::xi_change_cursor(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_change_hierarchy_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_change_hierarchy_checked(connection(),
                                                std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_change_hierarchy(Parameter && ... parameter) const
    {
        cppual::input::xi_change_hierarchy(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_set_client_pointer_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_set_client_pointer_checked(connection(),
                                                  std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_set_client_pointer(Parameter && ... parameter) const
    {
        cppual::input::xi_set_client_pointer(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_get_client_pointer(Parameter && ... parameter) const
    -> reply::checked::xi_get_client_pointer<Connection>
    {
        return cppual::input::xi_get_client_pointer(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_get_client_pointer_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_get_client_pointer<Connection>
    {
        return cppual::input::xi_get_client_pointer_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_select_events_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_select_events_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_select_events(Parameter && ... parameter) const
    {
        cppual::input::xi_select_events(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_query_version(Parameter && ... parameter) const
    -> reply::checked::xi_query_version<Connection>
    {
        return cppual::input::xi_query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_query_version<Connection>
    {
        return cppual::input::xi_query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_query_device(Parameter && ... parameter) const
    -> reply::checked::xi_query_device<Connection>
    {
        return cppual::input::xi_query_device(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_query_device_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_query_device<Connection>
    {
        return cppual::input::xi_query_device_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_set_focus_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_set_focus_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_set_focus(Parameter && ... parameter) const
    {
        cppual::input::xi_set_focus(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_get_focus(Parameter && ... parameter) const
    -> reply::checked::xi_get_focus<Connection>
    {
        return cppual::input::xi_get_focus(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_get_focus_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_get_focus<Connection>
    {
        return cppual::input::xi_get_focus_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_grab_device(Parameter && ... parameter) const
    -> reply::checked::xi_grab_device<Connection>
    {
        return cppual::input::xi_grab_device(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_grab_device_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_grab_device<Connection>
    {
        return cppual::input::xi_grab_device_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_ungrab_device_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_ungrab_device_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_ungrab_device(Parameter && ... parameter) const
    {
        cppual::input::xi_ungrab_device(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_allow_events_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_allow_events_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_allow_events(Parameter && ... parameter) const
    {
        cppual::input::xi_allow_events(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_passive_grab_device(Parameter && ... parameter) const
    -> reply::checked::xi_passive_grab_device<Connection>
    {
        return cppual::input::xi_passive_grab_device(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_passive_grab_device_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_passive_grab_device<Connection>
    {
        return cppual::input::xi_passive_grab_device_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_passive_ungrab_device_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_passive_ungrab_device_checked(connection(),
                                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_passive_ungrab_device(Parameter && ... parameter) const
    {
        cppual::input::xi_passive_ungrab_device(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_list_properties(Parameter && ... parameter) const
    -> reply::checked::xi_list_properties<Connection>
    {
        return cppual::input::xi_list_properties(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_list_properties_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_list_properties<Connection>
    {
        return cppual::input::xi_list_properties_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_change_property_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_change_property_checked(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_change_property(Parameter && ... parameter) const
    {
        cppual::input::xi_change_property(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_delete_property_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_delete_property_checked(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_delete_property(Parameter && ... parameter) const
    {
        cppual::input::xi_delete_property(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_get_property(Parameter && ... parameter) const
    -> reply::checked::xi_get_property<Connection>
    {
        return cppual::input::xi_get_property(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_get_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_get_property<Connection>
    {
        return cppual::input::xi_get_property_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    xi_get_selected_events(Parameter && ... parameter) const
    -> reply::checked::xi_get_selected_events<Connection>
    {
        return cppual::input::xi_get_selected_events(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    xi_get_selected_events_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::xi_get_selected_events<Connection>
    {
        return cppual::input::xi_get_selected_events_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    xi_barrier_release_pointer_checked(Parameter && ... parameter) const
    {
        cppual::input::xi_barrier_release_pointer_checked(connection(),
                                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    xi_barrier_release_pointer(Parameter && ... parameter) const
    {
        cppual::input::xi_barrier_release_pointer(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    send_extension_event_checked(Parameter && ... parameter) const
    {
        cppual::input::send_extension_event_checked(connection(),
                                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    send_extension_event(Parameter && ... parameter) const
    {
        cppual::input::send_extension_event(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace events {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::input::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::input::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t>& event) const
    {
        switch ((event->response_type & ~0x80) - _M_first_event) {

        case XCB_INPUT_DEVICE_VALUATOR:
            handler(device_valuator<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_KEY_PRESS:
            handler(device_key_press<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_KEY_RELEASE:
            handler(device_key_release<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_BUTTON_PRESS:
            handler(device_button_press<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_BUTTON_RELEASE:
            handler(device_button_release<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_MOTION_NOTIFY:
            handler(device_motion_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_FOCUS_IN:
            handler(device_focus_in<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_FOCUS_OUT:
            handler(device_focus_out<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_PROXIMITY_IN:
            handler(proximity_in<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_PROXIMITY_OUT:
            handler(proximity_out<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_STATE_NOTIFY:
            handler(device_state_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_MAPPING_NOTIFY:
            handler(device_mapping_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_CHANGE_DEVICE_NOTIFY:
            handler(change_device_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_KEY_STATE_NOTIFY:
            handler(device_key_state_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_BUTTON_STATE_NOTIFY:
            handler(device_button_state_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_PRESENCE_NOTIFY:
            handler(device_presence_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_DEVICE_PROPERTY_NOTIFY:
            handler(device_property_notify<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_MOTION:
            handler(raw_motion<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_TOUCH_BEGIN:
            handler(touch_begin<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_TOUCH_UPDATE:
            handler(touch_update<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_TOUCH_END:
            handler(touch_end<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_TOUCH_OWNERSHIP:
            handler(touch_ownership<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_TOUCH_BEGIN:
            handler(raw_touch_begin<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_TOUCH_UPDATE:
            handler(raw_touch_update<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_TOUCH_END:
            handler(raw_touch_end<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_BARRIER_HIT:
            handler(barrier_hit<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_BARRIER_LEAVE:
            handler(barrier_leave<Connection>(_M_c, _M_first_event, event));
            return true;

        };

        switch ((event->response_type & ~0x80) - _M_first_event) {

        case XCB_INPUT_DEVICE_CHANGED:
            handler(device_changed<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_KEY_PRESS:
            handler(key_press<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_KEY_RELEASE:
            handler(key_release<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_BUTTON_PRESS:
            handler(button_press<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_BUTTON_RELEASE:
            handler(button_release<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_MOTION:
            handler(motion<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_ENTER:
            handler(enter<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_LEAVE:
            handler(leave<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_FOCUS_IN:
            handler(focus_in<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_FOCUS_OUT:
            handler(focus_out<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_HIERARCHY:
            handler(hierarchy<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_PROPERTY:
            handler(property<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_KEY_PRESS:
            handler(raw_key_press<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_KEY_RELEASE:
            handler(raw_key_release<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_BUTTON_PRESS:
            handler(raw_button_press<Connection>(_M_c, _M_first_event, event));
            return true;

        case XCB_INPUT_RAW_BUTTON_RELEASE:
            handler(raw_button_release<Connection>(_M_c, _M_first_event, event));
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
    typedef cppual::input::extension extension;

    dispatcher(uint8_t first_error)
        : _M_first_error(first_error)
    {}

    dispatcher(const cppual::input::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - _M_first_error) {

        case XCB_INPUT_DEVICE: // 0
            throw cppual::input::error::device(error);

        case XCB_INPUT_EVENT: // 1
            throw cppual::input::error::event(error);

        case XCB_INPUT_MODE: // 2
            throw cppual::input::error::mode(error);

        case XCB_INPUT_DEVICE_BUSY: // 3
            throw cppual::input::error::device_busy(error);

        case XCB_INPUT_CLASS: // 4
            throw cppual::input::error::_class(error);

        };
    }

protected:
    uint8_t _M_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::input

#endif // CPPUAL_INPUT_HPP
