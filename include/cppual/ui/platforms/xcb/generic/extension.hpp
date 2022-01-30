#ifndef CPPUAL_GENERIC_EXTENSION_HPP
#define CPPUAL_GENERIC_EXTENSION_HPP

// #include <iostream>
#include <xcb/xcb.h>

namespace cppual { namespace generic {

template<typename Derived, ::xcb_extension_t * Id>
class extension
{
public:
    extension(xcb_connection_t * const c)
        : _M_c(c)
    {
        prefetch();
    }

    const ::xcb_query_extension_reply_t &
    operator*(void) const
    {
        return *_M_extension;
    }

    const ::xcb_query_extension_reply_t *
    operator->(void) const
    {
        return _M_extension;
    }

    operator const ::xcb_query_extension_reply_t *(void) const
    {
        return _M_extension;
    }

    Derived &
    get(void)
    {
        _M_extension = ::xcb_get_extension_data(_M_c, Id);
        return static_cast<Derived &>(*this);
    }

    Derived &
    prefetch(void)
    {
        ::xcb_prefetch_extension_data(_M_c, Id);
        return static_cast<Derived &>(*this);
    }

private:
    ::xcb_connection_t * _M_c = nullptr;
    // The result must not be freed.
    // This storage is managed by the cache itself.
    const ::xcb_query_extension_reply_t * _M_extension = nullptr;
}; // class extension

} } // namespace cppual::generic

#endif // CPPUAL_GENERIC_EXTENSION_HPP
