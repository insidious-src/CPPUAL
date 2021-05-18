#ifndef CPPUAL_XV_HPP
#define CPPUAL_XV_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>
#include <cppual/string.h>

#include <vector>

#include <xcb/xv.h>

namespace cppual { namespace xv {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_xv_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_xv_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::xv::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::xv::event::dispatcher<Connection>;
    using error_dispatcher = cppual::xv::error::dispatcher;
};

namespace event {
template<typename Connection>
class video_notify
        : public cppual::generic::event<xcb_xv_video_notify_event_t>
{
public:
    typedef cppual::xv::extension extension;

    typedef cppual::generic::event<xcb_xv_video_notify_event_t> base;

    template<typename C>
    video_notify(C && c,
                 uint8_t first_event,
                 const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~video_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XV_VIDEO_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xv::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XV_VIDEO_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_drawable_t, typename ... Parameter>
    ReturnType
    drawable(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->drawable),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->drawable,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_xv_port_t, typename ... Parameter>
    ReturnType
    port(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->port),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->port,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class video_notify



} // namespace event


namespace event {
template<typename Connection>
class port_notify
        : public cppual::generic::event<xcb_xv_port_notify_event_t>
{
public:
    typedef cppual::xv::extension extension;

    typedef cppual::generic::event<xcb_xv_port_notify_event_t> base;

    template<typename C>
    port_notify(C && c,
                uint8_t first_event,
                const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~port_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XV_PORT_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xv::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_XV_PORT_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_xv_port_t, typename ... Parameter>
    ReturnType
    port(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->port),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->port,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    attribute(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->attribute),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->attribute,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class port_notify



} // namespace event



namespace error {
class bad_port
        : public cppual::generic::error<bad_port,
        ::xcb_xv_bad_port_error_t>
{
public:
    using cppual::generic::error<bad_port, ::xcb_xv_bad_port_error_t>::error;

    virtual ~bad_port(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XV_BAD_PORT;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::xv::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_XV_BAD_PORT");
    }

protected:
    uint8_t m_first_error;
}; // class bad_port
} // namespace error


namespace error {
class bad_encoding
        : public cppual::generic::error<bad_encoding,
        ::xcb_xv_bad_encoding_error_t>
{
public:
    using cppual::generic::error<bad_encoding, ::xcb_xv_bad_encoding_error_t>::error;

    virtual ~bad_encoding(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XV_BAD_ENCODING;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::xv::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_XV_BAD_ENCODING");
    }

protected:
    uint8_t m_first_error;
}; // class bad_encoding
} // namespace error


namespace error {
class bad_control
        : public cppual::generic::error<bad_control,
        ::xcb_xv_bad_control_error_t>
{
public:
    using cppual::generic::error<bad_control, ::xcb_xv_bad_control_error_t>::error;

    virtual ~bad_control(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XV_BAD_CONTROL;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::xv::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_XV_BAD_CONTROL");
    }

protected:
    uint8_t m_first_error;
}; // class bad_control
} // namespace error


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_extension
        : public cppual::generic::reply<query_extension<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_query_extension_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_extension<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_query_extension_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_extension(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_extension

} // namespace detail

namespace checked {
template<typename Connection>
using query_extension = detail::query_extension<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_query_extension)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_extension = detail::query_extension<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_query_extension_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_extension<Connection>
query_extension(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_extension<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_extension<Connection>
query_extension_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_extension<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_adaptors
        : public cppual::generic::reply<query_adaptors<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_query_adaptors_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_adaptors<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_query_adaptors_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_adaptors(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xv_query_adaptors_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xv_adaptor_info_t,
            SIGNATURE(xcb_xv_adaptor_info_next),
            SIGNATURE(xcb_xv_adaptor_info_sizeof),
            SIGNATURE(xcb_xv_query_adaptors_info_iterator)>
            >
            info(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xv_query_adaptors_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xv_adaptor_info_t,
                SIGNATURE(xcb_xv_adaptor_info_next),
                SIGNATURE(xcb_xv_adaptor_info_sizeof),
                SIGNATURE(xcb_xv_query_adaptors_info_iterator)>
                >(this->m_c, this->get());
    }
}; // class query_adaptors

} // namespace detail

namespace checked {
template<typename Connection>
using query_adaptors = detail::query_adaptors<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_query_adaptors)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_adaptors = detail::query_adaptors<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_query_adaptors_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_adaptors<Connection>
query_adaptors(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_adaptors<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_adaptors<Connection>
query_adaptors_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_adaptors<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_encodings
        : public cppual::generic::reply<query_encodings<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_query_encodings_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_encodings<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_query_encodings_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_encodings(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xv_query_encodings_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xv_encoding_info_t,
            SIGNATURE(xcb_xv_encoding_info_next),
            SIGNATURE(xcb_xv_encoding_info_sizeof),
            SIGNATURE(xcb_xv_query_encodings_info_iterator)>
            >
            info(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xv_query_encodings_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xv_encoding_info_t,
                SIGNATURE(xcb_xv_encoding_info_next),
                SIGNATURE(xcb_xv_encoding_info_sizeof),
                SIGNATURE(xcb_xv_query_encodings_info_iterator)>
                >(this->m_c, this->get());
    }
}; // class query_encodings

} // namespace detail

namespace checked {
template<typename Connection>
using query_encodings = detail::query_encodings<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_query_encodings)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_encodings = detail::query_encodings<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_query_encodings_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_encodings<Connection>
query_encodings(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_encodings<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_encodings<Connection>
query_encodings_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_encodings<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class grab_port
        : public cppual::generic::reply<grab_port<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_grab_port_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<grab_port<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_grab_port_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    grab_port(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
            ::xcb_xv_grab_port_cookie_t
            cookie(xcb_connection_t * const c, ::xcb_xv_port_t port, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
    {
        return base::cookie(c, port, time);
    }
}; // class grab_port

} // namespace detail

namespace checked {
template<typename Connection>
using grab_port = detail::grab_port<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_grab_port)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using grab_port = detail::grab_port<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_grab_port_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::grab_port<Connection>
grab_port(Connection && c, Parameter && ... parameter)
{
    return reply::checked::grab_port<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::grab_port<Connection>
grab_port_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::grab_port<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
ungrab_port_checked(Connection && c, ::xcb_xv_port_t port, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_ungrab_port_checked(std::forward<Connection>(c), port, time));
}

template<typename Connection>
void
ungrab_port(Connection && c, ::xcb_xv_port_t port, ::xcb_timestamp_t time = XCB_TIME_CURRENT_TIME)
{
    ::xcb_xv_ungrab_port(std::forward<Connection>(c), port, time);
}

template<typename Connection, typename ... Parameter>
void
put_video_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_put_video_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
put_video(Parameter && ... parameter)
{
    ::xcb_xv_put_video(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
put_still_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_put_still_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
put_still(Parameter && ... parameter)
{
    ::xcb_xv_put_still(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
get_video_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_get_video_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
get_video(Parameter && ... parameter)
{
    ::xcb_xv_get_video(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
get_still_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_get_still_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
get_still(Parameter && ... parameter)
{
    ::xcb_xv_get_still(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
stop_video_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_stop_video_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
stop_video(Parameter && ... parameter)
{
    ::xcb_xv_stop_video(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_video_notify_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_select_video_notify_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_video_notify(Parameter && ... parameter)
{
    ::xcb_xv_select_video_notify(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_port_notify_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_select_port_notify_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_port_notify(Parameter && ... parameter)
{
    ::xcb_xv_select_port_notify(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_best_size
        : public cppual::generic::reply<query_best_size<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_query_best_size_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_best_size<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_query_best_size_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_best_size(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_best_size

} // namespace detail

namespace checked {
template<typename Connection>
using query_best_size = detail::query_best_size<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_query_best_size)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_best_size = detail::query_best_size<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_query_best_size_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_best_size<Connection>
query_best_size(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_best_size<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_best_size<Connection>
query_best_size_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_best_size<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_port_attribute_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_set_port_attribute_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_port_attribute(Parameter && ... parameter)
{
    ::xcb_xv_set_port_attribute(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_port_attribute
        : public cppual::generic::reply<get_port_attribute<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_get_port_attribute_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_port_attribute<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_get_port_attribute_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_port_attribute(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_port_attribute

} // namespace detail

namespace checked {
template<typename Connection>
using get_port_attribute = detail::get_port_attribute<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_get_port_attribute)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_port_attribute = detail::get_port_attribute<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_get_port_attribute_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_port_attribute<Connection>
get_port_attribute(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_port_attribute<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_port_attribute<Connection>
get_port_attribute_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_port_attribute<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_port_attributes
        : public cppual::generic::reply<query_port_attributes<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_query_port_attributes_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_port_attributes<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_query_port_attributes_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_port_attributes(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xv_query_port_attributes_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xv_attribute_info_t,
            SIGNATURE(xcb_xv_attribute_info_next),
            SIGNATURE(xcb_xv_attribute_info_sizeof),
            SIGNATURE(xcb_xv_query_port_attributes_attributes_iterator)>
            >
            attributes(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xv_query_port_attributes_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xv_attribute_info_t,
                SIGNATURE(xcb_xv_attribute_info_next),
                SIGNATURE(xcb_xv_attribute_info_sizeof),
                SIGNATURE(xcb_xv_query_port_attributes_attributes_iterator)>
                >(this->m_c, this->get());
    }
}; // class query_port_attributes

} // namespace detail

namespace checked {
template<typename Connection>
using query_port_attributes = detail::query_port_attributes<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_query_port_attributes)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_port_attributes = detail::query_port_attributes<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_query_port_attributes_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_port_attributes<Connection>
query_port_attributes(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_port_attributes<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_port_attributes<Connection>
query_port_attributes_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_port_attributes<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_image_formats
        : public cppual::generic::reply<list_image_formats<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_list_image_formats_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_image_formats<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_list_image_formats_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_image_formats(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xv_list_image_formats_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xv_image_format_info_t,
            SIGNATURE(xcb_xv_list_image_formats_format),
            SIGNATURE(xcb_xv_list_image_formats_format_length)>
            >
            format(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xv_list_image_formats_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xv_image_format_info_t,
                SIGNATURE(xcb_xv_list_image_formats_format),
                SIGNATURE(xcb_xv_list_image_formats_format_length)>
                >(this->m_c, this->get());
    }
}; // class list_image_formats

} // namespace detail

namespace checked {
template<typename Connection>
using list_image_formats = detail::list_image_formats<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_list_image_formats)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_image_formats = detail::list_image_formats<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_list_image_formats_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_image_formats<Connection>
list_image_formats(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_image_formats<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_image_formats<Connection>
list_image_formats_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_image_formats<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_image_attributes
        : public cppual::generic::reply<query_image_attributes<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xv_query_image_attributes_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_image_attributes<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xv_query_image_attributes_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_image_attributes(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xv_query_image_attributes_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xv_query_image_attributes_pitches),
            SIGNATURE(xcb_xv_query_image_attributes_pitches_length)>
            >
            pitches(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xv_query_image_attributes_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xv_query_image_attributes_pitches),
                SIGNATURE(xcb_xv_query_image_attributes_pitches_length)>
                >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_xv_query_image_attributes_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xv_query_image_attributes_offsets),
            SIGNATURE(xcb_xv_query_image_attributes_offsets_length)>
            >
            offsets(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xv_query_image_attributes_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xv_query_image_attributes_offsets),
                SIGNATURE(xcb_xv_query_image_attributes_offsets_length)>
                >(this->m_c, this->get());
    }
}; // class query_image_attributes

} // namespace detail

namespace checked {
template<typename Connection>
using query_image_attributes = detail::query_image_attributes<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xv_query_image_attributes)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_image_attributes = detail::query_image_attributes<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xv_query_image_attributes_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_image_attributes<Connection>
query_image_attributes(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_image_attributes<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_image_attributes<Connection>
query_image_attributes_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_image_attributes<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
put_image_checked(Connection && c, ::xcb_xv_port_t port, ::xcb_drawable_t drawable, ::xcb_gcontext_t gc, uint32_t id, int16_t src_x, int16_t src_y, uint16_t src_w, uint16_t src_h, int16_t drw_x, int16_t drw_y, uint16_t drw_w, uint16_t drw_h, uint16_t width, uint16_t height, uint32_t data_len, const uint8_t * data)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_put_image_checked(std::forward<Connection>(c), port, drawable, gc, id, src_x, src_y, src_w, src_h, drw_x, drw_y, drw_w, drw_h, width, height, data_len, data));
}

template<typename Connection>
void
put_image(Connection && c, ::xcb_xv_port_t port, ::xcb_drawable_t drawable, ::xcb_gcontext_t gc, uint32_t id, int16_t src_x, int16_t src_y, uint16_t src_w, uint16_t src_h, int16_t drw_x, int16_t drw_y, uint16_t drw_w, uint16_t drw_h, uint16_t width, uint16_t height, uint32_t data_len, const uint8_t * data)
{
    ::xcb_xv_put_image(std::forward<Connection>(c), port, drawable, gc, id, src_x, src_y, src_w, src_h, drw_x, drw_y, drw_w, drw_h, width, height, data_len, data);
}

template<typename Connection, typename Data_Iterator>
void
put_image_checked(Connection && c, ::xcb_xv_port_t port, ::xcb_drawable_t drawable, ::xcb_gcontext_t gc, uint32_t id, int16_t src_x, int16_t src_y, uint16_t src_w, uint16_t src_h, int16_t drw_x, int16_t drw_y, uint16_t drw_w, uint16_t drw_h, uint16_t width, uint16_t height, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_put_image_checked(std::forward<Connection>(c), port, drawable, gc, id, src_x, src_y, src_w, src_h, drw_x, drw_y, drw_w, drw_h, width, height, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data())));
}

template<typename Connection, typename Data_Iterator>
void
put_image(Connection && c, ::xcb_xv_port_t port, ::xcb_drawable_t drawable, ::xcb_gcontext_t gc, uint32_t id, int16_t src_x, int16_t src_y, uint16_t src_w, uint16_t src_h, int16_t drw_x, int16_t drw_y, uint16_t drw_w, uint16_t drw_h, uint16_t width, uint16_t height, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    ::xcb_xv_put_image(std::forward<Connection>(c), port, drawable, gc, id, src_x, src_y, src_w, src_h, drw_x, drw_y, drw_w, drw_h, width, height, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data()));
}

template<typename Connection, typename ... Parameter>
void
shm_put_image_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xv::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xv_shm_put_image_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
shm_put_image(Parameter && ... parameter)
{
    ::xcb_xv_shm_put_image(std::forward<Parameter>(parameter) ...);
}













template<typename Derived, typename Connection>
class port
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_xv_port_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~port(void) {}

    template<typename ... Parameter>
    auto
    query_encodings(Parameter && ... parameter) const
    -> reply::checked::query_encodings<Connection>
    {
        return cppual::xv::query_encodings(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_encodings_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_encodings<Connection>
    {
        return cppual::xv::query_encodings_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    grab(Parameter && ... parameter) const
    -> reply::checked::grab_port<Connection>
    {
        return cppual::xv::grab_port(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    grab_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::grab_port<Connection>
    {
        return cppual::xv::grab_port_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    ungrab_checked(Parameter && ... parameter) const
    {
        cppual::xv::ungrab_port_checked(connection(),
                                     resource(),
                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    ungrab(Parameter && ... parameter) const
    {
        cppual::xv::ungrab_port(connection(),
                             resource(),
                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    put_video_checked(Parameter && ... parameter) const
    {
        cppual::xv::put_video_checked(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    put_video(Parameter && ... parameter) const
    {
        cppual::xv::put_video(connection(),
                           resource(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    put_still_checked(Parameter && ... parameter) const
    {
        cppual::xv::put_still_checked(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    put_still(Parameter && ... parameter) const
    {
        cppual::xv::put_still(connection(),
                           resource(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    get_video_checked(Parameter && ... parameter) const
    {
        cppual::xv::get_video_checked(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    get_video(Parameter && ... parameter) const
    {
        cppual::xv::get_video(connection(),
                           resource(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    get_still_checked(Parameter && ... parameter) const
    {
        cppual::xv::get_still_checked(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    get_still(Parameter && ... parameter) const
    {
        cppual::xv::get_still(connection(),
                           resource(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    stop_video_checked(Parameter && ... parameter) const
    {
        cppual::xv::stop_video_checked(connection(),
                                    resource(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    stop_video(Parameter && ... parameter) const
    {
        cppual::xv::stop_video(connection(),
                            resource(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_notify_checked(Parameter && ... parameter) const
    {
        cppual::xv::select_port_notify_checked(connection(),
                                            resource(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_notify(Parameter && ... parameter) const
    {
        cppual::xv::select_port_notify(connection(),
                                    resource(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_best_size(Parameter && ... parameter) const
    -> reply::checked::query_best_size<Connection>
    {
        return cppual::xv::query_best_size(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_best_size_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_best_size<Connection>
    {
        return cppual::xv::query_best_size_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_attribute_checked(Parameter && ... parameter) const
    {
        cppual::xv::set_port_attribute_checked(connection(),
                                            resource(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_attribute(Parameter && ... parameter) const
    {
        cppual::xv::set_port_attribute(connection(),
                                    resource(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_attribute(Parameter && ... parameter) const
    -> reply::checked::get_port_attribute<Connection>
    {
        return cppual::xv::get_port_attribute(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_attribute_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_port_attribute<Connection>
    {
        return cppual::xv::get_port_attribute_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_attributes(Parameter && ... parameter) const
    -> reply::checked::query_port_attributes<Connection>
    {
        return cppual::xv::query_port_attributes(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_attributes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_port_attributes<Connection>
    {
        return cppual::xv::query_port_attributes_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_image_formats(Parameter && ... parameter) const
    -> reply::checked::list_image_formats<Connection>
    {
        return cppual::xv::list_image_formats(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_image_formats_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_image_formats<Connection>
    {
        return cppual::xv::list_image_formats_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_image_attributes(Parameter && ... parameter) const
    -> reply::checked::query_image_attributes<Connection>
    {
        return cppual::xv::query_image_attributes(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_image_attributes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_image_attributes<Connection>
    {
        return cppual::xv::query_image_attributes_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    put_image_checked(Parameter && ... parameter) const
    {
        cppual::xv::put_image_checked(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    put_image(Parameter && ... parameter) const
    {
        cppual::xv::put_image(connection(),
                           resource(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    shm_put_image_checked(Parameter && ... parameter) const
    {
        cppual::xv::shm_put_image_checked(connection(),
                                       resource(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    shm_put_image(Parameter && ... parameter) const
    {
        cppual::xv::shm_put_image(connection(),
                               resource(),
                               std::forward<Parameter>(parameter) ...);
    }



}; // class port






















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
    typedef cppual::xv::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    xv(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_extension(Parameter && ... parameter) const
    -> reply::checked::query_extension<Connection>
    {
        return cppual::xv::query_extension(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_extension_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_extension<Connection>
    {
        return cppual::xv::query_extension_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_adaptors(Parameter && ... parameter) const
    -> reply::checked::query_adaptors<Connection>
    {
        return cppual::xv::query_adaptors(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_adaptors_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_adaptors<Connection>
    {
        return cppual::xv::query_adaptors_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_encodings(Parameter && ... parameter) const
    -> reply::checked::query_encodings<Connection>
    {
        return cppual::xv::query_encodings(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_encodings_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_encodings<Connection>
    {
        return cppual::xv::query_encodings_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    grab_port(Parameter && ... parameter) const
    -> reply::checked::grab_port<Connection>
    {
        return cppual::xv::grab_port(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    grab_port_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::grab_port<Connection>
    {
        return cppual::xv::grab_port_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    ungrab_port_checked(Parameter && ... parameter) const
    {
        cppual::xv::ungrab_port_checked(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    ungrab_port(Parameter && ... parameter) const
    {
        cppual::xv::ungrab_port(connection(),
                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    put_video_checked(Parameter && ... parameter) const
    {
        cppual::xv::put_video_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    put_video(Parameter && ... parameter) const
    {
        cppual::xv::put_video(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    put_still_checked(Parameter && ... parameter) const
    {
        cppual::xv::put_still_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    put_still(Parameter && ... parameter) const
    {
        cppual::xv::put_still(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    get_video_checked(Parameter && ... parameter) const
    {
        cppual::xv::get_video_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    get_video(Parameter && ... parameter) const
    {
        cppual::xv::get_video(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    get_still_checked(Parameter && ... parameter) const
    {
        cppual::xv::get_still_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    get_still(Parameter && ... parameter) const
    {
        cppual::xv::get_still(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    stop_video_checked(Parameter && ... parameter) const
    {
        cppual::xv::stop_video_checked(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    stop_video(Parameter && ... parameter) const
    {
        cppual::xv::stop_video(connection(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_video_notify_checked(Parameter && ... parameter) const
    {
        cppual::xv::select_video_notify_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_video_notify(Parameter && ... parameter) const
    {
        cppual::xv::select_video_notify(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_port_notify_checked(Parameter && ... parameter) const
    {
        cppual::xv::select_port_notify_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_port_notify(Parameter && ... parameter) const
    {
        cppual::xv::select_port_notify(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_best_size(Parameter && ... parameter) const
    -> reply::checked::query_best_size<Connection>
    {
        return cppual::xv::query_best_size(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_best_size_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_best_size<Connection>
    {
        return cppual::xv::query_best_size_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_port_attribute_checked(Parameter && ... parameter) const
    {
        cppual::xv::set_port_attribute_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_port_attribute(Parameter && ... parameter) const
    {
        cppual::xv::set_port_attribute(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_port_attribute(Parameter && ... parameter) const
    -> reply::checked::get_port_attribute<Connection>
    {
        return cppual::xv::get_port_attribute(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_port_attribute_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_port_attribute<Connection>
    {
        return cppual::xv::get_port_attribute_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_port_attributes(Parameter && ... parameter) const
    -> reply::checked::query_port_attributes<Connection>
    {
        return cppual::xv::query_port_attributes(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_port_attributes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_port_attributes<Connection>
    {
        return cppual::xv::query_port_attributes_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_image_formats(Parameter && ... parameter) const
    -> reply::checked::list_image_formats<Connection>
    {
        return cppual::xv::list_image_formats(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_image_formats_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_image_formats<Connection>
    {
        return cppual::xv::list_image_formats_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_image_attributes(Parameter && ... parameter) const
    -> reply::checked::query_image_attributes<Connection>
    {
        return cppual::xv::query_image_attributes(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_image_attributes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_image_attributes<Connection>
    {
        return cppual::xv::query_image_attributes_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    put_image_checked(Parameter && ... parameter) const
    {
        cppual::xv::put_image_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    put_image(Parameter && ... parameter) const
    {
        cppual::xv::put_image(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    shm_put_image_checked(Parameter && ... parameter) const
    {
        cppual::xv::shm_put_image_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    shm_put_image(Parameter && ... parameter) const
    {
        cppual::xv::shm_put_image(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::xv::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::xv::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - m_first_event) {

        case XCB_XV_VIDEO_NOTIFY:
            handler(cppual::xv::event::video_notify<Connection>(m_c, m_first_event, event));
            return true;

        case XCB_XV_PORT_NOTIFY:
            handler(cppual::xv::event::port_notify<Connection>(m_c, m_first_event, event));
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
    typedef cppual::xv::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::xv::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - m_first_error) {

        case XCB_XV_BAD_PORT: // 0
            throw cppual::xv::error::bad_port(error);

        case XCB_XV_BAD_ENCODING: // 1
            throw cppual::xv::error::bad_encoding(error);

        case XCB_XV_BAD_CONTROL: // 2
            throw cppual::xv::error::bad_control(error);

        };
    }

protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::xv

#endif // CPPUAL_XV_HPP
