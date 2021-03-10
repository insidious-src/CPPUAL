#ifndef CPPUAL_GLX_HPP
#define CPPUAL_GLX_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/glx.h>

namespace cppual { namespace glx {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_glx_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_glx_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::glx::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::glx::event::dispatcher<Connection>;
    using error_dispatcher = cppual::glx::error::dispatcher;
};

namespace event {
template<typename Connection>
class pbuffer_clobber
        : public cppual::generic::event<xcb_glx_pbuffer_clobber_event_t>
{
public:
    typedef cppual::glx::extension extension;

    typedef cppual::generic::event<xcb_glx_pbuffer_clobber_event_t> base;

    template<typename C>
    pbuffer_clobber(C && c,
                    uint8_t first_event,
                    const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~pbuffer_clobber(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_PBUFFER_CLOBBER;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_PBUFFER_CLOBBER");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_glx_drawable_t, typename ... Parameter>
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

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class pbuffer_clobber



} // namespace event


namespace event {
template<typename Connection>
class buffer_swap_complete
        : public cppual::generic::event<xcb_glx_buffer_swap_complete_event_t>
{
public:
    typedef cppual::glx::extension extension;

    typedef cppual::generic::event<xcb_glx_buffer_swap_complete_event_t> base;

    template<typename C>
    buffer_swap_complete(C && c,
                         uint8_t first_event,
                         const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~buffer_swap_complete(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BUFFER_SWAP_COMPLETE;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BUFFER_SWAP_COMPLETE");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_glx_drawable_t, typename ... Parameter>
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

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class buffer_swap_complete



} // namespace event



namespace error {
class generic
        : public cppual::generic::error<generic,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<generic, ::xcb_glx_generic_error_t>::error;

    virtual ~generic(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_GENERIC;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_GENERIC");
    }

protected:
    uint8_t m_first_error;
}; // class generic
} // namespace error


namespace error {
class bad_context
        : public cppual::generic::error<bad_context,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_context, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_context(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_CONTEXT;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_CONTEXT");
    }

protected:
    uint8_t m_first_error;
}; // class bad_context
} // namespace error


namespace error {
class bad_context_state
        : public cppual::generic::error<bad_context_state,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_context_state, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_context_state(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_CONTEXT_STATE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_CONTEXT_STATE");
    }

protected:
    uint8_t m_first_error;
}; // class bad_context_state
} // namespace error


namespace error {
class bad_drawable
        : public cppual::generic::error<bad_drawable,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_drawable, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_drawable(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_DRAWABLE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_DRAWABLE");
    }

protected:
    uint8_t m_first_error;
}; // class bad_drawable
} // namespace error


namespace error {
class bad_pixmap
        : public cppual::generic::error<bad_pixmap,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_pixmap, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_pixmap(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_PIXMAP;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_PIXMAP");
    }

protected:
    uint8_t m_first_error;
}; // class bad_pixmap
} // namespace error


namespace error {
class bad_context_tag
        : public cppual::generic::error<bad_context_tag,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_context_tag, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_context_tag(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_CONTEXT_TAG;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_CONTEXT_TAG");
    }

protected:
    uint8_t m_first_error;
}; // class bad_context_tag
} // namespace error


namespace error {
class bad_current_window
        : public cppual::generic::error<bad_current_window,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_current_window, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_current_window(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_CURRENT_WINDOW;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_CURRENT_WINDOW");
    }

protected:
    uint8_t m_first_error;
}; // class bad_current_window
} // namespace error


namespace error {
class bad_render_request
        : public cppual::generic::error<bad_render_request,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_render_request, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_render_request(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_RENDER_REQUEST;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_RENDER_REQUEST");
    }

protected:
    uint8_t m_first_error;
}; // class bad_render_request
} // namespace error


namespace error {
class bad_large_request
        : public cppual::generic::error<bad_large_request,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_large_request, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_large_request(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_LARGE_REQUEST;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_LARGE_REQUEST");
    }

protected:
    uint8_t m_first_error;
}; // class bad_large_request
} // namespace error


namespace error {
class unsupported_private_request
        : public cppual::generic::error<unsupported_private_request,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<unsupported_private_request, ::xcb_glx_generic_error_t>::error;

    virtual ~unsupported_private_request(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_UNSUPPORTED_PRIVATE_REQUEST;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_UNSUPPORTED_PRIVATE_REQUEST");
    }

protected:
    uint8_t m_first_error;
}; // class unsupported_private_request
} // namespace error


namespace error {
class bad_fb_config
        : public cppual::generic::error<bad_fb_config,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_fb_config, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_fb_config(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_FB_CONFIG;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_FB_CONFIG");
    }

protected:
    uint8_t m_first_error;
}; // class bad_fb_config
} // namespace error


namespace error {
class bad_pbuffer
        : public cppual::generic::error<bad_pbuffer,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_pbuffer, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_pbuffer(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_PBUFFER;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_PBUFFER");
    }

protected:
    uint8_t m_first_error;
}; // class bad_pbuffer
} // namespace error


namespace error {
class bad_current_drawable
        : public cppual::generic::error<bad_current_drawable,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_current_drawable, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_current_drawable(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_CURRENT_DRAWABLE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_CURRENT_DRAWABLE");
    }

protected:
    uint8_t m_first_error;
}; // class bad_current_drawable
} // namespace error


namespace error {
class bad_window
        : public cppual::generic::error<bad_window,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<bad_window, ::xcb_glx_generic_error_t>::error;

    virtual ~bad_window(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_BAD_WINDOW;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_BAD_WINDOW");
    }

protected:
    uint8_t m_first_error;
}; // class bad_window
} // namespace error


namespace error {
class glx_bad_profile_arb
        : public cppual::generic::error<glx_bad_profile_arb,
        ::xcb_glx_generic_error_t>
{
public:
    using cppual::generic::error<glx_bad_profile_arb, ::xcb_glx_generic_error_t>::error;

    virtual ~glx_bad_profile_arb(void) {}

    static uint8_t opcode(void)
    {
        return XCB_GLX_GLX_BAD_PROFILE_ARB;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::glx::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_GLX_GLX_BAD_PROFILE_ARB");
    }

protected:
    uint8_t m_first_error;
}; // class glx_bad_profile_arb
} // namespace error


template<typename Connection>
void
render_checked(Connection && c, ::xcb_glx_context_tag_t context_tag, uint32_t data_len, const uint8_t * data)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_render_checked(std::forward<Connection>(c), context_tag, data_len, data));
}

template<typename Connection>
void
render(Connection && c, ::xcb_glx_context_tag_t context_tag, uint32_t data_len, const uint8_t * data)
{
    ::xcb_glx_render(std::forward<Connection>(c), context_tag, data_len, data);
}

template<typename Connection, typename Data_Iterator>
void
render_checked(Connection && c, ::xcb_glx_context_tag_t context_tag, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_render_checked(std::forward<Connection>(c), context_tag, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data())));
}

template<typename Connection, typename Data_Iterator>
void
render(Connection && c, ::xcb_glx_context_tag_t context_tag, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    ::xcb_glx_render(std::forward<Connection>(c), context_tag, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data()));
}

template<typename Connection>
void
render_large_checked(Connection && c, ::xcb_glx_context_tag_t context_tag, uint16_t request_num, uint16_t request_total, uint32_t data_len, const uint8_t * data)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_render_large_checked(std::forward<Connection>(c), context_tag, request_num, request_total, data_len, data));
}

template<typename Connection>
void
render_large(Connection && c, ::xcb_glx_context_tag_t context_tag, uint16_t request_num, uint16_t request_total, uint32_t data_len, const uint8_t * data)
{
    ::xcb_glx_render_large(std::forward<Connection>(c), context_tag, request_num, request_total, data_len, data);
}

template<typename Connection, typename Data_Iterator>
void
render_large_checked(Connection && c, ::xcb_glx_context_tag_t context_tag, uint16_t request_num, uint16_t request_total, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_render_large_checked(std::forward<Connection>(c), context_tag, request_num, request_total, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data())));
}

template<typename Connection, typename Data_Iterator>
void
render_large(Connection && c, ::xcb_glx_context_tag_t context_tag, uint16_t request_num, uint16_t request_total, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    ::xcb_glx_render_large(std::forward<Connection>(c), context_tag, request_num, request_total, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data()));
}

template<typename Connection, typename ... Parameter>
void
create_context_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_create_context_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_context(Parameter && ... parameter)
{
    ::xcb_glx_create_context(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_context_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_destroy_context_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_context(Parameter && ... parameter)
{
    ::xcb_glx_destroy_context(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class make_current
        : public cppual::generic::reply<make_current<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_make_current_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<make_current<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_make_current_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    make_current(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class make_current

} // namespace detail

namespace checked {
template<typename Connection>
using make_current = detail::make_current<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_make_current)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using make_current = detail::make_current<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_make_current_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::make_current<Connection>
make_current(Connection && c, Parameter && ... parameter)
{
    return reply::checked::make_current<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::make_current<Connection>
make_current_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::make_current<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class is_direct
        : public cppual::generic::reply<is_direct<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_is_direct_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<is_direct<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_is_direct_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    is_direct(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class is_direct

} // namespace detail

namespace checked {
template<typename Connection>
using is_direct = detail::is_direct<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_is_direct)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using is_direct = detail::is_direct<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_is_direct_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::is_direct<Connection>
is_direct(Connection && c, Parameter && ... parameter)
{
    return reply::checked::is_direct<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::is_direct<Connection>
is_direct_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::is_direct<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_version
        : public cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_query_version_reply),
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
SIGNATURE(xcb_glx_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_query_version_unchecked)>;
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
wait_gl_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_wait_gl_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
wait_gl(Parameter && ... parameter)
{
    ::xcb_glx_wait_gl(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
wait_x_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_wait_x_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
wait_x(Parameter && ... parameter)
{
    ::xcb_glx_wait_x(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
copy_context_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_copy_context_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
copy_context(Parameter && ... parameter)
{
    ::xcb_glx_copy_context(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
swap_buffers_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_swap_buffers_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
swap_buffers(Parameter && ... parameter)
{
    ::xcb_glx_swap_buffers(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
use_x_font_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_use_x_font_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
use_x_font(Parameter && ... parameter)
{
    ::xcb_glx_use_x_font(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_glx_pixmap_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_create_glx_pixmap_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_glx_pixmap(Parameter && ... parameter)
{
    ::xcb_glx_create_glx_pixmap(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_visual_configs
        : public cppual::generic::reply<get_visual_configs<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_visual_configs_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_visual_configs<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_visual_configs_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_visual_configs(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_visual_configs_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_get_visual_configs_property_list),
            SIGNATURE(xcb_glx_get_visual_configs_property_list_length)>
            >
            property_list(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_visual_configs_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_get_visual_configs_property_list),
                SIGNATURE(xcb_glx_get_visual_configs_property_list_length)>
                >(this->m_c, this->get());
    }
}; // class get_visual_configs

} // namespace detail

namespace checked {
template<typename Connection>
using get_visual_configs = detail::get_visual_configs<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_visual_configs)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_visual_configs = detail::get_visual_configs<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_visual_configs_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_visual_configs<Connection>
get_visual_configs(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_visual_configs<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_visual_configs<Connection>
get_visual_configs_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_visual_configs<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_glx_pixmap_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_destroy_glx_pixmap_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_glx_pixmap(Parameter && ... parameter)
{
    ::xcb_glx_destroy_glx_pixmap(std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
vendor_private_checked(Connection && c, uint32_t vendor_code, ::xcb_glx_context_tag_t context_tag, uint32_t data_len, const uint8_t * data)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_vendor_private_checked(std::forward<Connection>(c), vendor_code, context_tag, data_len, data));
}

template<typename Connection>
void
vendor_private(Connection && c, uint32_t vendor_code, ::xcb_glx_context_tag_t context_tag, uint32_t data_len, const uint8_t * data)
{
    ::xcb_glx_vendor_private(std::forward<Connection>(c), vendor_code, context_tag, data_len, data);
}

template<typename Connection, typename Data_Iterator>
void
vendor_private_checked(Connection && c, uint32_t vendor_code, ::xcb_glx_context_tag_t context_tag, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_vendor_private_checked(std::forward<Connection>(c), vendor_code, context_tag, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data())));
}

template<typename Connection, typename Data_Iterator>
void
vendor_private(Connection && c, uint32_t vendor_code, ::xcb_glx_context_tag_t context_tag, Data_Iterator data_begin, Data_Iterator data_end)
{
    typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> data =
    { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

    ::xcb_glx_vendor_private(std::forward<Connection>(c), vendor_code, context_tag, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data()));
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class vendor_private_with_reply
        : public cppual::generic::reply<vendor_private_with_reply<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_vendor_private_with_reply_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<vendor_private_with_reply<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_vendor_private_with_reply_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    vendor_private_with_reply(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

    static
            ::xcb_glx_vendor_private_with_reply_cookie_t
            cookie(xcb_connection_t * const c, uint32_t vendor_code, ::xcb_glx_context_tag_t context_tag, uint32_t data_len, const uint8_t * data)
    {
        return base::cookie(c, vendor_code, context_tag, data_len, data);
    }

    template<typename Data_Iterator>
    static
            ::xcb_glx_vendor_private_with_reply_cookie_t
            cookie(xcb_connection_t * const c, uint32_t vendor_code, ::xcb_glx_context_tag_t context_tag, Data_Iterator data_begin, Data_Iterator data_end)
    {
        typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
                vector_type;
        std::vector<vector_type> data =
        { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

        return base::cookie(c, vendor_code, context_tag, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data()));
    }

    cppual::generic::list<Connection,
            ::xcb_glx_vendor_private_with_reply_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_vendor_private_with_reply_data_2),
            SIGNATURE(xcb_glx_vendor_private_with_reply_data_2_length)>
            >
            data_2(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_vendor_private_with_reply_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_vendor_private_with_reply_data_2),
                SIGNATURE(xcb_glx_vendor_private_with_reply_data_2_length)>
                >(this->m_c, this->get());
    }
}; // class vendor_private_with_reply

} // namespace detail

namespace checked {
template<typename Connection>
using vendor_private_with_reply = detail::vendor_private_with_reply<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_vendor_private_with_reply)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using vendor_private_with_reply = detail::vendor_private_with_reply<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_vendor_private_with_reply_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::vendor_private_with_reply<Connection>
vendor_private_with_reply(Connection && c, Parameter && ... parameter)
{
    return reply::checked::vendor_private_with_reply<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::vendor_private_with_reply<Connection>
vendor_private_with_reply_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::vendor_private_with_reply<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_extensions_string
        : public cppual::generic::reply<query_extensions_string<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_query_extensions_string_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_extensions_string<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_query_extensions_string_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_extensions_string(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_extensions_string

} // namespace detail

namespace checked {
template<typename Connection>
using query_extensions_string = detail::query_extensions_string<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_query_extensions_string)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_extensions_string = detail::query_extensions_string<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_query_extensions_string_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_extensions_string<Connection>
query_extensions_string(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_extensions_string<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_extensions_string<Connection>
query_extensions_string_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_extensions_string<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_server_string
        : public cppual::generic::reply<query_server_string<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_query_server_string_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_server_string<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_query_server_string_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_server_string(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            string(void)
    {
        return std::string(xcb_glx_query_server_string_string(this->get().get()),
                           ::xcb_glx_query_server_string_string_length(this->get().get()));
    }

}; // class query_server_string

} // namespace detail

namespace checked {
template<typename Connection>
using query_server_string = detail::query_server_string<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_query_server_string)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_server_string = detail::query_server_string<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_query_server_string_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_server_string<Connection>
query_server_string(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_server_string<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_server_string<Connection>
query_server_string_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_server_string<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
client_info_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_client_info_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
client_info(Parameter && ... parameter)
{
    ::xcb_glx_client_info(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_fb_configs
        : public cppual::generic::reply<get_fb_configs<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_fb_configs_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_fb_configs<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_fb_configs_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_fb_configs(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_fb_configs_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_get_fb_configs_property_list),
            SIGNATURE(xcb_glx_get_fb_configs_property_list_length)>
            >
            property_list(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_fb_configs_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_get_fb_configs_property_list),
                SIGNATURE(xcb_glx_get_fb_configs_property_list_length)>
                >(this->m_c, this->get());
    }
}; // class get_fb_configs

} // namespace detail

namespace checked {
template<typename Connection>
using get_fb_configs = detail::get_fb_configs<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_fb_configs)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_fb_configs = detail::get_fb_configs<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_fb_configs_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_fb_configs<Connection>
get_fb_configs(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_fb_configs<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_fb_configs<Connection>
get_fb_configs_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_fb_configs<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_pixmap_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_create_pixmap_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_pixmap(Parameter && ... parameter)
{
    ::xcb_glx_create_pixmap(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_pixmap_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_destroy_pixmap_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_pixmap(Parameter && ... parameter)
{
    ::xcb_glx_destroy_pixmap(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_new_context_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_create_new_context_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_new_context(Parameter && ... parameter)
{
    ::xcb_glx_create_new_context(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_context
        : public cppual::generic::reply<query_context<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_query_context_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_context<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_query_context_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_context(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_query_context_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_query_context_attribs),
            SIGNATURE(xcb_glx_query_context_attribs_length)>
            >
            attribs(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_query_context_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_query_context_attribs),
                SIGNATURE(xcb_glx_query_context_attribs_length)>
                >(this->m_c, this->get());
    }
}; // class query_context

} // namespace detail

namespace checked {
template<typename Connection>
using query_context = detail::query_context<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_query_context)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_context = detail::query_context<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_query_context_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_context<Connection>
query_context(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_context<Connection>
query_context_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_context<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class make_context_current
        : public cppual::generic::reply<make_context_current<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_make_context_current_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<make_context_current<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_make_context_current_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    make_context_current(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class make_context_current

} // namespace detail

namespace checked {
template<typename Connection>
using make_context_current = detail::make_context_current<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_make_context_current)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using make_context_current = detail::make_context_current<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_make_context_current_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::make_context_current<Connection>
make_context_current(Connection && c, Parameter && ... parameter)
{
    return reply::checked::make_context_current<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::make_context_current<Connection>
make_context_current_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::make_context_current<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_pbuffer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_create_pbuffer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_pbuffer(Parameter && ... parameter)
{
    ::xcb_glx_create_pbuffer(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_pbuffer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_destroy_pbuffer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_pbuffer(Parameter && ... parameter)
{
    ::xcb_glx_destroy_pbuffer(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_drawable_attributes
        : public cppual::generic::reply<get_drawable_attributes<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_drawable_attributes_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_drawable_attributes<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_drawable_attributes_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_drawable_attributes(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_drawable_attributes_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_get_drawable_attributes_attribs),
            SIGNATURE(xcb_glx_get_drawable_attributes_attribs_length)>
            >
            attribs(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_drawable_attributes_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_get_drawable_attributes_attribs),
                SIGNATURE(xcb_glx_get_drawable_attributes_attribs_length)>
                >(this->m_c, this->get());
    }
}; // class get_drawable_attributes

} // namespace detail

namespace checked {
template<typename Connection>
using get_drawable_attributes = detail::get_drawable_attributes<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_drawable_attributes)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_drawable_attributes = detail::get_drawable_attributes<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_drawable_attributes_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_drawable_attributes<Connection>
get_drawable_attributes(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_drawable_attributes<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_drawable_attributes<Connection>
get_drawable_attributes_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_drawable_attributes<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_drawable_attributes_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_change_drawable_attributes_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_drawable_attributes(Parameter && ... parameter)
{
    ::xcb_glx_change_drawable_attributes(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_window_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_create_window_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_window(Parameter && ... parameter)
{
    ::xcb_glx_create_window(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_window_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_delete_window_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_window(Parameter && ... parameter)
{
    ::xcb_glx_delete_window(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_client_info_arb_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_set_client_info_arb_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_client_info_arb(Parameter && ... parameter)
{
    ::xcb_glx_set_client_info_arb(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_context_attribs_arb_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_create_context_attribs_arb_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_context_attribs_arb(Parameter && ... parameter)
{
    ::xcb_glx_create_context_attribs_arb(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_client_info_2arb_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_set_client_info_2arb_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_client_info_2arb(Parameter && ... parameter)
{
    ::xcb_glx_set_client_info_2arb(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
new_list_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_new_list_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
new_list(Parameter && ... parameter)
{
    ::xcb_glx_new_list(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
end_list_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_end_list_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
end_list(Parameter && ... parameter)
{
    ::xcb_glx_end_list(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_lists_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_delete_lists_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_lists(Parameter && ... parameter)
{
    ::xcb_glx_delete_lists(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class gen_lists
        : public cppual::generic::reply<gen_lists<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_gen_lists_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<gen_lists<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_gen_lists_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    gen_lists(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class gen_lists

} // namespace detail

namespace checked {
template<typename Connection>
using gen_lists = detail::gen_lists<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_gen_lists)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using gen_lists = detail::gen_lists<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_gen_lists_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::gen_lists<Connection>
gen_lists(Connection && c, Parameter && ... parameter)
{
    return reply::checked::gen_lists<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::gen_lists<Connection>
gen_lists_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::gen_lists<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
feedback_buffer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_feedback_buffer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
feedback_buffer(Parameter && ... parameter)
{
    ::xcb_glx_feedback_buffer(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_buffer_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_select_buffer_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_buffer(Parameter && ... parameter)
{
    ::xcb_glx_select_buffer(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class render_mode
        : public cppual::generic::reply<render_mode<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_render_mode_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<render_mode<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_render_mode_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    render_mode(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_render_mode_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_render_mode_data),
            SIGNATURE(xcb_glx_render_mode_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_render_mode_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_render_mode_data),
                SIGNATURE(xcb_glx_render_mode_data_length)>
                >(this->m_c, this->get());
    }
}; // class render_mode

} // namespace detail

namespace checked {
template<typename Connection>
using render_mode = detail::render_mode<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_render_mode)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using render_mode = detail::render_mode<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_render_mode_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::render_mode<Connection>
render_mode(Connection && c, Parameter && ... parameter)
{
    return reply::checked::render_mode<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::render_mode<Connection>
render_mode_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::render_mode<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class finish
        : public cppual::generic::reply<finish<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_finish_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<finish<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_finish_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    finish(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class finish

} // namespace detail

namespace checked {
template<typename Connection>
using finish = detail::finish<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_finish)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using finish = detail::finish<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_finish_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::finish<Connection>
finish(Connection && c, Parameter && ... parameter)
{
    return reply::checked::finish<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::finish<Connection>
finish_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::finish<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
pixel_storef_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_pixel_storef_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
pixel_storef(Parameter && ... parameter)
{
    ::xcb_glx_pixel_storef(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
pixel_storei_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_pixel_storei_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
pixel_storei(Parameter && ... parameter)
{
    ::xcb_glx_pixel_storei(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class read_pixels
        : public cppual::generic::reply<read_pixels<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_read_pixels_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<read_pixels<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_read_pixels_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    read_pixels(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_read_pixels_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_read_pixels_data),
            SIGNATURE(xcb_glx_read_pixels_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_read_pixels_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_read_pixels_data),
                SIGNATURE(xcb_glx_read_pixels_data_length)>
                >(this->m_c, this->get());
    }
}; // class read_pixels

} // namespace detail

namespace checked {
template<typename Connection>
using read_pixels = detail::read_pixels<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_read_pixels)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using read_pixels = detail::read_pixels<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_read_pixels_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::read_pixels<Connection>
read_pixels(Connection && c, Parameter && ... parameter)
{
    return reply::checked::read_pixels<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::read_pixels<Connection>
read_pixels_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::read_pixels<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_booleanv
        : public cppual::generic::reply<get_booleanv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_booleanv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_booleanv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_booleanv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_booleanv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_booleanv_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_booleanv_data),
            SIGNATURE(xcb_glx_get_booleanv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_booleanv_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_booleanv_data),
                SIGNATURE(xcb_glx_get_booleanv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_booleanv

} // namespace detail

namespace checked {
template<typename Connection>
using get_booleanv = detail::get_booleanv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_booleanv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_booleanv = detail::get_booleanv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_booleanv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_booleanv<Connection>
get_booleanv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_booleanv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_booleanv<Connection>
get_booleanv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_booleanv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_clip_plane
        : public cppual::generic::reply<get_clip_plane<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_clip_plane_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_clip_plane<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_clip_plane_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_clip_plane(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_clip_plane_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float64_t,
            SIGNATURE(xcb_glx_get_clip_plane_data),
            SIGNATURE(xcb_glx_get_clip_plane_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_clip_plane_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float64_t,
                SIGNATURE(xcb_glx_get_clip_plane_data),
                SIGNATURE(xcb_glx_get_clip_plane_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_clip_plane

} // namespace detail

namespace checked {
template<typename Connection>
using get_clip_plane = detail::get_clip_plane<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_clip_plane)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_clip_plane = detail::get_clip_plane<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_clip_plane_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_clip_plane<Connection>
get_clip_plane(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_clip_plane<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_clip_plane<Connection>
get_clip_plane_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_clip_plane<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_doublev
        : public cppual::generic::reply<get_doublev<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_doublev_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_doublev<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_doublev_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_doublev(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_doublev_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float64_t,
            SIGNATURE(xcb_glx_get_doublev_data),
            SIGNATURE(xcb_glx_get_doublev_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_doublev_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float64_t,
                SIGNATURE(xcb_glx_get_doublev_data),
                SIGNATURE(xcb_glx_get_doublev_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_doublev

} // namespace detail

namespace checked {
template<typename Connection>
using get_doublev = detail::get_doublev<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_doublev)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_doublev = detail::get_doublev<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_doublev_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_doublev<Connection>
get_doublev(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_doublev<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_doublev<Connection>
get_doublev_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_doublev<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_error
        : public cppual::generic::reply<get_error<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_error_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_error<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_error_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_error(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class get_error

} // namespace detail

namespace checked {
template<typename Connection>
using get_error = detail::get_error<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_error)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_error = detail::get_error<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_error_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_error<Connection>
get_error(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_error<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_error<Connection>
get_error_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_error<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_floatv
        : public cppual::generic::reply<get_floatv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_floatv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_floatv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_floatv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_floatv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_floatv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_floatv_data),
            SIGNATURE(xcb_glx_get_floatv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_floatv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_floatv_data),
                SIGNATURE(xcb_glx_get_floatv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_floatv

} // namespace detail

namespace checked {
template<typename Connection>
using get_floatv = detail::get_floatv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_floatv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_floatv = detail::get_floatv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_floatv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_floatv<Connection>
get_floatv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_floatv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_floatv<Connection>
get_floatv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_floatv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_integerv
        : public cppual::generic::reply<get_integerv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_integerv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_integerv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_integerv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_integerv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_integerv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_integerv_data),
            SIGNATURE(xcb_glx_get_integerv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_integerv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_integerv_data),
                SIGNATURE(xcb_glx_get_integerv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_integerv

} // namespace detail

namespace checked {
template<typename Connection>
using get_integerv = detail::get_integerv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_integerv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_integerv = detail::get_integerv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_integerv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_integerv<Connection>
get_integerv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_integerv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_integerv<Connection>
get_integerv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_integerv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_lightfv
        : public cppual::generic::reply<get_lightfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_lightfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_lightfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_lightfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_lightfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_lightfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_lightfv_data),
            SIGNATURE(xcb_glx_get_lightfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_lightfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_lightfv_data),
                SIGNATURE(xcb_glx_get_lightfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_lightfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_lightfv = detail::get_lightfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_lightfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_lightfv = detail::get_lightfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_lightfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_lightfv<Connection>
get_lightfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_lightfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_lightfv<Connection>
get_lightfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_lightfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_lightiv
        : public cppual::generic::reply<get_lightiv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_lightiv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_lightiv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_lightiv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_lightiv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_lightiv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_lightiv_data),
            SIGNATURE(xcb_glx_get_lightiv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_lightiv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_lightiv_data),
                SIGNATURE(xcb_glx_get_lightiv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_lightiv

} // namespace detail

namespace checked {
template<typename Connection>
using get_lightiv = detail::get_lightiv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_lightiv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_lightiv = detail::get_lightiv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_lightiv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_lightiv<Connection>
get_lightiv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_lightiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_lightiv<Connection>
get_lightiv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_lightiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_mapdv
        : public cppual::generic::reply<get_mapdv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_mapdv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_mapdv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_mapdv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_mapdv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_mapdv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float64_t,
            SIGNATURE(xcb_glx_get_mapdv_data),
            SIGNATURE(xcb_glx_get_mapdv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_mapdv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float64_t,
                SIGNATURE(xcb_glx_get_mapdv_data),
                SIGNATURE(xcb_glx_get_mapdv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_mapdv

} // namespace detail

namespace checked {
template<typename Connection>
using get_mapdv = detail::get_mapdv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_mapdv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_mapdv = detail::get_mapdv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_mapdv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_mapdv<Connection>
get_mapdv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_mapdv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_mapdv<Connection>
get_mapdv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_mapdv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_mapfv
        : public cppual::generic::reply<get_mapfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_mapfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_mapfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_mapfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_mapfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_mapfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_mapfv_data),
            SIGNATURE(xcb_glx_get_mapfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_mapfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_mapfv_data),
                SIGNATURE(xcb_glx_get_mapfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_mapfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_mapfv = detail::get_mapfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_mapfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_mapfv = detail::get_mapfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_mapfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_mapfv<Connection>
get_mapfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_mapfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_mapfv<Connection>
get_mapfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_mapfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_mapiv
        : public cppual::generic::reply<get_mapiv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_mapiv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_mapiv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_mapiv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_mapiv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_mapiv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_mapiv_data),
            SIGNATURE(xcb_glx_get_mapiv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_mapiv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_mapiv_data),
                SIGNATURE(xcb_glx_get_mapiv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_mapiv

} // namespace detail

namespace checked {
template<typename Connection>
using get_mapiv = detail::get_mapiv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_mapiv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_mapiv = detail::get_mapiv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_mapiv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_mapiv<Connection>
get_mapiv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_mapiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_mapiv<Connection>
get_mapiv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_mapiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_materialfv
        : public cppual::generic::reply<get_materialfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_materialfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_materialfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_materialfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_materialfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_materialfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_materialfv_data),
            SIGNATURE(xcb_glx_get_materialfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_materialfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_materialfv_data),
                SIGNATURE(xcb_glx_get_materialfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_materialfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_materialfv = detail::get_materialfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_materialfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_materialfv = detail::get_materialfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_materialfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_materialfv<Connection>
get_materialfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_materialfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_materialfv<Connection>
get_materialfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_materialfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_materialiv
        : public cppual::generic::reply<get_materialiv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_materialiv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_materialiv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_materialiv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_materialiv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_materialiv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_materialiv_data),
            SIGNATURE(xcb_glx_get_materialiv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_materialiv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_materialiv_data),
                SIGNATURE(xcb_glx_get_materialiv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_materialiv

} // namespace detail

namespace checked {
template<typename Connection>
using get_materialiv = detail::get_materialiv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_materialiv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_materialiv = detail::get_materialiv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_materialiv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_materialiv<Connection>
get_materialiv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_materialiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_materialiv<Connection>
get_materialiv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_materialiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_pixel_mapfv
        : public cppual::generic::reply<get_pixel_mapfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_pixel_mapfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_pixel_mapfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_pixel_mapfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_pixel_mapfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_pixel_mapfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_pixel_mapfv_data),
            SIGNATURE(xcb_glx_get_pixel_mapfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_pixel_mapfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_pixel_mapfv_data),
                SIGNATURE(xcb_glx_get_pixel_mapfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_pixel_mapfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_pixel_mapfv = detail::get_pixel_mapfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_pixel_mapfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_pixel_mapfv = detail::get_pixel_mapfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_pixel_mapfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_pixel_mapfv<Connection>
get_pixel_mapfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_pixel_mapfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_pixel_mapfv<Connection>
get_pixel_mapfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_pixel_mapfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_pixel_mapuiv
        : public cppual::generic::reply<get_pixel_mapuiv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_pixel_mapuiv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_pixel_mapuiv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_pixel_mapuiv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_pixel_mapuiv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_pixel_mapuiv_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_get_pixel_mapuiv_data),
            SIGNATURE(xcb_glx_get_pixel_mapuiv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_pixel_mapuiv_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_get_pixel_mapuiv_data),
                SIGNATURE(xcb_glx_get_pixel_mapuiv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_pixel_mapuiv

} // namespace detail

namespace checked {
template<typename Connection>
using get_pixel_mapuiv = detail::get_pixel_mapuiv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_pixel_mapuiv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_pixel_mapuiv = detail::get_pixel_mapuiv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_pixel_mapuiv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_pixel_mapuiv<Connection>
get_pixel_mapuiv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_pixel_mapuiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_pixel_mapuiv<Connection>
get_pixel_mapuiv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_pixel_mapuiv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_pixel_mapusv
        : public cppual::generic::reply<get_pixel_mapusv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_pixel_mapusv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_pixel_mapusv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_pixel_mapusv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_pixel_mapusv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_pixel_mapusv_reply_t,
            cppual::generic::iterator<Connection,
            uint16_t,
            SIGNATURE(xcb_glx_get_pixel_mapusv_data),
            SIGNATURE(xcb_glx_get_pixel_mapusv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_pixel_mapusv_reply_t,
                cppual::generic::iterator<Connection,
                uint16_t,
                SIGNATURE(xcb_glx_get_pixel_mapusv_data),
                SIGNATURE(xcb_glx_get_pixel_mapusv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_pixel_mapusv

} // namespace detail

namespace checked {
template<typename Connection>
using get_pixel_mapusv = detail::get_pixel_mapusv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_pixel_mapusv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_pixel_mapusv = detail::get_pixel_mapusv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_pixel_mapusv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_pixel_mapusv<Connection>
get_pixel_mapusv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_pixel_mapusv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_pixel_mapusv<Connection>
get_pixel_mapusv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_pixel_mapusv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_polygon_stipple
        : public cppual::generic::reply<get_polygon_stipple<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_polygon_stipple_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_polygon_stipple<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_polygon_stipple_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_polygon_stipple(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_polygon_stipple_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_polygon_stipple_data),
            SIGNATURE(xcb_glx_get_polygon_stipple_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_polygon_stipple_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_polygon_stipple_data),
                SIGNATURE(xcb_glx_get_polygon_stipple_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_polygon_stipple

} // namespace detail

namespace checked {
template<typename Connection>
using get_polygon_stipple = detail::get_polygon_stipple<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_polygon_stipple)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_polygon_stipple = detail::get_polygon_stipple<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_polygon_stipple_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_polygon_stipple<Connection>
get_polygon_stipple(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_polygon_stipple<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_polygon_stipple<Connection>
get_polygon_stipple_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_polygon_stipple<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_string
        : public cppual::generic::reply<get_string<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_string_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_string<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_string_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_string(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            string(void)
    {
        return std::string(xcb_glx_get_string_string(this->get().get()),
                           ::xcb_glx_get_string_string_length(this->get().get()));
    }

}; // class get_string

} // namespace detail

namespace checked {
template<typename Connection>
using get_string = detail::get_string<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_string)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_string = detail::get_string<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_string_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_string<Connection>
get_string(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_string<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_string<Connection>
get_string_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_string<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_envfv
        : public cppual::generic::reply<get_tex_envfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_envfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_envfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_envfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_envfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_envfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_tex_envfv_data),
            SIGNATURE(xcb_glx_get_tex_envfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_envfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_tex_envfv_data),
                SIGNATURE(xcb_glx_get_tex_envfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_envfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_envfv = detail::get_tex_envfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_envfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_envfv = detail::get_tex_envfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_envfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_envfv<Connection>
get_tex_envfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_envfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_envfv<Connection>
get_tex_envfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_envfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_enviv
        : public cppual::generic::reply<get_tex_enviv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_enviv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_enviv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_enviv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_enviv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_enviv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_tex_enviv_data),
            SIGNATURE(xcb_glx_get_tex_enviv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_enviv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_tex_enviv_data),
                SIGNATURE(xcb_glx_get_tex_enviv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_enviv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_enviv = detail::get_tex_enviv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_enviv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_enviv = detail::get_tex_enviv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_enviv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_enviv<Connection>
get_tex_enviv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_enviv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_enviv<Connection>
get_tex_enviv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_enviv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_gendv
        : public cppual::generic::reply<get_tex_gendv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_gendv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_gendv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_gendv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_gendv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_gendv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float64_t,
            SIGNATURE(xcb_glx_get_tex_gendv_data),
            SIGNATURE(xcb_glx_get_tex_gendv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_gendv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float64_t,
                SIGNATURE(xcb_glx_get_tex_gendv_data),
                SIGNATURE(xcb_glx_get_tex_gendv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_gendv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_gendv = detail::get_tex_gendv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_gendv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_gendv = detail::get_tex_gendv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_gendv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_gendv<Connection>
get_tex_gendv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_gendv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_gendv<Connection>
get_tex_gendv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_gendv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_genfv
        : public cppual::generic::reply<get_tex_genfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_genfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_genfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_genfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_genfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_genfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_tex_genfv_data),
            SIGNATURE(xcb_glx_get_tex_genfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_genfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_tex_genfv_data),
                SIGNATURE(xcb_glx_get_tex_genfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_genfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_genfv = detail::get_tex_genfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_genfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_genfv = detail::get_tex_genfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_genfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_genfv<Connection>
get_tex_genfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_genfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_genfv<Connection>
get_tex_genfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_genfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_geniv
        : public cppual::generic::reply<get_tex_geniv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_geniv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_geniv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_geniv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_geniv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_geniv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_tex_geniv_data),
            SIGNATURE(xcb_glx_get_tex_geniv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_geniv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_tex_geniv_data),
                SIGNATURE(xcb_glx_get_tex_geniv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_geniv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_geniv = detail::get_tex_geniv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_geniv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_geniv = detail::get_tex_geniv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_geniv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_geniv<Connection>
get_tex_geniv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_geniv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_geniv<Connection>
get_tex_geniv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_geniv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_image
        : public cppual::generic::reply<get_tex_image<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_image_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_image<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_image_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_image(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_image_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_tex_image_data),
            SIGNATURE(xcb_glx_get_tex_image_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_image_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_tex_image_data),
                SIGNATURE(xcb_glx_get_tex_image_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_image

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_image = detail::get_tex_image<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_image)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_image = detail::get_tex_image<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_image_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_image<Connection>
get_tex_image(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_image<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_image<Connection>
get_tex_image_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_image<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_parameterfv
        : public cppual::generic::reply<get_tex_parameterfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_parameterfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_parameterfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_parameterfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_parameterfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_parameterfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_tex_parameterfv_data),
            SIGNATURE(xcb_glx_get_tex_parameterfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_parameterfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_tex_parameterfv_data),
                SIGNATURE(xcb_glx_get_tex_parameterfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_parameterfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_parameterfv = detail::get_tex_parameterfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_parameterfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_parameterfv = detail::get_tex_parameterfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_parameterfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_parameterfv<Connection>
get_tex_parameterfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_parameterfv<Connection>
get_tex_parameterfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_parameteriv
        : public cppual::generic::reply<get_tex_parameteriv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_parameteriv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_parameteriv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_parameteriv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_parameteriv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_parameteriv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_tex_parameteriv_data),
            SIGNATURE(xcb_glx_get_tex_parameteriv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_parameteriv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_tex_parameteriv_data),
                SIGNATURE(xcb_glx_get_tex_parameteriv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_parameteriv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_parameteriv = detail::get_tex_parameteriv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_parameteriv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_parameteriv = detail::get_tex_parameteriv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_parameteriv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_parameteriv<Connection>
get_tex_parameteriv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_parameteriv<Connection>
get_tex_parameteriv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_level_parameterfv
        : public cppual::generic::reply<get_tex_level_parameterfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_level_parameterfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_level_parameterfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_level_parameterfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_level_parameterfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_level_parameterfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_tex_level_parameterfv_data),
            SIGNATURE(xcb_glx_get_tex_level_parameterfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_level_parameterfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_tex_level_parameterfv_data),
                SIGNATURE(xcb_glx_get_tex_level_parameterfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_level_parameterfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_level_parameterfv = detail::get_tex_level_parameterfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_level_parameterfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_level_parameterfv = detail::get_tex_level_parameterfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_level_parameterfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_level_parameterfv<Connection>
get_tex_level_parameterfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_level_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_level_parameterfv<Connection>
get_tex_level_parameterfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_level_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_tex_level_parameteriv
        : public cppual::generic::reply<get_tex_level_parameteriv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_tex_level_parameteriv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_tex_level_parameteriv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_tex_level_parameteriv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_tex_level_parameteriv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_tex_level_parameteriv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_tex_level_parameteriv_data),
            SIGNATURE(xcb_glx_get_tex_level_parameteriv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_tex_level_parameteriv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_tex_level_parameteriv_data),
                SIGNATURE(xcb_glx_get_tex_level_parameteriv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_tex_level_parameteriv

} // namespace detail

namespace checked {
template<typename Connection>
using get_tex_level_parameteriv = detail::get_tex_level_parameteriv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_tex_level_parameteriv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_tex_level_parameteriv = detail::get_tex_level_parameteriv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_tex_level_parameteriv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_tex_level_parameteriv<Connection>
get_tex_level_parameteriv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_tex_level_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_tex_level_parameteriv<Connection>
get_tex_level_parameteriv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_tex_level_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class is_enabled
        : public cppual::generic::reply<is_enabled<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_is_enabled_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<is_enabled<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_is_enabled_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    is_enabled(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class is_enabled

} // namespace detail

namespace checked {
template<typename Connection>
using is_enabled = detail::is_enabled<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_is_enabled)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using is_enabled = detail::is_enabled<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_is_enabled_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::is_enabled<Connection>
is_enabled(Connection && c, Parameter && ... parameter)
{
    return reply::checked::is_enabled<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::is_enabled<Connection>
is_enabled_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::is_enabled<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class is_list
        : public cppual::generic::reply<is_list<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_is_list_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<is_list<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_is_list_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    is_list(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class is_list

} // namespace detail

namespace checked {
template<typename Connection>
using is_list = detail::is_list<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_is_list)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using is_list = detail::is_list<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_is_list_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::is_list<Connection>
is_list(Connection && c, Parameter && ... parameter)
{
    return reply::checked::is_list<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::is_list<Connection>
is_list_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::is_list<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
flush_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_flush_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
flush(Parameter && ... parameter)
{
    ::xcb_glx_flush(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class are_textures_resident
        : public cppual::generic::reply<are_textures_resident<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_are_textures_resident_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<are_textures_resident<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_are_textures_resident_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    are_textures_resident(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_are_textures_resident_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_are_textures_resident_data),
            SIGNATURE(xcb_glx_are_textures_resident_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_are_textures_resident_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_are_textures_resident_data),
                SIGNATURE(xcb_glx_are_textures_resident_data_length)>
                >(this->m_c, this->get());
    }
}; // class are_textures_resident

} // namespace detail

namespace checked {
template<typename Connection>
using are_textures_resident = detail::are_textures_resident<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_are_textures_resident)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using are_textures_resident = detail::are_textures_resident<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_are_textures_resident_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::are_textures_resident<Connection>
are_textures_resident(Connection && c, Parameter && ... parameter)
{
    return reply::checked::are_textures_resident<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::are_textures_resident<Connection>
are_textures_resident_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::are_textures_resident<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_textures_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_delete_textures_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_textures(Parameter && ... parameter)
{
    ::xcb_glx_delete_textures(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class gen_textures
        : public cppual::generic::reply<gen_textures<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_gen_textures_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<gen_textures<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_gen_textures_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    gen_textures(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_gen_textures_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_gen_textures_data),
            SIGNATURE(xcb_glx_gen_textures_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_gen_textures_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_gen_textures_data),
                SIGNATURE(xcb_glx_gen_textures_data_length)>
                >(this->m_c, this->get());
    }
}; // class gen_textures

} // namespace detail

namespace checked {
template<typename Connection>
using gen_textures = detail::gen_textures<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_gen_textures)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using gen_textures = detail::gen_textures<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_gen_textures_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::gen_textures<Connection>
gen_textures(Connection && c, Parameter && ... parameter)
{
    return reply::checked::gen_textures<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::gen_textures<Connection>
gen_textures_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::gen_textures<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class is_texture
        : public cppual::generic::reply<is_texture<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_is_texture_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<is_texture<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_is_texture_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    is_texture(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class is_texture

} // namespace detail

namespace checked {
template<typename Connection>
using is_texture = detail::is_texture<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_is_texture)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using is_texture = detail::is_texture<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_is_texture_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::is_texture<Connection>
is_texture(Connection && c, Parameter && ... parameter)
{
    return reply::checked::is_texture<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::is_texture<Connection>
is_texture_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::is_texture<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_color_table
        : public cppual::generic::reply<get_color_table<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_color_table_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_color_table<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_color_table_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_color_table(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_color_table_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_color_table_data),
            SIGNATURE(xcb_glx_get_color_table_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_color_table_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_color_table_data),
                SIGNATURE(xcb_glx_get_color_table_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_color_table

} // namespace detail

namespace checked {
template<typename Connection>
using get_color_table = detail::get_color_table<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_color_table)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_color_table = detail::get_color_table<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_color_table_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_color_table<Connection>
get_color_table(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_color_table<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_color_table<Connection>
get_color_table_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_color_table<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_color_table_parameterfv
        : public cppual::generic::reply<get_color_table_parameterfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_color_table_parameterfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_color_table_parameterfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_color_table_parameterfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_color_table_parameterfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_color_table_parameterfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_color_table_parameterfv_data),
            SIGNATURE(xcb_glx_get_color_table_parameterfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_color_table_parameterfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_color_table_parameterfv_data),
                SIGNATURE(xcb_glx_get_color_table_parameterfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_color_table_parameterfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_color_table_parameterfv = detail::get_color_table_parameterfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_color_table_parameterfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_color_table_parameterfv = detail::get_color_table_parameterfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_color_table_parameterfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_color_table_parameterfv<Connection>
get_color_table_parameterfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_color_table_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_color_table_parameterfv<Connection>
get_color_table_parameterfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_color_table_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_color_table_parameteriv
        : public cppual::generic::reply<get_color_table_parameteriv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_color_table_parameteriv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_color_table_parameteriv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_color_table_parameteriv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_color_table_parameteriv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_color_table_parameteriv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_color_table_parameteriv_data),
            SIGNATURE(xcb_glx_get_color_table_parameteriv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_color_table_parameteriv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_color_table_parameteriv_data),
                SIGNATURE(xcb_glx_get_color_table_parameteriv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_color_table_parameteriv

} // namespace detail

namespace checked {
template<typename Connection>
using get_color_table_parameteriv = detail::get_color_table_parameteriv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_color_table_parameteriv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_color_table_parameteriv = detail::get_color_table_parameteriv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_color_table_parameteriv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_color_table_parameteriv<Connection>
get_color_table_parameteriv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_color_table_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_color_table_parameteriv<Connection>
get_color_table_parameteriv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_color_table_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_convolution_filter
        : public cppual::generic::reply<get_convolution_filter<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_convolution_filter_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_convolution_filter<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_convolution_filter_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_convolution_filter(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_convolution_filter_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_convolution_filter_data),
            SIGNATURE(xcb_glx_get_convolution_filter_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_convolution_filter_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_convolution_filter_data),
                SIGNATURE(xcb_glx_get_convolution_filter_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_convolution_filter

} // namespace detail

namespace checked {
template<typename Connection>
using get_convolution_filter = detail::get_convolution_filter<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_convolution_filter)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_convolution_filter = detail::get_convolution_filter<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_convolution_filter_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_convolution_filter<Connection>
get_convolution_filter(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_convolution_filter<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_convolution_filter<Connection>
get_convolution_filter_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_convolution_filter<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_convolution_parameterfv
        : public cppual::generic::reply<get_convolution_parameterfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_convolution_parameterfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_convolution_parameterfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_convolution_parameterfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_convolution_parameterfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_convolution_parameterfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_convolution_parameterfv_data),
            SIGNATURE(xcb_glx_get_convolution_parameterfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_convolution_parameterfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_convolution_parameterfv_data),
                SIGNATURE(xcb_glx_get_convolution_parameterfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_convolution_parameterfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_convolution_parameterfv = detail::get_convolution_parameterfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_convolution_parameterfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_convolution_parameterfv = detail::get_convolution_parameterfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_convolution_parameterfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_convolution_parameterfv<Connection>
get_convolution_parameterfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_convolution_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_convolution_parameterfv<Connection>
get_convolution_parameterfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_convolution_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_convolution_parameteriv
        : public cppual::generic::reply<get_convolution_parameteriv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_convolution_parameteriv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_convolution_parameteriv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_convolution_parameteriv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_convolution_parameteriv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_convolution_parameteriv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_convolution_parameteriv_data),
            SIGNATURE(xcb_glx_get_convolution_parameteriv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_convolution_parameteriv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_convolution_parameteriv_data),
                SIGNATURE(xcb_glx_get_convolution_parameteriv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_convolution_parameteriv

} // namespace detail

namespace checked {
template<typename Connection>
using get_convolution_parameteriv = detail::get_convolution_parameteriv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_convolution_parameteriv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_convolution_parameteriv = detail::get_convolution_parameteriv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_convolution_parameteriv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_convolution_parameteriv<Connection>
get_convolution_parameteriv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_convolution_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_convolution_parameteriv<Connection>
get_convolution_parameteriv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_convolution_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_separable_filter
        : public cppual::generic::reply<get_separable_filter<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_separable_filter_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_separable_filter<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_separable_filter_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_separable_filter(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_separable_filter_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_separable_filter_rows_and_cols),
            SIGNATURE(xcb_glx_get_separable_filter_rows_and_cols_length)>
            >
            rows_and_cols(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_separable_filter_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_separable_filter_rows_and_cols),
                SIGNATURE(xcb_glx_get_separable_filter_rows_and_cols_length)>
                >(this->m_c, this->get());
    }
}; // class get_separable_filter

} // namespace detail

namespace checked {
template<typename Connection>
using get_separable_filter = detail::get_separable_filter<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_separable_filter)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_separable_filter = detail::get_separable_filter<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_separable_filter_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_separable_filter<Connection>
get_separable_filter(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_separable_filter<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_separable_filter<Connection>
get_separable_filter_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_separable_filter<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_histogram
        : public cppual::generic::reply<get_histogram<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_histogram_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_histogram<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_histogram_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_histogram(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_histogram_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_histogram_data),
            SIGNATURE(xcb_glx_get_histogram_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_histogram_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_histogram_data),
                SIGNATURE(xcb_glx_get_histogram_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_histogram

} // namespace detail

namespace checked {
template<typename Connection>
using get_histogram = detail::get_histogram<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_histogram)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_histogram = detail::get_histogram<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_histogram_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_histogram<Connection>
get_histogram(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_histogram<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_histogram<Connection>
get_histogram_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_histogram<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_histogram_parameterfv
        : public cppual::generic::reply<get_histogram_parameterfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_histogram_parameterfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_histogram_parameterfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_histogram_parameterfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_histogram_parameterfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_histogram_parameterfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_histogram_parameterfv_data),
            SIGNATURE(xcb_glx_get_histogram_parameterfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_histogram_parameterfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_histogram_parameterfv_data),
                SIGNATURE(xcb_glx_get_histogram_parameterfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_histogram_parameterfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_histogram_parameterfv = detail::get_histogram_parameterfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_histogram_parameterfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_histogram_parameterfv = detail::get_histogram_parameterfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_histogram_parameterfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_histogram_parameterfv<Connection>
get_histogram_parameterfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_histogram_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_histogram_parameterfv<Connection>
get_histogram_parameterfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_histogram_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_histogram_parameteriv
        : public cppual::generic::reply<get_histogram_parameteriv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_histogram_parameteriv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_histogram_parameteriv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_histogram_parameteriv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_histogram_parameteriv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_histogram_parameteriv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_histogram_parameteriv_data),
            SIGNATURE(xcb_glx_get_histogram_parameteriv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_histogram_parameteriv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_histogram_parameteriv_data),
                SIGNATURE(xcb_glx_get_histogram_parameteriv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_histogram_parameteriv

} // namespace detail

namespace checked {
template<typename Connection>
using get_histogram_parameteriv = detail::get_histogram_parameteriv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_histogram_parameteriv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_histogram_parameteriv = detail::get_histogram_parameteriv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_histogram_parameteriv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_histogram_parameteriv<Connection>
get_histogram_parameteriv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_histogram_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_histogram_parameteriv<Connection>
get_histogram_parameteriv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_histogram_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_minmax
        : public cppual::generic::reply<get_minmax<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_minmax_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_minmax<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_minmax_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_minmax(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_minmax_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_minmax_data),
            SIGNATURE(xcb_glx_get_minmax_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_minmax_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_minmax_data),
                SIGNATURE(xcb_glx_get_minmax_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_minmax

} // namespace detail

namespace checked {
template<typename Connection>
using get_minmax = detail::get_minmax<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_minmax)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_minmax = detail::get_minmax<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_minmax_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_minmax<Connection>
get_minmax(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_minmax<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_minmax<Connection>
get_minmax_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_minmax<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_minmax_parameterfv
        : public cppual::generic::reply<get_minmax_parameterfv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_minmax_parameterfv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_minmax_parameterfv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_minmax_parameterfv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_minmax_parameterfv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_minmax_parameterfv_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_glx_float32_t,
            SIGNATURE(xcb_glx_get_minmax_parameterfv_data),
            SIGNATURE(xcb_glx_get_minmax_parameterfv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_minmax_parameterfv_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_glx_float32_t,
                SIGNATURE(xcb_glx_get_minmax_parameterfv_data),
                SIGNATURE(xcb_glx_get_minmax_parameterfv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_minmax_parameterfv

} // namespace detail

namespace checked {
template<typename Connection>
using get_minmax_parameterfv = detail::get_minmax_parameterfv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_minmax_parameterfv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_minmax_parameterfv = detail::get_minmax_parameterfv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_minmax_parameterfv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_minmax_parameterfv<Connection>
get_minmax_parameterfv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_minmax_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_minmax_parameterfv<Connection>
get_minmax_parameterfv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_minmax_parameterfv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_minmax_parameteriv
        : public cppual::generic::reply<get_minmax_parameteriv<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_minmax_parameteriv_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_minmax_parameteriv<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_minmax_parameteriv_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_minmax_parameteriv(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_minmax_parameteriv_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_minmax_parameteriv_data),
            SIGNATURE(xcb_glx_get_minmax_parameteriv_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_minmax_parameteriv_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_minmax_parameteriv_data),
                SIGNATURE(xcb_glx_get_minmax_parameteriv_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_minmax_parameteriv

} // namespace detail

namespace checked {
template<typename Connection>
using get_minmax_parameteriv = detail::get_minmax_parameteriv<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_minmax_parameteriv)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_minmax_parameteriv = detail::get_minmax_parameteriv<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_minmax_parameteriv_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_minmax_parameteriv<Connection>
get_minmax_parameteriv(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_minmax_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_minmax_parameteriv<Connection>
get_minmax_parameteriv_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_minmax_parameteriv<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_compressed_tex_image_arb
        : public cppual::generic::reply<get_compressed_tex_image_arb<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_compressed_tex_image_arb_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_compressed_tex_image_arb<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_compressed_tex_image_arb_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_compressed_tex_image_arb(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_compressed_tex_image_arb_reply_t,
            cppual::generic::iterator<Connection,
            uint8_t,
            SIGNATURE(xcb_glx_get_compressed_tex_image_arb_data),
            SIGNATURE(xcb_glx_get_compressed_tex_image_arb_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_compressed_tex_image_arb_reply_t,
                cppual::generic::iterator<Connection,
                uint8_t,
                SIGNATURE(xcb_glx_get_compressed_tex_image_arb_data),
                SIGNATURE(xcb_glx_get_compressed_tex_image_arb_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_compressed_tex_image_arb

} // namespace detail

namespace checked {
template<typename Connection>
using get_compressed_tex_image_arb = detail::get_compressed_tex_image_arb<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_compressed_tex_image_arb)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_compressed_tex_image_arb = detail::get_compressed_tex_image_arb<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_compressed_tex_image_arb_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_compressed_tex_image_arb<Connection>
get_compressed_tex_image_arb(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_compressed_tex_image_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_compressed_tex_image_arb<Connection>
get_compressed_tex_image_arb_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_compressed_tex_image_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_queries_arb_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::glx::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_glx_delete_queries_arb_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_queries_arb(Parameter && ... parameter)
{
    ::xcb_glx_delete_queries_arb(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class gen_queries_arb
        : public cppual::generic::reply<gen_queries_arb<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_gen_queries_arb_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<gen_queries_arb<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_gen_queries_arb_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    gen_queries_arb(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_gen_queries_arb_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_gen_queries_arb_data),
            SIGNATURE(xcb_glx_gen_queries_arb_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_gen_queries_arb_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_gen_queries_arb_data),
                SIGNATURE(xcb_glx_gen_queries_arb_data_length)>
                >(this->m_c, this->get());
    }
}; // class gen_queries_arb

} // namespace detail

namespace checked {
template<typename Connection>
using gen_queries_arb = detail::gen_queries_arb<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_gen_queries_arb)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using gen_queries_arb = detail::gen_queries_arb<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_gen_queries_arb_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::gen_queries_arb<Connection>
gen_queries_arb(Connection && c, Parameter && ... parameter)
{
    return reply::checked::gen_queries_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::gen_queries_arb<Connection>
gen_queries_arb_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::gen_queries_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class is_query_arb
        : public cppual::generic::reply<is_query_arb<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_is_query_arb_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<is_query_arb<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_is_query_arb_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    is_query_arb(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class is_query_arb

} // namespace detail

namespace checked {
template<typename Connection>
using is_query_arb = detail::is_query_arb<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_is_query_arb)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using is_query_arb = detail::is_query_arb<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_is_query_arb_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::is_query_arb<Connection>
is_query_arb(Connection && c, Parameter && ... parameter)
{
    return reply::checked::is_query_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::is_query_arb<Connection>
is_query_arb_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::is_query_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_queryiv_arb
        : public cppual::generic::reply<get_queryiv_arb<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_queryiv_arb_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_queryiv_arb<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_queryiv_arb_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_queryiv_arb(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_queryiv_arb_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_queryiv_arb_data),
            SIGNATURE(xcb_glx_get_queryiv_arb_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_queryiv_arb_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_queryiv_arb_data),
                SIGNATURE(xcb_glx_get_queryiv_arb_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_queryiv_arb

} // namespace detail

namespace checked {
template<typename Connection>
using get_queryiv_arb = detail::get_queryiv_arb<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_queryiv_arb)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_queryiv_arb = detail::get_queryiv_arb<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_queryiv_arb_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_queryiv_arb<Connection>
get_queryiv_arb(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_queryiv_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_queryiv_arb<Connection>
get_queryiv_arb_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_queryiv_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_query_objectiv_arb
        : public cppual::generic::reply<get_query_objectiv_arb<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_query_objectiv_arb_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_query_objectiv_arb<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_query_objectiv_arb_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_query_objectiv_arb(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_query_objectiv_arb_reply_t,
            cppual::generic::iterator<Connection,
            int32_t,
            SIGNATURE(xcb_glx_get_query_objectiv_arb_data),
            SIGNATURE(xcb_glx_get_query_objectiv_arb_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_query_objectiv_arb_reply_t,
                cppual::generic::iterator<Connection,
                int32_t,
                SIGNATURE(xcb_glx_get_query_objectiv_arb_data),
                SIGNATURE(xcb_glx_get_query_objectiv_arb_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_query_objectiv_arb

} // namespace detail

namespace checked {
template<typename Connection>
using get_query_objectiv_arb = detail::get_query_objectiv_arb<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_query_objectiv_arb)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_query_objectiv_arb = detail::get_query_objectiv_arb<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_query_objectiv_arb_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_query_objectiv_arb<Connection>
get_query_objectiv_arb(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_query_objectiv_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_query_objectiv_arb<Connection>
get_query_objectiv_arb_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_query_objectiv_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_query_objectuiv_arb
        : public cppual::generic::reply<get_query_objectuiv_arb<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_glx_get_query_objectuiv_arb_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_query_objectuiv_arb<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_glx_get_query_objectuiv_arb_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_query_objectuiv_arb(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_glx_get_query_objectuiv_arb_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_glx_get_query_objectuiv_arb_data),
            SIGNATURE(xcb_glx_get_query_objectuiv_arb_data_length)>
            >
            data(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_glx_get_query_objectuiv_arb_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_glx_get_query_objectuiv_arb_data),
                SIGNATURE(xcb_glx_get_query_objectuiv_arb_data_length)>
                >(this->m_c, this->get());
    }
}; // class get_query_objectuiv_arb

} // namespace detail

namespace checked {
template<typename Connection>
using get_query_objectuiv_arb = detail::get_query_objectuiv_arb<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_glx_get_query_objectuiv_arb)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_query_objectuiv_arb = detail::get_query_objectuiv_arb<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_glx_get_query_objectuiv_arb_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_query_objectuiv_arb<Connection>
get_query_objectuiv_arb(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_query_objectuiv_arb<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_query_objectuiv_arb<Connection>
get_query_objectuiv_arb_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_query_objectuiv_arb<Connection>(
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

    const ::xcb_glx_context_t &
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
        cppual::glx::create_context_checked(connection(),
                                         resource(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
        cppual::glx::create_context(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_context_checked(connection(),
                                          resource(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::glx::destroy_context(connection(),
                                  resource(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    is_direct(Parameter && ... parameter) const
    -> reply::checked::is_direct<Connection>
    {
        return cppual::glx::is_direct(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    is_direct_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::is_direct<Connection>
    {
        return cppual::glx::is_direct_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    copy_checked(Parameter && ... parameter) const
    {
        cppual::glx::copy_context_checked(connection(),
                                       resource(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    copy(Parameter && ... parameter) const
    {
        cppual::glx::copy_context(connection(),
                               resource(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_new_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_new_context_checked(connection(),
                                             resource(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_new(Parameter && ... parameter) const
    {
        cppual::glx::create_new_context(connection(),
                                     resource(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query(Parameter && ... parameter) const
    -> reply::checked::query_context<Connection>
    {
        return cppual::glx::query_context(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_context<Connection>
    {
        return cppual::glx::query_context_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_attribs_arb_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_context_attribs_arb_checked(connection(),
                                                     resource(),
                                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_attribs_arb(Parameter && ... parameter) const
    {
        cppual::glx::create_context_attribs_arb(connection(),
                                             resource(),
                                             std::forward<Parameter>(parameter) ...);
    }



}; // class context


template<typename Derived, typename Connection>
class pbuffer
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_glx_pbuffer_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~pbuffer(void) {}

    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pbuffer_checked(connection(),
                                          resource(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pbuffer(connection(),
                                  resource(),
                                  std::forward<Parameter>(parameter) ...);
    }



}; // class pbuffer










template<typename Derived, typename Connection>
class drawable
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_glx_drawable_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~drawable(void) {}

    template<typename ... Parameter>
    auto
    make_current(Parameter && ... parameter) const
    -> reply::checked::make_current<Connection>
    {
        return cppual::glx::make_current(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    make_current_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::make_current<Connection>
    {
        return cppual::glx::make_current_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_attributes(Parameter && ... parameter) const
    -> reply::checked::get_drawable_attributes<Connection>
    {
        return cppual::glx::get_drawable_attributes(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_attributes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_drawable_attributes<Connection>
    {
        return cppual::glx::get_drawable_attributes_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_attributes_checked(Parameter && ... parameter) const
    {
        cppual::glx::change_drawable_attributes_checked(connection(),
                                                     resource(),
                                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_attributes(Parameter && ... parameter) const
    {
        cppual::glx::change_drawable_attributes(connection(),
                                             resource(),
                                             std::forward<Parameter>(parameter) ...);
    }



}; // class drawable



template<typename Derived, typename Connection>
class pixmap
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_glx_pixmap_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~pixmap(void) {}

    template<typename ... Parameter>
    void
    destroy_glx_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_glx_pixmap_checked(connection(),
                                             resource(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_glx(Parameter && ... parameter) const
    {
        cppual::glx::destroy_glx_pixmap(connection(),
                                     resource(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pixmap_checked(connection(),
                                         resource(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pixmap(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }



}; // class pixmap







template<typename Derived, typename Connection>
class window
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_glx_window_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~window(void) {}

    template<typename ... Parameter>
    void
    delete_checked(Parameter && ... parameter) const
    {
        cppual::glx::delete_window_checked(connection(),
                                        resource(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete(Parameter && ... parameter) const
    {
        cppual::glx::delete_window(connection(),
                                resource(),
                                std::forward<Parameter>(parameter) ...);
    }



}; // class window





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
    typedef cppual::glx::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    glx(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    void
    render_checked(Parameter && ... parameter) const
    {
        cppual::glx::render_checked(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    render(Parameter && ... parameter) const
    {
        cppual::glx::render(connection(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    render_large_checked(Parameter && ... parameter) const
    {
        cppual::glx::render_large_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    render_large(Parameter && ... parameter) const
    {
        cppual::glx::render_large(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_context_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_context_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_context(Parameter && ... parameter) const
    {
        cppual::glx::create_context(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_context_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_context_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_context(Parameter && ... parameter) const
    {
        cppual::glx::destroy_context(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    make_current(Parameter && ... parameter) const
    -> reply::checked::make_current<Connection>
    {
        return cppual::glx::make_current(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    make_current_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::make_current<Connection>
    {
        return cppual::glx::make_current_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    is_direct(Parameter && ... parameter) const
    -> reply::checked::is_direct<Connection>
    {
        return cppual::glx::is_direct(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    is_direct_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::is_direct<Connection>
    {
        return cppual::glx::is_direct_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::glx::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::glx::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    wait_gl_checked(Parameter && ... parameter) const
    {
        cppual::glx::wait_gl_checked(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    wait_gl(Parameter && ... parameter) const
    {
        cppual::glx::wait_gl(connection(),
                          std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    wait_x_checked(Parameter && ... parameter) const
    {
        cppual::glx::wait_x_checked(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    wait_x(Parameter && ... parameter) const
    {
        cppual::glx::wait_x(connection(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    copy_context_checked(Parameter && ... parameter) const
    {
        cppual::glx::copy_context_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    copy_context(Parameter && ... parameter) const
    {
        cppual::glx::copy_context(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    swap_buffers_checked(Parameter && ... parameter) const
    {
        cppual::glx::swap_buffers_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    swap_buffers(Parameter && ... parameter) const
    {
        cppual::glx::swap_buffers(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    use_x_font_checked(Parameter && ... parameter) const
    {
        cppual::glx::use_x_font_checked(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    use_x_font(Parameter && ... parameter) const
    {
        cppual::glx::use_x_font(connection(),
                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_glx_pixmap_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_glx_pixmap_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_glx_pixmap(Parameter && ... parameter) const
    {
        cppual::glx::create_glx_pixmap(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_visual_configs(Parameter && ... parameter) const
    -> reply::checked::get_visual_configs<Connection>
    {
        return cppual::glx::get_visual_configs(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_visual_configs_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_visual_configs<Connection>
    {
        return cppual::glx::get_visual_configs_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_glx_pixmap_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_glx_pixmap_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_glx_pixmap(Parameter && ... parameter) const
    {
        cppual::glx::destroy_glx_pixmap(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    vendor_private_checked(Parameter && ... parameter) const
    {
        cppual::glx::vendor_private_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    vendor_private(Parameter && ... parameter) const
    {
        cppual::glx::vendor_private(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    vendor_private_with_reply(Parameter && ... parameter) const
    -> reply::checked::vendor_private_with_reply<Connection>
    {
        return cppual::glx::vendor_private_with_reply(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    vendor_private_with_reply_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::vendor_private_with_reply<Connection>
    {
        return cppual::glx::vendor_private_with_reply_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_extensions_string(Parameter && ... parameter) const
    -> reply::checked::query_extensions_string<Connection>
    {
        return cppual::glx::query_extensions_string(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_extensions_string_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_extensions_string<Connection>
    {
        return cppual::glx::query_extensions_string_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_server_string(Parameter && ... parameter) const
    -> reply::checked::query_server_string<Connection>
    {
        return cppual::glx::query_server_string(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_server_string_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_server_string<Connection>
    {
        return cppual::glx::query_server_string_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    client_info_checked(Parameter && ... parameter) const
    {
        cppual::glx::client_info_checked(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    client_info(Parameter && ... parameter) const
    {
        cppual::glx::client_info(connection(),
                              std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_fb_configs(Parameter && ... parameter) const
    -> reply::checked::get_fb_configs<Connection>
    {
        return cppual::glx::get_fb_configs(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_fb_configs_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_fb_configs<Connection>
    {
        return cppual::glx::get_fb_configs_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_pixmap_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_pixmap_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_pixmap(Parameter && ... parameter) const
    {
        cppual::glx::create_pixmap(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_pixmap_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pixmap_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_pixmap(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pixmap(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_new_context_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_new_context_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_new_context(Parameter && ... parameter) const
    {
        cppual::glx::create_new_context(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_context(Parameter && ... parameter) const
    -> reply::checked::query_context<Connection>
    {
        return cppual::glx::query_context(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_context_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_context<Connection>
    {
        return cppual::glx::query_context_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    make_context_current(Parameter && ... parameter) const
    -> reply::checked::make_context_current<Connection>
    {
        return cppual::glx::make_context_current(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    make_context_current_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::make_context_current<Connection>
    {
        return cppual::glx::make_context_current_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_pbuffer_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_pbuffer_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_pbuffer(Parameter && ... parameter) const
    {
        cppual::glx::create_pbuffer(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_pbuffer_checked(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pbuffer_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_pbuffer(Parameter && ... parameter) const
    {
        cppual::glx::destroy_pbuffer(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_drawable_attributes(Parameter && ... parameter) const
    -> reply::checked::get_drawable_attributes<Connection>
    {
        return cppual::glx::get_drawable_attributes(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_drawable_attributes_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_drawable_attributes<Connection>
    {
        return cppual::glx::get_drawable_attributes_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_drawable_attributes_checked(Parameter && ... parameter) const
    {
        cppual::glx::change_drawable_attributes_checked(connection(),
                                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_drawable_attributes(Parameter && ... parameter) const
    {
        cppual::glx::change_drawable_attributes(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_window_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_window_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_window(Parameter && ... parameter) const
    {
        cppual::glx::create_window(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_window_checked(Parameter && ... parameter) const
    {
        cppual::glx::delete_window_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_window(Parameter && ... parameter) const
    {
        cppual::glx::delete_window(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_client_info_arb_checked(Parameter && ... parameter) const
    {
        cppual::glx::set_client_info_arb_checked(connection(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_client_info_arb(Parameter && ... parameter) const
    {
        cppual::glx::set_client_info_arb(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_context_attribs_arb_checked(Parameter && ... parameter) const
    {
        cppual::glx::create_context_attribs_arb_checked(connection(),
                                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_context_attribs_arb(Parameter && ... parameter) const
    {
        cppual::glx::create_context_attribs_arb(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_client_info_2arb_checked(Parameter && ... parameter) const
    {
        cppual::glx::set_client_info_2arb_checked(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_client_info_2arb(Parameter && ... parameter) const
    {
        cppual::glx::set_client_info_2arb(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    new_list_checked(Parameter && ... parameter) const
    {
        cppual::glx::new_list_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    new_list(Parameter && ... parameter) const
    {
        cppual::glx::new_list(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    end_list_checked(Parameter && ... parameter) const
    {
        cppual::glx::end_list_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    end_list(Parameter && ... parameter) const
    {
        cppual::glx::end_list(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_lists_checked(Parameter && ... parameter) const
    {
        cppual::glx::delete_lists_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_lists(Parameter && ... parameter) const
    {
        cppual::glx::delete_lists(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    gen_lists(Parameter && ... parameter) const
    -> reply::checked::gen_lists<Connection>
    {
        return cppual::glx::gen_lists(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    gen_lists_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::gen_lists<Connection>
    {
        return cppual::glx::gen_lists_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    feedback_buffer_checked(Parameter && ... parameter) const
    {
        cppual::glx::feedback_buffer_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    feedback_buffer(Parameter && ... parameter) const
    {
        cppual::glx::feedback_buffer(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_buffer_checked(Parameter && ... parameter) const
    {
        cppual::glx::select_buffer_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_buffer(Parameter && ... parameter) const
    {
        cppual::glx::select_buffer(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    render_mode(Parameter && ... parameter) const
    -> reply::checked::render_mode<Connection>
    {
        return cppual::glx::render_mode(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    render_mode_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::render_mode<Connection>
    {
        return cppual::glx::render_mode_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    finish(Parameter && ... parameter) const
    -> reply::checked::finish<Connection>
    {
        return cppual::glx::finish(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    finish_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::finish<Connection>
    {
        return cppual::glx::finish_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    pixel_storef_checked(Parameter && ... parameter) const
    {
        cppual::glx::pixel_storef_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    pixel_storef(Parameter && ... parameter) const
    {
        cppual::glx::pixel_storef(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    pixel_storei_checked(Parameter && ... parameter) const
    {
        cppual::glx::pixel_storei_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    pixel_storei(Parameter && ... parameter) const
    {
        cppual::glx::pixel_storei(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    read_pixels(Parameter && ... parameter) const
    -> reply::checked::read_pixels<Connection>
    {
        return cppual::glx::read_pixels(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    read_pixels_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::read_pixels<Connection>
    {
        return cppual::glx::read_pixels_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_booleanv(Parameter && ... parameter) const
    -> reply::checked::get_booleanv<Connection>
    {
        return cppual::glx::get_booleanv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_booleanv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_booleanv<Connection>
    {
        return cppual::glx::get_booleanv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_clip_plane(Parameter && ... parameter) const
    -> reply::checked::get_clip_plane<Connection>
    {
        return cppual::glx::get_clip_plane(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_clip_plane_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_clip_plane<Connection>
    {
        return cppual::glx::get_clip_plane_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_doublev(Parameter && ... parameter) const
    -> reply::checked::get_doublev<Connection>
    {
        return cppual::glx::get_doublev(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_doublev_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_doublev<Connection>
    {
        return cppual::glx::get_doublev_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_error(Parameter && ... parameter) const
    -> reply::checked::get_error<Connection>
    {
        return cppual::glx::get_error(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_error_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_error<Connection>
    {
        return cppual::glx::get_error_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_floatv(Parameter && ... parameter) const
    -> reply::checked::get_floatv<Connection>
    {
        return cppual::glx::get_floatv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_floatv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_floatv<Connection>
    {
        return cppual::glx::get_floatv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_integerv(Parameter && ... parameter) const
    -> reply::checked::get_integerv<Connection>
    {
        return cppual::glx::get_integerv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_integerv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_integerv<Connection>
    {
        return cppual::glx::get_integerv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_lightfv(Parameter && ... parameter) const
    -> reply::checked::get_lightfv<Connection>
    {
        return cppual::glx::get_lightfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_lightfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_lightfv<Connection>
    {
        return cppual::glx::get_lightfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_lightiv(Parameter && ... parameter) const
    -> reply::checked::get_lightiv<Connection>
    {
        return cppual::glx::get_lightiv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_lightiv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_lightiv<Connection>
    {
        return cppual::glx::get_lightiv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_mapdv(Parameter && ... parameter) const
    -> reply::checked::get_mapdv<Connection>
    {
        return cppual::glx::get_mapdv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_mapdv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_mapdv<Connection>
    {
        return cppual::glx::get_mapdv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_mapfv(Parameter && ... parameter) const
    -> reply::checked::get_mapfv<Connection>
    {
        return cppual::glx::get_mapfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_mapfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_mapfv<Connection>
    {
        return cppual::glx::get_mapfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_mapiv(Parameter && ... parameter) const
    -> reply::checked::get_mapiv<Connection>
    {
        return cppual::glx::get_mapiv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_mapiv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_mapiv<Connection>
    {
        return cppual::glx::get_mapiv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_materialfv(Parameter && ... parameter) const
    -> reply::checked::get_materialfv<Connection>
    {
        return cppual::glx::get_materialfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_materialfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_materialfv<Connection>
    {
        return cppual::glx::get_materialfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_materialiv(Parameter && ... parameter) const
    -> reply::checked::get_materialiv<Connection>
    {
        return cppual::glx::get_materialiv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_materialiv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_materialiv<Connection>
    {
        return cppual::glx::get_materialiv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_pixel_mapfv(Parameter && ... parameter) const
    -> reply::checked::get_pixel_mapfv<Connection>
    {
        return cppual::glx::get_pixel_mapfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_pixel_mapfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_pixel_mapfv<Connection>
    {
        return cppual::glx::get_pixel_mapfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_pixel_mapuiv(Parameter && ... parameter) const
    -> reply::checked::get_pixel_mapuiv<Connection>
    {
        return cppual::glx::get_pixel_mapuiv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_pixel_mapuiv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_pixel_mapuiv<Connection>
    {
        return cppual::glx::get_pixel_mapuiv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_pixel_mapusv(Parameter && ... parameter) const
    -> reply::checked::get_pixel_mapusv<Connection>
    {
        return cppual::glx::get_pixel_mapusv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_pixel_mapusv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_pixel_mapusv<Connection>
    {
        return cppual::glx::get_pixel_mapusv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_polygon_stipple(Parameter && ... parameter) const
    -> reply::checked::get_polygon_stipple<Connection>
    {
        return cppual::glx::get_polygon_stipple(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_polygon_stipple_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_polygon_stipple<Connection>
    {
        return cppual::glx::get_polygon_stipple_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_string(Parameter && ... parameter) const
    -> reply::checked::get_string<Connection>
    {
        return cppual::glx::get_string(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_string_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_string<Connection>
    {
        return cppual::glx::get_string_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_envfv(Parameter && ... parameter) const
    -> reply::checked::get_tex_envfv<Connection>
    {
        return cppual::glx::get_tex_envfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_envfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_envfv<Connection>
    {
        return cppual::glx::get_tex_envfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_enviv(Parameter && ... parameter) const
    -> reply::checked::get_tex_enviv<Connection>
    {
        return cppual::glx::get_tex_enviv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_enviv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_enviv<Connection>
    {
        return cppual::glx::get_tex_enviv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_gendv(Parameter && ... parameter) const
    -> reply::checked::get_tex_gendv<Connection>
    {
        return cppual::glx::get_tex_gendv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_gendv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_gendv<Connection>
    {
        return cppual::glx::get_tex_gendv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_genfv(Parameter && ... parameter) const
    -> reply::checked::get_tex_genfv<Connection>
    {
        return cppual::glx::get_tex_genfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_genfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_genfv<Connection>
    {
        return cppual::glx::get_tex_genfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_geniv(Parameter && ... parameter) const
    -> reply::checked::get_tex_geniv<Connection>
    {
        return cppual::glx::get_tex_geniv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_geniv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_geniv<Connection>
    {
        return cppual::glx::get_tex_geniv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_image(Parameter && ... parameter) const
    -> reply::checked::get_tex_image<Connection>
    {
        return cppual::glx::get_tex_image(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_image_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_image<Connection>
    {
        return cppual::glx::get_tex_image_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_parameterfv(Parameter && ... parameter) const
    -> reply::checked::get_tex_parameterfv<Connection>
    {
        return cppual::glx::get_tex_parameterfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_parameterfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_parameterfv<Connection>
    {
        return cppual::glx::get_tex_parameterfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_parameteriv(Parameter && ... parameter) const
    -> reply::checked::get_tex_parameteriv<Connection>
    {
        return cppual::glx::get_tex_parameteriv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_parameteriv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_parameteriv<Connection>
    {
        return cppual::glx::get_tex_parameteriv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_level_parameterfv(Parameter && ... parameter) const
    -> reply::checked::get_tex_level_parameterfv<Connection>
    {
        return cppual::glx::get_tex_level_parameterfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_level_parameterfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_level_parameterfv<Connection>
    {
        return cppual::glx::get_tex_level_parameterfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_tex_level_parameteriv(Parameter && ... parameter) const
    -> reply::checked::get_tex_level_parameteriv<Connection>
    {
        return cppual::glx::get_tex_level_parameteriv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_tex_level_parameteriv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_tex_level_parameteriv<Connection>
    {
        return cppual::glx::get_tex_level_parameteriv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    is_enabled(Parameter && ... parameter) const
    -> reply::checked::is_enabled<Connection>
    {
        return cppual::glx::is_enabled(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    is_enabled_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::is_enabled<Connection>
    {
        return cppual::glx::is_enabled_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    is_list(Parameter && ... parameter) const
    -> reply::checked::is_list<Connection>
    {
        return cppual::glx::is_list(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    is_list_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::is_list<Connection>
    {
        return cppual::glx::is_list_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    flush_checked(Parameter && ... parameter) const
    {
        cppual::glx::flush_checked(connection(),
                                std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    flush(Parameter && ... parameter) const
    {
        cppual::glx::flush(connection(),
                        std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    are_textures_resident(Parameter && ... parameter) const
    -> reply::checked::are_textures_resident<Connection>
    {
        return cppual::glx::are_textures_resident(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    are_textures_resident_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::are_textures_resident<Connection>
    {
        return cppual::glx::are_textures_resident_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_textures_checked(Parameter && ... parameter) const
    {
        cppual::glx::delete_textures_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_textures(Parameter && ... parameter) const
    {
        cppual::glx::delete_textures(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    gen_textures(Parameter && ... parameter) const
    -> reply::checked::gen_textures<Connection>
    {
        return cppual::glx::gen_textures(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    gen_textures_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::gen_textures<Connection>
    {
        return cppual::glx::gen_textures_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    is_texture(Parameter && ... parameter) const
    -> reply::checked::is_texture<Connection>
    {
        return cppual::glx::is_texture(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    is_texture_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::is_texture<Connection>
    {
        return cppual::glx::is_texture_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_color_table(Parameter && ... parameter) const
    -> reply::checked::get_color_table<Connection>
    {
        return cppual::glx::get_color_table(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_color_table_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_color_table<Connection>
    {
        return cppual::glx::get_color_table_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_color_table_parameterfv(Parameter && ... parameter) const
    -> reply::checked::get_color_table_parameterfv<Connection>
    {
        return cppual::glx::get_color_table_parameterfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_color_table_parameterfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_color_table_parameterfv<Connection>
    {
        return cppual::glx::get_color_table_parameterfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_color_table_parameteriv(Parameter && ... parameter) const
    -> reply::checked::get_color_table_parameteriv<Connection>
    {
        return cppual::glx::get_color_table_parameteriv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_color_table_parameteriv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_color_table_parameteriv<Connection>
    {
        return cppual::glx::get_color_table_parameteriv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_convolution_filter(Parameter && ... parameter) const
    -> reply::checked::get_convolution_filter<Connection>
    {
        return cppual::glx::get_convolution_filter(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_convolution_filter_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_convolution_filter<Connection>
    {
        return cppual::glx::get_convolution_filter_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_convolution_parameterfv(Parameter && ... parameter) const
    -> reply::checked::get_convolution_parameterfv<Connection>
    {
        return cppual::glx::get_convolution_parameterfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_convolution_parameterfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_convolution_parameterfv<Connection>
    {
        return cppual::glx::get_convolution_parameterfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_convolution_parameteriv(Parameter && ... parameter) const
    -> reply::checked::get_convolution_parameteriv<Connection>
    {
        return cppual::glx::get_convolution_parameteriv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_convolution_parameteriv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_convolution_parameteriv<Connection>
    {
        return cppual::glx::get_convolution_parameteriv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_separable_filter(Parameter && ... parameter) const
    -> reply::checked::get_separable_filter<Connection>
    {
        return cppual::glx::get_separable_filter(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_separable_filter_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_separable_filter<Connection>
    {
        return cppual::glx::get_separable_filter_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_histogram(Parameter && ... parameter) const
    -> reply::checked::get_histogram<Connection>
    {
        return cppual::glx::get_histogram(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_histogram_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_histogram<Connection>
    {
        return cppual::glx::get_histogram_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_histogram_parameterfv(Parameter && ... parameter) const
    -> reply::checked::get_histogram_parameterfv<Connection>
    {
        return cppual::glx::get_histogram_parameterfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_histogram_parameterfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_histogram_parameterfv<Connection>
    {
        return cppual::glx::get_histogram_parameterfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_histogram_parameteriv(Parameter && ... parameter) const
    -> reply::checked::get_histogram_parameteriv<Connection>
    {
        return cppual::glx::get_histogram_parameteriv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_histogram_parameteriv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_histogram_parameteriv<Connection>
    {
        return cppual::glx::get_histogram_parameteriv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_minmax(Parameter && ... parameter) const
    -> reply::checked::get_minmax<Connection>
    {
        return cppual::glx::get_minmax(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_minmax_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_minmax<Connection>
    {
        return cppual::glx::get_minmax_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_minmax_parameterfv(Parameter && ... parameter) const
    -> reply::checked::get_minmax_parameterfv<Connection>
    {
        return cppual::glx::get_minmax_parameterfv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_minmax_parameterfv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_minmax_parameterfv<Connection>
    {
        return cppual::glx::get_minmax_parameterfv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_minmax_parameteriv(Parameter && ... parameter) const
    -> reply::checked::get_minmax_parameteriv<Connection>
    {
        return cppual::glx::get_minmax_parameteriv(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_minmax_parameteriv_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_minmax_parameteriv<Connection>
    {
        return cppual::glx::get_minmax_parameteriv_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_compressed_tex_image_arb(Parameter && ... parameter) const
    -> reply::checked::get_compressed_tex_image_arb<Connection>
    {
        return cppual::glx::get_compressed_tex_image_arb(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_compressed_tex_image_arb_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_compressed_tex_image_arb<Connection>
    {
        return cppual::glx::get_compressed_tex_image_arb_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_queries_arb_checked(Parameter && ... parameter) const
    {
        cppual::glx::delete_queries_arb_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_queries_arb(Parameter && ... parameter) const
    {
        cppual::glx::delete_queries_arb(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    gen_queries_arb(Parameter && ... parameter) const
    -> reply::checked::gen_queries_arb<Connection>
    {
        return cppual::glx::gen_queries_arb(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    gen_queries_arb_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::gen_queries_arb<Connection>
    {
        return cppual::glx::gen_queries_arb_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    is_query_arb(Parameter && ... parameter) const
    -> reply::checked::is_query_arb<Connection>
    {
        return cppual::glx::is_query_arb(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    is_query_arb_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::is_query_arb<Connection>
    {
        return cppual::glx::is_query_arb_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_queryiv_arb(Parameter && ... parameter) const
    -> reply::checked::get_queryiv_arb<Connection>
    {
        return cppual::glx::get_queryiv_arb(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_queryiv_arb_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_queryiv_arb<Connection>
    {
        return cppual::glx::get_queryiv_arb_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_query_objectiv_arb(Parameter && ... parameter) const
    -> reply::checked::get_query_objectiv_arb<Connection>
    {
        return cppual::glx::get_query_objectiv_arb(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_query_objectiv_arb_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_query_objectiv_arb<Connection>
    {
        return cppual::glx::get_query_objectiv_arb_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_query_objectuiv_arb(Parameter && ... parameter) const
    -> reply::checked::get_query_objectuiv_arb<Connection>
    {
        return cppual::glx::get_query_objectuiv_arb(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_query_objectuiv_arb_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_query_objectuiv_arb<Connection>
    {
        return cppual::glx::get_query_objectuiv_arb_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::glx::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::glx::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - m_first_event) {

        case XCB_GLX_PBUFFER_CLOBBER:
            handler(cppual::glx::event::pbuffer_clobber<Connection>(m_c, m_first_event, event));
            return true;

        case XCB_GLX_BUFFER_SWAP_COMPLETE:
            handler(cppual::glx::event::buffer_swap_complete<Connection>(m_c, m_first_event, event));
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
    typedef cppual::glx::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::glx::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - m_first_error) {

        case XCB_GLX_GENERIC: // -1
            throw cppual::glx::error::generic(error);

        case XCB_GLX_BAD_CONTEXT: // 0
            throw cppual::glx::error::bad_context(error);

        case XCB_GLX_BAD_CONTEXT_STATE: // 1
            throw cppual::glx::error::bad_context_state(error);

        case XCB_GLX_BAD_DRAWABLE: // 2
            throw cppual::glx::error::bad_drawable(error);

        case XCB_GLX_BAD_PIXMAP: // 3
            throw cppual::glx::error::bad_pixmap(error);

        case XCB_GLX_BAD_CONTEXT_TAG: // 4
            throw cppual::glx::error::bad_context_tag(error);

        case XCB_GLX_BAD_CURRENT_WINDOW: // 5
            throw cppual::glx::error::bad_current_window(error);

        case XCB_GLX_BAD_RENDER_REQUEST: // 6
            throw cppual::glx::error::bad_render_request(error);

        case XCB_GLX_BAD_LARGE_REQUEST: // 7
            throw cppual::glx::error::bad_large_request(error);

        case XCB_GLX_UNSUPPORTED_PRIVATE_REQUEST: // 8
            throw cppual::glx::error::unsupported_private_request(error);

        case XCB_GLX_BAD_FB_CONFIG: // 9
            throw cppual::glx::error::bad_fb_config(error);

        case XCB_GLX_BAD_PBUFFER: // 10
            throw cppual::glx::error::bad_pbuffer(error);

        case XCB_GLX_BAD_CURRENT_DRAWABLE: // 11
            throw cppual::glx::error::bad_current_drawable(error);

        case XCB_GLX_BAD_WINDOW: // 12
            throw cppual::glx::error::bad_window(error);

        case XCB_GLX_GLX_BAD_PROFILE_ARB: // 13
            throw cppual::glx::error::glx_bad_profile_arb(error);

        };
    }

protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::glx

#endif // CPPUAL_GLX_HPP
