#ifndef CPPUAL_SHM_HPP
#define CPPUAL_SHM_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>
#include <cppual/string.h>

#include <vector>

#include <xcb/shm.h>

namespace cppual { namespace shm {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_shm_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_shm_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::shm::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::shm::event::dispatcher<Connection>;
    using error_dispatcher = cppual::shm::error::dispatcher;
};

namespace event {
template<typename Connection>
class completion
        : public cppual::generic::event<xcb_shm_completion_event_t>
{
public:
    typedef cppual::shm::extension extension;

    typedef cppual::generic::event<xcb_shm_completion_event_t> base;

    template<typename C>
    completion(C && c,
               uint8_t first_event,
               const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~completion(void) {}

    static uint8_t opcode(void)
    {
        return XCB_SHM_COMPLETION;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::shm::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_SHM_COMPLETION");
    }

    uint8_t first_event(void)
    {
        return _M_first_event;
    }

    template<typename ReturnType = ::xcb_drawable_t, typename ... Parameter>
    ReturnType
    drawable(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->drawable),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->drawable,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_shm_seg_t, typename ... Parameter>
    ReturnType
    shmseg(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->shmseg),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->shmseg,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class completion



} // namespace event



namespace error {
class bad_seg
        : public cppual::generic::error<bad_seg,
        ::xcb_value_error_t>
{
public:
    using cppual::generic::error<bad_seg, ::xcb_value_error_t>::error;

    virtual ~bad_seg(void) {}

    static uint8_t opcode(void)
    {
        return XCB_SHM_BAD_SEG;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::shm::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_SHM_BAD_SEG");
    }

protected:
    uint8_t _M_first_error;
}; // class bad_seg
} // namespace error


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_version
        : public cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_shm_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_shm_query_version_reply),
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
SIGNATURE(xcb_shm_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_shm_query_version_unchecked)>;
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

template<typename Connection, typename ... Parameter>
void
attach_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shm::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shm_attach_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
attach(Parameter && ... parameter)
{
    ::xcb_shm_attach(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
detach_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shm::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shm_detach_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
detach(Parameter && ... parameter)
{
    ::xcb_shm_detach(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
put_image_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shm::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shm_put_image_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
put_image(Parameter && ... parameter)
{
    ::xcb_shm_put_image(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_image
        : public cppual::generic::reply<get_image<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_shm_get_image_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_image<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_shm_get_image_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_image(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_image

} // namespace detail

namespace checked {
template<typename Connection>
using get_image = detail::get_image<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_shm_get_image)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_image = detail::get_image<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_shm_get_image_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_image<Connection>
get_image(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_image<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_image<Connection>
get_image_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_image<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_pixmap_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shm::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shm_create_pixmap_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_pixmap(Parameter && ... parameter)
{
    ::xcb_shm_create_pixmap(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
attach_fd_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shm::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shm_attach_fd_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
attach_fd(Parameter && ... parameter)
{
    ::xcb_shm_attach_fd(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class create_segment
        : public cppual::generic::reply<create_segment<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_shm_create_segment_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<create_segment<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_shm_create_segment_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    create_segment(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class create_segment

} // namespace detail

namespace checked {
template<typename Connection>
using create_segment = detail::create_segment<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_shm_create_segment)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using create_segment = detail::create_segment<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_shm_create_segment_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::create_segment<Connection>
create_segment(Connection && c, Parameter && ... parameter)
{
    return reply::checked::create_segment<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::create_segment<Connection>
create_segment_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::create_segment<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}


















template<typename Derived, typename Connection>
class seg
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_shm_seg_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~seg(void) {}

    template<typename ... Parameter>
    void
    attach_checked(Parameter && ... parameter) const
    {
        cppual::shm::attach_checked(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    attach(Parameter && ... parameter) const
    {
        cppual::shm::attach(connection(),
                         resource(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    detach_checked(Parameter && ... parameter) const
    {
        cppual::shm::detach_checked(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    detach(Parameter && ... parameter) const
    {
        cppual::shm::detach(connection(),
                         resource(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    attach_fd_checked(Parameter && ... parameter) const
    {
        cppual::shm::attach_fd_checked(connection(),
                                    resource(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    attach_fd(Parameter && ... parameter) const
    {
        cppual::shm::attach_fd(connection(),
                            resource(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    createment(Parameter && ... parameter) const
    -> reply::checked::create_segment<Connection>
    {
        return cppual::shm::create_segment(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    createment_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_segment<Connection>
    {
        return cppual::shm::create_segment_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }



}; // class seg

















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
    typedef cppual::shm::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    shm(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::shm::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::shm::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    attach_checked(Parameter && ... parameter) const
    {
        cppual::shm::attach_checked(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    attach(Parameter && ... parameter) const
    {
        cppual::shm::attach(connection(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    detach_checked(Parameter && ... parameter) const
    {
        cppual::shm::detach_checked(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    detach(Parameter && ... parameter) const
    {
        cppual::shm::detach(connection(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    put_image_checked(Parameter && ... parameter) const
    {
        cppual::shm::put_image_checked(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    put_image(Parameter && ... parameter) const
    {
        cppual::shm::put_image(connection(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_image(Parameter && ... parameter) const
    -> reply::checked::get_image<Connection>
    {
        return cppual::shm::get_image(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_image_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_image<Connection>
    {
        return cppual::shm::get_image_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_pixmap_checked(Parameter && ... parameter) const
    {
        cppual::shm::create_pixmap_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_pixmap(Parameter && ... parameter) const
    {
        cppual::shm::create_pixmap(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    attach_fd_checked(Parameter && ... parameter) const
    {
        cppual::shm::attach_fd_checked(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    attach_fd(Parameter && ... parameter) const
    {
        cppual::shm::attach_fd(connection(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    create_segment(Parameter && ... parameter) const
    -> reply::checked::create_segment<Connection>
    {
        return cppual::shm::create_segment(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_segment_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_segment<Connection>
    {
        return cppual::shm::create_segment_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::shm::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::shm::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - _M_first_event) {

        case XCB_SHM_COMPLETION:
            handler(cppual::shm::event::completion<Connection>(_M_c, _M_first_event, event));
            return true;

        };

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
    typedef cppual::shm::extension extension;

    dispatcher(uint8_t first_error)
        : _M_first_error(first_error)
    {}

    dispatcher(const cppual::shm::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - _M_first_error) {

        case XCB_SHM_BAD_SEG: // 0
            throw cppual::shm::error::bad_seg(error);

        };
    }

protected:
    uint8_t _M_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::shm

#endif // CPPUAL_SHM_HPP
