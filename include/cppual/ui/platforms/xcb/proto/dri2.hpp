#ifndef CPPUAL_DRI2_HPP
#define CPPUAL_DRI2_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>
#include <cppual/string.h>

#include <vector>

#include <xcb/dri2.h>

namespace cppual { namespace dri2 {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
  : public cppual::generic::extension<extension, &xcb_dri2_id>
{
  public:
    using base = cppual::generic::extension<extension, &xcb_dri2_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::dri2::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::dri2::event::dispatcher<Connection>;
    using error_dispatcher = cppual::dri2::error::dispatcher;
};

namespace event {
template<typename Connection>
class buffer_swap_complete
  : public cppual::generic::event<xcb_dri2_buffer_swap_complete_event_t>
{
  public:
    typedef cppual::dri2::extension extension;

    typedef cppual::generic::event<xcb_dri2_buffer_swap_complete_event_t> base;

    template<typename C>
    buffer_swap_complete(C && c,
                         uint8_t first_event,
                         const std::shared_ptr<xcb_generic_event_t> & event)
      : base(event)
      , _M_c(std::forward<C>(c))
      , _M_first_event(first_event)
    {}

    virtual ~buffer_swap_complete(void) {}

    static uint8_t opcode(void)
    {
      return XCB_DRI2_BUFFER_SWAP_COMPLETE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
      return first_event + opcode();
    }

    static uint8_t opcode(const cppual::dri2::extension & extension)
    {
      return opcode(extension->first_event);
    }

    static string description(void)
    {
      return string("XCB_DRI2_BUFFER_SWAP_COMPLETE");
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

  protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class buffer_swap_complete



} // namespace event


namespace event {
template<typename Connection>
class invalidate_buffers
  : public cppual::generic::event<xcb_dri2_invalidate_buffers_event_t>
{
  public:
    typedef cppual::dri2::extension extension;

    typedef cppual::generic::event<xcb_dri2_invalidate_buffers_event_t> base;

    template<typename C>
    invalidate_buffers(C && c,
                       uint8_t first_event,
                       const std::shared_ptr<xcb_generic_event_t> & event)
      : base(event)
      , _M_c(std::forward<C>(c))
      , _M_first_event(first_event)
    {}

    virtual ~invalidate_buffers(void) {}

    static uint8_t opcode(void)
    {
      return XCB_DRI2_INVALIDATE_BUFFERS;
    }

    static uint8_t opcode(uint8_t first_event)
    {
      return first_event + opcode();
    }

    static uint8_t opcode(const cppual::dri2::extension & extension)
    {
      return opcode(extension->first_event);
    }

    static string description(void)
    {
      return string("XCB_DRI2_INVALIDATE_BUFFERS");
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

  protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class invalidate_buffers



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
                               SIGNATURE(xcb_dri2_query_version_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_query_version_reply),
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
    SIGNATURE(xcb_dri2_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_query_version_unchecked)>;
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
class connect
  : public cppual::generic::reply<connect<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_connect_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<connect<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_connect_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    connect(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    string
    driver_name(void)
    {
      return string(xcb_dri2_connect_driver_name(this->get().get()),
                         ::xcb_dri2_connect_driver_name_length(this->get().get()));
    }



    template<typename Type>
    cppual::generic::list<Connection,
                       ::xcb_dri2_connect_reply_t,
                       cppual::generic::iterator<Connection,
                                              Type,
                                              SIGNATURE(xcb_dri2_connect_alignment_pad),
                                              SIGNATURE(xcb_dri2_connect_alignment_pad_length)>
                      >
    alignment_pad(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_dri2_connect_reply_t,
                                cppual::generic::iterator<Connection,
                                                       Type,
                                                       SIGNATURE(xcb_dri2_connect_alignment_pad),
                                                       SIGNATURE(xcb_dri2_connect_alignment_pad_length)>
                               >(this->_M_c, this->get());
    }


    string
    device_name(void)
    {
      return string(xcb_dri2_connect_device_name(this->get().get()),
                         ::xcb_dri2_connect_device_name_length(this->get().get()));
    }

}; // class connect

} // namespace detail

namespace checked {
template<typename Connection>
using connect = detail::connect<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_connect)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using connect = detail::connect<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_connect_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::connect<Connection>
connect(Connection && c, Parameter && ... parameter)
{
  return reply::checked::connect<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::connect<Connection>
connect_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::connect<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class authenticate
  : public cppual::generic::reply<authenticate<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_authenticate_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<authenticate<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_authenticate_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    authenticate(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class authenticate

} // namespace detail

namespace checked {
template<typename Connection>
using authenticate = detail::authenticate<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_authenticate)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using authenticate = detail::authenticate<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_authenticate_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::authenticate<Connection>
authenticate(Connection && c, Parameter && ... parameter)
{
  return reply::checked::authenticate<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::authenticate<Connection>
authenticate_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::authenticate<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_drawable_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::dri2::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_dri2_create_drawable_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_drawable(Parameter && ... parameter)
{
  ::xcb_dri2_create_drawable(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_drawable_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::dri2::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_dri2_destroy_drawable_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_drawable(Parameter && ... parameter)
{
  ::xcb_dri2_destroy_drawable(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_buffers
  : public cppual::generic::reply<get_buffers<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_get_buffers_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_buffers<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_get_buffers_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_buffers(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_dri2_get_buffers_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_drawable_t drawable, uint32_t count, uint32_t attachments_len, const uint32_t * attachments)
    {
      return base::cookie(c, drawable, count, attachments_len, attachments);
    }

    template<typename Attachments_Iterator>
    static
    ::xcb_dri2_get_buffers_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_drawable_t drawable, uint32_t count, Attachments_Iterator attachments_begin, Attachments_Iterator attachments_end)
    {
      typedef typename value_type<Attachments_Iterator, ! std::is_pointer<Attachments_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> attachments =
        { value_iterator<Attachments_Iterator>(attachments_begin), value_iterator<Attachments_Iterator>(attachments_end) };

      return base::cookie(c, drawable, count, static_cast<uint32_t>(attachments.size()), const_cast<const vector_type *>(attachments.data()));
    }

    cppual::generic::list<Connection,
                       ::xcb_dri2_get_buffers_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_dri2_dri2_buffer_t,
                                              SIGNATURE(xcb_dri2_get_buffers_buffers),
                                              SIGNATURE(xcb_dri2_get_buffers_buffers_length)>
                      >
    buffers(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_dri2_get_buffers_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_dri2_dri2_buffer_t,
                                                       SIGNATURE(xcb_dri2_get_buffers_buffers),
                                                       SIGNATURE(xcb_dri2_get_buffers_buffers_length)>
                               >(this->_M_c, this->get());
    }
}; // class get_buffers

} // namespace detail

namespace checked {
template<typename Connection>
using get_buffers = detail::get_buffers<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_get_buffers)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_buffers = detail::get_buffers<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_get_buffers_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_buffers<Connection>
get_buffers(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_buffers<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_buffers<Connection>
get_buffers_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_buffers<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class copy_region
  : public cppual::generic::reply<copy_region<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_copy_region_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<copy_region<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_copy_region_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    copy_region(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class copy_region

} // namespace detail

namespace checked {
template<typename Connection>
using copy_region = detail::copy_region<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_copy_region)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using copy_region = detail::copy_region<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_copy_region_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::copy_region<Connection>
copy_region(Connection && c, Parameter && ... parameter)
{
  return reply::checked::copy_region<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::copy_region<Connection>
copy_region_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::copy_region<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_buffers_with_format
  : public cppual::generic::reply<get_buffers_with_format<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_get_buffers_with_format_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_buffers_with_format<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_get_buffers_with_format_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_buffers_with_format(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
    ::xcb_dri2_get_buffers_with_format_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_drawable_t drawable, uint32_t count, uint32_t attachments_len, const ::xcb_dri2_attach_format_t * attachments)
    {
      return base::cookie(c, drawable, count, attachments_len, attachments);
    }

    template<typename Attachments_Iterator>
    static
    ::xcb_dri2_get_buffers_with_format_cookie_t
    cookie(xcb_connection_t * const c, ::xcb_drawable_t drawable, uint32_t count, Attachments_Iterator attachments_begin, Attachments_Iterator attachments_end)
    {
      typedef typename value_type<Attachments_Iterator, ! std::is_pointer<Attachments_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> attachments =
        { value_iterator<Attachments_Iterator>(attachments_begin), value_iterator<Attachments_Iterator>(attachments_end) };

      return base::cookie(c, drawable, count, static_cast<uint32_t>(attachments.size()), const_cast<const vector_type *>(attachments.data()));
    }

    cppual::generic::list<Connection,
                       ::xcb_dri2_get_buffers_with_format_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_dri2_dri2_buffer_t,
                                              SIGNATURE(xcb_dri2_get_buffers_with_format_buffers),
                                              SIGNATURE(xcb_dri2_get_buffers_with_format_buffers_length)>
                      >
    buffers(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_dri2_get_buffers_with_format_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_dri2_dri2_buffer_t,
                                                       SIGNATURE(xcb_dri2_get_buffers_with_format_buffers),
                                                       SIGNATURE(xcb_dri2_get_buffers_with_format_buffers_length)>
                               >(this->_M_c, this->get());
    }
}; // class get_buffers_with_format

} // namespace detail

namespace checked {
template<typename Connection>
using get_buffers_with_format = detail::get_buffers_with_format<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_get_buffers_with_format)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_buffers_with_format = detail::get_buffers_with_format<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_get_buffers_with_format_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_buffers_with_format<Connection>
get_buffers_with_format(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_buffers_with_format<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_buffers_with_format<Connection>
get_buffers_with_format_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_buffers_with_format<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class swap_buffers
  : public cppual::generic::reply<swap_buffers<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_swap_buffers_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<swap_buffers<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_swap_buffers_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    swap_buffers(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class swap_buffers

} // namespace detail

namespace checked {
template<typename Connection>
using swap_buffers = detail::swap_buffers<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_swap_buffers)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using swap_buffers = detail::swap_buffers<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_swap_buffers_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::swap_buffers<Connection>
swap_buffers(Connection && c, Parameter && ... parameter)
{
  return reply::checked::swap_buffers<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::swap_buffers<Connection>
swap_buffers_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::swap_buffers<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_msc
  : public cppual::generic::reply<get_msc<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_get_msc_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_msc<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_get_msc_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_msc(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_msc

} // namespace detail

namespace checked {
template<typename Connection>
using get_msc = detail::get_msc<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_get_msc)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_msc = detail::get_msc<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_get_msc_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_msc<Connection>
get_msc(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_msc<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_msc<Connection>
get_msc_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_msc<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class wait_msc
  : public cppual::generic::reply<wait_msc<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_wait_msc_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<wait_msc<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_wait_msc_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    wait_msc(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class wait_msc

} // namespace detail

namespace checked {
template<typename Connection>
using wait_msc = detail::wait_msc<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_wait_msc)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using wait_msc = detail::wait_msc<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_wait_msc_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::wait_msc<Connection>
wait_msc(Connection && c, Parameter && ... parameter)
{
  return reply::checked::wait_msc<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::wait_msc<Connection>
wait_msc_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::wait_msc<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class wait_sbc
  : public cppual::generic::reply<wait_sbc<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_wait_sbc_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<wait_sbc<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_wait_sbc_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    wait_sbc(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class wait_sbc

} // namespace detail

namespace checked {
template<typename Connection>
using wait_sbc = detail::wait_sbc<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_wait_sbc)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using wait_sbc = detail::wait_sbc<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_wait_sbc_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::wait_sbc<Connection>
wait_sbc(Connection && c, Parameter && ... parameter)
{
  return reply::checked::wait_sbc<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::wait_sbc<Connection>
wait_sbc_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::wait_sbc<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
swap_interval_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::dri2::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_dri2_swap_interval_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
swap_interval(Parameter && ... parameter)
{
  ::xcb_dri2_swap_interval(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_param
  : public cppual::generic::reply<get_param<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_dri2_get_param_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<get_param<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_dri2_get_param_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    get_param(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_param

} // namespace detail

namespace checked {
template<typename Connection>
using get_param = detail::get_param<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_dri2_get_param)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_param = detail::get_param<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_dri2_get_param_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_param<Connection>
get_param(Connection && c, Parameter && ... parameter)
{
  return reply::checked::get_param<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_param<Connection>
get_param_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::get_param<Connection>(
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
    typedef cppual::dri2::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    dri2(void)
    {
      return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
      return cppual::dri2::query_version(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
      return cppual::dri2::query_version_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    connect(Parameter && ... parameter) const
    -> reply::checked::connect<Connection>
    {
      return cppual::dri2::connect(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    connect_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::connect<Connection>
    {
      return cppual::dri2::connect_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    authenticate(Parameter && ... parameter) const
    -> reply::checked::authenticate<Connection>
    {
      return cppual::dri2::authenticate(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    authenticate_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::authenticate<Connection>
    {
      return cppual::dri2::authenticate_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_drawable_checked(Parameter && ... parameter) const
    {
      cppual::dri2::create_drawable_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_drawable(Parameter && ... parameter) const
    {
      cppual::dri2::create_drawable(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_drawable_checked(Parameter && ... parameter) const
    {
      cppual::dri2::destroy_drawable_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_drawable(Parameter && ... parameter) const
    {
      cppual::dri2::destroy_drawable(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_buffers(Parameter && ... parameter) const
    -> reply::checked::get_buffers<Connection>
    {
      return cppual::dri2::get_buffers(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_buffers_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_buffers<Connection>
    {
      return cppual::dri2::get_buffers_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    copy_region(Parameter && ... parameter) const
    -> reply::checked::copy_region<Connection>
    {
      return cppual::dri2::copy_region(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    copy_region_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::copy_region<Connection>
    {
      return cppual::dri2::copy_region_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_buffers_with_format(Parameter && ... parameter) const
    -> reply::checked::get_buffers_with_format<Connection>
    {
      return cppual::dri2::get_buffers_with_format(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_buffers_with_format_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_buffers_with_format<Connection>
    {
      return cppual::dri2::get_buffers_with_format_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    swap_buffers(Parameter && ... parameter) const
    -> reply::checked::swap_buffers<Connection>
    {
      return cppual::dri2::swap_buffers(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    swap_buffers_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::swap_buffers<Connection>
    {
      return cppual::dri2::swap_buffers_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_msc(Parameter && ... parameter) const
    -> reply::checked::get_msc<Connection>
    {
      return cppual::dri2::get_msc(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_msc_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_msc<Connection>
    {
      return cppual::dri2::get_msc_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    wait_msc(Parameter && ... parameter) const
    -> reply::checked::wait_msc<Connection>
    {
      return cppual::dri2::wait_msc(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    wait_msc_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::wait_msc<Connection>
    {
      return cppual::dri2::wait_msc_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    wait_sbc(Parameter && ... parameter) const
    -> reply::checked::wait_sbc<Connection>
    {
      return cppual::dri2::wait_sbc(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    wait_sbc_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::wait_sbc<Connection>
    {
      return cppual::dri2::wait_sbc_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    swap_interval_checked(Parameter && ... parameter) const
    {
      cppual::dri2::swap_interval_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    swap_interval(Parameter && ... parameter) const
    {
      cppual::dri2::swap_interval(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_param(Parameter && ... parameter) const
    -> reply::checked::get_param<Connection>
    {
      return cppual::dri2::get_param(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_param_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_param<Connection>
    {
      return cppual::dri2::get_param_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
  public:
    typedef cppual::dri2::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
      : _M_c(std::forward<C>(c))
      , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::dri2::extension & extension)
      : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
      switch ((event->response_type & ~0x80) - _M_first_event) {

        case XCB_DRI2_BUFFER_SWAP_COMPLETE:
          handler(cppual::dri2::event::buffer_swap_complete<Connection>(_M_c, _M_first_event, event));
          return true;

        case XCB_DRI2_INVALIDATE_BUFFERS:
          handler(cppual::dri2::event::invalidate_buffers<Connection>(_M_c, _M_first_event, event));
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
    typedef cppual::dri2::extension extension;

    dispatcher(uint8_t first_error)
      : _M_first_error(first_error)
    {}

    dispatcher(const cppual::dri2::extension & extension)
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


} } // namespace cppual::dri2

#endif // CPPUAL_DRI2_HPP
