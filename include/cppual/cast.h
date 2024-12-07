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

#ifndef CPPUAL_CAST_H_
#define CPPUAL_CAST_H_
#ifdef __cplusplus

#include <cppual/concept/concepts.h>

#include <type_traits>
#include <bit>

namespace cppual {

// =========================================================

template <typename Out, typename In>
union cast_union
{
    Out out;
    In  in ;

    cast_union () = delete;
    constexpr cast_union (In val) noexcept : in (val) { }
};

// =========================================================

template<typename To, typename From>
constexpr To safe_member_cast (From from)
{
    static_assert (sizeof (To) == sizeof (From));
    static_assert (std::is_trivially_copyable_v<To>);
    static_assert (std::is_trivially_copyable_v<From>);

    return std::bit_cast<To> (from);
}

// =========================================================

template <non_void_t Out, non_void_t In>
constexpr Out direct_cast (In val) noexcept
{
    static_assert (sizeof (In) == sizeof (Out), "The sizes of In & Out are NOT equal!");
    return cast_union<Out, In> (val).out;
}

template <non_void_t Out, non_void_t In>
constexpr Out unsafe_direct_cast (In val) noexcept
{
    return cast_union<Out, In> (val).out;
}

template <class_t Out, class_t In>
constexpr Out& class_cast (In& obj) noexcept
{
    static_assert (std::is_base_of_v<In, Out> || std::is_base_of_v<Out, In>,
                   "Out and In classes are NOT related!");
    return *direct_cast<Out*, In*> (&obj);
}

// =========================================================

template <class_t D, class_t B, typename R, typename... Args>
constexpr auto method_cast (R(B::* fn)(Args...)) noexcept -> R(D::*)(Args...)
{
    static_assert (std::is_base_of_v<B, D>, "D is NOT derived from B class!");
    return direct_cast<R(D::*)(Args...)> (fn);
}

// =========================================================

template <class_t Out, class_t In>
constexpr Out& object_cast (In& obj) noexcept
{
#   ifdef DEBUG_MODE
    return *dynamic_cast<Out*> (&obj);
#   else
    return *static_cast <Out*> (&obj);
#   endif
}

// =========================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_CAST_H_
