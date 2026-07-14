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

#ifndef CPPUAL_TEXT_ROPE_H_
#define CPPUAL_TEXT_ROPE_H_
#ifdef __cplusplus

#include <cppual/casts>
#include <cppual/string>
#include <cppual/concepts>
#include <cppual/containers>
#include <cppual/memory_allocator>
//#include <cppual/iterators/rope_iterator.h>

#include <string_view>
//#include <iterator>
//#include <variant>
//#include <memory>
#include <list>

// ====================================================

namespace cppual {

// ====================================================

template <symbolic_char T, allocator_like A = memory::allocator<T>>
class SHARED_API frope :
public std::list<cow_string<T, A>, typename A::template rebind_t<cow_string<T, A>>>
{
public:
    typedef frope<T, A>                              self_type             ;
    typedef std::list<cow_string<T, A>, A>           base_type             ;
    typedef A::template rebind_t<cow_string<T, A>>   allocator_type        ;
    typedef memory::allocator_traits<allocator_type> alloc_traits          ;
    typedef alloc_traits::value_type                 value_type            ;
    typedef value_type const                         const_value           ;
    typedef alloc_traits::size_type                  size_type             ;
    typedef size_type const                          const_size            ;
    typedef alloc_traits::reference                  reference             ;
    typedef alloc_traits::const_reference            const_reference       ;
    typedef alloc_traits::pointer                    pointer               ;
    typedef alloc_traits::const_pointer              const_pointer         ;
    //typedef rope_iterator<self_type>               iterator              ;
    //typedef rope_iterator<const self_type>         const_iterator        ;
    //typedef std::reverse_iterator<iterator>        reverse_iterator      ;
    //typedef std::reverse_iterator<const_iterator>  reverse_const_iterator;

    // ====================================================

    constexpr const_reference operator [] (size_type index) const noexcept;
    constexpr reference       operator [] (size_type index)       noexcept;
};

// ====================================================

template <char_t T, allocator_like A>
constexpr bool operator <=> (frope<T, A> const& lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) <=> std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_like A>
constexpr bool operator <=> (frope<T, A> const& lhs, T const* rhs) noexcept
{
    return std::basic_string_view<T> (lhs) <=> std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_like A>
constexpr bool operator <=> (T const* lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) <=> std::basic_string_view<T> (rhs);
}

// ====================================================

template <char_t T, allocator_like A>
constexpr bool operator == (frope<T, A> const& lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) == std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_like A>
constexpr bool operator == (frope<T, A> const& lhs, T const* rhs) noexcept
{
    return std::basic_string_view<T> (lhs) == std::basic_string_view<T> (rhs);
}

template <char_t T, allocator_like A>
constexpr bool operator == (T const* lhs, frope<T, A> const& rhs) noexcept
{
    return std::basic_string_view<T> (lhs) == std::basic_string_view<T> (rhs);
}

// ====================================================

template <char_t T, allocator_like A>
constexpr bool operator != (frope<T, A> const& lhObj, frope<T, A> const& rhObj) noexcept
{ return !(lhObj == rhObj); }

template <char_t T, allocator_like A>
constexpr bool operator != (frope<T, A> const& lhObj, T const* pText2) noexcept
{ return !(lhObj == pText2); }

template <char_t T, allocator_like A>
constexpr bool operator != (T const* pText1, frope<T, A> const& rhObj) noexcept
{ return !(pText1 == rhObj); }

// ====================================================

// template <char_t T, allocator_like A>
// constexpr frope<T, A>& operator += (frope<T, A>& lhObj, frope<T, A> const& rhObj) noexcept
// { return add_to_string (lhObj, rhObj._M_gBuffer.heap.data, rhObj._M_uLength); }

// template <char_t T, allocator_like A>
// constexpr frope<T, A>& operator += (frope<T>& gObj, T const* pText) noexcept
// { return add_to_string (gObj, pText, str_size (pText)); }

// ====================================================

// template <char_t T, allocator_like A>
// constexpr frope<T, A> operator + (frope<T, A> const& lhObj, frope<T, A> const& rhObj) noexcept
// {
//     frope<T, A> gStr (lhObj);
//     return std::move (add_to_string (gStr, rhObj._M_gBuffer.heap.data, rhObj._M_uLength));
// }

// ====================================================

// template <char_t T, allocator_like A>
// constexpr frope<T, A> operator + (frope<T, A> const& gObj, T const* pText) noexcept
// {
//     frope<T, A> gStr (gObj);
//     return std::move (add_to_string (gStr, pText, str_size (pText)));
// }

// ====================================================

template <char_t T, allocator_like A>
constexpr void swap (frope<T, A>& lhs, frope<T, A>& rhs) noexcept
{
    std::swap (lhs._M_gBuffer, rhs._M_gBuffer);
    std::swap (lhs._M_uLength, rhs._M_uLength);
}

// ====================================================

typedef frope<char8 , memory::allocator<cow_string<char8 >>> fu8rope ;
typedef frope<char16, memory::allocator<cow_string<char16>>> fu16rope;
typedef frope<char32, memory::allocator<cow_string<char32>>> fu32rope;
typedef frope<wchar , memory::allocator<cow_string<wchar >>> fwrope  ;

// ====================================================

template <allocator_like A>
struct is_string <frope<char, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <frope<char8, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <frope<char16, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <frope<char32, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <frope<wchar, A>> : std::true_type
{ };

} // namespace cppual

#endif // __cplusplus
#endif // CPPUAL_TEXT_ROPE_H_