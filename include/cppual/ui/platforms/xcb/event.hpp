#ifndef CPPUAL_EVENT_HPP
#define CPPUAL_EVENT_HPP

#include <climits>
#include <map>
#include <vector>
#include <unordered_map>

#include <cppual/ui/platforms/xcb/proto/x.hpp>

#define MAX_PRIORITY UINT32_MAX

namespace cppual {

namespace event {

namespace detail {

class dispatcher {
public:
    virtual ~dispatcher(void) {}
    template<typename Event> void dispatch(const Event & e);
}; // class dispatcher

template<typename Event>
class sink : virtual public dispatcher
{
public:
    virtual ~sink(void) {}
    virtual void handle(const Event &) = 0;
};

} // namespace detail

template<typename Event, typename ... Events>
class sink
        : public detail::sink<Event>
        , public detail::sink<Events> ...
{};

template<typename Connection, typename ... Extensions>
class registry
        : public cppual::x::event::dispatcher<Connection>
        , public Extensions::template event_dispatcher<Connection> ...
{
public:
    typedef unsigned int priority;

    template<typename C>
    explicit
    registry(C && c)
        : cppual::x::event::dispatcher<Connection>(std::forward<C>(c))
        , Extensions::template event_dispatcher<Connection>(
              std::forward<C>(c), c.template extension<Extensions>()) ...
        , _M_c(std::forward<C>(c))
    {}

    bool
    dispatch(const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        return dispatch<cppual::x::extension, Extensions ...>(event);
    }

    template<typename Event, typename ... Rest>
    void
    attach(priority p, sink<Event, Rest ...> * s)
    {
        attach<sink<Event, Rest ...>, Event, Rest ...>(p, s);
    }

    template<typename Event, typename ... Rest>
    void
    detach(priority p, sink<Event, Rest ...> * s)
    {
        detach<sink<Event, Rest ...>, Event, Rest ...>(p, s);
    }

private:
    typedef std::multimap<priority, detail::dispatcher *> priority_map;

    Connection _M_c;
    std::unordered_map<uint8_t, priority_map> _M_dispatchers;

    template<typename Event>
    uint8_t opcode(const cppual::x::extension &) const
    {
        return Event::opcode();
    }

    template<typename Event, typename Extension>
    uint8_t opcode(const Extension & extension) const
    {
        return Event::opcode(extension);
    }

    template<typename Event>
    uint8_t opcode(void) const
    {
        return opcode<Event>(_M_c.template extension<typename Event::extension>());
    }

    template<typename Event>
    void
    handle(const Event & event) const
    {
        try {
            for (auto & item : _M_dispatchers.at(opcode<Event>())) {
                item.second->dispatch(event);
            }
        } catch (...) {}
    }

    struct handler {
        handler(const registry<Connection, Extensions ...> & registry)
            : _M_registry(registry)
        {}

        const registry<Connection, Extensions ...> & _M_registry;

        template<typename Event>
        void
        operator()(const Event & event) const
        {
            _M_registry.handle(event);
        }
    };

    template<typename Extension>
    bool
    dispatch(const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        typedef const typename Extension::template event_dispatcher<Connection> & dispatcher;
        return static_cast<dispatcher>(*this)(handler(*this), event);
    }

    template<typename Extension, typename Next, typename ... Rest>
    bool
    dispatch(const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        dispatch<Extension>(event);
        return dispatch<Next, Rest ...>(event);
    }

    template<typename Sink, typename Event>
    void
    attach(priority p, Sink * s)
    {
        attach(p, s, opcode<Event>());
    }

    template<typename Sink, typename Event, typename Next, typename ... Rest>
    void
    attach(priority p, Sink * s)
    {
        attach(p, s, opcode<Event>());
        attach<Sink, Next, Rest ...>(p, s);
    }

    void attach(priority p, detail::dispatcher * d, uint8_t opcode)
    {
        _M_dispatchers[opcode].emplace(p, d);
    }

    template<typename Sink, typename Event>
    void
    detach(priority p, Sink * s)
    {
        detach(p, s, opcode<Event>());
    }

    template<typename Sink, typename Event, typename Next, typename ... Rest>
    void
    detach(priority p, Sink * s)
    {
        detach(p, s, opcode<Event>());
        detach<Sink, Next, Rest ...>(p, s);
    }

    void
    detach(priority p, detail::dispatcher * d, uint8_t opcode)
    {
        try {
            auto & prio_map = _M_dispatchers.at(opcode);
            const auto & prio_sink_pair = prio_map.equal_range(p);
            for (auto it = prio_sink_pair.first; it != prio_sink_pair.second; ) {
                if (d == it->second) {
                    it = prio_map.erase(it);
                } else {
                    ++it;
                }
            }
        } catch (...) {}
    }

}; // xpp::event::source

} // namespace event

} // namespace cppual

template<typename Event>
void cppual::event::detail::dispatcher::dispatch(const Event & e)
{
    auto event_sink = dynamic_cast<cppual::event::detail::sink<Event> *>(this);
    if (event_sink != nullptr) {
        event_sink->handle(e);
    }
}

#endif // CPPUAL_EVENT_HPP
