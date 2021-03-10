#ifndef CPPUAL_DRAWABLE_HPP
#define CPPUAL_DRAWABLE_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class drawable
        : public cppual::generic::resource<Connection, ::xcb_drawable_t,
        cppual::x::drawable, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_drawable_t,
    cppual::x::drawable, Interfaces ...>;

public:
    using base::base;
    using base::operator=;
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::drawable<Connection, Interfaces ...>>
{
    typedef ::xcb_drawable_t type;
};

} // namespace generic

} // namespace cppual

#endif // CPPUAL_DRAWABLE_HPP
