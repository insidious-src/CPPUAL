#ifndef CPPUAL_XEVIE_HPP
#define CPPUAL_XEVIE_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/xevie.h>

namespace cppual { namespace xevie {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_xevie_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_xevie_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::xevie::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::xevie::event::dispatcher<Connection>;
    using error_dispatcher = cppual::xevie::error::dispatcher;
};


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_version
        : public cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xevie_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xevie_query_version_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_version(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_version

} // namespace detail

namespace checked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xevie_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xevie_query_version_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_version<Connection>
query_version(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_version<Connection>
query_version_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class start
        : public cppual::generic::reply<start<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xevie_start_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<start<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xevie_start_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    start(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class start

} // namespace detail

namespace checked {
template<typename Connection>
using start = detail::start<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xevie_start)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using start = detail::start<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xevie_start_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::start<Connection>
start(Connection && c, Parameter && ... parameter)
{
    return reply::checked::start<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::start<Connection>
start_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::start<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class end
        : public cppual::generic::reply<end<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xevie_end_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<end<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xevie_end_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    end(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class end

} // namespace detail

namespace checked {
template<typename Connection>
using end = detail::end<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xevie_end)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using end = detail::end<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xevie_end_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::end<Connection>
end(Connection && c, Parameter && ... parameter)
{
    return reply::checked::end<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::end<Connection>
end_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::end<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class send
        : public cppual::generic::reply<send<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xevie_send_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<send<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xevie_send_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    send(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class send

} // namespace detail

namespace checked {
template<typename Connection>
using send = detail::send<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xevie_send)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using send = detail::send<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xevie_send_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::send<Connection>
send(Connection && c, Parameter && ... parameter)
{
    return reply::checked::send<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::send<Connection>
send_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::send<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class select_input
        : public cppual::generic::reply<select_input<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xevie_select_input_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<select_input<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xevie_select_input_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    select_input(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class select_input

} // namespace detail

namespace checked {
template<typename Connection>
using select_input = detail::select_input<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xevie_select_input)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using select_input = detail::select_input<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xevie_select_input_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::select_input<Connection>
select_input(Connection && c, Parameter && ... parameter)
{
    return reply::checked::select_input<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::select_input<Connection>
select_input_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::select_input<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

































template<typename Derived, typename Connection>
class xevent
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_xevie_event_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~xevent(void) {}

    template<typename ... Parameter>
    auto
    send(Parameter && ... parameter) const
    -> reply::checked::send<Connection>
    {
        return cppual::xevie::send(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    send_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::send<Connection>
    {
        return cppual::xevie::send_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }



}; // class event


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
    typedef cppual::xevie::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    xevie(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::xevie::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::xevie::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    start(Parameter && ... parameter) const
    -> reply::checked::start<Connection>
    {
        return cppual::xevie::start(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    start_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::start<Connection>
    {
        return cppual::xevie::start_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    end(Parameter && ... parameter) const
    -> reply::checked::end<Connection>
    {
        return cppual::xevie::end(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    end_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::end<Connection>
    {
        return cppual::xevie::end_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    send(Parameter && ... parameter) const
    -> reply::checked::send<Connection>
    {
        return cppual::xevie::send(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    send_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::send<Connection>
    {
        return cppual::xevie::send_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    select_input(Parameter && ... parameter) const
    -> reply::checked::select_input<Connection>
    {
        return cppual::xevie::select_input(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    select_input_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::select_input<Connection>
    {
        return cppual::xevie::select_input_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::xevie::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::xevie::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler,
               const std::shared_ptr<xcb_generic_event_t> &) const
    {
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
    typedef cppual::xevie::extension extension;

    dispatcher(uint8_t first_error)
        : _M_first_error(first_error)
    {}

    dispatcher(const cppual::xevie::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> &) const
    {
    }

protected:
    uint8_t _M_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::xevie

#endif // CPPUAL_XEVIE_HPP
