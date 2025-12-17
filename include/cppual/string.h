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

#ifndef CPPUAL_TEXT_STRING_H_
#define CPPUAL_TEXT_STRING_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/concepts>
#include <cppual/iterator>
#include <cppual/noncopyable>
#include <cppual/memory_allocator>

#include <type_traits>
#include <string_view>
#include <iterator>
#include <sstream>
#include <cstddef>
#include <memory>
#include <string>
#include <cctype>

// ====================================================

namespace cppual {

// ====================================================

template <typename A>
concept allocator_or_void = memory::allocator_like<A> || void_t<A>;

// ====================================================

template <char_t T = char>
struct char_traits;

// ====================================================

template <char_t            T = char,
          struct_or_class   E = char_traits<T>,
          allocator_or_void A = memory::allocator<T>
          >
class cow_string;

// ====================================================

typedef cow_string<char  , char_traits<char  >, memory::allocator<char  >> fstring   ;
typedef cow_string<char8 , char_traits<char8 >, memory::allocator<char8 >> fu8string ;
typedef cow_string<char16, char_traits<char16>, memory::allocator<char16>> fu16string;
typedef cow_string<char32, char_traits<char32>, memory::allocator<char32>> fu32string;
typedef cow_string<wchar , char_traits<wchar >, memory::allocator<wchar >> fwstring  ;

// ====================================================

typedef cow_string<char  , char_traits<char  >, void> fstring_view   ;
typedef cow_string<char8 , char_traits<char8 >, void> fu8string_view ;
typedef cow_string<char16, char_traits<char16>, void> fu16string_view;
typedef cow_string<char32, char_traits<char32>, void> fu32string_view;
typedef cow_string<wchar , char_traits<wchar >, void> fwstring_view  ;

// ====================================================

typedef cow_string<uchar, char_traits<uchar>, memory::allocator<uchar>> byte_stream     ;
typedef cow_string<uchar, char_traits<uchar>, void>                     byte_stream_view;

// ====================================================

template <char_t T>
struct char_traits : public std::char_traits<T>
{ };

// ====================================================

/**
 ** @brief cow_string (fast string) is a string implementation with locale traits and
 ** small string optimization (SSO) for better performance with small strings.
 ** for SSO it allocates memory on the stack. for char & char8_t -> 15 chars,
 ** for char16_t -> 7 chars and for char32_t -> 3 chars.
 ** the last character is always '\0'.
 ** It can be used as a drop-in replacement for std::string_view
 **/
template <char_t T, struct_or_class E, allocator_or_void A>
class SHARED_API cow_string : private A
{
public:
    typedef cow_string<T, E, A>                        self_type             ;
    typedef E                                          traits_type           ;
    typedef memory::allocator_traits<A>                alloc_traits          ;
    typedef alloc_traits::allocator_type               allocator_type        ;
    typedef remove_cref_t<T>                           value_type            ;
    typedef value_type *                               pointer               ;
    typedef value_type const*                          const_pointer         ;
    typedef value_type &                               reference             ;
    typedef value_type const&                          const_reference       ;
    typedef alloc_traits::size_type                    size_type             ;
    typedef size_type const                            const_size            ;
    typedef ptrdiff                                    difference_type       ;
    typedef bi_iterator<self_type>                     iterator              ;
    typedef bi_iterator<self_type const>               const_iterator        ;
    typedef std::reverse_iterator<iterator>            reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
    typedef cow_string<T, char_traits<T>, void>        string_view           ;
    typedef std::char_traits<T>                        std_traits_type       ;
    typedef std::basic_string<T, std_traits_type, A>   std_string            ;
    typedef std::basic_string_view<T, std_traits_type> std_string_view       ;

    inline constexpr static const_size npos = size_type (-1);
    inline constexpr static cbool  is_cow_v = std::is_same_v<E, void>;

    union buffer
    {
        struct buffer_heap ;
        struct buffer_stack;

        inline constexpr static size_type const sso_capacity =
            sizeof (buffer_heap) / sizeof (value_type) - 1;
        inline constexpr static size_type const sso_bytes =
            sizeof (buffer_heap) - sizeof (value_type);

        struct buffer_heap
        {
            pointer   data     { };
            size_type capacity { };

            constexpr buffer_heap (buffer_heap const&) = default;

            constexpr buffer_heap () noexcept
            : data     ()
            , capacity ()
            { }

            constexpr buffer_heap (const_pointer pdata, const_size cap) noexcept
            : data (const_cast<pointer> (pdata)), capacity (cap)
            { }

            constexpr buffer_heap (pointer pdata, const_size cap) noexcept
            : data (pdata), capacity (cap)
            { }
        }
        heap;

        struct buffer_stack
        {
            value_type data[sso_capacity + 1];

            constexpr buffer_stack (buffer_stack const&) = default;
            constexpr buffer_stack () noexcept { }
        }
        stack;

        constexpr buffer (buffer &&)                 noexcept = default;
        consteval buffer (buffer const&)             noexcept = default;
        constexpr buffer& operator = (buffer &&)     noexcept = default;
        constexpr buffer& operator = (buffer const&) noexcept = default;

        constexpr buffer (const_pointer pdata, const_size cap) noexcept
        : heap (pdata, cap)
        { }

        constexpr buffer (pointer pdata, const_size cap) noexcept
        : heap (pdata, cap)
        { }

        constexpr buffer (const_size cap) noexcept
        : heap (pointer (), cap)
        { }

        constexpr buffer () noexcept
        : heap ()
        { }
    };

    constexpr cow_string () noexcept = default;

    constexpr cow_string (std::nullptr_t) noexcept
    : self_type ()
    { }

    constexpr cow_string (const_pointer pText, const_size len = npos) noexcept
    : allocator_type ()
    , _M_uLength (len == npos ? size (pText) : len)
    , _M_gBuffer (pText, _M_uLength)
    { }

    constexpr cow_string (allocator_type const& ator) noexcept
    : allocator_type (ator)
    , _M_uLength     (    )
    , _M_gBuffer     (    )
    { }

    constexpr cow_string (string_view const& sv, bool copy = !is_cow_v) noexcept
    : allocator_type ()
    , _M_uLength (sv.size ())
    , _M_gBuffer (copy ? is_on_stack () ? buffer () : buffer (size ()) : buffer (sv.data (), size ()))
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (sv.begin (), sv.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + size ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, sv.data (), size ());
        }
    }

    constexpr cow_string (std_string const& str, bool copy = !is_cow_v) noexcept
    : allocator_type ()
    , _M_uLength (str.size ())
    , _M_gBuffer (copy ? is_on_stack () ? buffer () : buffer (size ()) : buffer (str.data (), size ()))
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (str.begin (), str.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + size ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, str.data (), size ());
        }
    }

    constexpr explicit cow_string (const_pointer pText,
                                   allocator_type const& ator,
                                   bool copy = !is_cow_v)
    : allocator_type (ator)
    , _M_uLength (size (pText))
    , _M_gBuffer (copy ? is_on_stack () ? buffer () : buffer (size ()) : buffer (pText, _M_uLength))
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (pText, pText + _M_uLength, _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + size ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, pText, size ());
        }
    }

    template <common_iterator Iterator>
    constexpr cow_string (Iterator first, Iterator last,
                       allocator_type const& ator = allocator_type (),
                       bool copy = !is_cow_v)
    : allocator_type (ator)
    , _M_uLength (last - first)
    , _M_gBuffer (copy ? is_on_stack () ? buffer () : buffer (size ()) : buffer (first, _M_uLength))
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (first, last, _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + size ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, first, size ());
        }
    }

    constexpr cow_string (std::initializer_list<value_type> list,
                       allocator_type const& ator = allocator_type ())
    : allocator_type (ator)
    , _M_uLength (list.size ())
    , _M_gBuffer ()
    {
        if (is_on_stack ())
        {
            std::copy (list.begin (), list.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + size ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, list.begin (), size ());
        }
    }

    constexpr cow_string (self_type const& rh)
    : allocator_type (rh.allocator_type::select_on_container_copy_construction ())
    , _M_uLength (rh.size ())
    , _M_gBuffer (!is_cow_v ? rh.is_on_stack () ? buffer () : buffer (rh.size ()) :
                                                  buffer (rh.data (), _M_uLength))
    {
        if (is_on_stack ())
        {
            std::copy (rh.begin (), rh.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + size ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, rh.begin (), size ());
        }
    }


    constexpr cow_string (self_type&& rh) noexcept
    : allocator_type (rh)
    , _M_uLength     (rh._M_uLength)
    , _M_gBuffer     (rh._M_gBuffer)
    {
        rh._M_uLength = size_type ();
        rh._M_gBuffer = buffer    ();
    }

    constexpr cow_string (size_type uCapacity)
    : self_type ()
    {
        if (uCapacity > buffer::sso_capacity)
        {
            _M_gBuffer.heap.data     = allocator_type::allocate (uCapacity + 1);
            _M_gBuffer.heap.capacity = (_M_gBuffer.heap.data ? uCapacity : size_type ());
        }
    }

    constexpr ~cow_string ()
    {
        if (!is_on_stack () && size () > buffer::sso_capacity && !is_cow_v)
            allocator_type::deallocate (_M_gBuffer.heap.data, capacity () + 1);
    }

    constexpr self_type& operator = (self_type&& rh) noexcept
    {
        if (this == &rh) return *this;

        if (is_cow_v)
        {
            _M_gBuffer = buffer (rh.data (), rh.size ());
            return *this;
        }

        if (!is_on_stack () && size () > buffer::sso_capacity)
            allocator_type::deallocate (_M_gBuffer.heap.data, capacity () + 1);

        _M_uLength = rh._M_uLength;
        _M_gBuffer = rh._M_gBuffer;

        rh = self_type ();
        return *this;
    }

    constexpr self_type& operator = (self_type const& rh) noexcept
    {
        if (this == &rh) return *this;

        if (is_cow_v)
        {
            _M_gBuffer = buffer (rh.data (), rh.size ());
            return *this;
        }

        return assign_to_string (*this, rh.data (), rh.size ());
    }

    constexpr self_type& operator = (std_string const& rh) noexcept
    {
        return assign_to_string (*this, rh.data (), rh.size ());
    }

    constexpr self_type& operator = (std_string&& rh) noexcept
    {
        assign_to_string (*this, rh.data (), rh.size ());
        rh.clear ();

        return *this;
    }

    constexpr self_type& operator = (const_pointer pText) noexcept
    {
        if (is_cow_v)
        {
            _M_gBuffer = buffer (pText, size (pText));
            return *this;
        }

        return assign_to_string (*this, pText, size (pText));
    }

    /**
     * Explicitly converts to const char*.
     */
    // consteval explicit operator const_pointer () const noexcept
    // {
    //     return _M_gBuffer.heap.data;
    // }

    /**
     * Explicitly converts to std::string.
     */
    constexpr explicit operator std_string () noexcept
    {
        return std_string (_M_gBuffer.heap.data, size (_M_gBuffer.heap.data));
    }

    constexpr operator string_view () const noexcept
    {
        return string_view (_M_gBuffer.heap.data, size (_M_gBuffer.heap.data));
    }

    constexpr operator std_string_view () const noexcept
    {
        return std_string_view (_M_gBuffer.heap.data, size (_M_gBuffer.heap.data));
    }

    /**
     ** @brief str()
     ** Returns the contained string as an std::string.
     **/
    constexpr std_string str () noexcept
    {
        return std_string (_M_gBuffer.heap.data, size (_M_gBuffer.heap.data));
    }

    self_type& assign (self_type const& str);
    self_type& assign (self_type&& str) noexcept;
    self_type& assign (size_type count, value_type ch);
    self_type& assign (const_pointer s, size_type count);
    self_type& assign (const_pointer s);

    template <str_view_like U>
    self_type& assign (U const& t);

    template <str_view_like U>
    self_type& assign (U const& t, size_type pos, size_type count = npos);
    self_type& assign (self_type const& str, size_type pos, size_type count = npos);

    template <input_iterator It>
    self_type& assign (It first, It last);
    self_type& assign (std::initializer_list<value_type> ilist);

    bool      contains    (string_view sv) const noexcept;
    bool      contains    (value_type ch) const noexcept;
    bool      contains    (const_pointer str) const;
    self_type substr      (size_type begin_pos, size_type end_pos = npos);
    bool      starts_with (string_view sv) const noexcept;
    bool      starts_with (value_type ch) const noexcept;
    bool      starts_with (const_pointer str) const;
    bool      ends_with   (string_view sv) const noexcept;
    bool      ends_with   (value_type ch) const noexcept;
    bool      ends_with   (const_pointer str) const;
    void      resize      (size_type count);
    void      resize      (size_type count, value_type ch);
    size_type copy        (pointer dest, size_type count, size_type pos = size_type ()) const;
    void      push_back   (value_type ch);
    void      clear       () noexcept;
    void      find        (self_type const& str, size_type pos = 0, size_type count = 0);
    void      find        (value_type const& str, size_type pos = 0, size_type count = 0);
    void      find        (value_type ch, size_type pos = 0);

    template <str_view_like U>
    void find  (U const& sv, size_type pos = 0, size_type count = 0);

    void rfind (self_type const& str, size_type pos = npos, size_type count = 0);
    void rfind (value_type const& str, size_type pos = npos, size_type count = 0);
    void rfind (value_type ch, size_type pos = npos);

    template <str_view_like U>
    void rfind (U const& sv, size_type pos = npos, size_type count = 0);

    constexpr const_pointer  c_str         () const noexcept { return  _M_gBuffer.heap.data; }
    constexpr const_pointer  data          () const noexcept { return  _M_gBuffer.heap.data; }
    constexpr pointer        data          ()       noexcept { return  _M_gBuffer.heap.data; }
    constexpr size_type      size          () const noexcept { return  _M_uLength          ; }
    constexpr size_type      length        () const noexcept { return  size ()             ; }
    constexpr bool           empty         () const noexcept { return !_M_uLength          ; }
    consteval allocator_type get_allocator () const noexcept { return *this                ; }

    constexpr static size_type size (const_pointer pText) noexcept
    { return traits_type::length (pText); }

    constexpr static size_type length (const_pointer pText) noexcept
    { return size (pText); }

    constexpr size_type size_bytes () const noexcept
    { return  _M_uLength * sizeof (value_type); }

    constexpr size_type capacity () const noexcept
    { return  is_on_stack () ? buffer::sso_capacity : _M_gBuffer.heap.capacity; }

    //! full means the allocated memory equals the length of the string (capacity == size)
    constexpr bool full () const noexcept
    { return  capacity () == size (); }

    //! how many characters remain to fill the whole allocated memory (capacity > size)
    constexpr size_type space_remaining () const noexcept
    { return  capacity () - size (); }

    constexpr iterator begin () noexcept
    {  return iterator (*this, size_type ()); }

    constexpr const_iterator begin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr iterator end () noexcept
    {  return iterator (*this, size ()); }

    constexpr const_iterator end () const noexcept
    {  return const_iterator (*this, size ()); }

    constexpr const_iterator cend () noexcept
    {  return const_iterator (*this, size ()); }

    constexpr const_iterator cend () const noexcept
    {  return const_iterator (*this, size ()); }

    constexpr reverse_iterator rbegin () noexcept
    {  return reverse_iterator (*this, size () - 1); }

    constexpr const_reverse_iterator rbegin () const noexcept
    {  return const_reverse_iterator (*this, size () - 1); }

    constexpr const_reverse_iterator crbegin () noexcept
    {  return const_reverse_iterator (*this, size () - 1); }

    constexpr const_reverse_iterator crbegin () const noexcept
    {  return const_reverse_iterator (*this, size () - 1); }

    constexpr reverse_iterator rend () noexcept
    {  return reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator rend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr reference front () noexcept
    { return *_M_gBuffer.heap.data; }

    constexpr const_reference front () const noexcept
    { return *_M_gBuffer.heap.data; }

    constexpr reference back () noexcept
    { return *(_M_gBuffer.heap.data + (size () - 1)); }

    constexpr const_reference back () const noexcept
    { return *(_M_gBuffer.heap.data + (size () - 1)); }

    constexpr reference at (size_type uPos) noexcept
    {
        assert  (uPos < size () && "pos out of range (equal or larger than the size)!");
        return *(_M_gBuffer.heap.data + uPos);
    }

    constexpr const_reference at (size_type uPos) const noexcept
    {
        assert  (uPos < size () && "pos out of range (equal or larger than the size)!");
        return *(_M_gBuffer.heap.data + uPos);
    }

    consteval reference operator [] (size_type uPos) noexcept
    { return *(_M_gBuffer.heap.data + uPos); }

    consteval const_reference operator [] (size_type uPos) const noexcept
    { return *(_M_gBuffer.heap.data + uPos); }

    inline void pop_back () noexcept
    {
        back () = value_type ();
        --_M_uLength;
    }

    inline void reserve (size_type new_cap)
    {
        if (new_cap <= capacity ()) return;

        pointer pNewData = allocator_type::allocate (new_cap + 1);

        std::copy (begin (), end (), pNewData);
        if (!is_on_stack ()) allocator_type::deallocate (data (), capacity () + 1);

        _M_gBuffer.heap.data     = pNewData;
        _M_gBuffer.heap.capacity = new_cap;
    }

    inline void shrink_to_fit ()
    {
        if (is_on_stack () || full ()) return;

        const_size new_cap = size ();
        pointer   pNewData = allocator_type::allocate (new_cap + 1);

        std::copy (begin (), end (), pNewData);
        allocator_type::deallocate (data (), capacity () + 1);

        _M_gBuffer.heap.data     = pNewData;
        _M_gBuffer.heap.capacity = _M_uLength = new_cap;
    }

    inline void erase () noexcept
    {
        if (!_M_uLength || !_M_gBuffer.heap.data) return;
        _M_gBuffer.heap.data[0] = value_type ();
        _M_uLength              = size_type  ();
    }

    template <char_t U, struct_or_class _E, allocator_or_void _A, common_iterator Iterator>
    friend cow_string<U, _E, _A>& copy_to_string (cow_string<U, _E, _A>& copy_to,
                                               Iterator            copy_from,
                                               size_type           length);

    template <char_t U, struct_or_class _E, allocator_or_void _A, common_iterator Iterator>
    friend cow_string<U, _E, _A>& assign_to_string (cow_string<U, _E, _A>& assign_to,
                                                 Iterator            assign_from,
                                                 size_type           length);

    template <char_t U, struct_or_class _E, allocator_or_void _A, common_iterator Iterator>
    friend cow_string<U, _E, _A>& add_to_string (cow_string<U, _E, _A>& add_to,
                                              Iterator            add_from,
                                              size_type           add_length);

    template <char_t U, struct_or_class _E, allocator_or_void _A>
    friend constexpr cow_string<U, _E, _A> operator + (cow_string<U, _E, _A> const& obj1,
                                          cow_string<U, _E, _A> const& obj2) noexcept;

    template <char_t U, struct_or_class _E, allocator_or_void _A>
    friend constexpr cow_string<U, _E, _A> operator + (cow_string<U, _E, _A> const& obj1, U const* obj2) noexcept;

    template <char_t U, struct_or_class _E, allocator_or_void _A>
    friend constexpr cow_string<U, _E, _A>& operator += (cow_string<U, _E, _A>& obj1,
                                            cow_string<U, _E, _A> const& obj2) noexcept;

    template <char_t U, struct_or_class _E, allocator_or_void _A>
    friend constexpr cow_string<U, _E, _A>& operator += (cow_string<U, _E, _A>& obj1, U const* obj2) noexcept;

    template <char_t U, struct_or_class _E, allocator_or_void _A>
    friend constexpr void swap (cow_string<U, _E, _A>& lhs, cow_string<U, _E, _A>& rhs) noexcept;

private:
    constexpr bool is_on_stack () const noexcept
    { return _M_uLength <= buffer::sso_capacity; }

private:
    size_type _M_uLength { };
    buffer    _M_gBuffer { };
};

// ====================================================

template <char_t T, struct_or_class E>
class SHARED_API cow_string <T, E, void>
{
public:
    typedef cow_string<T, E, void>                                         self_type             ;
    typedef E                                                              traits_type           ;
    typedef T                                                              value_type            ;
    typedef value_type *                                                   pointer               ;
    typedef value_type const*                                              const_pointer         ;
    typedef value_type &                                                   reference             ;
    typedef value_type const&                                              const_reference       ;
    typedef std::size_t                                                    size_type             ;
    typedef size_type const                                                const_size            ;
    typedef ptrdiff                                                        difference_type       ;
    typedef bi_iterator<self_type>                                         iterator              ;
    typedef bi_iterator<self_type const>                                   const_iterator        ;
    typedef std::reverse_iterator<iterator>                                reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>                          const_reverse_iterator;
    typedef cow_string<value_type>::allocator_type                         allocator_type        ;
    typedef cow_string<value_type, char_traits<value_type>, void>          string_view           ;
    typedef std::char_traits<value_type>                                   std_traits_type       ;
    typedef std::basic_string<value_type, std_traits_type, allocator_type> std_string            ;
    typedef std::basic_string_view<value_type, std_traits_type>            std_string_view       ;

    inline constexpr static const_size npos = size_type (-1);
    inline constexpr static cbool  is_cow_v = std::is_same_v<E, void>;

    union buffer
    {
        struct buffer_heap ;
        struct buffer_stack;

        inline constexpr static size_type const sso_capacity =
            sizeof (buffer_heap) / sizeof (value_type) - 1;
        inline constexpr static size_type const sso_bytes =
            sizeof (buffer_heap) - sizeof (value_type);

        struct buffer_heap
        {
            pointer   data     { };
            size_type capacity { };

            constexpr buffer_heap (buffer_heap const&) = default;

            constexpr buffer_heap () noexcept
            : data     ()
            , capacity ()
            { }

            constexpr buffer_heap (const_pointer pdata, const_size cap) noexcept
            : data (const_cast<pointer> (pdata)), capacity (cap)
            { }

            constexpr buffer_heap (pointer pdata, const_size cap) noexcept
            : data (pdata), capacity (cap)
            { }
        }
        heap;

        struct buffer_stack
        {
            value_type data[sso_capacity + 1];

            constexpr buffer_stack (buffer_stack const&) = default;
            constexpr buffer_stack () noexcept { }
        }
        stack;

        constexpr buffer (buffer &&)                 noexcept = default;
        consteval buffer (buffer const&)             noexcept = default;
        constexpr buffer& operator = (buffer &&)     noexcept = default;
        constexpr buffer& operator = (buffer const&) noexcept = default;

        constexpr buffer (const_pointer pdata, const_size cap) noexcept
        : heap (pdata, cap)
        { }

        constexpr buffer (pointer pdata, const_size cap) noexcept
        : heap (pdata, cap)
        { }

        constexpr buffer (const_size cap) noexcept
        : heap (pointer (), cap)
        { }

        constexpr buffer () noexcept
        : heap ()
        { }
    };

    constexpr cow_string () noexcept = default;

    constexpr cow_string (std::nullptr_t) noexcept
    : self_type ()
    { }
};

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
cow_string<T, E, A> cow_string<T, E, A>::substr (size_type uBeginPos, size_type uEndPos)
{
    if (empty () || static_cast<difference_type> (uEndPos - uBeginPos) <= 0 || uBeginPos >= _M_uLength)
        return self_type ();

    self_type gSubStr (uEndPos - uBeginPos);
    //size_type i = 0;

    std::copy (begin () + uBeginPos,
               begin () + uBeginPos + gSubStr.capacity (), gSubStr.begin ());

    //while (uBeginPos <= uEndPos && i < _M_uLength)
    //    gSubStr._M_gBuffer.heap.data[i++] = _M_gBuffer.heap.data[uBeginPos++];

    *gSubStr.end () = value_type ();

    return std::move (gSubStr);
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A, common_iterator Iterator>
cow_string<T, E, A>& copy_to_string (cow_string<T, E, A>&                    rh,
                                     Iterator                                pFromText,
                                     typename cow_string<T, E, A>::size_type uLength)
{
    if (!pFromText) return rh;

    if (uLength > rh.capacity ())
    {
        if (!rh.is_on_stack () && !rh.is_cow_v)
             rh.deallocate (rh.data (), rh.capacity () + 1);

        if (!(rh._M_gBuffer.heap.data = rh.allocate (uLength + 1)))
        {
            rh._M_uLength    = 0;
            rh.stack.data[0] = cow_string<T, E, A>::value_type ();
            return rh;
        }

        rh._M_gBuffer.heap.capacity = uLength;
    }

    std::copy (pFromText, pFromText + uLength, rh._M_gBuffer.heap.data);
    rh._M_uLength = uLength;
    rh._M_gBuffer.heap.is_copied = true;

    rh._M_gBuffer.heap.data[uLength] = typename cow_string<T, E, A>::value_type ();
    return rh;
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A, common_iterator Iterator>
cow_string<T, E, A>& assign_to_string (cow_string<T, E, A>&                    rh,
                                       Iterator                                pFromText,
                                       typename cow_string<T, E, A>::size_type uLength)
{
    if (!pFromText) return rh;

    if (uLength > rh.capacity ())
    {
        if (!rh.is_cow_v) rh.deallocate (rh.data (), rh.capacity () + 1);

        if (!(rh._M_gBuffer.heap.data = rh.allocate (uLength + 1)))
        {
            rh._M_uLength    = 0;
            rh.stack.data[0] = cow_string<T, E, A>::value_type ();
            return rh;
        }

        rh._M_gBuffer.heap.capacity = uLength;
    }

    std::copy (pFromText, pFromText + uLength, rh._M_gBuffer.heap.data);
    rh._M_uLength = uLength;

    rh._M_gBuffer.heap.data[uLength] = cow_string<T, E, A>::value_type ();
    return rh;
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A, common_iterator Iterator>
cow_string<T, E, A>& add_to_string (cow_string<T, E, A>& rh,
                                    Iterator             pFromText,
                                    std::size_t          uAddLength)
{
    auto const uLength = rh.size () + uAddLength;

    if (uLength > rh.capacity ())
    {
        auto const pOldBuffer   = rh.data     ();
        auto const uOldCapacity = rh.capacity ();

        if (!(rh._M_gBuffer.heap.data = rh.allocate (uLength + 1))) return rh;

        std::copy (pOldBuffer, pOldBuffer + rh.size (), rh.data ());

        rh._M_gBuffer.heap.capacity = uLength;
        if (!rh.is_cow_v) rh.deallocate (pOldBuffer, uOldCapacity + 1);
    }

    std::copy (pFromText, pFromText + ++uAddLength, rh.data () + rh.size ());

    rh._M_uLength = uLength;
    return rh;
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator <=> (cow_string<T, E, A> const& lhs, cow_string<T, E, A> const& rhs) noexcept
{
    return cow_string<T, E, void> (lhs) <=> cow_string<T, E, void> (rhs);
}

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator <=> (cow_string<T, E, A> const& lhs, T const* rhs) noexcept
{
    return cow_string<T, E, void> (lhs) <=> cow_string<T, E, void> (rhs);
}

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator <=> (T const* lhs, cow_string<T, E, A> const& rhs) noexcept
{
    return cow_string<T, E, void> (lhs) <=> cow_string<T, E, void> (rhs);
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator == (cow_string<T, E, A> const& lhs,
                            cow_string<T, E, A> const& rhs) noexcept
{
    return cow_string<T, E, void> (lhs) == cow_string<T, E, void> (rhs);
}

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator == (cow_string<T, E, A> const& lhs, T const* rhs) noexcept
{
    return cow_string<T, E, void> (lhs) == cow_string<T, E, void> (rhs);
}

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator == (T const* lhs, cow_string<T, E, A> const& rhs) noexcept
{
    return cow_string<T, E, void> (lhs) == cow_string<T, E, void> (rhs);
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator != (cow_string<T, E, A> const& lh,
                            cow_string<T, E, A> const& rh) noexcept
{ return !(lh == rh); }

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator != (cow_string<T, E, A> const& lh, T const* pText2) noexcept
{ return !(lh == pText2); }

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr bool operator != (T const* pText1, cow_string<T, E, A> const& rh) noexcept
{ return !(pText1 == rh); }

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr cow_string<T, E, A>& operator += (cow_string<T, E, A>& lh,
                                            cow_string<T, E, A> const& rh) noexcept
{ return add_to_string (lh, rh._M_gBuffer.heap.data, rh._M_uLength); }

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr cow_string<T, E, A>& operator += (cow_string<T, E, A>& rh, T const* pText) noexcept
{ return add_to_string (rh, pText, str_size (pText)); }

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr cow_string<T, E, A> operator + (cow_string<T, E, A> const& lh,
                                          cow_string<T, E, A> const& rh) noexcept
{
    cow_string<T, E, A> gStr (lh);
    return std::move (add_to_string (gStr, rh._M_gBuffer.heap.data, rh._M_uLength));
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr cow_string<T, E, A> operator + (cow_string<T, E, A> const& lh, T const* pText) noexcept
{
    cow_string<T, E, A> gStr (lh);
    return std::move (add_to_string (gStr, pText, str_size (pText)));
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr cow_string<T, E, A> operator + (T const* pText, cow_string<T, E, A> const& rh) noexcept
{
    cow_string<T, E, A> gStr (pText);
    return std::move (add_to_string (gStr, rh._M_gBuffer.heap.data, rh._M_uLength));
}

// ====================================================

template <char_t T, struct_or_class E, allocator_or_void A>
constexpr void swap (cow_string<T, E, A>& lhs, cow_string<T, E, A>& rhs) noexcept
{
    std::swap (lhs._M_gBuffer, rhs._M_gBuffer);
    std::swap (lhs._M_uLength, rhs._M_uLength);
}

// ====================================================

template <char_t T = char>
using std_string = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

template <char_t T = char>
using std_isstream = std::basic_istringstream<T, std::char_traits<T>, std::allocator<T>>;

template <char_t T = char>
using std_osstream = std::basic_ostringstream<T, std::char_traits<T>, std::allocator<T>>;
template <char_t T = char>
using std_sstream = std::basic_stringstream<T, std::char_traits<T>, std::allocator<T>>;

// ====================================================

template <char_t T = char>
using used_string = std::basic_string<T, std::char_traits<T>, memory::allocator<T>>;

template <char_t T = char>
using used_isstream = std::basic_istringstream<T, std::char_traits<T>, memory::allocator<T>>;

template <char_t T = char>
using used_osstream = std::basic_ostringstream<T, std::char_traits<T>, memory::allocator<T>>;
template <char_t T = char>
using used_sstream = std::basic_stringstream<T, std::char_traits<T>, memory::allocator<T>>;

// ====================================================

/// is of string type
template <typename>
struct is_string : std::false_type
{ };

/// is std::basic_string type
template <memory::allocator_like A>
struct is_string <std::basic_string<char, std::char_traits<char>, A>> : std::true_type
{ };

/// is std::basic_string type
template <memory::allocator_like A>
struct is_string<std::basic_string<char8, std::char_traits<char8>, A>> : std::true_type
{ };

/// is std::basic_string type
template <memory::allocator_like A>
struct is_string <std::basic_string<char16, std::char_traits<char16>, A>> : std::true_type
{ };

/// is std::basic_string type
template <memory::allocator_like A>
struct is_string <std::basic_string<char32, std::char_traits<char32>, A>> : std::true_type
{ };

/// is std::basic_string type
template <memory::allocator_like A>
struct is_string <std::basic_string<wchar, std::char_traits<wchar>, A>> : std::true_type
{ };

/// is std::basic_string type
template <>
struct is_string <std::basic_string_view<char, std::char_traits<char>>> : std::true_type
{ };

/// is std::basic_string type
template <>
struct is_string <std::basic_string_view<char8, std::char_traits<char8>>> : std::true_type
{ };

/// is std::basic_string type
template <>
struct is_string <std::basic_string_view<char16, std::char_traits<char16>>> : std::true_type
{ };

/// is std::basic_string type
template <>
struct is_string <std::basic_string_view<char32, std::char_traits<char32>>> : std::true_type
{ };

/// is std::basic_string type
template <>
struct is_string <std::basic_string_view<wchar, std::char_traits<wchar>>> : std::true_type
{ };

template <memory::allocator_like A>
struct is_string <cow_string<char, A>> : std::true_type
{ };

template <memory::allocator_like A>
struct is_string <cow_string<char8, A>> : std::true_type
{ };

template <memory::allocator_like A>
struct is_string <cow_string<char16, A>> : std::true_type
{ };

template <memory::allocator_like A>
struct is_string <cow_string<char32, A>> : std::true_type
{ };

template <memory::allocator_like A>
struct is_string <cow_string<wchar, A>> : std::true_type
{ };

/// is of string type -> value
template <typename T>
inline constexpr auto const is_string_v = is_string<T>::value;

template <typename T>
using StringType = std::enable_if_t<is_string_v<T>, T>;

template <typename T>
concept string_t = is_string_v<T>;

} // namespace cppual

// ====================================================

// namespace std {

//  using cppual::char_t;
// using cppual::memory::allocator_like;

// template <char_t Char>
// struct hash<basic_string<Char, std::traits_type<Char>, cppual::memory::allocator<Char>>>
// {
//    typedef basic_string<Char, std::traits_type<Char>, cppual::memory::allocator<Char>> string_type;

//    size_t operator () (string_type const& value) const
//    {
//        return cppual::consteval_char_hash<value.c_str ()> ();
//    }
// };

// template <template <char_t T = char, allocator_like A = cppual::memory::allocator<T>>
// class uses_allocator <cppual::fstring<T, E, A>, A> : public std::true_type { };

// ====================================================

// } // namespace std

// ====================================================

namespace std {

using cppual::char_t           ;
using cppual::struct_or_class  ;
using cppual::cow_string       ;
using cppual::allocator_or_void;

template <char_t T, struct_or_class E, allocator_or_void A>
struct uses_allocator <cow_string<T, E, A>, A> : std::true_type
{ };

} // namespace std

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_TEXT_STRING_H_
