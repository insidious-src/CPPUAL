#ifndef CPPUAL_XINERAMA_HPP
#define CPPUAL_XINERAMA_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/xinerama.h>

namespace cppual { namespace xinerama {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_xinerama_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_xinerama_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::xinerama::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::xinerama::event::dispatcher<Connection>;
    using error_dispatcher = cppual::xinerama::error::dispatcher;
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
        SIGNATURE(xcb_xinerama_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xinerama_query_version_reply),
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
SIGNATURE(xcb_xinerama_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xinerama_query_version_unchecked)>;
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
class get_state
        : public cppual::generic::reply<get_state<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xinerama_get_state_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_state<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xinerama_get_state_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_state(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            window(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->window),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->window,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_state

} // namespace detail

namespace checked {
template<typename Connection>
using get_state = detail::get_state<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xinerama_get_state)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_state = detail::get_state<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xinerama_get_state_unchecked)>;
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

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_screen_count
        : public cppual::generic::reply<get_screen_count<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xinerama_get_screen_count_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_screen_count<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xinerama_get_screen_count_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_screen_count(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            window(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->window),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->window,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_screen_count

} // namespace detail

namespace checked {
template<typename Connection>
using get_screen_count = detail::get_screen_count<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xinerama_get_screen_count)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_screen_count = detail::get_screen_count<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xinerama_get_screen_count_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_screen_count<Connection>
get_screen_count(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_screen_count<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_screen_count<Connection>
get_screen_count_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_screen_count<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_screen_size
        : public cppual::generic::reply<get_screen_size<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xinerama_get_screen_size_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_screen_size<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xinerama_get_screen_size_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_screen_size(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            window(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->window),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      this->get()->window,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_screen_size

} // namespace detail

namespace checked {
template<typename Connection>
using get_screen_size = detail::get_screen_size<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xinerama_get_screen_size)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_screen_size = detail::get_screen_size<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xinerama_get_screen_size_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_screen_size<Connection>
get_screen_size(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_screen_size<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_screen_size<Connection>
get_screen_size_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_screen_size<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class is_active
        : public cppual::generic::reply<is_active<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xinerama_is_active_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<is_active<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xinerama_is_active_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    is_active(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class is_active

} // namespace detail

namespace checked {
template<typename Connection>
using is_active = detail::is_active<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xinerama_is_active)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using is_active = detail::is_active<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xinerama_is_active_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::is_active<Connection>
is_active(Connection && c, Parameter && ... parameter)
{
    return reply::checked::is_active<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::is_active<Connection>
is_active_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::is_active<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_screens
        : public cppual::generic::reply<query_screens<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xinerama_query_screens_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_screens<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xinerama_query_screens_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_screens(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xinerama_query_screens_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xinerama_screen_info_t,
            SIGNATURE(xcb_xinerama_query_screens_screen_info),
            SIGNATURE(xcb_xinerama_query_screens_screen_info_length)>
            >
            screen_info(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xinerama_query_screens_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xinerama_screen_info_t,
                SIGNATURE(xcb_xinerama_query_screens_screen_info),
                SIGNATURE(xcb_xinerama_query_screens_screen_info_length)>
                >(this->_M_c, this->get());
    }
}; // class query_screens

} // namespace detail

namespace checked {
template<typename Connection>
using query_screens = detail::query_screens<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xinerama_query_screens)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_screens = detail::query_screens<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xinerama_query_screens_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_screens<Connection>
query_screens(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_screens<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_screens<Connection>
query_screens_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_screens<Connection>(
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
    typedef cppual::xinerama::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    xinerama(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::xinerama::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::xinerama::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_state(Parameter && ... parameter) const
    -> reply::checked::get_state<Connection>
    {
        return cppual::xinerama::get_state(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_state_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_state<Connection>
    {
        return cppual::xinerama::get_state_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_screen_count(Parameter && ... parameter) const
    -> reply::checked::get_screen_count<Connection>
    {
        return cppual::xinerama::get_screen_count(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_screen_count_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_screen_count<Connection>
    {
        return cppual::xinerama::get_screen_count_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_screen_size(Parameter && ... parameter) const
    -> reply::checked::get_screen_size<Connection>
    {
        return cppual::xinerama::get_screen_size(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_screen_size_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_screen_size<Connection>
    {
        return cppual::xinerama::get_screen_size_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    is_active(Parameter && ... parameter) const
    -> reply::checked::is_active<Connection>
    {
        return cppual::xinerama::is_active(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    is_active_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::is_active<Connection>
    {
        return cppual::xinerama::is_active_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_screens(Parameter && ... parameter) const
    -> reply::checked::query_screens<Connection>
    {
        return cppual::xinerama::query_screens(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_screens_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_screens<Connection>
    {
        return cppual::xinerama::query_screens_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::xinerama::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::xinerama::extension & extension)
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
    typedef cppual::xinerama::extension extension;

    dispatcher(uint8_t first_error)
        : _M_first_error(first_error)
    {}

    dispatcher(const cppual::xinerama::extension & extension)
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


} } // namespace cppual::xinerama

#endif // CPPUAL_XINERAMA_HPP
