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

#include <cppual/concept/concepts.h>
#include <cppual/types.h>

namespace cppual {

template <arithmetic_t> struct point2;
template <arithmetic_t> struct point3;
template <arithmetic_t> struct point4;
template <arithmetic_t> struct angle ;

typedef point2<i16   > point2i  ;
typedef point2<u16   > point2u  ;
typedef point2<u32   > point2u32;
typedef point2<float > point2f  ;
typedef point3<float > point3f  ;
typedef point3<float > vec3     ;
typedef point3<double> point3d  ;
typedef point3<i16   > point3i  ;
typedef point3<u32   > point3u32;
typedef point4<float > point4f  ;
typedef point4<float > vec4     ;
typedef point4<double> point4d  ;
typedef angle <float > anglef   ;
typedef angle <double> angled   ;

// ====================================================

template <arithmetic_t T>
struct point2
{
    typedef remove_cvrefptr_t<T> value_type;

    value_type x { }, y { };

    constexpr point2 () noexcept = default;

    constexpr point2 (value_type x_, value_type y_) noexcept
    : x (x_),  y (y_)
    { }

    template <arithmetic_t U>
    constexpr point2 (U x_, U y_) noexcept
    : x (value_type (x_)),  y (value_type (y_))
    { }
};

template <arithmetic_t T>
constexpr bool operator == (point2<T> const& gObj1,
                            point2<T> const& gObj2) noexcept
{ return (gObj1.x == gObj2.x and gObj1.y == gObj2.y); }

template <arithmetic_t T>
constexpr bool operator != (point2<T> const& gObj1,
                            point2<T> const& gObj2) noexcept
{ return (gObj1.x != gObj2.x or gObj1.y != gObj2.y); }

// ====================================================

template <arithmetic_t T>
struct point3
{
    typedef remove_cvrefptr_t<T> value_type;

    value_type x { }, y { }, z { };

    constexpr point3 () noexcept = default;

    constexpr point3 (value_type x_, value_type y_, value_type z_) noexcept
    : x (x_),  y (y_), z (z_)
    { }

    template <arithmetic_t U>
    constexpr point3 (U x_, U y_, U z_) noexcept
    : x (value_type (x_)),  y (value_type (y_), z (value_type (z_)))
    { }
};

template <arithmetic_t T>
constexpr bool operator == (point3<T> const& gObj1,
                            point3<T> const& gObj2) noexcept
{ return (gObj1.x == gObj2.x and gObj1.y == gObj2.y and gObj1.z == gObj2.z); }

template <arithmetic_t T>
constexpr bool operator != (point3<T> const& gObj1,
                            point3<T> const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// ====================================================

template <arithmetic_t T>
struct point4
{
    typedef remove_cvrefptr_t<T> value_type;

    value_type x { }, y { }, z { }, w { };

    constexpr point4 () noexcept = default;
};

template <arithmetic_t T>
constexpr bool operator == (point4<T> const& gObj1,
                            point4<T> const& gObj2) noexcept
{
    return (gObj1.x == gObj2.x and
            gObj1.y == gObj2.y and
            gObj1.z == gObj2.z and
            gObj1.w == gObj2.w);
}

template <arithmetic_t T>
constexpr bool operator != (point4<T> const& gObj1,
                            point4<T> const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// ====================================================

struct rect
{
    typedef i16 value_type;

    value_type left { }, top { }, right { }, bottom { };

    constexpr rect () noexcept = default;

    constexpr rect (value_type x, value_type y, u16 width, u16 height) noexcept
    : left   (x),
      top    (y),
      right  (static_cast<value_type> (x + width )),
      bottom (static_cast<value_type> (y + height))
    { }

    constexpr rect (point2u size) noexcept
    : left   (),
      top    (),
      right  (static_cast<value_type> (size.x)),
      bottom (static_cast<value_type> (size.y))
    { }

    constexpr u16 width () const noexcept
    {  return static_cast<u16> (right - left); }

    constexpr u16 height () const noexcept
    {  return static_cast<u16> (bottom - top); }

    constexpr point2i position () const noexcept
    {  return point2i (left, top) ; }

    constexpr point2u size () const noexcept
    {  return { width (), height () }; }

    inline void set_width  (u16 uWidth ) noexcept
    { right  = static_cast<value_type> (left + uWidth); }

    inline void set_height (u16 uHeight) noexcept
    { bottom = static_cast<value_type> (top + uHeight); }

    inline void reset () noexcept
    { left = top = right = bottom = value_type (); }

    constexpr bool is_point () const noexcept
    {  return (left == right && top == bottom); }

    constexpr bool intersects (rect const& gObj) const noexcept
    {
        return (right > gObj.left  && bottom > gObj.top   ) ||
               (left  < gObj.right && top    < gObj.bottom);
    }

    constexpr bool contains (point2i gPoint) const noexcept
    {
        return (left < gPoint.x && gPoint.x < right &&
                top  < gPoint.y && gPoint.y < bottom);
    }

    inline rect& operator = (point2i gPoint) noexcept
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

inline rect& operator += (rect& gObj, point2i gPoint) noexcept
{
   gObj.left   += gPoint.x;
   gObj.right  += gPoint.x;
   gObj.top    += gPoint.y;
   gObj.bottom += gPoint.y;
   return gObj;
}

inline rect& operator -= (rect& gObj, point2i gPoint) noexcept
{
   gObj.left   -= gPoint.x;
   gObj.right  -= gPoint.x;
   gObj.top    -= gPoint.y;
   gObj.bottom -= gPoint.y;
   return gObj;
}

// ====================================================

template <arithmetic_t T>
struct angle final
{
    typedef remove_cvrefptr_t<T> value_type;

    value_type radians { };

    constexpr angle () noexcept = default;

    template <arithmetic_t U>
    constexpr angle (angle<U> const& gObj) noexcept
    : radians (value_type (gObj.radians))
    { }
};

template <arithmetic_t T>
constexpr bool operator == (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians == b.radians; }

template <arithmetic_t T>
constexpr bool operator != (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians != b.radians; }

template <arithmetic_t T>
constexpr bool operator < (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians < b.radians; }

template <arithmetic_t T>
constexpr bool operator > (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians > b.radians; }

template <arithmetic_t T>
constexpr bool operator <= (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians <= b.radians; }

template <arithmetic_t T>
constexpr bool operator >= (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians >= b.radians; }

// ====================================================

} // cppual

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_COORDINATES_H_
