#ifndef CPPUAL_PIXMAP_HPP
#define CPPUAL_PIXMAP_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class pixmap
        : public cppual::generic::resource<Connection, ::xcb_pixmap_t,
        cppual::x::pixmap, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_pixmap_t,
    cppual::x::pixmap, Interfaces ...>;

    template<typename C, typename Create, typename Destroy>
    pixmap(C && c, Create && create, Destroy && destroy)
        : base(base::make(std::forward<C>(c),
                          std::forward<Create>(create),
                          std::forward<Destroy>(destroy)))
    {}

public:
    using base::base;
    using base::operator=;

    template<typename C>
    static
    pixmap<Connection, Interfaces ...>
    create(C && c, uint8_t depth, ::xcb_drawable_t drawable,
           uint16_t width, uint16_t height)
    {
        return pixmap(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_pixmap_t & pixmap)
        {
            cppual::x::create_pixmap(c, depth, pixmap, drawable, width, height);
        },
        [&](const Connection & c, const ::xcb_pixmap_t & pixmap)
        {
            cppual::x::free_pixmap(c, pixmap);
        });
    }

    template<typename C>
    static
    pixmap<Connection, Interfaces ...>
    create_checked(C && c, uint8_t depth, ::xcb_drawable_t drawable,
                   uint16_t width, uint16_t height)
    {
        return pixmap(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_pixmap_t & pixmap)
        {
            cppual::x::create_pixmap_checked(c, depth, pixmap, drawable, width, height);
        },
        [&](const Connection & c, const ::xcb_pixmap_t & pixmap)
        {
            cppual::x::free_pixmap_checked(c, pixmap);
        });
    }
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::pixmap<Connection, Interfaces ...>>
{
    typedef ::xcb_pixmap_t type;
};

} // namespace generic

} // namespace xpp

#endif // CPPUAL_PIXMAP_HPP
