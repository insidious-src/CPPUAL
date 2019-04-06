#ifndef CPPUAL_RENDER_HPP
#define CPPUAL_RENDER_HPP

#include <cppual/ui/platforms/xcb/generic.hpp>

#include <string>
#include <vector>

#include <xcb/render.h>

namespace cppual { namespace render {

template<typename Derived, typename Connection>
class interface;

namespace event { template<typename Connection> class dispatcher; }
namespace error { class dispatcher; }

class extension
  : public cppual::generic::extension<extension, &xcb_render_id>
{
  public:
    using base = cppual::generic::extension<extension, &xcb_render_id>;
    using base::base;
    template<typename Derived, typename Connection>
    using interface = cppual::render::interface<Derived, Connection>;
    template<typename Connection>
    using event_dispatcher = cppual::render::event::dispatcher<Connection>;
    using error_dispatcher = cppual::render::error::dispatcher;
};


namespace error {
class pict_format
  : public cppual::generic::error<pict_format,
                               ::xcb_render_pict_format_error_t>
{
  public:
    using cppual::generic::error<pict_format, ::xcb_render_pict_format_error_t>::error;

    virtual ~pict_format(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RENDER_PICT_FORMAT;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::render::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RENDER_PICT_FORMAT");
    }

  protected:
    uint8_t m_first_error;
}; // class pict_format
} // namespace error


namespace error {
class picture
  : public cppual::generic::error<picture,
                               ::xcb_render_picture_error_t>
{
  public:
    using cppual::generic::error<picture, ::xcb_render_picture_error_t>::error;

    virtual ~picture(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RENDER_PICTURE;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::render::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RENDER_PICTURE");
    }

  protected:
    uint8_t m_first_error;
}; // class picture
} // namespace error


namespace error {
class pict_op
  : public cppual::generic::error<pict_op,
                               ::xcb_render_pict_op_error_t>
{
  public:
    using cppual::generic::error<pict_op, ::xcb_render_pict_op_error_t>::error;

    virtual ~pict_op(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RENDER_PICT_OP;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::render::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RENDER_PICT_OP");
    }

  protected:
    uint8_t m_first_error;
}; // class pict_op
} // namespace error


namespace error {
class glyph_set
  : public cppual::generic::error<glyph_set,
                               ::xcb_render_glyph_set_error_t>
{
  public:
    using cppual::generic::error<glyph_set, ::xcb_render_glyph_set_error_t>::error;

    virtual ~glyph_set(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RENDER_GLYPH_SET;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::render::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RENDER_GLYPH_SET");
    }

  protected:
    uint8_t m_first_error;
}; // class glyph_set
} // namespace error


namespace error {
class glyph
  : public cppual::generic::error<glyph,
                               ::xcb_render_glyph_error_t>
{
  public:
    using cppual::generic::error<glyph, ::xcb_render_glyph_error_t>::error;

    virtual ~glyph(void) {}

    static uint8_t opcode(void)
    {
      return XCB_RENDER_GLYPH;
    }

    static uint8_t opcode(uint8_t first_error)
    {
      return first_error + opcode();
    }

    static uint8_t opcode(const cppual::render::extension & extension)
    {
      return opcode(extension->first_error);
    }

    static std::string description(void)
    {
      return std::string("XCB_RENDER_GLYPH");
    }

  protected:
    uint8_t m_first_error;
}; // class glyph
} // namespace error


namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_version
  : public cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_render_query_version_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_version<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_render_query_version_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_version(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}

}; // class query_version

} // namespace detail

namespace checked {
template<typename Connection>
using query_version = detail::query_version<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_render_query_version)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_version = detail::query_version<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_render_query_version_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_version<Connection>
query_version(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_version<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_version<Connection>
query_version_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_version<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_pict_formats
  : public cppual::generic::reply<query_pict_formats<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_render_query_pict_formats_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_pict_formats<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_render_query_pict_formats_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_pict_formats(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_render_query_pict_formats_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_render_pictforminfo_t,
                                              SIGNATURE(xcb_render_query_pict_formats_formats),
                                              SIGNATURE(xcb_render_query_pict_formats_formats_length)>
                      >
    formats(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_render_query_pict_formats_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_render_pictforminfo_t,
                                                       SIGNATURE(xcb_render_query_pict_formats_formats),
                                                       SIGNATURE(xcb_render_query_pict_formats_formats_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_render_query_pict_formats_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_render_pictscreen_t,
                                              SIGNATURE(xcb_render_pictscreen_next),
                                              SIGNATURE(xcb_render_pictscreen_sizeof),
                                              SIGNATURE(xcb_render_query_pict_formats_screens_iterator)>
                      >
    screens(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_render_query_pict_formats_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_render_pictscreen_t,
                                                       SIGNATURE(xcb_render_pictscreen_next),
                                                       SIGNATURE(xcb_render_pictscreen_sizeof),
                                                       SIGNATURE(xcb_render_query_pict_formats_screens_iterator)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_render_query_pict_formats_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint32_t,
                                              SIGNATURE(xcb_render_query_pict_formats_subpixels),
                                              SIGNATURE(xcb_render_query_pict_formats_subpixels_length)>
                      >
    subpixels(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_render_query_pict_formats_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint32_t,
                                                       SIGNATURE(xcb_render_query_pict_formats_subpixels),
                                                       SIGNATURE(xcb_render_query_pict_formats_subpixels_length)>
                               >(this->m_c, this->get());
    }
}; // class query_pict_formats

} // namespace detail

namespace checked {
template<typename Connection>
using query_pict_formats = detail::query_pict_formats<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_render_query_pict_formats)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_pict_formats = detail::query_pict_formats<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_render_query_pict_formats_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_pict_formats<Connection>
query_pict_formats(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_pict_formats<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_pict_formats<Connection>
query_pict_formats_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_pict_formats<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_pict_index_values
  : public cppual::generic::reply<query_pict_index_values<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_render_query_pict_index_values_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_pict_index_values<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_render_query_pict_index_values_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_pict_index_values(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_render_query_pict_index_values_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_render_indexvalue_t,
                                              SIGNATURE(xcb_render_query_pict_index_values_values),
                                              SIGNATURE(xcb_render_query_pict_index_values_values_length)>
                      >
    values(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_render_query_pict_index_values_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_render_indexvalue_t,
                                                       SIGNATURE(xcb_render_query_pict_index_values_values),
                                                       SIGNATURE(xcb_render_query_pict_index_values_values_length)>
                               >(this->m_c, this->get());
    }
}; // class query_pict_index_values

} // namespace detail

namespace checked {
template<typename Connection>
using query_pict_index_values = detail::query_pict_index_values<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_render_query_pict_index_values)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_pict_index_values = detail::query_pict_index_values<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_render_query_pict_index_values_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_pict_index_values<Connection>
query_pict_index_values(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_pict_index_values<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_pict_index_values<Connection>
query_pict_index_values_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_pict_index_values<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_picture_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_picture_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_picture(Parameter && ... parameter)
{
  ::xcb_render_create_picture(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
change_picture_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_change_picture_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
change_picture(Parameter && ... parameter)
{
  ::xcb_render_change_picture(std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_picture_clip_rectangles_checked(Connection && c, ::xcb_render_picture_t picture, int16_t clip_x_origin, int16_t clip_y_origin, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_set_picture_clip_rectangles_checked(std::forward<Connection>(c), picture, clip_x_origin, clip_y_origin, rectangles_len, rectangles));
}

template<typename Connection>
void
set_picture_clip_rectangles(Connection && c, ::xcb_render_picture_t picture, int16_t clip_x_origin, int16_t clip_y_origin, uint32_t rectangles_len, const ::xcb_rectangle_t * rectangles)
{
  ::xcb_render_set_picture_clip_rectangles(std::forward<Connection>(c), picture, clip_x_origin, clip_y_origin, rectangles_len, rectangles);
}

template<typename Connection, typename Rectangles_Iterator>
void
set_picture_clip_rectangles_checked(Connection && c, ::xcb_render_picture_t picture, int16_t clip_x_origin, int16_t clip_y_origin, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
      typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> rectangles =
        { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_set_picture_clip_rectangles_checked(std::forward<Connection>(c), picture, clip_x_origin, clip_y_origin, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data())));
}

template<typename Connection, typename Rectangles_Iterator>
void
set_picture_clip_rectangles(Connection && c, ::xcb_render_picture_t picture, int16_t clip_x_origin, int16_t clip_y_origin, Rectangles_Iterator rectangles_begin, Rectangles_Iterator rectangles_end)
{
      typedef typename value_type<Rectangles_Iterator, ! std::is_pointer<Rectangles_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> rectangles =
        { value_iterator<Rectangles_Iterator>(rectangles_begin), value_iterator<Rectangles_Iterator>(rectangles_end) };

  ::xcb_render_set_picture_clip_rectangles(std::forward<Connection>(c), picture, clip_x_origin, clip_y_origin, static_cast<uint32_t>(rectangles.size()), const_cast<const vector_type *>(rectangles.data()));
}

template<typename Connection, typename ... Parameter>
void
free_picture_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_free_picture_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
free_picture(Parameter && ... parameter)
{
  ::xcb_render_free_picture(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
composite_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_composite_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
composite(Parameter && ... parameter)
{
  ::xcb_render_composite(std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
trapezoids_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t traps_len, const ::xcb_render_trapezoid_t * traps)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_trapezoids_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, traps_len, traps));
}

template<typename Connection>
void
trapezoids(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t traps_len, const ::xcb_render_trapezoid_t * traps)
{
  ::xcb_render_trapezoids(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, traps_len, traps);
}

template<typename Connection, typename Traps_Iterator>
void
trapezoids_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Traps_Iterator traps_begin, Traps_Iterator traps_end)
{
      typedef typename value_type<Traps_Iterator, ! std::is_pointer<Traps_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> traps =
        { value_iterator<Traps_Iterator>(traps_begin), value_iterator<Traps_Iterator>(traps_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_trapezoids_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(traps.size()), const_cast<const vector_type *>(traps.data())));
}

template<typename Connection, typename Traps_Iterator>
void
trapezoids(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Traps_Iterator traps_begin, Traps_Iterator traps_end)
{
      typedef typename value_type<Traps_Iterator, ! std::is_pointer<Traps_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> traps =
        { value_iterator<Traps_Iterator>(traps_begin), value_iterator<Traps_Iterator>(traps_end) };

  ::xcb_render_trapezoids(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(traps.size()), const_cast<const vector_type *>(traps.data()));
}

template<typename Connection>
void
triangles_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t triangles_len, const ::xcb_render_triangle_t * triangles)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_triangles_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, triangles_len, triangles));
}

template<typename Connection>
void
triangles(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t triangles_len, const ::xcb_render_triangle_t * triangles)
{
  ::xcb_render_triangles(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, triangles_len, triangles);
}

template<typename Connection, typename Triangles_Iterator>
void
triangles_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Triangles_Iterator triangles_begin, Triangles_Iterator triangles_end)
{
      typedef typename value_type<Triangles_Iterator, ! std::is_pointer<Triangles_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> triangles =
        { value_iterator<Triangles_Iterator>(triangles_begin), value_iterator<Triangles_Iterator>(triangles_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_triangles_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(triangles.size()), const_cast<const vector_type *>(triangles.data())));
}

template<typename Connection, typename Triangles_Iterator>
void
triangles(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Triangles_Iterator triangles_begin, Triangles_Iterator triangles_end)
{
      typedef typename value_type<Triangles_Iterator, ! std::is_pointer<Triangles_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> triangles =
        { value_iterator<Triangles_Iterator>(triangles_begin), value_iterator<Triangles_Iterator>(triangles_end) };

  ::xcb_render_triangles(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(triangles.size()), const_cast<const vector_type *>(triangles.data()));
}

template<typename Connection>
void
tri_strip_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t points_len, const ::xcb_render_pointfix_t * points)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_tri_strip_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, points_len, points));
}

template<typename Connection>
void
tri_strip(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t points_len, const ::xcb_render_pointfix_t * points)
{
  ::xcb_render_tri_strip(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, points_len, points);
}

template<typename Connection, typename Points_Iterator>
void
tri_strip_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Points_Iterator points_begin, Points_Iterator points_end)
{
      typedef typename value_type<Points_Iterator, ! std::is_pointer<Points_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> points =
        { value_iterator<Points_Iterator>(points_begin), value_iterator<Points_Iterator>(points_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_tri_strip_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(points.size()), const_cast<const vector_type *>(points.data())));
}

template<typename Connection, typename Points_Iterator>
void
tri_strip(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Points_Iterator points_begin, Points_Iterator points_end)
{
      typedef typename value_type<Points_Iterator, ! std::is_pointer<Points_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> points =
        { value_iterator<Points_Iterator>(points_begin), value_iterator<Points_Iterator>(points_end) };

  ::xcb_render_tri_strip(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(points.size()), const_cast<const vector_type *>(points.data()));
}

template<typename Connection>
void
tri_fan_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t points_len, const ::xcb_render_pointfix_t * points)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_tri_fan_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, points_len, points));
}

template<typename Connection>
void
tri_fan(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, uint32_t points_len, const ::xcb_render_pointfix_t * points)
{
  ::xcb_render_tri_fan(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, points_len, points);
}

template<typename Connection, typename Points_Iterator>
void
tri_fan_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Points_Iterator points_begin, Points_Iterator points_end)
{
      typedef typename value_type<Points_Iterator, ! std::is_pointer<Points_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> points =
        { value_iterator<Points_Iterator>(points_begin), value_iterator<Points_Iterator>(points_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_tri_fan_checked(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(points.size()), const_cast<const vector_type *>(points.data())));
}

template<typename Connection, typename Points_Iterator>
void
tri_fan(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, int16_t src_x, int16_t src_y, Points_Iterator points_begin, Points_Iterator points_end)
{
      typedef typename value_type<Points_Iterator, ! std::is_pointer<Points_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> points =
        { value_iterator<Points_Iterator>(points_begin), value_iterator<Points_Iterator>(points_end) };

  ::xcb_render_tri_fan(std::forward<Connection>(c), op, src, dst, mask_format, src_x, src_y, static_cast<uint32_t>(points.size()), const_cast<const vector_type *>(points.data()));
}

template<typename Connection, typename ... Parameter>
void
create_glyph_set_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_glyph_set_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_glyph_set(Parameter && ... parameter)
{
  ::xcb_render_create_glyph_set(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
reference_glyph_set_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_reference_glyph_set_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
reference_glyph_set(Parameter && ... parameter)
{
  ::xcb_render_reference_glyph_set(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
free_glyph_set_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_free_glyph_set_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
free_glyph_set(Parameter && ... parameter)
{
  ::xcb_render_free_glyph_set(std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
add_glyphs_checked(Connection && c, ::xcb_render_glyphset_t glyphset, uint32_t glyphs_len, const uint32_t * glyphids, const ::xcb_render_glyphinfo_t * glyphs, uint32_t data_len, const uint8_t * data)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_add_glyphs_checked(std::forward<Connection>(c), glyphset, glyphs_len, glyphids, glyphs, data_len, data));
}

template<typename Connection>
void
add_glyphs(Connection && c, ::xcb_render_glyphset_t glyphset, uint32_t glyphs_len, const uint32_t * glyphids, const ::xcb_render_glyphinfo_t * glyphs, uint32_t data_len, const uint8_t * data)
{
  ::xcb_render_add_glyphs(std::forward<Connection>(c), glyphset, glyphs_len, glyphids, glyphs, data_len, data);
}

template<typename Connection, typename Data_Iterator>
void
add_glyphs_checked(Connection && c, ::xcb_render_glyphset_t glyphset, uint32_t glyphs_len, const uint32_t * glyphids, const ::xcb_render_glyphinfo_t * glyphs, Data_Iterator data_begin, Data_Iterator data_end)
{
      typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> data =
        { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_add_glyphs_checked(std::forward<Connection>(c), glyphset, glyphs_len, glyphids, glyphs, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data())));
}

template<typename Connection, typename Data_Iterator>
void
add_glyphs(Connection && c, ::xcb_render_glyphset_t glyphset, uint32_t glyphs_len, const uint32_t * glyphids, const ::xcb_render_glyphinfo_t * glyphs, Data_Iterator data_begin, Data_Iterator data_end)
{
      typedef typename value_type<Data_Iterator, ! std::is_pointer<Data_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> data =
        { value_iterator<Data_Iterator>(data_begin), value_iterator<Data_Iterator>(data_end) };

  ::xcb_render_add_glyphs(std::forward<Connection>(c), glyphset, glyphs_len, glyphids, glyphs, static_cast<uint32_t>(data.size()), const_cast<const vector_type *>(data.data()));
}

template<typename Connection>
void
free_glyphs_checked(Connection && c, ::xcb_render_glyphset_t glyphset, uint32_t glyphs_len, const ::xcb_render_glyph_t * glyphs)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_free_glyphs_checked(std::forward<Connection>(c), glyphset, glyphs_len, glyphs));
}

template<typename Connection>
void
free_glyphs(Connection && c, ::xcb_render_glyphset_t glyphset, uint32_t glyphs_len, const ::xcb_render_glyph_t * glyphs)
{
  ::xcb_render_free_glyphs(std::forward<Connection>(c), glyphset, glyphs_len, glyphs);
}

template<typename Connection, typename Glyphs_Iterator>
void
free_glyphs_checked(Connection && c, ::xcb_render_glyphset_t glyphset, Glyphs_Iterator glyphs_begin, Glyphs_Iterator glyphs_end)
{
      typedef typename value_type<Glyphs_Iterator, ! std::is_pointer<Glyphs_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphs =
        { value_iterator<Glyphs_Iterator>(glyphs_begin), value_iterator<Glyphs_Iterator>(glyphs_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_free_glyphs_checked(std::forward<Connection>(c), glyphset, static_cast<uint32_t>(glyphs.size()), const_cast<const vector_type *>(glyphs.data())));
}

template<typename Connection, typename Glyphs_Iterator>
void
free_glyphs(Connection && c, ::xcb_render_glyphset_t glyphset, Glyphs_Iterator glyphs_begin, Glyphs_Iterator glyphs_end)
{
      typedef typename value_type<Glyphs_Iterator, ! std::is_pointer<Glyphs_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphs =
        { value_iterator<Glyphs_Iterator>(glyphs_begin), value_iterator<Glyphs_Iterator>(glyphs_end) };

  ::xcb_render_free_glyphs(std::forward<Connection>(c), glyphset, static_cast<uint32_t>(glyphs.size()), const_cast<const vector_type *>(glyphs.data()));
}

template<typename Connection>
void
composite_glyphs_8_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, uint32_t glyphcmds_len, const uint8_t * glyphcmds)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_composite_glyphs_8_checked(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, glyphcmds_len, glyphcmds));
}

template<typename Connection>
void
composite_glyphs_8(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, uint32_t glyphcmds_len, const uint8_t * glyphcmds)
{
  ::xcb_render_composite_glyphs_8(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, glyphcmds_len, glyphcmds);
}

template<typename Connection, typename Glyphcmds_Iterator>
void
composite_glyphs_8_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, Glyphcmds_Iterator glyphcmds_begin, Glyphcmds_Iterator glyphcmds_end)
{
      typedef typename value_type<Glyphcmds_Iterator, ! std::is_pointer<Glyphcmds_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphcmds =
        { value_iterator<Glyphcmds_Iterator>(glyphcmds_begin), value_iterator<Glyphcmds_Iterator>(glyphcmds_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_composite_glyphs_8_checked(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, static_cast<uint32_t>(glyphcmds.size()), const_cast<const vector_type *>(glyphcmds.data())));
}

template<typename Connection, typename Glyphcmds_Iterator>
void
composite_glyphs_8(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, Glyphcmds_Iterator glyphcmds_begin, Glyphcmds_Iterator glyphcmds_end)
{
      typedef typename value_type<Glyphcmds_Iterator, ! std::is_pointer<Glyphcmds_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphcmds =
        { value_iterator<Glyphcmds_Iterator>(glyphcmds_begin), value_iterator<Glyphcmds_Iterator>(glyphcmds_end) };

  ::xcb_render_composite_glyphs_8(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, static_cast<uint32_t>(glyphcmds.size()), const_cast<const vector_type *>(glyphcmds.data()));
}

template<typename Connection>
void
composite_glyphs_16_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, uint32_t glyphcmds_len, const uint8_t * glyphcmds)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_composite_glyphs_16_checked(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, glyphcmds_len, glyphcmds));
}

template<typename Connection>
void
composite_glyphs_16(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, uint32_t glyphcmds_len, const uint8_t * glyphcmds)
{
  ::xcb_render_composite_glyphs_16(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, glyphcmds_len, glyphcmds);
}

template<typename Connection, typename Glyphcmds_Iterator>
void
composite_glyphs_16_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, Glyphcmds_Iterator glyphcmds_begin, Glyphcmds_Iterator glyphcmds_end)
{
      typedef typename value_type<Glyphcmds_Iterator, ! std::is_pointer<Glyphcmds_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphcmds =
        { value_iterator<Glyphcmds_Iterator>(glyphcmds_begin), value_iterator<Glyphcmds_Iterator>(glyphcmds_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_composite_glyphs_16_checked(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, static_cast<uint32_t>(glyphcmds.size()), const_cast<const vector_type *>(glyphcmds.data())));
}

template<typename Connection, typename Glyphcmds_Iterator>
void
composite_glyphs_16(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, Glyphcmds_Iterator glyphcmds_begin, Glyphcmds_Iterator glyphcmds_end)
{
      typedef typename value_type<Glyphcmds_Iterator, ! std::is_pointer<Glyphcmds_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphcmds =
        { value_iterator<Glyphcmds_Iterator>(glyphcmds_begin), value_iterator<Glyphcmds_Iterator>(glyphcmds_end) };

  ::xcb_render_composite_glyphs_16(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, static_cast<uint32_t>(glyphcmds.size()), const_cast<const vector_type *>(glyphcmds.data()));
}

template<typename Connection>
void
composite_glyphs_32_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, uint32_t glyphcmds_len, const uint8_t * glyphcmds)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_composite_glyphs_32_checked(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, glyphcmds_len, glyphcmds));
}

template<typename Connection>
void
composite_glyphs_32(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, uint32_t glyphcmds_len, const uint8_t * glyphcmds)
{
  ::xcb_render_composite_glyphs_32(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, glyphcmds_len, glyphcmds);
}

template<typename Connection, typename Glyphcmds_Iterator>
void
composite_glyphs_32_checked(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, Glyphcmds_Iterator glyphcmds_begin, Glyphcmds_Iterator glyphcmds_end)
{
      typedef typename value_type<Glyphcmds_Iterator, ! std::is_pointer<Glyphcmds_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphcmds =
        { value_iterator<Glyphcmds_Iterator>(glyphcmds_begin), value_iterator<Glyphcmds_Iterator>(glyphcmds_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_composite_glyphs_32_checked(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, static_cast<uint32_t>(glyphcmds.size()), const_cast<const vector_type *>(glyphcmds.data())));
}

template<typename Connection, typename Glyphcmds_Iterator>
void
composite_glyphs_32(Connection && c, uint8_t op, ::xcb_render_picture_t src, ::xcb_render_picture_t dst, ::xcb_render_pictformat_t mask_format, ::xcb_render_glyphset_t glyphset, int16_t src_x, int16_t src_y, Glyphcmds_Iterator glyphcmds_begin, Glyphcmds_Iterator glyphcmds_end)
{
      typedef typename value_type<Glyphcmds_Iterator, ! std::is_pointer<Glyphcmds_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> glyphcmds =
        { value_iterator<Glyphcmds_Iterator>(glyphcmds_begin), value_iterator<Glyphcmds_Iterator>(glyphcmds_end) };

  ::xcb_render_composite_glyphs_32(std::forward<Connection>(c), op, src, dst, mask_format, glyphset, src_x, src_y, static_cast<uint32_t>(glyphcmds.size()), const_cast<const vector_type *>(glyphcmds.data()));
}

template<typename Connection>
void
fill_rectangles_checked(Connection && c, uint8_t op, ::xcb_render_picture_t dst, ::xcb_render_color_t color, uint32_t rects_len, const ::xcb_rectangle_t * rects)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_fill_rectangles_checked(std::forward<Connection>(c), op, dst, color, rects_len, rects));
}

template<typename Connection>
void
fill_rectangles(Connection && c, uint8_t op, ::xcb_render_picture_t dst, ::xcb_render_color_t color, uint32_t rects_len, const ::xcb_rectangle_t * rects)
{
  ::xcb_render_fill_rectangles(std::forward<Connection>(c), op, dst, color, rects_len, rects);
}

template<typename Connection, typename Rects_Iterator>
void
fill_rectangles_checked(Connection && c, uint8_t op, ::xcb_render_picture_t dst, ::xcb_render_color_t color, Rects_Iterator rects_begin, Rects_Iterator rects_end)
{
      typedef typename value_type<Rects_Iterator, ! std::is_pointer<Rects_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> rects =
        { value_iterator<Rects_Iterator>(rects_begin), value_iterator<Rects_Iterator>(rects_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_fill_rectangles_checked(std::forward<Connection>(c), op, dst, color, static_cast<uint32_t>(rects.size()), const_cast<const vector_type *>(rects.data())));
}

template<typename Connection, typename Rects_Iterator>
void
fill_rectangles(Connection && c, uint8_t op, ::xcb_render_picture_t dst, ::xcb_render_color_t color, Rects_Iterator rects_begin, Rects_Iterator rects_end)
{
      typedef typename value_type<Rects_Iterator, ! std::is_pointer<Rects_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> rects =
        { value_iterator<Rects_Iterator>(rects_begin), value_iterator<Rects_Iterator>(rects_end) };

  ::xcb_render_fill_rectangles(std::forward<Connection>(c), op, dst, color, static_cast<uint32_t>(rects.size()), const_cast<const vector_type *>(rects.data()));
}

template<typename Connection, typename ... Parameter>
void
create_cursor_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_cursor_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_cursor(Parameter && ... parameter)
{
  ::xcb_render_create_cursor(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
set_picture_transform_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_set_picture_transform_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
set_picture_transform(Parameter && ... parameter)
{
  ::xcb_render_set_picture_transform(std::forward<Parameter>(parameter) ...);
}

namespace reply {

namespace detail {

template<typename Connection,
         typename Check,
         typename CookieFunction>
class query_filters
  : public cppual::generic::reply<query_filters<Connection, Check, CookieFunction>,
                               Connection,
                               Check,
                               SIGNATURE(xcb_render_query_filters_reply),
                               CookieFunction>
{
  public:
    typedef cppual::generic::reply<query_filters<Connection, Check, CookieFunction>,
                                Connection,
                                Check,
                                SIGNATURE(xcb_render_query_filters_reply),
                                CookieFunction>
                                  base;

    template<typename C, typename ... Parameter>
    query_filters(C && c, Parameter && ... parameter)
      : base(std::forward<C>(c), std::forward<Parameter>(parameter) ...)
    {}


    cppual::generic::list<Connection,
                       ::xcb_render_query_filters_reply_t,
                       cppual::generic::iterator<Connection,
                                              uint16_t,
                                              SIGNATURE(xcb_render_query_filters_aliases),
                                              SIGNATURE(xcb_render_query_filters_aliases_length)>
                      >
    aliases(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_render_query_filters_reply_t,
                                cppual::generic::iterator<Connection,
                                                       uint16_t,
                                                       SIGNATURE(xcb_render_query_filters_aliases),
                                                       SIGNATURE(xcb_render_query_filters_aliases_length)>
                               >(this->m_c, this->get());
    }


    cppual::generic::list<Connection,
                       ::xcb_render_query_filters_reply_t,
                       cppual::generic::iterator<Connection,
                                              ::xcb_str_t,
                                              SIGNATURE(xcb_str_next),
                                              SIGNATURE(xcb_str_sizeof),
                                              SIGNATURE(xcb_render_query_filters_filters_iterator)>
                      >
    filters(void)
    {
      return cppual::generic::list<Connection,
                                ::xcb_render_query_filters_reply_t,
                                cppual::generic::iterator<Connection,
                                                       ::xcb_str_t,
                                                       SIGNATURE(xcb_str_next),
                                                       SIGNATURE(xcb_str_sizeof),
                                                       SIGNATURE(xcb_render_query_filters_filters_iterator)>
                               >(this->m_c, this->get());
    }
}; // class query_filters

} // namespace detail

namespace checked {
template<typename Connection>
using query_filters = detail::query_filters<
    Connection, cppual::generic::checked_tag,
    SIGNATURE(xcb_render_query_filters)>;
} // namespace checked

namespace unchecked {
template<typename Connection>
using query_filters = detail::query_filters<
    Connection, cppual::generic::unchecked_tag,
    SIGNATURE(xcb_render_query_filters_unchecked)>;
} // namespace unchecked

} // namespace reply


template<typename Connection, typename ... Parameter>
reply::checked::query_filters<Connection>
query_filters(Connection && c, Parameter && ... parameter)
{
  return reply::checked::query_filters<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
reply::unchecked::query_filters<Connection>
query_filters_unchecked(Connection && c, Parameter && ... parameter)
{
  return reply::unchecked::query_filters<Connection>(
      std::forward<Connection>(c), std::forward<Parameter>(parameter) ...);
}

template<typename Connection>
void
set_picture_filter_checked(Connection && c, ::xcb_render_picture_t picture, uint16_t filter_len, const char * filter, uint32_t values_len, const ::xcb_render_fixed_t * values)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_set_picture_filter_checked(std::forward<Connection>(c), picture, filter_len, filter, values_len, values));
}

template<typename Connection>
void
set_picture_filter(Connection && c, ::xcb_render_picture_t picture, uint16_t filter_len, const char * filter, uint32_t values_len, const ::xcb_render_fixed_t * values)
{
  ::xcb_render_set_picture_filter(std::forward<Connection>(c), picture, filter_len, filter, values_len, values);
}

template<typename Connection>
void
set_picture_filter_checked(Connection && c, ::xcb_render_picture_t picture, const std::string & filter, uint32_t values_len, const ::xcb_render_fixed_t * values)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_set_picture_filter_checked(std::forward<Connection>(c), picture, static_cast<uint16_t>(filter.length()), filter.c_str(), values_len, values));
}

template<typename Connection>
void
set_picture_filter(Connection && c, ::xcb_render_picture_t picture, const std::string & filter, uint32_t values_len, const ::xcb_render_fixed_t * values)
{
  ::xcb_render_set_picture_filter(std::forward<Connection>(c), picture, static_cast<uint16_t>(filter.length()), filter.c_str(), values_len, values);
}

template<typename Connection>
void
create_anim_cursor_checked(Connection && c, ::xcb_cursor_t cid, uint32_t cursors_len, const ::xcb_render_animcursorelt_t * cursors)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_anim_cursor_checked(std::forward<Connection>(c), cid, cursors_len, cursors));
}

template<typename Connection>
void
create_anim_cursor(Connection && c, ::xcb_cursor_t cid, uint32_t cursors_len, const ::xcb_render_animcursorelt_t * cursors)
{
  ::xcb_render_create_anim_cursor(std::forward<Connection>(c), cid, cursors_len, cursors);
}

template<typename Connection, typename Cursors_Iterator>
void
create_anim_cursor_checked(Connection && c, ::xcb_cursor_t cid, Cursors_Iterator cursors_begin, Cursors_Iterator cursors_end)
{
      typedef typename value_type<Cursors_Iterator, ! std::is_pointer<Cursors_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> cursors =
        { value_iterator<Cursors_Iterator>(cursors_begin), value_iterator<Cursors_Iterator>(cursors_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_anim_cursor_checked(std::forward<Connection>(c), cid, static_cast<uint32_t>(cursors.size()), const_cast<const vector_type *>(cursors.data())));
}

template<typename Connection, typename Cursors_Iterator>
void
create_anim_cursor(Connection && c, ::xcb_cursor_t cid, Cursors_Iterator cursors_begin, Cursors_Iterator cursors_end)
{
      typedef typename value_type<Cursors_Iterator, ! std::is_pointer<Cursors_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> cursors =
        { value_iterator<Cursors_Iterator>(cursors_begin), value_iterator<Cursors_Iterator>(cursors_end) };

  ::xcb_render_create_anim_cursor(std::forward<Connection>(c), cid, static_cast<uint32_t>(cursors.size()), const_cast<const vector_type *>(cursors.data()));
}

template<typename Connection>
void
add_traps_checked(Connection && c, ::xcb_render_picture_t picture, int16_t x_off, int16_t y_off, uint32_t traps_len, const ::xcb_render_trap_t * traps)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_add_traps_checked(std::forward<Connection>(c), picture, x_off, y_off, traps_len, traps));
}

template<typename Connection>
void
add_traps(Connection && c, ::xcb_render_picture_t picture, int16_t x_off, int16_t y_off, uint32_t traps_len, const ::xcb_render_trap_t * traps)
{
  ::xcb_render_add_traps(std::forward<Connection>(c), picture, x_off, y_off, traps_len, traps);
}

template<typename Connection, typename Traps_Iterator>
void
add_traps_checked(Connection && c, ::xcb_render_picture_t picture, int16_t x_off, int16_t y_off, Traps_Iterator traps_begin, Traps_Iterator traps_end)
{
      typedef typename value_type<Traps_Iterator, ! std::is_pointer<Traps_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> traps =
        { value_iterator<Traps_Iterator>(traps_begin), value_iterator<Traps_Iterator>(traps_end) };

  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_add_traps_checked(std::forward<Connection>(c), picture, x_off, y_off, static_cast<uint32_t>(traps.size()), const_cast<const vector_type *>(traps.data())));
}

template<typename Connection, typename Traps_Iterator>
void
add_traps(Connection && c, ::xcb_render_picture_t picture, int16_t x_off, int16_t y_off, Traps_Iterator traps_begin, Traps_Iterator traps_end)
{
      typedef typename value_type<Traps_Iterator, ! std::is_pointer<Traps_Iterator>::value>::type
                vector_type;
      std::vector<vector_type> traps =
        { value_iterator<Traps_Iterator>(traps_begin), value_iterator<Traps_Iterator>(traps_end) };

  ::xcb_render_add_traps(std::forward<Connection>(c), picture, x_off, y_off, static_cast<uint32_t>(traps.size()), const_cast<const vector_type *>(traps.data()));
}

template<typename Connection, typename ... Parameter>
void
create_solid_fill_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_solid_fill_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_solid_fill(Parameter && ... parameter)
{
  ::xcb_render_create_solid_fill(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_linear_gradient_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_linear_gradient_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_linear_gradient(Parameter && ... parameter)
{
  ::xcb_render_create_linear_gradient(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_radial_gradient_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_radial_gradient_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_radial_gradient(Parameter && ... parameter)
{
  ::xcb_render_create_radial_gradient(std::forward<Parameter>(parameter) ...);
}

template<typename Connection, typename ... Parameter>
void
create_conical_gradient_checked(Connection && c, Parameter && ... parameter)
{
  cppual::generic::check<Connection, cppual::render::error::dispatcher>(
      std::forward<Connection>(c),
      ::xcb_render_create_conical_gradient_checked(
          std::forward<Connection>(c),
          std::forward<Parameter>(parameter) ...));
}

template<typename ... Parameter>
void
create_conical_gradient(Parameter && ... parameter)
{
  ::xcb_render_create_conical_gradient(std::forward<Parameter>(parameter) ...);
}









template<typename Derived, typename Connection>
class pictformat
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_render_pictformat_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~pictformat(void) {}

    template<typename ... Parameter>
    auto
    query_pict_index_values(Parameter && ... parameter) const
    -> reply::checked::query_pict_index_values<Connection>
    {
      return cppual::render::query_pict_index_values(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_pict_index_values_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_pict_index_values<Connection>
    {
      return cppual::render::query_pict_index_values_unchecked(
          connection(),
          resource(),
          std::forward<Parameter>(parameter) ...);
    }



}; // class pictformat





template<typename Derived, typename Connection>
class picture
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_render_picture_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~picture(void) {}

    template<typename ... Parameter>
    void
    create_checked(Parameter && ... parameter) const
    {
      cppual::render::create_picture_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create(Parameter && ... parameter) const
    {
      cppual::render::create_picture(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_checked(Parameter && ... parameter) const
    {
      cppual::render::change_picture_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change(Parameter && ... parameter) const
    {
      cppual::render::change_picture(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_clip_rectangles_checked(Parameter && ... parameter) const
    {
      cppual::render::set_picture_clip_rectangles_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_clip_rectangles(Parameter && ... parameter) const
    {
      cppual::render::set_picture_clip_rectangles(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_checked(Parameter && ... parameter) const
    {
      cppual::render::free_picture_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free(Parameter && ... parameter) const
    {
      cppual::render::free_picture(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_transform_checked(Parameter && ... parameter) const
    {
      cppual::render::set_picture_transform_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_transform(Parameter && ... parameter) const
    {
      cppual::render::set_picture_transform(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_filter_checked(Parameter && ... parameter) const
    {
      cppual::render::set_picture_filter_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_filter(Parameter && ... parameter) const
    {
      cppual::render::set_picture_filter(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    add_traps_checked(Parameter && ... parameter) const
    {
      cppual::render::add_traps_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    add_traps(Parameter && ... parameter) const
    {
      cppual::render::add_traps(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_solid_fill_checked(Parameter && ... parameter) const
    {
      cppual::render::create_solid_fill_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_solid_fill(Parameter && ... parameter) const
    {
      cppual::render::create_solid_fill(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_linear_gradient_checked(Parameter && ... parameter) const
    {
      cppual::render::create_linear_gradient_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_linear_gradient(Parameter && ... parameter) const
    {
      cppual::render::create_linear_gradient(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_radial_gradient_checked(Parameter && ... parameter) const
    {
      cppual::render::create_radial_gradient_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_radial_gradient(Parameter && ... parameter) const
    {
      cppual::render::create_radial_gradient(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_conical_gradient_checked(Parameter && ... parameter) const
    {
      cppual::render::create_conical_gradient_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_conical_gradient(Parameter && ... parameter) const
    {
      cppual::render::create_conical_gradient(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }



}; // class picture







template<typename Derived, typename Connection>
class glyphset
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

    const ::xcb_render_glyphset_t &
    resource(void) const
    {
      return static_cast<const Derived *>(this)->resource();
    }

  public:
    virtual ~glyphset(void) {}

    template<typename ... Parameter>
    void
    create_glyph_set_checked(Parameter && ... parameter) const
    {
      cppual::render::create_glyph_set_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_glyph_set(Parameter && ... parameter) const
    {
      cppual::render::create_glyph_set(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    reference_glyph_set_checked(Parameter && ... parameter) const
    {
      cppual::render::reference_glyph_set_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    reference_glyph_set(Parameter && ... parameter) const
    {
      cppual::render::reference_glyph_set(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_glyph_set_checked(Parameter && ... parameter) const
    {
      cppual::render::free_glyph_set_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free_glyph_set(Parameter && ... parameter) const
    {
      cppual::render::free_glyph_set(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    add_glyphs_checked(Parameter && ... parameter) const
    {
      cppual::render::add_glyphs_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    add_glyphs(Parameter && ... parameter) const
    {
      cppual::render::add_glyphs(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_glyphs_checked(Parameter && ... parameter) const
    {
      cppual::render::free_glyphs_checked(connection(),
                          resource(),
                          std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free_glyphs(Parameter && ... parameter) const
    {
      cppual::render::free_glyphs(connection(),
                  resource(),
                  std::forward<Parameter>(parameter) ...);
    }



}; // class glyphset














template<typename Derived, typename Connection>
class interface
{
  protected:
    Connection
    connection(void) const
    {
      return static_cast<const Derived *>(this)->connection();
    }

  public:
    typedef cppual::render::extension extension;


    virtual ~interface(void) {}

    const interface<Derived, Connection> &
    render(void)
    {
      return *this;
    }

    template<typename ... Parameter>
    auto
    query_version(Parameter && ... parameter) const
    -> reply::checked::query_version<Connection>
    {
      return cppual::render::query_version(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_version_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_version<Connection>
    {
      return cppual::render::query_version_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_pict_formats(Parameter && ... parameter) const
    -> reply::checked::query_pict_formats<Connection>
    {
      return cppual::render::query_pict_formats(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_pict_formats_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_pict_formats<Connection>
    {
      return cppual::render::query_pict_formats_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_pict_index_values(Parameter && ... parameter) const
    -> reply::checked::query_pict_index_values<Connection>
    {
      return cppual::render::query_pict_index_values(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_pict_index_values_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_pict_index_values<Connection>
    {
      return cppual::render::query_pict_index_values_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_picture_checked(Parameter && ... parameter) const
    {
      cppual::render::create_picture_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_picture(Parameter && ... parameter) const
    {
      cppual::render::create_picture(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    change_picture_checked(Parameter && ... parameter) const
    {
      cppual::render::change_picture_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    change_picture(Parameter && ... parameter) const
    {
      cppual::render::change_picture(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_picture_clip_rectangles_checked(Parameter && ... parameter) const
    {
      cppual::render::set_picture_clip_rectangles_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_picture_clip_rectangles(Parameter && ... parameter) const
    {
      cppual::render::set_picture_clip_rectangles(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_picture_checked(Parameter && ... parameter) const
    {
      cppual::render::free_picture_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free_picture(Parameter && ... parameter) const
    {
      cppual::render::free_picture(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    composite_checked(Parameter && ... parameter) const
    {
      cppual::render::composite_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    composite(Parameter && ... parameter) const
    {
      cppual::render::composite(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    trapezoids_checked(Parameter && ... parameter) const
    {
      cppual::render::trapezoids_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    trapezoids(Parameter && ... parameter) const
    {
      cppual::render::trapezoids(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    triangles_checked(Parameter && ... parameter) const
    {
      cppual::render::triangles_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    triangles(Parameter && ... parameter) const
    {
      cppual::render::triangles(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    tri_strip_checked(Parameter && ... parameter) const
    {
      cppual::render::tri_strip_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    tri_strip(Parameter && ... parameter) const
    {
      cppual::render::tri_strip(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    tri_fan_checked(Parameter && ... parameter) const
    {
      cppual::render::tri_fan_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    tri_fan(Parameter && ... parameter) const
    {
      cppual::render::tri_fan(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_glyph_set_checked(Parameter && ... parameter) const
    {
      cppual::render::create_glyph_set_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_glyph_set(Parameter && ... parameter) const
    {
      cppual::render::create_glyph_set(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    reference_glyph_set_checked(Parameter && ... parameter) const
    {
      cppual::render::reference_glyph_set_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    reference_glyph_set(Parameter && ... parameter) const
    {
      cppual::render::reference_glyph_set(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_glyph_set_checked(Parameter && ... parameter) const
    {
      cppual::render::free_glyph_set_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free_glyph_set(Parameter && ... parameter) const
    {
      cppual::render::free_glyph_set(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    add_glyphs_checked(Parameter && ... parameter) const
    {
      cppual::render::add_glyphs_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    add_glyphs(Parameter && ... parameter) const
    {
      cppual::render::add_glyphs(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    free_glyphs_checked(Parameter && ... parameter) const
    {
      cppual::render::free_glyphs_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    free_glyphs(Parameter && ... parameter) const
    {
      cppual::render::free_glyphs(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    composite_glyphs_8_checked(Parameter && ... parameter) const
    {
      cppual::render::composite_glyphs_8_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    composite_glyphs_8(Parameter && ... parameter) const
    {
      cppual::render::composite_glyphs_8(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    composite_glyphs_16_checked(Parameter && ... parameter) const
    {
      cppual::render::composite_glyphs_16_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    composite_glyphs_16(Parameter && ... parameter) const
    {
      cppual::render::composite_glyphs_16(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    composite_glyphs_32_checked(Parameter && ... parameter) const
    {
      cppual::render::composite_glyphs_32_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    composite_glyphs_32(Parameter && ... parameter) const
    {
      cppual::render::composite_glyphs_32(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    fill_rectangles_checked(Parameter && ... parameter) const
    {
      cppual::render::fill_rectangles_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    fill_rectangles(Parameter && ... parameter) const
    {
      cppual::render::fill_rectangles(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_cursor_checked(Parameter && ... parameter) const
    {
      cppual::render::create_cursor_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_cursor(Parameter && ... parameter) const
    {
      cppual::render::create_cursor(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_picture_transform_checked(Parameter && ... parameter) const
    {
      cppual::render::set_picture_transform_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_picture_transform(Parameter && ... parameter) const
    {
      cppual::render::set_picture_transform(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    auto
    query_filters(Parameter && ... parameter) const
    -> reply::checked::query_filters<Connection>
    {
      return cppual::render::query_filters(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    auto
    query_filters_unchecked(Parameter && ... parameter) const
    -> reply::unchecked::query_filters<Connection>
    {
      return cppual::render::query_filters_unchecked(
          connection(),
                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    set_picture_filter_checked(Parameter && ... parameter) const
    {
      cppual::render::set_picture_filter_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    set_picture_filter(Parameter && ... parameter) const
    {
      cppual::render::set_picture_filter(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_anim_cursor_checked(Parameter && ... parameter) const
    {
      cppual::render::create_anim_cursor_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_anim_cursor(Parameter && ... parameter) const
    {
      cppual::render::create_anim_cursor(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    add_traps_checked(Parameter && ... parameter) const
    {
      cppual::render::add_traps_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    add_traps(Parameter && ... parameter) const
    {
      cppual::render::add_traps(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_solid_fill_checked(Parameter && ... parameter) const
    {
      cppual::render::create_solid_fill_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_solid_fill(Parameter && ... parameter) const
    {
      cppual::render::create_solid_fill(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_linear_gradient_checked(Parameter && ... parameter) const
    {
      cppual::render::create_linear_gradient_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_linear_gradient(Parameter && ... parameter) const
    {
      cppual::render::create_linear_gradient(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_radial_gradient_checked(Parameter && ... parameter) const
    {
      cppual::render::create_radial_gradient_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_radial_gradient(Parameter && ... parameter) const
    {
      cppual::render::create_radial_gradient(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


    template<typename ... Parameter>
    void
    create_conical_gradient_checked(Parameter && ... parameter) const
    {
      cppual::render::create_conical_gradient_checked(connection(),
                                                    std::forward<Parameter>(parameter) ...);
    }

    template<typename ... Parameter>
    void
    create_conical_gradient(Parameter && ... parameter) const
    {
      cppual::render::create_conical_gradient(connection(),
                                    std::forward<Parameter>(parameter) ...);
    }


}; // class interface

namespace event {

template<typename Connection>
class dispatcher
{
  public:
    typedef cppual::render::extension extension;

    template<typename C>
    dispatcher(C && c, uint8_t first_event)
      : m_c(std::forward<C>(c))
      , m_first_event(first_event)
    {}

    template<typename C>
    dispatcher(C && c, const cppual::render::extension & extension)
      : dispatcher(std::forward<C>(c), extension->first_event)
    {}

    template<typename Handler>
    bool
    operator()(Handler,
               const std::shared_ptr<xcb_generic_event_t> &) const
    {
      return false;
    }

  protected:
    Connection m_c;
    uint8_t m_first_event;
}; // class dispatcher

} // namespace event

namespace error {

class dispatcher
{
  public:
    typedef cppual::render::extension extension;

    dispatcher(uint8_t first_error)
      : m_first_error(first_error)
    {}

    dispatcher(const cppual::render::extension & extension)
      : dispatcher(extension->first_error)
    {}

    void
    operator()(const std::shared_ptr<xcb_generic_error_t> & error) const
    {
      switch (error->error_code - m_first_error) {

        case XCB_RENDER_PICT_FORMAT: // 0
          throw cppual::render::error::pict_format(error);

        case XCB_RENDER_PICTURE: // 1
          throw cppual::render::error::picture(error);

        case XCB_RENDER_PICT_OP: // 2
          throw cppual::render::error::pict_op(error);

        case XCB_RENDER_GLYPH_SET: // 3
          throw cppual::render::error::glyph_set(error);

        case XCB_RENDER_GLYPH: // 4
          throw cppual::render::error::glyph(error);

      };
    }

  protected:
    uint8_t m_first_error;
}; // class dispatcher

} // namespace error


} } // namespace xpp::render

#endif // CPPUAL_RENDER_HPP
