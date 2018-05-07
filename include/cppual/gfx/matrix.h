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

#ifndef CPPUAL_GFX_MATRIX_H_
#define CPPUAL_GFX_MATRIX_H_
#ifdef __cplusplus

#include <deque>
#include <array>
#include <memory>
#include <cppual/gfx/coord.h>

using std::array;
using std::deque;

namespace cppual { namespace Graphics {

enum class MatrixType : unsigned char
{
    Projection,
    ModelTransform,
    Texture,
    Color
};

// =========================================================

template <typename T, typename Allocator = std::allocator<T> >
class DynamicMatrix
{
public:
    typedef T                             value_type;
    typedef T const*                      const_pointer;
    typedef deque<T, Allocator>           array_type;
    typedef Allocator                     allocator_type;
    typedef typename Allocator::size_type size_type;

    DynamicMatrix<T>& identity ();

    constexpr size_type size   () const noexcept { return m_uCols * m_uRows; }
    constexpr size_type cols   () const noexcept { return m_uCols; }
    constexpr size_type rows   () const noexcept { return m_uRows; }
    constexpr size_type maxRow () noexcept { return m_uRows - 1; }
    constexpr size_type maxCol () noexcept { return m_uCols - 1; }

    constexpr value_type& operator [] (size_type uIdx) const noexcept
    { return m_matrix[uIdx]; }

    value_type& operator [] (size_type uIdx) noexcept
    { return m_matrix[uIdx]; }

    constexpr void swapCoord () noexcept
    { if (m_uCols != m_uRows) std::swap (m_uCols, m_uRows); }

    constexpr const_pointer data () const noexcept
    { return m_matrix.data (); }

    virtual ~DynamicMatrix ()
    { }

    constexpr DynamicMatrix () noexcept
    : m_matrix (),
      m_uCols (),
      m_uRows ()
    { }

    DynamicMatrix (DynamicMatrix&& gObj) noexcept
    : m_matrix (std::move (gObj.m_matrix)),
      m_uCols (gObj.m_uCols),
      m_uRows (gObj.m_uRows)
    { }

    DynamicMatrix (DynamicMatrix const& gObj) noexcept
    : m_matrix (gObj.m_matrix),
      m_uCols (gObj.m_uCols),
      m_uRows (gObj.m_uRows)
    { }

    template <typename... Ts>
    DynamicMatrix (Ts... vals) noexcept
    : m_matrix { vals... }
    { static_assert (sizeof... (vals) == size (), "Wrong number of initializers!"); }

    inline
    DynamicMatrix&
    setColumn (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= m_uCols) idx = maxCol ();
        for (auto i = 0; i < uSize and i < rows (); ++i)
            m_matrix[(m_uCols * i) + idx] = *values[i];
        return *this;
    }

    inline
    DynamicMatrix&
    fillColumn (size_type idx, value_type value) noexcept
    {
        if (idx >= m_uCols) idx = maxCol ();
        for (auto i = 0; i < rows (); ++i)
            m_matrix[(m_uCols * i) + idx] = value;
        return *this;
    }

    inline
    DynamicMatrix&
    setRow (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= m_uRows) idx = maxRow ();
        for (auto i = 0; i < uSize and i < cols (); ++i)
            m_matrix[(m_uCols * idx) + i] = *values[i];
        return *this;
    }

    inline
    DynamicMatrix&
    fillRow (size_type idx, value_type value) noexcept
    {
        if (idx >= m_uRows) idx = maxRow ();
        for (auto i = 0; i < cols (); ++i)
            m_matrix[(m_uCols * idx) + i] = value;
        return *this;
    }

private:
    array_type m_matrix;
    size_type  m_uCols, m_uRows;
};

// =========================================================

template <u16 C, u16 R, typename T = float>
class StaticMatrix
{
public:
    static_assert (1 < C or 1 < R, "Ivalid number of dimensions!");

    typedef T               value_type   ;
    typedef T const*        const_pointer;
    typedef array<T, C * R> array_type   ;
    typedef u16             size_type    ;

    StaticMatrix<C, R, T>& identity ()
    { return *this; }

    constexpr size_type cols () const noexcept
    { return C != R ? m_uCols : C; }

    constexpr size_type rows () const noexcept
    { return C != R ? m_uRows : R; }

    constexpr size_type maxRow () noexcept
    { return m_uRows - 1; }

    constexpr size_type maxCol () noexcept
    { return m_uCols - 1; }

    constexpr size_type colMul (size_type n) noexcept
    { return m_uCols * n; }

    inline
    StaticMatrix<C, R, T>&
    setColumn (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= m_uCols) idx = maxCol ();
        for (auto i = 0; i < uSize and i < rows (); ++i)
            m_matrix[(m_uCols * i) + idx] = *values[i];
        return *this;
    }

    inline
    StaticMatrix<C, R, T>&
    fillColumn (size_type idx, value_type value) noexcept
    {
        if (idx >= m_uCols) idx = maxCol ();
        for (auto i = 0; i < rows (); ++i)
            m_matrix[(m_uCols * i) + idx] = value;
        return *this;
    }

    inline
    StaticMatrix<C, R, T>&
    setRow (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= m_uRows) idx = maxRow ();
        for (auto i = 0; i < uSize and i < cols (); ++i)
            m_matrix[colMul (idx) + i] = *values[i];
        return *this;
    }

    inline
    StaticMatrix<C, R, T>&
    fillRow (size_type idx, value_type value) noexcept
    {
        if (idx >= m_uRows) idx = maxRow ();
        for (auto i = 0; i < cols (); ++i)
            m_matrix[colMul (idx) + i] = value;
        return *this;
    }

    virtual ~StaticMatrix ()
    { }

    constexpr StaticMatrix (StaticMatrix<C, R, T> const& gObj) noexcept
    : m_uCols  (gObj.m_uCols),
      m_uRows  (gObj.m_uRows),
      m_matrix (gObj.m_matrix)
    { }

    constexpr StaticMatrix () noexcept
    : m_uCols  (C),
      m_uRows  (R),
      m_matrix ()
    { }

    template <typename... Ts>
    constexpr StaticMatrix (Ts... vals) noexcept
    : m_uCols (C),
      m_uRows (R),
      m_matrix { vals... }
    { static_assert (sizeof... (vals) == size (), "Wrong number of initializers!"); }

    constexpr value_type& operator [] (size_type uIdx) const noexcept
    { return m_matrix[uIdx]; }

    value_type& operator [] (size_type uIdx) noexcept
    { return m_matrix[uIdx]; }

    static constexpr size_type size () noexcept
    { return C * R; }

    constexpr void swapCoord () noexcept
    { if (C != R) std::swap (m_uCols, m_uRows); }

    const_pointer data () const noexcept
    { return m_matrix.data (); }

    StaticMatrix<C, R, T>& operator  = (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator *= (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator *  (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator += (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator +  (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator -= (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator -  (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator != (StaticMatrix<C, R, T> const&);
    StaticMatrix<C, R, T>& operator == (StaticMatrix<C, R, T> const&);

private:
    size_type  m_uCols, m_uRows;
    array_type m_matrix;
};

// =========================================================

class Matrix2 : public StaticMatrix <2, 2, float>
{
public:
    Matrix2& invert    ();
    Matrix2& transpose () noexcept;
};

// =========================================================

class Matrix3 : public StaticMatrix <3, 3, float>
{
public:
    Matrix3& invert    ();
    Matrix3& transpose () noexcept;
};

// =========================================================

class Matrix4 : public StaticMatrix <4, 4, float>
{
public:
    using StaticMatrix<4, 4, float>::StaticMatrix;

    Matrix4& invert    ();
    Matrix4& transpose () noexcept;
    Matrix4& rotate    (anglef, point3f);
    Matrix4& rotateX   (anglef);
    Matrix4& rotateY   (anglef);
    Matrix4& rotateZ   (anglef);
    Matrix4& translate (point3f) noexcept;
    Matrix4& scale     (value_type);
    Matrix4& scale     (point3f);

    const_pointer transposed () const noexcept
    { return &m_fTranspose[0]; }

private:
    value_type m_fTranspose[size ()];
};

// =========================================================

class ViewMatrix : public Matrix4
{
public:
    enum class Type : bool
    {
        Perspective,
        Orthographic
    };
};

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_MATRIX_H_
