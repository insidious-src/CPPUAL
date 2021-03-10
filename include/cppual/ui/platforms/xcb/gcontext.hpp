#ifndef CPPUAL_GCONTEXT_HPP
#define CPPUAL_GCONTEXT_HPP

#include <cppual/ui/platforms/xcb/proto/x.hpp>
#include <cppual/ui/platforms/xcb/generic/resource.hpp>

namespace cppual {

template<typename Connection, template<typename, typename> class ... Interfaces>
class gcontext
        : public cppual::generic::resource<Connection, ::xcb_gcontext_t,
        cppual::x::gcontext, Interfaces ...>
{
protected:
    using base = cppual::generic::resource<Connection, ::xcb_gcontext_t,
    cppual::x::gcontext, Interfaces ...>;

    template<typename C, typename Create, typename Destroy>
    gcontext(C && c, Create && create, Destroy && destroy)
        : base(base::make(std::forward<C>(c),
                          std::forward<Create>(create),
                          std::forward<Destroy>(destroy)))
    {}

public:
    using base::base;
    using base::operator=;

    template<typename C>
    static
    gcontext<Connection, Interfaces ...>
    create(C && c, ::xcb_drawable_t drawable,
           uint32_t value_mask, const uint32_t * value_list)
    {
        return gcontext(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::create_gc(c, gcontext, drawable, value_mask, value_list);
        },
        [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::free_gc(c, gcontext);
        });
    }

    template<typename C>
    static
    gcontext<Connection, Interfaces ...>
    create_checked(C && c, ::xcb_drawable_t drawable,
                   uint32_t value_mask, const uint32_t * value_list)
    {
        return gcontext(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::create_gc_checked(c, gcontext, drawable,
                                         value_mask, value_list);
        },
        [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::free_gc_checked(c, gcontext);
        });
    }

    template<typename C>
    static
    gcontext<Connection, Interfaces ...>
    copy(C && c, ::xcb_gcontext_t src_gc, uint32_t value_mask)
    {
        return gcontext(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::copy_gc(c, src_gc, gcontext, value_mask);
        },
        [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::free_gc(c, gcontext);
        });
    }


    template<typename C>
    static
    gcontext<Connection, Interfaces ...>
    copy_checked(C && c, ::xcb_gcontext_t src_gc, uint32_t value_mask)
    {
        return gcontext(
                    std::forward<C>(c),
                    [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::copy_gc_checked(c, src_gc, gcontext, value_mask);
        },
        [&](const Connection & c, const ::xcb_gcontext_t & gcontext)
        {
            cppual::x::free_gc_checked(c, gcontext);
        });
    }
};

namespace generic {

template<typename Connection, template<typename, typename> class ... Interfaces>
struct traits<cppual::gcontext<Connection, Interfaces ...>>
{
    typedef ::xcb_gcontext_t type;
};

} // namespace generic

} // namespace cppual

#endif // CPPUAL_GCONTEXT_HPP
