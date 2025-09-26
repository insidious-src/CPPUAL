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

#ifndef CPPUAL_ARRAY_MAP_H_
#define CPPUAL_ARRAY_MAP_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/string>
#include <cppual/concepts>
#include <cppual/meta_type>
#include <cppual/containers>
#include <cppual/memory_allocator>

#include <utility>

// ====================================================

namespace cppual {

using memory::allocator_t;

template <typename T>
concept array_map_key_t = unsigned_integer_t<T>;

// ====================================================

template <array_map_key_t K, non_void V, allocator_t A = memory::allocator<std::pair<K, V>>>
class dyn_array_map : public std::vector<std::pair<K, V>, A>
{
public:
    typedef dyn_array_map<K, V, A>                self_type             ;
    typedef std::vector<std::pair<K, V>, A>       base_type             ;
    typedef remove_cref_t<K>                      key_type              ;
    typedef key_type const                        const_key             ;
    typedef remove_cref_t<V>                      mapped_type           ;
    typedef mapped_type const                     const_mapped          ;
    typedef std::pair<K, V>                       value_type            ;
    typedef value_type const                      const_value           ;
    typedef value_type const&                     const_reference       ;
    typedef value_type &                          reference             ;
    typedef std::allocator_traits<A>              alloc_traits          ;
    typedef alloc_traits::allocator_type          allocator_type        ;
    typedef std::size_t                           size_type             ;
    typedef size_type const                       const_size            ;
    typedef ptrdiff                               difference_type       ;
    typedef fstring_view                          string_view           ;
    typedef string                                string_type           ;
    typedef base_type::iterator                   iterator              ;
    typedef base_type::const_iterator             const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;
    typedef std::pair<iterator, bool>             iterator_pair         ;
    typedef cchar*                                char_ptr              ;

    // ====================================================

    inline constexpr static const_size npos = static_cast<size_type> (-1);

    // ====================================================

    using base_type::base_type;
    using base_type::operator [];
    using base_type::get_allocator;
    using base_type::size;
    using base_type::capacity;
    using base_type::empty;
    using base_type::front;
    using base_type::back;
    using base_type::data;
    using base_type::begin;
    using base_type::end;
    using base_type::cbegin;
    using base_type::cend;
    using base_type::rbegin;
    using base_type::rend;
    using base_type::crbegin;
    using base_type::crend;
    using base_type::reserve;
    using base_type::shrink_to_fit;
    using base_type::max_size;
    using base_type::clear;
    using base_type::swap;
    using base_type::erase;
    using base_type::assign;

    constexpr dyn_array_map (self_type &&)             = default;
    constexpr dyn_array_map (self_type const&)         = default;
    constexpr self_type& operator = (self_type &&)     = default;
    constexpr self_type& operator = (self_type const&) = default;

    iterator lower_bound (key_type key);
    const_iterator lower_bound (key_type key) const;
    template <unsigned_integer_t Key>
    iterator lower_bound (Key x);
    template <unsigned_integer_t Key>
    const_iterator lower_bound (Key x) const;

    template <typename... Args>
    iterator emplace (const_iterator pos, Args&&... args);
    void push_back (const_reference val);
    void push_back (value_type&& val);
    void resize (size_type count, const_reference value = value_type ());
    iterator insert( const_iterator pos, const_reference val);
    iterator insert (const_iterator pos, value_type&& value);
    iterator insert (const_iterator pos, size_type count, const_reference val);
    template <iterator_t InputIt>
    iterator insert (const_iterator pos, InputIt first, InputIt last);
    iterator insert (const_iterator pos, std::initializer_list<value_type> list);
    template <typename... Args>
    void emplace_back (Args&&... args);
    template <typename... Args>
    reference emplace_back (Args&&... args);
    void pop_back ();

    iterator find (key_type key) { return &(*this)[get_index<key> ()]; }
    const_iterator find (key_type key) const { return &(*this)[get_index<key> ()]; }
    template <array_map_key_t Key> requires (sizeof (Key) <= sizeof (u32))
    iterator find (Key k) { return &(*this)[get_index<k> ()]; }
    template <array_map_key_t Key> requires (sizeof (Key) <= sizeof (u32))
    const_iterator find (Key k) const { return &(*this)[get_index<k> ()]; }
    consteval static size_type max_bucket_count () noexcept { return 1; }
    consteval static size_type bucket_count () noexcept { return 1; }
    consteval static size_type bucket_size (size_type) noexcept { return 1; }
    std::pair<iterator, iterator> equal_range (key_type key);
    std::pair<const_iterator, const_iterator> equal_range (key_type key) const;
    template <array_map_key_t Key>
    std::pair<iterator, iterator> equal_range (Key x);
    template <array_map_key_t Key>
    std::pair<const_iterator, const_iterator> equal_range (Key const& x) const;
    consteval static size_type count (key_type const& = key_type ()) { return 1; }
    template <array_map_key_t Key>
    consteval static size_type count (Key = Key ()) noexcept { return 1; }
    template <typename M>
    std::pair<iterator, bool> insert_or_assign (key_type k, M&& obj);
    template <array_map_key_t Key, typename M>
    std::pair<iterator, bool> insert_or_assign (Key k, M&& obj);

    constexpr dyn_array_map (size_type n = 10, allocator_type const& ator = allocator_type ())
    : base_type (n, value_type (), ator)
    { }

    constexpr const_reference operator [] (string_view const& key_str) const
    {
        return (*this)[get_index (key_str.data ())];
    }

    constexpr reference operator [] (string_view const& key_str)
    {
        return (*this)[get_index (key_str.data ())];
    }

    constexpr const_reference at (string_view const& key_str) const
    {
        auto const& ref = (*this)[get_index (key_str.data ())];

        assert (ref.first && "key NOT found!");
        return ref;
    }

    constexpr reference at (string_view const& key_str)
    {
        auto& ref = (*this)[get_index (key_str.data ())];

        assert (ref.first && "key NOT found!");
        return ref;
    }

    template <key_type... Keys>
    constexpr bool contains () const noexcept
    {
        return ((contains<Keys> ()) && ...);
    }

    template <char_ptr... Keys>
    constexpr bool contains () const noexcept
    {
        return ((contains<Keys> ()) && ...);
    }

    template <c_str_const_t... Keys>
    constexpr bool contains (Keys... keys) const noexcept
    {
        return ((contains<keys> ()) && ...);
    }

    template <key_type K_>
    constexpr bool contains () const noexcept
    {
        return (*this)[get_index<K_> ()].first != key_type ();
    }

    template <char_ptr Name>
    constexpr bool contains () const noexcept
    {
        return (*this)[get_index<Name> ()].first != key_type ();
    }

    constexpr bool contains (key_type key) const noexcept
    {
        return (*this)[get_index<key> ()].first != key_type ();
    }

    template <array_map_key_t Key>
    constexpr bool contains (Key k) const
    {
        return (*this)[get_index<static_cast<key_type> (k)> ()].first != key_type ();
    }

    constexpr bool contains (string_view const& name) const noexcept
    {
        return (*this)[get_index<name> ()].first != key_type ();
    }

    template <key_type K_>
    consteval size_type get_index () const noexcept
    { return K_ % size (); }

    template <char_ptr Name>
    consteval size_type get_index () const noexcept
    { return char_hash<Name> () % size (); }

    constexpr size_type get_index (string_view const& name) const noexcept
    { return char_hash (name.data ()) % size (); }
};

// ====================================================

} // cppual

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_ARRAY_MAP_H_
