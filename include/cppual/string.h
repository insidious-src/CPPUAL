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

#ifndef CPPUAL_COW_STRING_H_
#define CPPUAL_COW_STRING_H_
#ifdef __cplusplus

#include <cppual/types>
#include <cppual/concepts>
#include <cppual/iterator>
#include <cppual/containers>
#include <cppual/noncopyable>
#include <cppual/meta_string>
#include <cppual/memory_allocator>

#include <string_view>
#include <type_traits>
#include <iterator>
#include <sstream>
#include <cstddef>
#include <memory>
#include <string>
//#include <cctype>
#include <ranges>

// ====================================================

namespace cppual {

// ====================================================

template <typename A>
concept allocator_or_void = allocator_like<A> || void_t<A>;

// ====================================================

template <symbolic_char T = char>
struct locale_traits;

// ====================================================

template <symbolic_char     T = char,
          structure         E = locale_traits<T>,
          allocator_or_void A = memory::allocator<T>
          >
class cow_string;

// ====================================================

typedef cow_string<char  , locale_traits<char  >, memory::allocator<char  >> fstring   ;
typedef cow_string<char8 , locale_traits<char8 >, memory::allocator<char8 >> fu8string ;
typedef cow_string<char16, locale_traits<char16>, memory::allocator<char16>> fu16string;
typedef cow_string<char32, locale_traits<char32>, memory::allocator<char32>> fu32string;
typedef cow_string<wchar , locale_traits<wchar >, memory::allocator<wchar >> fwstring  ;

// ====================================================

typedef cow_string<char  , locale_traits<char  >, void> fstring_view   ;
typedef cow_string<char8 , locale_traits<char8 >, void> fu8string_view ;
typedef cow_string<char16, locale_traits<char16>, void> fu16string_view;
typedef cow_string<char32, locale_traits<char32>, void> fu32string_view;
typedef cow_string<wchar , locale_traits<wchar >, void> fwstring_view  ;

// ====================================================

typedef cow_string<uchar, locale_traits<uchar>, memory::allocator<uchar>> byte_stream     ;
typedef cow_string<uchar, locale_traits<uchar>, void>                     byte_stream_view;

// ====================================================

template <symbolic_char T>
struct locale_traits : public std::char_traits<T>
{ };

template <auto C_STR> requires (c_const_tstr<decltype (C_STR)>)
inline constexpr const std::size_t c_str_length_v =
locale_traits<remove_cvptr_t<decltype (C_STR)>>::length (C_STR);

// ====================================================

/**
 ** @brief cow_string (fast string) is a string implementation with locale traits and
 ** small string optimization (SSO) for better performance with small strings.
 ** for SSO it allocates memory on the stack. for char & char8_t -> 15 chars,
 ** for char16_t -> 7 chars and for char32_t -> 3 chars.
 ** the last character is always '\0'.
 ** It can be used as a drop-in replacement for std::string_view
 **/
template <symbolic_char T, structure E, allocator_or_void A>
class SHARED_API cow_string : private A
{
public:
    typedef cow_string<T, E, A>                       self_type             ;
    typedef E                                         locale_type           ;
    typedef memory::allocator_traits<A>               alloc_traits          ;
    typedef alloc_traits::allocator_type              allocator_type        ;
    typedef remove_cvref_t<T>                         value_type            ;
    typedef value_type const                          const_value           ;
    typedef value_type *                              pointer               ;
    typedef value_type const*                         const_pointer         ;
    typedef value_type &                              reference             ;
    typedef value_type const&                         const_reference       ;
    typedef alloc_traits::size_type                   size_type             ;
    typedef size_type const                           const_size            ;
    typedef ptrdiff                                   difference_type       ;
    typedef bi_iterator<self_type>                    iterator              ;
    typedef bi_iterator<self_type const>              const_iterator        ;
    typedef std::reverse_iterator<iterator>           reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;
    typedef cow_string<value_type, locale_type, void> string_view           ;
    typedef string                                    std_string            ;
    typedef std::basic_string_view<value_type>        std_string_view       ;

    inline constexpr static const_size npos = size_type (-1);
    inline constexpr static cbool  is_cow_v = are_same<A, void>;

    union buffer
    {
        typedef buffer self_type;

        struct buffer_heap ;
        struct buffer_stack;

        inline constexpr static size_type const sso_capacity =
            sizeof (buffer_heap) / sizeof (value_type) - 1;
        inline constexpr static size_type const sso_bytes    =
            sizeof (buffer_heap) - sizeof (value_type);

        struct buffer_heap
        {
            typedef buffer_heap self_type;

            pointer   c_str    { };
            size_type capacity { };

            consteval buffer_heap ()                           noexcept = default;
            constexpr buffer_heap (self_type &&)               noexcept = default;
            constexpr buffer_heap (self_type const&)           noexcept = default;
            constexpr self_type& operator = (self_type &&)     noexcept = default;
            constexpr self_type& operator = (self_type const&) noexcept = default;

            constexpr buffer_heap (const_pointer pdata, const_size cap) noexcept
            : c_str (const_cast<pointer> (pdata)), capacity (cap)
            { }

            constexpr explicit buffer_heap (pointer pdata, const_size cap) noexcept
            : c_str (pdata), capacity (cap)
            { }
        }
        heap { };

        struct buffer_stack
        {
            typedef buffer_stack self_type;

            value_type data[sso_capacity + 1];

            consteval buffer_stack ()                          noexcept = default;
            constexpr buffer_stack (self_type &&)              noexcept = default;
            constexpr buffer_stack (self_type const&)          noexcept = default;
            constexpr self_type& operator = (self_type &&)     noexcept = default;
            constexpr self_type& operator = (self_type const&) noexcept = default;
        }
        stack;

        consteval buffer ()                                noexcept = default;
        constexpr buffer (self_type &&)                    noexcept = default;
        constexpr buffer (self_type const&)                noexcept = default;
        constexpr self_type& operator = (self_type &&)     noexcept = default;
        constexpr self_type& operator = (self_type const&) noexcept = default;

        constexpr buffer (const_pointer pdata, const_size cap) noexcept
        : heap (pdata, cap)
        { }

        constexpr buffer (pointer pdata, const_size cap) noexcept
        : heap (pdata, cap)
        { }

        constexpr buffer (const_size cap) noexcept
        : heap (pointer (), cap)
        { }
    };

    consteval cow_string () noexcept = default;

    constexpr cow_string (const_pointer pText, const_size len = npos) noexcept
    : allocator_type ()
    , _M_uLength     (len == npos ? size (pText) : len)
    , _M_gBuffer     (pText, _M_uLength)
    { }

    constexpr cow_string (allocator_type const& ator) noexcept
    : allocator_type (ator)
    , _M_uLength     (    )
    , _M_gBuffer     (    )
    { }

    constexpr cow_string (string_view const& sv, bool copy = !is_cow_v) noexcept
    : allocator_type ()
    , _M_uLength (sv.length ())
    , _M_gBuffer (copy ? is_on_stack () ? buffer () : buffer (length ()) : buffer (sv.data (), length ()))
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (sv.begin (), sv.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + length ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, sv.data (), length ());
        }
    }

    constexpr cow_string (std_string const& str, bool copy = !is_cow_v) noexcept
    : allocator_type ()
    , _M_uLength (str.length ())
    , _M_gBuffer (copy ? is_on_stack () ? buffer () : buffer (length ()) : buffer (str.data (), length ()))
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (str.begin (), str.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + length ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, str.data (), length ());
        }
    }

    constexpr explicit cow_string (const_pointer pText,
                                   allocator_type const& ator,
                                   bool copy = !is_cow_v)
    : allocator_type (ator)
    , _M_uLength     (size (pText))
    , _M_gBuffer     (copy ? is_on_stack () ? buffer () : buffer (length ()) : buffer (pText, _M_uLength))
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (pText, pText + _M_uLength, _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + length ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, pText, length ());
        }
    }

    template <generic_iterator Iterator>
    constexpr cow_string (Iterator first, Iterator last,
                          allocator_type const& ator = allocator_type (),
                          bool copy = !is_cow_v)
    : allocator_type (ator)
    , _M_gBuffer     (copy ? is_on_stack () ? buffer () : buffer (length ()) : buffer (first, _M_uLength))
    , _M_uLength     (last - first)
    {
        if (!copy) return;

        if (is_on_stack ())
        {
            std::copy (first, last, _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + length ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, first, length ());
        }
    }

    constexpr cow_string (std::initializer_list<value_type> list,
                          allocator_type const& ator = allocator_type ())
    : allocator_type (ator)
    , _M_gBuffer     ()
    , _M_uLength     (list.size ())
    {
        if (is_on_stack ())
        {
            std::copy (list.begin (), list.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + length ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, list.begin (), length ());
        }
    }

    constexpr cow_string (self_type const& rh)
    : allocator_type (rh.select_on_container_copy_construction ())
    , _M_gBuffer     (!is_cow_v ? rh.is_on_stack () ? buffer () : buffer (rh.length ()) :
                                                      buffer (rh.data (), _M_uLength))
    , _M_uLength     (rh.length ())
    {
        if (is_on_stack ())
        {
            std::copy (rh.begin (), rh.end (), _M_gBuffer.stack.data);
            *(_M_gBuffer.stack.data + length ()) = value_type ();
        }
        else
        {
            copy_to_string (*this, rh.begin (), length ());
        }
    }

    constexpr cow_string (self_type&& rh) noexcept
    : allocator_type (rh           )
    , _M_gBuffer     (rh._M_gBuffer)
    , _M_uLength     (rh._M_uLength)
    {
        rh._M_uLength = size_type ();
        rh._M_gBuffer = buffer    ();
    }

    constexpr cow_string (size_type uCapacity, allocator_type const& ator = allocator_type ())
    : allocator_type (ator)
    , _M_gBuffer     (uCapacity > buffer::sso_capacity ?
                      buffer (allocator_type::allocate (uCapacity + 1),
                              data () ? uCapacity : size_type ()) :
                      buffer ())
    , _M_uLength     ()
    { }

    constexpr ~cow_string ()
    {
        if (!is_on_stack () && size () > buffer::sso_capacity && !is_cow_v)
            allocator_type::deallocate (data (), capacity () + 1);
    }

    inline self_type& operator = (self_type&& rh) noexcept
    {
        if (this == &rh) return *this;

        if (is_cow_v)
        {
            _M_gBuffer = buffer (rh.data (), rh.length ());
            return *this;
        }

        if (!is_on_stack () && length () > buffer::sso_capacity)
            allocator_type::deallocate (data (), capacity () + 1);

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
    constexpr explicit operator const_pointer () const noexcept
    {
        return _M_gBuffer.heap.c_str;
    }

    /**
     * Explicit converts.
     */
    constexpr explicit operator std_string () noexcept
    {
        return std_string (_M_gBuffer.heap.c_str, length ());
    }

    constexpr explicit operator string_view () const noexcept
    {
        return string_view (_M_gBuffer.heap.c_str, length ());
    }

    constexpr explicit operator std_string_view () const noexcept
    {
        return std_string_view (_M_gBuffer.heap.c_str, length ());
    }

    /**
     ** @brief str()
     ** Returns the contained string as an std::string.
     **/
    constexpr std_string str () noexcept
    {
        return std_string (_M_gBuffer.heap.c_str, size (_M_gBuffer.heap.c_str));
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

    template <str_view_like U>
    bool      contains    (U const& sv) const noexcept;
    bool      contains    (value_type ch) const noexcept;
    bool      contains    (const_pointer str) const;
    self_type substr      (size_type begin_pos, size_type end_pos = npos);

    template <str_view_like U>
    bool      starts_with (U const& sv) const noexcept;
    bool      starts_with (value_type ch) const noexcept;
    bool      starts_with (const_pointer str) const;

    template <str_view_like U>
    bool      ends_with   (U const& sv) const noexcept;
    bool      ends_with   (value_type ch) const noexcept;
    bool      ends_with   (const_pointer str) const;
    void      resize      (size_type count);
    void      resize      (size_type count, value_type ch);
    size_type copy        (pointer dest, size_type count, size_type pos = size_type ()) const;
    void      push_back   (value_type ch);
    void      clear       () noexcept;

    template <str_view_like U>
    size_type  find (U const& sv, size_type pos = 0, size_type count = 0) const;
    size_type  find (self_type const& str, size_type pos = 0, size_type count = 0) const;
    size_type  find (value_type const& str, size_type pos = 0, size_type count = 0) const;
    size_type  find (value_type ch, size_type pos = 0) const;

    template <str_view_like U>
    size_type rfind (U const& sv, size_type pos = npos, size_type count = 0) const;
    size_type rfind (self_type const& str, size_type pos = npos, size_type count = 0) const;
    size_type rfind (value_type const& str, size_type pos = npos, size_type count = 0) const;
    size_type rfind (value_type ch, size_type pos = npos) const;

    template <str_view_like U>
    constexpr int compare (U const& sv) const noexcept;

    template <str_view_like U>
    constexpr int compare (size_type pos1, size_type count1, U const& sv) const;

    template <str_view_like U>
    constexpr int compare (size_type pos1, size_type count1, U const& sv,
                           size_type pos2, size_type count2 ) const;
    constexpr int compare (const_pointer str) const;
    constexpr int compare (size_type pos1, size_type count1, const_pointer str) const;
    constexpr int compare (size_type pos1, size_type count1, const_pointer str, size_type count2) const;

    template <str_view_like U>
    constexpr size_type find_first_of (U const& sv, size_type pos = 0) const noexcept;
    constexpr size_type find_first_of (value_type ch, size_type pos = 0) const noexcept;
    constexpr size_type find_first_of (const_pointer s, size_type pos, size_type count) const;
    constexpr size_type find_first_of (const_pointer s, size_type pos = 0 ) const;

    template <str_view_like U>
    constexpr size_type find_first_not_of (U const& sv, size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of (value_type ch, size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of (const_pointer s, size_type pos, size_type count) const;
    constexpr size_type find_first_not_of (const_pointer s, size_type pos = 0) const;

    template <str_view_like U>
    constexpr size_type find_last_of (U const& sv, size_type pos = npos) const noexcept;
    constexpr size_type find_last_of (value_type ch, size_type pos = npos) const noexcept;
    constexpr size_type find_last_of (const_pointer str, size_type pos, size_type count) const;
    constexpr size_type find_last_of (const_pointer str, size_type pos = npos) const;

    template <str_view_like U>
    constexpr size_type find_last_not_of (U const& sv, size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of (value_type ch, size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of (const_pointer s, size_type pos, size_type count) const;
    constexpr size_type find_last_not_of (const_pointer s, size_type pos = npos) const;

    inline    const_pointer  data          () const noexcept { return  _M_gBuffer.heap.c_str; }
    constexpr const_pointer  c_str         () const noexcept { return  data ()              ; }
    constexpr size_type      length        () const noexcept { return  _M_uLength           ; }
    constexpr size_type      size          () const noexcept { return  length ()            ; }
    constexpr bool           empty         () const noexcept { return !length ()            ; }
    constexpr allocator_type get_allocator () const noexcept { return *this                 ; }

    constexpr static size_type size (const_pointer str) noexcept
    { return locale_type::length (str); }

    constexpr size_type size_bytes () const noexcept
    { return  _M_uLength * sizeof (value_type); }

    constexpr size_type max_size () const noexcept
    {  return allocator_type::max_size (); }

    constexpr size_type capacity () const noexcept
    { return  is_on_stack () ? buffer::sso_capacity : _M_gBuffer.heap.capacity; }

    //! full means the allocated memory equals the length of the string (capacity == size)
    constexpr bool full () const noexcept
    { return  capacity () == length (); }

    //! how many characters remain to fill the whole allocated memory (capacity > size)
    constexpr size_type space_remaining () const noexcept
    { return  capacity () - length (); }

    constexpr iterator begin () noexcept
    {  return iterator (*this, size_type ()); }

    constexpr const_iterator begin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr iterator end () noexcept
    {  return iterator (*this, length ()); }

    constexpr const_iterator end () const noexcept
    {  return const_iterator (*this, length ()); }

    constexpr const_iterator cend () noexcept
    {  return const_iterator (*this, length ()); }

    constexpr const_iterator cend () const noexcept
    {  return const_iterator (*this, length ()); }

    constexpr reverse_iterator rbegin () noexcept
    {  return reverse_iterator (*this, length () - 1); }

    constexpr const_reverse_iterator rbegin () const noexcept
    {  return const_reverse_iterator (*this, length () - 1); }

    constexpr const_reverse_iterator crbegin () noexcept
    {  return const_reverse_iterator (*this, length () - 1); }

    constexpr const_reverse_iterator crbegin () const noexcept
    {  return const_reverse_iterator (*this, length () - 1); }

    constexpr reverse_iterator rend () noexcept
    {  return reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator rend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr reference front () noexcept
    { return *_M_gBuffer.heap.c_str; }

    constexpr const_reference front () const noexcept
    { return *_M_gBuffer.heap.c_str; }

    constexpr reference back () noexcept
    { return *(_M_gBuffer.heap.c_str + (length () - 1)); }

    constexpr const_reference back () const noexcept
    { return *(_M_gBuffer.heap.c_str + (length () - 1)); }

    constexpr reference at (size_type uPos) noexcept
    {
        assert  (uPos < size () && "pos out of range (equal or larger than the size)!");
        return *(_M_gBuffer.heap.c_str + uPos);
    }

    constexpr const_reference at (size_type uPos) const noexcept
    {
        assert  (uPos < length () && "pos out of range (equal or larger than the size)!");
        return *(_M_gBuffer.heap.c_str + uPos);
    }

    constexpr reference operator [] (size_type uPos) noexcept
    { return *(_M_gBuffer.heap.c_str + uPos); }

    constexpr const_reference operator [] (size_type uPos) const noexcept
    { return *(_M_gBuffer.heap.c_str + uPos); }

    constexpr void pop_back () noexcept
    {
        back () = value_type ();
        --_M_uLength;
    }

    constexpr void reserve (size_type new_cap)
    {
        if (new_cap <= capacity ()) return;

        pointer pNewData = allocator_type::allocate (new_cap + 1);

        std::copy (begin (), end (), pNewData);
        if (!is_on_stack ()) allocator_type::deallocate (data (), capacity () + 1);

        _M_gBuffer.heap.c_str    = pNewData;
        _M_gBuffer.heap.capacity = new_cap ;
    }

    constexpr void shrink_to_fit ()
    {
        if (is_on_stack () || full ()) return;

        const_size new_cap = size ();
        pointer   pNewData = allocator_type::allocate (new_cap + 1);

        std::copy (begin (), end (), pNewData);
        allocator_type::deallocate (data (), capacity () + 1);

        _M_gBuffer.heap.c_str    = pNewData;
        _M_gBuffer.heap.capacity = _M_uLength = new_cap;
    }

    constexpr void erase () noexcept
    {
        if (!_M_uLength || !_M_gBuffer.heap.data) return;
        _M_gBuffer.heap.c_str[0] = value_type ();
        _M_uLength               = size_type  ();
    }

    template <symbolic_char U, structure _E, allocator_like _A, generic_iterator Iterator>
    friend cow_string<U, _E, _A>& copy_to_string (cow_string<U, _E, _A>& copy_to,
                                                  Iterator            copy_from,
                                                  size_type           length);

    template <symbolic_char U, structure _E, allocator_like _A, generic_iterator Iterator>
    friend cow_string<U, _E, _A>& assign_to_string (cow_string<U, _E, _A>& assign_to,
                                                    Iterator            assign_from,
                                                    size_type           length);

    template <symbolic_char U, structure _E, allocator_like _A, generic_iterator Iterator>
    friend cow_string<U, _E, _A>& add_to_string (cow_string<U, _E, _A>& add_to,
                                                 Iterator            add_from,
                                                 size_type           add_length);

    template <symbolic_char U, structure _E, allocator_or_void _A>
    friend constexpr cow_string<U, _E> operator + (cow_string<U, _E, _A> const& obj1,
                                                   cow_string<U, _E, _A> const& obj2) noexcept;

    template <symbolic_char U, structure _E, allocator_or_void _A>
    friend constexpr cow_string<U, _E> operator + (cow_string<U, _E, _A> const& obj1,
                                                   U const* obj2) noexcept;

    template <symbolic_char U, structure _E, allocator_or_void _A1, allocator_like _A2>
    friend constexpr cow_string<U, _E, _A2>& operator += (cow_string<U, _E, _A2>& obj1,
                                                          cow_string<U, _E, _A1> const& obj2) noexcept;

    template <symbolic_char U, structure _E, allocator_or_void _A>
    friend constexpr cow_string<U, _E, _A>& operator += (cow_string<U, _E, _A>& obj1,
                                                         U const* obj2) noexcept;

    template <symbolic_char U, structure _E, allocator_or_void _A>
    friend constexpr void swap (cow_string<U, _E, _A>& lhs,
                                cow_string<U, _E, _A>& rhs) noexcept;

protected:
    constexpr bool is_on_stack () const noexcept
    { return _M_uLength <= buffer::sso_capacity; }

private:
    buffer    _M_gBuffer { };
    size_type _M_uLength { };
};

// ====================================================

template <symbolic_char T, structure E>
class SHARED_API cow_string <T, E, void>
{
public:
    typedef cow_string<T, E, void>                self_type             ;
    typedef E                                     locale_type           ;
    typedef T                                     value_type            ;
    typedef value_type const                      const_value           ;
    typedef value_type *                          pointer               ;
    typedef value_type const*                     const_pointer         ;
    typedef value_type &                          reference             ;
    typedef value_type const&                     const_reference       ;
    typedef std::size_t                           size_type             ;
    typedef size_type const                       const_size            ;
    typedef ptrdiff                               difference_type       ;
    typedef bi_iterator<self_type>                iterator              ;
    typedef bi_iterator<self_type const>          const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef cow_string<value_type, locale_type>   string_type           ;
    typedef std::basic_string_view<value_type>    std_string_view       ;

    typedef std::basic_string<value_type, std::char_traits<value_type>, memory::allocator<value_type>>
    std_string;

    inline constexpr static const_size npos = size_type (-1);

    constexpr static size_type size (const_pointer str) noexcept
    { return locale_type::length (str); }

    consteval cow_string () noexcept                            = default;
    constexpr cow_string (self_type &&)                         = default;
    constexpr cow_string (self_type const&) noexcept            = default;
    constexpr self_type& operator = (self_type &&) noexcept     = default;
    constexpr self_type& operator = (self_type const&) noexcept = default;

    constexpr cow_string (const_pointer str, const_size len = npos) noexcept
    : _M_c_str  (str)
    , _M_length (len == npos ? size (str) : len)
    { }

    constexpr cow_string (std_string_view sv) noexcept
    : _M_c_str  (sv.data ())
    , _M_length (sv.size ())
    { }

    constexpr cow_string (std_string const& str) noexcept
    : _M_c_str  (str.c_str  ())
    , _M_length (str.length ())
    { }

    constexpr cow_string (string_type const& str) noexcept
    : _M_c_str  (str.c_str  ())
    , _M_length (str.length ())
    { }

    /**
     ** Explicitly converts to const char*.
     **/
    constexpr explicit operator const_pointer () const noexcept
    { return _M_c_str; }

    /**
     ** Explicit converts.
     **/
    constexpr explicit operator std_string () noexcept
    { return std_string (_M_c_str, length ()); }

    constexpr explicit operator std_string_view () const noexcept
    { return std_string_view (_M_c_str, length ()); }

    /**
     ** @brief str()
     ** Returns the contained string as an std::string.
     **/
    constexpr std_string str () noexcept
    { return std_string (_M_c_str, size (_M_c_str)); }

    template <str_view_like U>
    bool      contains    (U const& sv) const noexcept;
    bool      contains    (value_type ch) const noexcept;
    bool      contains    (const_pointer str) const;
    self_type substr      (size_type begin_pos, size_type end_pos = npos);

    template <str_view_like U>
    bool      starts_with (U const& sv) const noexcept;
    bool      starts_with (value_type ch) const noexcept;
    bool      starts_with (const_pointer str) const;

    template <str_view_like U>
    bool      ends_with   (U const& sv) const noexcept;
    bool      ends_with   (value_type ch) const noexcept;
    bool      ends_with   (const_pointer str) const;
    size_type copy        (pointer dest, size_type count, size_type pos = size_type ()) const;

    template <str_view_like U>
    size_type  find (U const& sv, size_type pos = 0, size_type count = 0) const;
    size_type  find (self_type const& str, size_type pos = 0, size_type count = 0) const;
    size_type  find (value_type const& str, size_type pos = 0, size_type count = 0) const;
    size_type  find (value_type ch, size_type pos = 0) const;
    size_type  find (const_pointer str, size_type pos = 0) const;

    template <str_view_like U>
    size_type rfind (U const& sv, size_type pos = npos, size_type count = 0) const;
    size_type rfind (self_type const& str, size_type pos = npos, size_type count = 0) const;
    size_type rfind (value_type const& str, size_type pos = npos, size_type count = 0) const;
    size_type rfind (value_type ch, size_type pos = npos) const;
    size_type rfind (const_pointer str, size_type pos = npos) const;

    template <str_view_like U>
    constexpr int compare (U const& sv) const noexcept;

    template <str_view_like U>
    constexpr int compare (size_type pos1, size_type count1, U const& sv) const;

    template <str_view_like U>
    constexpr int compare (size_type pos1, size_type count1, U const& sv,
                           size_type pos2, size_type count2 ) const;
    constexpr int compare (const_pointer str) const;
    constexpr int compare (size_type pos1, size_type count1,  const_pointer str) const;
    constexpr int compare (size_type pos1, size_type count1, const_pointer str, size_type count2) const;

    template <str_view_like U>
    constexpr size_type find_first_of (U const& sv, size_type pos = 0) const noexcept;
    constexpr size_type find_first_of (value_type ch, size_type pos = 0) const noexcept;
    constexpr size_type find_first_of (const_pointer s, size_type pos, size_type count) const;
    constexpr size_type find_first_of (const_pointer s, size_type pos = 0 ) const;

    template <str_view_like U>
    constexpr size_type find_first_not_of (U const& sv, size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of (value_type ch, size_type pos = 0) const noexcept;
    constexpr size_type find_first_not_of (const_pointer s, size_type pos, size_type count) const;
    constexpr size_type find_first_not_of (const_pointer s, size_type pos = 0) const;

    template <str_view_like U>
    constexpr size_type find_last_of (U const& sv, size_type pos = npos) const noexcept;
    constexpr size_type find_last_of (value_type ch, size_type pos = npos) const noexcept;
    constexpr size_type find_last_of (const_pointer str, size_type pos, size_type count) const;
    constexpr size_type find_last_of (const_pointer str, size_type pos = npos) const;

    template <str_view_like U>
    constexpr size_type find_last_not_of (U const& sv, size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of (value_type ch, size_type pos = npos) const noexcept;
    constexpr size_type find_last_not_of (const_pointer s, size_type pos, size_type count) const;
    constexpr size_type find_last_not_of (const_pointer s, size_type pos = npos) const;

    constexpr void remove_prefix (size_type n);
    constexpr void remove_suffix (size_type n);

    /**
     ** @brief str()
     ** Returns the contained string as an fstring.
     **/
    constexpr string_type str () const noexcept
    {  return string_type (data (), length ()); }

    constexpr bool empty () const noexcept
    {  return length () == 0; }

    consteval static bool full () noexcept
    {  return true; }

    consteval static size_type space_remaining () noexcept
    {  return 0; }

    constexpr size_type size_bytes () const noexcept
    {  return _M_length * sizeof (value_type); }

    constexpr size_type capacity () const noexcept
    {  return  _M_length; }

    constexpr const_pointer data () const noexcept
    {  return _M_c_str; }

    constexpr const_pointer c_str () const noexcept
    {  return data (); }

    constexpr size_type length () const noexcept
    {  return _M_length; }

    constexpr size_type size () const noexcept
    {  return length (); }

    constexpr iterator begin () noexcept
    {  return iterator (*this, size_type ()); }

    constexpr const_iterator begin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr const_iterator cbegin () const noexcept
    {  return const_iterator (*this, size_type ()); }

    constexpr iterator end () noexcept
    {  return iterator (*this, length ()); }

    constexpr const_iterator end () const noexcept
    {  return const_iterator (*this, length ()); }

    constexpr const_iterator cend () noexcept
    {  return const_iterator (*this, length ()); }

    constexpr const_iterator cend () const noexcept
    {  return const_iterator (*this, length ()); }

    constexpr reverse_iterator rbegin () noexcept
    {  return reverse_iterator (*this, length () - 1); }

    constexpr const_reverse_iterator rbegin () const noexcept
    {  return const_reverse_iterator (*this, length () - 1); }

    constexpr const_reverse_iterator crbegin () noexcept
    {  return const_reverse_iterator (*this, length () - 1); }

    constexpr const_reverse_iterator crbegin () const noexcept
    {  return const_reverse_iterator (*this, length () - 1); }

    constexpr reverse_iterator rend () noexcept
    {  return reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator rend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr const_reverse_iterator crend () const noexcept
    {  return const_reverse_iterator (*this, npos); }

    constexpr reference front () noexcept
    { return *_M_c_str; }

    constexpr const_reference front () const noexcept
    { return *_M_c_str; }

    constexpr reference back () noexcept
    { return *(_M_c_str + (length () - 1)); }

    constexpr const_reference back () const noexcept
    { return *(_M_c_str + (length () - 1)); }

    constexpr reference at (size_type uPos) noexcept
    {
        assert  (uPos < length () && "pos out of range (equal or larger than the size)!");
        return *(_M_c_str + uPos);
    }

    constexpr const_reference at (size_type uPos) const noexcept
    {
        assert  (uPos < length () && "pos out of range (equal or larger than the size)!");
        return *(_M_c_str + uPos);
    }

    constexpr reference operator [] (size_type uPos) noexcept
    { return *(_M_c_str + uPos); }

    constexpr const_reference operator [] (size_type uPos) const noexcept
    { return *(_M_c_str + uPos); }

public:
    const_pointer _M_c_str  { };
    size_type     _M_length { };
};

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
cow_string<T, E, A> cow_string<T, E, A>::substr (size_type uBeginPos, size_type uEndPos)
{
    if (empty () || static_cast<difference_type> (uEndPos - uBeginPos) <= 0 || uBeginPos >= length ())
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

template <symbolic_char T, structure E, allocator_like A, generic_iterator Iterator>
cow_string<T, E, A>& copy_to_string (cow_string<T, E, A>&                    rh,
                                     Iterator                                pFromText,
                                     typename cow_string<T, E, A>::size_type uLength)
{
    if (!pFromText) return rh;

    if (uLength > rh.capacity ())
    {
        if (!rh.is_on_stack () && !rh.is_cow_v)
             rh.deallocate (rh.data (), rh.capacity () + 1);

        if (!(rh._M_gBuffer.heap.c_str = rh.allocate (uLength + 1)))
        {
            rh._M_uLength    = 0;
            rh._M_gBuffer.stack.data[0] = typename cow_string<T, E, A>::value_type ();
            return rh;
        }

        rh._M_gBuffer.heap.capacity = uLength;
    }

    std::copy (pFromText, pFromText + uLength, rh._M_gBuffer.heap.c_str);
    rh._M_uLength = uLength;
    //rh._M_gBuffer.heap.is_copied = true;

    rh._M_gBuffer.heap.c_str[uLength] = typename cow_string<T, E, A>::value_type ();
    return rh;
}

// ====================================================

template <symbolic_char T, structure E, allocator_like A, generic_iterator Iterator>
cow_string<T, E, A>& assign_to_string (cow_string<T, E, A>&                    rh,
                                       Iterator                                pFromText,
                                       typename cow_string<T, E, A>::size_type uLength)
{
    if (!pFromText) return rh;

    if (uLength > rh.capacity ())
    {
        if (!rh.is_cow_v) rh.deallocate (rh.data (), rh.capacity () + 1);

        if (!(rh._M_gBuffer.heap.c_str = rh.allocate (uLength + 1)))
        {
            rh._M_uLength    = 0;
            rh._M_gBuffer.stack.data[0] = typename cow_string<T, E, A>::value_type ();
            return rh;
        }

        rh._M_gBuffer.heap.capacity = uLength;
    }

    std::copy (pFromText, pFromText + uLength, rh._M_gBuffer.heap.c_str);
    rh._M_uLength = uLength;

    rh._M_gBuffer.heap.c_str[uLength] = typename cow_string<T, E, A>::value_type ();
    return rh;
}

// ====================================================

template <symbolic_char T, structure E, allocator_like A, generic_iterator Iterator>
cow_string<T, E, A>& add_to_string (cow_string<T, E, A>& rh,
                                    Iterator const       pFromText,
                                    std::size_t          uAddLength)
{
    auto const uLength = rh.size () + uAddLength;

    if (uLength > rh.capacity ())
    {
        auto const pOldBuffer   = rh.data     ();
        auto const uOldCapacity = rh.capacity ();

        if (!(rh._M_gBuffer.heap.c_str = rh.allocate (uLength + 1))) return rh;

        std::copy (pOldBuffer, pOldBuffer + rh.length (), &rh[0]);

        rh._M_gBuffer.heap.capacity = uLength;
        if (!rh.is_cow_v) rh.deallocate (pOldBuffer, uOldCapacity + 1);
    }

    std::copy (pFromText, pFromText + ++uAddLength, &rh[0] + rh.size ());

    rh._M_uLength = uLength;
    return rh;
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
constexpr auto operator <=> (cow_string<T, E, A> const& lhs, cow_string<T, E, A> const& rhs) noexcept
{
    return lhs.data () <=> rhs.data () && lhs.length () <=> rhs.length ();
}

template <symbolic_char T, structure E, allocator_or_void A>
constexpr auto operator <=> (cow_string<T, E, A> const& lhs, T const* rhs) noexcept
{
    return lhs <=> cow_string<T, E, A> (rhs);
}

template <symbolic_char T, structure E, allocator_or_void A>
constexpr auto operator <=> (T const* lhs, cow_string<T, E, A> const& rhs) noexcept
{
    return cow_string<T, E, A> (lhs) <=> rhs;
}

template <symbolic_char T, structure E, allocator_or_void A1, allocator_like A2>
constexpr auto operator <=> (cow_string<T, E, A1> const& lhs,
                             std::basic_string<T, std::char_traits<T>, A2> const& rhs) noexcept
{
    return lhs <=> rhs.data ();
}

template <symbolic_char T, structure E, allocator_like A1, allocator_or_void A2>
constexpr auto operator <=> (std::basic_string<T, std::char_traits<T>, A1> const& lhs,
                             cow_string<T, E, A2> const& rhs) noexcept
{
    return lhs.data () <=> rhs;
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
constexpr bool operator == (cow_string<T, E, A> const& lhs, cow_string<T, E, A> const& rhs) noexcept
{
    return lhs.data   () == rhs.data   () ||
          (lhs.length () == rhs.length () &&
           cow_string<T, E, A>::locale_type::compare (lhs.data (), rhs.data (), lhs.length ()) == 0);
}

template <symbolic_char T, structure E, allocator_or_void A>
constexpr bool operator == (cow_string<T, E, A> const& lhs, T const* rhs) noexcept
{
    return lhs == cow_string<T, E, A> (rhs);
}

template <symbolic_char T, structure E, allocator_or_void A>
constexpr bool operator == (T const* lhs, cow_string<T, E, A> const& rhs) noexcept
{
    return cow_string<T, E, A> (lhs) == rhs;
}

template <symbolic_char T, structure E, allocator_or_void A1, allocator_like A2>
constexpr bool operator == (cow_string<T, E, A1> const& lhs,
                            std::basic_string<T, std::char_traits<T>, A2> const& rhs) noexcept
{
    return lhs == rhs.data ();
}

template <symbolic_char T, structure E, allocator_like A1, allocator_or_void A2>
constexpr bool operator == (std::basic_string<T, std::char_traits<T>, A1> const& lhs,
                            cow_string<T, E, A2> const& rhs) noexcept
{
    return lhs.data () == rhs;
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
constexpr bool operator != (cow_string<T, E, A> const& lhs, cow_string<T, E, A> const& rhs) noexcept
{ return !(lhs == rhs); }

template <symbolic_char T, structure E, allocator_or_void A>
constexpr bool operator != (cow_string<T, E, A> const& lhs, T const* str) noexcept
{ return !(lhs == str); }

template <symbolic_char T, structure E, allocator_or_void A>
constexpr bool operator != (T const* str, cow_string<T, E, A> const& rhs) noexcept
{ return !(str == rhs); }

template <symbolic_char T, structure E, allocator_or_void A1, allocator_like A2>
constexpr bool operator != (cow_string<T, E, A1> const& lhs,
                            std::basic_string<T, std::char_traits<T>, A2> const& rhs) noexcept
{
    return !(lhs == rhs);
}

template <symbolic_char T, structure E, allocator_like A1, allocator_or_void A2>
constexpr bool operator != (std::basic_string<T, std::char_traits<T>, A1> const& lhs,
                            cow_string<T, E, A2> const& rhs) noexcept
{
    return !(lhs == rhs);
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A1, allocator_like A2>
constexpr cow_string<T, E, A2>& operator += (cow_string<T, E, A2>& lhs,
                                             cow_string<T, E, A1> const& rhs) noexcept
{
    return add_to_string (lhs, rhs.data (), rhs.length ());
}

template <symbolic_char T, structure E, allocator_like A>
constexpr cow_string<T, E, A>& operator += (cow_string<T, E, A>& lhs, T const* str) noexcept
{
    return add_to_string (lhs, str, str_size (str));
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
constexpr cow_string<T, E> operator + (cow_string<T, E, A> const& lhs,
                                       cow_string<T, E, A> const& rhs) noexcept
{
    cow_string<T, E> gStr (lhs);
    return std::move (add_to_string (gStr, rhs.data (), rhs.length ()));
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
constexpr cow_string<T, E> operator + (cow_string<T, E, A> const& lhs, T const* str) noexcept
{
    cow_string<T, E> gStr (lhs);
    return std::move (add_to_string (gStr, str, str_size (str)));
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
constexpr cow_string<T, E> operator + (T const* str, cow_string<T, E, A> const& rhs) noexcept
{
    cow_string<T, E> gStr (str);
    return std::move (add_to_string (gStr, rhs.data (), rhs.length ()));
}

// ====================================================

template <symbolic_char T, structure E, allocator_or_void A>
constexpr void swap (cow_string<T, E, A>& lhs, cow_string<T, E, A>& rhs) noexcept
{
    std::swap (lhs._M_gBuffer, rhs._M_gBuffer);
    std::swap (lhs._M_uLength, rhs._M_uLength);
}

// ====================================================

template <symbolic_char T = char>
using used_string = std::basic_string<T, std::char_traits<T>, memory::allocator<T>>;

template <symbolic_char T = char>
using used_isstream = std::basic_istringstream<T, std::char_traits<T>, memory::allocator<T>>;

template <symbolic_char T = char>
using used_osstream = std::basic_ostringstream<T, std::char_traits<T>, memory::allocator<T>>;
template <symbolic_char T = char>
using used_sstream = std::basic_stringstream<T, std::char_traits<T>, memory::allocator<T>>;

// ====================================================

/// is of string type
template <typename>
struct is_string : std::false_type
{ };

/// is std::basic_string type
template <allocator_like A>
struct is_string <std::basic_string<char, std::char_traits<char>, A>> : std::true_type
{ };

/// is std::basic_string type
template <allocator_like A>
struct is_string<std::basic_string<char8, std::char_traits<char8>, A>> : std::true_type
{ };

/// is std::basic_string type
template <allocator_like A>
struct is_string <std::basic_string<char16, std::char_traits<char16>, A>> : std::true_type
{ };

/// is std::basic_string type
template <allocator_like A>
struct is_string <std::basic_string<char32, std::char_traits<char32>, A>> : std::true_type
{ };

/// is std::basic_string type
template <allocator_like A>
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

template <allocator_like A>
struct is_string <cow_string<char, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <cow_string<char8, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <cow_string<char16, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <cow_string<char32, A>> : std::true_type
{ };

template <allocator_like A>
struct is_string <cow_string<wchar, A>> : std::true_type
{ };

/// is of string type -> value
template <typename T>
inline constexpr auto const is_string_v = is_string<T>::value || requires (T& t)
{
    typename T::traits_type           ;
    typename T::allocator_type        ;
    typename T::value_type            ;
    typename T::size_type             ;
    typename T::pointer               ;
    typename T::const_pointer         ;
    typename T::reference             ;
    typename T::const_reference       ;
    typename T::difference_type       ;
    typename T::iterator              ;
    typename T::const_iterator        ;
    typename T::reverse_iterator      ;
    typename T::const_reverse_iterator;

    { t.c_str   () } -> are_same<typename T::const_pointer>;
    { t.data    () } -> are_same<typename T::const_pointer>;
    { t.size    () } -> are_same<typename T::size_type>;
    { t.length  () } -> are_same<typename T::size_type>;
    { t.empty   () } -> are_same<bool>;
    { t.begin   () } -> are_same<typename T::iterator>;
    { t.end     () } -> are_same<typename T::iterator>;
    { t.cbegin  () } -> are_same<typename T::const_iterator>;
    { t.cend    () } -> are_same<typename T::const_iterator>;
    { t.rbegin  () } -> are_same<typename T::reverse_iterator>;
    { t.rend    () } -> are_same<typename T::reverse_iterator>;
    { t.crbegin () } -> are_same<typename T::const_reverse_iterator>;
    { t.crend   () } -> are_same<typename T::const_reverse_iterator>;
    { t.front   () } -> are_same<typename T::reference>;
    { t.back    () } -> are_same<typename T::reference>;
    { t.at      (typename T::size_type ()) } -> are_same<typename T::reference>;
    { t.operator [] (typename T::size_type ()) } -> are_same<typename T::reference>;
    { t.max_size () } -> are_same<typename T::size_type>;
    { t.capacity () } -> are_same<typename T::size_type>;
    { t.reserve (typename T::size_type ()) } -> are_same<void>;
    { t.shrink_to_fit () } -> are_same<void>;
    { t.erase (typename T::iterator ()) } -> are_same<typename T::iterator>;
    { t.clear () } -> are_same<void>;
    { t.pop_back () } -> are_same<void>;
    { t.pop_front () } -> are_same<void>;
    { t.get_allocator () } -> are_same<typename T::allocator_type>;
    { t.swap (t) } -> are_same<void>;
    { t.remove_prefix (typename T::size_type ()) } -> are_same<void>;
    { t.remove_suffix (typename T::size_type ()) } -> are_same<void>;
    { t.resize (typename T::size_type ()) } -> are_same<void>;
    { t.find (typename T::const_pointer ()) } -> are_same<typename T::size_type>;
    { t.substr () } -> are_same<T>;
    { t.compare (typename T::const_pointer ()) } -> are_same<int>;
};

template <typename T>
concept string_like = is_string_v<T>;

template <typename T>
using StringType = std::enable_if_t<is_string_v<T>, T>;

} // namespace cppual

// ====================================================

namespace std {

template <symbolic_char T, structure E, allocator_or_void A>
struct uses_allocator <cow_string<T, E, A>, A> : bool_constant<allocator_like<A>>
{ };

template <symbolic_char T, structure E, allocator_or_void A>
inline constexpr cbool ranges::enable_borrowed_range<cow_string<T, E, A>> = true;

template <symbolic_char T, structure E, allocator_or_void A>
inline constexpr cbool ranges::enable_view<cow_string<T, E, A>> = true;

template <symbolic_char T, structure E, allocator_or_void A>
struct hash <cow_string<T, E, A>>
{
   constexpr std::size_t operator () (cow_string<T, E, A> const& value) const noexcept
   {
       return char_hash (value.data ());
   }
};

template <symbolic_char T, structure E, allocator_or_void A>
basic_ostream<T, E>& operator << (basic_ostream<T, E>& os, cow_string<T, E, A> const& str)
{ os << str.data (); return os; }

} //! namespace std

// ====================================================

#endif // __cplusplus
#endif // CPPUAL_COW_STRING_H_