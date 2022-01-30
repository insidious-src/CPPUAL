#ifndef CPPUAL_BIG_REQUESTS_HPP
#define CPPUAL_BIG_REQUESTS_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/bigreq.h>

namespace cppual { namespace big_requests {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_big_requests_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_big_requests_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::big_requests::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::big_requests::event::dispatcher<Connection>;
    using error_dispatcher = cppual::big_requests::error::dispatcher;
};


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class enable
        : public cppual::generic::reply<enable<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_big_requests_enable_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<enable<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_big_requests_enable_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    enable(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class enable

} // namespace detail

namespace checked {
template<typename Connection>
using enable = detail::enable<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_big_requests_enable)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using enable = detail::enable<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_big_requests_enable_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::enable<Connection>
enable(Connection && c, Parameter && ... parameter)
{
    return reply::checked::enable<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::enable<Connection>
enable_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::enable<Connection>(
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
    typedef cppual::big_requests::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    big_requests(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    enable(Parameter && ... parameter) const
    -> reply::checked::enable<Connection>
    {
        return cppual::big_requests::enable(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    enable_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::enable<Connection>
    {
        return cppual::big_requests::enable_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::big_requests::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::big_requests::extension & extension)
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
    typedef cppual::big_requests::extension extension;

    dispatcher(uint8_t first_error)
        : _M_first_error(first_error)
    {}

    dispatcher(const cppual::big_requests::extension & extension)
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


} } // namespace cppual::big_requests

#endif // CPPUAL_BIG_REQUESTS_HPP
