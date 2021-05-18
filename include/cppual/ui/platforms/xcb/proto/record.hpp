#ifndef CPPUAL_RECORD_HPP
#define CPPUAL_RECORD_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>
#include <cppual/string.h>

#include <vector>

#include <xcb/record.h>

namespace cppual { namespace record {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
  : public cppual::generic::extension<extension, &xcb_record_id>
{
  public:
    using base = cppual::generic::extension<extension, &xcb_record_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::record::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::record::event::dispatcher<Connection>;
    using error_dispatcher = cppual::record::error::dispatcher;
};


namespace error {
class bad_context
  : public cppual::generic::error<bad_context,
                               ::xcb_record_bad_context_error_t>
{
  public:
    using cppual::generic::error<bad_context, ::xcb_record_bad_context_error_t>::error;

    virtual ~bad_context(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RECORD_BAD_CONTEXT;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::record::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static string description(void)
    {
      return string("XCB_RECORD_BAD_CONTEXT");
    }

  protected:
    uint8_t m_first_error;
}; // class bad_context
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
                               SIGNATURE(xcb_record_query_version_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_record_query_version_reply),
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
    SIGNATURE(xcb_record_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_record_query_version_unchecked)>;
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
create_context_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::record::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_record_create_context_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_context(Parameter && ... parameter)
{
  ::xcb_record_create_context(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
register_clients_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::record::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_record_register_clients_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
register_clients(Parameter && ... parameter)
{
  ::xcb_record_register_clients(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
unregister_clients_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::record::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_record_unregister_clients_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
unregister_clients(Parameter && ... parameter)
{
  ::xcb_record_unregister_clients(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_context
  : public cppual::generic::reply<get_context<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_record_get_context_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_context<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_record_get_context_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_context(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_record_get_context_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_record_client_info_t,
                                              SIGNATURE(xcb_record_client_info_next),
                                              SIGNATURE(xcb_record_client_info_sizeof),
                                              SIGNATURE(xcb_record_get_context_intercepted_clients_iterator)>
                      >
    intercepted_clients(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_record_get_context_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_record_client_info_t,
                                                       SIGNATURE(xcb_record_client_info_next),
                                                       SIGNATURE(xcb_record_client_info_sizeof),
                                                       SIGNATURE(xcb_record_get_context_intercepted_clients_iterator)>
                               >(this->m_c, this->get());
    }
}; // class get_context

} // namespace detail

namespace checked {
template<typename Connection>
using get_context = detail::get_context<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_record_get_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_context = detail::get_context<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_record_get_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_context<Connection>
get_context(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_context<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_context<Connection>
get_context_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_context<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class enable_context
  : public cppual::generic::reply<enable_context<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_record_enable_context_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<enable_context<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_record_enable_context_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    enable_context(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_record_enable_context_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint8_t,
                                              SIGNATURE(xcb_record_enable_context_data),
                                              SIGNATURE(xcb_record_enable_context_data_length)>
                      >
    data(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_record_enable_context_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint8_t,
                                                       SIGNATURE(xcb_record_enable_context_data),
                                                       SIGNATURE(xcb_record_enable_context_data_length)>
                               >(this->m_c, this->get());
    }
}; // class enable_context

} // namespace detail

namespace checked {
template<typename Connection>
using enable_context = detail::enable_context<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_record_enable_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using enable_context = detail::enable_context<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_record_enable_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::enable_context<Connection>
enable_context(Connection && c, Parameter && ... parameter)
{
  return reply::checked::enable_context<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::enable_context<Connection>
enable_context_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::enable_context<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
disable_context_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::record::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_record_disable_context_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
disable_context(Parameter && ... parameter)
{
  ::xcb_record_disable_context(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
free_context_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::record::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_record_free_context_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
free_context(Parameter && ... parameter)
{
  ::xcb_record_free_context(std::forward<Parameter>(parameter) ...);
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

    const ::xcb_record_context_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~context(void) {}

    template<typename ... Parameter>
    void
    create_checked(Parameter && ... parameter) const
    {
      cppual::record::create_context_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
      cppual::record::create_context(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    register_clients_checked(Parameter && ... parameter) const
    {
      cppual::record::register_clients_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    register_clients(Parameter && ... parameter) const
    {
      cppual::record::register_clients(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    unregister_clients_checked(Parameter && ... parameter) const
    {
      cppual::record::unregister_clients_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    unregister_clients(Parameter && ... parameter) const
    {
      cppual::record::unregister_clients(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get(Parameter && ... parameter) const
    -> reply::checked::get_context<Connection>
    {
      return cppual::record::get_context(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_context<Connection>
    {
      return cppual::record::get_context_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    enable(Parameter && ... parameter) const
    -> reply::checked::enable_context<Connection>
    {
      return cppual::record::enable_context(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    enable_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::enable_context<Connection>
    {
      return cppual::record::enable_context_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    disable_checked(Parameter && ... parameter) const
    {
      cppual::record::disable_context_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    disable(Parameter && ... parameter) const
    {
      cppual::record::disable_context(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_checked(Parameter && ... parameter) const
    {
      cppual::record::free_context_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free(Parameter && ... parameter) const
    {
      cppual::record::free_context(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }



}; // class context



























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
    typedef cppual::record::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    record(void)
    {
      return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
      return cppual::record::query_version(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
      return cppual::record::query_version_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_context_checked(Parameter && ... parameter) const
    {
      cppual::record::create_context_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_context(Parameter && ... parameter) const
    {
      cppual::record::create_context(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    register_clients_checked(Parameter && ... parameter) const
    {
      cppual::record::register_clients_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    register_clients(Parameter && ... parameter) const
    {
      cppual::record::register_clients(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    unregister_clients_checked(Parameter && ... parameter) const
    {
      cppual::record::unregister_clients_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    unregister_clients(Parameter && ... parameter) const
    {
      cppual::record::unregister_clients(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_context(Parameter && ... parameter) const
    -> reply::checked::get_context<Connection>
    {
      return cppual::record::get_context(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_context<Connection>
    {
      return cppual::record::get_context_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    enable_context(Parameter && ... parameter) const
    -> reply::checked::enable_context<Connection>
    {
      return cppual::record::enable_context(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    enable_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::enable_context<Connection>
    {
      return cppual::record::enable_context_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    disable_context_checked(Parameter && ... parameter) const
    {
      cppual::record::disable_context_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    disable_context(Parameter && ... parameter) const
    {
      cppual::record::disable_context(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_context_checked(Parameter && ... parameter) const
    {
      cppual::record::free_context_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free_context(Parameter && ... parameter) const
    {
      cppual::record::free_context(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
  public:
    typedef cppual::record::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
      : m_c(std::forward<C>(c))
      , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::record::extension & extension)
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
    typedef cppual::record::extension extension;

    dispatcher(uint8_t first_error)
      : m_first_error(first_error)
    {}

    dispatcher(const cppual::record::extension & extension)
      : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
      switch (error->error_code - m_first_error) {

        case XCB_RECORD_BAD_CONTEXT: // 0
          throw cppual::record::error::bad_context(error);

      };
    }

  protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::record

#endif // CPPUAL_RECORD_HPP
