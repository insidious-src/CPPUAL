#ifndef CPPUAL_COLORMAP_HPP
#define CPPUAL_COLORMAP_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class colormap
        : public cppual::generic::resource<Connection, ::xcb_colormap_t,
        cppual::x::colormap, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_colormap_t,
    cppual::x::colormap, Interfaces ...>;

    template<typename C, typename Create, typename Destroy>
    colormap(C && c, Create && create, Destroy && destroy)
        : base(base::make(std::forward<C>(c),
                          std::forward<Create>(create),
                          std::forward<Destroy>(destroy)))
    {}

public:
    using base::base;
    using base::operator=;

    template<typename C>
    static
    colormap<Connection, Interfaces ...>
    create(C && c, uint8_t alloc, ::xcb_window_t window, ::xcb_visualid_t visual)
    {
        return colormap(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_colormap_t & colormap)
        {
            cppual::x::create_colormap(c, alloc, colormap, window, visual);
        },
        [&](const Connection & c, const ::xcb_colormap_t & colormap)
        {
            cppual::x::free_colormap(c, colormap);
        });
    }

    template<typename C>
    static
    colormap<Connection, Interfaces ...>
    create_checked(C && c, uint8_t alloc,
                   ::xcb_window_t window, ::xcb_visualid_t visual)
    {
        return colormap(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_colormap_t & colormap)
        {
            cppual::x::create_colormap_checked(c, alloc, colormap, window, visual);
        },
        [&](const Connection & c, const ::xcb_colormap_t & colormap)
        {
            cppual::x::free_colormap_checked(c, colormap);
        });
    }
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::colormap<Connection, Interfaces ...>>
{
    typedef ::xcb_colormap_t type;
};

} // namespace generic

} // namespace xpp

#endif // CPPUAL_COLORMAP_HPP
