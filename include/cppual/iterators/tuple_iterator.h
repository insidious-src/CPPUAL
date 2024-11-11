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

#ifndef CPPUAL_TUPLE_ITERATOR_H_
#define CPPUAL_TUPLE_ITERATOR_H_
#ifdef __cplusplus

#include <cppual/decl.h>

#include <tuple>
#include <functional>
#include <variant>
#include <array>

namespace cppual { namespace detail {

/// Exposes types required by TupleIterator for standards compliance.
///
/// The types are derived from Tuple, which is assumed to satisfy the following:
///   - constexpr auto std::get<I>(std::declval<Tuple&>()) -> std::tuple_element_t<I, Tuple>&
///   - constexpr auto std::tuple_size<Tuple>() -> size_t
///
/// std::tuple, std::pair, and std::array define these overloads by default, but you can create
/// overloads for your own custom classes as necessary.
template <typename Tuple>
struct itr_traits_impl
{
private:
    template <size_t... I>
    static constexpr auto val_type_impl(std::index_sequence<I...> _) ->
        std::variant<std::reference_wrapper<std::tuple_element_t<I, Tuple>>...>;

public:
    using value_type     = decltype(val_type_impl(std::make_index_sequence<std::tuple_size_v<Tuple>>()));
    using reference_type = value_type;
};

// Builds an array of std::get accessors for the given type: Tuple.
template <typename Tuple>
struct getters_impl
{
public:
    static constexpr auto make_getters()
    {
        return make_getters_impl(std::make_index_sequence<std::tuple_size_v<Tuple>>());
    }

private:
    using reference_type = typename itr_traits_impl<Tuple>::reference_type;
    using getter_pointer = reference_type(*)(Tuple&);
    using getter_array   = std::array<const getter_pointer, std::tuple_size_v<Tuple>>;

    template <size_t... I>
    static constexpr getter_array make_getters_impl(std::index_sequence<I...>)
    {
        return { getter_impl<I>... };
    }

    template <size_t I>
    static constexpr reference_type getter_impl(Tuple& t)
    {
        return reference_type{std::in_place_index<I>, std::reference_wrapper{std::get<I>(t)}};
    }
};

} // namespace detail

/// Provides interface for creating tuple iterators.
template <typename Tuple>
class tuple_iterator
{
private:
    static constexpr const auto kGetters = detail::getters_impl<Tuple>::make_getters();
    using getter_itr = typename decltype(kGetters)::const_iterator;

public:
    using reference = typename detail::itr_traits_impl<Tuple>::reference_type;
    using value_type = typename detail::itr_traits_impl<Tuple>::value_type;
    using pointer = typename std::iterator_traits<getter_itr>::pointer;
    using difference_type = typename std::iterator_traits<getter_itr>::difference_type;
    using iterator_category = typename std::iterator_traits<getter_itr>::iterator_category;

    // Provides the interface for creating tuple iterators.
    // friend class tuple_range<Tuple>;

    // Returns a singular iterator, that is, an iterator that is not associated with any tuple.
    // Such instances are semantically equivalent to nullptr, and should therefore never be modified
    // or dereferenced; only reassignment is allowed.
    constexpr tuple_iterator() : tuple_ptr_{nullptr}, getter_itr_{std::cend(kGetters)} {}

    ~tuple_iterator() = default;
    constexpr tuple_iterator(const tuple_iterator& src) = default;
    constexpr tuple_iterator(tuple_iterator&& src) = default;
    constexpr tuple_iterator& operator=(const tuple_iterator& src) = default;
    constexpr tuple_iterator& operator=(tuple_iterator&& src) = default;

    constexpr reference operator*() { return (*getter_itr_)(*tuple_ptr_); }
    constexpr reference operator[](difference_type i) { return getter_itr_[i](*tuple_ptr_); }
    // NOTE: operator-> is not defined because there is no way to make it standard compliant.
    //
    // The standard expects that values returned by operator-> may eventually be resolved by 1+
    // repeated applications. However, we can only return a std::variant of pointers. This implies
    // that eventually, a call to std::visit *must be made*.
    //
    // For now, I've chosen to simply leave out the definition of operator-> while keeping the
    // definition of a "pointer"-type because it is required by std::distance.

    constexpr tuple_iterator& operator++() { ++getter_itr_; return *this; }
    constexpr tuple_iterator& operator--() { --getter_itr_; return *this; }
    constexpr tuple_iterator& operator+=(difference_type n) { getter_itr_ += n; return *this; }
    constexpr tuple_iterator& operator-=(difference_type n) { getter_itr_ -= n; return *this; }

    // Returns the distance from the given iterator.
    constexpr difference_type operator-(const tuple_iterator& rhs) const {
        return getter_itr_ - rhs.getter_itr_;
    }

    // Returns whether this iterator is singular.
    constexpr bool operator==(std::nullptr_t rhs) const { return tuple_ptr_ == rhs; }

    constexpr bool operator==(const tuple_iterator& rhs) const {
        return tuple_ptr_ == rhs.tuple_ptr_ && getter_itr_ == rhs.getter_itr_;
    }

    constexpr bool operator<(const tuple_iterator& rhs) const {
        return tuple_ptr_ == rhs.tuple_ptr_ ? getter_itr_ < rhs.getter_itr_
                                            : tuple_ptr_ < rhs.tuple_ptr_;
    }

    // All of the following operators are derived from those defined above.

    constexpr reference operator*() const { return *(*this); }
    constexpr reference operator[](difference_type i) const { return (*this)[i]; }

    constexpr tuple_iterator operator++(int) { tuple_iterator i{*this}; ++(*this); return i; }
    constexpr tuple_iterator operator--(int) { tuple_iterator i{*this}; --(*this); return i; }
    constexpr tuple_iterator operator+(difference_type n) const { return tuple_iterator{*this} += n; }
    constexpr tuple_iterator operator-(difference_type n) const { return tuple_iterator{*this} -= n; }

    constexpr bool operator!=(std::nullptr_t rhs) const { return !(*this == rhs); }
    constexpr bool operator!=(const tuple_iterator& rhs) const { return !(*this == rhs); }
    constexpr bool operator>(const tuple_iterator& rhs) const { return rhs < *this; }
    constexpr bool operator<=(const tuple_iterator& rhs) const { return !(rhs < *this); }
    constexpr bool operator>=(const tuple_iterator& rhs) const { return !(*this < rhs); }

    constexpr tuple_iterator(Tuple& t, getter_itr i) : tuple_ptr_{&t}, getter_itr_{i} {}

private:
    Tuple*     tuple_ptr_ ;
    getter_itr getter_itr_;
};

// The following operators are defined as free functions because they can not be defined as members.

template <typename Tuple>
constexpr bool operator == (std::nullptr_t lhs, const tuple_iterator<Tuple>& rhs)
{
    return rhs == lhs;
}

template <typename Tuple>
constexpr bool operator != (std::nullptr_t lhs, const tuple_iterator<Tuple>& rhs)
{
    return rhs != lhs;
}

template <typename Tuple>
constexpr tuple_iterator<Tuple> operator + (
        typename std::iterator_traits<tuple_iterator<Tuple>>::difference_type n,
        const tuple_iterator<Tuple>& i)
{
    return i + n;
}

} // cppual

#endif // __cplusplus
#endif // CPPUAL_TUPLE_ITERATOR_H_
