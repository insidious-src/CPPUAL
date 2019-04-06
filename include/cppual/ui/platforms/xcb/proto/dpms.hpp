#ifndef CPPUAL_DPMS_HPP
#define CPPUAL_DPMS_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/dpms.h>

namespace cppual { namespace dpms {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_dpms_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_dpms_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::dpms::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::dpms::event::dispatcher<Connection>;
    using error_dispatcher = cppual::dpms::error::dispatcher;
};


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_version
        : public cppual::generic::reply<get_version<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dpms_get_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dpms_get_version_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_version(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_version

} // namespace detail

namespace checked {
template<typename Connection>
using get_version = detail::get_version<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dpms_get_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_version = detail::get_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dpms_get_version_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_version<Connection>
get_version(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_version<Connection>
get_version_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_version<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class capable
        : public cppual::generic::reply<capable<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dpms_capable_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<capable<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dpms_capable_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    capable(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class capable

} // namespace detail

namespace checked {
template<typename Connection>
using capable = detail::capable<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dpms_capable)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using capable = detail::capable<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dpms_capable_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::capable<Connection>
capable(Connection && c, Parameter && ... parameter)
{
    return reply::checked::capable<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::capable<Connection>
capable_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::capable<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_timeouts
        : public cppual::generic::reply<get_timeouts<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dpms_get_timeouts_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_timeouts<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dpms_get_timeouts_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_timeouts(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_timeouts

} // namespace detail

namespace checked {
template<typename Connection>
using get_timeouts = detail::get_timeouts<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dpms_get_timeouts)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_timeouts = detail::get_timeouts<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dpms_get_timeouts_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_timeouts<Connection>
get_timeouts(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_timeouts<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_timeouts<Connection>
get_timeouts_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_timeouts<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_timeouts_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::dpms::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_dpms_set_timeouts_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_timeouts(Parameter && ... parameter)
{
    ::xcb_dpms_set_timeouts(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
enable_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::dpms::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_dpms_enable_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
enable(Parameter && ... parameter)
{
    ::xcb_dpms_enable(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
disable_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::dpms::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_dpms_disable_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
disable(Parameter && ... parameter)
{
    ::xcb_dpms_disable(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
force_level_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::dpms::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_dpms_force_level_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
force_level(Parameter && ... parameter)
{
    ::xcb_dpms_force_level(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class info
        : public cppual::generic::reply<info<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dpms_info_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<info<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dpms_info_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    info(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class info

} // namespace detail

namespace checked {
template<typename Connection>
using info = detail::info<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dpms_info)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using info = detail::info<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dpms_info_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::info<Connection>
info(Connection && c, Parameter && ... parameter)
{
    return reply::checked::info<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::info<Connection>
info_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::info<Connection>(
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
    typedef cppual::dpms::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    dpms(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    get_version(Parameter && ... parameter) const
    -> reply::checked::get_version<Connection>
    {
        return cppual::dpms::get_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_version<Connection>
    {
        return cppual::dpms::get_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    capable(Parameter && ... parameter) const
    -> reply::checked::capable<Connection>
    {
        return cppual::dpms::capable(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    capable_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::capable<Connection>
    {
        return cppual::dpms::capable_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_timeouts(Parameter && ... parameter) const
    -> reply::checked::get_timeouts<Connection>
    {
        return cppual::dpms::get_timeouts(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_timeouts_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_timeouts<Connection>
    {
        return cppual::dpms::get_timeouts_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_timeouts_checked(Parameter && ... parameter) const
    {
        cppual::dpms::set_timeouts_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_timeouts(Parameter && ... parameter) const
    {
        cppual::dpms::set_timeouts(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    enable_checked(Parameter && ... parameter) const
    {
        cppual::dpms::enable_checked(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    enable(Parameter && ... parameter) const
    {
        cppual::dpms::enable(connection(),
                          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    disable_checked(Parameter && ... parameter) const
    {
        cppual::dpms::disable_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    disable(Parameter && ... parameter) const
    {
        cppual::dpms::disable(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    force_level_checked(Parameter && ... parameter) const
    {
        cppual::dpms::force_level_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    force_level(Parameter && ... parameter) const
    {
        cppual::dpms::force_level(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    info(Parameter && ... parameter) const
    -> reply::checked::info<Connection>
    {
        return cppual::dpms::info(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::info<Connection>
    {
        return cppual::dpms::info_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::dpms::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::dpms::extension & extension)
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
    Connection m_c;
    uint8_t m_first_event;
}; // class dispatcher

} // namespace event

namespace error {

class dispatcher
{
public:
    typedef cppual::dpms::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::dpms::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> &) const
    {
    }

protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace xpp::dpms

#endif // CPPUAL_DPMS_HPP
