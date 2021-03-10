#ifndef CPPUAL_SELINUX_HPP
#define CPPUAL_SELINUX_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/xselinux.h>

namespace cppual { namespace selinux {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_selinux_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_selinux_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::selinux::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::selinux::event::dispatcher<Connection>;
    using error_dispatcher = cppual::selinux::error::dispatcher;
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
        SIGNATURE(xcb_selinux_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_query_version_reply),
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
SIGNATURE(xcb_selinux_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_query_version_unchecked)>;
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

template<typename Connection>
void
set_device_create_context_checked(Connection && c, uint32_t context_len, const char * context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_device_create_context_checked(std::forward<Connection>(c), context_len, context));
}

template<typename Connection>
void
set_device_create_context(Connection && c, uint32_t context_len, const char * context)
{
    ::xcb_selinux_set_device_create_context(std::forward<Connection>(c), context_len, context);
}

template<typename Connection>
void
set_device_create_context_checked(Connection && c, const std::string & context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_device_create_context_checked(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str()));
}

template<typename Connection>
void
set_device_create_context(Connection && c, const std::string & context)
{
    ::xcb_selinux_set_device_create_context(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str());
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_create_context
        : public cppual::generic::reply<get_device_create_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_device_create_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_create_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_device_create_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_create_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_device_create_context_context(this->get().get()),
                           ::xcb_selinux_get_device_create_context_context_length(this->get().get()));
    }

}; // class get_device_create_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_create_context = detail::get_device_create_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_device_create_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_create_context = detail::get_device_create_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_device_create_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_create_context<Connection>
get_device_create_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_create_context<Connection>
get_device_create_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_device_context_checked(Connection && c, uint32_t device, uint32_t context_len, const char * context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_device_context_checked(std::forward<Connection>(c), device, context_len, context));
}

template<typename Connection>
void
set_device_context(Connection && c, uint32_t device, uint32_t context_len, const char * context)
{
    ::xcb_selinux_set_device_context(std::forward<Connection>(c), device, context_len, context);
}

template<typename Connection>
void
set_device_context_checked(Connection && c, uint32_t device, const std::string & context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_device_context_checked(std::forward<Connection>(c), device, static_cast<uint32_t>(context.length()), context.c_str()));
}

template<typename Connection>
void
set_device_context(Connection && c, uint32_t device, const std::string & context)
{
    ::xcb_selinux_set_device_context(std::forward<Connection>(c), device, static_cast<uint32_t>(context.length()), context.c_str());
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_device_context
        : public cppual::generic::reply<get_device_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_device_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_device_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_device_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_device_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_device_context_context(this->get().get()),
                           ::xcb_selinux_get_device_context_context_length(this->get().get()));
    }

}; // class get_device_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_device_context = detail::get_device_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_device_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_device_context = detail::get_device_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_device_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_device_context<Connection>
get_device_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_device_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_device_context<Connection>
get_device_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_device_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_window_create_context_checked(Connection && c, uint32_t context_len, const char * context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_window_create_context_checked(std::forward<Connection>(c), context_len, context));
}

template<typename Connection>
void
set_window_create_context(Connection && c, uint32_t context_len, const char * context)
{
    ::xcb_selinux_set_window_create_context(std::forward<Connection>(c), context_len, context);
}

template<typename Connection>
void
set_window_create_context_checked(Connection && c, const std::string & context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_window_create_context_checked(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str()));
}

template<typename Connection>
void
set_window_create_context(Connection && c, const std::string & context)
{
    ::xcb_selinux_set_window_create_context(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str());
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_window_create_context
        : public cppual::generic::reply<get_window_create_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_window_create_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_window_create_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_window_create_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_window_create_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_window_create_context_context(this->get().get()),
                           ::xcb_selinux_get_window_create_context_context_length(this->get().get()));
    }

}; // class get_window_create_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_window_create_context = detail::get_window_create_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_window_create_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_window_create_context = detail::get_window_create_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_window_create_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_window_create_context<Connection>
get_window_create_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_window_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_window_create_context<Connection>
get_window_create_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_window_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_window_context
        : public cppual::generic::reply<get_window_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_window_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_window_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_window_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_window_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_window_context_context(this->get().get()),
                           ::xcb_selinux_get_window_context_context_length(this->get().get()));
    }

}; // class get_window_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_window_context = detail::get_window_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_window_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_window_context = detail::get_window_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_window_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_window_context<Connection>
get_window_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_window_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_window_context<Connection>
get_window_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_window_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_property_create_context_checked(Connection && c, uint32_t context_len, const char * context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_property_create_context_checked(std::forward<Connection>(c), context_len, context));
}

template<typename Connection>
void
set_property_create_context(Connection && c, uint32_t context_len, const char * context)
{
    ::xcb_selinux_set_property_create_context(std::forward<Connection>(c), context_len, context);
}

template<typename Connection>
void
set_property_create_context_checked(Connection && c, const std::string & context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_property_create_context_checked(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str()));
}

template<typename Connection>
void
set_property_create_context(Connection && c, const std::string & context)
{
    ::xcb_selinux_set_property_create_context(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str());
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_property_create_context
        : public cppual::generic::reply<get_property_create_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_property_create_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_property_create_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_property_create_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_property_create_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_property_create_context_context(this->get().get()),
                           ::xcb_selinux_get_property_create_context_context_length(this->get().get()));
    }

}; // class get_property_create_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_property_create_context = detail::get_property_create_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_property_create_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_property_create_context = detail::get_property_create_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_property_create_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_property_create_context<Connection>
get_property_create_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_property_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_property_create_context<Connection>
get_property_create_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_property_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_property_use_context_checked(Connection && c, uint32_t context_len, const char * context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_property_use_context_checked(std::forward<Connection>(c), context_len, context));
}

template<typename Connection>
void
set_property_use_context(Connection && c, uint32_t context_len, const char * context)
{
    ::xcb_selinux_set_property_use_context(std::forward<Connection>(c), context_len, context);
}

template<typename Connection>
void
set_property_use_context_checked(Connection && c, const std::string & context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_property_use_context_checked(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str()));
}

template<typename Connection>
void
set_property_use_context(Connection && c, const std::string & context)
{
    ::xcb_selinux_set_property_use_context(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str());
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_property_use_context
        : public cppual::generic::reply<get_property_use_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_property_use_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_property_use_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_property_use_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_property_use_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_property_use_context_context(this->get().get()),
                           ::xcb_selinux_get_property_use_context_context_length(this->get().get()));
    }

}; // class get_property_use_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_property_use_context = detail::get_property_use_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_property_use_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_property_use_context = detail::get_property_use_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_property_use_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_property_use_context<Connection>
get_property_use_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_property_use_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_property_use_context<Connection>
get_property_use_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_property_use_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_property_context
        : public cppual::generic::reply<get_property_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_property_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_property_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_property_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_property_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_property_context_context(this->get().get()),
                           ::xcb_selinux_get_property_context_context_length(this->get().get()));
    }

}; // class get_property_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_property_context = detail::get_property_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_property_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_property_context = detail::get_property_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_property_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_property_context<Connection>
get_property_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_property_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_property_context<Connection>
get_property_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_property_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_property_data_context
        : public cppual::generic::reply<get_property_data_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_property_data_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_property_data_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_property_data_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_property_data_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_property_data_context_context(this->get().get()),
                           ::xcb_selinux_get_property_data_context_context_length(this->get().get()));
    }

}; // class get_property_data_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_property_data_context = detail::get_property_data_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_property_data_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_property_data_context = detail::get_property_data_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_property_data_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_property_data_context<Connection>
get_property_data_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_property_data_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_property_data_context<Connection>
get_property_data_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_property_data_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_properties
        : public cppual::generic::reply<list_properties<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_list_properties_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_properties<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_list_properties_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_properties(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_selinux_list_properties_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_selinux_list_item_t,
            SIGNATURE(xcb_selinux_list_item_next),
            SIGNATURE(xcb_selinux_list_item_sizeof),
            SIGNATURE(xcb_selinux_list_properties_properties_iterator)>
            >
            properties(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_selinux_list_properties_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_selinux_list_item_t,
                SIGNATURE(xcb_selinux_list_item_next),
                SIGNATURE(xcb_selinux_list_item_sizeof),
                SIGNATURE(xcb_selinux_list_properties_properties_iterator)>
                >(this->m_c, this->get());
    }
}; // class list_properties

} // namespace detail

namespace checked {
template<typename Connection>
using list_properties = detail::list_properties<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_list_properties)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_properties = detail::list_properties<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_list_properties_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_properties<Connection>
list_properties(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_properties<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_properties<Connection>
list_properties_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_properties<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_selection_create_context_checked(Connection && c, uint32_t context_len, const char * context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_selection_create_context_checked(std::forward<Connection>(c), context_len, context));
}

template<typename Connection>
void
set_selection_create_context(Connection && c, uint32_t context_len, const char * context)
{
    ::xcb_selinux_set_selection_create_context(std::forward<Connection>(c), context_len, context);
}

template<typename Connection>
void
set_selection_create_context_checked(Connection && c, const std::string & context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_selection_create_context_checked(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str()));
}

template<typename Connection>
void
set_selection_create_context(Connection && c, const std::string & context)
{
    ::xcb_selinux_set_selection_create_context(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str());
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_selection_create_context
        : public cppual::generic::reply<get_selection_create_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_selection_create_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_selection_create_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_selection_create_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_selection_create_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_selection_create_context_context(this->get().get()),
                           ::xcb_selinux_get_selection_create_context_context_length(this->get().get()));
    }

}; // class get_selection_create_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_selection_create_context = detail::get_selection_create_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_selection_create_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_selection_create_context = detail::get_selection_create_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_selection_create_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_selection_create_context<Connection>
get_selection_create_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_selection_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_selection_create_context<Connection>
get_selection_create_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_selection_create_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_selection_use_context_checked(Connection && c, uint32_t context_len, const char * context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_selection_use_context_checked(std::forward<Connection>(c), context_len, context));
}

template<typename Connection>
void
set_selection_use_context(Connection && c, uint32_t context_len, const char * context)
{
    ::xcb_selinux_set_selection_use_context(std::forward<Connection>(c), context_len, context);
}

template<typename Connection>
void
set_selection_use_context_checked(Connection && c, const std::string & context)
{
    cppual::generic::check<Connection, cppual::selinux::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_selinux_set_selection_use_context_checked(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str()));
}

template<typename Connection>
void
set_selection_use_context(Connection && c, const std::string & context)
{
    ::xcb_selinux_set_selection_use_context(std::forward<Connection>(c), static_cast<uint32_t>(context.length()), context.c_str());
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_selection_use_context
        : public cppual::generic::reply<get_selection_use_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_selection_use_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_selection_use_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_selection_use_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_selection_use_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_selection_use_context_context(this->get().get()),
                           ::xcb_selinux_get_selection_use_context_context_length(this->get().get()));
    }

}; // class get_selection_use_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_selection_use_context = detail::get_selection_use_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_selection_use_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_selection_use_context = detail::get_selection_use_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_selection_use_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_selection_use_context<Connection>
get_selection_use_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_selection_use_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_selection_use_context<Connection>
get_selection_use_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_selection_use_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_selection_context
        : public cppual::generic::reply<get_selection_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_selection_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_selection_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_selection_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_selection_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_selection_context_context(this->get().get()),
                           ::xcb_selinux_get_selection_context_context_length(this->get().get()));
    }

}; // class get_selection_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_selection_context = detail::get_selection_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_selection_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_selection_context = detail::get_selection_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_selection_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_selection_context<Connection>
get_selection_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_selection_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_selection_context<Connection>
get_selection_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_selection_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_selection_data_context
        : public cppual::generic::reply<get_selection_data_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_selection_data_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_selection_data_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_selection_data_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_selection_data_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_selection_data_context_context(this->get().get()),
                           ::xcb_selinux_get_selection_data_context_context_length(this->get().get()));
    }

}; // class get_selection_data_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_selection_data_context = detail::get_selection_data_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_selection_data_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_selection_data_context = detail::get_selection_data_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_selection_data_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_selection_data_context<Connection>
get_selection_data_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_selection_data_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_selection_data_context<Connection>
get_selection_data_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_selection_data_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_selections
        : public cppual::generic::reply<list_selections<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_list_selections_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<list_selections<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_list_selections_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    list_selections(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_selinux_list_selections_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_selinux_list_item_t,
            SIGNATURE(xcb_selinux_list_item_next),
            SIGNATURE(xcb_selinux_list_item_sizeof),
            SIGNATURE(xcb_selinux_list_selections_selections_iterator)>
            >
            selections(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_selinux_list_selections_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_selinux_list_item_t,
                SIGNATURE(xcb_selinux_list_item_next),
                SIGNATURE(xcb_selinux_list_item_sizeof),
                SIGNATURE(xcb_selinux_list_selections_selections_iterator)>
                >(this->m_c, this->get());
    }
}; // class list_selections

} // namespace detail

namespace checked {
template<typename Connection>
using list_selections = detail::list_selections<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_list_selections)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_selections = detail::list_selections<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_list_selections_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_selections<Connection>
list_selections(Connection && c, Parameter && ... parameter)
{
    return reply::checked::list_selections<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_selections<Connection>
list_selections_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::list_selections<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_client_context
        : public cppual::generic::reply<get_client_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_selinux_get_client_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_client_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_selinux_get_client_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_client_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            context(void)
    {
        return std::string(xcb_selinux_get_client_context_context(this->get().get()),
                           ::xcb_selinux_get_client_context_context_length(this->get().get()));
    }

}; // class get_client_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_client_context = detail::get_client_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_selinux_get_client_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_client_context = detail::get_client_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_selinux_get_client_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_client_context<Connection>
get_client_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_client_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_client_context<Connection>
get_client_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_client_context<Connection>(
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
    typedef cppual::selinux::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    selinux(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::selinux::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::selinux::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_device_create_context_checked(Parameter && ... parameter) const
    {
        cppual::selinux::set_device_create_context_checked(connection(),
                                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_device_create_context(Parameter && ... parameter) const
    {
        cppual::selinux::set_device_create_context(connection(),
                                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_create_context(Parameter && ... parameter) const
    -> reply::checked::get_device_create_context<Connection>
    {
        return cppual::selinux::get_device_create_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_create_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_create_context<Connection>
    {
        return cppual::selinux::get_device_create_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_device_context_checked(Parameter && ... parameter) const
    {
        cppual::selinux::set_device_context_checked(connection(),
                                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_device_context(Parameter && ... parameter) const
    {
        cppual::selinux::set_device_context(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_device_context(Parameter && ... parameter) const
    -> reply::checked::get_device_context<Connection>
    {
        return cppual::selinux::get_device_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_device_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_device_context<Connection>
    {
        return cppual::selinux::get_device_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_window_create_context_checked(Parameter && ... parameter) const
    {
        cppual::selinux::set_window_create_context_checked(connection(),
                                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_window_create_context(Parameter && ... parameter) const
    {
        cppual::selinux::set_window_create_context(connection(),
                                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_window_create_context(Parameter && ... parameter) const
    -> reply::checked::get_window_create_context<Connection>
    {
        return cppual::selinux::get_window_create_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_window_create_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_window_create_context<Connection>
    {
        return cppual::selinux::get_window_create_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_window_context(Parameter && ... parameter) const
    -> reply::checked::get_window_context<Connection>
    {
        return cppual::selinux::get_window_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_window_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_window_context<Connection>
    {
        return cppual::selinux::get_window_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_property_create_context_checked(Parameter && ... parameter) const
    {
        cppual::selinux::set_property_create_context_checked(connection(),
                                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_property_create_context(Parameter && ... parameter) const
    {
        cppual::selinux::set_property_create_context(connection(),
                                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_property_create_context(Parameter && ... parameter) const
    -> reply::checked::get_property_create_context<Connection>
    {
        return cppual::selinux::get_property_create_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_property_create_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_property_create_context<Connection>
    {
        return cppual::selinux::get_property_create_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_property_use_context_checked(Parameter && ... parameter) const
    {
        cppual::selinux::set_property_use_context_checked(connection(),
                                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_property_use_context(Parameter && ... parameter) const
    {
        cppual::selinux::set_property_use_context(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_property_use_context(Parameter && ... parameter) const
    -> reply::checked::get_property_use_context<Connection>
    {
        return cppual::selinux::get_property_use_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_property_use_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_property_use_context<Connection>
    {
        return cppual::selinux::get_property_use_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_property_context(Parameter && ... parameter) const
    -> reply::checked::get_property_context<Connection>
    {
        return cppual::selinux::get_property_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_property_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_property_context<Connection>
    {
        return cppual::selinux::get_property_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_property_data_context(Parameter && ... parameter) const
    -> reply::checked::get_property_data_context<Connection>
    {
        return cppual::selinux::get_property_data_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_property_data_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_property_data_context<Connection>
    {
        return cppual::selinux::get_property_data_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_properties(Parameter && ... parameter) const
    -> reply::checked::list_properties<Connection>
    {
        return cppual::selinux::list_properties(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_properties_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_properties<Connection>
    {
        return cppual::selinux::list_properties_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_selection_create_context_checked(Parameter && ... parameter) const
    {
        cppual::selinux::set_selection_create_context_checked(connection(),
                                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_selection_create_context(Parameter && ... parameter) const
    {
        cppual::selinux::set_selection_create_context(connection(),
                                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_selection_create_context(Parameter && ... parameter) const
    -> reply::checked::get_selection_create_context<Connection>
    {
        return cppual::selinux::get_selection_create_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_selection_create_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_selection_create_context<Connection>
    {
        return cppual::selinux::get_selection_create_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_selection_use_context_checked(Parameter && ... parameter) const
    {
        cppual::selinux::set_selection_use_context_checked(connection(),
                                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_selection_use_context(Parameter && ... parameter) const
    {
        cppual::selinux::set_selection_use_context(connection(),
                                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_selection_use_context(Parameter && ... parameter) const
    -> reply::checked::get_selection_use_context<Connection>
    {
        return cppual::selinux::get_selection_use_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_selection_use_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_selection_use_context<Connection>
    {
        return cppual::selinux::get_selection_use_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_selection_context(Parameter && ... parameter) const
    -> reply::checked::get_selection_context<Connection>
    {
        return cppual::selinux::get_selection_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_selection_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_selection_context<Connection>
    {
        return cppual::selinux::get_selection_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_selection_data_context(Parameter && ... parameter) const
    -> reply::checked::get_selection_data_context<Connection>
    {
        return cppual::selinux::get_selection_data_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_selection_data_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_selection_data_context<Connection>
    {
        return cppual::selinux::get_selection_data_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_selections(Parameter && ... parameter) const
    -> reply::checked::list_selections<Connection>
    {
        return cppual::selinux::list_selections(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_selections_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_selections<Connection>
    {
        return cppual::selinux::list_selections_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_client_context(Parameter && ... parameter) const
    -> reply::checked::get_client_context<Connection>
    {
        return cppual::selinux::get_client_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_client_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_client_context<Connection>
    {
        return cppual::selinux::get_client_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::selinux::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::selinux::extension & extension)
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
    typedef cppual::selinux::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::selinux::extension & extension)
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


} } // namespace cppual::selinux

#endif // CPPUAL_SELINUX_HPP
