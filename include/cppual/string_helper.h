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

#include <cppual/decl>
#include <cppual/concepts>
#include <cppual/meta_type>
#include <cppual/containers>
#include <cppual/memory_allocator>

#include <cppual/string.h>

#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <cassert>

namespace cppual {

template <typename>
class consteval_array_iterator;

template <non_void, std::size_t>
class consteval_array;

// ====================================================

using memory::allocator_t;

// ====================================================

template <typename T>
class consteval_array_iterator
{
public:
    typedef consteval_array_iterator<T>     self_type        ;
    typedef std::remove_reference_t<T>      buf_type         ;
    typedef T::value_type                   value_type       ;
    typedef T::const_value                  const_value      ;
    typedef T::pointer                      pointer          ;
    typedef T::const_pointer                const_pointer    ;
    typedef T::reference                    reference        ;
    typedef T::const_reference              const_reference  ;
    typedef T::string_type                  string_type      ;
    typedef T::difference_type              difference_type  ;
    typedef T::size_type                    size_type        ;
    typedef size_type const                 const_size       ;
    typedef std::bidirectional_iterator_tag iterator_category;

    template <typename U>
    using self_type_t = consteval_array_iterator<U>;

    typedef std::conditional_t<std::is_const_v<buf_type>, const_value, value_type> elem_type;

    friend class consteval_array_iterator<buf_type const>;
    friend class consteval_array_iterator<std::remove_const_t<buf_type>>;

    constexpr string_type  operator  * () const { return  (*_M_buf)[_M_pos]; }
    constexpr string_type* operator -> () const { return &(*_M_buf)[_M_pos]; }

    consteval_array_iterator () = delete;

    constexpr consteval_array_iterator (buf_type const& b, const_size p) noexcept
    : _M_buf (&b), _M_pos (p)
    { }

    //! converting a const iterator to a non-const iterator
    constexpr
    consteval_array_iterator (self_type_t<buf_type const> const& other) noexcept
    : _M_buf (const_cast<buf_type*> (other._M_buf)), _M_pos (other._M_pos)
    { }

    /// converting a non-const iterator to a const iterator
    constexpr
    consteval_array_iterator (self_type_t<std::remove_const_t<buf_type>> const& other) noexcept
    : _M_buf (other._M_buf), _M_pos (other._M_pos)
    { }

    /// converting a non-const iterator to a const iterator
    constexpr
    self_type& operator = (self_type_t<std::remove_const_t<buf_type>> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_buf = other._M_buf;
        _M_pos = other._M_pos;

        return *this;
    }

    /// converting a const iterator to a non-const iterator
    constexpr self_type& operator = (self_type_t<buf_type const> const& other) noexcept
    {
        if (this == &other) return *this;

        _M_buf = const_cast<buf_type*> (other._M_buf);
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

    constexpr buf_type const& get () const noexcept
    { return *_M_buf; }

    constexpr size_type pos () const noexcept
    { return _M_pos; }

    template <typename U>
    friend
    constexpr self_type_t<U> operator + (self_type_t<U> const&, typename self_type_t<U>::difference_type);

    template <typename U>
    friend
    constexpr self_type_t<U> operator - (self_type_t<U> const&, typename self_type_t<U>::difference_type);

private:
    buf_type* _M_buf { };
    size_type _M_pos { };
};

// =========================================================

template <typename T>
constexpr bool operator == (consteval_array_iterator<T> const& lh, consteval_array_iterator<T> const& rh)
{ return lh.pos () == rh.pos () && &lh.get () == &rh.get (); }

template <typename T>
constexpr bool operator != (consteval_array_iterator<T> const& lh, consteval_array_iterator<T> const& rh)
{ return !(lh == rh); }

template <typename T>
constexpr bool operator > (consteval_array_iterator<T> const& lh, consteval_array_iterator<T> const& rh)
{ return lh.pos () > rh.pos (); }

template <typename T>
constexpr bool operator >= (consteval_array_iterator<T> const& lh, consteval_array_iterator<T> const& rh)
{ return lh.pos () >= rh.pos (); }

template <typename T>
constexpr bool operator < (consteval_array_iterator<T> const& lh, consteval_array_iterator<T> const& rh)
{ return lh.pos () < rh.pos (); }

template <typename T>
constexpr bool operator <= (consteval_array_iterator<T> const& lh, consteval_array_iterator<T> const& rh)
{ return lh.pos () <= rh.pos (); }

// =========================================================

template <typename T>
constexpr consteval_array_iterator<T> operator + (consteval_array_iterator<T> const& lh,
                                                  typename consteval_array_iterator<T>::difference_type n)
{
    consteval_array_iterator<T> ret_obj (lh);
    ret_obj._M_pos += static_cast<consteval_array_iterator<T>::size_type> (n);
    return ret_obj;
}

template <typename T>
constexpr consteval_array_iterator<T> operator - (consteval_array_iterator<T> const& lh,
                                                  typename consteval_array_iterator<T>::difference_type n)
{
    consteval_array_iterator<T> ret_obj (lh);
    ret_obj._M_pos -= static_cast<consteval_array_iterator<T>::size_type> (n);
    return ret_obj;
}

template <typename T>
constexpr
consteval_array_iterator<T>::difference_type
operator + (consteval_array_iterator<T> const& a, consteval_array_iterator<T> const& b)
{ return static_cast<consteval_array_iterator<T>::difference_type> (a.pos () + b.pos ()); }

template <typename T>
constexpr
consteval_array_iterator<T>::difference_type
operator - (consteval_array_iterator<T> const& a, consteval_array_iterator<T> const& b)
{ return static_cast<consteval_array_iterator<T>::difference_type> (a.pos () - b.pos ()); }

//======================================================

template <non_void T, std::size_t N>
class consteval_array
{
public:
    typedef consteval_array<T, N>                     self_type             ;
    typedef T                                         value_type            ;
    typedef value_type const                          const_value           ;
    typedef value_type const*                         pointer               ;
    typedef value_type const*                         const_pointer         ;
    typedef value_type const&                         reference             ;
    typedef value_type const&                         const_reference       ;
    typedef std::basic_string_view<T>                 string_type           ;
    typedef decltype (N)                              size_type             ;
    typedef size_type const                           const_size            ;
    typedef std::ptrdiff_t                            difference_type       ;
    typedef consteval_array_iterator<self_type>       iterator              ;
    typedef consteval_array_iterator<self_type const> const_iterator        ;
    typedef std::reverse_iterator<iterator>           reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator>     const_reverse_iterator;

    consteval static size_type size () noexcept { return N; }

    static_assert (size () > 0, "consteval_array is empty!");

    inline constexpr static const_size npos = static_cast<size_type> (-1);

    typedef std::conditional_t<is_char_v<T>, const_pointer, value_type> elem_type;
    typedef std::conditional_t<is_char_v<T>, string_type&, const_reference> elem_reference;
    typedef std::conditional_t<is_char_v<T>, string_type const&, const_reference> elem_const_reference;
    typedef std::conditional_t<is_char_v<T>, string_type, value_type> array_type;

    consteval_array () = delete;

    template <c_str_const_t... Ts>
    requires (sizeof... (Ts) == size ())
    consteval consteval_array (Ts... array) noexcept
    : _M_array { std::forward<Ts> (array)... }
    {
        static_assert (are_of_same_type_v<elem_type, Ts...>,
                       "some of the args are NOT of the same type!");
    }

    consteval consteval_array (std::initializer_list<value_type> list) noexcept
    : _M_array (list)
    {
        static_assert (list.size () == size (), "string array initializer list size mismatch!");
    }

    constexpr elem_const_reference operator [] (const_size i) const noexcept
    {
        assert (i < size () && "index out of range!");
        return _M_array[i];
    }

    constexpr elem_reference operator [] (const_size i) noexcept
    {
        assert (i < size () && "index out of range!");
        return _M_array[i];
    }

    consteval size_type hash (const_size i) const noexcept
    {
        static_assert (is_char_v<value_type>, "hash is only available for C string types!");

        assert (i < size () && "index out of range!");
        return char_hash<(*this)[i]> ();
    }

    consteval elem_const_reference front  () const noexcept { return _M_array[0]; }
    consteval elem_const_reference back   () const noexcept { return _M_array[size () - 1]; }
    consteval const_iterator       begin  () const noexcept { return const_iterator (*this, 0); }
    consteval const_iterator       cbegin () const noexcept { return const_iterator (*this, 0); }
    consteval const_iterator       end    () const noexcept { return const_iterator (*this, size ()); }
    consteval const_iterator       cend   () const noexcept { return const_iterator (*this, size ()); }

    consteval const_reverse_iterator rbegin () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    consteval const_reverse_iterator rend () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, npos)); }

    consteval const_reverse_iterator crbegin () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    consteval const_reverse_iterator crend () const noexcept
    {  return const_reverse_iterator (const_iterator (*this, npos)); }

private:
    array_type _M_array[size ()];
};

//======================================================

template <non_void T = char, c_str_const_t... Ts>
consteval auto make_consteval_array (Ts... ts) -> consteval_array<T, sizeof... (Ts)>
{
    return { std::forward<Ts> (ts)... };
}

template <non_void T = char>
consteval auto make_consteval_array (std::initializer_list<T> list) -> consteval_array<T, list.size ()>
{
    return consteval_array<T, list.size ()> (list);
}

// ====================================================

template <char_t T = char, allocator_t A = memory::allocator<T>>
auto split_string (std::basic_string<T, std::char_traits<T>, A> const& str, T delim)
{
    static_assert (std::is_same_v<T, typename A::value_type>, "");

    typedef std::size_t size_type;
    typedef std::basic_string<T, std::char_traits<T>, A> string_type;

    using allocator_type = typename std::allocator_traits<A>::template rebind_alloc<string_type>;

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

    using allocator_type = std::allocator_traits<A>::template rebind_alloc<string_type>;

    std::vector<string_type, allocator_type> out (std::is_same_v<A, typename memory::allocator<T>> ?
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

template <non_void  T,
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
inline auto to_std_string (std::basic_string<Char, std::char_traits<Char>, memory::allocator<Char>> const& val)
{
    typedef std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> std_string_type;

    return std_string_type (val.c_str (), val.size ());
}

template <char_t Char = char>
inline auto to_std_string (fstring<Char>, char_traits<Char>, memory::allocator<Char> const& val)
{
    typedef std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> std_string_type;

    return std_string_type (val.c_str (), val.size ());
}

//======================================================

} // cppual

//======================================================

// template <cppual::number_t T>
// constexpr cppual::string operator + (cppual::cchar* lh, T rh)
// {
//     cppual::string ret_obj (lh);
//     ret_obj += cppual::number_to_string<T> (rh);
//     return ret_obj;
// }

// template <cppual::number_t T>
// constexpr cppual::string operator + (T lh, cppual::cchar* rh)
// {
//     cppual::string ret_obj (cppual::number_to_string<T> (lh));
//     ret_obj += rh;
//     return ret_obj;
// }

//======================================================

#endif // __cplusplus
#endif // CPPUAL_STRING_HELPER_H_
