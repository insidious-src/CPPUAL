#ifndef CPPUAL_DAMAGE_HPP
#define CPPUAL_DAMAGE_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>
#include <cppual/string.h>

#include <vector>

#include <xcb/damage.h>

namespace cppual { namespace damage {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
        : public cppual::generic::extension<extension, &xcb_damage_id>
{
public:
    using base = cppual::generic::extension<extension, &xcb_damage_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::damage::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::damage::event::dispatcher<Connection>;
    using error_dispatcher = cppual::damage::error::dispatcher;
};

namespace event {
template<typename Connection>
class notify
        : public cppual::generic::event<xcb_damage_notify_event_t>
{
public:
    typedef cppual::damage::extension extension;

    typedef cppual::generic::event<xcb_damage_notify_event_t> base;

    template<typename C>
    notify(C && c,
           uint8_t first_event,
           const std::shared_ptr<xcb_generic_event_t> & event)
        : base(event)
        , _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    virtual ~notify(void) {}

    static uint8_t opcode(void)
    {
        return XCB_DAMAGE_NOTIFY;
    }

    static uint8_t opcode(uint8_t first_event)
    {
        return first_event + opcode();
    }

    static uint8_t opcode(const cppual::damage::extension & extension)
    {
        return opcode(extension->first_event);
    }

    static string description(void)
    {
        return string("XCB_DAMAGE_NOTIFY");
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

    template<typename ReturnType = ::xcb_damage_damage_t, typename ... Parameter>
    ReturnType
    damage(Parameter && ... parameter) const
    {
        using make = cppual::generic::factory::make<Connection,
        decltype((*this)->damage),
        ReturnType,
        Parameter ...>;
        return make()(this->_M_c,
                      (*this)->damage,
                      std::forward<Parameter>(parameter) ...);
    }

protected:
    Connection _M_c;
    const uint8_t _M_first_event;
}; // class notify



} // namespace event



namespace error {
class bad_damage
        : public cppual::generic::error<bad_damage,
        ::xcb_damage_bad_damage_error_t>
{
public:
    using cppual::generic::error<bad_damage, ::xcb_damage_bad_damage_error_t>::error;

    virtual ~bad_damage(void) {}

    static uint8_t opcode(void)
    {
        return XCB_DAMAGE_BAD_DAMAGE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
        return first_error + opcode();
    }

    static uint8_t opcode(const cppual::damage::extension & extension)
    {
        return opcode(extension->first_error);
    }

    static string description(void)
    {
        return string("XCB_DAMAGE_BAD_DAMAGE");
    }

protected:
    uint8_t _M_first_error;
}; // class bad_damage
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
        SIGNATURE(xcb_damage_query_version_reply),
CookieFunction>
{
    public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
            Connection,
            Check,
            SIGNATURE(xcb_damage_query_version_reply),
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
SIGNATURE(xcb_damage_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
Connection, cppual::generic::unchecked_tag,
SIGNATURE(xcb_damage_query_version_unchecked)>;
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
create_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::damage::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_damage_create_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create(Parameter && ... parameter)
{
    ::xcb_damage_create(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
destroy_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::damage::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_damage_destroy_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
destroy(Parameter && ... parameter)
{
    ::xcb_damage_destroy(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
subtract_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::damage::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_damage_subtract_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
subtract(Parameter && ... parameter)
{
    ::xcb_damage_subtract(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
add_checked(Connection && c, Parameter && ... parameter)
{
    cppual::generic::check<Connection, cppual::damage::error::dispatcher>(
                std::forward<Connection>(c),
                ::xcb_damage_add_checked(
                    std::forward<Connection>(c),
                    std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
add(Parameter && ... parameter)
{
    ::xcb_damage_add(std::forward<Parameter>(parameter) ...);
}



























template<typename Derived, typename Connection>
class damage
{
protected:
    Connection
    connection(void) const
    {
        return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_damage_damage_t &
    resource(void) const
    {
        return static_cast<const Derived *>(this)->resource();
    }

public:
    virtual ~damage(void) {}

    template<typename ... Parameter>
    void
    create_checked(Parameter && ... parameter) const
    {
        cppual::damage::create_checked(connection(),
                                    resource(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
        cppual::damage::create(connection(),
                            resource(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::damage::destroy_checked(connection(),
                                     resource(),
                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::damage::destroy(connection(),
                             resource(),
                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    subtract_checked(Parameter && ... parameter) const
    {
        cppual::damage::subtract_checked(connection(),
                                      resource(),
                                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    subtract(Parameter && ... parameter) const
    {
        cppual::damage::subtract(connection(),
                              resource(),
                              std::forward<Parameter>(parameter) ...);
    }



}; // class damage








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
    typedef cppual::damage::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    damage(void)
    {
        return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
        return cppual::damage::query_version(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
        return cppual::damage::query_version_unchecked(
                    connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_checked(Parameter && ... parameter) const
    {
        cppual::damage::create_checked(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
        cppual::damage::create(connection(),
                            std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    destroy_checked(Parameter && ... parameter) const
    {
        cppual::damage::destroy_checked(connection(),
                                     std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    destroy(Parameter && ... parameter) const
    {
        cppual::damage::destroy(connection(),
                             std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    subtract_checked(Parameter && ... parameter) const
    {
        cppual::damage::subtract_checked(connection(),
                                      std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    subtract(Parameter && ... parameter) const
    {
        cppual::damage::subtract(connection(),
                              std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    add_checked(Parameter && ... parameter) const
    {
        cppual::damage::add_checked(connection(),
                                 std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    add(Parameter && ... parameter) const
    {
        cppual::damage::add(connection(),
                         std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
public:
    typedef cppual::damage::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
        : _M_c(std::forward<C>(c))
        , _M_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::damage::extension & extension)
        : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler handler,
               const std::shared_ptr<xcb_generic_event_t> & event) const
    {
        switch ((event->response_type & ~0x80) - _M_first_event) {

        case XCB_DAMAGE_NOTIFY:
            handler(cppual::damage::event::notify<Connection>(_M_c, _M_first_event, event));
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
    typedef cppual::damage::extension extension;

    dispatcher(uint8_t first_error)
        : _M_first_error(first_error)
    {}

    dispatcher(const cppual::damage::extension & extension)
        : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        switch (error->error_code - _M_first_error) {

        case XCB_DAMAGE_BAD_DAMAGE: // 0
            throw cppual::damage::error::bad_damage(error);

        };
    }

protected:
    uint8_t _M_first_error;
}; // class dispatcher

} // namespace error


} } // namespace cppual::damage

#endif // CPPUAL_DAMAGE_HPP
