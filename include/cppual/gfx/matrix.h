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

#ifndef CPPUAL_GFX_MATRIX_H_
#define CPPUAL_GFX_MATRIX_H_
#ifdef __cplusplus


#include <cppual/memory_allocator>
#include <cppual/gfx/coord.h>
#include <cppual/gfx/color.h>
#include <cppual/containers>

// =========================================================

namespace cppual::gfx {

// =========================================================

typedef enum class matrix_type : byte
{
    projection,
    model_transform,
    texture,
    color
}
const const_matrix;

// =========================================================

typedef enum class matrix_view_type : byte
{
    perspective,
    orthographic
}
const const_matrix_view;

// =========================================================

template <non_void T, typename A = memory::allocator<T>>
class dynamic_matrix
{
public:
    typedef dynamic_matrix<T, A>         self_type      ;
    typedef T                            value_type     ;
    typedef T const                      const_value    ;
    typedef T *                          pointer        ;
    typedef T const*                     const_pointer  ;
    typedef T &                          reference      ;
    typedef T const&                     const_reference;
    typedef memory::allocator_traits<A>  alloc_traits   ;
    typedef alloc_traits::allocator_type allocator_type ;
    typedef allocator_type::size_type    size_type      ;
    typedef size_type const              const_size     ;
    typedef deque<T>                     array_type     ;

    constexpr dynamic_matrix () noexcept = default;

    constexpr dynamic_matrix (self_type&& gObj) noexcept
    : _M_matrix (std::move (gObj._M_matrix))
    , _M_uCols  (std::move (gObj._M_uCols ))
    , _M_uRows  (std::move (gObj._M_uRows ))
    { }

    constexpr dynamic_matrix (self_type const& gObj) noexcept
    : _M_matrix (gObj._M_matrix)
    , _M_uCols  (gObj._M_uCols )
    , _M_uRows  (gObj._M_uRows )
    { }

    template <non_void... Ts> requires (std::same_as<value_type, Ts> && ...)
    constexpr dynamic_matrix (Ts... vals) noexcept
    : _M_matrix { vals... }
    { static_assert (sizeof... (vals) == size (), "Wrong number of initializers!"); }

    constexpr const_reference operator [] (size_type uIdx) const noexcept
    { return _M_matrix[uIdx]; }

    constexpr reference operator [] (size_type uIdx) noexcept
    { return _M_matrix[uIdx]; }

    self_type& identity ();

    constexpr size_type size    () const noexcept { return _M_uCols * _M_uRows; }
    constexpr size_type cols    () const noexcept { return _M_uCols           ; }
    constexpr size_type rows    () const noexcept { return _M_uRows           ; }
    constexpr size_type max_row ()       noexcept { return _M_uRows - 1       ; }
    constexpr size_type max_col ()       noexcept { return _M_uCols - 1       ; }

    constexpr void swap_coord () noexcept
    { if (_M_uCols != _M_uRows) std::swap (_M_uCols, _M_uRows); }

    constexpr const_pointer data () const noexcept
    { return _M_matrix.data (); }

    constexpr
    self_type&
    set_column (size_type idx, const_pointer values, size_type uSize) noexcept
    {
        if  (idx >= _M_uCols) idx = max_col ();
        for (size_type i = 0; i < uSize && i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = *values[i];
        return *this;
    }

    constexpr
    self_type&
    fill_column (size_type idx, value_type value) noexcept
    {
        if  (idx >= _M_uCols) idx = max_col ();
        for (size_type i = 0; i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = value;
        return *this;
    }

    constexpr
    self_type&
    set_row (size_type idx, const_pointer values, size_type uSize) noexcept
    {
        if  (idx >= _M_uRows) idx = max_row ();
        for (size_type i = 0; i < uSize && i < cols (); ++i)
            _M_matrix[(_M_uCols * idx) + i] = *values[i];
        return *this;
    }

    constexpr
    self_type&
    fill_row (size_type idx, value_type value) noexcept
    {
        if  (idx >= _M_uRows) idx = max_row ();
        for (size_type i = 0; i < cols (); ++i)
            _M_matrix[(_M_uCols * idx) + i] = value;
        return *this;
    }

private:
    array_type _M_matrix { };
    size_type  _M_uCols  { };
    size_type  _M_uRows  { };
};

// =========================================================

template <u16 C, u16 R, arithmetic T = float>
class static_matrix
{
public:
    typedef static_matrix<C, R, T> self_type      ;
    typedef T                      value_type     ;
    typedef value_type const       const_value    ;
    typedef T *                    pointer        ;
    typedef T const*               const_pointer  ;
    typedef T &                    reference      ;
    typedef T const&               const_reference;
    typedef u16                    size_type      ;
    typedef size_type const        const_size     ;
    typedef std::array<T, C * R>   array_type     ;

    static_assert (1 < C && 1 < R, "Ivalid number of dimensions!");

    consteval static_matrix () noexcept = default;

    constexpr static_matrix (self_type const& gObj) noexcept
    : _M_uCols  (gObj._M_uCols )
    , _M_uRows  (gObj._M_uRows )
    , _M_matrix (gObj._M_matrix)
    { }

    template <non_void... Ts> requires (std::same_as<value_type, Ts> && ...)
    constexpr static_matrix (Ts... vals) noexcept
    : self_type ()
    , _M_matrix { vals... }
    { static_assert (sizeof... (vals) == size (), "Wrong number of initializers!"); }

    constexpr const_reference operator [] (size_type uIdx) const noexcept
    { return _M_matrix[uIdx]; }

    constexpr reference operator [] (size_type uIdx) noexcept
    { return _M_matrix[uIdx]; }

    constexpr self_type const& identity () const noexcept
    { return *this; }

    constexpr self_type& identity () noexcept
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

    constexpr
    self_type&
    set_column (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= _M_uCols) idx = max_col ();
        for (auto i = 0; i < uSize and i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = *values[i];
        return *this;
    }

    constexpr
    self_type&
    fill_column (size_type idx, value_type value) noexcept
    {
        if (idx >= _M_uCols) idx = max_col ();
        for (auto i = 0; i < rows (); ++i)
            _M_matrix[(_M_uCols * i) + idx] = value;
        return *this;
    }

    constexpr
    self_type&
    set_row (size_type idx, value_type const* values, size_type uSize) noexcept
    {
        if (idx >= _M_uRows) idx = max_row ();
        for (auto i = 0; i < uSize and i < cols (); ++i)
            _M_matrix[col_mul (idx) + i] = *values[i];
        return *this;
    }

    constexpr
    self_type&
    fill_row (size_type idx, value_type value) noexcept
    {
        if (idx >= _M_uRows) idx = max_row ();
        for (auto i = 0; i < cols (); ++i)
            _M_matrix[col_mul (idx) + i] = value;
        return *this;
    }

    consteval static size_type size () noexcept
    { return C * R; }

    constexpr void swap_coord () noexcept
    { if (C != R) std::swap (_M_uCols, _M_uRows); }

    constexpr const_pointer data () const noexcept
    { return _M_matrix.data (); }

    self_type& operator  = (self_type const&);
    self_type& operator *= (self_type const&);
    self_type& operator *  (self_type const&);
    self_type& operator += (self_type const&);
    self_type& operator +  (self_type const&);
    self_type& operator -= (self_type const&);
    self_type& operator -  (self_type const&);
    self_type& operator != (self_type const&);
    self_type& operator == (self_type const&);

private:
    size_type  _M_uCols  { C };
    size_type  _M_uRows  { R };
    array_type _M_matrix {   };
};

// =========================================================

class matrix2 : public static_matrix<2, 2, float>
{
public:
    typedef matrix2                    self_type;
    typedef static_matrix<2, 2, float> base_type;

    using base_type::base_type;

    self_type& invert    ();
    self_type& transpose () noexcept;
};

// =========================================================

class matrix3 : public static_matrix<3, 3, float>
{
public:
    typedef matrix3                    self_type;
    typedef static_matrix<3, 3, float> base_type;

    using base_type::base_type;

    self_type& invert    ();
    self_type& transpose () noexcept;
};

// =========================================================

class matrix4 : public static_matrix<4, 4, float>
{
public:
    typedef matrix4                    self_type;
    typedef static_matrix<4, 4, float> base_type;

    using base_type::base_type;

    self_type& invert    ();
    self_type& transpose () noexcept;
    self_type& rotate    (anglef, point3f);
    self_type& rotate_x  (anglef);
    self_type& rotate_y  (anglef);
    self_type& rotate_z  (anglef);
    self_type& translate (point3f) noexcept;
    self_type& scale     (value_type);
    self_type& scale     (point3f);

    const_pointer transposed () const noexcept
    { return &_M_fTranspose[0]; }

private:
    value_type _M_fTranspose[size ()];
};

// =========================================================

class view_matrix : public dynamic_matrix<rgb_color, memory::allocator<rgb_color>>
{
public:
    typedef view_matrix                                             self_type;
    typedef dynamic_matrix<rgb_color, memory::allocator<rgb_color>> base_type;

    using base_type::base_type;
};

// =========================================================

} // namespace gfx

// =========================================================

#endif // __cplusplus
#endif // CPPUAL_GFX_MATRIX_H_
