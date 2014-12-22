/*
 * Product: C++ Unified Abstraction Library
 * Author: Kurec
 * Description: This file is a part of CPPUAL.
 *
 * Copyright (C) 2012 - 2014 Kurec
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

#ifndef CPPUAL_GFX_SPACE_MATRIX_H_
#define CPPUAL_GFX_SPACE_MATRIX_H_
#ifdef __cplusplus

#include <cppual/gfx/coord.h>
#include <deque>
#include <array>
#include <memory>
//#include <algorithm>

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

template <typename T = float>
class DynamicMatrix
{
public:
	typedef T           value_type;
	typedef T const*    const_pointer;
	typedef deque<T>    array_type;
	typedef std::size_t size_type;

	DynamicMatrix<T>& identity ();

	constexpr size_type size   () const noexcept { return m_uCols * m_uRows; }
	constexpr size_type cols   () const noexcept { return m_uCols; }
	constexpr size_type rows   () const noexcept { return m_uRows; }
	constexpr size_type maxRow () noexcept { return m_uRows - 1; }
	constexpr size_type maxCol () noexcept { return m_uCols - 1; }

	constexpr value_type& operator [] (size_type uIdx) noexcept
	{ return m_matrix[uIdx]; }

//	inline value_type operator [] (size_type uIdx) noexcept
//	{ return m_matrix[uIdx]; }

	constexpr void swapCoord () noexcept
	{ if (m_uCols != m_uRows) std::swap (m_uCols, m_uRows); }

	constexpr const_pointer data () const noexcept
	{ return m_matrix.data (); }

	inline virtual ~DynamicMatrix ()
	{ }

	constexpr DynamicMatrix () noexcept
	: m_matrix (),
	  m_uCols (),
	  m_uRows ()
	{ }

	inline DynamicMatrix (DynamicMatrix&& gObj) noexcept
	: m_matrix (std::move (gObj.m_matrix)),
	  m_uCols (gObj.m_uCols),
	  m_uRows (gObj.m_uRows)
	{ }

	inline DynamicMatrix (DynamicMatrix const& gObj) noexcept
	: m_matrix (gObj.m_matrix),
	  m_uCols (gObj.m_uCols),
	  m_uRows (gObj.m_uRows)
	{ }

	template <typename... Ts>
	inline DynamicMatrix (Ts... vals) noexcept
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

	typedef T               value_type;
	typedef T const*        const_pointer;
	typedef array<T, C * R> array_type;
	typedef u16             size_type;

	StaticMatrix<C, R, T>& identity () { return *this; }

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

	inline virtual ~StaticMatrix ()
	{ }

	constexpr StaticMatrix (StaticMatrix<C, R, T> const& gObj) noexcept
	: m_matrix (gObj.m_matrix),
	  m_uCols (gObj.m_uCols),
	  m_uRows (gObj.m_uRows)
	{ }

	constexpr StaticMatrix () noexcept
	: m_matrix (),
	  m_uCols (C),
	  m_uRows (R)
	{ }

	template <typename... Ts>
	constexpr StaticMatrix (Ts... vals) noexcept
	: m_matrix { vals... },
	  m_uCols (C),
	  m_uRows (R)
	{ static_assert (sizeof... (vals) == size (), "Wrong number of initializers!"); }

	constexpr value_type& operator [] (size_type uIdx) noexcept
	{ return m_matrix[uIdx]; }

//	inline value_type operator [] (size_type uIdx) noexcept
//	{ return m_matrix[uIdx]; }

	static constexpr size_type size () noexcept
	{ return C * R; }

	constexpr void swapCoord () noexcept
	{ if (C != R) std::swap (m_uCols, m_uRows); }

	inline const_pointer data () const noexcept
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

protected:
	array_type m_matrix;

private:
	size_type m_uCols, m_uRows;
};

// =========================================================

template <u16 N>
class UniformMatrix : public StaticMatrix <N, N, float>
{
	static_assert (N <= 4, "Ivalid uniform matrix!");
};

// =========================================================

template <>
class UniformMatrix <2> : public StaticMatrix <2, 2, float>
{
public:
	UniformMatrix<2>& invert ();
	UniformMatrix<2>& transpose () noexcept;
};

// =========================================================

template <>
class UniformMatrix <3> : public StaticMatrix <3, 3, float>
{
public:
	UniformMatrix<3>& invert ();
	UniformMatrix<3>& transpose () noexcept;
};

// =========================================================

template <>
class UniformMatrix <4> : public StaticMatrix <4, 4, float>
{
public:
	using StaticMatrix<4, 4, float>::StaticMatrix;

	UniformMatrix<4>& invert ();
	UniformMatrix<4>& transpose () noexcept;
	UniformMatrix<4>& rotate (anglef, point3f);
	UniformMatrix<4>& rotateX (anglef);
	UniformMatrix<4>& rotateY (anglef);
	UniformMatrix<4>& rotateZ (anglef);
	UniformMatrix<4>& translate (point3f) noexcept;
	UniformMatrix<4>& scale (value_type);
	UniformMatrix<4>& scale (point3f);

	inline cfloat* transposed () const noexcept
	{ return &m_fTranspose[0]; }

private:
	value_type m_fTranspose[size ()];
};

// =========================================================

class ViewMatrix : public UniformMatrix<4>
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
#endif // CPPUAL_GFX_SPACE_MATRIX_H_
