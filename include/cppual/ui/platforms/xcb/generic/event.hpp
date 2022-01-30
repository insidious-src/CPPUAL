#ifndef CPPUAL_GENERIC_EVENT_HPP
#define CPPUAL_GENERIC_EVENT_HPP

#include <memory> // shared_ptr
#include <xcb/xcb.h> // ::xcb_generic_event_t

namespace cppual { namespace generic {

template<typename Event>
class event {
public:
    event(const std::shared_ptr<xcb_generic_event_t> & event)
        : _M_event(event)
    {}

    virtual
    ~event(void) {}

    virtual
    operator const Event &(void) const
    {
        return reinterpret_cast<const Event &>(*_M_event);
    }

    virtual
    const Event &
    operator*(void) const
    {
        return reinterpret_cast<const Event &>(*_M_event);
    }

    virtual
    Event *
    operator->(void) const
    {
        return reinterpret_cast<Event * const>(_M_event.get());
    }

protected:
    std::shared_ptr<xcb_generic_event_t> _M_event;
}; // class event

} } // namespace cppual::generic

#endif // CPPUAL_GENERIC_EVENT_HPP
