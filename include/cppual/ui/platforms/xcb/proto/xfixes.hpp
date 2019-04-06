#ifndef CPPUAL_XFIXES_HPP
#define CPPUAL_XFIXES_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/xfixes.h>

namespace cppual { namespace xfixes {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_xfixes_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_xfixes_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::xfixes::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::xfixes::event::dispatcher<Connection>;
    using error_dispatcher = cppual::xfixes::error::dispatcher;
};

namespace event {
template<typename Connection>
class selection_notify
        : public cppual::generic::event<xcb_xfixes_selection_notify_event_t>
{
public:
    typedef cppual::xfixes::extension extension;

    typedef cppual::generic::event<xcb_xfixes_selection_notify_event_t> base;

    template<typename C>
    selection_notify(C && c,
                     uint8_t first_event,
                     const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~selection_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XFIXES_SELECTION_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xfixes::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_XFIXES_SELECTION_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    window(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->window),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->window,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    owner(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->owner),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->owner,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    selection(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->selection),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->selection,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class selection_notify



} // namespace event


namespace event {
template<typename Connection>
class cursor_notify
        : public cppual::generic::event<xcb_xfixes_cursor_notify_event_t>
{
public:
    typedef cppual::xfixes::extension extension;

    typedef cppual::generic::event<xcb_xfixes_cursor_notify_event_t> base;

    template<typename C>
    cursor_notify(C && c,
                  uint8_t first_event,
                  const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    virtual ~cursor_notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XFIXES_CURSOR_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::xfixes::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_XFIXES_CURSOR_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    window(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->window),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->window,
                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
    name(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->name),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->name,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class cursor_notify



} // namespace event



namespace error {
class bad_region
        : public cppual::generic::error<bad_region,
        ::xcb_xfixes_bad_region_error_t>
{
public:
    using cppual::generic::error<bad_region, ::xcb_xfixes_bad_region_error_t>::error;

    virtual ~bad_region(void) {}

    static uint8_t opcode(void)
    {
        return XCB_XFIXES_BAD_REGION;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::xfixes::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static std::string description(void)
    {
        return std::string("XCB_XFIXES_BAD_REGION");
    }

protected:
    uint8_t m_first_error;
}; // class bad_region
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
        SIGNATURE(xcb_xfixes_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xfixes_query_version_reply),
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
SIGNATURE(xcb_xfixes_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xfixes_query_version_unchecked)>;
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
change_save_set_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_change_save_set_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_save_set(Parameter && ... parameter)
{
    ::xcb_xfixes_change_save_set(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_selection_input_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_select_selection_input_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_selection_input(Parameter && ... parameter)
{
    ::xcb_xfixes_select_selection_input(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_cursor_input_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_select_cursor_input_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_cursor_input(Parameter && ... parameter)
{
    ::xcb_xfixes_select_cursor_input(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_cursor_image
        : public cppual::generic::reply<get_cursor_image<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xfixes_get_cursor_image_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_cursor_image<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xfixes_get_cursor_image_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_cursor_image(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xfixes_get_cursor_image_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xfixes_get_cursor_image_cursor_image),
            SIGNATURE(xcb_xfixes_get_cursor_image_cursor_image_length)>
            >
            cursor_image(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xfixes_get_cursor_image_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xfixes_get_cursor_image_cursor_image),
                SIGNATURE(xcb_xfixes_get_cursor_image_cursor_image_length)>
                >(this->m_c, this->get());
    }
}; // class get_cursor_image

} // namespace detail

namespace checked {
template<typename Connection>
using get_cursor_image = detail::get_cursor_image<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xfixes_get_cursor_image)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_cursor_image = detail::get_cursor_image<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xfixes_get_cursor_image_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_cursor_image<Connection>
get_cursor_image(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_cursor_image<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_cursor_image<Connection>
get_cursor_image_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_cursor_image<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
create_region_checked(Connection && c, ::xcb_xfixes_region_t region, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_create_region_checked(std::forward<Connection>(c), region, rectangles_len, rectangles));
}

template<typename Connection>
void
create_region(Connection && c, ::xcb_xfixes_region_t region, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
    ::xcb_xfixes_create_region(std::forward<Connection>(c), region, rectangles_len, rectangles);
}

template<typename Connection, typename Rectangles_Iterator>
void
create_region_checked(Connection && c, ::xcb_xfixes_region_t region, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
    typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> rectangles =
    { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_create_region_checked(std::forward<Connection>(c), region, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data())));
}

template<typename Connection, typename Rectangles_Iterator>
void
create_region(Connection && c, ::xcb_xfixes_region_t region, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
    typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> rectangles =
    { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

    ::xcb_xfixes_create_region(std::forward<Connection>(c), region, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data()));
}

template<typename Connection, typename ... Parameter>
void
create_region_from_bitmap_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_create_region_from_bitmap_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_region_from_bitmap(Parameter && ... parameter)
{
    ::xcb_xfixes_create_region_from_bitmap(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_region_from_window_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_create_region_from_window_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_region_from_window(Parameter && ... parameter)
{
    ::xcb_xfixes_create_region_from_window(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_region_from_gc_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_create_region_from_gc_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_region_from_gc(Parameter && ... parameter)
{
    ::xcb_xfixes_create_region_from_gc(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_region_from_picture_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_create_region_from_picture_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_region_from_picture(Parameter && ... parameter)
{
    ::xcb_xfixes_create_region_from_picture(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_destroy_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy_region(Parameter && ... parameter)
{
    ::xcb_xfixes_destroy_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_region_checked(Connection && c, ::xcb_xfixes_region_t region, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_set_region_checked(std::forward<Connection>(c), region, rectangles_len, rectangles));
}

template<typename Connection>
void
set_region(Connection && c, ::xcb_xfixes_region_t region, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
    ::xcb_xfixes_set_region(std::forward<Connection>(c), region, rectangles_len, rectangles);
}

template<typename Connection, typename Rectangles_Iterator>
void
set_region_checked(Connection && c, ::xcb_xfixes_region_t region, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
    typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> rectangles =
    { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_set_region_checked(std::forward<Connection>(c), region, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data())));
}

template<typename Connection, typename Rectangles_Iterator>
void
set_region(Connection && c, ::xcb_xfixes_region_t region, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
    typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> rectangles =
    { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

    ::xcb_xfixes_set_region(std::forward<Connection>(c), region, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data()));
}

template<typename Connection, typename ... Parameter>
void
copy_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_copy_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
copy_region(Parameter && ... parameter)
{
    ::xcb_xfixes_copy_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
union_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_union_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
union_region(Parameter && ... parameter)
{
    ::xcb_xfixes_union_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
intersect_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_intersect_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
intersect_region(Parameter && ... parameter)
{
    ::xcb_xfixes_intersect_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
subtract_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_subtract_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
subtract_region(Parameter && ... parameter)
{
    ::xcb_xfixes_subtract_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
invert_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_invert_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
invert_region(Parameter && ... parameter)
{
    ::xcb_xfixes_invert_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
translate_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_translate_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
translate_region(Parameter && ... parameter)
{
    ::xcb_xfixes_translate_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
region_extents_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_region_extents_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
region_extents(Parameter && ... parameter)
{
    ::xcb_xfixes_region_extents(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class fetch_region
        : public cppual::generic::reply<fetch_region<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xfixes_fetch_region_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<fetch_region<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xfixes_fetch_region_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    fetch_region(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xfixes_fetch_region_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_rectangle_t,
            SIGNATURE(xcb_xfixes_fetch_region_rectangles),
            SIGNATURE(xcb_xfixes_fetch_region_rectangles_length)>
            >
            rectangles(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xfixes_fetch_region_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_rectangle_t,
                SIGNATURE(xcb_xfixes_fetch_region_rectangles),
                SIGNATURE(xcb_xfixes_fetch_region_rectangles_length)>
                >(this->m_c, this->get());
    }
}; // class fetch_region

} // namespace detail

namespace checked {
template<typename Connection>
using fetch_region = detail::fetch_region<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xfixes_fetch_region)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using fetch_region = detail::fetch_region<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xfixes_fetch_region_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::fetch_region<Connection>
fetch_region(Connection && c, Parameter && ... parameter)
{
    return reply::checked::fetch_region<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::fetch_region<Connection>
fetch_region_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::fetch_region<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_gc_clip_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_set_gc_clip_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_gc_clip_region(Parameter && ... parameter)
{
    ::xcb_xfixes_set_gc_clip_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_window_shape_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_set_window_shape_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_window_shape_region(Parameter && ... parameter)
{
    ::xcb_xfixes_set_window_shape_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_picture_clip_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_set_picture_clip_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_picture_clip_region(Parameter && ... parameter)
{
    ::xcb_xfixes_set_picture_clip_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_cursor_name_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_set_cursor_name_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_cursor_name(Parameter && ... parameter)
{
    ::xcb_xfixes_set_cursor_name(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_cursor_name
        : public cppual::generic::reply<get_cursor_name<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xfixes_get_cursor_name_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_cursor_name<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xfixes_get_cursor_name_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_cursor_name(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    std::string
            name(void)
    {
        return std::string(xcb_xfixes_get_cursor_name_name(this->get().get()),
                           ::xcb_xfixes_get_cursor_name_name_length(this->get().get()));
    }


    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
            atom(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->atom),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      this->get()->atom,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_cursor_name

} // namespace detail

namespace checked {
template<typename Connection>
using get_cursor_name = detail::get_cursor_name<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xfixes_get_cursor_name)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_cursor_name = detail::get_cursor_name<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xfixes_get_cursor_name_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_cursor_name<Connection>
get_cursor_name(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_cursor_name<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_cursor_name<Connection>
get_cursor_name_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_cursor_name<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_cursor_image_and_name
        : public cppual::generic::reply<get_cursor_image_and_name<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_xfixes_get_cursor_image_and_name_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_cursor_image_and_name<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_xfixes_get_cursor_image_and_name_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_cursor_image_and_name(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_xfixes_get_cursor_image_and_name_reply_t,
            cppual::generic::iterator<Connection,
            uint32_t,
            SIGNATURE(xcb_xfixes_get_cursor_image_and_name_cursor_image),
            SIGNATURE(xcb_xfixes_get_cursor_image_and_name_cursor_image_length)>
            >
            cursor_image(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_xfixes_get_cursor_image_and_name_reply_t,
                cppual::generic::iterator<Connection,
                uint32_t,
                SIGNATURE(xcb_xfixes_get_cursor_image_and_name_cursor_image),
                SIGNATURE(xcb_xfixes_get_cursor_image_and_name_cursor_image_length)>
                >(this->m_c, this->get());
    }


    std::string
            name(void)
    {
        return std::string(xcb_xfixes_get_cursor_image_and_name_name(this->get().get()),
                           ::xcb_xfixes_get_cursor_image_and_name_name_length(this->get().get()));
    }


    template<typename ReturnType = ::xcb_atom_t, typename ... Parameter>
    ReturnType
            cursor_atom(Parameter && ... parameter)
    {
        using make = cppual::generic::factory::make<Connection,
        decltype(this->get()->cursor_atom),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      this->get()->cursor_atom,
                      std::forward<Parameter>(parameter) ...);
    }
}; // class get_cursor_image_and_name

} // namespace detail

namespace checked {
template<typename Connection>
using get_cursor_image_and_name = detail::get_cursor_image_and_name<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_xfixes_get_cursor_image_and_name)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_cursor_image_and_name = detail::get_cursor_image_and_name<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_xfixes_get_cursor_image_and_name_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_cursor_image_and_name<Connection>
get_cursor_image_and_name(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_cursor_image_and_name<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_cursor_image_and_name<Connection>
get_cursor_image_and_name_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_cursor_image_and_name<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_cursor_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_change_cursor_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_cursor(Parameter && ... parameter)
{
    ::xcb_xfixes_change_cursor(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_cursor_by_name_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_change_cursor_by_name_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_cursor_by_name(Parameter && ... parameter)
{
    ::xcb_xfixes_change_cursor_by_name(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
expand_region_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_expand_region_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
expand_region(Parameter && ... parameter)
{
    ::xcb_xfixes_expand_region(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
hide_cursor_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_hide_cursor_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
hide_cursor(Parameter && ... parameter)
{
    ::xcb_xfixes_hide_cursor(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
show_cursor_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_show_cursor_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
show_cursor(Parameter && ... parameter)
{
    ::xcb_xfixes_show_cursor(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_pointer_barrier_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_create_pointer_barrier_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_pointer_barrier(Parameter && ... parameter)
{
    ::xcb_xfixes_create_pointer_barrier(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
delete_pointer_barrier_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::xfixes::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_xfixes_delete_pointer_barrier_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
delete_pointer_barrier(Parameter && ... parameter)
{
    ::xcb_xfixes_delete_pointer_barrier(std::forward<Parameter>(parameter) ...);
}















template<typename Derived, typename Connection>
class barrier
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_xfixes_barrier_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~barrier(void) {}

    template<typename ... Parameter>
    void
    create_pointer_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_pointer_barrier_checked(connection(),
                                                    resource(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_pointer(Parameter && ... parameter) const
    {
        cppual::xfixes::create_pointer_barrier(connection(),
                                            resource(),
                                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_pointer_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::delete_pointer_barrier_checked(connection(),
                                                    resource(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_pointer(Parameter && ... parameter) const
    {
        cppual::xfixes::delete_pointer_barrier(connection(),
                                            resource(),
                                            std::forward<Parameter>(parameter) ...);
    }



}; // class barrier











template<typename Derived, typename Connection>
class region
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_xfixes_region_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~region(void) {}

    template<typename ... Parameter>
    void
    create_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_checked(connection(),
                                           resource(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_from_bitmap_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_bitmap_checked(connection(),
                                                       resource(),
                                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_from_bitmap(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_bitmap(connection(),
                                               resource(),
                                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_from_window_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_window_checked(connection(),
                                                       resource(),
                                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_from_window(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_window(connection(),
                                               resource(),
                                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_from_gc_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_gc_checked(connection(),
                                                   resource(),
                                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_from_gc(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_gc(connection(),
                                           resource(),
                                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_from_picture_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_picture_checked(connection(),
                                                        resource(),
                                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_from_picture(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_picture(connection(),
                                                resource(),
                                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::destroy_region_checked(connection(),
                                            resource(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::xfixes::destroy_region(connection(),
                                    resource(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::set_region_checked(connection(),
                                        resource(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set(Parameter && ... parameter) const
    {
        cppual::xfixes::set_region(connection(),
                                resource(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    copy_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::copy_region_checked(connection(),
                                         resource(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    copy(Parameter && ... parameter) const
    {
        cppual::xfixes::copy_region(connection(),
                                 resource(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    union_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::union_region_checked(connection(),
                                          resource(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    union(Parameter && ... parameter) const
    {
        cppual::xfixes::union_region(connection(),
                                  resource(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    intersect_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::intersect_region_checked(connection(),
                                              resource(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    intersect(Parameter && ... parameter) const
    {
        cppual::xfixes::intersect_region(connection(),
                                      resource(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    subtract_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::subtract_region_checked(connection(),
                                             resource(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    subtract(Parameter && ... parameter) const
    {
        cppual::xfixes::subtract_region(connection(),
                                     resource(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    invert_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::invert_region_checked(connection(),
                                           resource(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    invert(Parameter && ... parameter) const
    {
        cppual::xfixes::invert_region(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    translate_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::translate_region_checked(connection(),
                                              resource(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    translate(Parameter && ... parameter) const
    {
        cppual::xfixes::translate_region(connection(),
                                      resource(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    region_extents_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::region_extents_checked(connection(),
                                            resource(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    region_extents(Parameter && ... parameter) const
    {
        cppual::xfixes::region_extents(connection(),
                                    resource(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    fetch(Parameter && ... parameter) const
    -> reply::checked::fetch_region<Connection>
    {
        return cppual::xfixes::fetch_region(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    fetch_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::fetch_region<Connection>
    {
        return cppual::xfixes::fetch_region_unchecked(
                    connection(),
                    resource(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    expand_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::expand_region_checked(connection(),
                                           resource(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    expand(Parameter && ... parameter) const
    {
        cppual::xfixes::expand_region(connection(),
                                   resource(),
                                   std::forward<Parameter>(parameter) ...);
    }



}; // class region









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
    typedef cppual::xfixes::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    xfixes(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::xfixes::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::xfixes::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_save_set_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::change_save_set_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_save_set(Parameter && ... parameter) const
    {
        cppual::xfixes::change_save_set(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_selection_input_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::select_selection_input_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_selection_input(Parameter && ... parameter) const
    {
        cppual::xfixes::select_selection_input(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_cursor_input_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::select_cursor_input_checked(connection(),
                                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_cursor_input(Parameter && ... parameter) const
    {
        cppual::xfixes::select_cursor_input(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_cursor_image(Parameter && ... parameter) const
    -> reply::checked::get_cursor_image<Connection>
    {
        return cppual::xfixes::get_cursor_image(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_cursor_image_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_cursor_image<Connection>
    {
        return cppual::xfixes::get_cursor_image_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_region(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_region_from_bitmap_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_bitmap_checked(connection(),
                                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_region_from_bitmap(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_bitmap(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_region_from_window_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_window_checked(connection(),
                                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_region_from_window(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_window(connection(),
                                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_region_from_gc_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_gc_checked(connection(),
                                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_region_from_gc(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_gc(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_region_from_picture_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_picture_checked(connection(),
                                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_region_from_picture(Parameter && ... parameter) const
    {
        cppual::xfixes::create_region_from_picture(connection(),
                                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::destroy_region_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy_region(Parameter && ... parameter) const
    {
        cppual::xfixes::destroy_region(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::set_region_checked(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_region(Parameter && ... parameter) const
    {
        cppual::xfixes::set_region(connection(),
                                std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    copy_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::copy_region_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    copy_region(Parameter && ... parameter) const
    {
        cppual::xfixes::copy_region(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    union_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::union_region_checked(connection(),
                                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    union_region(Parameter && ... parameter) const
    {
        cppual::xfixes::union_region(connection(),
                                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    intersect_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::intersect_region_checked(connection(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    intersect_region(Parameter && ... parameter) const
    {
        cppual::xfixes::intersect_region(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    subtract_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::subtract_region_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    subtract_region(Parameter && ... parameter) const
    {
        cppual::xfixes::subtract_region(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    invert_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::invert_region_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    invert_region(Parameter && ... parameter) const
    {
        cppual::xfixes::invert_region(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    translate_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::translate_region_checked(connection(),
                                              std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    translate_region(Parameter && ... parameter) const
    {
        cppual::xfixes::translate_region(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    region_extents_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::region_extents_checked(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    region_extents(Parameter && ... parameter) const
    {
        cppual::xfixes::region_extents(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    fetch_region(Parameter && ... parameter) const
    -> reply::checked::fetch_region<Connection>
    {
        return cppual::xfixes::fetch_region(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    fetch_region_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::fetch_region<Connection>
    {
        return cppual::xfixes::fetch_region_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_gc_clip_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::set_gc_clip_region_checked(connection(),
                                                std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_gc_clip_region(Parameter && ... parameter) const
    {
        cppual::xfixes::set_gc_clip_region(connection(),
                                        std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_window_shape_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::set_window_shape_region_checked(connection(),
                                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_window_shape_region(Parameter && ... parameter) const
    {
        cppual::xfixes::set_window_shape_region(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_picture_clip_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::set_picture_clip_region_checked(connection(),
                                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_picture_clip_region(Parameter && ... parameter) const
    {
        cppual::xfixes::set_picture_clip_region(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_cursor_name_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::set_cursor_name_checked(connection(),
                                             std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_cursor_name(Parameter && ... parameter) const
    {
        cppual::xfixes::set_cursor_name(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_cursor_name(Parameter && ... parameter) const
    -> reply::checked::get_cursor_name<Connection>
    {
        return cppual::xfixes::get_cursor_name(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_cursor_name_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_cursor_name<Connection>
    {
        return cppual::xfixes::get_cursor_name_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_cursor_image_and_name(Parameter && ... parameter) const
    -> reply::checked::get_cursor_image_and_name<Connection>
    {
        return cppual::xfixes::get_cursor_image_and_name(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_cursor_image_and_name_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_cursor_image_and_name<Connection>
    {
        return cppual::xfixes::get_cursor_image_and_name_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_cursor_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::change_cursor_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_cursor(Parameter && ... parameter) const
    {
        cppual::xfixes::change_cursor(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_cursor_by_name_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::change_cursor_by_name_checked(connection(),
                                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_cursor_by_name(Parameter && ... parameter) const
    {
        cppual::xfixes::change_cursor_by_name(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    expand_region_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::expand_region_checked(connection(),
                                           std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    expand_region(Parameter && ... parameter) const
    {
        cppual::xfixes::expand_region(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    hide_cursor_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::hide_cursor_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    hide_cursor(Parameter && ... parameter) const
    {
        cppual::xfixes::hide_cursor(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    show_cursor_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::show_cursor_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    show_cursor(Parameter && ... parameter) const
    {
        cppual::xfixes::show_cursor(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_pointer_barrier_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::create_pointer_barrier_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_pointer_barrier(Parameter && ... parameter) const
    {
        cppual::xfixes::create_pointer_barrier(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    delete_pointer_barrier_checked(Parameter && ... parameter) const
    {
        cppual::xfixes::delete_pointer_barrier_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    delete_pointer_barrier(Parameter && ... parameter) const
    {
        cppual::xfixes::delete_pointer_barrier(connection(),
                                            std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::xfixes::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::xfixes::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - m_first_event) {

        case XCB_XFIXES_SELECTION_NOTIFY:
            handler(cppual::xfixes::event::selection_notify<Connection>(m_c, m_first_event, event));
            return true;

        case XCB_XFIXES_CURSOR_NOTIFY:
            handler(cppual::xfixes::event::cursor_notify<Connection>(m_c, m_first_event, event));
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
    typedef cppual::xfixes::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::xfixes::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - m_first_error) {

        case XCB_XFIXES_BAD_REGION: // 0
            throw cppual::xfixes::error::bad_region(error);

        };
    }

protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace xpp::xfixes

#endif // CPPUAL_XFIXES_HPP
