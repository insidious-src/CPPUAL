#ifndef CPPUAL_FONT_HPP
#define CPPUAL_FONT_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class font
        : public cppual::generic::resource<Connection, ::xcb_font_t,
        cppual::x::font, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_font_t,
    cppual::x::font, Interfaces ...>;

    template<typename C, typename Create, typename Destroy>
    font(C && c, Create && create, Destroy && destroy)
        : base(base::make(std::forward<C>(c),
                          std::forward<Create>(create),
                          std::forward<Destroy>(destroy)))
    {}

public:
    using base::base;
    using base::operator=;

    template<typename C>
    static
    font<Connection, Interfaces ...>
    open(C && c, const std::string & name) noexcept
    {
        return font(std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_font_t & font)
        {
            cppual::x::open_font(c, font, name);
        },
        [&](const Connection & c, const ::xcb_font_t & font)
        {
            cppual::x::close_font(c, font);
        });
    }

    template<typename C>
    static
    font<Connection, Interfaces ...>
    open_checked(C && c, const std::string & name)
    {
        return font(std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_font_t & font)
        {
            cppual::x::open_font_checked(c, font, name);
        },
        [&](const Connection & c, const ::xcb_font_t & font)
        {
            cppual::x::close_font_checked(c, font);
        });
    }
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::font<Connection, Interfaces ...>>
{
    typedef ::xcb_font_t type;
};

} // namespace generic

} // namespace cppual

#endif // CPPUAL_FONT_HPP
