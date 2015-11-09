/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2015 insidious
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

#include <memory>
#include <cstring>
#include <cppual/meta.h>
#include <cppual/memory/mop.h>

namespace cppual { namespace Text {

template <typename T>
struct Buffer
{
    static_assert (is_char<T>::value, "T is not of any char type");

    T*          data;
    std::size_t capacity;
};

template <typename T, class TAlloc = std::allocator<T>>
class FastString : TAlloc
{
public:
    typedef std::size_t size_type;
    typedef T           value_type;
    typedef T*          pointer;
    typedef T const*    const_pointer;
    typedef T&          reference;
    typedef T const&    const_reference;

    constexpr FastString () noexcept = default;
    FastString<T, TAlloc> substr (size_type begin_pos, size_type end_pos);

    constexpr const_pointer c_str         () const noexcept { return m_gBuffer.data; }
    constexpr const_pointer data          () const noexcept { return m_gBuffer.data; }
    constexpr size_type     length        () const noexcept { return m_uLength; }
    constexpr size_type     size          () const noexcept { return m_gBuffer.capacity; }
    constexpr bool          empty         () const noexcept { return !m_uLength; }
    constexpr TAlloc        get_allocator () const noexcept { return *this; }

    inline FastString& operator = (T const* pText) noexcept
    { return assignToString (*this, pText, std::strlen (pText)); }

    inline FastString& operator = (FastString<T, TAlloc> const& gObj) noexcept
    { return assignToString (*this, gObj.m_gBuffer.data, gObj.m_uLength); }

    constexpr reference operator [] (size_type uPos) noexcept
    { return *(m_gBuffer.data + (uPos * sizeof (T))); }

//    inline T operator [] (size_type uPos) noexcept
//    { return *(m_gBuffer.data + (uPos * sizeof (T))); }

    inline ~FastString () noexcept
    { if (m_gBuffer.data) this->TAlloc::deallocate (m_gBuffer.data, 0); }

    inline explicit FastString (pointer       pText,
                                TAlloc const& gAtor = TAlloc ()) noexcept
    : TAlloc (gAtor),
      m_gBuffer ()
    { copyToString (*this, pText, std::strlen (pText)); }

    constexpr FastString (TAlloc const& gAtor = TAlloc ()) noexcept
    : TAlloc (gAtor),
      m_gBuffer (),
      m_uLength ()
    { }

    inline FastString (pointer pText) noexcept
        : TAlloc (), m_gBuffer { pText, std::strlen (pText) }
    { m_uLength = m_gBuffer.capacity; }

    inline FastString (FastString<T, TAlloc> const& gObj) noexcept
    : TAlloc (gObj), m_gBuffer ()
    { copyToString (*this, gObj.m_gBuffer.data, gObj.m_uLength); }

    inline FastString (FastString<T, TAlloc>&& gObj) noexcept
    : TAlloc (gObj),
      m_gBuffer (gObj.m_gBuffer),
      m_uLength (gObj.m_uLength)
    {
        gObj.m_gBuffer.data     = nullptr;
        gObj.m_gBuffer.capacity = gObj.m_uLength = 0;
    }

    inline FastString (size_type uCapacity) noexcept : FastString (nullptr)
    {
        if (uCapacity)
        {
            m_gBuffer.data = this->TAlloc::allocate (uCapacity + 1U);
            m_uLength = m_gBuffer.capacity = (m_gBuffer.data ? uCapacity : 0);
        }
    }

    constexpr FastString (std::nullptr_t) noexcept
    : TAlloc (), m_gBuffer (), m_uLength ()
    { }

    inline FastString& operator = (FastString<T, TAlloc>&& gObj) noexcept
    {
        m_uLength      = gObj.m_uLength;
        m_gBuffer      = gObj.m_gBuffer;
        gObj.m_gBuffer = { 0 };
        gObj.m_uLength = 0;
        return *this;
    }

    inline void erase () noexcept
    {
        if (!m_gBuffer.data or !m_uLength) return;
        do m_gBuffer.data[--m_uLength] = '\0';
        while (m_uLength);
    }

    template <typename U, class TAtor>
    friend void copyToString (FastString<U, TAtor>& copy_to,
                              U const*              copy_from,
                              size_type             length) noexcept;

    template <typename U, class TAtor>
    friend FastString<U, TAtor>& assignToString (FastString<U, TAtor>& assign_to,
                                                 U const*              assign_from,
                                                 size_type             length) noexcept;

    template <typename U, class TAtor>
    friend FastString<U, TAtor>& addToString (FastString<U, TAtor>& add_to,
                                              U const*              add_from,
                                              size_type             add_length) noexcept;

    template <typename U, class TAtor>
    friend FastString<U, TAtor> operator + (FastString<U, TAtor> const& obj1,
                                            FastString<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend FastString<U, TAtor> operator + (FastString<U, TAtor> const& obj1,
                                            U const*                    obj2) noexcept;

    template <typename U, class TAtor>
    friend FastString<U, TAtor>& operator += (FastString<U, TAtor>&       obj1,
                                              FastString<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend FastString<U, TAtor>& operator += (FastString<U, TAtor>& obj1,
                                              U const*              obj2) noexcept;

    template <typename U, class TAtor>
    friend bool operator == (FastString<U, TAtor> const& obj1,
                             FastString<U, TAtor> const& obj2) noexcept;

    template <typename U, class TAtor>
    friend bool operator == (FastString<U, TAtor> const& obj1,
                             U const*                    text2) noexcept;

private:
    Buffer<T> m_gBuffer;
    size_type m_uLength;
};

template <typename T, class TAlloc>
FastString<T, TAlloc> FastString<T, TAlloc>::substr (size_type uBeginPos,
                                                     size_type uEndPos)
{
    typedef FastString<T, TAlloc> fstring;

    if (empty () or
            static_cast<ptrdiff> (uEndPos - uBeginPos) <= 0 or
            uBeginPos > (m_uLength - 1))
        return fstring (nullptr);

    fstring   gSubStr (uEndPos - uBeginPos);
    size_type i = 0;

    while (uBeginPos <= uEndPos and i < m_uLength)
        gSubStr.m_gBuffer.data[i++] = m_gBuffer.data[uBeginPos++];

    gSubStr[i] = T (0);
    return std::move (gSubStr);
}

// ====================================================

template <typename T, class TAlloc>
void copyToString (FastString<T, TAlloc>&                    gObj,
                   T const*                                  pFromText,
                   typename FastString<T, TAlloc>::size_type uLength) noexcept
{
    if (pFromText)
    {
        if (!(gObj.m_gBuffer.data = gObj.allocate (++uLength)))
            return;

        std::copy (pFromText, pFromText + uLength, gObj.m_gBuffer.data);
        gObj.m_gBuffer.capacity = gObj.m_uLength = --uLength;
    }
}

// ====================================================

template <typename T>
constexpr T const* strend (T const* pBegin, std::size_t uLen) noexcept
{ return pBegin + (sizeof (T) * uLen) + sizeof (T); }

template <typename T, class TAlloc>
FastString<T, TAlloc>& assignToString (FastString<T, TAlloc>&                    gObj,
                                       T const*                                  pFromText,
                                       typename FastString<T, TAlloc>::size_type uLength) noexcept
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
    return gObj;
}

// ====================================================

template <typename T, class TAlloc>
FastString<T, TAlloc>& addToString (FastString<T, TAlloc>& gObj,
                                    T const*               pFromText,
                                    std::size_t            uAddLength) noexcept
{
    typename FastString<T, TAlloc>::size_type const uLength =
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
               pFromText + (sizeof (T) * ++uAddLength),
               gObj.m_gBuffer.data + (sizeof (T) * gObj.m_uLength));

    gObj.m_uLength = uLength;
    return gObj;
}

// ====================================================

template <typename T, class TAlloc>
bool operator == (FastString<T, TAlloc> const& lhObj,
                  FastString<T, TAlloc> const& rhObj) noexcept
{
    if (lhObj.m_uLength != rhObj.m_uLength) return false;

    for (auto i = 0; i < lhObj.m_uLength; ++i)
        if (lhObj.m_gBuffer.data[i] != rhObj.m_gBuffer.data[i]) return false;
    return true;
}
// ====================================================

template <typename T, class TAlloc>
bool operator == (FastString<T, TAlloc> const& lhObj, T const* pText2) noexcept
{
    const auto uSize2 = strlen (pText2);
    if (lhObj.m_uLength != uSize2) return false;

    for (auto i = 0; i < uSize2; ++i)
        if (lhObj.m_gBuffer.data[i] != pText2[i]) return false;
    return true;
}

// ====================================================

template <typename T, class TAlloc>
bool operator != (FastString<T, TAlloc> const& lhObj,
                  FastString<T, TAlloc> const& rhObj) noexcept
{ return !(lhObj == rhObj); }


template <typename T, class TAlloc>
bool operator != (FastString<T, TAlloc> const& lhObj,
                  T const*                     pText2) noexcept
{ return !(lhObj == pText2); }

// ====================================================

template <typename T, class TAlloc>
inline FastString<T, TAlloc>& operator += (FastString<T, TAlloc>&        lhObj,
                                           FastString<T, TAlloc> const& rhObj) noexcept
{ return addToString (lhObj, rhObj.m_gBuffer.data, rhObj.m_uLength); }

template <typename T, class TAlloc>
inline FastString<T, TAlloc>& operator += (FastString<T>& gObj,
                                           T const*       pText) noexcept
{ return addToString (gObj, pText, std::strlen (pText)); }

// ====================================================

template <typename T, class TAlloc>
inline FastString<T, TAlloc> operator + (FastString<T, TAlloc> const& lhObj,
                                         FastString<T, TAlloc> const& rhObj) noexcept
{
    FastString<T, TAlloc> gStr (lhObj);
    return addToString (gStr, rhObj.m_gBuffer.data, rhObj.m_uLength);
}

// ====================================================

template <typename T, class TAlloc>
inline FastString<T, TAlloc> operator + (FastString<T, TAlloc> const& gObj,
                                         T const*                      pText) noexcept
{
    FastString<T, TAlloc> gStr (gObj);
    return addToString (gStr, pText, std::strlen (pText));
}

} } // namespace Text

#endif // __cplusplus
#endif // CPPUAL_TEXT_STRING_H_
