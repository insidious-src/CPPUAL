#ifndef CPPUAL_ATOM_HPP
#define CPPUAL_ATOM_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class atom
        : public cppual::generic::resource<Connection, ::xcb_atom_t,
        cppual::x::atom, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_atom_t, Interfaces ...>;

public:
    using base::base;
    using base::operator=;
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::atom<Connection, Interfaces ...>>
{
    typedef ::xcb_atom_t type;
};

} // namespace generic

} // namespace xpp

#endif // CPPUAL_ATOM_HPP
