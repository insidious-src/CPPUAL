#ifndef CPPUAL_SHAPE_HPP
#define CPPUAL_SHAPE_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/shape.h>

namespace cppual { namespace shape {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_shape_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_shape_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::shape::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::shape::event::dispatcher<Connection>;
    using error_dispatcher = cppual::shape::error::dispatcher;
};

namespace event {
template<typename Connection>
class notify
        : public cppual::generic::event<xcb_shape_notify_event_t>
{
public:
    typedef cppual::shape::extension extension;

    typedef cppual::generic::event<xcb_shape_notify_event_t> base;

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
        return XCB_SHAPE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::shape::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static std::string description(void)
    {
        return std::string("XCB_SHAPE_NOTIFY");
    }

    uint8_t first_event(void)
    {
        return m_first_event;
    }

    template<typename ReturnType = ::xcb_window_t, typename ... Parameter>
    ReturnType
    affected_window(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->affected_window),
        ReturnType,
        Parameter ...>;
        return make()(this->m_c,
                      (*this)->affected_window,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection m_c;
    const uint8_t m_first_event;
}; // class notify



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
        SIGNATURE(xcb_shape_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_shape_query_version_reply),
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
SIGNATURE(xcb_shape_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_shape_query_version_unchecked)>;
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
rectangles_checked(Connection && c, ::xcb_shape_op_t operation, ::xcb_shape_kind_t destination_kind, uint8_t ordering, ::xcb_window_t destination_window, int16_t x_offset, int16_t y_offset, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
    cppual::generic::check<Connection, cppual::shape::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shape_rectangles_checked(std::forward<Connection>(c), operation, destination_kind, ordering, destination_window, x_offset, y_offset, rectangles_len, rectangles));
}

template<typename Connection>
void
rectangles(Connection && c, ::xcb_shape_op_t operation, ::xcb_shape_kind_t destination_kind, uint8_t ordering, ::xcb_window_t destination_window, int16_t x_offset, int16_t y_offset, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
    ::xcb_shape_rectangles(std::forward<Connection>(c), operation, destination_kind, ordering, destination_window, x_offset, y_offset, rectangles_len, rectangles);
}

template<typename Connection, typename Rectangles_Iterator>
void
rectangles_checked(Connection && c, ::xcb_shape_op_t operation, ::xcb_shape_kind_t destination_kind, uint8_t ordering, ::xcb_window_t destination_window, int16_t x_offset, int16_t y_offset, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
    typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> rectangles =
    { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

    cppual::generic::check<Connection, cppual::shape::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shape_rectangles_checked(std::forward<Connection>(c), operation, destination_kind, ordering, destination_window, x_offset, y_offset, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data())));
}

template<typename Connection, typename Rectangles_Iterator>
void
rectangles(Connection && c, ::xcb_shape_op_t operation, ::xcb_shape_kind_t destination_kind, uint8_t ordering, ::xcb_window_t destination_window, int16_t x_offset, int16_t y_offset, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
    typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
            vector_type;
    std::vector<vector_type> rectangles =
    { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

    ::xcb_shape_rectangles(std::forward<Connection>(c), operation, destination_kind, ordering, destination_window, x_offset, y_offset, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data()));
}

template<typename Connection, typename ... Parameter>
void
mask_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shape::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shape_mask_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
mask(Parameter && ... parameter)
{
    ::xcb_shape_mask(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
combine_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shape::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shape_combine_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
combine(Parameter && ... parameter)
{
    ::xcb_shape_combine(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
offset_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shape::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shape_offset_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
offset(Parameter && ... parameter)
{
    ::xcb_shape_offset(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_extents
        : public cppual::generic::reply<query_extents<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_shape_query_extents_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_extents<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_shape_query_extents_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    query_extents(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_extents

} // namespace detail

namespace checked {
template<typename Connection>
using query_extents = detail::query_extents<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_shape_query_extents)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_extents = detail::query_extents<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_shape_query_extents_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_extents<Connection>
query_extents(Connection && c, Parameter && ... parameter)
{
    return reply::checked::query_extents<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_extents<Connection>
query_extents_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::query_extents<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
select_input_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::shape::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_shape_select_input_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
select_input(Parameter && ... parameter)
{
    ::xcb_shape_select_input(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class input_selected
        : public cppual::generic::reply<input_selected<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_shape_input_selected_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<input_selected<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_shape_input_selected_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    input_selected(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class input_selected

} // namespace detail

namespace checked {
template<typename Connection>
using input_selected = detail::input_selected<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_shape_input_selected)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using input_selected = detail::input_selected<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_shape_input_selected_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::input_selected<Connection>
input_selected(Connection && c, Parameter && ... parameter)
{
    return reply::checked::input_selected<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::input_selected<Connection>
input_selected_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::input_selected<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class get_rectangles
        : public cppual::generic::reply<get_rectangles<Connection, Check, CookieFunction>,
        Connection,
        Check,
        SIGNATURE(xcb_shape_get_rectangles_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<get_rectangles<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_shape_get_rectangles_reply),
            CookieFunction>
            base;

    template<typename C, typename ... Parameter>
    get_rectangles(C && c, Parameter && ... parameter)
        : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
            ::xcb_shape_get_rectangles_reply_t,
            cppual::generic::iterator<Connection,
            ::xcb_rectangle_t,
            SIGNATURE(xcb_shape_get_rectangles_rectangles),
            SIGNATURE(xcb_shape_get_rectangles_rectangles_length)>
            >
            rectangles(void)
    {
        return cppual::generic::list<Connection,
                ::xcb_shape_get_rectangles_reply_t,
                cppual::generic::iterator<Connection,
                ::xcb_rectangle_t,
                SIGNATURE(xcb_shape_get_rectangles_rectangles),
                SIGNATURE(xcb_shape_get_rectangles_rectangles_length)>
                >(this->m_c, this->get());
    }
}; // class get_rectangles

} // namespace detail

namespace checked {
template<typename Connection>
using get_rectangles = detail::get_rectangles<
Connection, cppual::generic::checked_tag,
SIGNATURE(xcb_shape_get_rectangles)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using get_rectangles = detail::get_rectangles<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_shape_get_rectangles_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::get_rectangles<Connection>
get_rectangles(Connection && c, Parameter && ... parameter)
{
    return reply::checked::get_rectangles<Connection>(
                std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::get_rectangles<Connection>
get_rectangles_unchecked(Connection && c, Parameter && ... parameter)
{
    return reply::unchecked::get_rectangles<Connection>(
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
    typedef cppual::shape::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    shape(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::shape::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::shape::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    rectangles_checked(Parameter && ... parameter) const
    {
        cppual::shape::rectangles_checked(connection(),
                                       std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    rectangles(Parameter && ... parameter) const
    {
        cppual::shape::rectangles(connection(),
                               std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    mask_checked(Parameter && ... parameter) const
    {
        cppual::shape::mask_checked(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    mask(Parameter && ... parameter) const
    {
        cppual::shape::mask(connection(),
                         std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    combine_checked(Parameter && ... parameter) const
    {
        cppual::shape::combine_checked(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    combine(Parameter && ... parameter) const
    {
        cppual::shape::combine(connection(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    offset_checked(Parameter && ... parameter) const
    {
        cppual::shape::offset_checked(connection(),
                                   std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    offset(Parameter && ... parameter) const
    {
        cppual::shape::offset(connection(),
                           std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_extents(Parameter && ... parameter) const
    -> reply::checked::query_extents<Connection>
    {
        return cppual::shape::query_extents(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_extents_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_extents<Connection>
    {
        return cppual::shape::query_extents_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    select_input_checked(Parameter && ... parameter) const
    {
        cppual::shape::select_input_checked(connection(),
                                         std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    select_input(Parameter && ... parameter) const
    {
        cppual::shape::select_input(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    input_selected(Parameter && ... parameter) const
    -> reply::checked::input_selected<Connection>
    {
        return cppual::shape::input_selected(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    input_selected_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::input_selected<Connection>
    {
        return cppual::shape::input_selected_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    get_rectangles(Parameter && ... parameter) const
    -> reply::checked::get_rectangles<Connection>
    {
        return cppual::shape::get_rectangles(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    get_rectangles_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::get_rectangles<Connection>
    {
        return cppual::shape::get_rectangles_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::shape::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : m_c(std::forward<C>(c))
        , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::shape::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - m_first_event) {

        case XCB_SHAPE_NOTIFY:
            handler(cppual::shape::event::notify<Connection>(m_c, m_first_event, event));
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
    typedef cppual::shape::extension extension;

    dispatcher(uint8_t first_error)
        : m_first_error(first_error)
    {}

    dispatcher(const cppual::shape::extension & extension)
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


} } // namespace xpp::shape

#endif // CPPUAL_SHAPE_HPP
