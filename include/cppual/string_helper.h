/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2022 K. Petrov
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
#include <cppual/meta.h>
#include <cppual/string.h>
#include <cppual/containers.h>

#include <cstring>
#include <string>
#include <vector>
#include <memory>
#include <array>

#include <assert.h>

namespace cppual {

// ====================================================

template <typename T    = char,
          typename Ator = memory::allocator<T>,
          typename      = typename std::enable_if<is_char<T>::value>::type>
auto split_string (std::basic_string<T, std::char_traits<T>, Ator> const& str,
                   T delim)
{
    typedef std::basic_string<T, std::char_traits<T>, Ator> string_type;

    using allocator_type = typename std::allocator_traits<Ator>::template rebind_alloc<string_type>;

    std::vector<string_type, allocator_type> out (std::is_same<Ator, memory::allocator<T>>::value ?
                                                  allocator_type (str.get_allocator ()) : allocator_type ());

    std::size_t start;
    std::size_t end = 0;

    std::size_t const delim_count =
            static_cast<std::size_t> (std::count (str.cbegin (), str.cend (), delim));

    if (delim_count > 0) out.reserve (delim_count + 1);

    while ((start = str.find_first_not_of (delim, end)) != string_type::npos)
    {
        end = str.find (delim, start);
        out.push_back (str.substr (start, end - start));
    }

    return out;
}

// ====================================================

template <typename T    = char,
          typename Ator = memory::allocator<T>,
          typename      = typename std::enable_if<is_char<T>::value>::type>
auto split_string (std::basic_string<T, std::char_traits<T>, Ator> const& str,
                   std::basic_string<T, std::char_traits<T>, Ator> const& delim)
{
    typedef std::basic_string<T, std::char_traits<T>, Ator> string_type;

    using allocator_type = typename std::allocator_traits<Ator>::template rebind_alloc<string_type>;

    std::vector<string_type, allocator_type> out (std::is_same<Ator, memory::allocator<T>>::value ?
                                                  allocator_type (str.get_allocator ()) : allocator_type ());

    std::size_t start;
    std::size_t end = 0;

    while ((start = str.find_first_not_of (delim, end)) != string_type::npos)
    {
        end = str.find (delim, start);
        out.push_back (str.substr (start, end - start));
    }

    return out;
}

// ====================================================

template <typename T>
class string_list_iterator
{
public:
    typedef T                               buf_type         ;
    typedef typename T::pointer             pointer          ;
    typedef typename T::const_pointer       const_pointer    ;
    typedef typename T::reference           reference        ;
    typedef typename T::const_reference     const_reference  ;
    typedef typename T::difference_type     difference_type  ;
    typedef typename T::size_type           size_type        ;
    typedef typename T::value_type          value_type       ;
    typedef string_list_iterator<T>         self_type        ;
    typedef std::bidirectional_iterator_tag iterator_category;

    typedef typename std::conditional<std::is_const<buf_type>::value,
    value_type const, value_type>::type
    elem_type;

    friend class string_list_iterator<buf_type const>;

    constexpr elem_type operator * () const { return get ()[_M_pos]; }

    constexpr string_list_iterator () noexcept
    : _M_buf (), _M_pos ()
    { }

    constexpr string_list_iterator (buf_type& b, size_type p) noexcept
    : _M_buf (&b), _M_pos (p)
    { }

    /// Converting a non-const iterator to a const iterator
    constexpr
    string_list_iterator (string_list_iterator<typename std::remove_const<buf_type>::type> const& other)
    noexcept
    : _M_buf (other._M_buf), _M_pos (other._M_pos)
    { }

    inline
    string_list_iterator& operator = (string_list_iterator<typename std::remove_const<buf_type>::type>
                                      const& other) noexcept
    {
        _M_buf = other._M_buf;
        _M_pos = other._M_pos;

        return *this;
    }

    inline self_type& operator ++ ()
    {
        ++_M_pos;
        return *this;
    }

    inline self_type operator ++ (int)
    {
        self_type tmp (*this);
        ++_M_pos;
        return tmp;
    }

    inline self_type& operator -- ()
    {
        --_M_pos;
        return *this;
    }
    inline self_type operator -- (int)
    {
        self_type tmp (*this);
        --_M_pos;
        return tmp;
    }

    inline self_type operator + (difference_type n) const
    {
        self_type tmp (*this);
        tmp._M_pos += static_cast<size_type> (n);
        return tmp;
    }

    inline self_type& operator += (difference_type n)
    {
        _M_pos += static_cast<size_type> (n);
        return *this;
    }

    inline self_type operator - (difference_type n) const
    {
        self_type tmp (*this);
        tmp._M_pos -= static_cast<size_type> (n);
        return tmp;
    }

    inline self_type& operator -= (difference_type n)
    {
        _M_pos -= static_cast<size_type> (n);
        return *this;
    }

    constexpr difference_type operator - (self_type const& c) const
    { return difference_type (_M_pos - c._M_pos); }

    constexpr bool operator == (self_type const& other) const
    { return _M_pos == other._M_pos and _M_buf == other._M_buf; }

    constexpr bool operator != (self_type const& other) const
    { return _M_pos != other._M_pos or _M_buf != other._M_buf; }

    constexpr bool operator > (self_type const& other) const
    { return _M_pos > other._M_pos; }

    constexpr bool operator >= (self_type const& other) const
    { return _M_pos >= other._M_pos; }

    constexpr bool operator < (self_type const& other) const
    { return _M_pos < other._M_pos; }

    constexpr bool operator <= (self_type const& other) const
    { return _M_pos <= other._M_pos; }

    constexpr buf_type& get () const noexcept
    { return *_M_buf; }

    constexpr size_type pos () const noexcept
    { return _M_pos; }

private:
    buf_type* _M_buf;
    size_type _M_pos;
};

// =========================================================

template <typename T>
constexpr
string_list_iterator<T>
operator + (const typename string_list_iterator<T>::difference_type& a,
            const string_list_iterator<T>                          & b)
{ return string_list_iterator<T> (b) + a; }

template <typename T>
constexpr
string_list_iterator<T>
operator - (const typename string_list_iterator<T>::difference_type& a,
            const string_list_iterator<T>                          & b)
{ return string_list_iterator<T> (b) - a; }

//======================================================

template <typename T,
          std::size_t N,
          typename = typename std::enable_if<is_char<T>::value>::type>
class string_list
{
public:
    typedef T const*                              value_type            ;
    typedef std::size_t                           size_type             ;
    typedef T const*                              pointer               ;
    typedef T const*                              const_pointer         ;
    typedef T const*                              reference             ;
    typedef T const*                              const_reference       ;
    typedef std::ptrdiff_t                        difference_type       ;
    typedef string_list<T, N>                     self_type             ;
    typedef string_list_iterator<self_type>       iterator              ;
    typedef string_list_iterator<self_type const> const_iterator        ;
    typedef std::reverse_iterator<iterator>       reverse_iterator      ;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    static_assert (N > 0, "string_list empty!");

    static constexpr const size_type npos = static_cast<size_type> (-1);

    string_list () = delete;

    template <typename... U>
    constexpr string_list(U... array) noexcept
    : _M_array { array... }
    {
        static_assert (are_of_same_type_v<value_type, U...>, "some of the args are not strings!");
    }

    constexpr const_pointer operator [] (size_type i) const
    {
        assert (i < size());
        return _M_array[i];
    }

    constexpr size_type hash (size_type i) const
    {
        return const_hash ((*this)[i]);
    }

    constexpr const_reference front  () const noexcept { return _M_array[0]; }
    constexpr const_reference back   () const noexcept { return _M_array[N - 1]; }
    constexpr size_type       size   () const noexcept { return N; }
    constexpr const_iterator  begin  () const noexcept { return const_iterator (*this, 0); }
    constexpr const_iterator  end    () const noexcept { return const_iterator (*this, size ()); }
    constexpr const_iterator  cbegin () const noexcept { return const_iterator (*this, 0); }
    constexpr const_iterator  cend   () const noexcept { return const_iterator (*this, size ()); }

    constexpr const_reverse_iterator rbegin () const noexcept
    { return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    constexpr const_reverse_iterator rend () const noexcept
    { return const_reverse_iterator (const_iterator (*this, npos)); }

    constexpr const_reverse_iterator crbegin () const noexcept
    { return const_reverse_iterator (const_iterator (*this, size () - 1)); }

    constexpr const_reverse_iterator crend () const noexcept
    { return const_reverse_iterator (const_iterator (*this, npos)); }

private:
    const_pointer _M_array[N];
};

//======================================================

template <typename T = char, typename... Ts>
constexpr auto make_string_list (Ts... ts) -> string_list<T, sizeof... (Ts)>
{
    return { std::forward<Ts> (ts)... };
}

// ====================================================

template <typename T,
          typename Char = char,
          typename Ator = memory::allocator<Char>,
          typename      = typename std::enable_if<(is_integer<T>::value ||
                                                   is_float<T>::value)  &&
                                                   is_char<Char>::value>::type
          >
inline auto number_to_string (T val)
{
    typedef std::basic_string<Char, std::char_traits<Char>, Ator> string_type;

    string_type str_val (sizeof (T), ' ');

    unordered_map<std::size_t, cchar*> formats_map
    {
        std::make_pair (typeid(short).hash_code   (), "%hd"),
        std::make_pair (typeid(int).hash_code     (), "%d"),
        std::make_pair (typeid(long).hash_code    (), "%ld"),
        std::make_pair (typeid(long64).hash_code  (), "%lld"),
        std::make_pair (typeid(ushort).hash_code  (), "%hu"),
        std::make_pair (typeid(uint).hash_code    (), "%u"),
        std::make_pair (typeid(ulong).hash_code   (), "%lu"),
        std::make_pair (typeid(ulong64).hash_code (), "%llu"),
        std::make_pair (typeid(float).hash_code   (), "%f"),
        std::make_pair (typeid(double).hash_code  (), "%lf"),
        std::make_pair (typeid(ldouble).hash_code (), "%Lf")
    };

    std::sprintf (&str_val[0], formats_map[typeid(T).hash_code ()], val);

    return str_val;
}

// ====================================================

template <typename T,
          typename Char  = char,
          typename Alloc = memory::allocator<Char>,
          typename       = typename std::enable_if<is_char<Char>::value &&
                                                   is_char<typename Alloc::value_type>::value>::type
          >
inline
auto to_string (T val, Alloc const& a = Alloc ())
{
    typedef std::basic_string<Char, std::char_traits<Char>, Alloc>        out_string_type  ;
    typedef std::basic_ostringstream<Char, std::char_traits<Char>, Alloc> out_ostringstream;

    out_string_type in_str (a);

    out_ostringstream ss(in_str);
    ss << val;

    return ss.str();
}

// ====================================================

template <typename Char = char,
          typename      = typename std::enable_if<is_char<Char>::value>::type
          >
inline
auto to_std_string (used_string<Char> const& val)
{
    typedef std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> out_string_type;

    return out_string_type(val.c_str(), val.size());
}

//======================================================

} // cppual

#endif // __cplusplus
#endif // CPPUAL_STRING_HELPER_H_
