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

#ifndef CPPUAL_GFX_MATRIX_H_
#define CPPUAL_GFX_MATRIX_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>
#include <cppual/gfx/color.h>
#include <cppual/containers.h>
#include <cppual/memory/allocator.h>

#include <array>
#include <memory>

namespace cppual { namespace gfx {

enum class MatrixType : byte
{
    Projection,
    ModelTransform,
    Texture,
    Color
};

// =========================================================

template < typename T, typename Alloc = memory::allocator<T> >
class dynamic_matrix
{
public:
    typedef T                                  value_type    ;
    typedef T const*                           const_pointer ;
    typedef memory::AllocatorType<Alloc>       allocator_type;
    typedef deque<T>                           array_type    ;
    typedef typename allocator_type::size_type size_type     ;

    dynamic_matrix<T>& identity ();

    constexpr size_type size    () const noexcept { return _M_uCols * _M_uRows; }
    constexpr size_type cols    () const noexcept { return _M_uCols; }
    constexpr size_type rows    () const noexcept { return _M_uRows; }
    constexpr size_type max_row () noexcept { return _M_uRows - 1; }
    constexpr size_type max_col () noexcept { return _M_uCols - 1; }

    constexpr value_type& operator [] (size_type uIdx) const noexcept
    { return _M_matrix[uIdx]; }

    value_type& operator [] (size_type uIdx) noexcept
    { return _M_matrix[uIdx]; }

    constexpr void swap_coord () noexcept
    { if (_M_uCols != _M_uRows) std::swap (_M_uCols, _M_uRows); }

    constexpr const_pointer data () const noexcept
    { return _M_matrix.data (); }

    virtual ~dynamic_matrix ()
    { }

    constexpr dynamic_matrix () noexcept
    : _M_matrix (),
      _M_uCols  (),
      _M_uRows  ()
    { }

    dynamic_matrix (dynamic_matrix&& gObj) noexcept
    : _M_matrix (std::move (gObj._M_matrix)),
      _M_uCols  (gObj._M_uCols),
      _M_uRows  (gObj._M_uRows)
    { }

    dynamic_matrix (dynamic_matrix const& gObj) noexcept
    : _M_matrix (gObj._M_matrix),
      _M_uCols  (gObj._M_uCols),
      _M_uRows  (gObj._M_uRows)
    { }

    template <typename... Ts>
    dynamic_matrix (Ts... vals) noexcept
    : _M_matrix { vals... }
    { static_assert (sizeof... (vals) == size (), "Wrong number of initializers!"); }

    inline
    dynamic_matrix&
    set_column (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= _M_uCols) idx = max_col ();
        for (auto i = 0; i < uSize and i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = *values[i];
        return *this;
    }

    inline
    dynamic_matrix&
    fill_column (size_type idx, value_type value) noexcept
    {
        if (idx >= _M_uCols) idx = max_col ();
        for (auto i = 0; i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = value;
        return *this;
    }

    inline
    dynamic_matrix&
    set_row (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= _M_uRows) idx = max_row ();
        for (auto i = 0; i < uSize and i < cols (); ++i)
            _M_matrix[(_M_uCols * idx) + i] = *values[i];
        return *this;
    }

    inline
    dynamic_matrix&
    fill_row (size_type idx, value_type value) noexcept
    {
        if (idx >= _M_uRows) idx = max_row ();
        for (auto i = 0; i < cols (); ++i)
            _M_matrix[(_M_uCols * idx) + i] = value;
        return *this;
    }

private:
    array_type _M_matrix;
    size_type  _M_uCols, _M_uRows;
};

// =========================================================

template <u16 C, u16 R, typename T = float>
class static_matrix
{
public:
    static_assert (1 < C || 1 < R, "Ivalid number of dimensions!");

    typedef T                    value_type   ;
    typedef T const*             const_pointer;
    typedef std::array<T, C * R> array_type   ;
    typedef u16                  size_type    ;

    static_matrix<C, R, T>& identity ()
    { return *this; }

    constexpr size_type cols () const noexcept
    { return C != R ? _M_uCols : C; }

    constexpr size_type rows () const noexcept
    { return C != R ? _M_uRows : R; }

    constexpr size_type max_row () noexcept
    { return _M_uRows - 1; }

    constexpr size_type max_col () noexcept
    { return _M_uCols - 1; }

    constexpr size_type col_mul (size_type n) noexcept
    { return _M_uCols * n; }

    inline
    static_matrix<C, R, T>&
    set_column (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= _M_uCols) idx = max_col ();
        for (auto i = 0; i < uSize and i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = *values[i];
        return *this;
    }

    inline
    static_matrix<C, R, T>&
    fill_column (size_type idx, value_type value) noexcept
    {
        if (idx >= _M_uCols) idx = max_col ();
        for (auto i = 0; i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = value;
        return *this;
    }

    inline
    static_matrix<C, R, T>&
    set_row (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= _M_uRows) idx = max_row ();
        for (auto i = 0; i < uSize and i < cols (); ++i)
            _M_matrix[col_mul (idx) + i] = *values[i];
        return *this;
    }

    inline
    static_matrix<C, R, T>&
    fill_row (size_type idx, value_type value) noexcept
    {
        if (idx >= _M_uRows) idx = max_row ();
        for (auto i = 0; i < cols (); ++i)
            _M_matrix[col_mul (idx) + i] = value;
        return *this;
    }

    virtual ~static_matrix ()
    { }

    constexpr static_matrix (static_matrix<C, R, T> const& gObj) noexcept
    : _M_uCols  (gObj._M_uCols ),
      _M_uRows  (gObj._M_uRows ),
      _M_matrix (gObj._M_matrix)
    { }

    constexpr static_matrix () noexcept
    : _M_uCols  (C),
      _M_uRows  (R),
      _M_matrix ()
    { }

    template <typename... Ts>
    constexpr static_matrix (Ts... vals) noexcept
    : _M_uCols  (C),
      _M_uRows  (R),
      _M_matrix { vals... }
    { static_assert (sizeof... (vals) == size (), "Wrong number of initializers!"); }

    constexpr value_type& operator [] (size_type uIdx) const noexcept
    { return _M_matrix[uIdx]; }

    value_type& operator [] (size_type uIdx) noexcept
    { return _M_matrix[uIdx]; }

    static constexpr size_type size () noexcept
    { return C * R; }

    constexpr void swap_coord () noexcept
    { if (C != R) std::swap (_M_uCols, _M_uRows); }

    const_pointer data () const noexcept
    { return _M_matrix.data (); }

    static_matrix<C, R, T>& operator  = (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator *= (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator *  (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator += (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator +  (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator -= (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator -  (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator != (static_matrix<C, R, T> const&);
    static_matrix<C, R, T>& operator == (static_matrix<C, R, T> const&);

private:
    size_type  _M_uCols, _M_uRows;
    array_type _M_matrix;
};

// =========================================================

class matrix2 : public static_matrix <2, 2, float>
{
public:
    matrix2& invert    ();
    matrix2& transpose () noexcept;
};

// =========================================================

class matrix3 : public static_matrix <3, 3, float>
{
public:
    matrix3& invert    ();
    matrix3& transpose () noexcept;
};

// =========================================================

class matrix4 : public static_matrix <4, 4, float>
{
public:
    using static_matrix<4, 4, float>::static_matrix;

    matrix4& invert    ();
    matrix4& transpose () noexcept;
    matrix4& rotate    (anglef, point3f);
    matrix4& rotate_x  (anglef);
    matrix4& rotate_y  (anglef);
    matrix4& rotate_z  (anglef);
    matrix4& translate (point3f) noexcept;
    matrix4& scale     (value_type);
    matrix4& scale     (point3f);

    const_pointer transposed () const noexcept
    { return &_M_fTranspose[0]; }

private:
    value_type _M_fTranspose[size ()];
};

// =========================================================

class view_matrix : public dynamic_matrix<rgb_color, memory::allocator<rgb_color>>
{
public:
    enum class Type : byte
    {
        Perspective,
        Orthographic
    };
};

} } // namespace Graphics

#endif // __cplusplus
#endif // CPPUAL_GFX_MATRIX_H_
