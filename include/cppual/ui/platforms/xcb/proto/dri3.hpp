#ifndef CPPUAL_DRI3_HPP
#define CPPUAL_DRI3_HPP

#include <string>
#include <vector>

#include <xcb/dri3.h>

#include <cppual/ui/platforms/xcb/generic.hpp>

namespace cppual { namespace dri3 {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_dri3_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_dri3_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::dri3::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::dri3::event::dispatcher<Connection>;
    using error_dispatcher = cppual::dri3::error::dispatcher;
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
        SIGNATURE(xcb_dri3_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dri3_query_version_reply),
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
SIGNATURE(xcb_dri3_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dri3_query_version_unchecked)>;
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
class open
        : public cppual::generic::reply<open<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dri3_open_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<open<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dri3_open_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    open(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class open

} // namespace detail

namespace checked {
template<typename Connection>
using open = detail::open<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dri3_open)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using open = detail::open<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dri3_open_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::open<Connection>
open(Connection && c, Parameter && ... parameter)
{
    return reply::checked::open<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::open<Connection>
open_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::open<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
pixmap_from_buffer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::dri3::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_dri3_pixmap_from_buffer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
pixmap_from_buffer(Parameter && ... parameter)
{
    ::xcb_dri3_pixmap_from_buffer(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class buffer_from_pixmap
        : public cppual::generic::reply<buffer_from_pixmap<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dri3_buffer_from_pixmap_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<buffer_from_pixmap<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dri3_buffer_from_pixmap_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    buffer_from_pixmap(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class buffer_from_pixmap

} // namespace detail

namespace checked {
template<typename Connection>
using buffer_from_pixmap = detail::buffer_from_pixmap<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dri3_buffer_from_pixmap)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using buffer_from_pixmap = detail::buffer_from_pixmap<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dri3_buffer_from_pixmap_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::buffer_from_pixmap<Connection>
buffer_from_pixmap(Connection && c, Parameter && ... parameter)
{
    return reply::checked::buffer_from_pixmap<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::buffer_from_pixmap<Connection>
buffer_from_pixmap_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::buffer_from_pixmap<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
fence_from_fd_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::dri3::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_dri3_fence_from_fd_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
fence_from_fd(Parameter && ... parameter)
{
    ::xcb_dri3_fence_from_fd(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class fd_from_fence
        : public cppual::generic::reply<fd_from_fence<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dri3_fd_from_fence_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<fd_from_fence<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dri3_fd_from_fence_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    fd_from_fence(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class fd_from_fence

} // namespace detail

namespace checked {
template<typename Connection>
using fd_from_fence = detail::fd_from_fence<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dri3_fd_from_fence)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using fd_from_fence = detail::fd_from_fence<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dri3_fd_from_fence_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::fd_from_fence<Connection>
fd_from_fence(Connection && c, Parameter && ... parameter)
{
    return reply::checked::fd_from_fence<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::fd_from_fence<Connection>
fd_from_fence_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::fd_from_fence<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_supported_modifiers
        : public cppual::generic::reply<get_supported_modifiers<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dri3_get_supported_modifiers_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_supported_modifiers<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dri3_get_supported_modifiers_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_supported_modifiers(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_dri3_get_supported_modifiers_reply_t,
            cppual::generic::iterator<Connection,
            uint64_t,
            SIGNATURE(xcb_dri3_get_supported_modifiers_window_modifiers),
            SIGNATURE(xcb_dri3_get_supported_modifiers_window_modifiers_length)>
            >
            window_modifiers(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_dri3_get_supported_modifiers_reply_t,
                cppual::generic::iterator<Connection,
                uint64_t,
                SIGNATURE(xcb_dri3_get_supported_modifiers_window_modifiers),
                SIGNATURE(xcb_dri3_get_supported_modifiers_window_modifiers_length)>
                >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_dri3_get_supported_modifiers_reply_t,
            cppual::generic::iterator<Connection,
            uint64_t,
            SIGNATURE(xcb_dri3_get_supported_modifiers_screen_modifiers),
            SIGNATURE(xcb_dri3_get_supported_modifiers_screen_modifiers_length)>
            >
            screen_modifiers(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_dri3_get_supported_modifiers_reply_t,
                cppual::generic::iterator<Connection,
                uint64_t,
                SIGNATURE(xcb_dri3_get_supported_modifiers_screen_modifiers),
                SIGNATURE(xcb_dri3_get_supported_modifiers_screen_modifiers_length)>
                >(this->m_c, this->get());
    }
}; // class get_supported_modifiers

} // namespace detail

namespace checked {
template<typename Connection>
using get_supported_modifiers = detail::get_supported_modifiers<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dri3_get_supported_modifiers)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_supported_modifiers = detail::get_supported_modifiers<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dri3_get_supported_modifiers_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_supported_modifiers<Connection>
get_supported_modifiers(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_supported_modifiers<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_supported_modifiers<Connection>
get_supported_modifiers_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_supported_modifiers<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
pixmap_from_buffers_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::dri3::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_dri3_pixmap_from_buffers_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
pixmap_from_buffers(Parameter && ... parameter)
{
    ::xcb_dri3_pixmap_from_buffers(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class buffers_from_pixmap
        : public cppual::generic::reply<buffers_from_pixmap<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_dri3_buffers_from_pixmap_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<buffers_from_pixmap<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_dri3_buffers_from_pixmap_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    buffers_from_pixmap(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_dri3_buffers_from_pixmap_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_dri3_buffers_from_pixmap_strides),
            SIGNATURE(xcb_dri3_buffers_from_pixmap_strides_length)>
            >
            strides(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_dri3_buffers_from_pixmap_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_dri3_buffers_from_pixmap_strides),
                SIGNATURE(xcb_dri3_buffers_from_pixmap_strides_length)>
                >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_dri3_buffers_from_pixmap_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_dri3_buffers_from_pixmap_offsets),
            SIGNATURE(xcb_dri3_buffers_from_pixmap_offsets_length)>
            >
            offsets(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_dri3_buffers_from_pixmap_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_dri3_buffers_from_pixmap_offsets),
                SIGNATURE(xcb_dri3_buffers_from_pixmap_offsets_length)>
                >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
            ::xcb_dri3_buffers_from_pixmap_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_dri3_buffers_from_pixmap_buffers),
            SIGNATURE(xcb_dri3_buffers_from_pixmap_buffers_length)>
            >
            buffers(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_dri3_buffers_from_pixmap_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_dri3_buffers_from_pixmap_buffers),
                SIGNATURE(xcb_dri3_buffers_from_pixmap_buffers_length)>
                >(this->m_c, this->get());
    }
}; // class buffers_from_pixmap

} // namespace detail

namespace checked {
template<typename Connection>
using buffers_from_pixmap = detail::buffers_from_pixmap<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_dri3_buffers_from_pixmap)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using buffers_from_pixmap = detail::buffers_from_pixmap<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_dri3_buffers_from_pixmap_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::buffers_from_pixmap<Connection>
buffers_from_pixmap(Connection && c, Parameter && ... parameter)
{
    return reply::checked::buffers_from_pixmap<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::buffers_from_pixmap<Connection>
buffers_from_pixmap_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::buffers_from_pixmap<Connection>(
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
    typedef cppual::dri3::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    dri3(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::dri3::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::dri3::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    open(Parameter && ... parameter) const
    -> reply::checked::open<Connection>
    {
        return cppual::dri3::open(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    open_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::open<Connection>
    {
        return cppual::dri3::open_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    pixmap_from_buffer_checked(Parameter && ... parameter) const
    {
        cppual::dri3::pixmap_from_buffer_checked(connection(),
                                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    pixmap_from_buffer(Parameter && ... parameter) const
    {
        cppual::dri3::pixmap_from_buffer(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    buffer_from_pixmap(Parameter && ... parameter) const
    -> reply::checked::buffer_from_pixmap<Connection>
    {
        return cppual::dri3::buffer_from_pixmap(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    buffer_from_pixmap_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::buffer_from_pixmap<Connection>
    {
        return cppual::dri3::buffer_from_pixmap_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    fence_from_fd_checked(Parameter && ... parameter) const
    {
        cppual::dri3::fence_from_fd_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    fence_from_fd(Parameter && ... parameter) const
    {
        cppual::dri3::fence_from_fd(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    fd_from_fence(Parameter && ... parameter) const
    -> reply::checked::fd_from_fence<Connection>
    {
        return cppual::dri3::fd_from_fence(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    fd_from_fence_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::fd_from_fence<Connection>
    {
        return cppual::dri3::fd_from_fence_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_supported_modifiers(Parameter && ... parameter) const
    -> reply::checked::get_supported_modifiers<Connection>
    {
        return cppual::dri3::get_supported_modifiers(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_supported_modifiers_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_supported_modifiers<Connection>
    {
        return cppual::dri3::get_supported_modifiers_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    pixmap_from_buffers_checked(Parameter && ... parameter) const
    {
        cppual::dri3::pixmap_from_buffers_checked(connection(),
                                                  std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    pixmap_from_buffers(Parameter && ... parameter) const
    {
        cppual::dri3::pixmap_from_buffers(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    buffers_from_pixmap(Parameter && ... parameter) const
    -> reply::checked::buffers_from_pixmap<Connection>
    {
        return cppual::dri3::buffers_from_pixmap(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    buffers_from_pixmap_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::buffers_from_pixmap<Connection>
    {
        return cppual::dri3::buffers_from_pixmap_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::dri3::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::dri3::extension & extension)
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
    typedef cppual::dri3::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::dri3::extension & extension)
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


} } // namespace cppual::dri3

#endif // CPPUAL_DRI3_HPP
