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

#ifndef CPPUAL_STRING_HELPER_H_
#define CPPUAL_STRING_HELPER_H_
#ifdef __cplusplus

#include <cppual/decl.h>
#include <cppual/string.h>
#include <cppual/type_meta.h>
#include <cppual/containers.h>
#include <cppual/concept/concepts.h>
#include <cppual/memory/allocator.h>

#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

namespace cppual {

template <typename>
class string_list_iterator;

template <std::size_t, char_t>
class string_list;

// ====================================================

using memory::allocator_t;

// ====================================================

template <typename T>
class string_list_iterator
{
public:
    typedef string_list_iterator<T>         self_type        ;
    typedef std::remove_reference_t<T>      buf_type         ;
    typedef T::value_type                   value_type       ;
    typedef value_type const                const_value      ;
    typedef T::pointer                      pointer          ;
    typedef T::const_pointer                const_pointer    ;
    typedef T::reference                    reference        ;
    typedef T::const_reference              const_reference  ;
    typedef T::string_type                  string_type      ;
    typedef T::difference_type              difference_type  ;
    typedef T::size_type                    size_type        ;
    typedef std::bidirectional_iterator_tag iterator_category;

    template <typename U>
    using self_type_t = string_list_iterator<U>;

    typedef std::conditional_t<std::is_const_v<buf_type>, const_value, value_type> elem_type;

    friend class string_list_iterator<buf_type const>;
    friend class string_list_iterator<std::remove_const_t<buf_type>>;

    constexpr string_type  operator  * () const { return  (*_M_buf)[_M_pos]; }
    constexpr string_type* operator -> () const { return &(*_M_buf)[_M_pos]; }

    constexpr string_list_iterator () noexcept
    : _M_buf (), _M_pos ()
    { }

    constexpr string_list_iterator (buf_type& b, size_type p) noexcept
    : _M_buf (&b), _M_pos (p)
    { }

    /// converting a non-const iterator to a const iterator
    constexpr
    string_list_iterator (self_type_t<std::remove_const_t<buf_type>> const& other) noexcept
    : _M_buf (other._M_buf), _M_pos (other._M_pos)
    { }

    constexpr
    self_type& operator = (self_type_t<std::remove_const_t<buf_type>> const& other) noexcept
    {
        _M_buf = other._M_buf;
        _M_pos = other._M_pos;

        return *this;
    }

    constexpr self_type& operator ++ () noexcept
    {
        ++_M_pos;
        return *this;
    }

    constexpr self_type operator ++ (int) noexcept
    {
        self_type ret_obj (*this);
        ++_M_pos;
        return ret_obj;
    }

    constexpr self_type& operator -- () noexcept
    {
        --_M_pos;
        return *this;
    }
    constexpr self_type operator -- (int) noexcept
    {
        self_type ret_obj (*this);
        --_M_pos;
        return ret_obj;
    }

    constexpr self_type& operator += (difference_type n) noexcept
    {
        _M_pos += static_cast<size_type> (n);
        return *this;
    }

    constexpr self_type& operator -= (difference_type n) noexcept
    {
        _M_pos -= static_cast<size_type> (n);
        return *this;
    }

    constexpr buf_type& get () const noexcept
    { return *_M_buf; }

    constexpr size_type pos () const noexcept
    { return _M_pos; }

    template <typename U>
    friend
    inline self_type_t<U> operator + (self_type_t<U> const&, typename self_type_t<U>::difference_type);

    template <typename U>
    friend
    inline self_type_t<U> operator - (self_type_t<U> const&, typename self_type_t<U>::difference_type);

private:
    buf_type* _M_buf;
    size_type _M_pos;
};

// =========================================================

template <typename T>
constexpr bool operator == (string_list_iterator<T> const& lhObj, string_list_iterator<T> const& other)
{ return lhObj.pos () == other.pos () && &lhObj.get () == &other.get (); }

template <typename T>
constexpr bool operator != (string_list_iterator<T> const& lhObj, string_list_iterator<T> const& other)
{ return !(lhObj == other); }

template <typename T>
constexpr bool operator > (string_list_iterator<T> const& lhObj, string_list_iterator<T> const& other)
{ return lhObj.pos () > other.pos (); }

template <typename T>
constexpr bool operator >= (string_list_iterator<T> const& lhObj, string_list_iterator<T> const& other)
{ return lhObj.pos () >= other.pos (); }

template <typename T>
constexpr bool operator < (string_list_iterator<T> const& lhObj, string_list_iterator<T> const& other)
{ return lhObj.pos () < other.pos (); }

template <typename T>
constexpr bool operator <= (string_list_iterator<T> const& lhObj, string_list_iterator<T> const& other)
{ return lhObj.pos () <= other.pos (); }

// =========================================================

template <typename T>
inline string_list_iterator<T> operator + (string_list_iterator<T> const& lhObj,
                                           typename string_list_iterator<T>::difference_type n)
{
    string_list_iterator<T> ret_obj (lhObj);
    ret_obj._M_pos += static_cast<string_list_iterator<T>::size_type> (n);
    return ret_obj;
}

template <typename T>
inline string_list_iterator<T> operator - (string_list_iterator<T> const& lhObj,
                                           typename string_list_iterator<T>::difference_type n)
{
    string_list_iterator<T> ret_obj (lhObj);
    ret_obj._M_pos -= static_cast<string_list_iterator<T>::size_type> (n);
    return ret_obj;
}

template <typename T>
constexpr
string_list_iterator<T>::difference_type
operator + (string_list_iterator<T> const& a, string_list_iterator<T> const& b)
{ return static_cast<string_list_iterator<T>::difference_type> (a.pos () + b.pos ()); }

template <typename T>
constexpr
string_list_iterator<T>::difference_type
operator - (string_list_iterator<T> const& a, string_list_iterator<T> const& b)
{ return static_cast<string_list_iterator<T>::difference_type> (a.pos () - b.pos ()); }

//======================================================

template <std::size_t N, char_t T = char>
class string_list
{
public:
    typedef string_list<N, T>                     self_type             ;
    typedef T                                     value_type            ;
    typedef value_type const                      const_value           ;
    typedef value_type const*                     pointer               ;
    typedef value_type const*                     const_pointer         ;
    typedef value_type const&                     reference             ;
    typedef value_type const&                     const_reference       ;
    typedef std::basic_string_view<T>             string_type           ;
    typedef string_type&                          str_reference         ;
    typedef string_type const&                    str_const_reference   ;
    typedef std::size_t                           size_type             ;
    typedef size_type                             const_size            ;
    typedef std::ptrdiff_t                        difference_type       ;
    typedef string_list_iterator<self_type>       iterator              ;
    typedef string_list_iterator<self_type const> const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static_assert (N > 0, "string_list is empty!");

    inline constexpr static const_size npos = static_cast<size_type> (-1);

    string_list () = delete;

    template <c_str_const_t... Ts>
    consteval string_list (Ts... array) noexcept
    : _M_array { array... }
    {
        static_assert (are_of_same_type_v<const_pointer, Ts...>, "some of the args are NOT C strings!");
    }

    constexpr str_const_reference operator [] (size_type i) const noexcept
    {
        assert (i < N && "index out of range!");
        return _M_array[i];
    }

    constexpr size_type hash (size_type i) const noexcept
    {
        assert (i < N && "index out of range!");
        return constexpr_char_hash ((*this)[i]);
    }

    constexpr str_const_reference front  () const noexcept { return _M_array[0]; }
    constexpr str_const_reference back   () const noexcept { return _M_array[size () - 1]; }
    constexpr const_iterator      begin  () const noexcept { return const_iterator (*this, 0); }
    constexpr const_iterator      cbegin () const noexcept { return const_iterator (*this, 0); }
    constexpr const_iterator      end    () const noexcept { return const_iterator (*this, size ()); }
    constexpr const_iterator      cend   () const noexcept { return const_iterator (*this, size ()); }

    constexpr const_reverse_iterator rbegin () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    constexpr const_reverse_iterator rend () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, npos)); }

    constexpr const_reverse_iterator crbegin () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    constexpr const_reverse_iterator crend () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, npos)); }

    consteval static size_type size () noexcept { return N; }

private:
    string_type _M_array[N];
};

//======================================================

template <char_t T = char, c_str_const_t... Ts>
consteval auto make_string_list (Ts... ts) -> string_list<sizeof... (Ts), T>
{
    return { std::forward<Ts> (ts)... };
}

// ====================================================

template <char_t T = char, allocator_t A = memory::allocator<T>>
auto split_string (std::basic_string<T, std::char_traits<T>, A> const& str, T delim)
{
    static_assert (std::is_same_v<T, typename A::value_type>, "");

    typedef std::size_t size_type;
    typedef std::basic_string<T, std::char_traits<T>, A> string_type;

    using allocator_type =
        typename std::allocator_traits<A>::template rebind_alloc<string_type>;

    std::vector<string_type, allocator_type> out (std::is_same_v<A, memory::allocator<T>> ?
                                                     allocator_type (str.get_allocator ()) : allocator_type ());

    size_type start;
    size_type end = 0U;

    size_type const delim_count =
        static_cast<size_type> (std::count (str.cbegin (), str.cend (), delim));

    if (delim_count > 0) out.reserve (delim_count + 1);

    while ((start = str.find_first_not_of (delim, end)) != string_type::npos)
    {
        end = str.find (delim, start);
        out.push_back (str.substr (start, end - start));
    }

    return out;
}

// ====================================================

template <char_t T = char, allocator_t A = memory::allocator<T>>
auto split_string (std::basic_string<T, std::char_traits<T>, A> const& str,
                  std::basic_string<T, std::char_traits<T>, A> const& delim)
{
    typedef std::basic_string<T, std::char_traits<T>, A> string_type;

    using allocator_type =
        typename std::allocator_traits<A>::template rebind_alloc<string_type>;

    std::vector<string_type, allocator_type> out (std::is_same_v<A, memory::allocator<T>> ?
                                                     allocator_type (str.get_allocator ()) : allocator_type ());

    std::size_t start;
    std::size_t end = 0U;

    while ((start = str.find_first_not_of (delim, end)) != string_type::npos)
    {
        end = str.find (delim, start);
        out.push_back (str.substr (start, end - start));
    }

    return out;
}

// ====================================================

template <number_t    T,
          char_t      Char = char,
          allocator_t A    = memory::allocator<Char>
          >
inline auto number_to_string (T val)
{
    typedef std::basic_string<Char, std::char_traits<Char>, A> string_type;
    typedef std::size_t size_type;

    string_type str_val (sizeof (T), ' ');

    unordered_map<size_type, cchar*> formats_map
    {
        std::make_pair (typeid(ushort).hash_code  (), "%hu"),
        std::make_pair (typeid(short).hash_code   (), "%hd"),
        std::make_pair (typeid(uint).hash_code    (), "%u"),
        std::make_pair (typeid(int).hash_code     (), "%d"),
        std::make_pair (typeid(ulong).hash_code   (), "%lu"),
        std::make_pair (typeid(long).hash_code    (), "%ld"),
        std::make_pair (typeid(ulong64).hash_code (), "%llu"),
        std::make_pair (typeid(long64).hash_code  (), "%lld"),
        std::make_pair (typeid(float).hash_code   (), "%f"),
        std::make_pair (typeid(double).hash_code  (), "%lf"),
        std::make_pair (typeid(ldouble).hash_code (), "%Lf")
    };

    std::sprintf (&str_val[0], formats_map[typeid(T).hash_code ()], val);

    return str_val;
}

// ====================================================

template <non_void_t  T,
          char_t      Char = char,
          allocator_t A    = memory::allocator<Char>,
          typename         = std::enable_if_t<std::is_same_v<Char, typename A::value_type>>
          >
inline auto to_string (T val, A const& ator = A ())
{
    typedef std::basic_string<Char, std::char_traits<Char>, A>        string_type  ;
    typedef std::basic_ostringstream<Char, std::char_traits<Char>, A> ostringstream;

    string_type   in_str (ator);
    ostringstream ss   (in_str);

    ss << val;

    return ss.str ();
}

// ====================================================

template <char_t Char = char>
inline auto to_std_string (used_string<Char> const& val)
{
    typedef std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> std_string_type;

    return std_string_type (val.c_str (), val.size ());
}

//======================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_STRING_HELPER_H_
