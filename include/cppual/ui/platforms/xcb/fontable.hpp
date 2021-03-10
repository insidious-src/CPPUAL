#ifndef CPPUAL_FONTABLE_HPP
#define CPPUAL_FONTABLE_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class fontable
        : public cppual::generic::resource<Connection, ::xcb_fontable_t,
        cppual::x::fontable, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_fontable_t,
    cppual::x::fontable, Interfaces ...>;

public:
    using base::base;
    using base::operator=;
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::fontable<Connection, Interfaces ...>>
{
    typedef ::xcb_fontable_t type;
};

} // namespace generic

} // namespace cppual

#endif // CPPUAL_FONTABLE_HPP
