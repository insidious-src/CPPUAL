/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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

#include <cppual/concepts.h>
#include <cppual/types.h>

namespace cppual {

template <typename> struct point2;
template <typename> struct point3;
template <typename> struct point4;
template <typename> struct angle ;

typedef point2<i16   > point2i;
typedef point2<u16   > point2u;
typedef point2<u32   > point2u32;
typedef point2<float > point2f;
typedef point3<float > point3f;
typedef point3<float > vec3   ;
typedef point3<double> point3d;
typedef point3<i16   > point3i;
typedef point3<u32   > point3u32;
typedef point4<float > point4f;
typedef point4<float > vec4   ;
typedef point4<float > point4d;
typedef angle <float > anglef ;
typedef angle <double> angled ;

// ====================================================

template <typename T>
struct point2 final
{
    Arithmetic<T> x, y;

    constexpr point2 () noexcept = default;

    template <typename U>
    constexpr point2 (U x_, U y_) noexcept
    : x (T (x_)),  y (T (y_))
    { }
};

template <typename T>
constexpr bool operator == (point2<T> const& gObj1,
                            point2<T> const& gObj2) noexcept
{ return (gObj1.x == gObj2.x and gObj1.y == gObj2.y); }

template <typename T>
constexpr bool operator != (point2<T> const& gObj1,
                            point2<T> const& gObj2) noexcept
{ return (gObj1.x != gObj2.x or gObj1.y != gObj2.y); }

// ====================================================

template <typename T>
struct point3 final
{
    Arithmetic<T> x, y, z;
};

template <typename T>
constexpr bool operator == (point3<T> const& gObj1,
                            point3<T> const& gObj2) noexcept
{ return (gObj1.x == gObj2.x and gObj1.y == gObj2.y and gObj1.z == gObj2.z); }

template <typename T>
constexpr bool operator != (point3<T> const& gObj1,
                            point3<T> const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// ====================================================

template <typename T>
struct point4 final
{
    Arithmetic<T> x, y, z, w;
};

template <typename T>
constexpr bool operator == (point4<T> const& gObj1,
                            point4<T> const& gObj2) noexcept
{
    return (gObj1.x == gObj2.x and
            gObj1.y == gObj2.y and
            gObj1.z == gObj2.z and
            gObj1.w == gObj2.w);
}

template <typename T>
constexpr bool operator != (point4<T> const& gObj1,
                            point4<T> const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

// ====================================================

struct rect final
{
    typedef i16 value_type;

    value_type left   { };
    value_type top    { };
    value_type right  { };
    value_type bottom { };

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
    { return static_cast<u16> (right - left); }

    constexpr u16 height () const noexcept
    { return static_cast<u16> (bottom - top); }

    constexpr point2u size () const noexcept
    { return { width (), height () }; }

    inline void set_width  (u16 uWidth ) noexcept
    { right  = static_cast<value_type> (left + uWidth); }

    inline void set_height (u16 uHeight) noexcept
    { bottom = static_cast<value_type> (top + uHeight); }

    inline void reset () noexcept
    { left = top = right = bottom = value_type (); }

    constexpr bool is_point () const noexcept
    { return (left == right and top == bottom); }

    constexpr bool intersects (rect const& gObj) const noexcept
    {
        return (right > gObj.left  and bottom > gObj.top   ) or
               (left  < gObj.right and top    < gObj.bottom);
    }

    constexpr bool contains (point2i gPoint) const noexcept
    {
        return (left < gPoint.x and gPoint.x < right and
                top  < gPoint.y and gPoint.y < bottom);
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
    return (gObj1.left  == gObj2.left  and gObj1.top    == gObj2.top and
            gObj1.right == gObj2.right and gObj1.bottom == gObj2.bottom);
}

constexpr bool operator != (rect const& gObj1, rect const& gObj2) noexcept
{ return !(gObj1 == gObj2); }

//inline Rect& operator += (Rect& gObj, point2i gPoint) noexcept
//{
//    gObj.left   += gPoint.x;
//    gObj.right  += gPoint.x;
//    gObj.top    += gPoint.y;
//    gObj.bottom += gPoint.y;
//    return gObj;
//}

//inline Rect& operator -= (Rect& gObj, point2i gPoint) noexcept
//{
//    gObj.left   -= gPoint.x;
//    gObj.right  -= gPoint.x;
//    gObj.top    -= gPoint.y;
//    gObj.bottom -= gPoint.y;
//    return gObj;
//}

// ====================================================

template <typename T>
struct angle final
{
    Arithmetic<T> radians { };

    constexpr angle () noexcept = default;

    template <typename U>
    constexpr angle (angle<U> const& gObj) noexcept
    : radians (T (gObj.radians))
    { }
};

template <typename T>
constexpr bool operator == (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians == b.radians; }

template <typename T>
constexpr bool operator != (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians != b.radians; }

template <typename T>
constexpr bool operator < (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians < b.radians; }

template <typename T>
constexpr bool operator > (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians > b.radians; }

template <typename T>
constexpr bool operator <= (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians <= b.radians; }

template <typename T>
constexpr bool operator >= (angle<T> const& a, angle<T> const& b) noexcept
{ return a.radians >= b.radians; }

} // cppual

#endif // __cplusplus
#endif // CPPUAL_COORDINATES_H_
