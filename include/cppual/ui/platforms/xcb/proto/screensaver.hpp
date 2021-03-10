#ifndef CPPUAL_SCREENSAVER_HPP
#define CPPUAL_SCREENSAVER_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/screensaver.h>

namespace cppual { namespace screensaver {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_screensaver_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_screensaver_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::screensaver::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::screensaver::event::dispatcher<Connection>;
    using error_dispatcher = cppual::screensaver::error::dispatcher;
};

namespace event {
template<typename Connection>
class notify
        : public cppual::generic::event<xcb_screensaver_notify_event_t>
{
public:
    typedef cppual::screensaver::extension extension;

    typedef cppual::generic::event<xcb_screensaver_notify_event_t> base;

    template<typename C>
    notify(C && c,
           uint8_t first_event,
           const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_SCREENSAVER_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::screensaver::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_SCREENSAVER_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->root),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->root,
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
        return make()(this->m_c,
                      (*this)->window,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class notify



} // namespace event



namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_version
        : public cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_screensaver_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_screensaver_query_version_reply),
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
SIGNATURE(xcb_screensaver_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_screensaver_query_version_unchecked)>;
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
class query_info
        : public cppual::generic::reply<query_info<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_screensaver_query_info_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_info<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_screensaver_query_info_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_info(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
            saver_window(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->saver_window),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      this->get()->saver_window,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class query_info

} // namespace detail

namespace checked {
template<typename Connection>
using query_info = detail::query_info<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_screensaver_query_info)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_info = detail::query_info<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_screensaver_query_info_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_info<Connection>
query_info(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_info<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_info<Connection>
query_info_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_info<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_input_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::screensaver::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_screensaver_select_input_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_input(Parameter && ... parameter)
{
    ::xcb_screensaver_select_input(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_attributes_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::screensaver::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_screensaver_set_attributes_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_attributes(Parameter && ... parameter)
{
    ::xcb_screensaver_set_attributes(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
unset_attributes_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::screensaver::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_screensaver_unset_attributes_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
unset_attributes(Parameter && ... parameter)
{
    ::xcb_screensaver_unset_attributes(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
suspend_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::screensaver::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_screensaver_suspend_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
suspend(Parameter && ... parameter)
{
    ::xcb_screensaver_suspend(std::forward<Parameter>(parameter) ...);
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
    typedef cppual::screensaver::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    screensaver(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::screensaver::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::screensaver::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_info(Parameter && ... parameter) const
    -> reply::checked::query_info<Connection>
    {
        return cppual::screensaver::query_info(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_info<Connection>
    {
        return cppual::screensaver::query_info_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_input_checked(Parameter && ... parameter) const
    {
        cppual::screensaver::select_input_checked(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_input(Parameter && ... parameter) const
    {
        cppual::screensaver::select_input(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_attributes_checked(Parameter && ... parameter) const
    {
        cppual::screensaver::set_attributes_checked(connection(),
                                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_attributes(Parameter && ... parameter) const
    {
        cppual::screensaver::set_attributes(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    unset_attributes_checked(Parameter && ... parameter) const
    {
        cppual::screensaver::unset_attributes_checked(connection(),
                                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    unset_attributes(Parameter && ... parameter) const
    {
        cppual::screensaver::unset_attributes(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    suspend_checked(Parameter && ... parameter) const
    {
        cppual::screensaver::suspend_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    suspend(Parameter && ... parameter) const
    {
        cppual::screensaver::suspend(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::screensaver::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::screensaver::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - m_first_event) {

        case XCB_SCREENSAVER_NOTIFY:
            handler(cppual::screensaver::event::notify<Connection>(m_c, m_first_event, event));
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
    typedef cppual::screensaver::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::screensaver::extension & extension)
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


} } // namespace cppual::screensaver

#endif // CPPUAL_SCREENSAVER_HPP
