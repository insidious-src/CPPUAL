#ifndef CPPUAL_CONNECTION_HPP
#define CPPUAL_CONNECTION_HPP

#include <cppual/ui/platforms/xcb/core.hpp>
#include <cppual/ui/platforms/xcb/generic/factory.hpp>

#include <cppual/ui/platforms/xcb/proto/x.hpp>

namespace cppual {

namespace detail {

template<typename Connection, typename ... Extensions>
class interfaces
        : public cppual::x::extension::interface<interfaces<Connection, Extensions ...>, Connection>
        , public Extensions::template interface<interfaces<Connection, Extensions ...>, Connection> ...
{
public:
    Connection
    connection(void) const
    {
        return static_cast<const Connection &>(*this);
    }
}; // class interfaces

} // namespace detail

template<typename ... Extensions>
class connection
        : public cppual::core
        , public cppual::generic::error_dispatcher
        , public detail::interfaces<connection<Extensions ...>, Extensions ...>
        // private interfaces: extensions and error_dispatcher
        , private cppual::x::extension
        , private cppual::x::extension::error_dispatcher
        , private Extensions ...
        , private Extensions::error_dispatcher ...
{
protected:
    typedef connection<Extensions ...> self;


public:
    template<typename ... Parameters>
    explicit
    connection(Parameters && ... parameters)
        : cppual::core::core(std::forward<Parameters>(parameters) ...)
        , detail::interfaces<connection<Extensions ...>, Extensions ...>(*this)
        , Extensions(static_cast<xcb_connection_t *>(*this)) ...
        , Extensions::error_dispatcher(static_cast<Extensions &>(*this).get()) ...
    {
        _M_root_window = screen_of_display(default_screen())->root;
    }

    virtual
    ~connection(void)
    {}

    virtual
    operator ::xcb_connection_t *(void) const
    {
        return *(static_cast<const core &>(*this));
    }

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        check<cppual::x::extension, Extensions ...>(error);
    }

    template<typename Extension>
    const Extension &
    extension(void) const
    {
        return static_cast<const Extension &>(*this);
    }

    // TODO
    // virtual operator Display * const(void) const
    // {
    // }

    template<typename Window = ::xcb_window_t>
    Window
    root(void)
    {
        using make = cppual::generic::factory::make<self, ::xcb_window_t, Window>;
        return make()(*this, _M_root_window);
    }

    template<typename Window = ::xcb_window_t>
    Window
    root(void) const
    {
        using make = cppual::generic::factory::make<self, ::xcb_window_t, Window>;
        return make()(*this, _M_root_window);
    }

    virtual
    shared_generic_event_ptr
    wait_for_event(void) const
    {
        try {
            return core::wait_for_event();
        } catch (const std::shared_ptr<xcb_generic_error_t> & error) {
            check<cppual::x::extension, Extensions ...>(error);
        }
        // re-throw any exception caused by wait_for_event
        throw;
    }

    virtual
    shared_generic_event_ptr
    wait_for_special_event(xcb_special_event_t * se) const
    {
        try {
            return core::wait_for_special_event(se);
        } catch (const std::shared_ptr<xcb_generic_error_t> & error) {
            check<cppual::x::extension, Extensions ...>(error);
        }
        // re-throw any exception caused by wait_for_special_event
        throw;
    }

private:
    ::xcb_window_t _M_root_window;

    template<typename Extension, typename Next, typename ... Rest>
    void
    check(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        check<Extension>(error);
        check<Next, Rest ...>(error);
    }

    template<typename Extension>
    void
    check(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
        using error_dispatcher = typename Extension::error_dispatcher;
        auto & dispatcher = static_cast<const error_dispatcher &>(*this);
        dispatcher(error);
    }
}; // class connection

template<>
template<typename ... Parameters>
connection<>::connection(Parameters && ... parameters)
    : cppual::core::core(std::forward<Parameters>(parameters) ...)
    , detail::interfaces<connection<>>(*this)
{
    _M_root_window = screen_of_display(static_cast<core &>(*this).default_screen())->root;
}

} // namespace cppual

#endif // CPPUAL_CONNECTION_HPP
