#ifndef CPPUAL_SYNC_HPP
#define CPPUAL_SYNC_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/sync.h>

namespace cppual { namespace sync {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_sync_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_sync_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::sync::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::sync::event::dispatcher<Connection>;
    using error_dispatcher = cppual::sync::error::dispatcher;
};

namespace event {
template<typename Connection>
class counter_notify
        : public cppual::generic::event<xcb_sync_counter_notify_event_t>
{
public:
    typedef cppual::sync::extension extension;

    typedef cppual::generic::event<xcb_sync_counter_notify_event_t> base;

    template<typename C>
    counter_notify(C && c,
                   uint8_t first_event,
                   const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~counter_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_SYNC_COUNTER_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::sync::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_SYNC_COUNTER_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_sync_counter_t, typename ... Parameter>
    ReturnType
    counter(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->counter),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->counter,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class counter_notify



} // namespace event


namespace event {
template<typename Connection>
class alarm_notify
        : public cppual::generic::event<xcb_sync_alarm_notify_event_t>
{
public:
    typedef cppual::sync::extension extension;

    typedef cppual::generic::event<xcb_sync_alarm_notify_event_t> base;

    template<typename C>
    alarm_notify(C && c,
                 uint8_t first_event,
                 const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~alarm_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_SYNC_ALARM_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::sync::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_SYNC_ALARM_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_sync_alarm_t, typename ... Parameter>
    ReturnType
    alarm(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->alarm),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->alarm,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class alarm_notify



} // namespace event



namespace error {
class counter
        : public cppual::generic::error<counter,
        ::xcb_sync_counter_error_t>
{
public:
    using cppual::generic::error<counter, ::xcb_sync_counter_error_t>::error;

    virtual ~counter(void) {}

    static uint8_t opcode(void)
    {
        return XCB_SYNC_COUNTER;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::sync::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_SYNC_COUNTER");
    }

protected:
    uint8_t m_first_error;
}; // class counter
} // namespace error


namespace error {
class alarm
        : public cppual::generic::error<alarm,
        ::xcb_sync_alarm_error_t>
{
public:
    using cppual::generic::error<alarm, ::xcb_sync_alarm_error_t>::error;

    virtual ~alarm(void) {}

    static uint8_t opcode(void)
    {
        return XCB_SYNC_ALARM;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::sync::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_SYNC_ALARM");
    }

protected:
    uint8_t m_first_error;
}; // class alarm
} // namespace error


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class initialize
        : public cppual::generic::reply<initialize<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_sync_initialize_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<initialize<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_sync_initialize_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    initialize(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class initialize

} // namespace detail

namespace checked {
template<typename Connection>
using initialize = detail::initialize<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_sync_initialize)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using initialize = detail::initialize<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_sync_initialize_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::initialize<Connection>
initialize(Connection && c, Parameter && ... parameter)
{
    return reply::checked::initialize<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::initialize<Connection>
initialize_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::initialize<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_system_counters
        : public cppual::generic::reply<list_system_counters<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_sync_list_system_counters_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_system_counters<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_sync_list_system_counters_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_system_counters(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_sync_list_system_counters_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_sync_systemcounter_t,
            SIGNATURE(xcb_sync_systemcounter_next),
            SIGNATURE(xcb_sync_systemcounter_sizeof),
            SIGNATURE(xcb_sync_list_system_counters_counters_iterator)>
            >
            counters(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_sync_list_system_counters_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_sync_systemcounter_t,
                SIGNATURE(xcb_sync_systemcounter_next),
                SIGNATURE(xcb_sync_systemcounter_sizeof),
                SIGNATURE(xcb_sync_list_system_counters_counters_iterator)>
                >(this->m_c, this->get());
    }
}; // class list_system_counters

} // namespace detail

namespace checked {
template<typename Connection>
using list_system_counters = detail::list_system_counters<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_sync_list_system_counters)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_system_counters = detail::list_system_counters<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_sync_list_system_counters_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_system_counters<Connection>
list_system_counters(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_system_counters<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_system_counters<Connection>
list_system_counters_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_system_counters<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_counter_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_create_counter_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_counter(Parameter && ... parameter)
{
    ::xcb_sync_create_counter(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_counter_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_destroy_counter_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_counter(Parameter && ... parameter)
{
    ::xcb_sync_destroy_counter(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_counter
        : public cppual::generic::reply<query_counter<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_sync_query_counter_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_counter<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_sync_query_counter_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_counter(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_counter

} // namespace detail

namespace checked {
template<typename Connection>
using query_counter = detail::query_counter<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_sync_query_counter)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_counter = detail::query_counter<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_sync_query_counter_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_counter<Connection>
query_counter(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_counter<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_counter<Connection>
query_counter_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_counter<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
await_checked(Connection && c, uint32_t wait_list_len, const ::xcb_sync_waitcondition_t * wait_list)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_await_checked(std::forward<Connection>(c), wait_list_len, wait_list));
}

template<typename Connection>
void
await(Connection && c, uint32_t wait_list_len, const ::xcb_sync_waitcondition_t * wait_list)
{
    ::xcb_sync_await(std::forward<Connection>(c), wait_list_len, wait_list);
}

template<typename Connection, typename Wait_list_Iterator>
void
await_checked(Connection && c, Wait_list_Iterator wait_list_begin, Wait_list_Iterator wait_list_end)
{
    typedef typename value_type<Wait_list_Iterator, ! std::is_pointer<Wait_list_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> wait_list =
    { value_iterator<Wait_list_Iterator>(wait_list_begin), value_iterator<Wait_list_Iterator>(wait_list_end) };

    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_await_checked(std::forward<Connection>(c), static_cast<uint32_t>(wait_list.size()), const_cast<const vector_type *>(wait_list.data())));
}

template<typename Connection, typename Wait_list_Iterator>
void
await(Connection && c, Wait_list_Iterator wait_list_begin, Wait_list_Iterator wait_list_end)
{
    typedef typename value_type<Wait_list_Iterator, ! std::is_pointer<Wait_list_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> wait_list =
    { value_iterator<Wait_list_Iterator>(wait_list_begin), value_iterator<Wait_list_Iterator>(wait_list_end) };

    ::xcb_sync_await(std::forward<Connection>(c), static_cast<uint32_t>(wait_list.size()), const_cast<const vector_type *>(wait_list.data()));
}

template<typename Connection, typename ... Parameter>
void
change_counter_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_change_counter_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_counter(Parameter && ... parameter)
{
    ::xcb_sync_change_counter(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_counter_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_set_counter_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_counter(Parameter && ... parameter)
{
    ::xcb_sync_set_counter(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_alarm_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_create_alarm_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_alarm(Parameter && ... parameter)
{
    ::xcb_sync_create_alarm(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_alarm_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_change_alarm_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_alarm(Parameter && ... parameter)
{
    ::xcb_sync_change_alarm(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_alarm_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_destroy_alarm_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_alarm(Parameter && ... parameter)
{
    ::xcb_sync_destroy_alarm(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_alarm
        : public cppual::generic::reply<query_alarm<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_sync_query_alarm_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_alarm<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_sync_query_alarm_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_alarm(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_alarm

} // namespace detail

namespace checked {
template<typename Connection>
using query_alarm = detail::query_alarm<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_sync_query_alarm)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_alarm = detail::query_alarm<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_sync_query_alarm_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_alarm<Connection>
query_alarm(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_alarm<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_alarm<Connection>
query_alarm_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_alarm<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_priority_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_set_priority_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_priority(Parameter && ... parameter)
{
    ::xcb_sync_set_priority(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_priority
        : public cppual::generic::reply<get_priority<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_sync_get_priority_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_priority<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_sync_get_priority_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_priority(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_priority

} // namespace detail

namespace checked {
template<typename Connection>
using get_priority = detail::get_priority<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_sync_get_priority)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_priority = detail::get_priority<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_sync_get_priority_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_priority<Connection>
get_priority(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_priority<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_priority<Connection>
get_priority_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_priority<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_fence_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_create_fence_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_fence(Parameter && ... parameter)
{
    ::xcb_sync_create_fence(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
trigger_fence_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_trigger_fence_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
trigger_fence(Parameter && ... parameter)
{
    ::xcb_sync_trigger_fence(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
reset_fence_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_reset_fence_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
reset_fence(Parameter && ... parameter)
{
    ::xcb_sync_reset_fence(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_fence_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_destroy_fence_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_fence(Parameter && ... parameter)
{
    ::xcb_sync_destroy_fence(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_fence
        : public cppual::generic::reply<query_fence<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_sync_query_fence_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_fence<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_sync_query_fence_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_fence(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_fence

} // namespace detail

namespace checked {
template<typename Connection>
using query_fence = detail::query_fence<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_sync_query_fence)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_fence = detail::query_fence<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_sync_query_fence_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_fence<Connection>
query_fence(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_fence<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_fence<Connection>
query_fence_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_fence<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
await_fence_checked(Connection && c, uint32_t fence_list_len, const ::xcb_sync_fence_t * fence_list)
{
    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_await_fence_checked(std::forward<Connection>(c), fence_list_len, fence_list));
}

template<typename Connection>
void
await_fence(Connection && c, uint32_t fence_list_len, const ::xcb_sync_fence_t * fence_list)
{
    ::xcb_sync_await_fence(std::forward<Connection>(c), fence_list_len, fence_list);
}

template<typename Connection, typename Fence_list_Iterator>
void
await_fence_checked(Connection && c, Fence_list_Iterator fence_list_begin, Fence_list_Iterator fence_list_end)
{
    typedef typename value_type<Fence_list_Iterator, ! std::is_pointer<Fence_list_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> fence_list =
    { value_iterator<Fence_list_Iterator>(fence_list_begin), value_iterator<Fence_list_Iterator>(fence_list_end) };

    cppual::generic::check<Connection, cppual::sync::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_sync_await_fence_checked(std::forward<Connection>(c), static_cast<uint32_t>(fence_list.size()), const_cast<const vector_type *>(fence_list.data())));
}

template<typename Connection, typename Fence_list_Iterator>
void
await_fence(Connection && c, Fence_list_Iterator fence_list_begin, Fence_list_Iterator fence_list_end)
{
    typedef typename value_type<Fence_list_Iterator, ! std::is_pointer<Fence_list_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> fence_list =
    { value_iterator<Fence_list_Iterator>(fence_list_begin), value_iterator<Fence_list_Iterator>(fence_list_end) };

    ::xcb_sync_await_fence(std::forward<Connection>(c), static_cast<uint32_t>(fence_list.size()), const_cast<const vector_type *>(fence_list.data()));
}






template<typename Derived, typename Connection>
class alarm
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_sync_alarm_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~alarm(void) {}

    template<typename ... Parameter>
    void
    create_checked(Parameter && ... parameter) const
    {
        cppual::sync::create_alarm_checked(connection(),
                                        resource(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
        cppual::sync::create_alarm(connection(),
                                resource(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_checked(Parameter && ... parameter) const
    {
        cppual::sync::change_alarm_checked(connection(),
                                        resource(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change(Parameter && ... parameter) const
    {
        cppual::sync::change_alarm(connection(),
                                resource(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::sync::destroy_alarm_checked(connection(),
                                         resource(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::sync::destroy_alarm(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query(Parameter && ... parameter) const
    -> reply::checked::query_alarm<Connection>
    {
        return cppual::sync::query_alarm(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_alarm<Connection>
    {
        return cppual::sync::query_alarm_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }



}; // class alarm

template<typename Derived, typename Connection>
class counter
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_sync_counter_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~counter(void) {}

    template<typename ... Parameter>
    void
    create_checked(Parameter && ... parameter) const
    {
        cppual::sync::create_counter_checked(connection(),
                                          resource(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
        cppual::sync::create_counter(connection(),
                                  resource(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::sync::destroy_counter_checked(connection(),
                                           resource(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::sync::destroy_counter(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query(Parameter && ... parameter) const
    -> reply::checked::query_counter<Connection>
    {
        return cppual::sync::query_counter(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_counter<Connection>
    {
        return cppual::sync::query_counter_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_checked(Parameter && ... parameter) const
    {
        cppual::sync::change_counter_checked(connection(),
                                          resource(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change(Parameter && ... parameter) const
    {
        cppual::sync::change_counter(connection(),
                                  resource(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_checked(Parameter && ... parameter) const
    {
        cppual::sync::set_counter_checked(connection(),
                                       resource(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set(Parameter && ... parameter) const
    {
        cppual::sync::set_counter(connection(),
                               resource(),
                               std::forward<Parameter>(parameter) ...);
    }



}; // class counter

















template<typename Derived, typename Connection>
class fence
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_sync_fence_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~fence(void) {}

    template<typename ... Parameter>
    void
    trigger_checked(Parameter && ... parameter) const
    {
        cppual::sync::trigger_fence_checked(connection(),
                                         resource(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    trigger(Parameter && ... parameter) const
    {
        cppual::sync::trigger_fence(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    reset_checked(Parameter && ... parameter) const
    {
        cppual::sync::reset_fence_checked(connection(),
                                       resource(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    reset(Parameter && ... parameter) const
    {
        cppual::sync::reset_fence(connection(),
                               resource(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::sync::destroy_fence_checked(connection(),
                                         resource(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::sync::destroy_fence(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query(Parameter && ... parameter) const
    -> reply::checked::query_fence<Connection>
    {
        return cppual::sync::query_fence(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_fence<Connection>
    {
        return cppual::sync::query_fence_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }



}; // class fence











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
    typedef cppual::sync::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    sync(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    initialize(Parameter && ... parameter) const
    -> reply::checked::initialize<Connection>
    {
        return cppual::sync::initialize(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    initialize_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::initialize<Connection>
    {
        return cppual::sync::initialize_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_system_counters(Parameter && ... parameter) const
    -> reply::checked::list_system_counters<Connection>
    {
        return cppual::sync::list_system_counters(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_system_counters_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_system_counters<Connection>
    {
        return cppual::sync::list_system_counters_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_counter_checked(Parameter && ... parameter) const
    {
        cppual::sync::create_counter_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_counter(Parameter && ... parameter) const
    {
        cppual::sync::create_counter(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_counter_checked(Parameter && ... parameter) const
    {
        cppual::sync::destroy_counter_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_counter(Parameter && ... parameter) const
    {
        cppual::sync::destroy_counter(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_counter(Parameter && ... parameter) const
    -> reply::checked::query_counter<Connection>
    {
        return cppual::sync::query_counter(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_counter_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_counter<Connection>
    {
        return cppual::sync::query_counter_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    await_checked(Parameter && ... parameter) const
    {
        cppual::sync::await_checked(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    await(Parameter && ... parameter) const
    {
        cppual::sync::await(connection(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_counter_checked(Parameter && ... parameter) const
    {
        cppual::sync::change_counter_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_counter(Parameter && ... parameter) const
    {
        cppual::sync::change_counter(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_counter_checked(Parameter && ... parameter) const
    {
        cppual::sync::set_counter_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_counter(Parameter && ... parameter) const
    {
        cppual::sync::set_counter(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_alarm_checked(Parameter && ... parameter) const
    {
        cppual::sync::create_alarm_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_alarm(Parameter && ... parameter) const
    {
        cppual::sync::create_alarm(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_alarm_checked(Parameter && ... parameter) const
    {
        cppual::sync::change_alarm_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_alarm(Parameter && ... parameter) const
    {
        cppual::sync::change_alarm(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_alarm_checked(Parameter && ... parameter) const
    {
        cppual::sync::destroy_alarm_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_alarm(Parameter && ... parameter) const
    {
        cppual::sync::destroy_alarm(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_alarm(Parameter && ... parameter) const
    -> reply::checked::query_alarm<Connection>
    {
        return cppual::sync::query_alarm(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_alarm_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_alarm<Connection>
    {
        return cppual::sync::query_alarm_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_priority_checked(Parameter && ... parameter) const
    {
        cppual::sync::set_priority_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_priority(Parameter && ... parameter) const
    {
        cppual::sync::set_priority(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_priority(Parameter && ... parameter) const
    -> reply::checked::get_priority<Connection>
    {
        return cppual::sync::get_priority(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_priority_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_priority<Connection>
    {
        return cppual::sync::get_priority_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_fence_checked(Parameter && ... parameter) const
    {
        cppual::sync::create_fence_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_fence(Parameter && ... parameter) const
    {
        cppual::sync::create_fence(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    trigger_fence_checked(Parameter && ... parameter) const
    {
        cppual::sync::trigger_fence_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    trigger_fence(Parameter && ... parameter) const
    {
        cppual::sync::trigger_fence(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    reset_fence_checked(Parameter && ... parameter) const
    {
        cppual::sync::reset_fence_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    reset_fence(Parameter && ... parameter) const
    {
        cppual::sync::reset_fence(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_fence_checked(Parameter && ... parameter) const
    {
        cppual::sync::destroy_fence_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_fence(Parameter && ... parameter) const
    {
        cppual::sync::destroy_fence(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_fence(Parameter && ... parameter) const
    -> reply::checked::query_fence<Connection>
    {
        return cppual::sync::query_fence(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_fence_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_fence<Connection>
    {
        return cppual::sync::query_fence_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    await_fence_checked(Parameter && ... parameter) const
    {
        cppual::sync::await_fence_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    await_fence(Parameter && ... parameter) const
    {
        cppual::sync::await_fence(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::sync::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::sync::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - m_first_event) {

        case XCB_SYNC_COUNTER_NOTIFY:
            handler(cppual::sync::event::counter_notify<Connection>(m_c, m_first_event, event));
            return true;

        case XCB_SYNC_ALARM_NOTIFY:
            handler(cppual::sync::event::alarm_notify<Connection>(m_c, m_first_event, event));
            return true;

        };

        return false;
    }

protected:
    Connection m_c;
    uint8_t m_first_event;
}; // class dispatcher

} // namespace event

namespace error {

class dispatcher
{
public:
    typedef cppual::sync::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::sync::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - m_first_error) {

        case XCB_SYNC_COUNTER: // 0
            throw cppual::sync::error::counter(error);

        case XCB_SYNC_ALARM: // 1
            throw cppual::sync::error::alarm(error);

        };
    }

protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace xpp::sync

#endif // CPPUAL_SYNC_HPP
