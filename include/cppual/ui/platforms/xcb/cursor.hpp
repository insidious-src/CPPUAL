#ifndef CPPUAL_CURSOR_HPP
#define CPPUAL_CURSOR_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class cursor
        : public cppual::generic::resource<Connection, ::xcb_cursor_t,
        cppual::x::cursor, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_cursor_t,
    cppual::x::cursor, Interfaces ...>;

    template<typename C, typename Create, typename Destroy>
    cursor(C && c, Create && create, Destroy && destroy)
        : base(base::make(std::forward<C>(c),
                          std::forward<Create>(create),
                          std::forward<Destroy>(destroy)))
    {}

public:
    using base::base;
    using base::operator=;

    template<typename C>
    static
    cursor<Connection, Interfaces ...>
    create(C && c,
           ::xcb_pixmap_t source, ::xcb_pixmap_t mask,
           uint16_t fore_red, uint16_t fore_green, uint16_t fore_blue,
           uint16_t back_red, uint16_t back_green, uint16_t back_blue,
           uint16_t x, uint16_t y)
    {
        return cursor(
                    std::forward<C>(c),
                    [&](Connection & c, const ::xcb_cursor_t & cursor)
        {
            cppual::x::create_cursor(c, cursor,
                                     source, mask,
                                     fore_red, fore_green, fore_blue,
                                     back_red, back_green, back_blue,
                                     x, y);
        },
        [&](Connection & c, const ::xcb_cursor_t & cursor)
        {
        cppual::x::free_cursor(c, cursor);
    });
}

template<typename C>
static
cursor<Connection, Interfaces ...>
create_checked(C && c,
               ::xcb_pixmap_t source, ::xcb_pixmap_t mask,
               uint16_t fore_red, uint16_t fore_green, uint16_t fore_blue,
               uint16_t back_red, uint16_t back_green, uint16_t back_blue,
               uint16_t x, uint16_t y)
{
    return cursor(
                std::forward<C>(c),
                [&](Connection & c, const ::xcb_cursor_t & cursor)
    {
        cppual::x::create_cursor_checked(c, cursor,
                                         source, mask,
                                         fore_red, fore_green, fore_blue,
                                         back_red, back_green, back_blue,
                                         x, y);
    },
    [&](Connection & c, const ::xcb_cursor_t & cursor)
    {
    cppual::x::free_cursor_checked(c, cursor);
});
}

template<typename C>
static
cursor<Connection, Interfaces ...>
create_glyph(C && c,
             ::xcb_font_t source_font, ::xcb_font_t mask_font,
             uint16_t source_char, uint16_t mask_char,
             uint16_t fore_red, uint16_t fore_green, uint16_t fore_blue,
             uint16_t back_red, uint16_t back_green, uint16_t back_blue)
{
    return cursor(
                std::forward<C>(c),
                [&](Connection & c, const ::xcb_cursor_t & cursor)
    {
        cppual::x::create_glyph_cursor(c, cursor,
                                       source_font, mask_font,
                                       source_char, mask_char,
                                       fore_red, fore_green, fore_blue,
                                       back_red, back_green, back_blue);
    },
    [](Connection & c, const ::xcb_cursor_t & cursor)
    {
    cppual::x::free_cursor(c, cursor);
});
}

template<typename C>
static
cursor<Connection, Interfaces ...>
create_glyph_checked(C && c,
                     ::xcb_font_t source_font, ::xcb_font_t mask_font,
                     uint16_t source_char, uint16_t mask_char,
                     uint16_t fore_red, uint16_t fore_green, uint16_t fore_blue,
                     uint16_t back_red, uint16_t back_green, uint16_t back_blue)
{
    return cursor(
                std::forward<C>(c),
                [&](Connection & c, const ::xcb_cursor_t & cursor)
    {
        cppual::x::create_glyph_cursor_checked(c, cursor,
                                               source_font, mask_font,
                                               source_char, mask_char,
                                               fore_red, fore_green, fore_blue,
                                               back_red, back_green, back_blue);
    },
    [](Connection & c, const ::xcb_cursor_t & cursor)
    {
    cppual::x::free_cursor_checked(c, cursor);
});
}
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::cursor<Connection, Interfaces ...>>
{
    typedef ::xcb_cursor_t type;
};

} // namespace generic

} // namespace xpp

#endif // CPPUAL_CURSOR_HPP
