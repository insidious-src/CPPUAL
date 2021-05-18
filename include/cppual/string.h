/*
 * Product: C++ Unified Abstraction Library
 * Author: K. Petrov
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2018 insidious
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

template <typename T, typename TAlloc = Memory::Allocator<T>>
class String : public TAlloc
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
        CharType<value_type>* data    ;
        size_type             capacity;
    };

    constexpr String () noexcept = default;
    String substr (size_type begin_pos, size_type end_pos);

    constexpr const_pointer  c_str         () const noexcept { return  m_gBuffer.data    ; }
    constexpr const_pointer  data          () const noexcept { return  m_gBuffer.data    ; }
    constexpr size_type      length        () const noexcept { return  m_uLength         ; }
    constexpr size_type      size          () const noexcept { return  size()            ; }
    constexpr bool           empty         () const noexcept { return !m_uLength         ; }
    constexpr allocator_type get_allocator () const noexcept { return *this              ; }

    inline String& operator = (T const* pText) noexcept
    { return assignToString (*this, pText, std::strlen (pText)); }

    inline String& operator = (String<T, TAlloc> const& gObj) noexcept
    { return assignToString (*this, gObj.m_gBuffer.data, gObj.m_uLength); }

    constexpr reference operator [] (size_type uPos) noexcept
    { return *(m_gBuffer.data + uPos); }

    constexpr const_reference operator [] (size_type uPos) const noexcept
    { return *(m_gBuffer.data + uPos); }

    inline ~String () noexcept
    { if (m_gBuffer.data) this->TAlloc::deallocate (m_gBuffer.data, 0); }

    inline explicit String (pointer       pText,
                            TAlloc const& gAtor = TAlloc ()) noexcept
    : allocator_type (gAtor),
      m_gBuffer ()
    { copyToString (*this, pText, std::strlen (pText)); }

    constexpr String (TAlloc const& gAtor) noexcept
    : allocator_type (gAtor),
      m_gBuffer (),
      m_uLength ()
    { }

    inline String (pointer pText) noexcept
    : allocator_type (), m_gBuffer { pText, std::strlen (pText) }
    { m_uLength = m_gBuffer.capacity; }

    inline String (String<T, TAlloc> const& gObj) noexcept
    : allocator_type (gObj), m_gBuffer ()
    { copyToString (*this, gObj.m_gBuffer.data, gObj.m_uLength); }

    inline String (String<T, TAlloc>&& gObj) noexcept
    : allocator_type (gObj),
      m_gBuffer (gObj.m_gBuffer),
      m_uLength (gObj.m_uLength)
    {
        gObj.m_gBuffer.data     = nullptr;
        gObj.m_gBuffer.capacity = gObj.m_uLength = 0;
    }

    inline String (size_type uCapacity) noexcept : String (nullptr)
    {
        if (uCapacity)
        {
            m_gBuffer.data = this->TAlloc::allocate (uCapacity + 1U);
            m_uLength = m_gBuffer.capacity = (m_gBuffer.data ? uCapacity : 0);
        }
    }

    constexpr String (std::nullptr_t) noexcept
    : allocator_type (), m_gBuffer (), m_uLength ()
    { }

    inline String& operator = (String<T, TAlloc>&& gObj) noexcept
    {
        m_uLength      = gObj.m_uLength;
        m_gBuffer      = gObj.m_gBuffer;
        gObj.m_gBuffer = { 0 }         ;
        gObj.m_uLength =   0           ;
        return *this;
    }

    inline void erase () noexcept
    {
        if (!m_gBuffer.data or !m_uLength) return;
        m_gBuffer.data[0] = '\0';
        m_uLength = 0;
    }

    template <typename U, class TAtor>
    friend void copyToString (String<U, TAtor>& copy_to,
                              U const*          copy_from,
                              size_type         length) noexcept;

    template <typename U, class TAtor>
    friend String<U, TAtor>& assignToString (String<U, TAtor>& assign_to,
                                             U const*          assign_from,
                                             size_type         length) noexcept;

    template <typename U, class TAtor>
    friend String<U, TAtor>& addToString (String<U, TAtor>& add_to,
                                          U const*          add_from,
                                          size_type         add_length) noexcept;

    template <typename U, class TAtor>
    friend String<U, TAtor> operator + (String<U, TAtor> const& obj1,
                                        String<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend String<U, TAtor> operator + (String<U, TAtor> const& obj1,
                                        U const*                obj2) noexcept;

    template <typename U, class TAtor>
    friend String<U, TAtor>& operator += (String<U, TAtor>&       obj1,
                                          String<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend String<U, TAtor>& operator += (String<U, TAtor>& obj1,
                                          U const*          obj2) noexcept;

    template <typename U, class TAtor>
    friend bool operator == (String<U, TAtor> const& obj1,
                             String<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend bool operator == (String<U, TAtor> const& obj1,
                             U const*                text2) noexcept;

private:
    Buffer    m_gBuffer;
    size_type m_uLength;
};

template <typename T, class TAlloc>
String<T, TAlloc> String<T, TAlloc>::substr (size_type uBeginPos,
                                             size_type uEndPos)
{
    typedef String<T, TAlloc> fstring;

    if (empty () or
            static_cast<ptrdiff> (uEndPos - uBeginPos) <= 0 or
            uBeginPos > (m_uLength - 1))
        return fstring (nullptr);

    fstring   gSubStr (uEndPos - uBeginPos);
    size_type i = 0;

    while (uBeginPos <= uEndPos and i < m_uLength)
        gSubStr.m_gBuffer.data[i++] = m_gBuffer.data[uBeginPos++];

    gSubStr[i] = T (0);
    return std::move (gSubStr); // ?
}

// ====================================================

template <typename T, class TAlloc>
void copyToString (String<T, TAlloc>&                    gObj,
                   T const*                              pFromText,
                   typename String<T, TAlloc>::size_type uLength) noexcept
{
    if (pFromText)
    {
        if (!(gObj.m_gBuffer.data = gObj.allocate (++uLength)))
            return;

        std::copy (pFromText, pFromText + uLength, gObj.m_gBuffer.data);

        gObj.m_gBuffer.data[--uLength] = '\0';
        gObj.m_gBuffer.capacity = gObj.m_uLength = uLength;
    }
}

// ====================================================

template <typename T>
constexpr T const* strend (T const* pBegin, std::size_t uLen) noexcept
{ return pBegin + uLen + sizeof (T); }


template <typename T, class TAlloc>
String<T, TAlloc>& assignToString (String<T, TAlloc>&                    gObj,
                                   T const*                              pFromText,
                                   typename String<T, TAlloc>::size_type uLength) noexcept
{
    if (!pFromText) return gObj;

    if (uLength > gObj.m_gBuffer.capacity)
    {
        gObj.TAlloc::deallocate (gObj.m_gBuffer.data);
        if (!(gObj.m_gBuffer.data = gObj.TAlloc::allocate (uLength + 1)))
        {
            gObj.m_gBuffer.capacity = gObj.m_uLength = 0;
            return gObj;
        }

        gObj.m_gBuffer.capacity = uLength;
    }

    std::copy (pFromText, strend (pFromText, uLength),
               gObj.m_gBuffer.data);
    gObj.m_uLength = uLength;

    gObj.m_gBuffer.data[uLength] = '\0';
    return gObj;
}

// ====================================================

template <typename T, class TAlloc>
String<T, TAlloc>& addToString (String<T, TAlloc>& gObj,
                                T const*           pFromText,
                                std::size_t        uAddLength) noexcept
{
    typename String<T, TAlloc>::size_type const uLength =
             gObj.m_uLength + uAddLength;

    if (uLength > gObj.m_gBuffer.capacity)
    {
        T* const pOldBuffer = gObj.m_gBuffer.data;

        if (!(gObj.m_gBuffer.data = gObj.TAlloc::allocate (uLength + 1)))
            return gObj;

        std::copy (pOldBuffer,
                   pOldBuffer + (sizeof (T) * gObj.m_uLength),
                   gObj.m_gBuffer.data);

        gObj.m_gBuffer.capacity = uLength;
        gObj.TAlloc::deallocate (pOldBuffer);
    }

    std::copy (pFromText,
               pFromText + ++uAddLength,
               gObj.m_gBuffer.data + gObj.m_uLength);

    gObj.m_uLength = uLength;
    return gObj;
}

// ====================================================

template <typename T, class TAlloc>
bool operator == (String<T, TAlloc> const& lhObj,
                  String<T, TAlloc> const& rhObj) noexcept
{
    if (lhObj.m_uLength != rhObj.m_uLength) return false;

    for (auto i = 0; i < lhObj.m_uLength; ++i)
        if (lhObj.m_gBuffer.data[i] != rhObj.m_gBuffer.data[i]) return false;
    return true;
}

// ====================================================

template <typename T, class TAlloc>
bool operator == (String<T, TAlloc> const& lhObj, T const* pText2) noexcept
{
    const auto uSize2 = strlen (pText2);
    if (lhObj.m_uLength != uSize2) return false;

    for (auto i = 0; i < uSize2; ++i)
        if (lhObj.m_gBuffer.data[i] != pText2[i]) return false;
    return true;
}

// ====================================================

template <typename T, class TAlloc>
bool operator != (String<T, TAlloc> const& lhObj,
                  String<T, TAlloc> const& rhObj) noexcept
{ return !(lhObj == rhObj); }


template <typename T, class TAlloc>
bool operator != (String<T, TAlloc> const& lhObj,
                  T const*                 pText2) noexcept
{ return !(lhObj == pText2); }

// ====================================================

template <typename T, class TAlloc>
inline String<T, TAlloc>& operator += (String<T, TAlloc>&       lhObj,
                                       String<T, TAlloc> const& rhObj) noexcept
{ return addToString (lhObj, rhObj.m_gBuffer.data, rhObj.m_uLength); }

template <typename T, class TAlloc>
inline String<T, TAlloc>& operator += (String<T>& gObj,
                                       T const*   pText) noexcept
{ return addToString (gObj, pText, std::strlen (pText)); }

// ====================================================

template <typename T, class TAlloc>
inline String<T, TAlloc> operator + (String<T, TAlloc> const& lhObj,
                                     String<T, TAlloc> const& rhObj) noexcept
{
    String<T, TAlloc> gStr (lhObj);
    return addToString (gStr, rhObj.m_gBuffer.data, rhObj.m_uLength);
}

// ====================================================

template <typename T, class TAlloc>
inline String<T, TAlloc> operator + (String<T, TAlloc> const& gObj,
                                     T const*                 pText) noexcept
{
    String<T, TAlloc> gStr (gObj);
    return addToString (gStr, pText, std::strlen (pText));
}

// ====================================================

template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using std_string = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using std_istringstream = std::basic_istringstream<T, std::char_traits<T>, std::allocator<T>>;

template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using std_ostringstream = std::basic_ostringstream<T, std::char_traits<T>, std::allocator<T>>;
template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using std_stringstream = std::basic_stringstream<T, std::char_traits<T>, std::allocator<T>>;

// ====================================================

template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using used_string = std::basic_string<T, std::char_traits<T>, Memory::Allocator<T>>;

template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using used_istringstream = std::basic_istringstream<T, std::char_traits<T>, Memory::Allocator<T>>;

template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using used_ostringstream = std::basic_ostringstream<T, std::char_traits<T>, Memory::Allocator<T>>;
template <typename T,
          typename = typename std::enable_if<is_char<T>::value>::type>
using used_stringstream = std::basic_stringstream<T, std::char_traits<T>, Memory::Allocator<T>>;

// ====================================================

typedef std::basic_string<char  , std::char_traits<char>  , Memory::Allocator<char>>   string    ;
typedef std::basic_string<char16, std::char_traits<char16>, Memory::Allocator<char16>> u16string ;
typedef std::basic_string<char32, std::char_traits<char32>, Memory::Allocator<char32>> u32string ;
typedef std::basic_string<wchar , std::char_traits<wchar> , Memory::Allocator<wchar>>  wstring   ;
typedef String<char  , Memory::Allocator<char>>                                        fstring   ;
typedef String<char16, Memory::Allocator<char16>>                                      fu16string;
typedef String<char32, Memory::Allocator<char32>>                                      fu32string;
typedef String<wchar , Memory::Allocator<wchar>>                                       fwstring  ;

typedef std::basic_istringstream<char  , std::char_traits<char>  , Memory::Allocator<char>>
istringstream;
typedef std::basic_istringstream<char16, std::char_traits<char16>, Memory::Allocator<char16>>
u16istringstream;
typedef std::basic_istringstream<char32, std::char_traits<char32>, Memory::Allocator<char32>>
u32istringstream;
typedef std::basic_istringstream<wchar , std::char_traits<wchar> , Memory::Allocator<wchar>>
wistringstream;

typedef std::basic_ostringstream<char  , std::char_traits<char>  , Memory::Allocator<char>>
ostringstream;
typedef std::basic_ostringstream<char16, std::char_traits<char16>, Memory::Allocator<char16>>
u16ostringstream;
typedef std::basic_ostringstream<char32, std::char_traits<char32>, Memory::Allocator<char32>>
u32ostringstream;
typedef std::basic_ostringstream<wchar , std::char_traits<wchar> , Memory::Allocator<wchar>>
wostringstream;

typedef std::basic_stringstream<char  , std::char_traits<char>  , Memory::Allocator<char>>
stringstream;
typedef std::basic_stringstream<char16, std::char_traits<char16>, Memory::Allocator<char16>>
u16stringstream;
typedef std::basic_stringstream<char32, std::char_traits<char32>, Memory::Allocator<char32>>
u32stringstream;
typedef std::basic_stringstream<wchar , std::char_traits<wchar> , Memory::Allocator<wchar>>
wstringstream;

// ====================================================

template <typename Allocator>
struct is_string<String<char, Allocator>> : std::true_type
{ };

template <typename Allocator>
struct is_string<String<char16, Allocator>> : std::true_type
{ };

template <typename Allocator>
struct is_string<String<char32, Allocator>> : std::true_type
{ };

template <typename Allocator>
struct is_string<String<wchar, Allocator>> : std::true_type
{ };

// ====================================================

template<typename T,
         typename Char = char,
         typename      = typename std::enable_if<is_char<Char>::value>::type>
inline used_string<Char> to_string
                            (T val, typename used_string<Char>::allocator_type const& a =
                             typename used_string<Char>::allocator_type())
{
    typedef std::basic_string<Char, std::char_traits<Char>, Memory::Allocator<Char>> out_string_type;

    out_string_type in_str (a);

    used_ostringstream<Char> ss(in_str);
    ss << val;

    return ss.str();
}

// ====================================================

template<typename Char = char,
         typename      = typename std::enable_if<is_char<Char>::value>::type>
inline std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> to_std_string
                            (used_string<Char> const& val)
{
    typedef std::basic_string<Char, std::char_traits<Char>, std::allocator<Char>> out_string_type;

    return out_string_type(val.c_str(), val.size());
}

} // namespace cppual

// ====================================================

namespace std {

template<typename Char>
struct hash<basic_string<Char, char_traits<Char>, cppual::Memory::Allocator<Char>>>
{
    static_assert (cppual::is_char<Char>::value, "hash is not a char!");

    typedef basic_string<Char, char_traits<Char>, cppual::Memory::Allocator<Char>> string_type;

    size_t operator () (string_type const& value) const
    {
        return cppual::const_hash(value.c_str());
    }
};

} // namespace std

#endif // __cplusplus
#endif // CPPUAL_TEXT_STRING_H_
