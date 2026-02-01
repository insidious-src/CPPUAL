/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_COORDINATES_H_
#define CPPUAL_COORDINATES_H_
#ifdef __cplusplus

#include <cppual/concepts>
#include <cppual/types>

#include <variant>

namespace cppual {

template <arithmetic> struct point2;
template <arithmetic> struct point3;
template <arithmetic> struct point4;
template <arithmetic> struct angle ;

typedef point2<i16   > point2i  ;
typedef point2<u16   > point2u  ;
typedef point2<u32   > point2u32;
typedef point2<u64   > point2u64;
typedef point2<float > point2f  ;
typedef point2<double> point2d  ;
typedef point3<float > vec3     ;
typedef point3<float > point3f  ;
typedef point3<double> point3d  ;
typedef point3<i16   > point3i  ;
typedef point3<u32   > point3u32;
typedef point3<u64   > point3u64;
typedef point4<float > vec4     ;
typedef point4<float > point4f  ;
typedef point4<double> point4d  ;
typedef angle <float > anglef   ;
typedef angle <double> angled   ;

// ====================================================

template <arithmetic T>
union real
{
    typedef std::conditional_t<sizeof (T) <= sizeof (float ), float,
            std::conditional_t<sizeof (T) <= sizeof (double), double, ldouble>>
    real_type;

    typedef real                                             self_type   ;
    typedef remove_crefptr_t<T>                              value_type  ;
    typedef value_type const                                 const_value ;
    typedef real_type  const                                 const_real  ;
    typedef std::variant<value_type, float, double, ldouble> variant_type;

    value_type   value  { };
    variant_type real_value;

    constexpr real () noexcept = default;

    template <variant_like U>
    constexpr real (U const& val) noexcept
    : real_value (val)
    { }

    constexpr real (value_type val) noexcept
    : value (val)
    { }

    constexpr real (real_type val) noexcept
    : real_value (val)
    { }
};

// ====================================================

template <arithmetic T>
struct point2
{
    typedef remove_cvrefptr_t<T>           value_type  ;
    typedef std::variant<T, float, double> variant_type;

    value_type x { }, y { };

    constexpr point2 () noexcept = default;

    constexpr point2 (value_type x_, value_type y_) noexcept
    : x (x_),  y (y_)
    { }

    template <arithmetic U>
    constexpr point2 (U x_, U y_) noexcept
    : x (value_type (x_)),  y (value_type (y_))
    { }
};

template <arithmetic T>
constexpr bool operator == (point2<T> const& gObj1,
                            point2<T> const& gObj2) noexcept
{ return (gObj1.x == gObj2.x and gObj1.y == gObj2.y); }

template <arithmetic T>
constexpr bool operator != (point2<T> const& gObj1,
                            point2<T> const& gObj2) noexcept
{ return (gObj1.x != gObj2.x or gObj1.y != gObj2.y); }

// ====================================================

template <arithmetic T>
struct point3
{
    typedef remove_cvrefptr_t<T> value_type;

    value_type x { }, y { }, z { };

    constexpr point3 () noexcept = default;

    constexpr point3 (value_type x_, value_type y_, value_type z_) noexcept
    : x (x_),  y (y_), z (z_)
    { }

    template <arithmetic U>
    constexpr point3 (U x_, U y_, U z_) noexcept
    : x (value_type (x_)),  y (value_type (y_), z (value_type (z_)))
    { }
};

template <arithmetic T>
constexpr bool operator == (point3<T> const& gObj1,
                            point3<T> const& gObj2) noexcept
{ return (gObj1.x == gObj2.x and gObj1.y == gObj2.y and gObj1.z == gObj2.z); }

template <arithmetic T>
constexpr bool operator != (point3<T> const& gObj1,
                            point3<T> const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// ====================================================

template <arithmetic T>
struct point4
{
    typedef remove_cvrefptr_t<T> value_type;

    value_type x { }, y { }, z { }, w { };

    constexpr point4 () noexcept = default;
};

template <arithmetic T>
constexpr bool operator == (point4<T> const& gObj1,
                            point4<T> const& gObj2) noexcept
{
    return (gObj1.x == gObj2.x and
            gObj1.y == gObj2.y and
            gObj1.z == gObj2.z and
            gObj1.w == gObj2.w);
}

template <arithmetic T>
constexpr bool operator != (point4<T> const& gObj1,
                            point4<T> const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// ====================================================

struct rect
{
    typedef rect             self_type  ;
    typedef i16              value_type ;
    typedef value_type const const_value;
    typedef u16              size_type  ;
    typedef size_type  const const_size ;

    value_type left { }, top { }, right { }, bottom { };

    constexpr rect ()                 noexcept = default;
    constexpr rect (self_type &&)     noexcept = default;
    constexpr rect (self_type const&) noexcept = default;

    constexpr self_type& operator = (self_type &&)     noexcept = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr rect (point2i pos, point2u size) noexcept
    : left   (pos.x)
    , top    (pos.y)
    , right  (static_cast<value_type> (pos.x + size.x))
    , bottom (static_cast<value_type> (pos.y + size.y))
    { }

    constexpr rect (value_type x, value_type y, size_type width, size_type height) noexcept
    : left   (x)
    , top    (y)
    , right  (static_cast<value_type> (x + width ))
    , bottom (static_cast<value_type> (y + height))
    { }

    constexpr rect (point2u size) noexcept
    : left   ()
    , top    ()
    , right  (static_cast<value_type> (size.x))
    , bottom (static_cast<value_type> (size.y))
    { }

    constexpr size_type width () const noexcept
    {  return static_cast<size_type> (right - left); }

    constexpr size_type height () const noexcept
    {  return static_cast<size_type> (bottom - top); }

    constexpr point2i position () const noexcept
    {  return point2i (left, top) ; }

    constexpr point2u size () const noexcept
    {  return { width (), height () }; }

    constexpr void set_width  (size_type uWidth ) noexcept
    { right  = static_cast<value_type> (left + uWidth); }

    constexpr void set_height (size_type uHeight) noexcept
    { bottom = static_cast<value_type> (top + uHeight); }

    constexpr void reset () noexcept
    { left = top = right = bottom = value_type (); }

    constexpr bool is_point () const noexcept
    {  return left == right && top == bottom; }

    constexpr bool intersects (self_type const& gObj) const noexcept
    {
        return (right > gObj.left  && bottom > gObj.top   ) ||
               (left  < gObj.right && top    < gObj.bottom);
    }

    constexpr bool contains (point2i gPoint) const noexcept
    {
        return (left < gPoint.x && gPoint.x < right &&
                top  < gPoint.y && gPoint.y < bottom);
    }

    constexpr self_type& operator = (point2i gPoint) noexcept
    {
        right  = static_cast<i16> (gPoint.x + width  ());
        bottom = static_cast<i16> (gPoint.y + height ());
        left   = gPoint.x;
        top    = gPoint.y;
        return *this;
    }
};

constexpr bool operator == (rect const& gObj1, rect const& gObj2) noexcept
{
    return (gObj1.left  == gObj2.left  && gObj1.top    == gObj2.top &&
            gObj1.right == gObj2.right && gObj1.bottom == gObj2.bottom);
}

constexpr bool operator != (rect const& gObj1, rect const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

constexpr rect& operator += (rect& gObj, point2i gPoint) noexcept
{
   gObj.left   += gPoint.x;
   gObj.right  += gPoint.x;
   gObj.top    += gPoint.y;
   gObj.bottom += gPoint.y;
   return gObj;
}

constexpr rect& operator -= (rect& gObj, point2i gPoint) noexcept
{
   gObj.left   -= gPoint.x;
   gObj.right  -= gPoint.x;
   gObj.top    -= gPoint.y;
   gObj.bottom -= gPoint.y;
   return gObj;
}

// ====================================================

template <arithmetic T>
struct angle final
{
    typedef remove_const_t<T> value_type;

    value_type radians { };

    constexpr angle () noexcept = default;

    template <arithmetic U>
    constexpr angle (angle<U> const& gObj) noexcept
    : radians (value_type (gObj.radians))
    { }
};

template <arithmetic T>
constexpr auto operator <=> (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians <=> b.radians; }

// ====================================================

} // cppual

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_COORDINATES_H_
