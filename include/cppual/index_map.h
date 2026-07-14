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

#ifndef CPPUAL_INDEX_MAP_H_
#define CPPUAL_INDEX_MAP_H_
#ifdef __cplusplus

#include <cppual/decl>
#include <cppual/types>
#include <cppual/string>
#include <cppual/concepts>
#include <cppual/containers>
#include <cppual/meta_string>
#include <cppual/memory_allocator>

#include <utility>
#include <vector>

// ====================================================

namespace cppual {

// ====================================================

template <typename K>
concept integer_or_void = unsigned_integer<K> || void_t<K>;

// ====================================================

//! A vector-based map with O(1) lookup time, using a modulo hash function to compute indices.
template <integer_or_void K, non_void V, allocator_like A = memory::allocator<std::pair<K const, V>>>
class dyn_index_map : public std::vector<std::pair<K const, V>, A>
{
public:
    typedef dyn_index_map<K, V, A>                self_type             ;
    typedef std::vector<std::pair<K const, V>, A> base_type             ;
    typedef remove_cref_t<K>                      key_type              ;
    typedef key_type const                        const_key             ;
    typedef remove_cref_t<V>                      mapped_type           ;
    typedef mapped_type const                     const_mapped          ;
    typedef base_type::value_type                 value_type            ;
    typedef value_type const                      const_value           ;
    typedef value_type const&                     const_reference       ;
    typedef value_type &                          reference             ;
    typedef memory::allocator_traits<A>           alloc_traits          ;
    typedef alloc_traits::allocator_type          allocator_type        ;
    typedef alloc_traits::size_type               size_type             ;
    typedef size_type const                       const_size            ;
    typedef ptrdiff                               difference_type       ;
    typedef fstring_view                          string_view           ;
    typedef fstring                               string_type           ;
    typedef base_type::iterator                   iterator              ;
    typedef base_type::const_iterator             const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> reverse_const_iterator;
    typedef std::pair<iterator, bool>             iterator_pair         ;
    typedef cchar*                                char_ptr              ;

    // ====================================================

    static_assert (are_same<value_type, typename allocator_type::value_type>,
        "value_type must be the same as allocator_type::value_type!");

    // ====================================================

    inline constexpr static const_size npos = static_cast<size_type> (-1);

    // ====================================================

    using base_type::base_type;
    using base_type::operator=;
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
    using base_type::max_size;
    using base_type::clear;
    using base_type::swap;
    using base_type::erase;
    using base_type::assign;

    constexpr dyn_index_map (size_type n = 10, allocator_type const& ator = allocator_type ())
    : base_type (n, ator)
    { }

    iterator lower_bound (key_type key);
    const_iterator lower_bound (key_type key) const;
    template <unsigned_integer Key>
    iterator lower_bound (Key x);
    template <unsigned_integer Key>
    const_iterator lower_bound (Key x) const;

    template <typename... Args>
    iterator emplace (const_iterator pos, Args&&... args);
    void push_back (const_reference val);
    void push_back (value_type&& val);
    void reserve (size_type count);
    template <typename... Args>
    void emplace_back (Args&&... args);
    template <typename... Args>
    reference emplace_back (Args&&... args);
    void pop_back ();

    template <size_type K_>
    iterator find () const noexcept { return &(*this)[get_index<K_> ()]; }
    template <size_type K_>
    const_iterator find () const noexcept { return &(*this)[get_index<K_> ()]; }
    template <auto K_> requires (c_const_str<decltype (K_)>)
    iterator find () const  noexcept { return &(*this)[get_index<K_> ()]; }
    template <auto K_> requires (c_const_str<decltype (K_)>)
    const_iterator find () const noexcept { return &(*this)[get_index<K_> ()]; }
    iterator find (size_type k) noexcept { return &(*this)[get_index<k> ()]; }
    const_iterator find (size_type k) const noexcept { return &(*this)[get_index<k> ()]; }
    template <str_view_like U>
    iterator find (U const& k) { return &(*this)[get_index (k.data ())]; }
    const_iterator find (string_view const& k) const noexcept { return &(*this)[get_index (k.data ())]; }
    consteval static size_type max_bucket_count () noexcept { return 1; }
    consteval static size_type bucket_count () noexcept { return 1; }
    consteval static size_type bucket_size (size_type) noexcept { return 1; }
    std::pair<iterator, iterator> equal_range (key_type key);
    std::pair<const_iterator, const_iterator> equal_range (key_type key) const;
    template <integer Key>
    std::pair<iterator, iterator> equal_range (Key x);
    template <integer Key>
    std::pair<const_iterator, const_iterator> equal_range (Key const& x) const;
    template <typename M>
    std::pair<iterator, bool> insert_or_assign (key_type k, M&& obj);
    template <integer Key, typename M>
    std::pair<iterator, bool> insert_or_assign (Key k, M&& obj);

    // ====================================================

    template <integer KSZ>
    constexpr const_reference operator [] (KSZ key) const noexcept
    {
        return base_type::operator [] (get_index (key));
    }

    template <integer KSZ>
    constexpr reference operator [] (KSZ key) noexcept
    {
        return base_type::operator [] (get_index (key));
    }

    constexpr const_reference operator [] (char_ptr key_str) const noexcept
    {
        return base_type::operator [] (get_index (key_str));
    }

    constexpr reference operator [] (char_ptr key_str) noexcept
    {
        return base_type::operator [] (get_index (key_str));
    }

    template <str_view_like U>
    constexpr const_reference operator [] (U const& key_str) const noexcept
    {
        return base_type::operator [] (get_index (key_str.data ()));
    }

    template <str_view_like U>
    constexpr reference operator [] (U const& key_str) noexcept
    {
        return base_type::operator [] (get_index (key_str.data ()));
    }

    // ====================================================

    template <integer KSZ>
    constexpr const_reference at (KSZ key) const noexcept
    {
        const_reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    template <integer KSZ>
    constexpr reference at (KSZ key) noexcept
    {
        reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    constexpr const_reference at (char_ptr key) const noexcept
    {
        const_reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    constexpr reference at (char_ptr key) noexcept
    {
        reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    template <str_view_like U>
    constexpr const_reference at (U const& key_str) const noexcept
    {
        const_reference ref = base_type::operator [] (get_index (key_str.data ()));

        assert (ref.first && "key NOT found!");
        return ref;
    }

    template <str_view_like U>
    constexpr reference at (U const& key_str) noexcept
    {
        reference ref = base_type::operator [] (get_index (key_str.data ()));

        assert (ref.first && "key NOT found!");
        return ref;
    }

    // ====================================================

    template <char_ptr K1, char_ptr K2, char_ptr... Ks>
    constexpr bool contains () const noexcept
    {
        return contains<K1> () && contains<K2> () && ((contains<Ks> ()) && ...);
    }

    template <auto K1, auto K2, auto... Ks>
    requires (integer<decltype (K1)> && integer<decltype (K2)> && (integer<decltype (Ks)> && ...))
    constexpr bool contains () const noexcept
    {
        return contains<K1> () && contains<K2> () && ((contains<Ks> ()) && ...);
    }

    template <auto K1, auto K2, auto... Ks>
    requires (str_view_like<decltype (K1)> &&
              str_view_like<decltype (K2)> &&
             (str_view_like<decltype (Ks)> && ...))
    constexpr bool contains () const noexcept
    {
        return contains<K1> () && contains<K2> () && ((contains<Ks> ()) && ...);
    }

    template <c_const_str K1, c_const_str K2, c_const_str... Ks>
    constexpr bool contains (K1 key1, K2 key2, Ks... keys) const noexcept
    {
        return contains (key1) && contains (key2) && ((contains (keys)) && ...);
    }

    template <integer K1, integer K2, integer... Ks>
    constexpr bool contains (K1 key1, K2 key2, Ks... keys) const noexcept
    {
        return contains (key1) && contains (key2) && ((contains (keys)) && ...);
    }

    template <str_view_like K1, str_view_like K2, str_view_like... Ks>
    constexpr bool contains (K1 key1, K2 key2, Ks... keys) const noexcept
    {
        return contains (key1) && contains (key2) && ((contains (keys)) && ...);
    }

    // ====================================================

    template <auto k> requires (integer<decltype (k)>)
    constexpr bool contains () const noexcept
    {
        return (*this)[k].first == k;
    }

    template <char_ptr Name>
    constexpr bool contains () const noexcept
    {
        return (*this)[Name].first == char_hash<Name> ();
    }

    template <integer Key>
    constexpr bool contains (Key k) const noexcept
    {
        return (*this)[k].first == k;
    }

    constexpr bool contains (string_view const& name) const noexcept
    {
        return (*this)[name].first == char_hash (name.data (), name.size ());
    }

    template <c_const_str Key>
    constexpr bool contains (Key name) const noexcept
    {
        return (*this)[name].first == char_hash (name);
    }

    // ====================================================

    template <auto Key> requires (integer<decltype (Key)>)
    consteval size_type get_index () const noexcept
    { return (Key % size () + Key) % capacity (); }

    template <char_ptr Key>
    consteval size_type get_index () const noexcept
    { return get_index<char_hash<Key> ()> () % capacity (); }

    template <auto Key> requires (str_view_like<decltype (Key)>)
    constexpr size_type get_index () const noexcept
    { return get_index<char_hash<Key.data (), Key.size ()>> () % capacity (); }

    template <str_view_like U>
    constexpr size_type get_index (U const& name) const noexcept
    { return get_index (char_hash (name.data (), name.size ())) % capacity (); }

    template <c_const_str Key>
    constexpr size_type get_index (Key name) const noexcept
    { return get_index (char_hash (name)) % capacity (); }

    template <integer Key>
    constexpr size_type get_index (Key k) const noexcept
    { return (k % size () + k) % capacity (); }
};

// ====================================================

//! A storage-optimized specialization of dyn_index_map
template <non_void V, allocator_like A>
class dyn_index_map <void, V, A>: public std::vector<V, A>
{
public:
    typedef dyn_index_map<void, V, A>             self_type             ;
    typedef std::vector<V, A>                     base_type             ;
    typedef std::size_t                           key_type              ;
    typedef key_type const                        const_key             ;
    typedef base_type::value_type                 value_type            ;
    typedef value_type const                      const_value           ;
    typedef value_type const&                     const_reference       ;
    typedef value_type &                          reference             ;
    typedef memory::allocator_traits<A>           alloc_traits          ;
    typedef alloc_traits::allocator_type          allocator_type        ;
    typedef alloc_traits::size_type               size_type             ;
    typedef size_type const                       const_size            ;
    typedef ptrdiff                               difference_type       ;
    typedef fstring_view                          string_view           ;
    typedef fstring                               string_type           ;
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
    using base_type::operator=;
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
    using base_type::max_size;
    using base_type::clear;
    using base_type::swap;
    using base_type::erase;
    using base_type::assign;

    constexpr dyn_index_map (size_type n = 10, allocator_type const& ator = allocator_type ())
    : base_type (n, ator)
    { }

    void push (const_reference val);
    void push (value_type&& val);
    void reserve (size_type count);
    template <typename... Args>
    void emplace (Args&&... args);
    template <typename... Args>
    reference emplace (Args&&... args);

    template <size_type K_>
    iterator find () const noexcept { return &(*this)[get_index<K_> ()]; }
    template <size_type K_>
    const_iterator find () const noexcept { return &(*this)[get_index<K_> ()]; }
    template <auto K_> requires (c_const_str<decltype (K_)>)
    iterator find () const  noexcept { return &(*this)[get_index<K_> ()]; }
    template <auto K_> requires (c_const_str<decltype (K_)>)
    const_iterator find () const noexcept { return &(*this)[get_index<K_> ()]; }
    iterator find (size_type k) noexcept { return &(*this)[get_index<k> ()]; }
    const_iterator find (size_type k) const noexcept { return &(*this)[get_index<k> ()]; }
    iterator find (string_view const& k) { return &(*this)[get_index (k.data ())]; }
    const_iterator find (string_view const& k) const noexcept { return &(*this)[get_index (k.data ())]; }
    consteval static size_type max_bucket_count () noexcept { return 1; }
    consteval static size_type bucket_count () noexcept { return 1; }
    consteval static size_type bucket_size (size_type) noexcept { return 1; }
    std::pair<iterator, iterator> equal_range (key_type key);
    std::pair<const_iterator, const_iterator> equal_range (key_type key) const;
    template <integer Key>
    std::pair<iterator, iterator> equal_range (Key x);
    template <integer Key>
    std::pair<const_iterator, const_iterator> equal_range (Key const& x) const;
    template <typename M>
    std::pair<iterator, bool> insert_or_assign (key_type k, M&& obj);
    template <integer Key, typename M>
    std::pair<iterator, bool> insert_or_assign (Key k, M&& obj);

    // ====================================================

    constexpr const_reference operator [] (key_type key) const noexcept
    {
        return base_type::operator [] (get_index (key));
    }

    constexpr reference operator [] (key_type key) noexcept
    {
        return base_type::operator [] (get_index (key));
    }

    constexpr const_reference operator [] (char_ptr key_str) const noexcept
    {
        return base_type::operator [] (get_index (key_str));
    }

    constexpr reference operator [] (char_ptr key_str) noexcept
    {
        return base_type::operator [] (get_index (key_str));
    }

    template <str_view_like U>
    constexpr const_reference operator [] (U const& key_str) const noexcept
    {
        return base_type::operator [] (get_index (key_str.data ()));
    }

    template <str_view_like U>
    constexpr reference operator [] (U const& key_str) noexcept
    {
        return base_type::operator [] (get_index (key_str.data ()));
    }

    // ====================================================

    constexpr const_reference at (key_type key) const noexcept
    {
        const_reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    constexpr reference at (key_type key) noexcept
    {
        reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    constexpr const_reference at (char_ptr key) const noexcept
    {
        const_reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    constexpr reference at (char_ptr key) noexcept
    {
        reference ref = base_type::operator [] (get_index<key> ());

        assert (ref.first && "key NOT found!");
        return ref;
    }

    template <str_view_like U>
    constexpr const_reference at (U const& key_str) const noexcept
    {
        const_reference ref = base_type::operator [] (get_index (key_str.data ()));

        assert (ref.first && "key NOT found!");
        return ref;
    }

    template <str_view_like U>
    constexpr reference at (U const& key_str) noexcept
    {
        reference ref = base_type::operator [] (get_index (key_str.data ()));

        assert (ref.first && "key NOT found!");
        return ref;
    }

    // ====================================================

    template <char_ptr K1, char_ptr K2, char_ptr... Ks>
    constexpr bool contains () const noexcept
    {
        return contains<K1> () && contains<K2> () && ((contains<Ks> ()) && ...);
    }

    template <key_type K1, key_type K2, key_type... Ks>
    constexpr bool contains () const noexcept
    {
        return contains<K1> () && contains<K2> () && ((contains<Ks> ()) && ...);
    }

    template <auto K1, auto K2, auto... Ks>
    requires (str_view_like<decltype (K1)> &&
              str_view_like<decltype (K2)> &&
             (str_view_like<decltype (Ks)> && ...))
    constexpr bool contains () const noexcept
    {
        return contains<K1> () && contains<K2> () && ((contains<Ks> ()) && ...);
    }

    template <c_const_str K1, c_const_str K2, c_const_str... Ks>
    constexpr bool contains (K1 key1, K2 key2, Ks... keys) const noexcept
    {
        return contains (key1) && contains (key2) && ((contains (keys)) && ...);
    }

    template <integer K1, integer K2, integer... Ks>
    constexpr bool contains (K1 key1, K2 key2, Ks... keys) const noexcept
    {
        return contains (key1) && contains (key2) && ((contains (keys)) && ...);
    }

    template <str_view_like K1, str_view_like K2, str_view_like... Ks>
    constexpr bool contains (K1 key1, K2 key2, Ks... keys) const noexcept
    {
        return contains (key1) && contains (key2) && ((contains (keys)) && ...);
    }

    // ====================================================

    template <key_type k>
    constexpr bool contains () const noexcept
    {
        return (*this)[k].first == k;
    }

    template <char_ptr Name>
    constexpr bool contains () const noexcept
    {
        return (*this)[Name].first == char_hash<Name> ();
    }

    template <integer Key>
    constexpr bool contains (Key k) const noexcept
    {
        return (*this)[k].first == k;
    }

    template <str_view_like U>
    constexpr bool contains (U const& name) const noexcept
    {
        return (*this)[name].first == char_hash (name.data (), name.size ());
    }

    template <c_const_str Key>
    constexpr bool contains (Key name) const noexcept
    {
        return (*this)[name].first == char_hash (name);
    }

    // ====================================================

    template <key_type Key>
    consteval size_type get_index () const noexcept
    { return (Key % size () + Key) % capacity (); }

    template <char_ptr Key>
    consteval size_type get_index () const noexcept
    { return get_index<char_hash<Key> ()> (); }

    template <auto k> requires (str_view_like<decltype (k)>)
    constexpr size_type get_index () const noexcept
    { return get_index<char_hash<k.data (), k.size ()> ()> (); }

    template <str_view_like U>
    constexpr size_type get_index (U const& name) const noexcept
    { return get_index (char_hash (name.data (), name.size ())); }

    template <c_const_str Key>
    constexpr size_type get_index (Key name) const noexcept
    { return get_index (char_hash (name)); }

    template <integer Key>
    constexpr size_type get_index (Key k) const noexcept
    { return (k % size () + k) % capacity (); }
};

// ====================================================

} // cppual

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_INDEX_MAP_H_
