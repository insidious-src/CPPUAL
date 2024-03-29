#ifndef CPPUAL_GENERIC_ERROR_HPP
#define CPPUAL_GENERIC_ERROR_HPP

#include <cppual/string.h>

#include <iostream> // shared_ptr
#include <memory> // shared_ptr
#include <xcb/xcb.h> // ::xcb_generic_error_t

namespace cppual { namespace generic {

class error_dispatcher {
public:
    virtual
    void operator()(const std::shared_ptr<xcb_generic_error_t> &) const = 0;
};

namespace detail {

template<typename Object>
void
dispatch(const Object & object,
         const std::shared_ptr<xcb_generic_error_t> & error,
         std::true_type)
{
    static_cast<const cppual::generic::error_dispatcher &>(object)(error);
}

template<typename Object>
void
dispatch(const Object &,
         const std::shared_ptr<xcb_generic_error_t> & error,
         std::false_type)
{
    throw error;
}

} // namespace detail

template<typename Object>
void
dispatch(const Object & object,
         const std::shared_ptr<xcb_generic_error_t> & error)
{
    detail::dispatch(object,
                     error,
                     std::is_base_of<cppual::generic::error_dispatcher, Object>());
}

template<typename Derived, typename Error>
class error
        : public std::runtime_error
{
public:
    error(const std::shared_ptr<xcb_generic_error_t> & error)
        : runtime_error(get_error_description(error.get()))
        , _M_error(error)
    {}

    virtual
    ~error(void)
    {}

    virtual
    operator const Error &(void) const
    {
        return reinterpret_cast<const Error &>(*_M_error);
    }

    virtual
    const Error &
    operator*(void) const
    {
        return reinterpret_cast<const Error &>(*_M_error);
    }

    virtual
    Error *
    operator->(void) const
    {
        return reinterpret_cast<Error * const>(_M_error.get());
    }

protected:
    virtual
    std::string
    get_error_description(xcb_generic_error_t * error) const
    {
        return std::string(Derived::description()) + " (" + std::to_string(error->error_code) + ")";
    }

    std::shared_ptr<xcb_generic_error_t> _M_error;
}; // class error

} } // xpp::generic

#endif // CPPUAL_GENERIC_ERROR_HPP
