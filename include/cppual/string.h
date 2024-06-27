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

#ifndef CPPUAL_TEXT_STRING_H_
#define CPPUAL_TEXT_STRING_H_
#ifdef __cplusplus

#include <cppual/meta.h>
#include <cppual/types.h>
#include <cppual/concepts.h>
#include <cppual/memory/allocator.h>

#include <memory>
#include <string>
#include <cstring>
#include <sstream>

namespace cppual {

template <typename T, typename TAlloc = memory::allocator<T>>
class SHARED_API cstring : private TAlloc
{
public:
    typedef std::allocator_traits<TAlloc>              allocator_traits;
    typedef typename allocator_traits::allocator_type  allocator_type  ;
    typedef typename allocator_traits::size_type       size_type       ;
    typedef typename allocator_traits::value_type      value_type      ;
    typedef typename allocator_traits::pointer         pointer         ;
    typedef typename allocator_traits::const_pointer   const_pointer   ;
    typedef typename allocator_traits::reference       reference       ;
    typedef typename allocator_traits::const_reference const_reference ;

    static_assert (is_char<value_type>::value &&
                   std::is_same<value_type, typename std::remove_cv<T>::type>::value,
                   "T is not a char!");

    struct Buffer
    {
        CharType<value_type>* data     { };
        size_type             capacity { };
    };

    constexpr cstring () noexcept = default;
    cstring substr (size_type begin_pos, size_type end_pos);

    constexpr const_pointer  c_str         () const noexcept { return  _M_gBuffer.data    ; }
    constexpr const_pointer  data          () const noexcept { return  _M_gBuffer.data    ; }
    constexpr size_type      capacity      () const noexcept { return  _M_gBuffer.capacity; }
    constexpr size_type      length        () const noexcept { return  _M_uLength         ; }
    constexpr size_type      size          () const noexcept { return  _M_uLength         ; }
    constexpr bool           empty         () const noexcept { return !_M_uLength         ; }
    constexpr allocator_type get_allocator () const noexcept { return *this               ; }

    inline cstring& operator = (T const* pText) noexcept
    { return assign_to_string (*this, pText, std::strlen (pText)); }

    inline cstring& operator = (cstring<T, TAlloc> const& gObj) noexcept
    { return assign_to_string (*this, gObj._M_gBuffer.data, gObj._M_uLength); }

    constexpr reference operator [] (size_type uPos) noexcept
    { return *(_M_gBuffer.data + uPos); }

    constexpr const_reference operator [] (size_type uPos) const noexcept
    { return *(_M_gBuffer.data + uPos); }

    inline ~cstring () noexcept
    { if (_M_gBuffer.data) TAlloc::deallocate (_M_gBuffer.data, 0); }

    inline explicit cstring (pointer       pText,
                             TAlloc const& gAtor = TAlloc ()) noexcept
    : allocator_type (gAtor),
      _M_gBuffer ()
    { copy_to_string (*this, pText, std::strlen (pText)); }

    constexpr cstring (TAlloc const& gAtor) noexcept
    : allocator_type (gAtor),
      _M_gBuffer (),
      _M_uLength ()
    { }

    inline cstring (pointer pText) noexcept
    : allocator_type (), _M_gBuffer { pText, std::strlen (pText) }
    { _M_uLength = _M_gBuffer.capacity; }

    inline cstring (cstring<T, TAlloc> const& gObj) noexcept
    : allocator_type (gObj), _M_gBuffer ()
    { copy_to_string (*this, gObj._M_gBuffer.data, gObj._M_uLength); }

    inline cstring (cstring<T, TAlloc>&& gObj) noexcept
    : allocator_type (gObj),
      _M_gBuffer (gObj._M_gBuffer),
      _M_uLength (gObj._M_uLength)
    {
        gObj._M_gBuffer.data     = nullptr;
        gObj._M_gBuffer.capacity = gObj._M_uLength = 0;
    }

    inline cstring (size_type uCapacity) noexcept : cstring (nullptr)
    {
        if (uCapacity)
        {
            _M_gBuffer.data = TAlloc::allocate (uCapacity + 1U);
            _M_uLength = _M_gBuffer.capacity = (_M_gBuffer.data ? uCapacity : 0);
        }
    }

    constexpr cstring (std::nullptr_t) noexcept
    : allocator_type (), _M_gBuffer (), _M_uLength ()
    { }

    inline cstring& operator = (cstring<T, TAlloc>&& gObj) noexcept
    {
        _M_uLength      = gObj._M_uLength;
        _M_gBuffer      = gObj._M_gBuffer;
        gObj._M_gBuffer = { 0 }         ;
        gObj._M_uLength =   0           ;
        return *this;
    }

    inline void erase () noexcept
    {
        if (!_M_gBuffer.data or !_M_uLength) return;
        _M_gBuffer.data[0] = '\0';
        _M_uLength = 0;
    }

    template <typename U, class TAtor>
    friend void copy_to_string (cstring<U, TAtor>& copy_to,
                                U const*           copy_from,
                                size_type          length) noexcept;

    template <typename U, class TAtor>
    friend cstring<U, TAtor>& assign_to_string (cstring<U, TAtor>& assign_to,
                                                U const*           assign_from,
                                                size_type          length) noexcept;

    template <typename U, class TAtor>
    friend cstring<U, TAtor>& add_to_string (cstring<U, TAtor>& add_to,
                                             U const*           add_from,
                                             size_type          add_length) noexcept;

    template <typename U, class TAtor>
    friend cstring<U, TAtor> operator + (cstring<U, TAtor> const& obj1,
                                         cstring<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend cstring<U, TAtor> operator + (cstring<U, TAtor> const& obj1,
                                         U const*                 obj2) noexcept;

    template <typename U, class TAtor>
    friend cstring<U, TAtor>& operator += (cstring<U, TAtor>&       obj1,
                                           cstring<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend cstring<U, TAtor>& operator += (cstring<U, TAtor>& obj1,
                                           U const*           obj2) noexcept;

    template <typename U, class TAtor>
    friend bool operator == (cstring<U, TAtor> const& obj1,
                             cstring<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend bool operator == (cstring<U, TAtor> const& obj1,
                             U const*                text2) noexcept;

private:
    Buffer    _M_gBuffer;
    size_type _M_uLength;
};

template <typename T, class TAlloc>
cstring<T, TAlloc> cstring<T, TAlloc>::substr (size_type uBeginPos,
                                               size_type uEndPos)
{
    typedef cstring<T, TAlloc> fstring;

    if (empty () or
            static_cast<ptrdiff> (uEndPos - uBeginPos) <= 0 or
            uBeginPos > (_M_uLength - 1))
        return fstring (nullptr);

    fstring   gSubStr (uEndPos - uBeginPos);
    size_type i = 0;

    while (uBeginPos <= uEndPos and i < _M_uLength)
        gSubStr._M_gBuffer.data[i++] = _M_gBuffer.data[uBeginPos++];

    gSubStr[i] = T (0);
    return std::move (gSubStr); // ?
}

// ====================================================

template <typename T, class TAlloc>
void copy_to_string (cstring<T, TAlloc>&                    gObj,
                     T const*                               pFromText,
                     typename cstring<T, TAlloc>::size_type uLength) noexcept
{
    if (pFromText)
    {
        if (!(gObj._M_gBuffer.data = gObj.allocate (++uLength)))
            return;

        std::copy (pFromText, pFromText + uLength, gObj._M_gBuffer.data);

        gObj._M_gBuffer.data[--uLength] = '\0';
        gObj._M_gBuffer.capacity = gObj._M_uLength = uLength;
    }
}

// ====================================================

template <typename T>
constexpr T const* strend (T const* pBegin, std::size_t uLen) noexcept
{ return pBegin + uLen + sizeof (T); }


template <typename T, class TAlloc>
cstring<T, TAlloc>& assign_to_string (cstring<T, TAlloc>&                    gObj,
                                      T const*                               pFromText,
                                      typename cstring<T, TAlloc>::size_type uLength) noexcept
{
    if (!pFromText) return gObj;

    if (uLength > gObj._M_gBuffer.capacity)
    {
        gObj.TAlloc::deallocate (gObj._M_gBuffer.data);
        if (!(gObj._M_gBuffer.data = gObj.TAlloc::allocate (uLength + 1)))
        {
            gObj._M_gBuffer.capacity = gObj._M_uLength = 0;
            return gObj;
        }

        gObj._M_gBuffer.capacity = uLength;
    }

    std::copy (pFromText, strend (pFromText, uLength),
               gObj._M_gBuffer.data);
    gObj._M_uLength = uLength;

    gObj._M_gBuffer.data[uLength] = '\0';
    return gObj;
}

// ====================================================

template <typename T, class TAlloc>
cstring<T, TAlloc>& add_to_string (cstring<T, TAlloc>& gObj,
                                   T const*            pFromText,
                                   std::size_t         uAddLength) noexcept
{
    typename cstring<T, TAlloc>::size_type const uLength =
             gObj._M_uLength + uAddLength;

    if (uLength > gObj._M_gBuffer.capacity)
    {
        T* const pOldBuffer = gObj._M_gBuffer.data;

        if (!(gObj._M_gBuffer.data = gObj.TAlloc::allocate (uLength + 1)))
            return gObj;

        std::copy (pOldBuffer,
                   pOldBuffer + (sizeof (T) * gObj._M_uLength),
                   gObj._M_gBuffer.data);

        gObj._M_gBuffer.capacity = uLength;
        gObj.TAlloc::deallocate (pOldBuffer);
    }

    std::copy (pFromText,
               pFromText + ++uAddLength,
               gObj._M_gBuffer.data + gObj._M_uLength);

    gObj._M_uLength = uLength;
    return gObj;
}

// ====================================================

template <typename T, class TAlloc>
bool operator == (cstring<T, TAlloc> const& lhObj,
                  cstring<T, TAlloc> const& rhObj) noexcept
{
    if (lhObj._M_uLength != rhObj._M_uLength) return false;

    for (auto i = 0; i < lhObj._M_uLength; ++i)
        if (lhObj._M_gBuffer.data[i] != rhObj._M_gBuffer.data[i]) return false;
    return true;
}

// ====================================================

template <typename T, class TAlloc>
bool operator == (cstring<T, TAlloc> const& lhObj, T const* pText2) noexcept
{
    const auto uSize2 = strlen (pText2);
    if (lhObj._M_uLength != uSize2) return false;

    for (auto i = 0; i < uSize2; ++i)
        if (lhObj._M_gBuffer.data[i] != pText2[i]) return false;
    return true;
}

// ====================================================

template <typename T, class TAlloc>
bool operator != (cstring<T, TAlloc> const& lhObj,
                  cstring<T, TAlloc> const& rhObj) noexcept
{ return !(lhObj == rhObj); }


template <typename T, class TAlloc>
bool operator != (cstring<T, TAlloc> const& lhObj,
                  T const*                  pText2) noexcept
{ return !(lhObj == pText2); }

// ====================================================

template <typename T, class TAlloc>
inline cstring<T, TAlloc>& operator += (cstring<T, TAlloc>&       lhObj,
                                        cstring<T, TAlloc> const& rhObj) noexcept
{ return add_to_string (lhObj, rhObj._M_gBuffer.data, rhObj._M_uLength); }

template <typename T, class TAlloc>
inline cstring<T, TAlloc>& operator += (cstring<T>& gObj,
                                        T const*    pText) noexcept
{ return add_to_string (gObj, pText, std::strlen (pText)); }

// ====================================================

template <typename T, class TAlloc>
inline cstring<T, TAlloc> operator + (cstring<T, TAlloc> const& lhObj,
                                      cstring<T, TAlloc> const& rhObj) noexcept
{
    cstring<T, TAlloc> gStr (lhObj);
    return add_to_string (gStr, rhObj._M_gBuffer.data, rhObj._M_uLength);
}

// ====================================================

template <typename T, class TAlloc>
inline cstring<T, TAlloc> operator + (cstring<T, TAlloc> const& gObj,
                                      T const*                  pText) noexcept
{
    cstring<T, TAlloc> gStr (gObj);
    return add_to_string (gStr, pText, std::strlen (pText));
}

// ====================================================

template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using std_string = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using std_istringstream = std::basic_istringstream<T, std::char_traits<T>, std::allocator<T>>;

template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using std_ostringstream = std::basic_ostringstream<T, std::char_traits<T>, std::allocator<T>>;
template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using std_stringstream = std::basic_stringstream<T, std::char_traits<T>, std::allocator<T>>;

// ====================================================

template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using used_string = std::basic_string<T, std::char_traits<T>, memory::allocator<T>>;

template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using used_istringstream = std::basic_istringstream<T, std::char_traits<T>, memory::allocator<T>>;

template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using used_ostringstream = std::basic_ostringstream<T, std::char_traits<T>, memory::allocator<T>>;
template <typename T,
          typename = typename std::enable_if_t<is_char<T>::value>>
using used_stringstream = std::basic_stringstream<T, std::char_traits<T>, memory::allocator<T>>;

// ====================================================

typedef std::basic_string<char  , std::char_traits<char>  , memory::allocator<char>>   string    ;
typedef std::basic_string<char16, std::char_traits<char16>, memory::allocator<char16>> u16string ;
typedef std::basic_string<char32, std::char_traits<char32>, memory::allocator<char32>> u32string ;
typedef std::basic_string<wchar , std::char_traits<wchar> , memory::allocator<wchar>>  wstring   ;
typedef cstring<char  , memory::allocator<char>>                                       fstring   ;
typedef cstring<char16, memory::allocator<char16>>                                     fu16string;
typedef cstring<char32, memory::allocator<char32>>                                     fu32string;
typedef cstring<wchar , memory::allocator<wchar>>                                      fwstring  ;

// ====================================================

typedef std::basic_istringstream<char  , std::char_traits<char>  , memory::allocator<char>>
istringstream;
typedef std::basic_istringstream<char16, std::char_traits<char16>, memory::allocator<char16>>
u16istringstream;
typedef std::basic_istringstream<char32, std::char_traits<char32>, memory::allocator<char32>>
u32istringstream;
typedef std::basic_istringstream<wchar , std::char_traits<wchar> , memory::allocator<wchar>>
wistringstream;

// ====================================================

typedef std::basic_ostringstream<char  , std::char_traits<char>  , memory::allocator<char>>
ostringstream;
typedef std::basic_ostringstream<char16, std::char_traits<char16>, memory::allocator<char16>>
u16ostringstream;
typedef std::basic_ostringstream<char32, std::char_traits<char32>, memory::allocator<char32>>
u32ostringstream;
typedef std::basic_ostringstream<wchar , std::char_traits<wchar> , memory::allocator<wchar>>
wostringstream;

// ====================================================

typedef std::basic_stringstream<char  , std::char_traits<char>  , memory::allocator<char>>
stringstream;
typedef std::basic_stringstream<char16, std::char_traits<char16>, memory::allocator<char16>>
u16stringstream;
typedef std::basic_stringstream<char32, std::char_traits<char32>, memory::allocator<char32>>
u32stringstream;
typedef std::basic_stringstream<wchar , std::char_traits<wchar> , memory::allocator<wchar>>
wstringstream;

// ====================================================

/// is of string type
template <typename T, typename = typename T::allocator_type>
struct is_string : std::false_type
{ };

/// is std::basic_string type
template <typename Allocator>
struct is_string<std::basic_string<char, std::char_traits<char>, Allocator>, Allocator> : std::true_type
{ };

/// is std::basic_string type
template <typename Allocator>
struct is_string<std::basic_string<char16, std::char_traits<char16>, Allocator>, Allocator> : std::true_type
{ };

/// is std::basic_string type
template <typename Allocator>
struct is_string<std::basic_string<char32, std::char_traits<char32>, Allocator>, Allocator> : std::true_type
{ };

/// is std::basic_string type
template <typename Allocator>
struct is_string<std::basic_string<wchar, std::char_traits<wchar>, Allocator>, Allocator> : std::true_type
{ };

template <typename Allocator>
struct is_string<cstring<char, Allocator>, Allocator> : std::true_type
{ };

template <typename Allocator>
struct is_string<cstring<char16, Allocator>, Allocator> : std::true_type
{ };

template <typename Allocator>
struct is_string<cstring<char32, Allocator>, Allocator> : std::true_type
{ };

template <typename Allocator>
struct is_string<cstring<wchar, Allocator>, Allocator> : std::true_type
{ };

/// is of string type -> value
template <typename T, typename Allocator = typename T::allocator_type>
inline constexpr auto const is_string_v = is_string<T, Allocator>::value;

} // namespace cppual

// ====================================================

namespace std {

//template <typename Char>
//struct hash<basic_string<Char, char_traits<Char>, cppual::memory::allocator<Char>>>
//{
//    static_assert (cppual::is_char<Char>::value, "hash is not a char!");

//    typedef basic_string<Char, char_traits<Char>, cppual::memory::allocator<Char>> string_type;

//    size_t operator () (string_type const& value) const
//    {
//        return cppual::constexpr_hash(value.c_str());
//    }
//};

// ====================================================

} // namespace std

#endif // __cplusplus
#endif // CPPUAL_TEXT_STRING_H_
