/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2024 K. Petrov
 *
 * This program is free software: you can redistribute it &&/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, ||
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY || FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CPPUAL_FORWARD_META_H_
#define CPPUAL_FORWARD_META_H_
#ifdef __cplusplus

#include <cppual/decl>
#include <cppual/concepts>

#include <utility>

// =========================================================

namespace cppual {

// =========================================================

using std::forward;

// =========================================================

template <non_void T, non_void U = remove_cvref_t<T>>
using forward_t = std::conditional_t<(std::is_class_v<U> || std::is_union_v<U>) && !std::is_pointer_v<T>,
                                     std::conditional_t<is_cref_v<T> && std::is_copy_constructible_v<U>,
                                                        U const&,
                                                        std::conditional_t<
                                                        is_ref_v<T> && std::is_move_constructible_v<U>,
                                                        U&&,
                                                        std::conditional_t<(sizeof (U) < sizeof (uptr)), U, U&>
                                                                           >
                                                        >,
                                                        std::conditional_t<std::is_fundamental_v<U>, U, U&&>
                                     >;

// =========================================================

template <non_void T>
constexpr std::conditional_t<is_compact_forwarded_v<T>, T, T&&> forward_compact (T&& val) noexcept
{
    return is_compact_forwarded_v<T> ? val : std::forward<T> (val);
}

template <non_void T>
constexpr std::conditional_t<is_compact_forwarded_v<T>, T, T&&> forward_compact (T& val) noexcept
{
    return is_compact_forwarded_v<T> ? val : std::forward<T> (val);
}

// =========================================================

// template <non_void T>
// constexpr forward_t<T> forward_perfect (T var) noexcept
// {
//     return var;
// }

template <non_void T>
constexpr forward_t<T> forward_perfect (remove_ref_t<T> var) noexcept
{
    return var;
}

template <non_void T>
constexpr forward_t<T> forward_perfect (remove_ref_t<T>&& var) noexcept
{
    return var;
}

// ====================================================

} // std

#endif // __cplusplus
#endif // CPPUAL_FORWARD_META_H_