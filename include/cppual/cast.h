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

#include <cppual/concepts>

#include <type_traits>

// =========================================================

namespace cppual {

template <typename T>
constexpr void* voidifty (T& obj) noexcept
{
    return const_cast<void*> (static_cast<cvvoid*> (std::addressof (obj)));
}

// =========================================================

template <non_void Out, non_void In>
union cast_union
{
    In  in ;
    Out out;

    cast_union () = delete;
    constexpr cast_union (In val) noexcept : in (val) { }
};

// =========================================================

//! (non-consteval) non-const safe cast using union for same size types
template <typename Out, typename In>
constexpr Out direct_cast (In val) noexcept
{
    static_assert (sizeof (In) == sizeof (Out), "The sizes of In & Out are NOT equal!");
    return cast_union<Out, In> (val).out;
}

//! (non-consteval) const safe cast using union for same size types
template <variable Out, constant In>
constexpr Out const_direct_cast (In val) noexcept
{
    static_assert (sizeof (In) == sizeof (Out), "The sizes of In & Out are NOT equal!");
    return cast_union<Out, In> (val).out;
}

//! (non-consteval) const safe cast using union for same size types
template <constant Out, variable In>
constexpr Out const_direct_cast (In val) noexcept
{
    static_assert (sizeof (In) == sizeof (Out), "The sizes of In & Out are NOT equal!");
    return cast_union<Out, In> (val).out;
}

//! (non-consteval) non-const safe cast using union without type size check
template <typename Out, typename In>
constexpr Out unsafe_direct_cast (In val) noexcept
{
    return cast_union<Out, In> (val).out;
}

// =========================================================

template<typename To, typename From>
constexpr To safe_member_cast (From from)
{
    static_assert (sizeof (To) == sizeof (From));
    static_assert (std::is_trivially_copyable_v<To>);
    static_assert (std::is_trivially_copyable_v<From>);

    return direct_cast<To, From> (from);
}

// =========================================================

template <structure Out, structure In>
constexpr Out& class_cast (In& obj) noexcept
{
    static_assert (std::is_base_of_v<In, Out> || std::is_base_of_v<Out, In>,
                   "Out and In classes are NOT related!");
    return *direct_cast<Out*, In*> (&obj);
}

template <structure Out, structure In>
constexpr Out* class_ptr_cast (In* obj) noexcept
{
    return direct_cast<Out*> (obj);
}

// =========================================================

template <structure Out, structure In>
constexpr Out& dyn_cast (In& obj) noexcept
{
#   ifdef DEBUG_MODE
    return *dynamic_cast<Out*> (&obj);
#   else
    return *static_cast <Out*> (&obj);
#   endif
}

// ====================================================

template <structure Out, structure In, typename R, typename... Args>
constexpr auto mem_fn_cast (R(In::* mem_fn)(Args...)) noexcept -> R(Out::*)(Args...)
{
    return cast_union<R(Out::*)(Args...), decltype (mem_fn)> (mem_fn).out;
}

template <structure Out, structure In, typename R, typename... Args>
constexpr auto mem_fn_cast (R(In::* mem_fn)(Args...) const) noexcept -> R(Out::*)(Args...)
{
    return cast_union<R(Out::*)(Args...), decltype (mem_fn)> (mem_fn).out;
}

template <structure Out, structure In, typename R, typename... Args>
constexpr auto const_mem_fn_cast (R(In::* mem_fn)(Args...) const) noexcept -> R(Out::*)(Args...) const
{
    return cast_union<R(Out::*)(Args...) const, decltype (mem_fn)> (mem_fn).out;
}

template <structure Out, structure In, typename R, typename... Args>
constexpr auto const_mem_fn_cast (R(In::* mem_fn)(Args...)) noexcept -> R(Out::*)(Args...) const
{
    return cast_union<R(Out::*)(Args...) const, decltype (mem_fn)> (mem_fn).out;
}

// =========================================================

} // cppual

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_CAST_H_
