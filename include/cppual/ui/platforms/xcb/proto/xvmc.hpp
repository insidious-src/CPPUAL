#ifndef CPPUAL_XVMC_HPP
#define CPPUAL_XVMC_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/xvmc.h>

namespace cppual { namespace xvmc {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_xvmc_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_xvmc_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::xvmc::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::xvmc::event::dispatcher<Connection>;
    using error_dispatcher = cppual::xvmc::error::dispatcher;
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
        SIGNATURE(xcb_xvmc_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xvmc_query_version_reply),
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
SIGNATURE(xcb_xvmc_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xvmc_query_version_unchecked)>;
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
class list_surface_types
        : public cppual::generic::reply<list_surface_types<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xvmc_list_surface_types_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_surface_types<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xvmc_list_surface_types_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_surface_types(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xvmc_list_surface_types_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xvmc_surface_info_t,
            SIGNATURE(xcb_xvmc_list_surface_types_surfaces),
            SIGNATURE(xcb_xvmc_list_surface_types_surfaces_length)>
            >
            surfaces(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xvmc_list_surface_types_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xvmc_surface_info_t,
                SIGNATURE(xcb_xvmc_list_surface_types_surfaces),
                SIGNATURE(xcb_xvmc_list_surface_types_surfaces_length)>
                >(this->m_c, this->get());
    }
}; // class list_surface_types

} // namespace detail

namespace checked {
template<typename Connection>
using list_surface_types = detail::list_surface_types<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xvmc_list_surface_types)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_surface_types = detail::list_surface_types<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xvmc_list_surface_types_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_surface_types<Connection>
list_surface_types(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_surface_types<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_surface_types<Connection>
list_surface_types_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_surface_types<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class create_context
        : public cppual::generic::reply<create_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xvmc_create_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<create_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xvmc_create_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    create_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xvmc_create_context_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xvmc_create_context_priv_data),
            SIGNATURE(xcb_xvmc_create_context_priv_data_length)>
            >
            priv_data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xvmc_create_context_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xvmc_create_context_priv_data),
                SIGNATURE(xcb_xvmc_create_context_priv_data_length)>
                >(this->m_c, this->get());
    }
}; // class create_context

} // namespace detail

namespace checked {
template<typename Connection>
using create_context = detail::create_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xvmc_create_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using create_context = detail::create_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xvmc_create_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::create_context<Connection>
create_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::create_context<Connection>
create_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_context_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xvmc::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xvmc_destroy_context_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_context(Parameter && ... parameter)
{
    ::xcb_xvmc_destroy_context(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class create_surface
        : public cppual::generic::reply<create_surface<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xvmc_create_surface_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<create_surface<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xvmc_create_surface_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    create_surface(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xvmc_create_surface_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xvmc_create_surface_priv_data),
            SIGNATURE(xcb_xvmc_create_surface_priv_data_length)>
            >
            priv_data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xvmc_create_surface_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xvmc_create_surface_priv_data),
                SIGNATURE(xcb_xvmc_create_surface_priv_data_length)>
                >(this->m_c, this->get());
    }
}; // class create_surface

} // namespace detail

namespace checked {
template<typename Connection>
using create_surface = detail::create_surface<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xvmc_create_surface)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using create_surface = detail::create_surface<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xvmc_create_surface_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::create_surface<Connection>
create_surface(Connection && c, Parameter && ... parameter)
{
    return reply::checked::create_surface<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::create_surface<Connection>
create_surface_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::create_surface<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_surface_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xvmc::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xvmc_destroy_surface_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_surface(Parameter && ... parameter)
{
    ::xcb_xvmc_destroy_surface(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class create_subpicture
        : public cppual::generic::reply<create_subpicture<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xvmc_create_subpicture_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<create_subpicture<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xvmc_create_subpicture_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    create_subpicture(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xvmc_create_subpicture_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xvmc_create_subpicture_priv_data),
            SIGNATURE(xcb_xvmc_create_subpicture_priv_data_length)>
            >
            priv_data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xvmc_create_subpicture_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xvmc_create_subpicture_priv_data),
                SIGNATURE(xcb_xvmc_create_subpicture_priv_data_length)>
                >(this->m_c, this->get());
    }
}; // class create_subpicture

} // namespace detail

namespace checked {
template<typename Connection>
using create_subpicture = detail::create_subpicture<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xvmc_create_subpicture)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using create_subpicture = detail::create_subpicture<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xvmc_create_subpicture_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::create_subpicture<Connection>
create_subpicture(Connection && c, Parameter && ... parameter)
{
    return reply::checked::create_subpicture<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::create_subpicture<Connection>
create_subpicture_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::create_subpicture<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_subpicture_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xvmc::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xvmc_destroy_subpicture_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_subpicture(Parameter && ... parameter)
{
    ::xcb_xvmc_destroy_subpicture(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_subpicture_types
        : public cppual::generic::reply<list_subpicture_types<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xvmc_list_subpicture_types_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_subpicture_types<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xvmc_list_subpicture_types_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_subpicture_types(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xvmc_list_subpicture_types_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_xv_image_format_info_t,
            SIGNATURE(xcb_xvmc_list_subpicture_types_types),
            SIGNATURE(xcb_xvmc_list_subpicture_types_types_length)>
            >
            types(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xvmc_list_subpicture_types_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_xv_image_format_info_t,
                SIGNATURE(xcb_xvmc_list_subpicture_types_types),
                SIGNATURE(xcb_xvmc_list_subpicture_types_types_length)>
                >(this->m_c, this->get());
    }
}; // class list_subpicture_types

} // namespace detail

namespace checked {
template<typename Connection>
using list_subpicture_types = detail::list_subpicture_types<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xvmc_list_subpicture_types)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_subpicture_types = detail::list_subpicture_types<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xvmc_list_subpicture_types_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_subpicture_types<Connection>
list_subpicture_types(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_subpicture_types<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_subpicture_types<Connection>
list_subpicture_types_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_subpicture_types<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}








template<typename Derived, typename Connection>
class context
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_xvmc_context_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~context(void) {}

    template<typename ... Parameter>
    auto
    create(Parameter && ... parameter) const
    -> reply::checked::create_context<Connection>
    {
        return cppual::xvmc::create_context(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_context<Connection>
    {
        return cppual::xvmc::create_context_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_context_checked(connection(),
                                           resource(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_context(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }



}; // class context









template<typename Derived, typename Connection>
class subpicture
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_xvmc_subpicture_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~subpicture(void) {}

    template<typename ... Parameter>
    auto
    create(Parameter && ... parameter) const
    -> reply::checked::create_subpicture<Connection>
    {
        return cppual::xvmc::create_subpicture(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_subpicture<Connection>
    {
        return cppual::xvmc::create_subpicture_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_subpicture_checked(connection(),
                                              resource(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_subpicture(connection(),
                                      resource(),
                                      std::forward<Parameter>(parameter) ...);
    }



}; // class subpicture











template<typename Derived, typename Connection>
class surface
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_xvmc_surface_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~surface(void) {}

    template<typename ... Parameter>
    auto
    create(Parameter && ... parameter) const
    -> reply::checked::create_surface<Connection>
    {
        return cppual::xvmc::create_surface(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_surface<Connection>
    {
        return cppual::xvmc::create_surface_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_surface_checked(connection(),
                                           resource(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_surface(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }



}; // class surface







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
    typedef cppual::xvmc::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    xvmc(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::xvmc::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::xvmc::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_surface_types(Parameter && ... parameter) const
    -> reply::checked::list_surface_types<Connection>
    {
        return cppual::xvmc::list_surface_types(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_surface_types_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_surface_types<Connection>
    {
        return cppual::xvmc::list_surface_types_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    create_context(Parameter && ... parameter) const
    -> reply::checked::create_context<Connection>
    {
        return cppual::xvmc::create_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_context<Connection>
    {
        return cppual::xvmc::create_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_context_checked(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_context_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_context(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_context(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    create_surface(Parameter && ... parameter) const
    -> reply::checked::create_surface<Connection>
    {
        return cppual::xvmc::create_surface(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_surface_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_surface<Connection>
    {
        return cppual::xvmc::create_surface_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_surface_checked(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_surface_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_surface(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_surface(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    create_subpicture(Parameter && ... parameter) const
    -> reply::checked::create_subpicture<Connection>
    {
        return cppual::xvmc::create_subpicture(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_subpicture_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_subpicture<Connection>
    {
        return cppual::xvmc::create_subpicture_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_subpicture_checked(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_subpicture_checked(connection(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_subpicture(Parameter && ... parameter) const
    {
        cppual::xvmc::destroy_subpicture(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_subpicture_types(Parameter && ... parameter) const
    -> reply::checked::list_subpicture_types<Connection>
    {
        return cppual::xvmc::list_subpicture_types(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_subpicture_types_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_subpicture_types<Connection>
    {
        return cppual::xvmc::list_subpicture_types_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::xvmc::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::xvmc::extension & extension)
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
    typedef cppual::xvmc::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::xvmc::extension & extension)
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


} } // namespace xpp::xvmc

#endif // CPPUAL_XVMC_HPP
