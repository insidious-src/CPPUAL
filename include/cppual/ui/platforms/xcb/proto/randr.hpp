#ifndef CPPUAL_RANDR_HPP
#define CPPUAL_RANDR_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/randr.h>

namespace cppual { namespace randr {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
  : public cppual::generic::extension<extension, &xcb_randr_id>
{
  public:
    using base = cppual::generic::extension<extension, &xcb_randr_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::randr::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::randr::event::dispatcher<Connection>;
    using error_dispatcher = cppual::randr::error::dispatcher;
};

namespace event {
template<typename Connection>
class screen_change_notify
  : public cppual::generic::event<xcb_randr_screen_change_notify_event_t>
{
  public:
    typedef cppual::randr::extension extension;

    typedef cppual::generic::event<xcb_randr_screen_change_notify_event_t> base;

    template<typename C>
    screen_change_notify(C && c,
                         uint8_t first_event,
                         const std::shared_ptr<xcb_generic_event_t> & event)
      : base(event)
      , m_c(std::forward<C>(c))
      , m_first_event(first_event)
    {}

    virtual ~screen_change_notify(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RANDR_SCREEN_CHANGE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
      return first_event + opcode();
    }

    static uint8_t opcode(const cppual::randr::extension & extension)
    {
      return opcode(extension->first_event);
    }

    static std::string description(void)
    {
      return std::string("XCB_RANDR_SCREEN_CHANGE_NOTIFY");
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
    request_window(Parameter && ... parameter) const
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype((*this)->request_window),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    (*this)->request_window,
                    std::forward<Parameter>(parameter) ...);
    }

  protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class screen_change_notify



} // namespace event


namespace event {
template<typename Connection>
class notify
  : public cppual::generic::event<xcb_randr_notify_event_t>
{
  public:
    typedef cppual::randr::extension extension;

    typedef cppual::generic::event<xcb_randr_notify_event_t> base;

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
      return XCB_RANDR_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
      return first_event + opcode();
    }

    static uint8_t opcode(const cppual::randr::extension & extension)
    {
      return opcode(extension->first_event);
    }

    static std::string description(void)
    {
      return std::string("XCB_RANDR_NOTIFY");
    }

    uint8_t first_event(void)
    {
      return m_first_event;
    }
  protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class notify
} // namespace event



namespace error {
class bad_output
  : public cppual::generic::error<bad_output,
                               ::xcb_randr_bad_output_error_t>
{
  public:
    using cppual::generic::error<bad_output, ::xcb_randr_bad_output_error_t>::error;

    virtual ~bad_output(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RANDR_BAD_OUTPUT;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::randr::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RANDR_BAD_OUTPUT");
    }

  protected:
    uint8_t m_first_error;
}; // class bad_output
} // namespace error


namespace error {
class bad_crtc
  : public cppual::generic::error<bad_crtc,
                               ::xcb_randr_bad_crtc_error_t>
{
  public:
    using cppual::generic::error<bad_crtc, ::xcb_randr_bad_crtc_error_t>::error;

    virtual ~bad_crtc(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RANDR_BAD_CRTC;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::randr::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RANDR_BAD_CRTC");
    }

  protected:
    uint8_t m_first_error;
}; // class bad_crtc
} // namespace error


namespace error {
class bad_mode
  : public cppual::generic::error<bad_mode,
                               ::xcb_randr_bad_mode_error_t>
{
  public:
    using cppual::generic::error<bad_mode, ::xcb_randr_bad_mode_error_t>::error;

    virtual ~bad_mode(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RANDR_BAD_MODE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::randr::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RANDR_BAD_MODE");
    }

  protected:
    uint8_t m_first_error;
}; // class bad_mode
} // namespace error


namespace error {
class bad_provider
  : public cppual::generic::error<bad_provider,
                               ::xcb_randr_bad_provider_error_t>
{
  public:
    using cppual::generic::error<bad_provider, ::xcb_randr_bad_provider_error_t>::error;

    virtual ~bad_provider(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RANDR_BAD_PROVIDER;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::randr::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RANDR_BAD_PROVIDER");
    }

  protected:
    uint8_t m_first_error;
}; // class bad_provider
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
                               SIGNATURE(xcb_randr_query_version_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_query_version_reply),
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
    SIGNATURE(xcb_randr_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_query_version_unchecked)>;
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
class set_screen_config
  : public cppual::generic::reply<set_screen_config<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_set_screen_config_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<set_screen_config<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_set_screen_config_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    set_screen_config(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_randr_set_screen_config_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_window_t window, ::xcb_timestamp_t timestamp, ::xcb_timestamp_t config_timestamp, uint16_t sizeID, uint16_t rotation, uint16_t rate)
    {
      return base::cookie(c, window, timestamp, config_timestamp, sizeID, rotation, rate);
    }

    static
    ::xcb_randr_set_screen_config_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_window_t window, uint16_t sizeID, uint16_t rotation, uint16_t rate, ::xcb_timestamp_t timestamp = XCB_TIME_CURRENT_TIME, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
    {
      return base::cookie(c, window, timestamp, config_timestamp, sizeID, rotation, rate);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->root),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->root,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class set_screen_config

} // namespace detail

namespace checked {
template<typename Connection>
using set_screen_config = detail::set_screen_config<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_set_screen_config)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_screen_config = detail::set_screen_config<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_set_screen_config_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_screen_config<Connection>
set_screen_config(Connection && c, Parameter && ... parameter)
{
  return reply::checked::set_screen_config<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_screen_config<Connection>
set_screen_config_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::set_screen_config<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_input_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_select_input_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_input(Parameter && ... parameter)
{
  ::xcb_randr_select_input(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_screen_info
  : public cppual::generic::reply<get_screen_info<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_screen_info_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_screen_info<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_screen_info_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_screen_info(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_screen_size_t,
                                              SIGNATURE(xcb_randr_get_screen_info_sizes),
                                              SIGNATURE(xcb_randr_get_screen_info_sizes_length)>
                      >
    sizes(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_screen_size_t,
                                                       SIGNATURE(xcb_randr_get_screen_info_sizes),
                                                       SIGNATURE(xcb_randr_get_screen_info_sizes_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_refresh_rates_t,
                                              SIGNATURE(xcb_randr_refresh_rates_next),
                                              SIGNATURE(xcb_randr_refresh_rates_sizeof),
                                              SIGNATURE(xcb_randr_get_screen_info_rates_iterator)>
                      >
    rates(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_refresh_rates_t,
                                                       SIGNATURE(xcb_randr_refresh_rates_next),
                                                       SIGNATURE(xcb_randr_refresh_rates_sizeof),
                                                       SIGNATURE(xcb_randr_get_screen_info_rates_iterator)>
                               >(this->m_c, this->get());
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    root(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->root),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->root,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class get_screen_info

} // namespace detail

namespace checked {
template<typename Connection>
using get_screen_info = detail::get_screen_info<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_screen_info)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_screen_info = detail::get_screen_info<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_screen_info_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_screen_info<Connection>
get_screen_info(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_screen_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_screen_info<Connection>
get_screen_info_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_screen_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_screen_size_range
  : public cppual::generic::reply<get_screen_size_range<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_screen_size_range_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_screen_size_range<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_screen_size_range_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_screen_size_range(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_screen_size_range

} // namespace detail

namespace checked {
template<typename Connection>
using get_screen_size_range = detail::get_screen_size_range<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_screen_size_range)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_screen_size_range = detail::get_screen_size_range<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_screen_size_range_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_screen_size_range<Connection>
get_screen_size_range(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_screen_size_range<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_screen_size_range<Connection>
get_screen_size_range_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_screen_size_range<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_screen_size_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_screen_size_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_screen_size(Parameter && ... parameter)
{
  ::xcb_randr_set_screen_size(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_screen_resources
  : public cppual::generic::reply<get_screen_resources<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_screen_resources_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_screen_resources<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_screen_resources_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_screen_resources(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_crtc_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_crtcs),
                                              SIGNATURE(xcb_randr_get_screen_resources_crtcs_length)>
                      >
    crtcs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_crtc_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_crtcs),
                                                       SIGNATURE(xcb_randr_get_screen_resources_crtcs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_output_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_outputs),
                                              SIGNATURE(xcb_randr_get_screen_resources_outputs_length)>
                      >
    outputs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_output_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_outputs),
                                                       SIGNATURE(xcb_randr_get_screen_resources_outputs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_mode_info_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_modes),
                                              SIGNATURE(xcb_randr_get_screen_resources_modes_length)>
                      >
    modes(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_mode_info_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_modes),
                                                       SIGNATURE(xcb_randr_get_screen_resources_modes_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint8_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_names),
                                              SIGNATURE(xcb_randr_get_screen_resources_names_length)>
                      >
    names(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint8_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_names),
                                                       SIGNATURE(xcb_randr_get_screen_resources_names_length)>
                               >(this->m_c, this->get());
    }
}; // class get_screen_resources

} // namespace detail

namespace checked {
template<typename Connection>
using get_screen_resources = detail::get_screen_resources<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_screen_resources)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_screen_resources = detail::get_screen_resources<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_screen_resources_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_screen_resources<Connection>
get_screen_resources(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_screen_resources<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_screen_resources<Connection>
get_screen_resources_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_screen_resources<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_output_info
  : public cppual::generic::reply<get_output_info<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_output_info_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_output_info<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_output_info_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_output_info(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_randr_get_output_info_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_output_t output, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
    {
      return base::cookie(c, output, config_timestamp);
    }

    cppual::generic::list<Connection,
                       ::xcb_randr_get_output_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_crtc_t,
                                              SIGNATURE(xcb_randr_get_output_info_crtcs),
                                              SIGNATURE(xcb_randr_get_output_info_crtcs_length)>
                      >
    crtcs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_output_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_crtc_t,
                                                       SIGNATURE(xcb_randr_get_output_info_crtcs),
                                                       SIGNATURE(xcb_randr_get_output_info_crtcs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_output_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_mode_t,
                                              SIGNATURE(xcb_randr_get_output_info_modes),
                                              SIGNATURE(xcb_randr_get_output_info_modes_length)>
                      >
    modes(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_output_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_mode_t,
                                                       SIGNATURE(xcb_randr_get_output_info_modes),
                                                       SIGNATURE(xcb_randr_get_output_info_modes_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_output_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_output_t,
                                              SIGNATURE(xcb_randr_get_output_info_clones),
                                              SIGNATURE(xcb_randr_get_output_info_clones_length)>
                      >
    clones(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_output_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_output_t,
                                                       SIGNATURE(xcb_randr_get_output_info_clones),
                                                       SIGNATURE(xcb_randr_get_output_info_clones_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_output_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint8_t,
                                              SIGNATURE(xcb_randr_get_output_info_name),
                                              SIGNATURE(xcb_randr_get_output_info_name_length)>
                      >
    name(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_output_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint8_t,
                                                       SIGNATURE(xcb_randr_get_output_info_name),
                                                       SIGNATURE(xcb_randr_get_output_info_name_length)>
                               >(this->m_c, this->get());
    }

    template<typename ReturnType = ::xcb_randr_crtc_t, typename ... Parameter>
    ReturnType
    crtc(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->crtc),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->crtc,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class get_output_info

} // namespace detail

namespace checked {
template<typename Connection>
using get_output_info = detail::get_output_info<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_output_info)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_output_info = detail::get_output_info<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_output_info_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_output_info<Connection>
get_output_info(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_output_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_output_info<Connection>
get_output_info_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_output_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_output_properties
  : public cppual::generic::reply<list_output_properties<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_list_output_properties_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<list_output_properties<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_list_output_properties_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    list_output_properties(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename Atoms = ::xcb_atom_t>
    cppual::generic::list<Connection,
                       ::xcb_randr_list_output_properties_reply_t,
                       cppual::generic::iterator<Connection,
                                              Atoms,
                                              SIGNATURE(xcb_randr_list_output_properties_atoms),
                                              SIGNATURE(xcb_randr_list_output_properties_atoms_length)>
                      >
    atoms(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_list_output_properties_reply_t,
                                cppual::generic::iterator<Connection,
                                                       Atoms,
                                                       SIGNATURE(xcb_randr_list_output_properties_atoms),
                                                       SIGNATURE(xcb_randr_list_output_properties_atoms_length)>
                               >(this->m_c, this->get());
    }
}; // class list_output_properties

} // namespace detail

namespace checked {
template<typename Connection>
using list_output_properties = detail::list_output_properties<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_list_output_properties)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_output_properties = detail::list_output_properties<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_list_output_properties_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_output_properties<Connection>
list_output_properties(Connection && c, Parameter && ... parameter)
{
  return reply::checked::list_output_properties<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_output_properties<Connection>
list_output_properties_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::list_output_properties<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_output_property
  : public cppual::generic::reply<query_output_property<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_query_output_property_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_output_property<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_query_output_property_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_output_property(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_query_output_property_reply_t,
                       cppual::generic::iterator<Connection,
                                              int32_t,
                                              SIGNATURE(xcb_randr_query_output_property_valid_values),
                                              SIGNATURE(xcb_randr_query_output_property_valid_values_length)>
                      >
    valid_values(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_query_output_property_reply_t,
                                cppual::generic::iterator<Connection,
                                                       int32_t,
                                                       SIGNATURE(xcb_randr_query_output_property_valid_values),
                                                       SIGNATURE(xcb_randr_query_output_property_valid_values_length)>
                               >(this->m_c, this->get());
    }
}; // class query_output_property

} // namespace detail

namespace checked {
template<typename Connection>
using query_output_property = detail::query_output_property<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_query_output_property)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_output_property = detail::query_output_property<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_query_output_property_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_output_property<Connection>
query_output_property(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_output_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_output_property<Connection>
query_output_property_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_output_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
configure_output_property_checked(Connection && c, ::xcb_randr_output_t output, ::xcb_atom_t property, uint8_t pending, uint8_t range, uint32_t values_len, const int32_t * values)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_configure_output_property_checked(std::forward<Connection>(c), output, property, pending, range, values_len, values));
}

template<typename Connection>
void
configure_output_property(Connection && c, ::xcb_randr_output_t output, ::xcb_atom_t property, uint8_t pending, uint8_t range, uint32_t values_len, const int32_t * values)
{
  ::xcb_randr_configure_output_property(std::forward<Connection>(c), output, property, pending, range, values_len, values);
}

template<typename Connection, typename Values_Iterator>
void
configure_output_property_checked(Connection && c, ::xcb_randr_output_t output, ::xcb_atom_t property, uint8_t pending, uint8_t range, Values_Iterator values_begin, Values_Iterator values_end)
{
      typedef typename value_type<Values_Iterator, ! std::is_pointer<Values_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> values =
        { value_iterator<Values_Iterator>(values_begin), value_iterator<Values_Iterator>(values_end) };

  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_configure_output_property_checked(std::forward<Connection>(c), output, property, pending, range, static_cast<uint32_t>(values.size()), const_cast<const vector_type *>(values.data())));
}

template<typename Connection, typename Values_Iterator>
void
configure_output_property(Connection && c, ::xcb_randr_output_t output, ::xcb_atom_t property, uint8_t pending, uint8_t range, Values_Iterator values_begin, Values_Iterator values_end)
{
      typedef typename value_type<Values_Iterator, ! std::is_pointer<Values_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> values =
        { value_iterator<Values_Iterator>(values_begin), value_iterator<Values_Iterator>(values_end) };

  ::xcb_randr_configure_output_property(std::forward<Connection>(c), output, property, pending, range, static_cast<uint32_t>(values.size()), const_cast<const vector_type *>(values.data()));
}

template<typename Connection, typename ... Parameter>
void
change_output_property_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_change_output_property_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_output_property(Parameter && ... parameter)
{
  ::xcb_randr_change_output_property(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_output_property_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_delete_output_property_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_output_property(Parameter && ... parameter)
{
  ::xcb_randr_delete_output_property(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_output_property
  : public cppual::generic::reply<get_output_property<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_output_property_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_output_property<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_output_property_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_output_property(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_get_output_property_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint8_t,
                                              SIGNATURE(xcb_randr_get_output_property_data),
                                              SIGNATURE(xcb_randr_get_output_property_data_length)>
                      >
    data(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_output_property_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint8_t,
                                                       SIGNATURE(xcb_randr_get_output_property_data),
                                                       SIGNATURE(xcb_randr_get_output_property_data_length)>
                               >(this->m_c, this->get());
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    type(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->type),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->type,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class get_output_property

} // namespace detail

namespace checked {
template<typename Connection>
using get_output_property = detail::get_output_property<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_output_property)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_output_property = detail::get_output_property<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_output_property_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_output_property<Connection>
get_output_property(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_output_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_output_property<Connection>
get_output_property_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_output_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class create_mode
  : public cppual::generic::reply<create_mode<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_create_mode_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<create_mode<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_create_mode_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    create_mode(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_randr_create_mode_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_window_t window, ::xcb_randr_mode_info_t mode_info, uint32_t name_len, const char * name)
    {
      return base::cookie(c, window, mode_info, name_len, name);
    }

    static
    ::xcb_randr_create_mode_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_window_t window, ::xcb_randr_mode_info_t mode_info, const std::string & name)
    {
      return base::cookie(c, window, mode_info, static_cast<uint32_t>(name.length()), name.c_str());
    }

    template<typename ReturnType = ::xcb_randr_mode_t, typename ... Parameter>
    ReturnType
    mode(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->mode),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->mode,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class create_mode

} // namespace detail

namespace checked {
template<typename Connection>
using create_mode = detail::create_mode<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_create_mode)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using create_mode = detail::create_mode<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_create_mode_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::create_mode<Connection>
create_mode(Connection && c, Parameter && ... parameter)
{
  return reply::checked::create_mode<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::create_mode<Connection>
create_mode_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::create_mode<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_mode_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_destroy_mode_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_mode(Parameter && ... parameter)
{
  ::xcb_randr_destroy_mode(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
add_output_mode_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_add_output_mode_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
add_output_mode(Parameter && ... parameter)
{
  ::xcb_randr_add_output_mode(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_output_mode_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_delete_output_mode_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_output_mode(Parameter && ... parameter)
{
  ::xcb_randr_delete_output_mode(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_crtc_info
  : public cppual::generic::reply<get_crtc_info<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_crtc_info_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_crtc_info<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_crtc_info_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_crtc_info(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_randr_get_crtc_info_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_crtc_t crtc, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
    {
      return base::cookie(c, crtc, config_timestamp);
    }

    cppual::generic::list<Connection,
                       ::xcb_randr_get_crtc_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_output_t,
                                              SIGNATURE(xcb_randr_get_crtc_info_outputs),
                                              SIGNATURE(xcb_randr_get_crtc_info_outputs_length)>
                      >
    outputs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_crtc_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_output_t,
                                                       SIGNATURE(xcb_randr_get_crtc_info_outputs),
                                                       SIGNATURE(xcb_randr_get_crtc_info_outputs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_crtc_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_output_t,
                                              SIGNATURE(xcb_randr_get_crtc_info_possible),
                                              SIGNATURE(xcb_randr_get_crtc_info_possible_length)>
                      >
    possible(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_crtc_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_output_t,
                                                       SIGNATURE(xcb_randr_get_crtc_info_possible),
                                                       SIGNATURE(xcb_randr_get_crtc_info_possible_length)>
                               >(this->m_c, this->get());
    }

    template<typename ReturnType = ::xcb_randr_mode_t, typename ... Parameter>
    ReturnType
    mode(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->mode),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->mode,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class get_crtc_info

} // namespace detail

namespace checked {
template<typename Connection>
using get_crtc_info = detail::get_crtc_info<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_crtc_info)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_crtc_info = detail::get_crtc_info<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_crtc_info_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_crtc_info<Connection>
get_crtc_info(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_crtc_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_crtc_info<Connection>
get_crtc_info_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_crtc_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class set_crtc_config
  : public cppual::generic::reply<set_crtc_config<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_set_crtc_config_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<set_crtc_config<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_set_crtc_config_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    set_crtc_config(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_randr_set_crtc_config_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_crtc_t crtc, ::xcb_timestamp_t timestamp, ::xcb_timestamp_t config_timestamp, int16_t x, int16_t y, ::xcb_randr_mode_t mode, uint16_t rotation, uint32_t outputs_len, const ::xcb_randr_output_t * outputs)
    {
      return base::cookie(c, crtc, timestamp, config_timestamp, x, y, mode, rotation, outputs_len, outputs);
    }

    static
    ::xcb_randr_set_crtc_config_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_crtc_t crtc, int16_t x, int16_t y, ::xcb_randr_mode_t mode, uint16_t rotation, uint32_t outputs_len, const ::xcb_randr_output_t * outputs, ::xcb_timestamp_t timestamp = XCB_TIME_CURRENT_TIME, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
    {
      return base::cookie(c, crtc, timestamp, config_timestamp, x, y, mode, rotation, outputs_len, outputs);
    }

    template<typename Outputs_Iterator>
    static
    ::xcb_randr_set_crtc_config_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_crtc_t crtc, int16_t x, int16_t y, ::xcb_randr_mode_t mode, uint16_t rotation, Outputs_Iterator outputs_begin, Outputs_Iterator outputs_end, ::xcb_timestamp_t timestamp = XCB_TIME_CURRENT_TIME, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
    {
      typedef typename value_type<Outputs_Iterator, ! std::is_pointer<Outputs_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> outputs =
        { value_iterator<Outputs_Iterator>(outputs_begin), value_iterator<Outputs_Iterator>(outputs_end) };

      return base::cookie(c, crtc, timestamp, config_timestamp, x, y, mode, rotation, static_cast<uint32_t>(outputs.size()), const_cast<const vector_type *>(outputs.data()));
    }
}; // class set_crtc_config

} // namespace detail

namespace checked {
template<typename Connection>
using set_crtc_config = detail::set_crtc_config<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_set_crtc_config)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_crtc_config = detail::set_crtc_config<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_set_crtc_config_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_crtc_config<Connection>
set_crtc_config(Connection && c, Parameter && ... parameter)
{
  return reply::checked::set_crtc_config<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_crtc_config<Connection>
set_crtc_config_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::set_crtc_config<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_crtc_gamma_size
  : public cppual::generic::reply<get_crtc_gamma_size<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_crtc_gamma_size_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_crtc_gamma_size<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_crtc_gamma_size_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_crtc_gamma_size(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_crtc_gamma_size

} // namespace detail

namespace checked {
template<typename Connection>
using get_crtc_gamma_size = detail::get_crtc_gamma_size<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_crtc_gamma_size)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_crtc_gamma_size = detail::get_crtc_gamma_size<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_crtc_gamma_size_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_crtc_gamma_size<Connection>
get_crtc_gamma_size(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_crtc_gamma_size<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_crtc_gamma_size<Connection>
get_crtc_gamma_size_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_crtc_gamma_size<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_crtc_gamma
  : public cppual::generic::reply<get_crtc_gamma<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_crtc_gamma_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_crtc_gamma<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_crtc_gamma_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_crtc_gamma(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_get_crtc_gamma_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint16_t,
                                              SIGNATURE(xcb_randr_get_crtc_gamma_red),
                                              SIGNATURE(xcb_randr_get_crtc_gamma_red_length)>
                      >
    red(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_crtc_gamma_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint16_t,
                                                       SIGNATURE(xcb_randr_get_crtc_gamma_red),
                                                       SIGNATURE(xcb_randr_get_crtc_gamma_red_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_crtc_gamma_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint16_t,
                                              SIGNATURE(xcb_randr_get_crtc_gamma_green),
                                              SIGNATURE(xcb_randr_get_crtc_gamma_green_length)>
                      >
    green(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_crtc_gamma_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint16_t,
                                                       SIGNATURE(xcb_randr_get_crtc_gamma_green),
                                                       SIGNATURE(xcb_randr_get_crtc_gamma_green_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_crtc_gamma_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint16_t,
                                              SIGNATURE(xcb_randr_get_crtc_gamma_blue),
                                              SIGNATURE(xcb_randr_get_crtc_gamma_blue_length)>
                      >
    blue(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_crtc_gamma_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint16_t,
                                                       SIGNATURE(xcb_randr_get_crtc_gamma_blue),
                                                       SIGNATURE(xcb_randr_get_crtc_gamma_blue_length)>
                               >(this->m_c, this->get());
    }
}; // class get_crtc_gamma

} // namespace detail

namespace checked {
template<typename Connection>
using get_crtc_gamma = detail::get_crtc_gamma<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_crtc_gamma)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_crtc_gamma = detail::get_crtc_gamma<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_crtc_gamma_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_crtc_gamma<Connection>
get_crtc_gamma(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_crtc_gamma<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_crtc_gamma<Connection>
get_crtc_gamma_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_crtc_gamma<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_crtc_gamma_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_crtc_gamma_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_crtc_gamma(Parameter && ... parameter)
{
  ::xcb_randr_set_crtc_gamma(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_screen_resources_current
  : public cppual::generic::reply<get_screen_resources_current<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_screen_resources_current_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_screen_resources_current<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_screen_resources_current_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_screen_resources_current(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_current_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_crtc_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_current_crtcs),
                                              SIGNATURE(xcb_randr_get_screen_resources_current_crtcs_length)>
                      >
    crtcs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_current_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_crtc_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_crtcs),
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_crtcs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_current_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_output_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_current_outputs),
                                              SIGNATURE(xcb_randr_get_screen_resources_current_outputs_length)>
                      >
    outputs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_current_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_output_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_outputs),
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_outputs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_current_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_mode_info_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_current_modes),
                                              SIGNATURE(xcb_randr_get_screen_resources_current_modes_length)>
                      >
    modes(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_current_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_mode_info_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_modes),
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_modes_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_screen_resources_current_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint8_t,
                                              SIGNATURE(xcb_randr_get_screen_resources_current_names),
                                              SIGNATURE(xcb_randr_get_screen_resources_current_names_length)>
                      >
    names(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_screen_resources_current_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint8_t,
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_names),
                                                       SIGNATURE(xcb_randr_get_screen_resources_current_names_length)>
                               >(this->m_c, this->get());
    }
}; // class get_screen_resources_current

} // namespace detail

namespace checked {
template<typename Connection>
using get_screen_resources_current = detail::get_screen_resources_current<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_screen_resources_current)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_screen_resources_current = detail::get_screen_resources_current<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_screen_resources_current_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_screen_resources_current<Connection>
get_screen_resources_current(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_screen_resources_current<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_screen_resources_current<Connection>
get_screen_resources_current_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_screen_resources_current<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_crtc_transform_checked(Connection && c, ::xcb_randr_crtc_t crtc, ::xcb_render_transform_t transform, uint16_t filter_len, const char * filter_name, uint32_t filter_params_len, const ::xcb_render_fixed_t * filter_params)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_crtc_transform_checked(std::forward<Connection>(c), crtc, transform, filter_len, filter_name, filter_params_len, filter_params));
}

template<typename Connection>
void
set_crtc_transform(Connection && c, ::xcb_randr_crtc_t crtc, ::xcb_render_transform_t transform, uint16_t filter_len, const char * filter_name, uint32_t filter_params_len, const ::xcb_render_fixed_t * filter_params)
{
  ::xcb_randr_set_crtc_transform(std::forward<Connection>(c), crtc, transform, filter_len, filter_name, filter_params_len, filter_params);
}

template<typename Connection, typename Filter_params_Iterator>
void
set_crtc_transform_checked(Connection && c, ::xcb_randr_crtc_t crtc, ::xcb_render_transform_t transform, uint16_t filter_len, const char * filter_name, Filter_params_Iterator filter_params_begin, Filter_params_Iterator filter_params_end)
{
      typedef typename value_type<Filter_params_Iterator, ! std::is_pointer<Filter_params_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> filter_params =
        { value_iterator<Filter_params_Iterator>(filter_params_begin), value_iterator<Filter_params_Iterator>(filter_params_end) };

  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_crtc_transform_checked(std::forward<Connection>(c), crtc, transform, filter_len, filter_name, static_cast<uint32_t>(filter_params.size()), const_cast<const vector_type *>(filter_params.data())));
}

template<typename Connection, typename Filter_params_Iterator>
void
set_crtc_transform(Connection && c, ::xcb_randr_crtc_t crtc, ::xcb_render_transform_t transform, uint16_t filter_len, const char * filter_name, Filter_params_Iterator filter_params_begin, Filter_params_Iterator filter_params_end)
{
      typedef typename value_type<Filter_params_Iterator, ! std::is_pointer<Filter_params_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> filter_params =
        { value_iterator<Filter_params_Iterator>(filter_params_begin), value_iterator<Filter_params_Iterator>(filter_params_end) };

  ::xcb_randr_set_crtc_transform(std::forward<Connection>(c), crtc, transform, filter_len, filter_name, static_cast<uint32_t>(filter_params.size()), const_cast<const vector_type *>(filter_params.data()));
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_crtc_transform
  : public cppual::generic::reply<get_crtc_transform<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_crtc_transform_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_crtc_transform<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_crtc_transform_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_crtc_transform(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
    pending_filter_name(void)
    {
      return std::string(xcb_randr_get_crtc_transform_pending_filter_name(this->get().get()),
                         ::xcb_randr_get_crtc_transform_pending_filter_name_length(this->get().get()));
    }



    cppual::generic::list<Connection,
                       ::xcb_randr_get_crtc_transform_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_render_fixed_t,
                                              SIGNATURE(xcb_randr_get_crtc_transform_pending_params),
                                              SIGNATURE(xcb_randr_get_crtc_transform_pending_params_length)>
                      >
    pending_params(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_crtc_transform_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_render_fixed_t,
                                                       SIGNATURE(xcb_randr_get_crtc_transform_pending_params),
                                                       SIGNATURE(xcb_randr_get_crtc_transform_pending_params_length)>
                               >(this->m_c, this->get());
    }


    std::string
    current_filter_name(void)
    {
      return std::string(xcb_randr_get_crtc_transform_current_filter_name(this->get().get()),
                         ::xcb_randr_get_crtc_transform_current_filter_name_length(this->get().get()));
    }



    cppual::generic::list<Connection,
                       ::xcb_randr_get_crtc_transform_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_render_fixed_t,
                                              SIGNATURE(xcb_randr_get_crtc_transform_current_params),
                                              SIGNATURE(xcb_randr_get_crtc_transform_current_params_length)>
                      >
    current_params(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_crtc_transform_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_render_fixed_t,
                                                       SIGNATURE(xcb_randr_get_crtc_transform_current_params),
                                                       SIGNATURE(xcb_randr_get_crtc_transform_current_params_length)>
                               >(this->m_c, this->get());
    }
}; // class get_crtc_transform

} // namespace detail

namespace checked {
template<typename Connection>
using get_crtc_transform = detail::get_crtc_transform<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_crtc_transform)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_crtc_transform = detail::get_crtc_transform<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_crtc_transform_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_crtc_transform<Connection>
get_crtc_transform(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_crtc_transform<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_crtc_transform<Connection>
get_crtc_transform_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_crtc_transform<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_panning
  : public cppual::generic::reply<get_panning<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_panning_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_panning<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_panning_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_panning(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_panning

} // namespace detail

namespace checked {
template<typename Connection>
using get_panning = detail::get_panning<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_panning)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_panning = detail::get_panning<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_panning_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_panning<Connection>
get_panning(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_panning<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_panning<Connection>
get_panning_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_panning<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class set_panning
  : public cppual::generic::reply<set_panning<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_set_panning_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<set_panning<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_set_panning_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    set_panning(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_randr_set_panning_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_crtc_t crtc, ::xcb_timestamp_t timestamp, uint16_t left, uint16_t top, uint16_t width, uint16_t height, uint16_t track_left, uint16_t track_top, uint16_t track_width, uint16_t track_height, int16_t border_left, int16_t border_top, int16_t border_right, int16_t border_bottom)
    {
      return base::cookie(c, crtc, timestamp, left, top, width, height, track_left, track_top, track_width, track_height, border_left, border_top, border_right, border_bottom);
    }

    static
    ::xcb_randr_set_panning_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_crtc_t crtc, uint16_t left, uint16_t top, uint16_t width, uint16_t height, uint16_t track_left, uint16_t track_top, uint16_t track_width, uint16_t track_height, int16_t border_left, int16_t border_top, int16_t border_right, int16_t border_bottom, ::xcb_timestamp_t timestamp = XCB_TIME_CURRENT_TIME)
    {
      return base::cookie(c, crtc, timestamp, left, top, width, height, track_left, track_top, track_width, track_height, border_left, border_top, border_right, border_bottom);
    }
}; // class set_panning

} // namespace detail

namespace checked {
template<typename Connection>
using set_panning = detail::set_panning<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_set_panning)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using set_panning = detail::set_panning<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_set_panning_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::set_panning<Connection>
set_panning(Connection && c, Parameter && ... parameter)
{
  return reply::checked::set_panning<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::set_panning<Connection>
set_panning_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::set_panning<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_output_primary_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_output_primary_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_output_primary(Parameter && ... parameter)
{
  ::xcb_randr_set_output_primary(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_output_primary
  : public cppual::generic::reply<get_output_primary<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_output_primary_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_output_primary<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_output_primary_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_output_primary(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename ReturnType = ::xcb_randr_output_t, typename ... Parameter>
    ReturnType
    output(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->output),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->output,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class get_output_primary

} // namespace detail

namespace checked {
template<typename Connection>
using get_output_primary = detail::get_output_primary<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_output_primary)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_output_primary = detail::get_output_primary<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_output_primary_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_output_primary<Connection>
get_output_primary(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_output_primary<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_output_primary<Connection>
get_output_primary_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_output_primary<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_providers
  : public cppual::generic::reply<get_providers<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_providers_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_providers<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_providers_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_providers(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_get_providers_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_provider_t,
                                              SIGNATURE(xcb_randr_get_providers_providers),
                                              SIGNATURE(xcb_randr_get_providers_providers_length)>
                      >
    providers(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_providers_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_provider_t,
                                                       SIGNATURE(xcb_randr_get_providers_providers),
                                                       SIGNATURE(xcb_randr_get_providers_providers_length)>
                               >(this->m_c, this->get());
    }
}; // class get_providers

} // namespace detail

namespace checked {
template<typename Connection>
using get_providers = detail::get_providers<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_providers)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_providers = detail::get_providers<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_providers_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_providers<Connection>
get_providers(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_providers<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_providers<Connection>
get_providers_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_providers<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_provider_info
  : public cppual::generic::reply<get_provider_info<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_provider_info_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_provider_info<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_provider_info_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_provider_info(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_randr_get_provider_info_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_randr_provider_t provider, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
    {
      return base::cookie(c, provider, config_timestamp);
    }

    cppual::generic::list<Connection,
                       ::xcb_randr_get_provider_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_crtc_t,
                                              SIGNATURE(xcb_randr_get_provider_info_crtcs),
                                              SIGNATURE(xcb_randr_get_provider_info_crtcs_length)>
                      >
    crtcs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_provider_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_crtc_t,
                                                       SIGNATURE(xcb_randr_get_provider_info_crtcs),
                                                       SIGNATURE(xcb_randr_get_provider_info_crtcs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_provider_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_output_t,
                                              SIGNATURE(xcb_randr_get_provider_info_outputs),
                                              SIGNATURE(xcb_randr_get_provider_info_outputs_length)>
                      >
    outputs(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_provider_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_output_t,
                                                       SIGNATURE(xcb_randr_get_provider_info_outputs),
                                                       SIGNATURE(xcb_randr_get_provider_info_outputs_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_provider_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_provider_t,
                                              SIGNATURE(xcb_randr_get_provider_info_associated_providers),
                                              SIGNATURE(xcb_randr_get_provider_info_associated_providers_length)>
                      >
    associated_providers(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_provider_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_provider_t,
                                                       SIGNATURE(xcb_randr_get_provider_info_associated_providers),
                                                       SIGNATURE(xcb_randr_get_provider_info_associated_providers_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_randr_get_provider_info_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint32_t,
                                              SIGNATURE(xcb_randr_get_provider_info_associated_capability),
                                              SIGNATURE(xcb_randr_get_provider_info_associated_capability_length)>
                      >
    associated_capability(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_provider_info_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint32_t,
                                                       SIGNATURE(xcb_randr_get_provider_info_associated_capability),
                                                       SIGNATURE(xcb_randr_get_provider_info_associated_capability_length)>
                               >(this->m_c, this->get());
    }


    std::string
    name(void)
    {
      return std::string(xcb_randr_get_provider_info_name(this->get().get()),
                         ::xcb_randr_get_provider_info_name_length(this->get().get()));
    }

}; // class get_provider_info

} // namespace detail

namespace checked {
template<typename Connection>
using get_provider_info = detail::get_provider_info<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_provider_info)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_provider_info = detail::get_provider_info<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_provider_info_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_provider_info<Connection>
get_provider_info(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_provider_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_provider_info<Connection>
get_provider_info_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_provider_info<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_provider_offload_sink_checked(Connection && c, ::xcb_randr_provider_t provider, ::xcb_randr_provider_t sink_provider, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_provider_offload_sink_checked(std::forward<Connection>(c), provider, sink_provider, config_timestamp));
}

template<typename Connection>
void
set_provider_offload_sink(Connection && c, ::xcb_randr_provider_t provider, ::xcb_randr_provider_t sink_provider, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
{
  ::xcb_randr_set_provider_offload_sink(std::forward<Connection>(c), provider, sink_provider, config_timestamp);
}

template<typename Connection>
void
set_provider_output_source_checked(Connection && c, ::xcb_randr_provider_t provider, ::xcb_randr_provider_t source_provider, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_provider_output_source_checked(std::forward<Connection>(c), provider, source_provider, config_timestamp));
}

template<typename Connection>
void
set_provider_output_source(Connection && c, ::xcb_randr_provider_t provider, ::xcb_randr_provider_t source_provider, ::xcb_timestamp_t config_timestamp = XCB_TIME_CURRENT_TIME)
{
  ::xcb_randr_set_provider_output_source(std::forward<Connection>(c), provider, source_provider, config_timestamp);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class list_provider_properties
  : public cppual::generic::reply<list_provider_properties<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_list_provider_properties_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<list_provider_properties<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_list_provider_properties_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    list_provider_properties(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename Atoms = ::xcb_atom_t>
    cppual::generic::list<Connection,
                       ::xcb_randr_list_provider_properties_reply_t,
                       cppual::generic::iterator<Connection,
                                              Atoms,
                                              SIGNATURE(xcb_randr_list_provider_properties_atoms),
                                              SIGNATURE(xcb_randr_list_provider_properties_atoms_length)>
                      >
    atoms(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_list_provider_properties_reply_t,
                                cppual::generic::iterator<Connection,
                                                       Atoms,
                                                       SIGNATURE(xcb_randr_list_provider_properties_atoms),
                                                       SIGNATURE(xcb_randr_list_provider_properties_atoms_length)>
                               >(this->m_c, this->get());
    }
}; // class list_provider_properties

} // namespace detail

namespace checked {
template<typename Connection>
using list_provider_properties = detail::list_provider_properties<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_list_provider_properties)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using list_provider_properties = detail::list_provider_properties<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_list_provider_properties_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::list_provider_properties<Connection>
list_provider_properties(Connection && c, Parameter && ... parameter)
{
  return reply::checked::list_provider_properties<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::list_provider_properties<Connection>
list_provider_properties_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::list_provider_properties<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_provider_property
  : public cppual::generic::reply<query_provider_property<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_query_provider_property_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_provider_property<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_query_provider_property_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_provider_property(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_query_provider_property_reply_t,
                       cppual::generic::iterator<Connection,
                                              int32_t,
                                              SIGNATURE(xcb_randr_query_provider_property_valid_values),
                                              SIGNATURE(xcb_randr_query_provider_property_valid_values_length)>
                      >
    valid_values(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_query_provider_property_reply_t,
                                cppual::generic::iterator<Connection,
                                                       int32_t,
                                                       SIGNATURE(xcb_randr_query_provider_property_valid_values),
                                                       SIGNATURE(xcb_randr_query_provider_property_valid_values_length)>
                               >(this->m_c, this->get());
    }
}; // class query_provider_property

} // namespace detail

namespace checked {
template<typename Connection>
using query_provider_property = detail::query_provider_property<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_query_provider_property)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_provider_property = detail::query_provider_property<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_query_provider_property_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_provider_property<Connection>
query_provider_property(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_provider_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_provider_property<Connection>
query_provider_property_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_provider_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
configure_provider_property_checked(Connection && c, ::xcb_randr_provider_t provider, ::xcb_atom_t property, uint8_t pending, uint8_t range, uint32_t values_len, const int32_t * values)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_configure_provider_property_checked(std::forward<Connection>(c), provider, property, pending, range, values_len, values));
}

template<typename Connection>
void
configure_provider_property(Connection && c, ::xcb_randr_provider_t provider, ::xcb_atom_t property, uint8_t pending, uint8_t range, uint32_t values_len, const int32_t * values)
{
  ::xcb_randr_configure_provider_property(std::forward<Connection>(c), provider, property, pending, range, values_len, values);
}

template<typename Connection, typename Values_Iterator>
void
configure_provider_property_checked(Connection && c, ::xcb_randr_provider_t provider, ::xcb_atom_t property, uint8_t pending, uint8_t range, Values_Iterator values_begin, Values_Iterator values_end)
{
      typedef typename value_type<Values_Iterator, ! std::is_pointer<Values_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> values =
        { value_iterator<Values_Iterator>(values_begin), value_iterator<Values_Iterator>(values_end) };

  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_configure_provider_property_checked(std::forward<Connection>(c), provider, property, pending, range, static_cast<uint32_t>(values.size()), const_cast<const vector_type *>(values.data())));
}

template<typename Connection, typename Values_Iterator>
void
configure_provider_property(Connection && c, ::xcb_randr_provider_t provider, ::xcb_atom_t property, uint8_t pending, uint8_t range, Values_Iterator values_begin, Values_Iterator values_end)
{
      typedef typename value_type<Values_Iterator, ! std::is_pointer<Values_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> values =
        { value_iterator<Values_Iterator>(values_begin), value_iterator<Values_Iterator>(values_end) };

  ::xcb_randr_configure_provider_property(std::forward<Connection>(c), provider, property, pending, range, static_cast<uint32_t>(values.size()), const_cast<const vector_type *>(values.data()));
}

template<typename Connection, typename ... Parameter>
void
change_provider_property_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_change_provider_property_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_provider_property(Parameter && ... parameter)
{
  ::xcb_randr_change_provider_property(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_provider_property_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_delete_provider_property_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_provider_property(Parameter && ... parameter)
{
  ::xcb_randr_delete_provider_property(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_provider_property
  : public cppual::generic::reply<get_provider_property<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_provider_property_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_provider_property<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_provider_property_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_provider_property(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    template<typename Type>
    cppual::generic::list<Connection,
                       ::xcb_randr_get_provider_property_reply_t,
                       cppual::generic::iterator<Connection,
                                              Type,
                                              SIGNATURE(xcb_randr_get_provider_property_data),
                                              SIGNATURE(xcb_randr_get_provider_property_data_length)>
                      >
    data(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_provider_property_reply_t,
                                cppual::generic::iterator<Connection,
                                                       Type,
                                                       SIGNATURE(xcb_randr_get_provider_property_data),
                                                       SIGNATURE(xcb_randr_get_provider_property_data_length)>
                               >(this->m_c, this->get());
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    type(Parameter && ... parameter)
    {
      using make = cppual::generic::factory::make<Connection,
                                               decltype(this->get()->type),
                                               ReturnType,
                                               Parameter ...>;
      return make()(this->m_c,
                    this->get()->type,
                    std::forward<Parameter>(parameter) ...);
    }
}; // class get_provider_property

} // namespace detail

namespace checked {
template<typename Connection>
using get_provider_property = detail::get_provider_property<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_provider_property)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_provider_property = detail::get_provider_property<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_provider_property_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_provider_property<Connection>
get_provider_property(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_provider_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_provider_property<Connection>
get_provider_property_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_provider_property<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_monitors
  : public cppual::generic::reply<get_monitors<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_get_monitors_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_monitors<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_get_monitors_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_monitors(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_randr_get_monitors_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_randr_monitor_info_t,
                                              SIGNATURE(xcb_randr_monitor_info_next),
                                              SIGNATURE(xcb_randr_monitor_info_sizeof),
                                              SIGNATURE(xcb_randr_get_monitors_monitors_iterator)>
                      >
    monitors(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_randr_get_monitors_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_randr_monitor_info_t,
                                                       SIGNATURE(xcb_randr_monitor_info_next),
                                                       SIGNATURE(xcb_randr_monitor_info_sizeof),
                                                       SIGNATURE(xcb_randr_get_monitors_monitors_iterator)>
                               >(this->m_c, this->get());
    }
}; // class get_monitors

} // namespace detail

namespace checked {
template<typename Connection>
using get_monitors = detail::get_monitors<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_get_monitors)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_monitors = detail::get_monitors<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_get_monitors_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_monitors<Connection>
get_monitors(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_monitors<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_monitors<Connection>
get_monitors_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_monitors<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_monitor_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_set_monitor_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_monitor(Parameter && ... parameter)
{
  ::xcb_randr_set_monitor(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_monitor_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_delete_monitor_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_monitor(Parameter && ... parameter)
{
  ::xcb_randr_delete_monitor(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class create_lease
  : public cppual::generic::reply<create_lease<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_randr_create_lease_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<create_lease<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_randr_create_lease_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    create_lease(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class create_lease

} // namespace detail

namespace checked {
template<typename Connection>
using create_lease = detail::create_lease<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_randr_create_lease)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using create_lease = detail::create_lease<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_randr_create_lease_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::create_lease<Connection>
create_lease(Connection && c, Parameter && ... parameter)
{
  return reply::checked::create_lease<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::create_lease<Connection>
create_lease_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::create_lease<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
free_lease_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::randr::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_randr_free_lease_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
free_lease(Parameter && ... parameter)
{
  ::xcb_randr_free_lease(std::forward<Parameter>(parameter) ...);
}





template<typename Derived, typename Connection>
class crtc
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_randr_crtc_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~crtc(void) {}

    template<typename ... Parameter>
    auto
    get_info(Parameter && ... parameter) const
    -> reply::checked::get_crtc_info<Connection>
    {
      return cppual::randr::get_crtc_info(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_info<Connection>
    {
      return cppual::randr::get_crtc_info_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_config(Parameter && ... parameter) const
    -> reply::checked::set_crtc_config<Connection>
    {
      return cppual::randr::set_crtc_config(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_config_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_crtc_config<Connection>
    {
      return cppual::randr::set_crtc_config_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_gamma_size(Parameter && ... parameter) const
    -> reply::checked::get_crtc_gamma_size<Connection>
    {
      return cppual::randr::get_crtc_gamma_size(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_gamma_size_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_gamma_size<Connection>
    {
      return cppual::randr::get_crtc_gamma_size_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_gamma(Parameter && ... parameter) const
    -> reply::checked::get_crtc_gamma<Connection>
    {
      return cppual::randr::get_crtc_gamma(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_gamma_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_gamma<Connection>
    {
      return cppual::randr::get_crtc_gamma_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_gamma_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_gamma_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_gamma(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_gamma(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_transform_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_transform_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_transform(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_transform(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_transform(Parameter && ... parameter) const
    -> reply::checked::get_crtc_transform<Connection>
    {
      return cppual::randr::get_crtc_transform(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_transform_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_transform<Connection>
    {
      return cppual::randr::get_crtc_transform_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_panning(Parameter && ... parameter) const
    -> reply::checked::get_panning<Connection>
    {
      return cppual::randr::get_panning(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_panning_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_panning<Connection>
    {
      return cppual::randr::get_panning_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_panning(Parameter && ... parameter) const
    -> reply::checked::set_panning<Connection>
    {
      return cppual::randr::set_panning(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_panning_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_panning<Connection>
    {
      return cppual::randr::set_panning_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }



}; // class crtc














template<typename Derived, typename Connection>
class mode
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_randr_mode_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~mode(void) {}

    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
      cppual::randr::destroy_mode_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
      cppual::randr::destroy_mode(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }



}; // class mode












template<typename Derived, typename Connection>
class provider
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_randr_provider_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~provider(void) {}

    template<typename ... Parameter>
    auto
    get_info(Parameter && ... parameter) const
    -> reply::checked::get_provider_info<Connection>
    {
      return cppual::randr::get_provider_info(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_provider_info<Connection>
    {
      return cppual::randr::get_provider_info_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_offload_sink_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_offload_sink_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_offload_sink(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_offload_sink(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_output_source_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_output_source_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_output_source(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_output_source(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_properties(Parameter && ... parameter) const
    -> reply::checked::list_provider_properties<Connection>
    {
      return cppual::randr::list_provider_properties(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_properties_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_provider_properties<Connection>
    {
      return cppual::randr::list_provider_properties_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_property(Parameter && ... parameter) const
    -> reply::checked::query_provider_property<Connection>
    {
      return cppual::randr::query_provider_property(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_provider_property<Connection>
    {
      return cppual::randr::query_provider_property_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    configure_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::configure_provider_property_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    configure_property(Parameter && ... parameter) const
    {
      cppual::randr::configure_provider_property(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::change_provider_property_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_property(Parameter && ... parameter) const
    {
      cppual::randr::change_provider_property(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::delete_provider_property_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_property(Parameter && ... parameter) const
    {
      cppual::randr::delete_provider_property(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_property(Parameter && ... parameter) const
    -> reply::checked::get_provider_property<Connection>
    {
      return cppual::randr::get_provider_property(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_provider_property<Connection>
    {
      return cppual::randr::get_provider_property_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }



}; // class provider

template<typename Derived, typename Connection>
class output
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_randr_output_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~output(void) {}

    template<typename ... Parameter>
    auto
    get_info(Parameter && ... parameter) const
    -> reply::checked::get_output_info<Connection>
    {
      return cppual::randr::get_output_info(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_output_info<Connection>
    {
      return cppual::randr::get_output_info_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_properties(Parameter && ... parameter) const
    -> reply::checked::list_output_properties<Connection>
    {
      return cppual::randr::list_output_properties(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_properties_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_output_properties<Connection>
    {
      return cppual::randr::list_output_properties_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_property(Parameter && ... parameter) const
    -> reply::checked::query_output_property<Connection>
    {
      return cppual::randr::query_output_property(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_output_property<Connection>
    {
      return cppual::randr::query_output_property_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    configure_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::configure_output_property_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    configure_property(Parameter && ... parameter) const
    {
      cppual::randr::configure_output_property(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::change_output_property_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_property(Parameter && ... parameter) const
    {
      cppual::randr::change_output_property(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_property_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_property(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_property(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_property(Parameter && ... parameter) const
    -> reply::checked::get_output_property<Connection>
    {
      return cppual::randr::get_output_property(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_output_property<Connection>
    {
      return cppual::randr::get_output_property_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    add_mode_checked(Parameter && ... parameter) const
    {
      cppual::randr::add_output_mode_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    add_mode(Parameter && ... parameter) const
    {
      cppual::randr::add_output_mode(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_mode_checked(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_mode_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_mode(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_mode(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }



}; // class output



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
    typedef cppual::randr::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    randr(void)
    {
      return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
      return cppual::randr::query_version(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
      return cppual::randr::query_version_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_screen_config(Parameter && ... parameter) const
    -> reply::checked::set_screen_config<Connection>
    {
      return cppual::randr::set_screen_config(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_screen_config_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_screen_config<Connection>
    {
      return cppual::randr::set_screen_config_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_input_checked(Parameter && ... parameter) const
    {
      cppual::randr::select_input_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_input(Parameter && ... parameter) const
    {
      cppual::randr::select_input(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_screen_info(Parameter && ... parameter) const
    -> reply::checked::get_screen_info<Connection>
    {
      return cppual::randr::get_screen_info(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_screen_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_screen_info<Connection>
    {
      return cppual::randr::get_screen_info_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_screen_size_range(Parameter && ... parameter) const
    -> reply::checked::get_screen_size_range<Connection>
    {
      return cppual::randr::get_screen_size_range(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_screen_size_range_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_screen_size_range<Connection>
    {
      return cppual::randr::get_screen_size_range_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_screen_size_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_screen_size_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_screen_size(Parameter && ... parameter) const
    {
      cppual::randr::set_screen_size(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_screen_resources(Parameter && ... parameter) const
    -> reply::checked::get_screen_resources<Connection>
    {
      return cppual::randr::get_screen_resources(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_screen_resources_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_screen_resources<Connection>
    {
      return cppual::randr::get_screen_resources_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_output_info(Parameter && ... parameter) const
    -> reply::checked::get_output_info<Connection>
    {
      return cppual::randr::get_output_info(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_output_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_output_info<Connection>
    {
      return cppual::randr::get_output_info_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_output_properties(Parameter && ... parameter) const
    -> reply::checked::list_output_properties<Connection>
    {
      return cppual::randr::list_output_properties(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_output_properties_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_output_properties<Connection>
    {
      return cppual::randr::list_output_properties_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_output_property(Parameter && ... parameter) const
    -> reply::checked::query_output_property<Connection>
    {
      return cppual::randr::query_output_property(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_output_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_output_property<Connection>
    {
      return cppual::randr::query_output_property_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    configure_output_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::configure_output_property_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    configure_output_property(Parameter && ... parameter) const
    {
      cppual::randr::configure_output_property(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_output_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::change_output_property_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_output_property(Parameter && ... parameter) const
    {
      cppual::randr::change_output_property(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_output_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_property_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_output_property(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_property(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_output_property(Parameter && ... parameter) const
    -> reply::checked::get_output_property<Connection>
    {
      return cppual::randr::get_output_property(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_output_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_output_property<Connection>
    {
      return cppual::randr::get_output_property_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    create_mode(Parameter && ... parameter) const
    -> reply::checked::create_mode<Connection>
    {
      return cppual::randr::create_mode(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_mode_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_mode<Connection>
    {
      return cppual::randr::create_mode_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_mode_checked(Parameter && ... parameter) const
    {
      cppual::randr::destroy_mode_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_mode(Parameter && ... parameter) const
    {
      cppual::randr::destroy_mode(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    add_output_mode_checked(Parameter && ... parameter) const
    {
      cppual::randr::add_output_mode_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    add_output_mode(Parameter && ... parameter) const
    {
      cppual::randr::add_output_mode(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_output_mode_checked(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_mode_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_output_mode(Parameter && ... parameter) const
    {
      cppual::randr::delete_output_mode(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_crtc_info(Parameter && ... parameter) const
    -> reply::checked::get_crtc_info<Connection>
    {
      return cppual::randr::get_crtc_info(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_crtc_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_info<Connection>
    {
      return cppual::randr::get_crtc_info_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_crtc_config(Parameter && ... parameter) const
    -> reply::checked::set_crtc_config<Connection>
    {
      return cppual::randr::set_crtc_config(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_crtc_config_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_crtc_config<Connection>
    {
      return cppual::randr::set_crtc_config_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_crtc_gamma_size(Parameter && ... parameter) const
    -> reply::checked::get_crtc_gamma_size<Connection>
    {
      return cppual::randr::get_crtc_gamma_size(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_crtc_gamma_size_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_gamma_size<Connection>
    {
      return cppual::randr::get_crtc_gamma_size_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_crtc_gamma(Parameter && ... parameter) const
    -> reply::checked::get_crtc_gamma<Connection>
    {
      return cppual::randr::get_crtc_gamma(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_crtc_gamma_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_gamma<Connection>
    {
      return cppual::randr::get_crtc_gamma_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_crtc_gamma_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_gamma_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_crtc_gamma(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_gamma(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_screen_resources_current(Parameter && ... parameter) const
    -> reply::checked::get_screen_resources_current<Connection>
    {
      return cppual::randr::get_screen_resources_current(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_screen_resources_current_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_screen_resources_current<Connection>
    {
      return cppual::randr::get_screen_resources_current_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_crtc_transform_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_transform_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_crtc_transform(Parameter && ... parameter) const
    {
      cppual::randr::set_crtc_transform(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_crtc_transform(Parameter && ... parameter) const
    -> reply::checked::get_crtc_transform<Connection>
    {
      return cppual::randr::get_crtc_transform(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_crtc_transform_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_crtc_transform<Connection>
    {
      return cppual::randr::get_crtc_transform_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_panning(Parameter && ... parameter) const
    -> reply::checked::get_panning<Connection>
    {
      return cppual::randr::get_panning(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_panning_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_panning<Connection>
    {
      return cppual::randr::get_panning_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    set_panning(Parameter && ... parameter) const
    -> reply::checked::set_panning<Connection>
    {
      return cppual::randr::set_panning(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    set_panning_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::set_panning<Connection>
    {
      return cppual::randr::set_panning_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_output_primary_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_output_primary_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_output_primary(Parameter && ... parameter) const
    {
      cppual::randr::set_output_primary(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_output_primary(Parameter && ... parameter) const
    -> reply::checked::get_output_primary<Connection>
    {
      return cppual::randr::get_output_primary(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_output_primary_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_output_primary<Connection>
    {
      return cppual::randr::get_output_primary_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_providers(Parameter && ... parameter) const
    -> reply::checked::get_providers<Connection>
    {
      return cppual::randr::get_providers(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_providers_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_providers<Connection>
    {
      return cppual::randr::get_providers_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_provider_info(Parameter && ... parameter) const
    -> reply::checked::get_provider_info<Connection>
    {
      return cppual::randr::get_provider_info(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_provider_info_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_provider_info<Connection>
    {
      return cppual::randr::get_provider_info_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_provider_offload_sink_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_offload_sink_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_provider_offload_sink(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_offload_sink(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_provider_output_source_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_output_source_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_provider_output_source(Parameter && ... parameter) const
    {
      cppual::randr::set_provider_output_source(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    list_provider_properties(Parameter && ... parameter) const
    -> reply::checked::list_provider_properties<Connection>
    {
      return cppual::randr::list_provider_properties(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    list_provider_properties_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::list_provider_properties<Connection>
    {
      return cppual::randr::list_provider_properties_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_provider_property(Parameter && ... parameter) const
    -> reply::checked::query_provider_property<Connection>
    {
      return cppual::randr::query_provider_property(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_provider_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_provider_property<Connection>
    {
      return cppual::randr::query_provider_property_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    configure_provider_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::configure_provider_property_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    configure_provider_property(Parameter && ... parameter) const
    {
      cppual::randr::configure_provider_property(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_provider_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::change_provider_property_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_provider_property(Parameter && ... parameter) const
    {
      cppual::randr::change_provider_property(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_provider_property_checked(Parameter && ... parameter) const
    {
      cppual::randr::delete_provider_property_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_provider_property(Parameter && ... parameter) const
    {
      cppual::randr::delete_provider_property(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_provider_property(Parameter && ... parameter) const
    -> reply::checked::get_provider_property<Connection>
    {
      return cppual::randr::get_provider_property(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_provider_property_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_provider_property<Connection>
    {
      return cppual::randr::get_provider_property_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_monitors(Parameter && ... parameter) const
    -> reply::checked::get_monitors<Connection>
    {
      return cppual::randr::get_monitors(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_monitors_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_monitors<Connection>
    {
      return cppual::randr::get_monitors_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_monitor_checked(Parameter && ... parameter) const
    {
      cppual::randr::set_monitor_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_monitor(Parameter && ... parameter) const
    {
      cppual::randr::set_monitor(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_monitor_checked(Parameter && ... parameter) const
    {
      cppual::randr::delete_monitor_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_monitor(Parameter && ... parameter) const
    {
      cppual::randr::delete_monitor(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    create_lease(Parameter && ... parameter) const
    -> reply::checked::create_lease<Connection>
    {
      return cppual::randr::create_lease(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    create_lease_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::create_lease<Connection>
    {
      return cppual::randr::create_lease_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_lease_checked(Parameter && ... parameter) const
    {
      cppual::randr::free_lease_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free_lease(Parameter && ... parameter) const
    {
      cppual::randr::free_lease(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
  public:
    typedef cppual::randr::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
      : m_c(std::forward<C>(c))
      , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::randr::extension & extension)
      : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
      switch ((event->response_type & ~0x80) - m_first_event) {

        case XCB_RANDR_SCREEN_CHANGE_NOTIFY:
          handler(cppual::randr::event::screen_change_notify<Connection>(m_c, m_first_event, event));
          return true;

        case XCB_RANDR_NOTIFY:
          handler(cppual::randr::event::notify<Connection>(m_c, m_first_event, event));
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
    typedef cppual::randr::extension extension;

    dispatcher(uint8_t first_error)
      : m_first_error(first_error)
    {}

    dispatcher(const cppual::randr::extension & extension)
      : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
      switch (error->error_code - m_first_error) {

        case XCB_RANDR_BAD_OUTPUT: // 0
          throw cppual::randr::error::bad_output(error);

        case XCB_RANDR_BAD_CRTC: // 1
          throw cppual::randr::error::bad_crtc(error);

        case XCB_RANDR_BAD_MODE: // 2
          throw cppual::randr::error::bad_mode(error);

        case XCB_RANDR_BAD_PROVIDER: // 3
          throw cppual::randr::error::bad_provider(error);

      };
    }

  protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace xpp::randr

#endif // CPPUAL_RANDR_HPP
