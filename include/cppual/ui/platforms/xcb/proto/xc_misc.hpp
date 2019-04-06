#ifndef CPPUAL_XC_MISC_HPP
#define CPPUAL_XC_MISC_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/xc_misc.h>

namespace cppual { namespace xc_misc {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_xc_misc_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_xc_misc_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::xc_misc::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::xc_misc::event::dispatcher<Connection>;
    using error_dispatcher = cppual::xc_misc::error::dispatcher;
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
        SIGNATURE(xcb_xc_misc_get_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xc_misc_get_version_reply),
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
SIGNATURE(xcb_xc_misc_get_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_version = detail::get_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xc_misc_get_version_unchecked)>;
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
class get_xid_range
        : public cppual::generic::reply<get_xid_range<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xc_misc_get_xid_range_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_xid_range<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xc_misc_get_xid_range_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_xid_range(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_xid_range

} // namespace detail

namespace checked {
template<typename Connection>
using get_xid_range = detail::get_xid_range<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xc_misc_get_xid_range)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_xid_range = detail::get_xid_range<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xc_misc_get_xid_range_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_xid_range<Connection>
get_xid_range(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_xid_range<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_xid_range<Connection>
get_xid_range_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_xid_range<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_xid_list
        : public cppual::generic::reply<get_xid_list<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xc_misc_get_xid_list_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_xid_list<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xc_misc_get_xid_list_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_xid_list(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xc_misc_get_xid_list_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xc_misc_get_xid_list_ids),
            SIGNATURE(xcb_xc_misc_get_xid_list_ids_length)>
            >
            ids(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xc_misc_get_xid_list_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xc_misc_get_xid_list_ids),
                SIGNATURE(xcb_xc_misc_get_xid_list_ids_length)>
                >(this->m_c, this->get());
    }
}; // class get_xid_list

} // namespace detail

namespace checked {
template<typename Connection>
using get_xid_list = detail::get_xid_list<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xc_misc_get_xid_list)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_xid_list = detail::get_xid_list<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xc_misc_get_xid_list_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_xid_list<Connection>
get_xid_list(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_xid_list<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_xid_list<Connection>
get_xid_list_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_xid_list<Connection>(
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
    typedef cppual::xc_misc::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    xc_misc(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    get_version(Parameter && ... parameter) const
    -> reply::checked::get_version<Connection>
    {
        return cppual::xc_misc::get_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_version<Connection>
    {
        return cppual::xc_misc::get_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_xid_range(Parameter && ... parameter) const
    -> reply::checked::get_xid_range<Connection>
    {
        return cppual::xc_misc::get_xid_range(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_xid_range_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_xid_range<Connection>
    {
        return cppual::xc_misc::get_xid_range_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_xid_list(Parameter && ... parameter) const
    -> reply::checked::get_xid_list<Connection>
    {
        return cppual::xc_misc::get_xid_list(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_xid_list_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_xid_list<Connection>
    {
        return cppual::xc_misc::get_xid_list_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::xc_misc::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::xc_misc::extension & extension)
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
    typedef cppual::xc_misc::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::xc_misc::extension & extension)
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


} } // namespace xpp::xc_misc

#endif // CPPUAL_XC_MISC_HPP
