#ifndef CPPUAL_RES_HPP
#define CPPUAL_RES_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/res.h>

namespace cppual { namespace res {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
  : public cppual::generic::extension<extension, &xcb_res_id>
{
  public:
    using base = cppual::generic::extension<extension, &xcb_res_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::res::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::res::event::dispatcher<Connection>;
    using error_dispatcher = cppual::res::error::dispatcher;
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
                               SIGNATURE(xcb_res_query_version_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_res_query_version_reply),
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
    SIGNATURE(xcb_res_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_res_query_version_unchecked)>;
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
class query_clients
  : public cppual::generic::reply<query_clients<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_res_query_clients_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_clients<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_res_query_clients_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_clients(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_res_query_clients_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_res_client_t,
                                              SIGNATURE(xcb_res_query_clients_clients),
                                              SIGNATURE(xcb_res_query_clients_clients_length)>
                      >
    clients(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_res_query_clients_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_res_client_t,
                                                       SIGNATURE(xcb_res_query_clients_clients),
                                                       SIGNATURE(xcb_res_query_clients_clients_length)>
                               >(this->m_c, this->get());
    }
}; // class query_clients

} // namespace detail

namespace checked {
template<typename Connection>
using query_clients = detail::query_clients<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_res_query_clients)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_clients = detail::query_clients<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_res_query_clients_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_clients<Connection>
query_clients(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_clients<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_clients<Connection>
query_clients_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_clients<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_client_resources
  : public cppual::generic::reply<query_client_resources<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_res_query_client_resources_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_client_resources<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_res_query_client_resources_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_client_resources(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_res_query_client_resources_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_res_type_t,
                                              SIGNATURE(xcb_res_query_client_resources_types),
                                              SIGNATURE(xcb_res_query_client_resources_types_length)>
                      >
    types(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_res_query_client_resources_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_res_type_t,
                                                       SIGNATURE(xcb_res_query_client_resources_types),
                                                       SIGNATURE(xcb_res_query_client_resources_types_length)>
                               >(this->m_c, this->get());
    }
}; // class query_client_resources

} // namespace detail

namespace checked {
template<typename Connection>
using query_client_resources = detail::query_client_resources<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_res_query_client_resources)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_client_resources = detail::query_client_resources<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_res_query_client_resources_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_client_resources<Connection>
query_client_resources(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_client_resources<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_client_resources<Connection>
query_client_resources_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_client_resources<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_client_pixmap_bytes
  : public cppual::generic::reply<query_client_pixmap_bytes<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_res_query_client_pixmap_bytes_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_client_pixmap_bytes<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_res_query_client_pixmap_bytes_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_client_pixmap_bytes(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_client_pixmap_bytes

} // namespace detail

namespace checked {
template<typename Connection>
using query_client_pixmap_bytes = detail::query_client_pixmap_bytes<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_res_query_client_pixmap_bytes)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_client_pixmap_bytes = detail::query_client_pixmap_bytes<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_res_query_client_pixmap_bytes_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_client_pixmap_bytes<Connection>
query_client_pixmap_bytes(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_client_pixmap_bytes<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_client_pixmap_bytes<Connection>
query_client_pixmap_bytes_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_client_pixmap_bytes<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_client_ids
  : public cppual::generic::reply<query_client_ids<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_res_query_client_ids_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_client_ids<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_res_query_client_ids_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_client_ids(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_res_query_client_ids_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_res_client_id_value_t,
                                              SIGNATURE(xcb_res_client_id_value_next),
                                              SIGNATURE(xcb_res_client_id_value_sizeof),
                                              SIGNATURE(xcb_res_query_client_ids_ids_iterator)>
                      >
    ids(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_res_query_client_ids_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_res_client_id_value_t,
                                                       SIGNATURE(xcb_res_client_id_value_next),
                                                       SIGNATURE(xcb_res_client_id_value_sizeof),
                                                       SIGNATURE(xcb_res_query_client_ids_ids_iterator)>
                               >(this->m_c, this->get());
    }
}; // class query_client_ids

} // namespace detail

namespace checked {
template<typename Connection>
using query_client_ids = detail::query_client_ids<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_res_query_client_ids)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_client_ids = detail::query_client_ids<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_res_query_client_ids_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_client_ids<Connection>
query_client_ids(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_client_ids<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_client_ids<Connection>
query_client_ids_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_client_ids<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_resource_bytes
  : public cppual::generic::reply<query_resource_bytes<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_res_query_resource_bytes_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_resource_bytes<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_res_query_resource_bytes_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_resource_bytes(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_res_query_resource_bytes_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_res_resource_size_value_t,
                                              SIGNATURE(xcb_res_resource_size_value_next),
                                              SIGNATURE(xcb_res_resource_size_value_sizeof),
                                              SIGNATURE(xcb_res_query_resource_bytes_sizes_iterator)>
                      >
    sizes(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_res_query_resource_bytes_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_res_resource_size_value_t,
                                                       SIGNATURE(xcb_res_resource_size_value_next),
                                                       SIGNATURE(xcb_res_resource_size_value_sizeof),
                                                       SIGNATURE(xcb_res_query_resource_bytes_sizes_iterator)>
                               >(this->m_c, this->get());
    }
}; // class query_resource_bytes

} // namespace detail

namespace checked {
template<typename Connection>
using query_resource_bytes = detail::query_resource_bytes<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_res_query_resource_bytes)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_resource_bytes = detail::query_resource_bytes<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_res_query_resource_bytes_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_resource_bytes<Connection>
query_resource_bytes(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_resource_bytes<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_resource_bytes<Connection>
query_resource_bytes_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_resource_bytes<Connection>(
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
    typedef cppual::res::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    res(void)
    {
      return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
      return cppual::res::query_version(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
      return cppual::res::query_version_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_clients(Parameter && ... parameter) const
    -> reply::checked::query_clients<Connection>
    {
      return cppual::res::query_clients(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_clients_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_clients<Connection>
    {
      return cppual::res::query_clients_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_client_resources(Parameter && ... parameter) const
    -> reply::checked::query_client_resources<Connection>
    {
      return cppual::res::query_client_resources(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_client_resources_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_client_resources<Connection>
    {
      return cppual::res::query_client_resources_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_client_pixmap_bytes(Parameter && ... parameter) const
    -> reply::checked::query_client_pixmap_bytes<Connection>
    {
      return cppual::res::query_client_pixmap_bytes(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_client_pixmap_bytes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_client_pixmap_bytes<Connection>
    {
      return cppual::res::query_client_pixmap_bytes_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_client_ids(Parameter && ... parameter) const
    -> reply::checked::query_client_ids<Connection>
    {
      return cppual::res::query_client_ids(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_client_ids_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_client_ids<Connection>
    {
      return cppual::res::query_client_ids_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_resource_bytes(Parameter && ... parameter) const
    -> reply::checked::query_resource_bytes<Connection>
    {
      return cppual::res::query_resource_bytes(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_resource_bytes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_resource_bytes<Connection>
    {
      return cppual::res::query_resource_bytes_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
  public:
    typedef cppual::res::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
      : m_c(std::forward<C>(c))
      , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::res::extension & extension)
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
    typedef cppual::res::extension extension;

    dispatcher(uint8_t first_error)
      : m_first_error(first_error)
    {}

    dispatcher(const cppual::res::extension & extension)
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


} } // namespace xpp::res

#endif // CPPUAL_RES_HPP
